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

#include <lumeview/lumeview_error.h>
#include <lumeview/gui/arc_ball_control.h>
#include <lumeview/math/vec_math.h>

namespace lumeview
{

using namespace camera;

std::optional <Camera> ArcBallControl::mouse_button (const Camera& currentCamera, int button, int action, int mods)
{
    m_eventListener.set_viewport (currentCamera.viewport ());
    m_eventListener.mouse_button (button, action, mods);
    m_arcBall.set_rotation (currentCamera.rotation ());
    m_arcBall.set_viewport (currentCamera.viewport ());

    if(button == MouseButton::LEFT){
        switch(action) {
            case MouseButtonAction::DOWN: {
                const glm::vec2& cursorPos = m_eventListener.cursor_position ();
                if (m_eventListener.was_double_click (button)) {
                    m_arcBall.end_drag ();
                    float z = currentCamera.depth_at_screen_coord (cursorPos);
                    if (z < 1.f) {
                        Camera camera = currentCamera;
                        glm::vec3 p = camera.unproject (glm::vec3(cursorPos.x, cursorPos.y, z));
                        camera.set_translation (p);
                        return camera;
                    }
                }
                else
                {
                    m_arcBall.set_rotation (currentCamera.rotation());
                    m_arcBall.set_viewport (currentCamera.viewport ());
                    m_arcBall.begin_drag (glm::vec2(cursorPos.x, cursorPos.y));
                }
            } break;

            case MouseButtonAction::UP: {
                if(m_arcBall.dragging ())
                    m_arcBall.end_drag ();
            } break;
        }
    }
    return {};
}


std::optional <Camera> ArcBallControl::mouse_move (const Camera& currentCamera, const glm::vec2& c)
{
    glm::vec2 lastCursorPos = m_eventListener.cursor_position();
    m_eventListener.set_viewport (currentCamera.viewport ());
    m_eventListener.mouse_move (c);

    if(m_arcBall.dragging()){
        Camera camera = currentCamera;
        m_arcBall.drag_to (c);
        camera.set_rotation (m_arcBall.rotation_quaternion());
        return camera;
    }
    else {
        if(m_eventListener.mouse_button_is_down (MouseButton::RIGHT)) {
            Camera camera = currentCamera;
            glm::vec2 d = m_eventListener.cursor_position() - lastCursorPos;
            glm::vec3 fp = camera.project (camera.to());
            fp += glm::vec3(d.x, d.y, 0);
            glm::vec3 nf = camera.unproject (fp);

            //  due to rounding issues, we have to make sure that the resulting direction
            //  is parallel to the camera plane.
            glm::vec3 nfDir = nf - camera.from();
            if (rayPlaneIntersection (nf,
                                      camera.from(),
                                      nfDir,
                                      camera.to(),
                                      camera.forward()))
            {
                camera.translate (camera.to() - nf);
                return camera;
            }
        }
    }
    return {};
}


std::optional <Camera> ArcBallControl::mouse_scroll (const Camera& currentCamera, const glm::vec2& o)
{
    Camera camera = currentCamera;
    m_eventListener.set_viewport (camera.viewport ());
    m_eventListener.mouse_scroll (o);
    float s = 1.f - o.y / 10.f;
    camera.scale (glm::vec3(s));

    return camera;
}
}//    end of namespace lumeview
