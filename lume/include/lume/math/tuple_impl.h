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

#include <lume/math/tuple.h>

namespace lume::math::detail
{

template <class Storage>
TupleTemplate <Storage>& TupleTemplate <Storage>:: operator = (value_type const& v)
{
    for(size_t i = 0; i < size (); ++i) {
        value (i) = v;
    }
    return *this;
}

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
    for(size_t i = 0; i < size (); ++i) {
        value (i) *= v [i];
    }
    return *this;
}

template <class Storage>
TupleTemplate <Storage>& TupleTemplate <Storage>:: operator /= (value_type const& v)
{
    for(size_t i = 0; i < size (); ++i) {
        value (i) /= v;
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

template <class Storage>
template <class Array>
typename TupleTemplate <Storage>:: value_type
TupleTemplate <Storage>:: 
dot (Array const& v) const
{
    assert (size () == v.size ());
    size_t const s = size ();
    value_type d = 0;
    
    for(size_t i = 0; i < s; ++i) {
        d += value (i) * v [i];
    }

    return d;
}

template <class Storage>
typename TupleTemplate <Storage>:: value_type
TupleTemplate <Storage>::
length_squared () const
{
    return dot (*this);
}

template <class Storage>
typename TupleTemplate <Storage>:: value_type
TupleTemplate <Storage>::
length () const
{
    return sqrt (length_squared ());
}

template <class Storage>
template <class Array>
typename TupleTemplate <Storage>:: value_type
TupleTemplate <Storage>:: 
distance_squared (Array const& v) const
{
    return (v - *this).length_squared ();
}

template <class Storage>
template <class Array>
typename TupleTemplate <Storage>:: value_type
TupleTemplate <Storage>::
distance (Array const& v) const
{
    return sqrt (distance_squared (v));
}

template <class Storage>
typename TupleTemplate <Storage>:: tuple_with_storage_t
TupleTemplate <Storage>::
normalized () const
{
    auto const len = length ();
    if (len == 0) {
        return (*this) * 0;
    }

    return (*this) / len;
}

template <class Storage>
TupleTemplate <Storage>&
TupleTemplate <Storage>::
normalize ()
{
    auto const len = length ();
    if (len == 0) {
        return (*this) *= 0;
    }

    (*this) /= len;
    return *this;
}

}// end of namespace lume::math::detail

template <class StorageA, class StorageB>
lume::math::TupleWithStorage <typename StorageA::value_type>
operator + (lume::math::detail::TupleTemplate <StorageA> const& a,
            lume::math::detail::TupleTemplate <StorageB> const& b)
{
    assert (a.size () == b.size ());
    auto r = lume::math::TupleWithStorage <typename StorageA::value_type>::uninitialized (a.size ());
    for(size_t i = 0; i < a.size (); ++i) {
        r [i] = a[i] + b[i];
    }
    return r;
}

template <class StorageA, class StorageB>
lume::math::TupleWithStorage <typename StorageA::value_type>
operator - (lume::math::detail::TupleTemplate <StorageA> const& a,
            lume::math::detail::TupleTemplate <StorageB> const& b)
{
    assert (a.size () == b.size ());
    auto r = lume::math::TupleWithStorage <typename StorageA::value_type>::uninitialized (a.size ());
    for(size_t i = 0; i < a.size (); ++i) {
        r [i] = a[i] - b[i];
    }
    return r;
}

template <class Storage>
lume::math::TupleWithStorage <typename Storage::value_type>
operator * (const typename Storage::value_type s,
            lume::math::detail::TupleTemplate <Storage> const& a)
{
    auto r = lume::math::TupleWithStorage <typename Storage::value_type>::uninitialized (a.size ());
    for(size_t i = 0; i < a.size (); ++i) {
        r [i] = s * a[i];
    }
    return r;
}

template <class Storage>
lume::math::TupleWithStorage <typename Storage::value_type>
operator * (lume::math::detail::TupleTemplate <Storage> const& a,
            const typename Storage::value_type s)
{
    return s * a;
}

template <class Storage>
lume::math::TupleWithStorage <typename Storage::value_type>
operator / (lume::math::detail::TupleTemplate <Storage> const& a,
            const typename Storage::value_type s)
{
    auto r = lume::math::TupleWithStorage <typename Storage:: value_type>:: uninitialized (a.size ());
    for(size_t i = 0; i < a.size (); ++i) {
        r [i] = a[i] / s;
    }
    return r;
}
