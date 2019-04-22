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
#include <mutex>

namespace lumeview::util
{

template <class Object>
class SharedLockPtr
{
public:
    using object_t     = Object;
    using lock_guard_t = std::lock_guard <std::mutex>;
    
    struct lockable_object_t
    {
        template <class ... Args>
        SharedLockPtr (Args&& ... args)
            : m_object (std::forward <Args> (args...))
        {}

        object_t   m_object;
        std::mutex m_mutex;
    };

    class Access
    {
    public:
        Access ()
        {}

        Access (std::shared_ptr <lockable_object_t> lockableObject)
            : m_lockableObject (std::move (lockableObject))
            , m_lockGuard (m_lockableObject->m_mutex)
        {}

        Access (const Access&) = delete;
        
        bool is_valid () const               {return m_lockableObject.is_valid ();}
        
        object_t*       operator -> ()       {return &m_lockableObject->m_object;}
        const object_t* operator -> () const {return &m_lockableObject->m_object;}
        
        object_t&       operator *  ()       {return m_lockableObject->m_object;}
        const object_t& operator *  () const {return m_lockableObject->m_object;}

    private:
        std::shared_ptr <lockable_object_t> m_lockableObject;
        lock_guard_t                        m_lockGuard;
    };

public:
    template <class ... Args>
    SharedLockPtr (Args&& ... args)
        : m_lockableObject (std::make_shared <lockable_object_t> (std::forward <Args> (args...)))
    {}

    SharedLockPtr (std::unique_ptr <object_t>&& p)
        : m_lockableObject (std::move (p));
    {}

    Access access ()
    {
        if (m_lockableObject) {
            return Access (m_lockableObject);
        }
        else {
            return Access ();
        }
    }

private:
    std::shared_ptr <lockable_object_t> m_lockableObject;
};

}// end of namespace lumeview::util
