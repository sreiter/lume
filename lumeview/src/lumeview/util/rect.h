// This file is part of lumeview, a lightweight viewer for unstructured meshes
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

#pragma once

#include <glm/vec2.hpp>

namespace lumeview::util
{

class Rect
{
public:
    using vec2 = glm::vec2;

public:
    Rect ()  {}

    Rect (float xMin, float yMin, float xMax, float yMax) :
        m_min (xMin, yMin), m_max (xMax, yMax)    {}

    Rect (const vec2& min, const vec2& max) :
        m_min (min), m_max (max)  {}

    float width () const        {return m_max.x - m_min.x;}
    float height () const       {return m_max.y - m_min.y;}

    vec2& min ()             {return m_min;}
    const vec2& min () const {return m_min;}

    vec2& max ()             {return m_max;}
    const vec2& max () const {return m_max;}

    /// returns true if the p.xy lies inside or on the bounds of the rectangle
    template <class vec_t>
    bool contains_point (const vec_t& p) const
    {
        return (p.x >= m_min.x) && 
               (p.y >= m_min.y) && 
               (p.x <= m_max.x) && 
               (p.y <= m_max.y);
    }

private:
    vec2 m_min;
    vec2 m_max;
};

}// end of namespace lumeview::util
