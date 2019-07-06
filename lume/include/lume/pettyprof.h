// This file is part of pettyprof, a very basic C++ profiler
//
// Copyright (C) 2018 Sebastian Reiter
// Copyright (C) 2018 G-CSC, Goethe University Frankfurt
// Author: Sebastian Reiter <s.b.reiter@gmail.com>
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
#include <iostream>
#include <stack>

namespace pepro {

namespace impl {

class ProfileMark {
public:
    friend class ProfileStack;

    inline ProfileMark (const char* name);
    inline ~ProfileMark ();

    void cancel ();

private:
    bool m_popOnDestruction {true};
    bool m_canceled {false};
};


class ProfileStack {
public:
    static void push (const char* name, ProfileMark* mark)
    {
        auto& stack = inst().m_stack;
        stack.emplace (name, clock::now(), mark);
    }

    static void pop ()
    {
        auto& stack = inst().m_stack;
        const auto& e = stack.top();
        if (!e.mark->m_canceled)
        {
            const auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - e.start);
            if (dur >= inst().m_outputThreshold) {
                std::cout << "PEPRO " << e.name << ":\t" << dur.count() / 1.e3 << " (s)" << std::endl;
                ++inst ().m_numOutputsSinceLastSeparator;
            }
        }
        e.mark->m_popOnDestruction = false;
        stack.pop();
        
        if (stack.empty () &&
            inst ().m_numOutputsSinceLastSeparator > 0)
        {
            inst ().m_numOutputsSinceLastSeparator = 0;
            std::cout << "PEPRO =================================================" << std::endl;
        }
    }

    static void set_output_threshold (std::chrono::milliseconds ms)
    {
        inst().m_outputThreshold = ms;
    }

private:
    ProfileStack () : m_outputThreshold (1) {}
    static ProfileStack& inst ()
    {
        static ProfileStack ps;
        return ps;
    }

    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;

    struct Entry {
        Entry (const char* _name, time_point _start, ProfileMark* _mark) :
            name (_name),
            start (_start),
            mark (_mark){}

        const char* name;
        time_point start;
        ProfileMark* mark;
    };

    std::stack <Entry>  m_stack;
    clock::duration     m_outputThreshold;
    size_t              m_numOutputsSinceLastSeparator {0};
};


inline ProfileMark::ProfileMark (const char* name)
{
    ProfileStack::push (name, this);
}

inline ProfileMark::~ProfileMark ()
{
    if (m_popOnDestruction)
        ProfileStack::pop ();
}

}// end of namespace impl
}// end of namespace pepro

#define PEPRO_FUNC() pepro::impl::ProfileMark peproMark_f_##__func__(__func__);
#define PEPRO_BEGIN(id) pepro::impl::ProfileMark peproMark_##id(#id);
#define PEPRO_END() pepro::impl::ProfileStack::pop();
#define PEPRO_CANCEL() pepro::impl::ProfileStack::top().mark->cancel (); pepro::impl::ProfileStack::pop();
