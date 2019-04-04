// This file is part of lumeview, a lightweight viewer for unstructured meshes
//
// Copyright (C) 2018, 2019 Sebastian Reiter
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

#pragma once

#include <glm/detail/type_vec3.hpp>

namespace lumeview::util {

template <class real_t>
struct Box {
    Box ();
    Box (real_t min, real_t max);
    Box (const glm::tvec3<real_t>& min, const glm::tvec3<real_t>& max);
    Box (glm::tvec3<real_t>&& min, glm::tvec3<real_t>&& max);
    Box (const Box& box);
    Box (Box&& box);

    Box& operator = (const Box& box);

    glm::tvec3<real_t>  minCorner;
    glm::tvec3<real_t>  maxCorner;
};

using FBox = Box <float>;
using DBox = Box <double>;


template <class real_t>
struct Sphere {
    Sphere ();
    Sphere (const glm::tvec3<real_t>& center, const real_t radius);
    Sphere (glm::tvec3<real_t>&& center, const real_t radius);
    Sphere (const Sphere& sphere);
    Sphere (Sphere&& sphere);

    Sphere& operator = (const Sphere& sphere);

    glm::tvec3<real_t>  center;
    real_t              radius;
};

using FSphere = Sphere <float>;
using DSphere = Sphere <double>;



/// Calcualtes the bounding box to an array of coordinates
/**
 * \param coords    array of all coordinates of the form:
 *                  `x0y0x1y1x2y2x3y3` (size 8, tupleSize 2) or
 *                  `x0y0z0x1y1z1x2y2z2` (size 9, tupleSize 3).
 *
 * \param size      total number of entries in `coords`. size should be a
 *                  multiple of `tupleSize`.
 *
 * \param tupleSize number of floating point values that make up a
 *                  coordinate-tuple.
 */
template <class real_t>
Box <real_t> BoxFromCoords (const real_t* coords, size_t size, size_t tupleSize);

/// Calcualtes a bounding sphere to an array of coordinates
/**
 * \param coords    array of all coordinates of the form:
 *                  `x0y0x1y1x2y2x3y3` (size 8, tupleSize 2) or
 *                  `x0y0z0x1y1z1x2y2z2` (size 9, tupleSize 3).
 *
 * \param size      total number of entries in `coords`. size should be a
 *                  multiple of `tupleSize`.
 *
 * \param tupleSize number of floating point values that make up a
 *                  coordinate-tuple.
 *
 * \note    The computed sphere is not necessarily the minimal sphere containing all points.
 *          However, it is guaranteed that the sphere contains all points and that at least one
 *          point lies on the rim of the sphere.*/
template <class real_t>
Sphere <real_t> SphereFromCoords (const real_t* coords, size_t size, size_t tupleSize);

}// end of namespace lumeview
