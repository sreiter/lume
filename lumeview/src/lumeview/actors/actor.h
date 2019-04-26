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

#include <lumeview/actors/active_actors.h>
#include <lumeview/cmd/command_queue.h>

namespace lumeview::actors
{

class Actor
{
public:
    Actor ()
    {
        ActiveActors::add_actor (this);
    }
    
    virtual ~Actor ()
    {
        if (!m_commandQueue.empty ()) {
            ActiveActors::remove_actor (this);
        }
    }

    void enqueue (std::shared_ptr <cmd::Command> command)
    {
        m_commandQueue.enqueue(std::move (command));
    }

    void tick ()
    {
        if (m_commandQueue.empty ()) {
            return;
        }

        m_commandQueue.tick ();

        if (m_commandQueue.empty ()) {
            ActiveActors::remove_actor (this);
        }
    }

private:
    cmd::CommandQueue m_commandQueue;
};

}// end of namespace lumeview::actors
