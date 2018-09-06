// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2017 Sebastian Reiter, G-CSC Frankfurt <s.b.reiter@gmail.com>

#include "neighbors.h"

namespace slimesh {

Neighbors::
Neighbors () :
	m_size (0),
	m_nbrs (0)
{

}

Neighbors::
Neighbors (const index_t _size, const index_t* _nbrs) :
	m_size (_size),
	m_nbrs (_nbrs)
{
}


index_t Neighbors::
size () const
{
	return m_size;
}

GrobIndex Neighbors::
neighbor (const index_t i) const
{
	const index_t* p = m_nbrs + i * 2;
	return GrobIndex (static_cast<grob_t>(p[0]), p[1]);
}


}//	end of namespace slimesh
