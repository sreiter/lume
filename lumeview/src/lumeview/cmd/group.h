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
#include <vector>
#include <lumeview/cmd/group_id.h>

namespace lumeview::cmd
{

class Group
{
public:
    Group ()
        : m_id (GroupId::InvalidGroupId)
        , m_name ("")
    {}

    Group (GroupId id, std::string name)
        : m_id (id)
        , m_name (std::move (name))
    {}

    static const Group& root ();
    static       Group& get  (GroupId id);

    std::string const& name () const  {return m_name;}
    
    void add_command (std::string commandName);

    std::vector <Group*>      const& children ()      const {return m_children;}
    std::vector <std::string> const& command_names () const {return m_commands;}

    GroupId id () const {return m_id;}
    
private:
    static void init ();

    Group& add_group (GroupId id, std::string name)
    {
        s_groups [static_cast <int> (id)] = Group (id, std::move (name));
        m_children.push_back (&s_groups [static_cast <int> (id)]);
        return *m_children.back ();
    }

    Group& add_leaf (GroupId id, std::string name)
    {
        add_group (id, name);
        return *this;
    }

private:
    static Group               s_root;
    static std::vector <Group> s_groups;

private:
    GroupId                   m_id;
    std::string               m_name;
    std::vector <Group*>      m_children;
    std::vector <std::string> m_commands;
};

}// end of namespace lumeview::cmd
