// This file is part of lume, a C++ library for lightweight unstructured meshes
//
// Copyright (C) 2018 Sebastian Reiter
// Copyright (C) 2018 G-CSC, Goethe University Frankfurt
// Author: Sebastian Reiter <s.b.reiter@gmail.com>
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

#include "lume/array_annex.h"
#include "lume/normals.h"
#include "lume/mesh.h"
#include "lume/math/geometry.h"
#include "lume/math/tuple_view.h"

namespace lume {

void ComputeFaceVertexNormals3 (Mesh& mesh,
                                const RealArrayAnnex& coordsAnnex,
                                RealArrayAnnex& normalAnnex)
{
    if (!mesh.has_annex (keys::vertexCoords))
        return;

    auto const coords = math::TupleView (coordsAnnex);
    auto normals      = math::TupleView (normalAnnex);

    if (coords.tuple_size() != 3)
        throw BadTupleSizeError () << coords.tuple_size();
    
    if (normals.tuple_size() != 3)
        throw BadTupleSizeError () << normals.tuple_size();

    if (normals.size () != coords.size ())
        throw AnnexError () << "Provided coordinate and normal annexes have different size.";

    math::VecSet (normalAnnex, 0);

    for (auto const grob : mesh.grobs (TRI))
    {
        std::array <real_t, 3> n;
        math::TriangleNormal3 (n, coords [grob.corner (0)], coords [grob.corner (1)], coords [grob.corner (2)]);

        for(int i = 0; i < 3; ++i) {
            normals [grob.corner (i)] += n;
        }
    }

    math::VecTupNormalizeInplace (normalAnnex);
}

void ComputeFaceVertexNormals3 (Mesh& mesh,
                                const RealArrayAnnex& coordsAnnex,
						        const TypedAnnexKey <RealArrayAnnex> vertexNormalsKey)
{
    if (!mesh.has_annex (vertexNormalsKey))
        mesh.set_annex (vertexNormalsKey, RealArrayAnnex {3});
    ComputeFaceVertexNormals3 (mesh, coordsAnnex, mesh.annex (vertexNormalsKey));
}

void ComputeFaceVertexNormals3 (Mesh& mesh)
{
    if (!mesh.has_annex (keys::vertexCoords))
        throw NoSuchAnnexError () << keys::vertexCoords.name ();

    ComputeFaceVertexNormals3 (mesh, mesh.annex (keys::vertexCoords), mesh.annex (keys::vertexNormals));
}

}// end of namespace lume
