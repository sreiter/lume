// This file is part of lume, a C++ library for lightweight unstructured meshes
//
// Copyright (C) 2019 Sebastian Reiter <s.b.reiter@gmail.com>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <lume/grob_hash.h>
#include <lume/refinement.h>
#include <lume/topology.h>
#include <lume/math/tuple_view.h>
#include <lume/math/grob_math.h>
#include <lume/parallel_for.h>

namespace lume
{

class Genealogy
{
public:
    Genealogy (CSPMesh   parentMesh,
               SPMesh    childMesh)
        : m_parentMesh (std::move (parentMesh))
        , m_childMesh  (std::move (childMesh))
    {}

    Genealogy (Genealogy& other) = delete;

    Genealogy (Genealogy&& other)
        : m_parentMesh (std::move (other.m_parentMesh))
        , m_childMesh  (std::move (other.m_childMesh))
    {
        for(size_t i = 0; i < m_parents.size (); ++i) {
            m_parents [i] = std::move (other.m_parents [i]);
        }
    }

    Mesh const& parent_mesh () const {return *m_parentMesh;}
    Mesh&       child_mesh  ()       {return *m_childMesh;}

    /** Returns an array of grobs referring to the parent mesh. Each such grob
        corresponds to the child at the same index of type childType in the child mesh.
        The returned array has the same size and order as `child_mesh ().grob_array (childType)`.*/
    std::vector <Grob> const& parents (GrobType childType)
    {
        assert ((m_childMesh == nullptr &&
                 m_parents [childType].empty ()) ||
                (m_childMesh != nullptr &&
                 m_parents [childType].size () == m_childMesh->num (childType)));
        return m_parents [childType];
    }

    void add_parent (GrobType const childType, const Grob& parentGrob)
    {
        m_parents [childType].push_back (parentGrob);
    }

    void add_parent (GrobType const childType, const Grob& parentGrob, size_t const index)
    {
        auto& parents = m_parents [childType];
        if (index >= parents.size ()) {
            parents.resize (index + 1, Grob (VERTEX, nullptr));
        }

        parents [index] = parentGrob;
    }

    template <class Container>
    void add_parents (GrobType const childType, Container const& newParents)
    {
        auto& parents = m_parents [childType];
        for (auto const& parent : newParents)
            parents.push_back (parent);
    }

private:
    CSPMesh   m_parentMesh;
    SPMesh    m_childMesh;
    std::array <std::vector <Grob>, NUM_GROB_TYPES> m_parents;
};


void RefinementCallback (Genealogy genealogy)
{
    Mesh const& parentMesh = genealogy.parent_mesh ();
    Mesh&       childMesh  = genealogy.child_mesh ();

    auto const& parentCoords = math::TupleView (parentMesh.annex (keys::vertexCoords));
    auto const  tupleSize = parentCoords.tuple_size ();

    RealArrayAnnex childCoordsAnnex (tupleSize, childMesh.num (VERTEX));
    auto childCoords = math::TupleView (childCoordsAnnex);

    auto const& parents = genealogy.parents (VERTEX);
    assert (childCoords.size () == parents.size ());

    size_t index = 0;
    for (auto const& parentGrob : parents)
    {
        childCoords [index++] = math::GrobCenter (parentGrob, parentCoords);
    }

    childMesh.set_annex (keys::vertexCoords, std::move (childCoordsAnnex));
}

std::vector <index_t> CreateTriangles (Mesh const& parentMesh,
                                       GrobHashMap <index_t> const& parentEdges)
{
    std::vector <index_t> newTris;
    newTris.resize (parentMesh.num_indices (TRIS) * 4);
    
    auto const& grobs = parentMesh.grobs (TRI);

    lume::parallel_for (0, grobs.size (), [&grobs, &parentEdges, &newTris] (size_t grobIndex)
    {
        auto const& grob = grobs [grobIndex];
        std::array <index_t, 3> parentEdgeIndices;
        for(index_t i = 0; i < 3; ++i) {
            parentEdgeIndices [i] = parentEdges.at (grob.side (1, i));
        }

        size_t ito = grobIndex * 12;
        for (index_t i = 0; i < 3; ++i)
        {
            newTris [ito++] = grob.corner (i);
            newTris [ito++] = parentEdgeIndices [i];
            newTris [ito++] = parentEdgeIndices [(i+2) % 3];
        }

        newTris [ito++] = parentEdgeIndices [0];
        newTris [ito++] = parentEdgeIndices [1];
        newTris [ito++] = parentEdgeIndices [2];
    });

    return newTris;
}

SPMesh RefineTriangles (CSPMesh meshIn)
{
    if (meshIn == nullptr) {
        return {};
    }

    Mesh const& parentMesh = *meshIn;
    index_t const numOldVertices = static_cast <index_t> (parentMesh.num (VERTEX));

    GrobHashMap <index_t> parentEdges;
    FindUniqueSidesNumbered (parentEdges, parentMesh, TRIS, 1, numOldVertices);

    index_t const numParentEdges = static_cast <index_t> (parentEdges.size ());
    index_t const numNewVertices   = numOldVertices + numParentEdges;

    auto childMesh = std::make_shared <Mesh> ();
    Genealogy genealogy (meshIn, childMesh);

    childMesh->resize_vertices (numNewVertices);

    genealogy.add_parents (VERTEX, parentMesh.grobs (VERTEX));

    for (auto const& entry : parentEdges) {
        genealogy.add_parent (VERTEX, entry.first, entry.second);
    }

    std::vector <index_t> newTris = CreateTriangles (parentMesh, parentEdges);

    childMesh->set_grobs (GrobArray (TRI, std::move (newTris)));

    for (auto const grob : parentMesh.grobs (TRI))
    {
        for(int i = 0; i < 4; ++i) {
            genealogy.add_parent (TRI, grob);
        }
    }

    RefinementCallback (std::move (genealogy));

    return childMesh;
}

}// end of namespace lume
