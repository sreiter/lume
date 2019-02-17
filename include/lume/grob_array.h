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


#ifndef __H__lume_grob_array
#define __H__lume_grob_array

#include "grob.h"
#include "grob_iterator.h"
#include "tuple_vector.h"
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
    using size_type = TupleVector<index_t>::size_type;

    GrobArray (GrobArray&& grobArray)
        : m_grobDesc (std::move (grobArray.m_grobDesc))
        , m_array (std::move (grobArray.m_array))
    {}

	GrobArray (GrobType grobType) :
		m_grobDesc (grobType),
		m_array (m_grobDesc.num_corners ())
	{}

    GrobArray (GrobType grobType, std::vector <index_t>&& inds) :
        m_grobDesc (grobType),
        m_array (m_grobDesc.num_corners (), std::move (inds))
    {}

    GrobArray (GrobType grobType, TupleVector <index_t>&& inds) :
        m_grobDesc (grobType),
        m_array (std::move (inds))
    {}

	inline void clear ()					{m_array.clear();}

	bool empty() const 						{return m_array.empty();}

	/// total number of grobs
	inline size_type size () const			{return m_array.num_tuples ();}
	inline size_type num_indices () const	{return m_array.size ();}

	inline index_t* data() 				    {return m_array.data();}
	inline const index_t* data () const 	{return m_array.data();}

	inline void resize (const size_type s)					{m_array.resize (s * m_array.tuple_size ());}
	inline void resize (const size_type s, const index_t v)	{m_array.resize (s * m_array.tuple_size (), v);}
	inline void reserve (const size_type s)					{m_array.reserve (s * m_array.tuple_size ());}

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

	inline const Grob operator [] (const size_type i) const	{return Grob (m_grobDesc.grob_type(), m_array.data () + i * num_grob_corners());}

	inline iterator begin () const						{return GrobIterator (m_grobDesc.grob_type(), m_array.data());}
	inline iterator end () const						{return GrobIterator (m_grobDesc.grob_type(), m_array.data() + m_array.size());}

	GrobDesc grob_desc () const							{return m_grobDesc;}

	TupleVector <index_t>& underlying_array ()				{return m_array;}
	const TupleVector <index_t>& underlying_array () const	{return m_array;}

private:
	inline index_t num_grob_corners () const			{return m_grobDesc.num_corners();}
	GrobDesc				m_grobDesc;
	TupleVector <index_t>	m_array;
};

using SPGrobArray	= std::shared_ptr <GrobArray>;
using CSPGrobArray	= std::shared_ptr <const GrobArray>;

}//	end of namespace lume

#endif	//__H__lume_grob_array
