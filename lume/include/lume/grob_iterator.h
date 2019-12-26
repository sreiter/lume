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


#ifndef __H__lume_grob_iterator
#define __H__lume_grob_iterator

#include "grob.h"
#include <iterator>

namespace lume {

template <class Grob>
class GenericGrobIterator
{
public:
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = Grob;
	using difference_type = std::ptrdiff_t;
	using pointer = Grob*;
	using reference = Grob&;

  GenericGrobIterator () = default;
  
  template <class GrobIterator>
  GenericGrobIterator (GrobIterator const& other)
   : m_grob (*other)
   , m_numCorners (other->num_corners ())
  {}

	GenericGrobIterator (GrobType grobType, index_t* globalCornerArray) :
		m_grob (grobType, globalCornerArray),
		m_numCorners (m_grob.num_corners ())
	{}

  GenericGrobIterator (GrobType grobType, index_t const* globalCornerArray) :
    m_grob (grobType, globalCornerArray),
    m_numCorners (m_grob.num_corners ())
  {}

  GenericGrobIterator& operator = (GenericGrobIterator const& other)
  {
    m_grob.reset (other.m_grob);
    m_numCorners = other.m_numCorners;
    return *this;
  }

	Grob& operator * ()
	{
		return m_grob;
	}

  Grob const& operator * () const
  {
    return m_grob;
  }

	Grob* operator -> ()
	{
		return &m_grob;
	}

  Grob const* operator -> () const
  {
    return &m_grob;
  }

	bool operator == (const GenericGrobIterator& i) const
	{
		return	m_grob.global_corner_array () == i.m_grob.global_corner_array ()
				&&	m_grob.grob_type () == i.m_grob.grob_type ();
	}

	bool operator != (const GenericGrobIterator& i) const
	{
		return	m_grob.global_corner_array () != i.m_grob.global_corner_array ()
				||	m_grob.grob_type () != i.m_grob.grob_type ();
	}

	GenericGrobIterator& operator += (difference_type n)
	{
		m_grob.set_global_corner_array (shifted_corner_array (n));
		return *this;
	}

	GenericGrobIterator& operator -= (difference_type n)
	{
		m_grob.set_global_corner_array (shifted_corner_array (-n));
		return *this;
	}

	GenericGrobIterator operator + (difference_type n) const
	{
		return GenericGrobIterator (m_grob.grob_type(), shifted_corner_array (n));
	}

	GenericGrobIterator operator - (difference_type n) const
	{
		return GenericGrobIterator (m_grob.grob_type(), shifted_corner_array (-n));
	}

	GenericGrobIterator& operator ++ ()
	{
		m_grob.set_global_corner_array (shifted_corner_array (1));
		return *this;
	}

	GenericGrobIterator operator ++ (int)
	{
		GenericGrobIterator i = *this;
		m_grob.set_global_corner_array (shifted_corner_array (1));
		return i;
	}

	GenericGrobIterator& operator -- ()
	{
		m_grob.set_global_corner_array (shifted_corner_array (-1));
		return *this;
	}

	GenericGrobIterator operator -- (int)
	{
		GenericGrobIterator i = *this;
		m_grob.set_global_corner_array (shifted_corner_array (-1));
		return i;
	}

private:
  /** Note: This method always returns a non-const index array, even for const types.
            The resulting array is however never used for manipulation and is instead
            re-cast to the constness of the underlying type by all callers
            The reason for this hack is to allow one iterator implementation for both
            `Grob` and `ConstGrob`.*/
	index_t* shifted_corner_array (difference_type n) const
	{
		return const_cast <index_t*> (m_grob.global_corner_array ()) + n * m_numCorners;
	}

	Grob m_grob;
	index_t m_numCorners;
};

using GrobIterator = GenericGrobIterator <Grob>;
using ConstGrobIterator = GenericGrobIterator <ConstGrob>;

}//	end of namespace lume

#endif	//__H__lume_grob_iterator
