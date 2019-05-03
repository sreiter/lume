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

#ifndef __H__arc_ball
#define __H__arc_ball

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <lumeview/camera/viewport.h>

namespace lumeview
{

class ArcBall
{
  public:
	ArcBall();

	void set_viewport (const camera::Viewport& viewport);
	void set_radius (const float radius);

	const camera::Viewport& viewport () const;
	float radius () const;

	inline const glm::mat4 rotation_matrix () const	{return glm::mat4_cast(m_rotQuat);}
	const glm::quat& rotation_quaternion () const;
	void set_rotation (const glm::quat& q);

	void begin_drag (const glm::vec2& c);
	void drag_to (const glm::vec2& c);
	void end_drag ();

	bool dragging () const {return m_dragging;}
	
  private:
  	using vec2 = glm::vec2;
  	using vec3 = glm::vec3;
  	using mat4 = glm::mat4;
  	using quat = glm::quat;

	vec3 get_ball_point_from_frame_coords (const vec2& c) const;
	quat get_quat_from_ball_points (const vec3& from, const vec3& to) const;

    vec2 viewport_offset () const;
    vec2 viewport_size () const;

	quat	m_rotQuat;

	quat	m_beginDragQuat;
	vec3	m_beginDragPos;

    camera::Viewport m_viewport;

	vec2	m_lastMousePos;

	float	m_radius;

	bool	m_dragging;
};

}// end of namespace lumeview

#endif	//__H__arc_ball
