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

#include <lume/refinement.h>

#include <lume/math/tuple_view.h>
#include <lume/math/grob_math.h>
#include <lume/grob_hash.h>
#include <lume/hierarchy.h>
#include <lume/parallel_for.h>
#include <lume/pettyprof.h>
#include <lume/topology.h>

namespace lume
{

void RefinementCallback (Hierarchy hierarchy)
{
    Mesh const& parentMesh = hierarchy.parent_mesh ();
    Mesh&       childMesh  = hierarchy.child_mesh ();

    auto const& parentCoords = math::TupleView (parentMesh.annex (keys::vertexCoords));
    auto const  tupleSize = parentCoords.tuple_size ();

    RealArrayAnnex childCoordsAnnex (tupleSize, childMesh.num (VERTEX));
    auto childCoords = math::TupleView (childCoordsAnnex);

    auto const& relations = hierarchy.relationsForChildType (VERTEX);
    for (auto const& relation : relations)
    {
        for (index_t childIndex : relation) {
            childCoords [childIndex] = math::GrobCenter (relation.parent, parentCoords);
        }
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
    PEPRO_FUNC ();

    PEPRO_BEGIN (RefineTriangles__preparations);
    if (meshIn == nullptr) {
        return {};
    }

    Mesh const& parentMesh = *meshIn;
    index_t const numOldVertices = static_cast <index_t> (parentMesh.num (VERTEX));
    PEPRO_END ();

    PEPRO_BEGIN (FindUniqueSidesNumbered);
    GrobHashMap <index_t> parentEdges;
    parentEdges.reserve (parentMesh.num (EDGE));
    FindUniqueSidesNumbered (parentEdges, parentMesh, TRIS, 1, numOldVertices);
    PEPRO_END ();


    PEPRO_BEGIN (AnalyzingHashMap);
    std::cout << "HashMap stats:" << std::endl;
    std::cout << "  bucket_count:     " << parentEdges.bucket_count () << std::endl;
    size_t minCount = parentMesh.num (EDGE);
    size_t maxCount = 0;
    for(size_t i = 0; i < parentEdges.bucket_count (); ++i) {
        minCount = std::min (minCount, parentEdges.bucket_size (i));
        maxCount = std::max (maxCount, parentEdges.bucket_size (i));
    }

    std::cout << "  min bucket size:  " << minCount << std::endl;
    std::cout << "  max bucket size:  " << maxCount << std::endl;
    PEPRO_END ();

    PEPRO_BEGIN (init_child_mesh);
    index_t const numParentEdges = static_cast <index_t> (parentEdges.size ());
    index_t const numNewVertices   = numOldVertices + numParentEdges;

    auto childMesh = std::make_shared <Mesh> ();
    PEPRO_END ();

    PEPRO_BEGIN (resize_vertices);
    childMesh->resize_vertices (numNewVertices);
    PEPRO_END ();

    PEPRO_BEGIN (prepare_hierarchy);
    Hierarchy hierarchy (meshIn, childMesh);
    hierarchy.reserve (VERTEX, numNewVertices);
    PEPRO_END ();

    PEPRO_BEGIN (build_hierarchy_1);
    auto const& parentVertices = parentMesh.grobs (VERTEX);
    for (size_t i = 0; i < parentVertices.size (); ++i) {
        hierarchy.add_relation (parentVertices [i], VERTEX, static_cast <index_t> (i), 1);
    }
    PEPRO_END ();

    PEPRO_BEGIN (build_hierarchy_2);
    for (auto const& entry : parentEdges) {
        hierarchy.add_relation (entry.first, VERTEX, entry.second, 1);
    }
    PEPRO_END ();

    PEPRO_BEGIN (CreateTriangles);
    std::vector <index_t> newTris = CreateTriangles (parentMesh, parentEdges);
    PEPRO_END ();

    PEPRO_BEGIN (childMesh__set_grobs);
    childMesh->set_grobs (GrobArray (TRI, std::move (newTris)));
    PEPRO_END ();

    PEPRO_BEGIN (build_hierarchy_3);
    auto const& parentTris = parentMesh.grobs (TRI);
    hierarchy.reserve (TRI, parentTris.size ());
    for (size_t i = 0; i < parentTris.size (); ++i) {
        hierarchy.add_relation (parentTris [i], TRI, static_cast <index_t> (i), 4);
    }
    PEPRO_END ();

    
    PEPRO_BEGIN (clearingHash);
    auto cleanUpFuture = std::async (std::launch::async, [&parentEdges] () {GrobHashMap <index_t> hashDeleter (std::move (parentEdges));});

    PEPRO_BEGIN (RefinementCallback);
    RefinementCallback (std::move (hierarchy));
    PEPRO_END ();

    cleanUpFuture.wait ();
    PEPRO_END ();
    return childMesh;
}

}// end of namespace lume
