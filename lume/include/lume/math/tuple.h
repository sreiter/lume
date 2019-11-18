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

#include <lume/math/raw/vector_math_raw.h>

namespace lume::math::detail::tuple_storage
{

template <class T>
class Pointer
{
public:
    using value_type     = T;
    using ptr_type       = T*;
    using const_ptr_type = const T*;
    using ref_type       = T&;

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
    using value_type     = T;
    using ptr_type       = const T*;
    using const_ptr_type = const T*;
    using ref_type       = const T&;

    ConstPointer (T const* data, size_t const)
        : m_data (data)
    {}

    T const* data () const {return m_data;}

private:
    T const* const m_data;
};

template <class T>
struct Array
{
    using value_type     = T;
    using ptr_type       = T*;
    using const_ptr_type = const T*;
    using ref_type       = T&;

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
    using value_type           = typename Storage::value_type;
    using ptr_type             = typename Storage::ptr_type;
    using const_ptr_type       = typename Storage::const_ptr_type;
    using ref_type             = typename Storage::ref_type;
    using array_storage_t      = tuple_storage::Array <value_type>;
    using tuple_with_storage_t = TupleTemplate <array_storage_t>;

    size_t size () const {return m_size;}

    static TupleTemplate <tuple_storage::Array <value_type>>
    uninitialized (size_t const size)
    {
        assert (size < array_storage_t::maxSize);
        return tuple_with_storage_t (size);
    }

    template <class Array>
    TupleTemplate (Array& t)
        : m_storage (t.data ())
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

    TupleTemplate& operator = (value_type const& v);
    
    template <class Array>
    TupleTemplate& operator = (Array const& v);

    template <class Array>
    TupleTemplate& operator += (Array const& v);

    template <class Array>
    TupleTemplate& operator -= (Array const& v);

    TupleTemplate& operator *= (value_type const& v);

    TupleTemplate& operator /= (value_type const& v);

    template <class Array>
    bool operator == (Array const& v) const;

    template <class Array>
    bool operator != (Array const& v) const;

    ref_type   operator [] (size_t const i)       {return data () [i];}
    value_type operator [] (size_t const i) const {return data () [i];}

    ref_type   value (size_t const i)             {return data () [i];}
    value_type value (size_t const i) const       {return data () [i];}

    ptr_type       data ()                        {return m_storage.data ();}
    const_ptr_type data () const                  {return m_storage.data ();}

    template <class Array>
    value_type dot (Array const& v) const;

    value_type length_squared () const;

    value_type length () const;

    template <class Array>
    value_type distance_squared (Array const& v) const;

    template <class Array>
    value_type distance (Array const& v) const;

    tuple_with_storage_t normalized () const;

    TupleTemplate& normalize ();

private:
    TupleTemplate () = default;
    
    TupleTemplate (size_t const size)
        : m_size (size)
    {}

private:
    Storage m_storage;
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
using Tuple            = detail::TupleTemplate <detail::tuple_storage::Pointer <T>>;

template <class T>
using ConstTuple       = detail::TupleTemplate <detail::tuple_storage::ConstPointer <T>>;
/** \} */

template <class T>
using TupleWithStorage = detail::TupleTemplate <detail::tuple_storage::Array <T>>;

}

template <class StorageA, class StorageB>
lume::math::TupleWithStorage <typename StorageA::value_type>
operator + (lume::math::detail::TupleTemplate <StorageA> const& a,
            lume::math::detail::TupleTemplate <StorageB> const& b);

template <class StorageA, class StorageB>
lume::math::TupleWithStorage <typename StorageA::value_type>
operator - (lume::math::detail::TupleTemplate <StorageA> const& a,
            lume::math::detail::TupleTemplate <StorageB> const& b);

template <class Storage>
lume::math::TupleWithStorage <typename Storage::value_type>
operator * (const typename Storage::value_type s,
            lume::math::detail::TupleTemplate <Storage> const& a);

template <class Storage>
lume::math::TupleWithStorage <typename Storage::value_type>
operator * (lume::math::detail::TupleTemplate <Storage> const& a,
            const typename Storage::value_type s);

template <class Storage>
lume::math::TupleWithStorage <typename Storage::value_type>
operator / (lume::math::detail::TupleTemplate <Storage> const& a,
            const typename Storage::value_type s);

#include <lume/math/tuple_impl.h>
