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

#include <string>
#include <lume/file_io.h>
#include <lumeview/scene/mesh_content.h>
#include <lumeview/widgets/mesh_contents.h>

namespace lumeview::scene
{

MeshContent::MeshContent (std::string filename)
{
    m_mesh = lume::CreateMeshFromFile (filename);
    m_boundingBox = util::BoxFromCoords (UNPACK_DST (m_mesh->annex (lume::keys::vertexCoords)));
    m_filename = std::move (filename);
    m_renderer.set_mesh_data (*m_mesh);
}

const std::string& MeshContent::name () const
{
    return m_filename;
}

bool MeshContent::has_imgui () const
{
    return m_mesh != nullptr;
}

void MeshContent::do_imgui ()
{
    if (!m_mesh) {
        return;
    }

    ImGui::BeginTabBar (m_filename.c_str ());
    if (ImGui::BeginTabItem ("Content"))
    {
        // ImGui::BeginChild ("ChildArea", ImVec2 (0, 100), true, 0);
        widgets::MeshContents (*m_mesh, m_boundingBox);
        // ImGui::EndChild ();
        ImGui::EndTabItem();
    }
    ImGui::EndTabBar ();
}

void MeshContent::render (const render::Camera& camera)
{
    m_renderer.render (camera);
}

std::optional <util::FBox> MeshContent::bounding_box () const
{
    return m_boundingBox;
}

}// end of namespace lumeview::scene
