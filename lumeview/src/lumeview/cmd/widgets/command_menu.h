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
#include <lumeview/cmd/command_factory.h>
#include <lumeview/cmd/group.h>
#include <lumeview/gui/imgui.h>

namespace lumeview::cmd::widgets
{

inline std::vector <Variant>
MakeVariantVec ()
{
    return {};
}

template <class T>
std::vector <Variant>
MakeVariantVec (T&& t)
{
    return {Variant (std::forward <T> (t))};
}

template <class T, class ... Args>
std::vector <Variant>
MakeVariantVec (Args&& ... args, T&& t)
{
    auto vec = MakeVariantVec (std::forward <Args> (args)...);
    vec.emplace_back (Variant {std::forward <T> (t)});
    return vec;
}

template <class ... Args>
std::shared_ptr <Command> CommandMenu (Group const& group, Args&& ... args)
{
    for (auto const& child : group.children ())
    {
        if (ImGui::BeginMenu (child->name ().c_str ()))
        {
            auto command = CommandMenu (*child, std::forward <Args> (args)...);
            ImGui::EndMenu();

            if (command != nullptr) {
                return command;
            }
        }
    }

    for (auto const& cmdName : group.command_names ())
    {
        auto const& cmdDesc = CommandFactory::command_desc (cmdName, group.id ());
        if (cmdDesc.arguments ().size () == sizeof... (args))
        {
            if (ImGui::Button (cmdName.c_str ()))
            {
                // return CommandFactory::make_command (cmdDesc, std::vector <Variant> {std::forward <Args> (args)...});
                return CommandFactory::new_command (cmdDesc, MakeVariantVec (std::forward <Args> (args)...));
            }
        }
        else
        {
            if (ImGui::BeginMenu (cmdName.c_str ()))
            {
                //widget
                ImGui::EndMenu();
            }
        }
    }
    return {};
}

}// end of namespace lumeview::cmd::widgets
