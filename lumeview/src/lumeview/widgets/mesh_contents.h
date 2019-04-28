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

#include <lume/mesh.h>
#include <lumeview/gui/imgui.h>
#include <lumeview/util/shapes.h>
#include <lumeview/util/to_string.h>

namespace lumeview::widgets
{

void MeshContents (lume::Mesh& mesh, const lumeview::util::FBox& box)
{
    auto grobTypes = mesh.grob_types ();

    ImGui::BeginGroup ();
    for (auto gt : grobTypes) {
        ImGui::Text ((lume::GrobSet (gt).name () + (":")).c_str ());
    }
    // ImGui::AlignTextToFramePadding();
    ImGui::Text ("box min:");
    // ImGui::AlignTextToFramePadding();
    ImGui::Text ("box max:");
    // ImGui::AlignTextToFramePadding();
    ImGui::Text ("box size:");
    ImGui::EndGroup ();

    ImGui::SameLine ();

    ImGui::BeginGroup ();
    ImGui::PushStyleVar (ImGuiStyleVar_FramePadding, ImVec2 (2, 0));
    for (auto gt : grobTypes) {
        auto value = lume::to_string (mesh.num (gt));
        ImGui::PushID (static_cast <int> (gt));
        ImGui::InputText ("", &value, ImGuiInputTextFlags_ReadOnly);
        ImGui::PopID ();
    }

    // ImGuiContext& imgui = *ImGui::GImGui;
    // float backupPadding = imgui.Style.FramePadding.y;
    // imgui.Style.FramePadding.y = 0.f;

    auto v = box.min ();
    ImGui::InputFloat3("box.min", &v.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
    v = box.max ();
    ImGui::InputFloat3("box.max", &v.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
    v = box.max () - box.min ();
    ImGui::InputFloat3("box.size", &v.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
    ImGui::PopStyleVar (1);
    // imgui.Style.FramePadding.y = backupPadding;

    ImGui::EndGroup ();
}

}// end of namespace lumeview::widgets
