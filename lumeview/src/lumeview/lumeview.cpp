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

// #include "log.h"
#include "lumeview.h"
// #include "imgui/imgui.h"
// #include "imgui/imgui_binding.h"

// #include "shapes.h"
// #include "plain_visualization.h"
// #include "subset_visualization.h"
// #include "subset_info_annex_imgui.h"

// #include "lume/normals.h"
// #include "lume/mesh.h"
// #include "lume/topology.h"
// #include "lume/vec_math_raw.h"
// #include "lume/file_io.h"

// #include "pettyprof/pettyprof.h"

using namespace std;
using namespace lume;

// namespace
// {
// bool InitializeImGuiExecutors ()
// {
//     // SubsetInfoAnnex::set_imgui_executor (&SubsetInfoAnnex_ImGui)
//     return true;
// }
// }// end of empty namespace

namespace lumeview {

// void LumeviewInit ()
// {
// 	SubsetInfoAnnex::set_imgui_executor (&SubsetInfoAnnex_ImGui);

// 	// if (!gladLoadGLLoader ((GLADloadproc)glfwGetProcAddress)) {
// 	// 	THROW("GLAD::INITIALIZATION\n  Failed to initialize GLAD" << endl);
// 	// }

// 	if (!gladLoadGL ()) {
// 		THROW("GLAD::INITIALIZATION\n  Failed to initialize GLAD" << endl);
// 	}

// 	lumeview::ImGui_Init();
// }

// void LumeviewShutdown ()
// {
// 	ImGui_Shutdown ();
// }

////////////////////////////////////////////////////////////////////////////////
// Lumeview

class Lumeview::ImGuiLifetimeManager
{
public:
    ImGuiLifetimeManager ()
    {
        lumeview::ImGui_Init();
    }

    ~ImGuiLifetimeManager ()
    {
        lumeview::ImGui_Shutdown();
    }
};

Lumeview::Lumeview () :
    m_view (std::make_shared <View> ()),
	m_guiShowScene (true),
	m_guiShowLog (true),
	m_guiShowDemo (false)
{
    static bool gladInitialized = gladLoadGL ();
    // static bool imguiExecutorsInitialized = InitializeImGuiExecutors ();

    if (s_imguiLifetimeManager.expired ())
    {
        m_imguiLifetimeManager = std::make_shared <ImGuiLifetimeManager> ();
        s_imguiLifetimeManager = m_imguiLifetimeManager;
    }
    else {
        m_imguiLifetimeManager = std::shared_ptr <ImGuiLifetimeManager> (s_imguiLifetimeManager);
    }

	m_imguiListener = ImGui_GetEventListener ();
}

void Lumeview::clear ()
{
    m_scene.reset();
}

void Lumeview::mouse_button (int button, int action, int mods)
{
	base_t::mouse_button (button, action, mods);

	m_imguiListener->mouse_button (button, action, mods);

	if(!ImGui::GetIO().WantCaptureMouse)
		m_arcBallView.mouse_button (button, action, mods);
}


void Lumeview::mouse_move (const glm::vec2& c)
{
	base_t::mouse_move (c);

	m_imguiListener->mouse_move (c);

	if(!ImGui::GetIO().WantCaptureMouse)
		m_arcBallView.mouse_move (c);
}


void Lumeview::mouse_scroll (const glm::vec2& o)
{
	base_t::mouse_scroll (o);

	m_imguiListener->mouse_scroll (o);

	if(!ImGui::GetIO().WantCaptureMouse)
		m_arcBallView.mouse_scroll (o);
}



void Lumeview::set_viewport (const glm::ivec4& vp)
{
	base_t::set_viewport (vp);

	m_imguiListener->set_viewport (vp);
	m_arcBallView.set_viewport (vp);
}



void Lumeview::key (int key, int scancode, int action, int mods)
{
	base_t::key (key, scancode, action, mods);

	m_imguiListener->key (key, scancode, action, mods);

	if(!ImGui::GetIO().WantCaptureKeyboard)
		m_arcBallView.key (key, scancode, action, mods);
}


void Lumeview::character (unsigned int c)
{
	base_t::character (c);

	m_imguiListener->character (c);

	if(!ImGui::GetIO().WantCaptureKeyboard)
		m_arcBallView.character (c);
}

void Lumeview::set_scene (const SPScene& scene)
{
	m_scene = scene;
}

void Lumeview::process_gui ()
{
	lumeview::ImGui_NewFrame();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Panels"))
        {
            ImGui::MenuItem("Show Visualization", NULL, &m_guiShowScene);
            ImGui::MenuItem("Show Log", NULL, &m_guiShowLog);
            ImGui::MenuItem("Show ImGui Demo", NULL, &m_guiShowDemo);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

	if (m_guiShowLog)
		DefLog().draw("log", &m_guiShowLog);

	if (m_guiShowDemo)
		ImGui::ShowDemoWindow (&m_guiShowDemo);

	if (m_guiShowScene && m_scene)
		m_scene->do_imgui (&m_guiShowScene);

	ImGui::Render();

	MessageQueue::dispatch ();
}

void Lumeview::render ()
{
	glEnable (GL_DEPTH_TEST);

	glClearColor (0.25f, 0.25f, 0.25f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_scene) {
		const glm::vec2 clipDists = m_scene->estimate_z_clip_dists(m_arcBallView.view());
		m_arcBallView.view().set_z_clip_dists (clipDists);
		m_scene->render (m_arcBallView.view());
	}

	lumeview::ImGui_Display();
}


// static void PrintMeshInfo (SPMesh mesh)
// {
// 	Box box = BoxFromCoords (UNPACK_DST(*mesh->coords()));

// 	LOGT(mesh, "  #vertices:    " << mesh->coords()->num_tuples() << std::endl);
// 	LOGT(mesh, "  #edges:       " << mesh->grobs(EDGE).size() << std::endl);
// 	LOGT(mesh, "  #triangles:   " << mesh->grobs(TRI).size() << std::endl);
// 	LOGT(mesh, "  #quads:       " << mesh->grobs(QUAD).size() << std::endl);
// 	LOGT(mesh, "  #tetrahedra:  " << mesh->grobs(TET).size() << std::endl);
// 	LOGT(mesh, "  #hexahedra:   " << mesh->grobs(HEX).size() << std::endl);
// 	LOGT(mesh, "  #pyramids:    " << mesh->grobs(PYRA).size() << std::endl);
// 	LOGT(mesh, "  #prisms:      " << mesh->grobs(PRISM).size() << std::endl);
// 	LOGT(mesh, "  Bounding box -> min: " << box.minCorner << std::endl);
// 	LOGT(mesh, "               -> max: " << box.maxCorner << std::endl);
// }

}//	end of namespace lumeview
