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

#include <lumeview/gui/arc_ball_control.h>
#include <lumeview/math/vec_math.h>

namespace lumeview
{

using namespace render;

ArcBallControl::ArcBallControl (std::shared_ptr <Camera> camera)
    : m_camera (std::move (camera))
{
//    throw_if <ControlError> (m_camera == nullptr) << "Invalid camera specified during ArcBallControl construction.";
    m_camera->set_rotation (m_arcBall.rotation_quaternion ());
    m_arcBall.set_rotation (m_camera->rotation());
    m_arcBall.set_viewport (m_camera->viewport ());
}

void ArcBallControl::mouse_button (int button, int action, int mods)
{
    // throw_if <ControlError> (m_camera == nullptr) << "Invalid associated camera.";

    base_t::mouse_button (button, action, mods);

    if(button == MouseButton::LEFT){
        switch(action) {
            case MouseButtonAction::DOWN: {
                const glm::vec2& cursorPos = base_t::cursor_position ();
                if (base_t::was_double_click (button)) {
                    m_arcBall.end_drag ();
                    float z = m_camera->depth_at_screen_coord (cursorPos);
                    if (z < 1.f) {
                        glm::vec3 p = m_camera->unproject (glm::vec3(cursorPos.x, cursorPos.y, z));
                        m_camera->set_translation (p);
                    }
                }
                else
                {
                    m_arcBall.set_rotation (m_camera->rotation());
                    m_arcBall.set_viewport (m_camera->viewport ());
                    m_arcBall.begin_drag (glm::vec2(cursorPos.x, cursorPos.y));
                }
            } break;

            case MouseButtonAction::UP: {
                if(m_arcBall.dragging ())
                    m_arcBall.end_drag ();
            } break;
        }
    }
}


void ArcBallControl::mouse_move (const glm::vec2& c)
{
    // throw_if <ControlError> (m_camera == nullptr) << "Invalid associated camera.";

    glm::vec2 lastCursorPos = base_t::cursor_position();
    base_t::mouse_move (c);

    if(m_arcBall.dragging()){
        m_arcBall.drag_to (c);
        m_camera->set_rotation (m_arcBall.rotation_quaternion());
    }
    else {
        if(base_t::mouse_button_is_down (MouseButton::RIGHT)) {
            glm::vec2 d = base_t::cursor_position() - lastCursorPos;
            glm::vec3 fp = m_camera->project (m_camera->to());
            fp += glm::vec3(d.x, d.y, 0);
            glm::vec3 nf = m_camera->unproject (fp);

            //  due to rounding issues, we have to make sure that the resulting direction
            //  is parallel to the camera plane.
            glm::vec3 nfDir = nf - m_camera->from();
            if (rayPlaneIntersection (nf,
                                      m_camera->from(),
                                      nfDir,
                                      m_camera->to(),
                                      m_camera->forward()))
            {
                m_camera->translate (m_camera->to() - nf);
            }
        }
    }
}


void ArcBallControl::mouse_scroll (const glm::vec2& o)
{
    // throw_if <ControlError> (m_camera == nullptr) << "Invalid associated camera.";

    base_t::mouse_scroll (o);
    float s = 1.f - o.y / 10.f;
    m_camera->scale (glm::vec3(s));
}
}//    end of namespace lumeview
