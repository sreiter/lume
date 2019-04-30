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

#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <glm/detail/qualifier.hpp>

namespace ImGui
{

struct IDScope
{
    IDScope (const char* str_id)
    {
        PushID (str_id);
    }

    IDScope (const char* str_id_begin, const char* str_id_end)
    {
        PushID (str_id_begin, str_id_end);
    }

    IDScope (const void* ptr_id)
    {
        PushID (ptr_id);
    }

    IDScope (int int_id)
    {
        PushID (int_id);
    }

    ~IDScope ()
    {
        PopID ();
    }
};

struct SlimScope
{
    SlimScope (float paddingX = 2, float paddingY = 0)
        : SlimScope (ImVec2 (paddingX, paddingY))
    {}

    SlimScope (const ImVec2& padding)
    {
        ImGui::PushStyleVar (ImGuiStyleVar_FramePadding, padding);
    }

    ~SlimScope ()
    {
        ImGui::PopStyleVar (1);
    }
};

inline void ReadOnly (const char* label, int v)
{
    ImGui::InputScalar (label, ImGuiDataType_S32, &v, NULL, NULL, NULL, ImGuiInputTextFlags_ReadOnly);
}

inline void ReadOnly (const char* label, float v, const char* format = "%.3f")
{
    ImGui::InputScalar (label, ImGuiDataType_Float, &v, NULL, NULL, format, ImGuiInputTextFlags_ReadOnly);
}

template <glm::length_t L, glm::qualifier Q>
void ReadOnly (const char* label, glm::vec <L, int, Q> v)
{
    static_assert (L > 0 && L <= 4);

    switch (L)
    {
    case 1:
        ReadOnly (label, v.x);
        break;
    case 2:
        ImGui::InputInt2(label, &v.x, ImGuiInputTextFlags_ReadOnly);
        break;
    case 3:
        ImGui::InputInt3(label, &v.x, ImGuiInputTextFlags_ReadOnly);
        break;
    case 4:
        ImGui::InputInt4(label, &v.x, ImGuiInputTextFlags_ReadOnly);
        break;
    }
}
// InputScalar(const char* label, ImGuiDataType data_type, void* v, const void* step = NULL,
//             const void* step_fast = NULL, const char* format = NULL, ImGuiInputTextFlags flags = 0);
template <glm::length_t L, glm::qualifier Q>
void ReadOnly (const char* label, glm::vec <L, float, Q> v, const char* format = "%.3f")
{
    static_assert (L > 0 && L <= 4);

    switch (L)
    {
    case 1:
        ReadOnly (label, v.x, format);
        break;
    case 2:
        ImGui::InputFloat2(label, &v.x, format, ImGuiInputTextFlags_ReadOnly);
        break;
    case 3:
        ImGui::InputFloat3(label, &v.x, format, ImGuiInputTextFlags_ReadOnly);
        break;
    case 4:
        ImGui::InputFloat4(label, &v.x, format, ImGuiInputTextFlags_ReadOnly);
        break;
    }
}

}// end of namespace ImGui
