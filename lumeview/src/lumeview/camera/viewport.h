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
#include <glm/vec4.hpp>
#include <lumeview/util/rect.h>

namespace lumeview::camera
{

/** Rendering area in window coordinates.
    - x, y: lower left corner of the viewport rectangle.
    - width, height: width and height of the viewport rectangle.
*/
class Viewport
{
public:
    Viewport()
        : m_x (0), m_y (0), m_width (0), m_height (0)
    {}

    Viewport(int x, int y, int width, int height)
        : m_x (x), m_y (y), m_width (width), m_height (height)
    {}

    int x () const       {return m_x;}
    int y () const       {return m_y;}
    int width () const   {return m_width;}
    int height () const  {return m_height;}

    glm::ivec2 size_i ()   const    {return glm::ivec2 (m_width, m_height);}
    glm::vec2  size_f ()   const    {return glm::vec2  (static_cast<float> (m_width), static_cast<float> (m_height));}
    glm::ivec4 to_ivec4 () const    {return glm::ivec4 (m_x, m_y, m_width, m_height);}
    
    util::Rect to_rect () const     {return {static_cast <float> (x ()),
                                             static_cast <float> (y ()),
                                             static_cast <float> (x () + width ()),
                                             static_cast <float> (y () + height ())};}

    static Viewport from_ivec4 (const glm::ivec4& v) 
    {
        return {v.x, v.y, v.z, v.w};
    }

    static Viewport from_rect (const util::Rect& rect) 
    {
        return {static_cast <int> (rect.min ().x),
                static_cast <int> (rect.min ().y),
                static_cast <int> (rect.width ()),
                static_cast <int> (rect.height ())};
    }
    
private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
};

}//    end of namespace lumeview::camera
