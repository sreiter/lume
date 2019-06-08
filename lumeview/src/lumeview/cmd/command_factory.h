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

#include <memory>
#include <map>
#include <lumeview/cmd/command.h>
#include <lumeview/cmd/command_desc.h>

namespace lumeview::cmd
{

class CommandFactory
{
public:
    template <class Cmd, class ... Args>
    static CommandDesc& add_command (std::string name)
    {
        s_commandDescs [name] = Entry (CommandDesc (std::move (name)),
                                       &CommandFactory::make_command <Cmd, Args...>);

        return s_commandDescs [name].m_commandDesc;
    }

private:
    struct Entry
    {
        using make_command_function_t =  std::shared_ptr <Command> (lumeview::cmd::CommandFactory::*) (std::vector <Variant> const&);
        Entry ()
            : m_commandDesc ("")
            , m_makeCommandFct (nullptr)
        {}

        Entry (CommandDesc&& desc, make_command_function_t makeCommandFunction)
            : m_commandDesc (std::move (desc))
            , m_makeCommandFct (makeCommandFunction)
        {}

        CommandDesc m_commandDesc;
        make_command_function_t m_makeCommandFct;
    };

    static inline std::map <std::string, Entry> s_commandDescs;

    template <class Cmd>
    std::shared_ptr <Command> make_command (std::vector <Variant> const& params)
    {
        // if (params.size () != 0) throw ...
        return std::make_shared <Cmd> ()
    }

    template <class Cmd, class T0>
    std::shared_ptr <Command> make_command (std::vector <Variant> const& params)
    {
        // if (params.size () != 1) throw ...
        return std::make_shared <Cmd> (std::get <T0> (params[0]));
    }

    template <class Cmd, class T0, class T1>
    std::shared_ptr <Command> make_command (std::vector <Variant> const& params)
    {
        // if (params.size () != 2) throw ...
        return std::make_shared <Cmd> (std::get <T0> (params[0]),
                                       std::get <T1> (params[1]));
    }
};

}// end of namespace lumeview::cmd
