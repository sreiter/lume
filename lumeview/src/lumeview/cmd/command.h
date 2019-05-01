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

#include <atomic>
#include <cassert>
#include <future>

namespace lumeview::cmd
{

class CommandQueue;

class Command
{
public:
    enum class ExecutionMode
    {
        Sync,
        Async
    };

    enum class Status
    {
        None,
        Scheduled,
        Preparing,
        Running,
        Yield,
        Done,
        Canceled
    };

    enum class PrepareResult
    {
        InProgress,
        Done
    };

    enum class RunResult
    {
        Done,
        Yield
    };

public:
    Command (ExecutionMode executionMode)
        : m_executionMode (executionMode)
    {}

    Command (const Command& cmd)
        : m_executionMode (cmd.m_executionMode)
    {
        set_status (cmd.status ());
    }

    void run ()
    {
        Status currentStatus = status ();

        if (currentStatus == Status::None ||
            currentStatus == Status::Canceled)
        {
            return;
        }

        assert (currentStatus == Status::Scheduled ||
                currentStatus == Status::Preparing ||
                currentStatus == Status::Yield);

        if (currentStatus == Status::Scheduled) {
            currentStatus = set_status (Status::Preparing);
        }
        
        if (currentStatus == Status::Preparing) {
            if (on_prepare () != PrepareResult::Done) {
                // the command is not Scheduled to execute yet.
                // It may be shared between multiple command queues.
                return;
            }
        }
        else if (currentStatus != Status::Yield) {
            assert (!"Invalid status encountered in 'Command::run' method");
        }


        if (m_executionMode == ExecutionMode::Async) {
          std::async (std::launch::async, &Command::runner, this);
        }
        else {
          runner ();
        }
    }

    Status status () const
    {
        return m_status.load ();
    }

    inline bool is_executing ()
    {
        auto const currentStatus = status ();
        return currentStatus == Status::Running ||
               currentStatus == Status::Yield;
    }

protected:
    virtual void          on_scheduled ()     {}
    virtual PrepareResult on_prepare   ()     {return PrepareResult::Done;}
    virtual RunResult     on_run       ()     = 0;
    virtual void          on_cancel    ()     {}

private:
    friend class CommandQueue;

    void runner  ()
    {
        assert (status () == Status::Preparing ||
                status () == Status::Yield);

        set_status (Status::Running);
        
        const auto runResult = on_run ();
        
        switch (runResult)
        {
            case RunResult::Done:
                set_status (Status::Done);
                break;
            case RunResult::Yield:
                set_status (Status::Yield);
                break;
        }
    }

    Status set_status (Status const status)
    {
        m_status.store (status);
        return status;
    }

    void canceled ()
    {
        if (status () != Status::Canceled)
        {
            set_status (Status::Canceled);
            on_cancel ();
        }
    }

    void scheduled ()
    {
        auto const currentStatus = status ();

        if (currentStatus == Status::Running ||
            currentStatus == Status::Yield)
        {
            canceled ();
        }

        set_status (Status::Scheduled);
        on_scheduled ();
    }

private:
    std::atomic <Status> m_status       {Status::None};
    ExecutionMode        m_executionMode;
};

class SynchronousCommand : public Command
{
public:
    SynchronousCommand ()
        : Command (Command::ExecutionMode::Sync)
    {}
};

class AsynchronousCommand : public Command
{
public:
    AsynchronousCommand ()
        : Command (Command::ExecutionMode::Async)
    {}
};

}// end of namespace lumeview::cmd
