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

#include "lume/grob_set.h"

namespace {

using namespace lume;

const index_t g_verticesDesc [] = {
    VERTICES,   // TYPE
    0,          // DIM
    1,          // SIZE
    VERTEX      // ENTRY 1
};

const index_t g_edgesDesc [] = {
    EDGES,      // TYPE
    1,          // DIM
    1,          // SIZE
    EDGE,       // ENTRY 1
    VERTICES    // 0D side list
};

const index_t g_trisDesc [] = {
    TRIS,       // TYPE
    2,          // DIM
    1,          // SIZE
    TRI,        // ENTRY 1
    VERTICES,   // 0D side list
    EDGES       // 1D side list
};

const index_t g_quadsDesc [] = {
    QUADS,      // TYPE
    2,          // DIM
    1,          // SIZE
    QUAD,       // ENTRY 1
    VERTICES,   // 0D side list
    EDGES       // 1D side list 
};

const index_t g_tetsDesc [] = {
    TETS,       // TYPE
    3,          // DIM
    1,          // SIZE
    TET,        // ENTRY 1
    VERTICES,   // 0D side list
    EDGES,      // 1D side list
    TRIS        // 2D side list
};

const index_t g_hexsDesc [] = {
    HEXS,       // TYPE
    3,          // DIM
    1,          // SIZE
    HEX,        // ENTRY 1
    VERTICES,   // 0D side list
    EDGES,      // 1D side list
    QUADS,      // 2D side list
};

const index_t g_pyrasDesc [] = {
    PYRAS,      // TYPE
    3,          // DIM
    1,          // SIZE
    PYRA,       // ENTRY 1
    VERTICES,   // 0D side list
    EDGES,      // 1D side list
    FACES       // 2D side list
};

const index_t g_prismsDesc [] = {
    PRISMS,     // TYPE
    3,          // DIM
    1,          // SIZE
    PRISM,      // ENTRY 1
    VERTICES,   // 0D side list
    EDGES,      // 1D side list
    FACES       // 2D side list
};

const index_t g_noGrobSetDesc [] = {
    NO_GROB_SET,    // TYPE
    0,                  // DIM
    0                   // SIZE
};

const index_t g_facesDesc [] = {
    FACES,      // TYPE
    2,          // DIM
    2,          // SIZE
    TRI,        // ENTRY 1
    QUAD,       // ENTRY 1
    VERTICES,   // 0D side list
    EDGES,      // 1D side list
};

const index_t g_cellsDesc [] = {
    CELLS,      // TYPE
    3,          // DIM
    4,          // SIZE
    TET,        // ENTRY 1
    HEX,        // ENTRY 2
    PYRA,       // ENTRY 3
    PRISM,      // ENTRY 4
    VERTICES,   // 0D side list
    EDGES,      // 1D side list
    FACES       // 2D side list
};

const index_t* g_rawGrobSetDescPointers [] = {
    g_verticesDesc,
    g_edgesDesc,
    g_trisDesc,
    g_quadsDesc,
    g_tetsDesc,
    g_hexsDesc,
    g_pyrasDesc,
    g_prismsDesc,
    g_noGrobSetDesc,
    g_facesDesc,
    g_cellsDesc
};

}// end of empty namespace

namespace lume {

GrobSet::GrobSet ()
    : m_rawDesc (g_rawGrobSetDescPointers [NO_GROB_SET])
{}

GrobSet::GrobSet (const GrobSetType grobSet)
    : m_rawDesc (g_rawGrobSetDescPointers [grobSet])
{}

GrobSet::GrobSet (const GrobType grobType)
    : m_rawDesc (g_rawGrobSetDescPointers [grobType])
{}

}//    end of namespace lume
