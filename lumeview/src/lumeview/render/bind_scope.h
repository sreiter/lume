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

namespace lumeview::render
{

/// Used to bind and automatically unbind a buffer using a scoped variable
/** Make sure to store the bind scope in a local variable. Otherwise it would
 *  go out of scope immediately after construction, resulting in immediante
 *  unbinding.*/
template <class T>
class BindScope
{
public:
    BindScope (const T* bindable)
        : m_bindable (bindable)
    {
        m_bindable->bind ();
    }
    
    BindScope (BindScope&& bindScope)
        : m_bindable (std::exchange (bindScope.m_bindable, nullptr))
    {}

    BindScope (const BindScope&) = delete;
    BindScope& operator = (const BindScope&) = delete;


    ~BindScope ()
    {
        if (m_bindable != nullptr) {
            m_bindable->unbind ();
        }
    }

private:
    const T* m_bindable;
};

}// end of namespace lumeview::render
