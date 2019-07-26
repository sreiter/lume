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

#include <lumeview/camera/camera.h>
#include <lumeview/camera/cmd/interpolate.h>
#include <lumeview/cmd/command.h>
#include <lumeview/cmd/command_queue.h>
#include <lumeview/editor/editor.h>
#include <lumeview/gui/arc_ball_control.h>
#include <lumeview/scene/node.h>
#include <lumeview/util/rect.h>

namespace lumeview::editor::meshed
{

class Meshed : public Editor
{
public:
    Meshed ();
    
    void mouse_button (int button, int action, int mods) override;
    void mouse_move   (const glm::vec2& c) override;
    void mouse_scroll (const glm::vec2& o) override;

    bool process_gui (util::Rect const& frame) override;
    void render      () override;

    scene::Node& scene ();

    std::shared_ptr<camera::Camera> camera ();

    void schedule_camera_command (std::shared_ptr <cmd::Command> command);

    void move_camera (const camera::Camera& to, const double duration);

private:
    bool draw_scene_gui (util::Rect const& frame);

private:
    ArcBallControl                             m_arcBallControl;
    std::shared_ptr <camera::Camera>           m_camera;
    std::shared_ptr <camera::cmd::Interpolate> m_cameraInterpolateCommand;
    std::shared_ptr <scene::Node>              m_scene;
    std::vector <std::weak_ptr <scene::Node>>  m_selectedNodes;

    cmd::CommandQueue m_cameraCommandQueue;

    bool m_guiExpanded {false};

    const float m_sceneWidgetWidth = 300.f;
    const float m_detailsWidgetHeight = 200.f;
};

}// end of namespace lumeview::editor::meshed
