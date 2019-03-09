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

#ifndef __H__lume_grob_set_types
#define __H__lume_grob_set_types

#include <string>
#include "grob_types.h"

namespace lume {

enum GrobSetType {
    VERTICES    = VERTEX,
    EDGES       = EDGE,
    TRIS        = TRI,
    QUADS       = QUAD,
    TETS        = TET,
    HEXS        = HEX,
    PYRAS       = PYRA,
    PRISMS      = PRISM,

    NO_GROB_SET,
    FACES,
    CELLS
};

/// returns the name of a grob set
const std::string& GrobSetTypeName (GrobSetType grobSet);

/// returns the largest constant from grob_set_t for a given dimension
/** If no grob set for the given dimension exists, `NO_GROB_SET` is returned.
 * \returns VERTICES (dim==0), EDGES (dim==1), FACES (dim==2), CELLS (dim==3), NO_GROB_SET (else)*/
GrobSetType GrobSetTypeByDim (index_t dim);

}//    end of namespace lume

#endif    //__H__lume_grob_set_types
