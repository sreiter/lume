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
#include <queue>
#include <future>
#include <lumeview/cmd/active_command_queues.h>
#include <lumeview/cmd/command.h>

namespace lumeview::cmd
{

class CommandQueue
{
public:
    using command_ptr_t = std::shared_ptr <Command>;

public:

    ~CommandQueue ()
    {
        if (!empty ()) {
            ActiveCommandQueues::remove (this);
        }
    }

    void enqueue (command_ptr_t const& command)
    {
        if (empty ()) {
            ActiveCommandQueues::add (this);
        }

        m_commandQueue.push (command);
        m_commandQueue.back ()->scheduled ();
    }

    bool empty () const
    {
        return m_commandQueue.empty () &&
               m_runningCommand == nullptr;
    }

    void cancel_all ()
    {
        if (m_runningCommand != nullptr) {
            m_runningCommand->canceled ();
        }
        m_runningCommand = nullptr;
        while (!m_commandQueue.empty ())
        {
            m_commandQueue.front ()->canceled ();
            m_commandQueue.pop ();
        }
    }

    /** If no command is currently running, the next valid command from the queue will be run.
        Canceled or invalid commands will be removed from the top of the queue until a
        command is found which is ready.
    */
    void tick ()
    {
        if (empty ()) {
            return;
        }

        if (m_runningCommand)
        {
            const auto status = m_runningCommand->status ();

            if (status == Command::Status::Yield)
            {
                m_runningCommandFuture = m_runningCommand->run ();
                return;
            }

            if (status == Command::Status::Preparing ||
                status == Command::Status::Running)
            {
                return;
            }
            else if (status == Command::Status::Done)
            {
                // todo: call 'apply_results' on m_runningCommand
            }

            assert (status != Command::Status::None &&
                    status != Command::Status::Scheduled);

            m_runningCommand = nullptr;
        }

        run_next_command ();
        
        if (empty ()) {
            ActiveCommandQueues::remove (this);
        }
    }

private:
    void pop_invalid_commands ()
    {
        while (!m_commandQueue.empty ())
        {
            if (m_commandQueue.front () == nullptr ||
                m_commandQueue.front ()->status () == Command::Status::Canceled)
            {
                m_commandQueue.pop ();
            }
            else {
                break;
            }
        }
    }

    void run_next_command ()
    {
        assert (m_runningCommand == nullptr ||
                m_runningCommand->executing () == false);

        assert (m_runningCommandFuture.valid () == false ||
                m_runningCommandFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready);

        pop_invalid_commands ();

        if (empty ()) {
            return;
        }

        m_runningCommand = m_commandQueue.front ();
        m_commandQueue.pop ();

        const auto status = m_runningCommand->status ();
        if (status != Command::Status::Scheduled &&
            status != Command::Status::Preparing)
        {
            assert (!"Command in invalid state encountered on front of the command queue.");
            m_runningCommand = nullptr;
            return;
        }

        m_runningCommandFuture = m_runningCommand->run ();
    }

private:
    std::queue <command_ptr_t> m_commandQueue;
    command_ptr_t              m_runningCommand;
    std::future <void>         m_runningCommandFuture;
};

}// end of namespace lumeview::cmd
