// This file is part of lumeview, a lightweight viewer for unstructured meshes
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

#include <algorithm>
#include <limits>
#include <glm/gtc/type_ptr.hpp>
#include <lume/vec_math_raw.h>
#include <lumeview/lumeview_error.h>
#include <lumeview/util/shapes.h>

using namespace std;
using namespace lume;

namespace lumeview::util
{

template <class real_t>
Box <real_t>::Box ()
{}

template <class real_t>
Box <real_t>::Box (real_t min, real_t max) :
    minCorner (min),
    maxCorner (max)
{}

template <class real_t>
Box <real_t>::Box (const glm::tvec3<real_t>& min, const glm::tvec3<real_t>& max) :
    minCorner (min),
    maxCorner (max)
{}

template <class real_t>
Box <real_t>::Box (glm::tvec3<real_t>&& min, glm::tvec3<real_t>&& max) :
    minCorner (std::move (min)),
    maxCorner (std::move (max))
{}

template <class real_t>
Box <real_t>::Box (const Box& box) :
    minCorner (box.minCorner),
    maxCorner (box.maxCorner)
{}

template <class real_t>
Box <real_t>::Box (Box&& box) :
    minCorner (std::move(box.minCorner)),
    maxCorner (std::move(box.maxCorner))
{}

template <class real_t>
Box <real_t>& Box <real_t>::
operator = (const Box& box)
{
    minCorner = box.minCorner;
    maxCorner = box.maxCorner;

    return *this;
}

template struct Box <float>;
template struct Box <double>;

template <class real_t>
Box <real_t> BoxFromCoords (const real_t* coords, size_t size, size_t tupleSize)
{
    const size_t cmps = std::min<size_t> (tupleSize, 3);

    Box <real_t> b (numeric_limits<real_t>::max(), numeric_limits<real_t>::lowest());
    for(size_t i = 0; i < size; i+=tupleSize){
        for(size_t j = 0; j < cmps; ++j){
            if (coords [i+j] < b.minCorner[j])
                b.minCorner[j] = coords [i+j];
            if (coords [i+j] > b.maxCorner[j])
                b.maxCorner[j] = coords [i+j];
        }
    }

    for(size_t i = tupleSize; i < 3; ++i){
        b.minCorner[i] = 0;
        b.maxCorner[i] = 0;
    }

    return b;
}

template Box <float>  BoxFromCoords <float>  (const float*,  size_t, size_t);
template Box <double> BoxFromCoords <double> (const double*, size_t, size_t);

template <class real_t>
Sphere <real_t>::Sphere ()
{}

template <class real_t>
Sphere <real_t>::Sphere (const glm::tvec3<real_t>& _center, const real_t _radius) :
    center (_center),
    radius (_radius)
{}

template <class real_t>
Sphere <real_t>::Sphere (glm::tvec3<real_t>&& _center, const real_t _radius) :
    center (std::move (_center)),
    radius (_radius)
{}

template <class real_t>
Sphere <real_t>::Sphere (const Sphere& sphere) :
    center (sphere.center),
    radius (sphere.radius)
{}

template <class real_t>
Sphere <real_t>::Sphere (Sphere&& sphere) :
    center (std::move (sphere.center)),
    radius (sphere.radius)
{}

template <class real_t>
Sphere <real_t>& Sphere <real_t>::
operator = (const Sphere& sphere)
{
    center = sphere.center;
    radius = sphere.radius;
    return *this;
}

template struct Sphere <float>;
template struct Sphere <double>;

template <class real_t>
Sphere <real_t> SphereFromCoords (const real_t* coords, size_t size, size_t tupleSize)
{
    if (tupleSize > 3) throw ValueError () << "SphereFromCoords: Max tuple size of 3 supported. Given: " << tupleSize;
    if (tupleSize == 0 || size < tupleSize) throw ValueError () << "SphereFromCoords: At least one coordinate is required";

//  compute center
    glm::tvec3<real_t> center (0);
    real_t* pcenter = glm::value_ptr(center);
    VecTupAverage (pcenter, coords, size, tupleSize);

//  find the coordinate with the largest index
    const size_t cmps = std::min<size_t> (tupleSize, 3);

    real_t maxRadSq = 0;
    for(size_t i = 0; i < size; i+=tupleSize) {
        const real_t d = VecDistSq (pcenter, tupleSize, coords + i);
        if (d > maxRadSq)
            maxRadSq = d;
    }

    return Sphere <real_t> (center, sqrt(maxRadSq));
}

template Sphere <float>  SphereFromCoords <float>  (const float*,  size_t, size_t);
template Sphere <double> SphereFromCoords <double> (const double*, size_t, size_t);

}// end of namespace lumeview::util
