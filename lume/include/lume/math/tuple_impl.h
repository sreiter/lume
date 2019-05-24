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

#pragma once

namespace lume::math::detail
{

template <class Storage>
template <class Array>
TupleTemplate <Storage>& TupleTemplate <Storage>:: operator = (Array const& v)
{
    assert (size () == v.size ());
    for(size_t i = 0; i < size (); ++i) {
        value (i) = v [i];
    }
    return *this;
}

template <class Storage>
template <class Array>
TupleTemplate <Storage>& TupleTemplate <Storage>:: operator += (Array const& v)
{
    assert (size () == v.size ());
    for(size_t i = 0; i < size (); ++i) {
        value (i) += v [i];
    }
    return *this;
}

template <class Storage>
template <class Array>
TupleTemplate <Storage>& TupleTemplate <Storage>:: operator -= (Array const& v)
{
    assert (size () == v.size ());
    for(size_t i = 0; i < size (); ++i) {
        value (i) -= v [i];
    }
    return *this;
}

template <class Storage>
TupleTemplate <Storage>& TupleTemplate <Storage>:: operator *= (value_type const& v)
{
    assert (size () == v.size ());
    for(size_t i = 0; i < size (); ++i) {
        value (i) *= v [i];
    }
    return *this;
}

template <class Storage>
TupleTemplate <Storage>& TupleTemplate <Storage>:: operator /= (value_type const& v)
{
    assert (size () == v.size ());
    for(size_t i = 0; i < size (); ++i) {
        value (i) /= v [i];
    }
    return *this;
}

template <class Storage>
template <class Array>
bool TupleTemplate <Storage>:: operator == (Array const& v) const
{
    assert (size () == v.size ());

    for(size_t i = 0; i < size (); ++i) {
        if (value (i) != v.value (i)) {
            return false;
        }
    }

    return true;
}

template <class Storage>
template <class Array>
bool TupleTemplate <Storage>:: operator != (Array const& v) const
{
    return ! (*this == v);
}

}// end of namespace lume::math::detail
