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
#include <lumeview/cmd/command.h>
#include <lumeview/mesh/mesh_content.h>
#include <lume/refinement.h>

namespace lumeview::mesh::cmd
{

class Refine : public lumeview::cmd::AsynchronousCommand
{
public:
    // static CommandDesc command_desc ()
    // {
    //     return MakeCommandDesc <Refine, MeshContent> (
    //             "Refine",
    //             "Refines the elements of a grid");
    // }

    Refine (std::weak_ptr <MeshContent> meshContent)
        : m_meshContent (std::move (meshContent))
    {}

    RunResult on_run () override
    {
        std::shared_ptr <MeshContent> meshContent (m_meshContent);
        if (meshContent == nullptr)
            return RunResult::Done;

        auto mesh = meshContent->mesh ();
        if (mesh == nullptr)
            return RunResult::Done;

        meshContent->set_status (lumeview::mesh::Status::Refining);
        auto refinedMesh = lume::RefineTriangles (mesh);

        // todo: transfer annex data from mesh to refinedMesh

        meshContent->set_mesh (refinedMesh);
        return RunResult::Done;
    }

private:
    std::weak_ptr <MeshContent> m_meshContent;
    std::string                 m_filename;
};

}