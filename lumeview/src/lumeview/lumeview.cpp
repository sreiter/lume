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


#include <string>

#include <glad/glad.h>	// include before other OpenGL related includes

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/io.hpp>

#include <imgui/imgui.h>

#include <lumeview/lumeview.h>
#include <lumeview/lumeview_error.h>
#include <lumeview/camera/cmd/interpolate.h>
#include <lumeview/gui/imgui_binding.h>

namespace
{

class StaticDependencies
{
public:
    static void init ()
    {
        static bool gladInitialized = gladLoadGL ();
        if (!gladInitialized) {
            throw lumeview::InitializationError () << "Unable to initialize glad.";
        }

        if (inst ().m_refCount == 0) {
            lumeview::ImGui_Init();
        }
        ++inst ().m_refCount;
    }

    static void shutdown ()
    {
        --inst ().m_refCount;
        
        if (inst ().m_refCount < 0)
        {
            assert (false);
            inst ().m_refCount = 0;
            return;
        }

        if (inst ().m_refCount == 0) {
            lumeview::ImGui_Shutdown();
            lumeview::ArcBall arcBall;
        }
    }

private:
    static StaticDependencies& inst ()
    {
        static StaticDependencies is;
        return is;
    }

    StaticDependencies () = default;

    int m_refCount {0};
};

}

namespace lumeview
{

bool Lumeview::ViewportOffsets::operator == (const ViewportOffsets& vo) const
{
    return m_left   == vo.m_left  &&
           m_top    == vo.m_top   &&
           m_right  == vo.m_right &&
           m_bottom == vo.m_bottom;

}

bool Lumeview::ViewportOffsets::operator != (const ViewportOffsets& vo) const
{
    return ! operator == (vo);
}

Lumeview::Lumeview () :
    m_camera (std::make_shared <camera::Camera> ()),
    m_cameraInterpolateCommand (std::make_shared<camera::cmd::Interpolate> ()),
	m_guiShowScene (true),
	m_guiShowLog (true),
	m_guiShowDemo (false)
{
    StaticDependencies::init ();
	m_imguiListener = ImGui_GetEventListener ();
}

Lumeview::~Lumeview ()
{
    StaticDependencies::shutdown ();
}

void Lumeview::clear ()
{
    // m_scene.reset();
}

void Lumeview::mouse_button (int button, int action, int mods)
{
	base_t::mouse_button (button, action, mods);

	m_imguiListener->mouse_button (button, action, mods);

	if(!ImGui::GetIO().WantCaptureMouse) {
		auto newCamera = m_arcBallControl.mouse_button (*m_camera, button, action, mods);
        if (newCamera) {
            move_camera (*newCamera, 0.2);
        }
    }
}

void Lumeview::mouse_move (const glm::vec2& c)
{
	base_t::mouse_move (c);

	m_imguiListener->mouse_move (c);

	if(!ImGui::GetIO().WantCaptureMouse) {
		auto newCamera = m_arcBallControl.mouse_move (*m_camera, c);
        if (newCamera) {
            move_camera (*newCamera, 0.0);
        }
    }
}

void Lumeview::mouse_scroll (const glm::vec2& o)
{
	base_t::mouse_scroll (o);

	m_imguiListener->mouse_scroll (o);

	if(!ImGui::GetIO().WantCaptureMouse) {
        auto const& sourceState = m_cameraInterpolateCommand->is_executing ()
                                ? m_cameraInterpolateCommand->target_state ()
                                : *m_camera;
                                
		auto newCamera = m_arcBallControl.mouse_scroll (sourceState, o);

        if (newCamera) {
            move_camera (*newCamera, 0.1);
        }
    }
}

std::shared_ptr<camera::Camera> Lumeview::camera ()
{
    return m_camera;
}

void Lumeview::schedule_camera_command (std::shared_ptr <cmd::Command> command)
{
    m_cameraCommandQueue.enqueue (std::move (command));
}

void Lumeview::move_camera (const camera::Camera& to, const double duration)
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

void Lumeview::set_viewport (const camera::Viewport& vp)
{
	base_t::set_viewport (vp);

    m_imguiListener->set_viewport (vp);
    update_scene_viewport ();
}

void Lumeview::key (int key, int scancode, int action, int mods)
{
	base_t::key (key, scancode, action, mods);

	m_imguiListener->key (key, scancode, action, mods);

	// if(!ImGui::GetIO().WantCaptureKeyboard) {
	// 	m_arcBallControl.key (key, scancode, action, mods);
 //    }
}

void Lumeview::character (unsigned int c)
{
	base_t::character (c);

	m_imguiListener->character (c);

	// if(!ImGui::GetIO().WantCaptureKeyboard) {
	// 	m_arcBallControl.character (c);
 //    }
}

scene::Node& Lumeview::scene ()
{
    return m_scene;
}

void Lumeview::process_gui ()
{
    ImGui::GetStyle().FrameRounding = 0;
	lumeview::ImGui_NewFrame();

    ViewportOffsets newSceneViewportOffsets;

    ImVec2 mainMenuSize (0, 0);
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Panels"))
        {
            ImGui::MenuItem("Show Visualization", NULL, &m_guiShowScene);
            ImGui::MenuItem("Show Log", NULL, &m_guiShowLog);
            ImGui::MenuItem("Show ImGui Demo", NULL, &m_guiShowDemo);
            ImGui::EndMenu();
        }
        mainMenuSize = ImGui::GetWindowSize ();
        ImGui::EndMainMenuBar();
    }

    // if (m_guiShowLog) {
    //     DefLog().draw("log", &m_guiShowLog);
    // }

	if (m_guiShowDemo) {
		ImGui::ShowDemoWindow (&m_guiShowDemo);
    }

    {

        const float sceneWidgetWidth = 300.f;
        ImGui::SetNextWindowPos (ImVec2 (0, mainMenuSize.y));
        ImGui::SetNextWindowSize (ImVec2 (sceneWidgetWidth, m_camera->viewport ().height () - mainMenuSize.y));
        
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove
                                      | ImGuiWindowFlags_NoResize;
                                      // | ImGuiWindowFlags_NoCollapse;

        if (ImGui::Begin("Scene", nullptr, window_flags)) {
            newSceneViewportOffsets.m_left = sceneWidgetWidth;
            m_scene.do_imgui ();
        }
        ImGui::End();
    }

	ImGui::Render();

    if (!(newSceneViewportOffsets == m_sceneViewportOffsets))
    {
        m_sceneViewportOffsets = newSceneViewportOffsets;
        update_scene_viewport ();
    }
}

void Lumeview::render ()
{
	glEnable (GL_DEPTH_TEST);

	glClearColor (0.25f, 0.25f, 0.25f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto const bbox = m_scene.bounding_box ();
    if (bbox) {
        m_camera->adjust_z_clip_dists (*bbox);
    }
    else {
        m_camera->set_z_clip_dists (glm::vec2 (0.001, 2));
    }

    auto const& vp = m_camera->viewport ();
    glViewport (vp.x (), vp.y (), vp.width (), vp.height ());

    m_scene.render (*m_camera);

	lumeview::ImGui_Display();
}

void Lumeview::update_scene_viewport ()
{
    const auto vp       = viewport ();
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

}//	end of namespace lumeview
