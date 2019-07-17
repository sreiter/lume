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


#ifndef __H__lume__normals
#define __H__lume__normals

#include <memory>
#include "lume/grob.h"
#include "lume/mesh.h"
#include "lume/types.h"
#include "lume/array_annex.h"
#include "lume/math/tuple_view.h"

namespace lume {

/**	Computes the 3 dimensional vertex normals of a mesh by averaging the normals of associated faces
    and stores them in an annex of the mesh's vertices.

    If not provided, the key `lume::keys::vertexCoords` is used to access vertex coordinates and
    `lume::keys::vertexNormals` is used to access vertex normals. The latter annex is created if
    not present during call-time. Both have to have a tuple size of 3.
    \{
*/

void ComputeFaceVertexNormals3 (Mesh& mesh);

void ComputeFaceVertexNormals3 (Mesh& mesh,
                                math::ConstTupleView <real_t> coords,
                                math::TupleView <real_t> normals);
/** \} */

}// end of namespace lume

#endif	//__H__lume__normals
