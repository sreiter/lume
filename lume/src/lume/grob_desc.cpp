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

#include "lume/grob_desc.h"

namespace {

using namespace lume;

const index_t g_vertexDesc [] = {
    VERTEX,        // TYPE
    0,             // DIM
    VERTICES,   // grob_set_t of 0D sides
    1,      // Offset to num 0D sides entry counting from this entry
    1,      // num 0D sides (CORNERS)
    1,      // Offset to first 0D side counting from the this entry
    VERTEX, 0
};

const index_t g_edgeDesc [] = {
    EDGE,   // TYPE
    1,      // DIM
    VERTICES,   // grob_set_t of 0D sides
    1,      // Offset to num 0D sides entry counting from this entry
    2,      // num 0D sides (CORNERS)
    2,      // Offset to first 0D side counting from the this entry
    3,      // Offset to second 0D side counting from the this entry
    VERTEX, 0,
    VERTEX, 1
};

const index_t g_triDesc [] = {
    TRI,    // TYPE
    2,      // DIM
    VERTICES,   // grob_set_t of 0D sides
    EDGES,      // grob_set_t of 1D sides
    2,      // Offset to num 0D sides entry counting from this entry
    11,     // Offset to num 1D sides entry counting from this entry
    3,      // num 0D sides (CORNERS)
    3,      // offset to first 0D side counting from this entry
    4,      // offset to second 0D side counting from this entry
    5,      // offset to third 0D side counting from this entry
    VERTEX, 0,  // 0D side 1 (type, corners)
    VERTEX, 1,  // 0D side 2 (type, corners)
    VERTEX, 2,  // 0D side 3 (type, corners)
    3,      // num 1D sides
    3,      // offset to first 1D side counting from this entry
    5,      // offset to second 1D side counting from this entry
    7,      // offset to third 1D side counting from this entry
    EDGE, 0, 1, // 1D side 1 (type, corners)
    EDGE, 1, 2, // 1D side 2 (type, corners)
    EDGE, 2, 0  // 1D side 3 (type, corners)
};

const index_t g_quadDesc [] = {
    QUAD,   // TYPE
    2,      // DIM
    VERTICES,   // grob_set_t of 0D sides
    EDGES,      // grob_set_t of 1D sides
    2,      // Offset to num 0D sides entry counting from this entry
    14,     // Offset to num 1D sides entry counting from this entry
    4,      // num 0D sides (CORNERS)
    4,      // offset to first 0D side counting from this entry
    5,      // offset to second 0D side counting from this entry
    6,      // offset to third 0D side counting from this entry
    7,      // offset to fourth 0D side counting from this entry
    VERTEX, 0,  // 0D side 1 (type, corners)
    VERTEX, 1,  // 0D side 2 (type, corners)
    VERTEX, 2,  // 0D side 3 (type, corners)
    VERTEX, 3,  // 0D side 4 (type, corners)
    4,      // num 1D sides
    4,      // offset to first 1D side counting from this entry
    6,      // offset to second 1D side counting from this entry
    8,      // offset to third 1D side counting from this entry
    10,     // offset to fourth 1D side counting from this entry
    EDGE, 0, 1, // 1D side 1 (type, corners)
    EDGE, 1, 2, // 1D side 2 (type, corners)
    EDGE, 2, 3, // 1D side 3 (type, corners)
    EDGE, 3, 0  // 1D side 4 (type, corners)
};

const index_t g_tetDesc [] = {
    TET,    // TYPE
    3,      // DIM
    VERTICES,   // grob_set_t of 0D sides
    EDGES,      // grob_set_t of 1D sides
    TRIS,       // grob_set_t of 2D sides
    3,      // Offset to num 0D sides entry counting from this entry
    15,     // Offset to num 1D sides entry counting from this entry
    39,     // Offset to num 2D sides entry counting from this entry
    4,      // num 0D sides (CORNERS)
    4,      // offset to first 0D side counting from this entry
    5,      // offset to second 0D side counting from this entry
    6,      // offset to third 0D side counting from this entry
    7,      // offset to fourth 0D side counting from this entry
    VERTEX, 0,  // 0D side 1 (type, corners)
    VERTEX, 1,  // 0D side 2 (type, corners)
    VERTEX, 2,  // 0D side 3 (type, corners)
    VERTEX, 3,  // 0D side 4 (type, corners)
    6,      // num 1D sides
    6,      // offset to first 1D side counting from this entry
    8,      // offset to second 1D side counting from this entry
    10,     // offset to third 1D side counting from this entry
    12,     // offset to fourth 1D side counting from this entry
    14,     // offset to fifth 1D side counting from this entry
    16,     // offset to sixth 1D side counting from this entry
    EDGE, 0, 1, // 1D side 1 (type, corners)
    EDGE, 1, 2, // 1D side 2 (type, corners)
    EDGE, 2, 0, // 1D side 3 (type, corners)
    EDGE, 0, 3, // 1D side 4 (type, corners)
    EDGE, 1, 3, // 1D side 5 (type, corners)
    EDGE, 2, 3, // 1D side 6 (type, corners)
    4,      // num 2d sides
    4,      // offset to first 2D side counting from this entry
    7,      // offset to second 2D side counting from this entry
    10,     // offset to third 2D side counting from this entry
    13,     // offset to fourth 2D side counting from this entry
    TRI, 0, 2, 1,   // 2D side 1 (type, corners)
    TRI, 0, 1, 3,   // 2D side 2 (type, corners)
    TRI, 1, 2, 3,   // 2D side 3 (type, corners)
    TRI, 2, 0, 3    // 2D side 4 (type, corners)
};

const index_t g_hexDesc [] = {
    HEX,    // TYPE
    3,      // DIM
    VERTICES,   // grob_set_t of 0D sides
    EDGES,      // grob_set_t of 1D sides
    QUADS,      // grob_set_t of 2D sides
    3,      // Offset to num 0D sides entry counting from this entry
    27,     // Offset to num 1D sides entry counting from this entry
    75,     // Offset to num 2D sides entry counting from this entry
    8,      // num 0D sides (CORNERS)
    8,      // offset to first 0D side counting from this entry
    9,      // offset to second 0D side counting from this entry
    10,     // offset to third 0D side counting from this entry
    11,     // offset to fourth 0D side counting from this entry
    12,     // offset to fifth 0D side counting from this entry
    13,     // offset to sixth 0D side counting from this entry
    14,     // offset to seventh 0D side counting from this entry
    15,     // offset to eighth 0D side counting from this entry
    VERTEX, 0,  // 0D side 1 (type, corners)
    VERTEX, 1,  // 0D side 2 (type, corners)
    VERTEX, 2,  // 0D side 3 (type, corners)
    VERTEX, 3,  // 0D side 4 (type, corners)
    VERTEX, 4,  // 0D side 5 (type, corners)
    VERTEX, 5,  // 0D side 6 (type, corners)
    VERTEX, 6,  // 0D side 7 (type, corners)
    VERTEX, 7,  // 0D side 8 (type, corners)
    12,     // num 1D sides
    12,     // offset to first 1D side counting from this entry
    14,     // offset to second 1D side counting from this entry
    16,     // offset to third 1D side counting from this entry
    18,     // offset to fourth 1D side counting from this entry
    20,     // offset to fifth 1D side counting from this entry
    22,     // offset to sixth 1D side counting from this entry
    24,     // offset to seventh 1D side counting from this entry
    26,     // offset to eighth 1D side counting from this entry
    28,     // offset to ninth 1D side counting from this entry
    30,     // offset to tenth 1D side counting from this entry
    32,     // offset to eleventh 1D side counting from this entry
    34,     // offset to twelfth 1D side counting from this entry
    EDGE, 0, 1, // 1D side 1 (type, corners)
    EDGE, 1, 2, // 1D side 2 (type, corners)
    EDGE, 2, 3, // 1D side 3 (type, corners)
    EDGE, 3, 0, // 1D side 4 (type, corners)
    EDGE, 0, 4, // 1D side 5 (type, corners)
    EDGE, 1, 5, // 1D side 6 (type, corners)
    EDGE, 2, 6, // 1D side 7 (type, corners)
    EDGE, 3, 7, // 1D side 8 (type, corners)
    EDGE, 4, 5, // 1D side 1 (type, corners)
    EDGE, 5, 6, // 1D side 2 (type, corners)
    EDGE, 6, 7, // 1D side 3 (type, corners)
    EDGE, 7, 4, // 1D side 4 (type, corners)
    6,      // num 2d sides
    6,      // offset to first 2D side counting from this entry
    10,     // offset to second 2D side counting from this entry
    14,     // offset to third 2D side counting from this entry
    18,     // offset to fourth 2D side counting from this entry
    22,     // offset to fifth 2D side counting from this entry
    26,     // offset to sicth 2D side counting from this entry
    QUAD, 0, 3, 2, 1,   // 2D side 1 (type, corners)
    QUAD, 0, 1, 5, 4,   // 2D side 2 (type, corners)
    QUAD, 1, 2, 6, 5,   // 2D side 3 (type, corners)
    QUAD, 2, 3, 7, 6,   // 2D side 4 (type, corners)
    QUAD, 3, 0, 4, 7,   // 2D side 4 (type, corners)
    QUAD, 4, 5, 6, 7    // 2D side 3 (type, corners)
};

const index_t g_pyraDesc [] = {
    PYRA,   // TYPE
    3,      // DIM
    VERTICES,   // grob_set_t of 0D sides
    EDGES,      // grob_set_t of 1D sides
    FACES,      // grob_set_t of 2D sides
    3,      // Offset to num 0D sides entry counting from this entry
    18,     // Offset to num 1D sides entry counting from this entry
    50,     // Offset to num 2D sides entry counting from this entry
    5,      // num 0D sides (CORNERS)
    5,      // offset to first 0D side counting from this entry
    6,      // offset to second 0D side counting from this entry
    7,      // offset to third 0D side counting from this entry
    8,      // offset to fourth 0D side counting from this entry
    9,      // offset to fifth 0D side counting from this entry
    VERTEX, 0,  // 0D side 1 (type, corners)
    VERTEX, 1,  // 0D side 2 (type, corners)
    VERTEX, 2,  // 0D side 3 (type, corners)
    VERTEX, 3,  // 0D side 4 (type, corners)
    VERTEX, 4,  // 0D side 4 (type, corners)
    8,      // num 1D sides
    8,      // offset to first 1D side counting from this entry
    10,     // offset to second 1D side counting from this entry
    12,     // offset to third 1D side counting from this entry
    14,     // offset to fourth 1D side counting from this entry
    16,     // offset to fifth 1D side counting from this entry
    18,     // offset to sixth 1D side counting from this entry
    20,     // offset to seventh 1D side counting from this entry
    22,     // offset to eighth 1D side counting from this entry
    EDGE, 0, 1, // 1D side 1 (type, corners)
    EDGE, 1, 2, // 1D side 2 (type, corners)
    EDGE, 2, 3, // 1D side 3 (type, corners)
    EDGE, 3, 0, // 1D side 4 (type, corners)
    EDGE, 0, 4, // 1D side 5 (type, corners)
    EDGE, 1, 4, // 1D side 6 (type, corners)
    EDGE, 2, 4, // 1D side 6 (type, corners)
    EDGE, 3, 4, // 1D side 7 (type, corners)
    5,      // num 2d sides
    5,      // offset to first 2D side counting from this entry
    9,      // offset to second 2D side counting from this entry
    12,     // offset to third 2D side counting from this entry
    15,     // offset to fourth 2D side counting from this entry
    18,     // offset to fifth 2D side counting from this entry
    QUAD, 0, 3, 2, 1,   // 2D side 1 (type, corners)
    TRI, 0, 1, 4,   // 2D side 2 (type, corners)
    TRI, 1, 2, 4,   // 2D side 3 (type, corners)
    TRI, 2, 3, 4,   // 2D side 4 (type, corners)
    TRI, 3, 0, 4    // 2D side 5 (type, corners)
};

const index_t g_prismDesc [] = {
    PRISM,  // TYPE
    3,      // DIM
    VERTICES,   // grob_set_t of 0D sides
    EDGES,      // grob_set_t of 1D sides
    FACES,      // grob_set_t of 2D sides
    3,      // Offset to num 0D sides entry counting from this entry
    21,     // Offset to num 1D sides entry counting from this entry
    57,     // Offset to num 2D sides entry counting from this entry
    6,      // num 0D sides (CORNERS)
    6,      // offset to first 0D side counting from this entry
    7,      // offset to second 0D side counting from this entry
    8,      // offset to third 0D side counting from this entry
    9,      // offset to fourth 0D side counting from this entry
    10,     // offset to fifth 0D side counting from this entry
    11,     // offset to sixth 0D side counting from this entry
    VERTEX, 0,  // 0D side 1 (type, corners)
    VERTEX, 1,  // 0D side 2 (type, corners)
    VERTEX, 2,  // 0D side 3 (type, corners)
    VERTEX, 3,  // 0D side 4 (type, corners)
    VERTEX, 4,  // 0D side 4 (type, corners)
    VERTEX, 5,  // 0D side 4 (type, corners)
    9,      // num 1D sides
    9,      // offset to first 1D side counting from this entry
    11,     // offset to second 1D side counting from this entry
    13,     // offset to third 1D side counting from this entry
    15,     // offset to fourth 1D side counting from this entry
    17,     // offset to fifth 1D side counting from this entry
    19,     // offset to sixth 1D side counting from this entry
    21,     // offset to seventh 1D side counting from this entry
    23,     // offset to eighth 1D side counting from this entry
    25,     // offset to ninth 1D side counting from this entry
    EDGE, 0, 1, // 1D side 1 (type, corners)
    EDGE, 1, 2, // 1D side 2 (type, corners)
    EDGE, 2, 0, // 1D side 3 (type, corners)
    EDGE, 0, 3, // 1D side 4 (type, corners)
    EDGE, 1, 4, // 1D side 5 (type, corners)
    EDGE, 2, 5, // 1D side 6 (type, corners)
    EDGE, 3, 4, // 1D side 6 (type, corners)
    EDGE, 4, 5, // 1D side 7 (type, corners)
    EDGE, 5, 3, // 1D side 8 (type, corners)
    5,      // num 2d sides
    5,      // offset to first 2D side counting from this entry
    8,      // offset to second 2D side counting from this entry
    12,     // offset to third 2D side counting from this entry
    16,     // offset to fourth 2D side counting from this entry
    20,     // offset to fifth 2D side counting from this entry
    TRI, 0, 2, 1,   // 2D side 1 (type, corners)
    QUAD, 0, 1, 4, 3,   // 2D side 2 (type, corners)
    QUAD, 1, 2, 5, 4,   // 2D side 3 (type, corners)
    QUAD, 2, 0, 3, 5,   // 2D side 4 (type, corners)
    TRI, 3, 4, 5        // 2D side 5 (type, corners)
};

/// Holds pointers to each *grid object type* raw descriptor.
/** Please do not work with this array directly, but use the `GrobDesc` class
 * instead.
 */
const index_t* g_rawGrobDescPointers [] = {
    g_vertexDesc,
    g_edgeDesc,
    g_triDesc,
    g_quadDesc,
    g_tetDesc,
    g_hexDesc,
    g_pyraDesc,
    g_prismDesc
};

}// end of empty namespace

namespace lume {

GrobDesc::GrobDesc (GrobType grobType)
    : m_rawDesc (g_rawGrobDescPointers [grobType])
{
}

}//    end of namespace lume