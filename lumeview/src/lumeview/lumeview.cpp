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

#include <iostream>
#include <string>

#include <glad/glad.h>  // include before other OpenGL related includes

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/io.hpp>

#include <imgui/imgui.h>

#include <lumeview/lumeview.h>
#include <lumeview/lumeview_error.h>
#include <lumeview/camera/cmd/interpolate.h>
#include <lumeview/gui/imgui_binding.h>
#include <lumeview/cmd/command_factory.h>

#include <lumeview/mesh/cmd/register_mesh_commands.h>

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

        static bool commandFactoryInitialized = false;
        if (!commandFactoryInitialized)
        {
            commandFactoryInitialized = true;
            using namespace lumeview;

            mesh::cmd::RegisterMeshCommands ();
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
            lumeview::cmd::CommandFactory::clear_command_descs ();
            lumeview::ImGui_Shutdown();
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

Lumeview::Lumeview ()
{
    StaticDependencies::init ();
	m_imguiListener = ImGui_GetEventListener ();
}

Lumeview::~Lumeview ()
{
    StaticDependencies::shutdown ();
}

void Lumeview::mouse_button (int button, int action, int mods)
{
	base_t::mouse_button (button, action, mods);

	m_imguiListener->mouse_button (button, action, mods);

	if(!ImGui::GetIO().WantCaptureMouse)
		call_editor ([=] (auto& editor) {editor.mouse_button (button, action, mods);});
}

void Lumeview::mouse_move (const glm::vec2& c)
{
	base_t::mouse_move (c);

	m_imguiListener->mouse_move (c);

	if(!ImGui::GetIO().WantCaptureMouse)
		call_editor ([=] (auto& editor) {editor.mouse_move (c);});
}

void Lumeview::mouse_scroll (const glm::vec2& o)
{
	base_t::mouse_scroll (o);

	m_imguiListener->mouse_scroll (o);

	if(!ImGui::GetIO().WantCaptureMouse)
        call_editor ([=] (auto& editor) {editor.mouse_scroll (o);});
}

void Lumeview::set_viewport (const camera::Viewport& vp)
{
	base_t::set_viewport (vp);

    m_imguiListener->set_viewport (vp);
    call_editors ([&] (auto& editor) {editor.set_viewport (vp);});
}

void Lumeview::key (int key, int scancode, int action, int mods)
{
	base_t::key (key, scancode, action, mods);

	m_imguiListener->key (key, scancode, action, mods);
}

void Lumeview::character (unsigned int c)
{
	base_t::character (c);

	m_imguiListener->character (c);
}

void Lumeview::add_editor (std::shared_ptr <editor::Editor> editor)
{
    if (editor != nullptr)
    {
        m_activeEditor = editor;
        m_editors.push_back (std::move (editor));
    }
}

void Lumeview::process_gui ()
{
    ImGui::GetStyle().FrameRounding = 0;
	lumeview::ImGui_NewFrame();

    ImVec2 mainMenuSize (0, 0);
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Panels"))
        {
            ImGui::MenuItem("Show Visualization", NULL, &m_guiShowScene);
            ImGui::MenuItem("Show ImGui Demo", NULL, &m_guiShowDemo);
            ImGui::EndMenu();
        }
        mainMenuSize = ImGui::GetWindowSize ();
        ImGui::EndMainMenuBar();
    }

	if (m_guiShowDemo) {
		ImGui::ShowDemoWindow (&m_guiShowDemo);
    }

    call_editor ([] (auto& editor) {editor.process_gui ();});

	ImGui::Render();
}

void Lumeview::render ()
{
	call_editor ([] (auto& editor) {editor.render ();});
	lumeview::ImGui_Display();
}

}//	end of namespace lumeview
