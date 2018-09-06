// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2018 Sebastian Reiter <s.b.reiter@gmail.com>

#ifndef __H__slimesh_grob_index
#define __H__slimesh_grob_index

#include "grob.h"

namespace slimesh {

/// Pairs an index with a grob type
/** \todo	grobType and index should be private and only availablle through method calls.
 * \todo	store grobType and index in one 64-bit unsigned integer.
 */
struct GrobIndex {
	GrobIndex () :
		grobType (NO_GROB),
		index (0)
	{}

	GrobIndex (grob_t _grobType, index_t _index) :
		grobType (_grobType),
		index (_index)
	{}

	grob_t	grobType;
	index_t	index;
};

}//	end of namespace slimesh

#endif	//__H__slimesh_grob_index
