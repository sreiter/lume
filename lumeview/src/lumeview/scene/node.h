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

#include <functional>
#include <memory>
#include <optional>
#include <vector>
#include <lumeview/render/camera.h>
#include <lumeview/scene/content.h>
#include <lumeview/util/shapes.h>

namespace lumeview::scene
{

class Node
{
public:
    Node () = default;
    Node (std::unique_ptr <Content> content);
    ~Node ();

    Node (const Node&) = delete;
    Node& operator = (const Node&) = delete;

    void clear ();

    void add_child (std::unique_ptr <Node> node);
    void add_child (std::unique_ptr <Content> content);

    void traverse (const std::function <void (Node&)>& callback);
    void traverse_children (const std::function <void (Node&)>& callback);
    
    bool has_content () const;
    Content& content ();
    const Content& content () const;

    void render (const render::Camera& camera);
    std::optional <util::FBox> bounding_box ();
    void do_imgui ();

private:
    void set_parent (Node* parent);
    void remove_child (Node* child);

    std::unique_ptr <Content>            m_content;
    std::vector <std::unique_ptr <Node>> m_children;
    Node*                                m_parent {nullptr};

    // imgui
    bool m_isSelected {false};
};

}// end of namespace lumeview::scene