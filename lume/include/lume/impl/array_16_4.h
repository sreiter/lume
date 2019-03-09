// This file is part of lume, a C++ library for lightweight unstructured meshes
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

#ifndef __H__lume_array_16_4
#define __H__lume_array_16_4

namespace lume {
namespace impl {

/// An array of 16 4-bit integers
class Array_16_4 {
public:
    Array_16_4 () : m_data (0) {}
    Array_16_4 (std::uint64_t v) : m_data (v) {}
    Array_16_4 (const Array_16_4& ar) : m_data (ar.m_data) {}

    static Array_16_4 ascending_order () {
        static const Array_16_4 ao (build_ascending_order()); 
        return ao;
    }

    bool operator == (const Array_16_4& a) const    {return m_data == a.m_data;}
    
    void clear () {
        m_data = 0;
    }

    index_t get (const index_t i) const {
        return (m_data >> i*4) & 0xF;
    }
    void set (const index_t i, const index_t v) {
        m_data &= ~(0xF << i*4);    // set i-th entry to 0 (important!)
        m_data |= (v & 0xF) << i*4; // set i-th entry to v
    }

private:
    static std::uint64_t build_ascending_order () {
        using u64 = std::uint64_t;
        u64 ao = 0;
        for(u64 i = 0; i < 16; ++i) {
            ao |= i << 4*i;
        }
        return ao;
    }
    std::uint64_t   m_data;
};

}// end of namespace impl    
}// end of namespace lume

#endif    //__H__lume_array_16_4
