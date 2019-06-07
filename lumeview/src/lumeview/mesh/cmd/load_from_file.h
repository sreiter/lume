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

#include <string>
#include <lume/file_io.h>
#include <lumeview/cmd/command.h>
#include <lumeview/mesh/mesh_content.h>

namespace lumeview::mesh::cmd
{

class LoadFromFile : public lumeview::cmd::AsynchronousCommand
{
public:
    // static CommandDesc command_desc ()
    // {
    //     using namespace lume::cmd;
    //     return MakeCommandDesc <MeshContent, LoadFromFile> (
    //             "LoadFromFile",
    //             "Loads the contents of a mesh from a specified file."
    //         .arg (Type::String, "filename", "The filename containing the mesh data.")
    //         .extend_menu (Menus::Load);
    // }

    LoadFromFile (std::weak_ptr <MeshContent> meshContent,
                  std::string filename)
        : m_meshContent (std::move (meshContent))
        , m_filename (std::move (filename))
    {}

    RunResult on_run () override
    {
        std::shared_ptr <MeshContent> meshContent (m_meshContent);
        if (meshContent != nullptr) {
            meshContent->set_status (lumeview::mesh::Status::Loading);
            auto mesh = lume::CreateMeshFromFile (m_filename);
            meshContent->set_mesh (mesh, m_filename);
        }
        return RunResult::Done;
    }

private:
    std::weak_ptr <MeshContent> m_meshContent;
    std::string                 m_filename;
};

}// end of namespace lumeview::mesh::cmd
