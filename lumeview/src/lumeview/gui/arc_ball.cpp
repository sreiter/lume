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
#include <glm/gtc/quaternion.hpp>
#include <lumeview/gui/arc_ball.h>

namespace lumeview {

ArcBall::ArcBall() :
	m_rotQuat (1.f, 0.f, 0.f, 0.f), // w = 1, rest 0
	m_radius (1.f),
	m_dragging (false)
{
}

void ArcBall::
set_viewport(const Viewport& viewport)
{
	m_viewport = viewport;
}

const Viewport& ArcBall::
viewport () const
{
    return m_viewport;
}

void ArcBall::
set_radius (const float radius)
{
	m_radius = radius;
}

float ArcBall::
radius () const
{
	return m_radius;
}

const glm::quat& ArcBall::rotation_quaternion() const
{
	return m_rotQuat;
}

void ArcBall::set_rotation(const glm::quat& q)
{
	end_drag();
	m_rotQuat = q;
}

void ArcBall::begin_drag(const glm::vec2& c)
{
	if(m_dragging)
		end_drag();

	m_beginDragQuat = m_rotQuat;
	m_beginDragPos = get_ball_point_from_frame_coords(glm::vec2(c.x, viewport_size ().y - c.y));

	m_dragging = true;
}

void ArcBall::drag_to(const glm::vec2& c)
{
	if(m_dragging)
	{
		glm::vec3 v = get_ball_point_from_frame_coords(glm::vec2(c.x, viewport_size ().y - c.y));
		m_rotQuat = m_beginDragQuat * get_quat_from_ball_points(m_beginDragPos, v);
	}
}

void ArcBall::end_drag()
{
	m_dragging = false;
}

glm::vec3 ArcBall::get_ball_point_from_frame_coords(const glm::vec2& c) const
{
    const vec2 vpOffset = viewport_offset ();
    const vec2 vpSize = viewport_size ();

    vec3 p;
    p.x   = (c.x - vpOffset.x - vpSize.x / 2.f) / (m_radius * vpSize.x / 2.f);
    p.y   = (c.y - vpOffset.y - vpSize.y / 2.f) / (m_radius * vpSize.y / 2.f);
    p.z   = 0.0f;

    const float mag = p.x * p.x + p.y * p.y;

    if (mag > 1.0f) {
        float scale = 1.0f / sqrtf (mag);
        p.x *= scale;
        p.y *= scale;
    }
    else
        p.z = sqrtf (1.0f - mag);

    return p;
}

glm::quat ArcBall::get_quat_from_ball_points(const glm::vec3& from, const glm::vec3& to) const
{
    float d = glm::dot (from, to);
    glm::vec3 p = glm::cross (to, from);

	glm::quat quat(d, p.x, p.y, p.z);
	return glm::normalize(quat);
}

glm::vec2 ArcBall::viewport_offset () const
{
    return vec2 (static_cast <float> (m_viewport.x ()), static_cast <float> (m_viewport.y ()));
}

glm::vec2 ArcBall::viewport_size () const
{
    return vec2 (static_cast <float> (std::max <int> (m_viewport.width (), 1)),
                 static_cast <float> (std::max <int> (m_viewport.height (), 1)));
}

}// end of namespace lumeview