// This file is part of lume, a C++ library for lightweight unstructured meshes
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

#ifndef __H__lume_commander
#define __H__lume_commander

#include <map>
#include "lume/lume_error.h"
#include "lume/commands/arguments.h"
#include "lume/commands/command.h"

namespace lume {
namespace commands {

DECLARE_CUSTOM_EXCEPTION (UnknownCommandError, LumeError);
DECLARE_CUSTOM_EXCEPTION (CommandExecutionError, LumeError);

class Commander
{
public:
    using CommandMap = std::map <std::string, std::unique_ptr <Command>>;

    template <class T, class ... Args>
    void add (Args&& ... args)
    {
        auto cmd = std::make_unique <T> (std::forward <Args> (args)...);
        m_commands.emplace (tolower (cmd->name ()), std::move (cmd));
    }

    template <class T>
    void add ()
    {
        auto cmd = std::make_unique <T> ();
        m_commands.emplace (tolower (cmd->name ()), std::move (cmd));
    }

    void run (const std::string& name, Arguments& args)
    {
        try {
            get_command (name).execute (args);
        }
        catch (std::runtime_error& err)
        {
            throw CommandExecutionError () << "In '" << name << "':\n" << "  -> " << err.what ();
        }
    }

    void run (const std::string& name, int argc, char** argv)
    {
        auto& command = get_command (name);

        try {
            std::vector <ArgumentDesc> argDescs = command.argument_descs ();
            std::vector <Variant>      argValues = TranslateArguments (argDescs, argc, argv);
            command.execute (Arguments::create (argDescs, argValues));
        }
        catch (std::runtime_error& err)
        {
            throw CommandExecutionError () << "In '" << name << "':\n" << "  -> " << err.what ();
        }
    }

    CommandMap::const_iterator begin () const
    {
        return m_commands.begin ();
    }

    CommandMap::const_iterator end () const
    {
        return m_commands.end ();
    }

private:
    Command& get_command (const std::string& name)
    {
        auto iter = m_commands.find (tolower (name));

        if (iter == m_commands.end () || iter->second == nullptr) {
            throw UnknownCommandError () << name;
        }

        return *iter->second;
    }

    std::string tolower (const std::string& str) const
    {
        std::string lowStr;
        lowStr.resize (str.size ());
        std::transform (str.begin (), str.end (), lowStr.begin (), ::tolower);
        return lowStr;
    }

    CommandMap m_commands;
};

}// end of namespace commands
}// end of namespace lume

#endif    //__H__lume_commander