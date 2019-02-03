// This file is part of lume, a C++ library for lightweight unstructured meshes
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


#ifndef __H__lume_data_buffer
#define __H__lume_data_buffer

#include "annex.h"
#include "tuple_vector.h"

namespace lume {

template <class T>
class ArrayAnnex : public Annex {
public:
	using value_type = T;
	using value_t = value_type;
	using size_type = typename TupleVector<T>::size_type;
	using iterator = typename TupleVector<T>::iterator;
	using const_iterator = typename TupleVector<T>::const_iterator;

	ArrayAnnex ()
        : m_vector (1)
    {}

	ArrayAnnex (const size_type tupleSize)
        : m_vector (tupleSize)
    {}

    ArrayAnnex (const size_type tupleSize, const size_type numTuples)
        : m_vector (tupleSize, numTuples)
    {}

    ArrayAnnex (ArrayAnnex&& aa)
        : m_vector (std::move (aa.m_vector))
    {}

    ArrayAnnex (const size_type tupleSize, std::vector <T>&& vec)
        : m_vector (tupleSize, std::move (vec))
    {}

    ArrayAnnex (TupleVector <T>&& vec)
        : m_vector (std::move (vec))
    {}

	const char* class_name () const override	{return "ArrayAnnex";}

    bool empty() const { return m_vector.empty(); }

    /// total number of entries, counting individual components
    inline size_type size () const            {return m_vector.size ();}

    inline size_type num_tuples () const      {return m_vector.num_tuples ();}

    /// number of individual components making up a tuple
    inline size_type tuple_size () const       {return m_vector.tuple_size ();}

    inline T* data()                        {m_vector.data ();}
    inline const T* data () const           {return m_vector.data ();}

	inline T& operator [] (const size_type i)				{return m_vector[i];}
	inline const T& operator [] (const size_type i) const	{return m_vector[i];}

	inline T& at (const size_type i)					{return m_vector.at(i);}
	inline const T& at (const size_type i) const		{return m_vector.at(i);}

	inline T& back ()						{return m_vector.back();}
	inline const T& back () const			{return m_vector.back();}

	inline iterator begin ()				{return m_vector.begin();}
	inline iterator end ()					{return m_vector.end();}
	inline const_iterator begin () const	{return m_vector.begin();}
	inline const_iterator end () const		{return m_vector.end();}

private:
	TupleVector <T>	m_vector;
};

using RealArrayAnnex		= ArrayAnnex <real_t>;
using IndexArrayAnnex		= ArrayAnnex <index_t>;

}//	end of namespace lume

#endif	//__H__lume_data_buffer
