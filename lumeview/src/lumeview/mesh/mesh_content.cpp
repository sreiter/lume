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
#include <lumeview/mesh/mesh_content.h>
#include <lumeview/widgets/mesh_contents.h>

namespace lumeview::mesh
{

MeshContent::MeshContent (std::string name)
    : m_name (std::move (name))
{}

const std::string& MeshContent::name () const
{
    std::lock_guard <std::mutex> lockGuard (m_mutex);
    return m_name;
}

bool MeshContent::has_imgui () const
{
    return true;
}

void MeshContent::do_imgui ()
{
    std::lock_guard <std::mutex> lockGuard (m_mutex);

    auto const status = this->status ();
    
    ImGui::StatusText (GetStatusMessage (status).c_str (), status != Status::Ready);

    if (m_mesh != nullptr) {
        ImGui::BeginTabBar (m_name.c_str ());
        if (ImGui::BeginTabItem ("Content"))
        {
            if (m_boundingBox) {
                widgets::MeshContents (*m_mesh, *m_boundingBox);
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar ();
    }
}

void MeshContent::render (const camera::Camera& camera)
{
    if (m_updateRendering) {
        m_updateRendering = false;
        if (m_mesh != nullptr) {
            m_renderer.set_mesh_data (*m_mesh);
        }
        else {
            m_renderer.clear ();
        }
    }
    std::lock_guard <std::mutex> lockGuard (m_mutex);
    m_renderer.render (camera);
}

std::optional <util::FBox> MeshContent::bounding_box () const
{
    std::lock_guard <std::mutex> lockGuard (m_mutex);
    return m_boundingBox;
}

void MeshContent::schedule (std::shared_ptr <lumeview::cmd::Command> cmd)
{
    std::lock_guard <std::mutex> lockGuard (m_mutex);
    m_commandQueue.enqueue (std::move (cmd));
}

void MeshContent::set_mesh (std::shared_ptr <lume::Mesh> mesh, std::optional <std::string> optionalFilename)
{
    set_status (Status::ComputingBoundingBox);
    auto const box = util::BoxFromCoords (UNPACK_DST (mesh->annex (lume::keys::vertexCoords)));
    set_status (Status::Processing);
    std::lock_guard <std::mutex> lockGuard (m_mutex);

    if (optionalFilename) {
        m_filename = *optionalFilename;
    }

    m_mesh = mesh;
    m_boundingBox = box;
    m_updateRendering = true;
    set_status (Status::Ready);
}

void MeshContent::set_status (Status const status)
{
    m_status.store (status);
}

Status MeshContent::status () const
{
    return m_status.load ();
}

}// end of namespace lumeview::mesh
