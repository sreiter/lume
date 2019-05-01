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
#include <vector>
#include <lumeview/cmd/camera/interpolate.h>
#include <lumeview/scene/node.h>

namespace lumeview::cmd::camera
{

class FocusNodes : public Interpolate
{
public:
    using base_t        = Interpolate;
    using camera_t      = render::Camera;
    using node_vector_t = std::vector <std::shared_ptr <scene::Node>>;

public:
    FocusNodes (std::weak_ptr <camera_t> camera,
                node_vector_t            nodes,
                double const             duration)
        : m_nodes  (std::move (nodes))
        , m_camera (camera)
        , m_duration (duration)
    {
    }

    void on_prepare () override
    {
        std::shared_ptr <camera_t> camera (m_camera);

        if (camera == nullptr ||
            m_nodes.empty ())
        {
            return;
        }

        auto targetState = *camera;
        auto const box = focus_box ();

        if (box) {
            targetState.center_sphere (util::FSphere::from_box (*box));
        }
        else {
            m_duration = 0;
        }

        base_t::set_parameters (camera, *camera, targetState, m_duration);

        base_t::on_prepare ();
    }

private:
    std::optional <util::FBox> focus_box () const
    {
        if (m_nodes.empty ()) {
            return {};
        }

        std::optional <util::FBox> box;
        for (auto const& node : m_nodes) {
            if (node == nullptr) {
                continue;
            }

            auto const nodeBox = node->bounding_box ();
            if (!nodeBox) {
                continue;
            }

            if (box) {
                box = util::FBox::from_boxes (*box, *nodeBox);
            }
            else {
                box = *nodeBox;
            }
        }

        return box;
    }

private:
    node_vector_t            m_nodes;
    std::weak_ptr <camera_t> m_camera;
    double                   m_duration;
};
}// end of namespace lumeview::cmd::camera
