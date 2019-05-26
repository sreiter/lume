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

namespace lume
{

SPMesh RefineTriangles (Mesh const & mesh)
{
    size_t const numOldVertices = mesh.num (VERTEX);

    GrobHashMap <index_t> childVertices;
    FindUniqueSidesNumbered (childVertices, mesh, TRIS, 1, numOldVertices);

    size_t const numChildVertices = childVertices.size ();
    size_t const numNewVertices   = numOldVertices + numChildVertices;

    auto newMesh = std::make_shared <Mesh> ();
    newMesh->resize_vertices (numNewVertices);

    auto const& oldCoordsAnnex = mesh.annex (keys::vertexCoords);
    auto const  tupleSize = oldCoordsAnnex.tuple_size ();
    
    RealArrayAnnex newCoordsAnnex (tupleSize, numNewVertices);
    auto newCoords = math::TupleView (newCoordsAnnex);

    math::raw::VecCopy (newCoordsAnnex.data (), oldCoordsAnnex.size (), oldCoordsAnnex.data ());

    for (auto const& entry : childVertices)
    {
        Grob    const& grob  = entry.first;
        index_t const  index = entry.second;
        newCoords [index] = 0.5 * (newCoords [grob.corner (0)] +
                                   newCoords [grob.corner (1)]);
    }

    std::vector <index_t> newTris;
    newTris.reserve (mesh.num_indices (TRIS) * 4);
    for (auto const grob : mesh.grobs (TRI))
    {
        for (index_t i = 0; i < 3; ++i)
        {
            newTris.push_back (grob.corner (i));
            newTris.push_back (childVertices [grob.side (1, i)]);
            newTris.push_back (childVertices [grob.side (1, (i+2) % 3)]);
        }

        newTris.push_back (childVertices [grob.side (1, 0)]);
        newTris.push_back (childVertices [grob.side (1, 1)]);
        newTris.push_back (childVertices [grob.side (1, 2)]);
    }

    newMesh->set_annex (keys::vertexCoords, std::move (newCoordsAnnex));
    newMesh->set_grobs (GrobArray (TRI, std::move (newTris)));
    return newMesh;
}

}// end of namespace lume
