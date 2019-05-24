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

#include <array>

namespace lume::math::detail::tuple_storage
{

template <class T>
class Pointer
{
public:
    using value_type = T;

    Pointer (T* data, size_t const)
        : m_data (data)
    {}

    T*       data ()       {return m_data;}
    T const* data () const {return m_data;}

private:
    T* const m_data;
};

template <class T>
class ConstPointer
{
public:
    using value_type = T;

    ConstPointer (T const* data)
        : m_data (data)
    {}

    T const* data () const {return m_data;}

private:
    T const* const m_data;
};

template <class T>
struct Array
{
    using value_type = T;

    static constexpr size_t maxSize = 4;

    Array () = default;

    Array (T const* data, size_t const size)
        : m_data (data)
    {
        assert (size < m_data.size ());
        lume::math::raw::VecCopy (m_data.data (), size, data);
    }

//todo: constructors
    T*       data ()       {return m_data.data ();}
    T const* data () const {return m_data.data ();}
    
private:
    std::array <T, maxSize> m_data;
};

}

namespace lume::math::detail
{

template <class Storage>
class TupleTemplate
{
public:
    using value_type = typename Storage::value_type;
    using array_storage_t = tuple_storage::Array <value_type>;

    size_t size () const {return m_size;}

    static TupleTemplate <tuple_storage::Array <value_type>>
    uninitialized (size_t const size)
    {
        assert (size < array_storage_t::maxSize);
        return TupleTemplate <array_storage_t> (size);
    }

    template <class Array>
    TupleTemplate (Array& t)
        : m_storage (t.m_data ())
        , m_size (t.size ())
    {}

    TupleTemplate (value_type* data, size_t const size)
        : m_storage (data, size)
        , m_size (size)
    {}

    template <class T>
    TupleTemplate (T* data, size_t const size)
        : m_storage (data, size)
        , m_size (size)
    {}

    template <class Array>
    TupleTemplate& operator = (Array const& v);

    template <class Array>
    TupleTemplate& operator += (Array const& v);

    template <class Array>
    TupleTemplate& operator -= (Array const& v);

    template <class Array>
    TupleTemplate& operator *= (Array const& v);

    template <class Array>
    TupleTemplate& operator /= (Array const& v);

    template <class Array>
    bool operator == (Array const& v) const;

    template <class Array>
    bool operator != (Array const& v) const;

    value_type& operator [] (size_t const i)         {return data () [i];}
    value_type  operator [] (size_t const i) const   {return data () [i];}

    value_type*       data ()                        {return m_storage.data ();}
    value_type const* data () const                  {return m_storage.data ();}

private:
    TupleTemplate () = default;
    
    TupleTemplate (size_t const size)
        : m_size (size)
    {}

private:
    Storage m_storage;
    size_t const m_size;
};

template <class T>
class ConstTupleTemplate
{
public:
    using const_pointer_storage_t = detail::tuple_storage::ConstPointer <T>;
    using value_type = typename const_pointer_storage_t::value_type;

    size_t size () const {return m_size;}

    ConstTupleTemplate () = delete;

    template <class Array>
    ConstTupleTemplate (Array const& t)
        : ConstTupleTemplate (t.data (), t.size ())
    {}

    ConstTupleTemplate (value_type const* data, size_t const size)
        : m_storage (data)
        , m_size (size)
    {}

    template <class Array>
    bool operator == (Array const& v) const;

    template <class Array>
    bool operator != (Array const& v) const;

    value_type operator [] (size_t const i) const    {return data () [i];}

    value_type const* data () const                  {return m_storage.data ();}

private:
    const_pointer_storage_t m_storage;
    size_t const m_size;
};

}

namespace lume::math
{

/** Tuple and ConstTuple are temporary wrappers around a given memory region in which a given number
    of POD types are stored contiguously.
    \warning Make sure that the underlying memory region is valid as long as the associated tuple exists.
    \{
*/
template <class T>
using Tuple            = detail::TupleTemplate <detail::tuple_storage::Pointer<T>>;

template <class T>
using ConstTuple       = detail::ConstTupleTemplate <T>;
/** \} */

template <class T>
using TupleWithStorage = detail::TupleTemplate <detail::tuple_storage::Array<T>>;

template <class TupleA, class TupleB>
TupleWithStorage <typename TupleA::value_type>
operator + (TupleA const& a, TupleB const& b)
{
    assert (a.size () == b.size ());
    auto r = TupleWithStorage <typename TupleA::value_type>::uninitialized (a.size ());
    for(size_t i = 0; i < a.size (); ++i) {
        r [i] = a[i] + b[i];
    }
    return r;
}

template <class TupleA, class TupleB>
TupleWithStorage <typename TupleA::value_type>
operator - (TupleA const& a, TupleB const& b)
{
    assert (a.size () == b.size ());
    auto r = TupleWithStorage <typename TupleA::value_type>::uninitialized (a.size ());
    for(size_t i = 0; i < a.size (); ++i) {
        r [i] = a[i] - b[i];
    }
    return r;
}

template <class Array>
TupleWithStorage <typename Array::value_type>
operator * (const typename Array::value_type s, Array const& a)
{
    auto r = TupleWithStorage <typename Array::value_type>::uninitialized (a.size ());
    for(size_t i = 0; i < a.size (); ++i) {
        r [i] = s * a[i];
    }
    return r;
}

template <class Array>
TupleWithStorage <typename Array::value_type>
operator * (Array const& a, const typename Array::value_type s)
{
    return s * a;
}

template <class Array>
TupleWithStorage <typename Array::value_type>
operator / (Array const& a, const typename Array::value_type s)
{
    auto r = TupleWithStorage <typename Array:: value_type>:: uninitialized (a.size ());
    for(size_t i = 0; i < a.size (); ++i) {
        r [i] = a[i] / s;
    }
    return r;
}

template <class TupleA, class TupleB>
typename TupleA::value_type
Dot (TupleA const& a, TupleB const& b)
{
    assert (a.size () == b.size ());
    typename TupleA::value_type d = 0;
    size_t const size = a.size ();
    
    for(size_t i = 0; i < size; ++i) {
        d += a [i] * b [i];
    }

    return d;
}

template <class Tuple>
typename Tuple::value_type
LengthSquared (Tuple const& a)
{
    return Dot (a, a);
}

template <class Tuple>
typename Tuple::value_type
Length (Tuple const& a)
{
    return sqrt (LengthSquared (a));
}

template <class TupleA, class TupleB>
typename TupleA::value_type
DistanceSquared (TupleA const& a, TupleB const& b)
{
    return LengthSquared (b - a);
}

template <class TupleA, class TupleB>
typename TupleA::value_type
Distance (TupleA const& a, TupleB const& b)
{
    return sqrt (DistanceSquared (a, b));
}

template <class Tuple>
TupleWithStorage <typename Tuple::value_type>
Normalized (Tuple const& a)
{
    auto const len = Length (a);
    if (len == 0) {
        return a * 0;
    }

    return a / len;
}

}// end of namespace lume::math

#include <lume/math/tuple_impl.h>
