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
#include "lume/vec_math_raw.h"

namespace lume {

real_t* TriangleNormal3 (real_t* normalOut,
                         const real_t* c0,
                         const real_t* c1,
                         const real_t* c2)
{
	real_t d0[3];
	real_t d1[3];

	VecSub (d0, 3, c1, c0);
	VecSub (d1, 3, c2, c0);

	return VecNormalize (VecCross3 (normalOut, d0, d1), 3);
}

void
ComputeFaceVertexNormals3 (Mesh& mesh,
                           RealArrayAnnex& normalAnnex)
{
    if (!mesh.has_annex (keys::vertexCoords))
        return;

    auto const& coordsAnnex = mesh.annex (keys::vertexCoords);

    if (coordsAnnex.tuple_size() != 3)
        throw BadTupleSizeError (std::to_string (coordsAnnex.tuple_size()));
    
    if (normalAnnex.tuple_size() != 3)
        throw BadTupleSizeError (std::to_string (normalAnnex.tuple_size()));

    if (normalAnnex.size () != coordsAnnex.size ())
        throw AnnexError ("Provided coordinate and normal annexes have different size.");

    VecSet (UNPACK_DS(normalAnnex), 0);

    const real_t*   coords      = coordsAnnex.data();
    real_t*         normals     = normalAnnex.data();
    
    for(auto gt : GrobSet (FACES)) {
        const index_t*  inds        = mesh.grobs (gt).data();
        const size_t   numInds     = mesh.grobs (gt).num_indices();
        const size_t   numCorners  = mesh.grobs (gt).grob_desc ().num_corners ();
        const size_t   offset = numCorners / 2;

        for (size_t i = 0; i < numInds; i += numCorners) {
            const index_t* elem = inds + i;

            real_t d0[3];
            real_t d1[3];

            VecSub (d0, 3, coords + elem[offset] * 3, coords + elem[0] * 3);
            VecSub (d1, 3, coords + elem[1 + offset] * 3, coords + elem[1] * 3);

            real_t n[3];
            VecNormalize (VecCross3 (n, d0, d1), 3);

            for(size_t j = 0; j < numCorners; ++j)
                VecAppend (normals + elem [j] * 3, 3, n);
        }
    }

    VecTupNormalize (UNPACK_DST(normalAnnex));
}

void
ComputeFaceVertexNormals3 (Mesh& mesh,
						   const std::string& normalId)
{
    auto const normalKey = TypedAnnexKey <RealArrayAnnex> (VERTEX, normalId);
    if (!mesh.has_annex (normalKey))
        mesh.set_annex (normalKey, RealArrayAnnex {3});
    ComputeFaceVertexNormals3 (mesh, mesh.annex (normalKey));
}

}// end of namespace lume
