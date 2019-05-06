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

#include <atomic>
#include <mutex>
#include <lume/mesh.h>
#include <lumeview/cmd/command_queue.h>
#include <lumeview/mesh/status.h>
#include <lumeview/render/triangle_renderer.h>
#include <lumeview/scene/content.h>
#include <lumeview/util/shapes.h>

namespace lumeview::mesh
{

class MeshContent : public scene::Content
{
public:
    MeshContent (std::string name);

    const std::string& name () const override;

    bool has_imgui () const override;
    void do_imgui () override;
    void render (const camera::Camera& camera) override;

    std::optional <util::FBox> bounding_box () const override;

    void schedule (std::shared_ptr <lumeview::cmd::Command> cmd);
    
    void set_mesh (std::shared_ptr <lume::Mesh> mesh, std::optional <std::string> optionalFilename);
    
    void   set_status (Status const status);
    Status status     () const;
    
private:
    std::shared_ptr <lume::Mesh> m_mesh;
    std::optional <util::FBox>   m_boundingBox;
    std::string                  m_name;
    std::string                  m_filename;
    render::TriangleRenderer     m_renderer;
    cmd::CommandQueue            m_commandQueue;
    bool                         m_updateRendering {false};
    mutable std::mutex           m_mutex;
    std::atomic <Status>         m_status {Status::Ready};
};

}// end of namespace lumeview::mesh
