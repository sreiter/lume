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

#include <algorithm>
#include <imgui/imgui.h>
#include <lumeview/lumeview_error.h>
#include <lumeview/scene/node.h>

namespace lumeview::scene
{

Node::Node (std::shared_ptr <Content> content)
    : m_content (std::move (content))
{
    if (m_content == nullptr) {
        throw ContentError () << "Invalid content provided to lumeview::scene::Node";
    }
}

Node::~Node ()
{
    if (m_parent) {
        m_parent->remove_child (this);
    }
}

void Node::clear ()
{
    std::vector <std::shared_ptr <Node>> tmpChildren;
    tmpChildren.swap (m_children);

    for (auto& child : tmpChildren) {
        child->set_parent (nullptr);
        child->clear ();
    }

    m_children.swap (tmpChildren);
    m_children.clear ();
}

void Node::add_child (std::shared_ptr <Node> node)
{
    m_children.emplace_back (std::move (node));
    m_children.back ()->set_parent (this);
}

void Node::add_child (std::shared_ptr <Content> content)
{
    m_children.emplace_back (std::make_shared <Node> (std::move (content)));
    m_children.back ()->set_parent (this);
}

void Node::traverse (const std::function <void (Node&)>& callback)
{
    callback (*this);
    traverse_children (callback);
}

void Node::traverse_children (const std::function <void (Node&)>& callback)
{
    for (auto& child : m_children) {
        child->traverse (callback);
    }
}

void Node::set_parent (Node* parent)
{
    if (m_parent &&
        m_parent != parent)
    {
        m_parent->remove_child (this);
    }

    m_parent = parent;
}

void Node::remove_child (Node* child)
{
    auto i = std::find_if (m_children.begin (),
                           m_children.end (),
                           [=](const auto& n) {return n.get () == child;});

    if (i != m_children.end ()) {
        m_children.erase (i);
    }
}

bool Node::has_content () const
{
    return m_content != nullptr;
}

Content& Node::content ()
{
    assert (has_content ());
    return *m_content;
}

const Content& Node::content () const
{
    assert (has_content ());
    return *m_content;
}

void Node::render (const camera::Camera& camera)
{
    traverse ([&camera](auto& node)
              {
                  if (node.has_content ())
                      node.content ().render (camera);
              });
}

std::optional <util::FBox> Node::bounding_box ()
{
    std::optional <util::FBox> box {};
    if (has_content ()) {
        box = content ().bounding_box ();
    }

    traverse_children ([&box] (auto& node)
                       {
                         auto const nodeBox = node.bounding_box ();
                         if (nodeBox) {
                            if (box) {
                                box = util::FBox::from_boxes (*box, *nodeBox);
                            }
                            else {
                                box = nodeBox;
                            }
                         }
                       });
    return box;
}

void Node::do_imgui ()
{
    if (m_content != nullptr
        && !m_content->name (). empty ())
    {
        const bool isLeaf = m_children.empty () && !m_content->has_imgui ();
        ImGuiTreeNodeFlags nodeFlags = //ImGuiTreeNodeFlags_OpenOnArrow
                                       ImGuiTreeNodeFlags_OpenOnDoubleClick
                                     | (m_isSelected ? ImGuiTreeNodeFlags_Selected : 0)
                                     //|
                                     | (isLeaf ? (ImGuiTreeNodeFlags_Leaf
                                                  | ImGuiTreeNodeFlags_NoTreePushOnOpen
                                                  | ImGuiTreeNodeFlags_Bullet)
                                                : 0);

        bool nodeOpen = ImGui::TreeNodeEx (static_cast <void*> (this), nodeFlags, m_content->name ().c_str ());
        
        if (ImGui::IsItemClicked()) {
            node_clicked (*this);
        }

        if (! isLeaf
            && nodeOpen)
        {
            if (m_content->has_imgui ()) {
                m_content->do_imgui ();
                // ImGui::Separator ();
            }

            ImGui::TreePop();
        }
    }

    traverse_children ([](auto& node) {
                            node.do_imgui ();
                            ImGui::Separator ();
                        });
}

void Node::node_clicked (Node& clickedNode)
{
    if (m_parent != nullptr) {
        m_parent->node_clicked (clickedNode);
    }
    else
    {
        ImGuiIO const& io = ImGui::GetIO ();
        if (io.KeyCtrl) {
            clickedNode.toggle_selection ();
        }
        else
        {
            traverse ([] (Node& node) {node.deselect ();});
            clickedNode.select ();
        }
    }
}
    
void Node::select ()
{
    m_isSelected = true;
}

void Node::deselect ()
{
    m_isSelected = false;
}

void Node::toggle_selection ()
{
    if (is_selected ())
        deselect ();
    else
        select ();
}

bool Node::is_selected () const
{
    return m_isSelected;
}


}//    end of namespace lumeview::scene
