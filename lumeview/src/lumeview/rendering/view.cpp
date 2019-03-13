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


#include <glad/glad.h>  // include before other OpenGL related includes
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/io.hpp>

#include <lumeview/rendering/view.h>

using namespace std;

namespace lumeview {

View::View () :
    m_viewport (0, 0, 1, 1),
    m_zClipDists (1.e-3, 1.e2)
{}

void View::apply () const
{
	const Viewport& vp = viewport();
    glViewport (vp.x (), vp.y (), vp.width (), vp.height ());
}

void View::set_viewport (const Viewport& vp)
{
    // COND_THROW((vp.x == vp.z) || (vp.y == vp.w),
    //            "Bad vp spefified: " << vp);
    m_viewport = vp;
}

const Viewport& View::viewport () const
{
    return m_viewport;
}

void View::set_camera (const Camera& camera)
{
    m_camera = camera;
}

void View::set_camera (Camera&& camera)
{
    m_camera = std::move (camera);
}

Camera& View::camera ()
{
    return m_camera;
}

const Camera& View::camera () const
{
    return m_camera;
}


glm::vec2 View::aspect_ratio () const
{
    const glm::vec2 size (m_viewport.width (),
                          m_viewport.height ());

    if (size.x == 0 || size.y == 0)
        return glm::vec2 (1.f, 1.f);

    if (size.x > size.y) {
        return glm::vec2 (size.x / size.y, 1.f);
    }
    else {
        return glm::vec2 (1.f, size.y / size.x);
    }
}

glm::mat4 View::view_matrix () const
{
   return m_camera.view_matrix();
}

glm::mat4 View::projection_matrix () const
{
    const auto ar = aspect_ratio ();
    if (ar.x == 0 || ar.y == 0) {
        return glm::mat4 (1.f);
    }
    else {
        return glm::perspective(glm::radians(45.0f), ar.x / ar.y, m_zClipDists.x, m_zClipDists.y);
    }
}

glm::vec3 View::unproject (const glm::vec3& c) const
{
    const Viewport& vp = m_viewport;
    return glm::unProject (glm::vec3(c.x, (vp.height () - c.y), c.z),
                             view_matrix(),
                             projection_matrix(),
                             m_viewport.to_ivec4 ());
}

glm::vec3 View::project (const glm::vec3& c) const
{
    const Viewport& vp = m_viewport;
    glm::vec3 cp = glm::project (c, view_matrix(), projection_matrix(), m_viewport.to_ivec4 ());
    return glm::vec3(cp.x, (vp.height () - cp.y), cp.z);
}

float View::depth_at_screen_coord (const glm::vec2& c) const
{
    const Viewport& vp = m_viewport;
    GLfloat depthVal;
    glReadPixels ((int)c.x, (int)(vp.height () - c.y), 1, 1,
                  GL_DEPTH_COMPONENT, GL_FLOAT, &depthVal);
    return depthVal;
}

void View::set_z_clip_dists (const glm::vec2& c)
{
    m_zClipDists = c;
}

glm::vec2 View::z_clip_dists () const
{
    return m_zClipDists;
}


}// end of namespace lumeview
