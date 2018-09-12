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

#ifndef __H__lume_array_iterator
#define __H__lume_array_iterator

#include <iterator>

namespace lume {

////////////////////////////////////////////////////////////////////////////////
/// An iterator for array types which provide element access through `RefT operator [] (index_t)`
template <class ArrayT,
		  class ValueT,
		  class IndexT,
		  class PtrT = ValueT*,
		  class RefT = ValueT&,
		  class PtrDiffT = std::ptrdiff_t
		  >
class ArrayIterator {
public:
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = ValueT;
	using difference_type = PtrDiffT;
	using pointer = PtrT*;
	using reference = RefT;

	ArrayIterator (ArrayT& array, IndexT index)			: m_array (array), m_index (index) {}

	reference operator * ()								{return m_array[m_index];}
	const reference operator * () const					{return m_array[m_index];}

	reference operator -> ()							{return m_array[m_index];}
	const reference operator -> () const				{return m_array[m_index];}

	bool operator == (const ArrayIterator& i) const		{return (m_index == i.m_index) && (&m_array == &i.m_array);}
	bool operator != (const ArrayIterator& i) const		{return (m_index != i.m_index) || (&m_array != &i.m_array);}

	ArrayIterator& operator += (difference_type n)		{m_index += n; return *this;}
	ArrayIterator& operator -= (difference_type n)		{m_index -= n; return *this;}

	ArrayIterator operator + (difference_type n) const	{return ArrayIterator (m_array, m_index + n);}
	ArrayIterator operator - (difference_type n) const	{return ArrayIterator (m_array, m_index - n);}

	ArrayIterator& operator ++ ()						{++m_index; return *this;}
	ArrayIterator operator ++ (int)						{ArrayIterator i = *this; ++m_index; return i;}

	ArrayIterator& operator -- ()						{--m_index; return *this;}
	ArrayIterator operator -- (int)						{ArrayIterator i = *this; --m_index; return i;}

private:
	ArrayT& m_array;
	IndexT	m_index;
};

////////////////////////////////////////////////////////////////////////////////
/// A const iterator for array types which provide element access through `RefT operator [] (index_t) const`
template <class ArrayT,
		  class ValueT,
		  class IndexT,
		  class PtrT = const ValueT*,
		  class RefT = const ValueT&,
		  class PtrDiffT = std::ptrdiff_t
		  >
class ConstArrayIterator {
public:
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = ValueT;
	using difference_type = PtrDiffT;
	using pointer = PtrT*;
	using reference = RefT;

	ConstArrayIterator (const ArrayT& array, IndexT index)	: m_array (array), m_index (index) {}

	reference operator * () const							{return m_array[m_index];}

	reference operator -> () const							{return m_array[m_index];}

	bool operator == (const ConstArrayIterator& i) const	{return (m_index == i.m_index) && (&m_array == &i.m_array);}
	bool operator != (const ConstArrayIterator& i) const	{return (m_index != i.m_index) || (&m_array != &i.m_array);}

	ConstArrayIterator& operator += (difference_type n)		{m_index += n; return *this;}
	ConstArrayIterator& operator -= (difference_type n)		{m_index -= n; return *this;}

	ConstArrayIterator operator + (difference_type n) const	{return ConstArrayIterator (m_array, m_index + n);}
	ConstArrayIterator operator - (difference_type n) const	{return ConstArrayIterator (m_array, m_index - n);}

	ConstArrayIterator& operator ++ ()						{++m_index; return *this;}
	ConstArrayIterator operator ++ (int)					{ConstArrayIterator i = *this; ++m_index; return i;}

	ConstArrayIterator& operator -- ()						{--m_index; return *this;}
	ConstArrayIterator operator -- (int)					{ConstArrayIterator i = *this; --m_index; return i;}

private:
	const ArrayT& m_array;
	IndexT	m_index;
};

}//	end of namespace lume

#endif	//__H__lume_array_iterator
