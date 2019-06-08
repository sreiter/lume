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

#include <lume/commands/arguments.h>
#include <lume/commands/types.h>
#include <lumeview/cmd/command.h>
#include <lumeview/cmd/group.h>

namespace lumeview::mesh
{
    class MeshContent;
}

namespace lumeview::cmd
{

enum class Type
{
    Char,
    UnsignedChar,
    Int,
    UnsignedInt,
    SizeT,
    Float,
    Double,
    String,
    MeshContent
};

using Variant = std::variant <char,
                              unsigned char,
                              int,
                              unsigned int,
                              size_t,
                              float,
                              double,
                              std::string,
                              std::shared_ptr <lumeview::mesh::MeshContent>>;


class Argument
{
public:
    Argument (Type type, std::string name, std::string help)
        : m_type (type)
        , m_name (std::move (name))
        , m_help (std::move (help))
    {}

    Type                type () const   {return m_type;}
    const std::string&  name () const   {return m_name;}
    const std::string&  help () const   {return m_help;}

private:
    Type         m_type;
    std::string  m_name;
    std::string  m_help;
};

class CommandDesc
{
public:
    CommandDesc (std::string name)
        : m_name (std::move (name))
    {}

    CommandDesc& help (std::string help)
    {
        m_help = std::move (help);
        return *this;
    }

    CommandDesc& arg (Type type, std::string name, std::string help)
    {
        m_arguments.emplace_back (Argument (type, std::move (name), std::move (help)));
        return *this;
    }

    CommandDesc& group (Group group)
    {
        m_groups.push_back (group);
        return *this;
    }

    std::vector <Argument> const& arguments () const
    {
        return m_arguments;
    }

private:
    std::string            m_name;
    std::string            m_help;
    std::vector <Argument> m_arguments;
    std::vector <Group>    m_groups;
};

}// end of namespace lumeview::cmd
