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

#include <map>
#include <vector>
#include <lumeview/cmd/active_command_queues.h>
#include <lumeview/cmd/command_queue.h>

namespace
{
    // We use a map here so that we can set the value to nullptr without affecting an ongoing iteration.
    std::map <lumeview::cmd::CommandQueue*, lumeview::cmd::CommandQueue*> s_activeQueues;
    std::vector <lumeview::cmd::CommandQueue*>                            s_addedQueues;
    std::vector <lumeview::cmd::CommandQueue*>                            s_removedQueues;
}

namespace lumeview::cmd
{

void ActiveCommandQueues::tick ()
{
    for (auto queue : s_removedQueues) {
        s_activeQueues.erase (queue);
    }
    s_removedQueues.clear ();

    for (auto queue : s_addedQueues) {
        s_activeQueues.insert (std::make_pair (queue, queue));
    }
    s_addedQueues.clear ();

    if (s_activeQueues.empty ()) {
        return;
    }
    
    for (auto entry : s_activeQueues) {
        if (entry.second != nullptr) {
            entry.second->tick ();
        }
    }
}

void ActiveCommandQueues::add (CommandQueue* queue)
{
    s_addedQueues.push_back (queue);
}

void ActiveCommandQueues::remove (CommandQueue* queue)
{
    auto const iter = s_activeQueues.find (queue);
    if (iter != s_activeQueues.end ()) {
        iter->second = nullptr;
    }

    s_removedQueues.push_back (queue);
}

}// end of namespace lumeview::cmd
