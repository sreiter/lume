// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2018 Sebastian Reiter <s.b.reiter@gmail.com>

#ifndef __H__lume_grob_iterator
#define __H__lume_grob_iterator

#include "grob.h"
#include <iterator>

namespace lume {

class GrobIterator {
public:
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = Grob;
	using difference_type = std::ptrdiff_t;
	using pointer = const Grob*;
	using reference = const Grob&;

	GrobIterator (grob_t grobType, const index_t* globalCornerArray) :
		m_grob (grobType, globalCornerArray),
		m_numCorners (m_grob.num_corners ())
	{}

	reference operator * () const
	{
		return m_grob;
	}

	bool operator == (const GrobIterator& i) const
	{
		return	m_grob.global_corner_array () == i.m_grob.global_corner_array ()
				&&	m_grob.grob_type () == i.m_grob.grob_type ();
	}

	bool operator != (const GrobIterator& i) const
	{
		return	m_grob.global_corner_array () != i.m_grob.global_corner_array ()
				||	m_grob.grob_type () != i.m_grob.grob_type ();
	}

	GrobIterator& operator += (difference_type n)
	{
		m_grob.set_global_corner_array (shifted_corner_array (n));
		return *this;
	}

	GrobIterator& operator -= (difference_type n)
	{
		m_grob.set_global_corner_array (shifted_corner_array (-n));
		return *this;
	}

	GrobIterator operator + (difference_type n) const
	{
		return GrobIterator (m_grob.grob_type(), shifted_corner_array (n));
	}

	GrobIterator operator - (difference_type n) const
	{
		return GrobIterator (m_grob.grob_type(), shifted_corner_array (-n));
	}

	GrobIterator& operator ++ ()
	{
		m_grob.set_global_corner_array (shifted_corner_array (1));
		return *this;
	}

	GrobIterator operator ++ (int)
	{
		GrobIterator i = *this;
		m_grob.set_global_corner_array (shifted_corner_array (1));
		return i;
	}

	GrobIterator& operator -- ()
	{
		m_grob.set_global_corner_array (shifted_corner_array (-1));
		return *this;
	}

	GrobIterator operator -- (int)
	{
		GrobIterator i = *this;
		m_grob.set_global_corner_array (shifted_corner_array (-1));
		return i;
	}

private:
	const index_t* shifted_corner_array (difference_type n) const
	{
		return m_grob.global_corner_array () + n * m_numCorners;
	}

	Grob m_grob;
	index_t m_numCorners;
};

}//	end of namespace lume

#endif	//__H__lume_grob_iterator
