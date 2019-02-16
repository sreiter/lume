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

#ifndef __H__lume_tuple_vector
#define __H__lume_tuple_vector

#include <cassert>
#include <iterator>
#include <vector>
#include "unpack.h"
#include "types.h"
#include "annex.h"
#include "custom_exception.h"

namespace lume {

DECLARE_CUSTOM_EXCEPTION (BadTupleSizeError, LumeError);

template <class T>
class TupleVector {
public:
    using value_type = T;
    using value_t = value_type;
    using size_type = size_t;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    TupleVector ()
        : m_tupleSize (1)
    {}

    TupleVector (const size_type tupleSize)
        : m_tupleSize (tupleSize)
    {
        assert (tupleSize != 0);
    }

    TupleVector (const size_type tupleSize, const size_type numTuples)
        : m_vector (tupleSize * numTuples)
        , m_tupleSize (tupleSize)
    {
        assert (tupleSize != 0);
    }

    TupleVector (TupleVector&& tv)
        : m_vector (std::move (tv.m_vector))
        , m_tupleSize (tv.m_tupleSize)
    {}

    TupleVector (const size_type tupleSize, std::vector <T>&& vec)
        : m_vector (std::move (vec))
        , m_tupleSize (tupleSize)
    {
        assert (tupleSize != 0);
        const size_t numTuples = vec.size () / m_tupleSize;
        if (m_vector.size () != m_tupleSize * numTuples)
            m_vector.resize (m_tupleSize * numTuples);
    }

    bool empty() const                      { return m_vector.empty(); }

    /// total number of entries, counting individual components
    inline size_type size () const            {return m_vector.size();}

    inline size_type num_tuples () const      {return size() / tuple_size();}

    /// number of individual components making up a tuple
    inline size_type tuple_size () const        {return m_tupleSize;}

    inline T* data()                            {return m_vector.data ();}
    inline const T* data () const               {return m_vector.data ();}

    inline void clear ()                                  {m_vector.clear();}
    inline void resize (const size_type s)                {m_vector.resize (s);}
    inline void resize (const size_type s, const T& v)    {m_vector.resize (s, v);}
    inline void set_num_tuples (const size_type num)      {m_vector.resize (num * tuple_size ());}
    inline void reserve (const size_type s)               {m_vector.reserve (s);}

    inline T& operator [] (const size_type i)             {return m_vector[i];}
    inline const T& operator [] (const size_type i) const {return m_vector[i];}

    inline T& at (const size_type i)                  {return m_vector.at(i);}
    inline const T& at (const size_type i) const      {return m_vector.at(i);}

    inline T& back ()                       {return m_vector.back();}
    inline const T& back () const           {return m_vector.back();}

    inline iterator begin ()                {return m_vector.begin();}
    inline iterator end ()                  {return m_vector.end();}
    inline const_iterator begin () const    {return m_vector.begin();}
    inline const_iterator end () const      {return m_vector.end();}

    inline void push_back (const T& t)      {m_vector.push_back (t);}

private:
    std::vector <T> m_vector;
    size_type       m_tupleSize;
};

}//    end of namespace lume

#endif    //__H__lume_tuple_vector
