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

#include <chrono>
#include <lumeview/cmd/command.h>
#include <lumeview/render/camera.h>
#include <lumeview/util/shapes.h>

namespace lumeview::cmd::camera
{

class Interpolate : public SynchronousCommand
{
public:
    using camera_t = render::Camera;

public:
    Interpolate ()
    {}
    
    Interpolate (std::weak_ptr <camera_t> camera,
                 const camera_t& sourceState,
                 const camera_t& targetState,
                 double duration)
        : m_camera (camera)
        , m_sourceState (sourceState)
        , m_targetState (targetState)
        , m_duration (duration)
    {
    }

    void set_parameters (std::weak_ptr <camera_t> camera,
                         const camera_t& sourceState,
                         const camera_t& targetState,
                         double duration)
    {
        m_camera      = camera;
        m_sourceState = sourceState;
        m_targetState = targetState;
        m_duration    = seconds_t (duration);
    }

    const camera_t& source_state () const   {return m_sourceState;}
    const camera_t& target_state () const   {return m_targetState;}

protected:
    void on_prepare () override
    {
        m_startTime = clock_t::now ();
    }

    RunResult on_run () override
    {
        auto camera = std::shared_ptr <camera_t> (m_camera);
        if (camera == nullptr) {
            return RunResult::Done;
        }

        // we don't want to alter the viewport...
        auto const vp = camera->viewport ();

        auto const timeEllapsed = std::chrono::duration_cast<seconds_t>(clock_t::now () - m_startTime);
        
        if (m_duration.count () == 0 ||
            timeEllapsed > m_duration)
        {
            *camera = m_targetState;
            camera->set_viewport (vp);
            return RunResult::Done;
        }

        float const ia = static_cast <float> (timeEllapsed.count () / m_duration.count ());

        *camera = camera_t::lerp (m_sourceState, m_targetState, ia);
        camera->set_viewport (vp);
        return RunResult::Yield;
    }

private:
    using clock_t   = std::chrono::high_resolution_clock;
    using seconds_t = std::chrono::duration <double>;
    using timepoint_t = std::chrono::time_point<clock_t, seconds_t>;

private:
    std::weak_ptr <camera_t> m_camera;
    camera_t                 m_sourceState;
    camera_t                 m_targetState;
    timepoint_t              m_startTime;
    seconds_t                m_duration;

};

}// end of namespace lumeview::cmd::camera
