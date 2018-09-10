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

#include <iterator>
#include <memory>
#include <vector>
#include "unpack.h"
#include "types.h"
#include "annex.h"
#include "custom_exception.h"

namespace lume {

DECLARE_CUSTOM_EXCEPTION (BadTupleSizeError, AnnexError);

template <class T>
class ArrayAnnex : public Annex {
public:
	using value_type = T;
	using value_t = value_type;
	using size_type = index_t;
	using iterator = typename std::vector<T>::iterator;
	using const_iterator = typename std::vector<T>::const_iterator;

	ArrayAnnex ()	: m_tupleSize (1) {}
	ArrayAnnex (const index_t tupleSize) : m_tupleSize (tupleSize) {}

	const char* class_name () const override	{return "ArrayAnnex";}

	inline void clear ()					{m_vector.clear();}

	bool empty() const { return m_vector.empty(); }

	/// total number of entries, counting individual components
	inline index_t size () const			{return static_cast<index_t>(m_vector.size());}

	inline index_t num_tuples () const		{return size() / tuple_size();}

	/// number of individual components making up a tuple
	inline index_t tuple_size () const				{return m_tupleSize;}
	inline void set_tuple_size (const index_t ts)	{m_tupleSize = ts;}

	inline T* raw_ptr() { if (size()) return &m_vector[0]; return NULL; }
	inline const T* raw_ptr () const { if (size()) return &m_vector[0]; return NULL; }

	inline void resize (const index_t s)				{m_vector.resize (s);}
	inline void resize (const index_t s, const T& v)	{m_vector.resize (s, v);}
	inline void reserve (const index_t s)				{m_vector.reserve (s);}

	inline void push_back (const T& v)					{m_vector.push_back (v);}

	inline T& operator [] (const index_t i)				{return m_vector[i];}
	inline const T& operator [] (const index_t i) const	{return m_vector[i];}

	inline T& at (const index_t i)					{return m_vector.at(i);}
	inline const T& at (const index_t i) const		{return m_vector.at(i);}

	inline T& back ()						{return m_vector.back();}
	inline const T& back () const			{return m_vector.back();}

	inline iterator begin ()				{return m_vector.begin();}
	inline iterator end ()					{return m_vector.end();}
	inline const_iterator begin () const	{return m_vector.begin();}
	inline const_iterator end () const		{return m_vector.end();}

private:
	std::vector <T>	m_vector;
	index_t			m_tupleSize;
};


using RealArrayAnnex		= ArrayAnnex <real_t>;
using IndexArrayAnnex		= ArrayAnnex <index_t>;

using SPRealArrayAnnex		= std::shared_ptr <RealArrayAnnex>;
using SPIndexArrayAnnex		= std::shared_ptr <IndexArrayAnnex>;
using CSPRealArrayAnnex		= std::shared_ptr <const RealArrayAnnex>;
using CSPIndexArrayAnnex	= std::shared_ptr <const IndexArrayAnnex>;

}//	end of namespace lume

#endif	//__H__lume_data_buffer
