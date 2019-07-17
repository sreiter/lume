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

#include <lume/math/vector_math.h>
#include <lume/math/tuple.h>
#include <lume/math/raw/vector_math_raw.h>

namespace lume::math
{

template <class T>
class TupleView
{
public:
    using value_type = T;
    using value_t = value_type;

    template <class Container>
    TupleView (Container& container)
        : TupleView (container.data (), container.num_tuples (), container.tuple_size ())
    {}

    TupleView (T* data, size_t const numTuples, size_t const tupleSize)
        : m_data (data)
        , m_numTuples (numTuples)
        , m_tupleSize (tupleSize)
    {}

    Tuple <T>      operator [] (size_t const itup)       {return Tuple <T> (m_data + m_tupleSize * itup, m_tupleSize);}
    ConstTuple <T> operator [] (size_t const itup) const {return Tuple <T> (m_data + m_tupleSize * itup, m_tupleSize);}

    T const* data ()           const {return m_data;}
    size_t   size ()           const {return m_numTuples;}
    size_t   tuple_size ()     const {return m_tupleSize;}
    size_t   num_components () const {return size () * tuple_size ();}

    TupleView& operator = (T const v) 
    {
        lume::math::raw::VecSet (m_data, num_components (), v);
        return *this;
    }
    
    TupleView& normalize ()
    {
        lume::math::raw::VecTupNormalizeInplace (m_data, num_components (), tuple_size ());
        return *this;
    }

private:
    T*     const m_data;
    size_t const m_numTuples;
    size_t const m_tupleSize;
};

template <class T>
class ConstTupleView
{
public:
    using value_type = T;
    using value_t = value_type;

    template <class Container>
    ConstTupleView (Container& container)
        : ConstTupleView (container.data (), container.num_tuples (), container.tuple_size ())
    {}

    ConstTupleView (T const* data, size_t const numTuples, size_t const tupleSize)
        : m_data (data)
        , m_numTuples (numTuples)
        , m_tupleSize (tupleSize)
    {}

    ConstTupleView (const TupleView <T>& t)
        : m_data (t.data ())
        , m_numTuples (t.size ())
        , m_tupleSize (t.tuple_size ())
    {}

    ConstTuple <T> operator [] (size_t const itup) const {return ConstTuple <T> (m_data + m_tupleSize * itup, m_tupleSize);}

    T const* data ()           const {return m_data;}
    size_t   size ()           const {return m_numTuples;}
    size_t   tuple_size ()     const {return m_tupleSize;}
    size_t   num_components () const {return size () * tuple_size ();}
    
private:
    T const* const m_data;
    size_t   const m_numTuples;
    size_t   const m_tupleSize;
};

template <class Container>
TupleView <typename Container::value_type>
MakeTupleView (Container& c)
{
    return TupleView <typename Container::value_type> (c.data (), c.num_tuples (), c.tuple_size ());
}

template <class Container>
ConstTupleView <typename Container::value_type>
MakeTupleView (Container const& c)
{
    return ConstTupleView <typename Container::value_type> (c.data (), c.num_tuples (), c.tuple_size ());
}

}// end of namespace lume::math
