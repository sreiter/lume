// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2018 Sebastian Reiter <s.b.reiter@gmail.com>

#ifndef __H__lume_grob_array
#define __H__lume_grob_array

#include "grob.h"
#include "grob_iterator.h"
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

namespace lume {

DECLARE_CUSTOM_EXCEPTION (BadNumberOfIndices, LumeError);

class GrobArray {
public:
	using iterator = GrobIterator;
	using const_iterator = GrobIterator;

	GrobArray () :
		m_grobDesc (VERTEX),
		m_array (m_grobDesc.num_corners ())
	{}

	GrobArray (grob_t grobType) :
		m_grobDesc (grobType),
		m_array (m_grobDesc.num_corners ())
	{}

	inline void clear ()					{m_array.clear();}

	bool empty() const 						{return m_array.empty();}

	/// total number of grobs
	inline index_t size () const			{return m_array.num_tuples ();}
	inline index_t num_indices () const		{return m_array.size ();}

	inline index_t* raw_ptr() 				{return m_array.raw_ptr();}
	inline const index_t* raw_ptr () const 	{return m_array.raw_ptr();}

	inline void resize (const index_t s)					{m_array.resize (s * num_grob_corners());}
	inline void resize (const index_t s, const index_t v)	{m_array.resize (s * num_grob_corners(), v);}
	inline void reserve (const index_t s)					{m_array.reserve (s * num_grob_corners());}

	inline void push_back (std::initializer_list <index_t> inds)
	{
		using std::string;
		using std::to_string;

		if (inds.size() != num_grob_corners())
			throw BadNumberOfIndices (string ("expected: ").
			                          append (to_string (num_grob_corners())).
			                          append (", given: ").
			                          append (to_string (inds.size())));

		for(auto i : inds) 
			m_array.push_back (i);
	}

	inline void push_back (const Grob& grob)
	{
		using std::string;
		using std::to_string;

		if (grob.num_corners() != num_grob_corners())
			throw BadNumberOfIndices (string ("expected: ").
			                          append (to_string (num_grob_corners())).
			                          append (", given: ").
			                          append (to_string (grob.num_corners())));

		for(index_t i = 0; i < grob.num_corners(); ++i)
			m_array.push_back (grob.corner(i));
	}

	inline const Grob operator [] (const index_t i) const	{return Grob (m_grobDesc.grob_type(), m_array.raw_ptr () + i * num_grob_corners());}

	inline iterator begin () const						{return GrobIterator (m_grobDesc.grob_type(), m_array.raw_ptr());}
	inline iterator end () const						{return GrobIterator (m_grobDesc.grob_type(), m_array.raw_ptr() + m_array.size());}

	GrobDesc grob_desc () const							{return m_grobDesc;}

	IndexArrayAnnex& underlying_array ()				{return m_array;}
	const IndexArrayAnnex& underlying_array () const	{return m_array;}

private:
	inline index_t num_grob_corners () const			{return m_grobDesc.num_corners();}
	GrobDesc				m_grobDesc;
	IndexArrayAnnex			m_array;
};

using SPGrobArray	= std::shared_ptr <GrobArray>;
using CSPGrobArray	= std::shared_ptr <const GrobArray>;

}//	end of namespace lume

#endif	//__H__lume_grob_array
