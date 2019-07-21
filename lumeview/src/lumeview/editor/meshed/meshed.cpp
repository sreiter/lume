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

#include <lumeview/editor/meshed/meshed.h>
#include <glad/glad.h>  // include before other OpenGL related includes
#include <imgui/imgui.h>

namespace lumeview::editor::meshed
{

Meshed::Meshed ()
    : m_camera (std::make_shared <camera::Camera> ())
    , m_cameraInterpolateCommand (std::make_shared<camera::cmd::Interpolate> ())
    , m_scene (std::make_shared <scene::Node> ())
{
}

bool Meshed::ViewportOffsets::operator == (const ViewportOffsets& vo) const
{
    return m_left   == vo.m_left  &&
           m_top    == vo.m_top   &&
           m_right  == vo.m_right &&
           m_bottom == vo.m_bottom;
}

bool Meshed::ViewportOffsets::operator != (const ViewportOffsets& vo) const
{
    return ! operator == (vo);
}

void Meshed::mouse_button (int button, int action, int mods)
{
    base_t::mouse_button (button, action, mods);

    auto newCamera = m_arcBallControl.mouse_button (*m_camera, button, action, mods);
    if (newCamera) {
        move_camera (*newCamera, 0.2);
    }
}

void Meshed::mouse_move (const glm::vec2& c)
{
    base_t::mouse_move (c);

    auto newCamera = m_arcBallControl.mouse_move (*m_camera, c);
    if (newCamera) {
        move_camera (*newCamera, 0.0);
    }
}

void Meshed::mouse_scroll (const glm::vec2& o)
{
    base_t::mouse_scroll (o);
    auto const& sourceState = m_cameraInterpolateCommand->is_executing ()
                            ? m_cameraInterpolateCommand->target_state ()
                            : *m_camera;
                            
    auto newCamera = m_arcBallControl.mouse_scroll (sourceState, o);

    if (newCamera) {
        move_camera (*newCamera, 0.1);
    }
}

std::shared_ptr <camera::Camera> Meshed::camera ()
{
    return m_camera;
}

void Meshed::schedule_camera_command (std::shared_ptr <cmd::Command> command)
{
    m_cameraCommandQueue.enqueue (std::move (command));
}

void Meshed::move_camera (const camera::Camera& to, const double duration)
{
    // if the interpolation command is currently running, we will tick it once more, so that
    // the camera is actually moving before the interpolation is cancelled again.
    if (m_cameraInterpolateCommand->is_executing ()) {
        m_cameraCommandQueue.tick ();
    }
    
    m_cameraCommandQueue.cancel_all ();
    if (duration <= 0) {
        *m_camera = to;
    }
    else
    {
        m_cameraInterpolateCommand->set_parameters (m_camera, *m_camera, to, duration);
        schedule_camera_command (m_cameraInterpolateCommand);
    }
}

scene::Node& Meshed::scene ()
{
    return *m_scene;
}

bool Meshed::process_gui ()
{
    ImVec2 mainMenuSize (0, 0);
    
    ViewportOffsets newSceneViewportOffsets = draw_scene_gui (mainMenuSize.y);

    if (newSceneViewportOffsets != m_sceneViewportOffsets)
    {
        m_sceneViewportOffsets = newSceneViewportOffsets;
        update_scene_viewport ();
    }

    return true;
}

Meshed::ViewportOffsets Meshed::draw_scene_gui (float const mainMenuHeight)
{
    ViewportOffsets newSceneViewportOffsets;
    const float sceneWidgetWidth = 300.f;
    const float detailsWidgetHeight = 200.f;
    ImVec2 sceneWidgetPos  (0, mainMenuHeight);
    ImVec2 sceneWidgetSize (sceneWidgetWidth, m_camera->viewport ().height () - mainMenuHeight - detailsWidgetHeight);
    ImGui::SetNextWindowPos  (sceneWidgetPos);
    ImGui::SetNextWindowSize (sceneWidgetSize);
    

    if (ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        newSceneViewportOffsets.m_left = sceneWidgetWidth;
        m_scene->draw_scene_tree_gui ();
        m_scene->collect_selection (m_selectedNodes);
        
        ImGui::SetNextWindowPos  (ImVec2 (0, sceneWidgetPos.y + sceneWidgetSize.y));
        ImGui::SetNextWindowSize (ImVec2 (sceneWidgetWidth, detailsWidgetHeight));
        if (ImGui::Begin("Details", nullptr, ImGuiWindowFlags_NoTitleBar |
                                             ImGuiWindowFlags_NoMove |
                                             ImGuiWindowFlags_NoResize |
                                             ImGuiWindowFlags_NoCollapse))
        {
            if (m_selectedNodes.size () == 1)
            {
                if (auto selectedNode = m_selectedNodes.front ().lock ())
                {
                    selectedNode->draw_details_gui ();
                }
            }
        }
        ImGui::End();
    }
    ImGui::End();

    return newSceneViewportOffsets;
}

void Meshed::render ()
{
    glEnable (GL_DEPTH_TEST);

    glClearColor (0.25f, 0.25f, 0.25f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto const bbox = m_scene->bounding_box ();
    if (bbox) {
        m_camera->adjust_z_clip_dists (*bbox);
    }
    else {
        m_camera->set_z_clip_dists (glm::vec2 (0.001, 2));
    }

    auto const& vp = m_camera->viewport ();
    glViewport (vp.x (), vp.y (), vp.width (), vp.height ());

    m_scene->render (*m_camera);
}

void Meshed::update_scene_viewport ()
{
    const auto  vp      = viewport ();
    const auto& offsets = m_sceneViewportOffsets;

    camera::Viewport sceneVP (static_cast <int> (vp.x ()       + offsets.m_left),
                              static_cast <int> (vp.y ()       + offsets.m_top),
                              static_cast <int> (vp.width ()   - (offsets.m_left + offsets.m_right)),
                              static_cast <int> (vp.height ()  - (offsets.m_top  + offsets.m_bottom)));

    if (sceneVP.width  () <= 0 ||
        sceneVP.height () <= 0)
    {
        sceneVP = camera::Viewport (0, 0, 1, 1);
    }
    
    m_camera->set_viewport (sceneVP);
}

}// end of namespace lumeview::editor::meshed
