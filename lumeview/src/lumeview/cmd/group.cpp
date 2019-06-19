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

#include <lumeview/cmd/group.h>

namespace lumeview::cmd
{

Group               Group::s_root;
std::vector <Group> Group::s_groups;

const Group& Group::root ()
{
    init ();
    return s_root;
}

Group& Group::get (GroupId id)
{
    init ();
    return s_groups [static_cast <int> (id)];
}

void Group::add_command (std::string commandName)
{
    m_commands.push_back (std::move (commandName));
}

void Group::init ()
{
    if (s_groups.empty ())
    {
        s_groups.resize (static_cast <int> (GroupId::NumGroupIds) + 1);

        s_root.add_group (GroupId::File, "File")
              .add_leaf  (GroupId::File_Load, "Load");

        s_root.add_group (GroupId::Scene, "Scene")
              .add_leaf  (GroupId::Scene_Mesh, "Mesh");
    }
}

}// end of namespace lumeview::cmd
