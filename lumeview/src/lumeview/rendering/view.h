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

#ifndef __H__view
#define __H__view

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <lumeview/rendering/camera.h>
#include <lumeview/rendering/viewport.h>

namespace lumeview {

class View {
public:
    View ();
    
    void apply () const;

    void set_viewport (const Viewport& vp);
    const Viewport& viewport () const;

    void            set_camera (const Camera& camera);
    void            set_camera (Camera&& camera);

    Camera&         camera ();
    const Camera&   camera () const;

    glm::vec2 aspect_ratio () const;

    glm::mat4 view_matrix () const;

    glm::mat4 projection_matrix () const;

    glm::vec3 unproject (const glm::vec3& c) const;

    glm::vec3 project (const glm::vec3& c) const;

    float depth_at_screen_coord (const glm::vec2& c) const;

    void set_z_clip_dists (const glm::vec2& c);
    glm::vec2 z_clip_dists () const;

private:
    Camera    m_camera;
    Viewport  m_viewport;
    glm::vec2 m_zClipDists;
};

}// end of namespace lumeview

#endif  //__H__view
