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

#include <glad/glad.h>  // include before other OpenGL related includes
#include <imgui/imgui.h>

#include <lumeview/editor/isodraw/isodraw.h>
#include <lumeview/mesh/mesh_factory.h>

using Rect = lumeview::util::Rect;

namespace lumeview::editor::isodraw
{

Isodraw::Isodraw ()
{
    auto canvasMesh = mesh::MeshFactory::create_rect_uv (Rect (-1, -1, 1, 1), Rect (0, 0, 1, 1));
    m_canvas.set_mesh_data (*canvasMesh);
    m_camera.set_translation (glm::vec3 (0, 0, 2.f));
}

Isodraw::~Isodraw ()
{

}

void delete_framebuffer ()
{
    if (m_framebuffer != 0)
    {
        glDeleteFrameBuffers (1, &m_framebuffer);
        m_framebuffer = 0;
    }
}

void Isodraw::init ()
{
    if (m_framebuffer != 0)
    {
        return;
    }

    glGenFramebuffers (1, &m_framebuffer);
    glBindFramebuffer (GL_FRAMEBUFFER, m_framebuffer);

    if (glCheckFramebufferStatus (m_framebuffer) != GL_FRAMEBUFFER_COMPLETE)
    {
        delete_framebuffer ();
        throw lumeview::FramebufferCreationFailed ();
    }

    if (m_texture != 0)
    {
        return;
    }

    glGenTextures (1, &m_texture);
    glBindTexture (GL_TEXTURE_2D, m_texture);
      
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, m_textureSize, m_textureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
}

bool Isodraw::process_gui (util::Rect const& frame)
{
    ImVec2 sceneWidgetPos  (frame.min ().x, frame.min ().y);
    ImVec2 sceneWidgetSize (m_sceneWidgetWidth, frame.height ());
    ImGui::SetNextWindowPos  (sceneWidgetPos);
    ImGui::SetNextWindowSize (sceneWidgetSize);
    
    bool guiExpanded = ImGui::Begin("Isodraw - tools", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    if (guiExpanded)
    {
    }
    ImGui::End();

    return true;
}

void Isodraw::render ()
{
    if (m_framebuffer == 0)
    {
        init ();
    }

    glEnable (GL_DEPTH_TEST);

    glClearColor (0.25f, 0.25f, 0.25f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_camera.set_viewport (viewport ());
    m_canvas.render (m_camera);
}

}// end of namespace lumeview::edutir::isodraw
