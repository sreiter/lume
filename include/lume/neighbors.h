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


#ifndef __H__lume_neighbors
#define __H__lume_neighbors

#include "array_iterator.h"
#include "grob_index.h"

namespace lume {

class Neighborhoods;

////////////////////////////////////////////////////////////////////////////////
class NeighborIndices {
public:
	using iterator_t = ConstArrayIterator <NeighborIndices, GrobIndex, index_t, GrobIndex*, GrobIndex>;
	using const_iterator_t = iterator_t;

	NeighborIndices (const GrobIndex& grobIndex,
			   		 const Neighborhoods* neighborhoods) :
		m_grobIndex (grobIndex),
		m_neighborhoods (neighborhoods)
	{}
	
	const GrobIndex& central_grob_index () const	{return m_grobIndex;}

	index_t size () const;
	GrobIndex operator [] (const index_t i) const	{return neighbor (i);}
	GrobIndex neighbor (const index_t i) const;

	const_iterator_t begin() const					{return iterator_t (*this, 0);}
	const_iterator_t end() const					{return iterator_t (*this, size());}

	const Neighborhoods* neighborhoods() const		{return m_neighborhoods;}

private:
	const GrobIndex			m_grobIndex;
	const Neighborhoods*	m_neighborhoods;
};


////////////////////////////////////////////////////////////////////////////////
class NeighborGrobs {
public:
	using iterator_t = ConstArrayIterator <NeighborGrobs, Grob, index_t, Grob*, Grob>;
	using const_iterator_t = iterator_t;

	NeighborGrobs (const GrobIndex& grobIndex,
			   	   const Neighborhoods* neighborhoods) :
		m_nbrInds (grobIndex, neighborhoods)
	{}

	NeighborGrobs (const NeighborIndices& nbrInds) :
		m_nbrInds (nbrInds)
	{}
	
	const GrobIndex& central_grob_index () const		{return m_nbrInds.central_grob_index();}
	Grob central_grob () const							{return to_grob(m_nbrInds.central_grob_index());}

	index_t size () const								{return m_nbrInds.size();}
	Grob operator [] (const index_t i) const			{return neighbor (i);}
	Grob neighbor (const index_t i) const				{return to_grob (m_nbrInds.neighbor(i));}

	const_iterator_t begin() const						{return iterator_t (*this, 0);}
	const_iterator_t end() const						{return iterator_t (*this, size());}

	const Neighborhoods* neighborhoods() const			{return m_nbrInds.neighborhoods();}

private:
	Grob to_grob (const GrobIndex& gi) const;

	NeighborIndices m_nbrInds;
};


}//	end of namespace lume

#endif	//__H__lume_neighbors
