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
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <lumeview/math/vec_math.h>
#include <lumeview/lumeview_error.h>
#include <lumeview/render/camera.h>

namespace lumeview::render
{

Camera Camera::lerp (const Camera& from, const Camera& to, float ia)
{
    Camera camera;
    camera.m_scale      = glm::lerp (from.m_scale, to.m_scale, ia);
    camera.m_rot        = glm::lerp (from.m_rot, to.m_rot, ia);
    camera.m_trans      = glm::lerp (from.m_trans, to.m_trans, ia);
    camera.m_zClipDists = glm::lerp (from.m_zClipDists, to.m_zClipDists, ia);
    
    const glm::vec4 vpFrom (from.m_viewport.to_ivec4 ());
    const glm::vec4 vpTo   (to.m_viewport.to_ivec4 ());
    camera.m_viewport.from_ivec4 (glm::lerp (vpFrom, vpTo, ia));

    return camera;
}

Camera::Camera () :
	m_scale (1.f, 1.f, 1.f),
	m_rot (1.f, 0.f, 0.f, 0.f),
	m_trans (0.f, 0.f, 0.f),
    m_viewport (0, 0, 1, 1),
    m_zClipDists (1.e-3, 1.e2)
{
}

void Camera::set_viewport (const Viewport& vp)
{
    if ((vp.width () <= 0) || (vp.height () <= 0)) {
        throw ValueError () << "Bad viewport specified: " << vp.to_ivec4 ();
    }

    m_viewport = vp;
}

const Viewport& Camera::viewport () const
{
    return m_viewport;
}

void Camera::set_scale (const glm::vec3& scale)
{
	m_scale = scale;
}

void Camera::set_rotation (const glm::quat& rot)
{
	m_rot = rot;
}

void Camera::set_translation (const glm::vec3& trans)
{
	m_trans = trans;
}

const glm::vec3& Camera::scale () const
{
	return m_scale;
}

const glm::quat& Camera::rotation () const
{
	return m_rot;
}


const glm::vec3& Camera::translation () const
{
	return m_trans;
}

void Camera::scale (const glm::vec3& scaleFac)
{
	m_scale *= scaleFac;
}

void Camera::rotate (const glm::quat& rot)
{
	m_rot = rot * m_rot;
}

void Camera::translate (const glm::vec3& offset)
{
	m_trans += offset;
}

glm::vec3 Camera::right () const
{
	return axis (m_rot * glm::quat (0, 1, 0, 0) * conjugate(m_rot));
}

glm::vec3 Camera::up () const
{
	return axis (m_rot * glm::quat (0, 0, 1, 0) * conjugate(m_rot));
}

glm::vec3 Camera::forward () const
{
	return axis (m_rot * glm::quat (0, 0, 0, -1) * conjugate(m_rot));
}

glm::vec3 Camera::from () const
{
	return m_trans - forward() * m_scale;
}

glm::vec3 Camera::to () const
{
	return m_trans;
}

glm::mat4 Camera::view_matrix () const
{
	return glm::lookAt (from(), to(), up());
}

glm::mat4 Camera::projection_matrix () const
{
    const auto ar = aspect_ratio ();
    if (ar.x == 0 || ar.y == 0) {
        return glm::mat4 (1.f);
    }
    else {
        return glm::perspective(glm::radians(45.0f), ar.x / ar.y, m_zClipDists.x, m_zClipDists.y);
    }
}

glm::vec2 Camera::aspect_ratio () const
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

glm::vec3 Camera::unproject (const glm::vec3& c) const
{
    return glm::unProject (glm::vec3(c.x, (m_viewport.height () - c.y), c.z),
                             view_matrix(),
                             projection_matrix(),
                             m_viewport.to_ivec4 ());
}

glm::vec3 Camera::project (const glm::vec3& c) const
{
    glm::vec3 cp = glm::project (c, view_matrix(), projection_matrix(), m_viewport.to_ivec4 ());
    return glm::vec3(cp.x, (m_viewport.height () - cp.y), cp.z);
}

float Camera::depth_at_screen_coord (const glm::vec2& c) const
{
    GLfloat depthVal;
    glReadPixels ((int)c.x, (int)(m_viewport.height () - c.y), 1, 1,
                  GL_DEPTH_COMPONENT, GL_FLOAT, &depthVal);
    return depthVal;
}

void Camera::set_z_clip_dists (const glm::vec2& c)
{
    m_zClipDists = c;
}

glm::vec2 Camera::z_clip_dists () const
{
    return m_zClipDists;
}

void Camera::adjust_z_clip_dists (const util::FBox& box)
{
    adjust_z_clip_dists (util::FSphere::from_box (box));
}

void Camera::adjust_z_clip_dists (const util::FSphere& sphere)
{
    using namespace glm;
    const vec3  n = normalize (forward ());
    const vec3  p = from ();
    const float o = offsetPointToPlane (sphere.center (), p, n);

    vec2 zDist (o - sphere.radius (), o + sphere.radius ());

    if (zDist.y <= 0)
        zDist.y = 1.f;

    if (zDist.x < zDist.y * 1.e-5f || zDist.x >= zDist.y)
        zDist.x = zDist.y * 1.e-5f;

    set_z_clip_dists (zDist * glm::vec2(0.9f, 1.1f));
}

void Camera::center_sphere (const util::FSphere& sphere)
{
    m_trans = sphere.center ();
    m_scale = glm::vec3 (3.f * sphere.radius ());
}

}// end of namespace lumeview

