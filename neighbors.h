// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2018 Sebastian Reiter <s.b.reiter@gmail.com>

#ifndef __H__lume_neighbors
#define __H__lume_neighbors

#include "grob_index.h"

namespace lume {
	
class Neighbors {
public:
	Neighbors ();
	/// Constructs an instance with a neighborhood of a given size
	/** \param _size	The number of neighbors
	 *	\param _nbrs	An array of the length `_size*2`, containing
	 *					`_size` index pairs (`grobType`, `index`).*/
	Neighbors (const index_t _size, const index_t* _nbrs);
	
	index_t size () const;
	GrobIndex operator [] (const index_t i) const		{return neighbor(i);}
	GrobIndex neighbor (const index_t i) const;

private:
	const index_t	m_size;
	const index_t* 	m_nbrs;
};

}//	end of namespace lume

#endif	//__H__lume_neighbors
