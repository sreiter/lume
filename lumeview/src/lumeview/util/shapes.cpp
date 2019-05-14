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
#include <lume/math/vector_math.h>
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
    m_min (min),
    m_max (max)
{}

template <class real_t>
Box <real_t>::Box (const vec_t& min, const vec_t& max) :
    m_min (min),
    m_max (max)
{}

template <class real_t>
Box <real_t>::Box (vec_t&& min, vec_t&& max) :
    m_min (std::move (min)),
    m_max (std::move (max))
{}

template <class real_t>
Box <real_t>::Box (const Box& box) :
    m_min (box.m_min),
    m_max (box.m_max)
{}

template <class real_t>
Box <real_t>::Box (Box&& box) :
    m_min (std::move(box.m_min)),
    m_max (std::move(box.m_max))
{}

template <class real_t>
Box <real_t> Box <real_t>::from_boxes (const Box <real_t>& a, const Box <real_t>& b)
{
    Box <real_t> box;
    for (glm::length_t i = 0; i < 3; ++i)
    {
        box.m_min [i] = std::min (a.m_min [i], b.m_min [i]);
        box.m_max [i] = std::max (a.m_max [i], b.m_max [i]);
    }
    return box;
}

template <class real_t>
Box <real_t>& Box <real_t>::
operator = (const Box& box)
{
    m_min = box.m_min;
    m_max = box.m_max;

    return *this;
}

template <class real_t>
typename Box <real_t>::vec_t&
Box <real_t>::min ()
{
    return m_min;
}

template <class real_t>
typename Box <real_t>::vec_t&
Box <real_t>::max ()
{
    return m_max;
}

template <class real_t>
const typename Box <real_t>::vec_t&
Box <real_t>::min () const
{
    return m_min;
}

template <class real_t>
const typename Box <real_t>::vec_t&
Box <real_t>::max () const
{
    return m_max;
}

template <class real_t>
typename Box <real_t>::vec_t
Box <real_t>::diagonal () const
{
    return m_max - m_min;
}

template class Box <float>;
template class Box <double>;

template <class real_t>
Box <real_t> BoxFromCoords (const real_t* coords, glm::length_t size, glm::length_t tupleSize)
{
    const glm::length_t cmps = std::min<glm::length_t> (tupleSize, 3);

    Box <real_t> b (numeric_limits<real_t>::max(), numeric_limits<real_t>::lowest());
    for(glm::length_t i = 0; i < size; i+= tupleSize){
        for(glm::length_t j = 0; j < cmps; ++j){
            if (coords [i+j] < b.min ()[j])
                b.min ()[j] = coords [i+j];
            if (coords [i+j] > b.max ()[j])
                b.max ()[j] = coords [i+j];
        }
    }

    for(glm::length_t i = tupleSize; i < 3; ++i){
        b.min ()[i] = 0;
        b.max ()[i] = 0;
    }

    return b;
}

template Box <float>  BoxFromCoords <float>  (const float*,  glm::length_t, glm::length_t);
template Box <double> BoxFromCoords <double> (const double*, glm::length_t, glm::length_t);

template <class real_t>
Sphere <real_t>::Sphere ()
{}

template <class real_t>
Sphere <real_t>::Sphere (const vec_t& center, const real_t radius) :
    m_center (center),
    m_radius (radius)
{}

template <class real_t>
Sphere <real_t>::Sphere (vec_t&& center, const real_t radius) :
    m_center (std::move (center)),
    m_radius (radius)
{}

template <class real_t>
Sphere <real_t>::Sphere (const Sphere& sphere) :
    m_center (sphere.m_center),
    m_radius (sphere.m_radius)
{}

template <class real_t>
Sphere <real_t>::Sphere (Sphere&& sphere) :
    m_center (std::move (sphere.m_center)),
    m_radius (sphere.m_radius)
{}

template <class real_t>
Sphere <real_t> Sphere <real_t>::from_box (const Box <real_t>& box)
{
    return Sphere <real_t> {(box.min () + box.max ()) * real_t (0.5), real_t (0.5) * glm::length (box.diagonal ())};
}

template <class real_t>
Sphere <real_t>& Sphere <real_t>::
operator = (const Sphere& sphere)
{
    m_center = sphere.m_center;
    m_radius = sphere.m_radius;
    return *this;
}

template <class real_t>
const typename Sphere <real_t>::vec_t&
Sphere <real_t>::center () const
{
    return m_center;
}

template <class real_t>
real_t
Sphere <real_t>::radius () const
{
    return m_radius;
}

template struct Sphere <float>;
template struct Sphere <double>;

template <class real_t>
Sphere <real_t> SphereFromCoords (const real_t* coords, glm::length_t size, glm::length_t tupleSize)
{
    if (tupleSize > 3) throw ValueError () << "SphereFromCoords: Max tuple size of 3 supported. Given: " << tupleSize;
    if (tupleSize == 0 || size < tupleSize) throw ValueError () << "SphereFromCoords: At least one coordinate is required";

//  compute center
    glm::tvec3<real_t> center (0);
    real_t* pcenter = glm::value_ptr(center);
    lume::math::raw::VecTupAverage (pcenter, coords, size, tupleSize);

//  find the coordinate with the largest index
    const glm::length_t cmps = std::min<glm::length_t> (tupleSize, 3);

    real_t maxRadSq = 0;
    for(glm::length_t i = 0; i < size; i+=tupleSize) {
        const real_t d = lume::math::raw::VecDistSq (pcenter, tupleSize, coords + i);
        if (d > maxRadSq)
            maxRadSq = d;
    }

    return Sphere <real_t> (center, sqrt(maxRadSq));
}

template Sphere <float>  SphereFromCoords <float>  (const float*,  glm::length_t, glm::length_t);
template Sphere <double> SphereFromCoords <double> (const double*, glm::length_t, glm::length_t);

}// end of namespace lumeview::util
