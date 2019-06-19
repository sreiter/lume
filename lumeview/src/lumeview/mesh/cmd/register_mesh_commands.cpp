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

#include <lumeview/mesh/mesh_content.h>
#include <lumeview/mesh/cmd/load_from_file.h>
#include <lumeview/mesh/cmd/refine.h>
#include <lumeview/mesh/cmd/register_mesh_commands.h>

namespace lumeview::mesh::cmd
{

void RegisterMeshCommands ()
{
    using namespace lumeview::cmd;
    
    using Factory       = CommandFactory;
    using SPMeshContent = std::shared_ptr <mesh::MeshContent>;

    Factory::add_command <LoadFromFile, SPMeshContent, std::string> (
        "LoadFromFile", GroupId::File_Load)
        .help ("Loads the specified geometry from file")
        .arg (Type::MeshContent, "mesh", "The mesh into which to load the specified file.")
        .arg (Type::String, "filename", "The name of the file containing the mesh data to load.");

    Factory::add_command <Refine, SPMeshContent> (
        "Refine", GroupId::Scene_Mesh)
        .help ("Refines the specified mesh.")
        .arg (Type::MeshContent, "mesh", "The mesh that shall be refined.");
}

}// end of namespace lumeview::mesh::cmd
