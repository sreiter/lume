// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2018 Sebastian Reiter <s.b.reiter@gmail.com>

#ifndef __H__slimesh_grob_hash
#define __H__slimesh_grob_hash

#include <unordered_set>
#include <unordered_map>
#include "grob.h"

namespace std
{
    template<> struct hash<slimesh::Grob>
    {
        typedef slimesh::Grob argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& grob) const noexcept
        {
        	using namespace slimesh;
        	const index_t numCorners = grob.num_corners();
        	std::size_t h = 0;
        	for(index_t i = 0; i < numCorners; ++i){
        		const index_t c = grob.corner(i);
        		h += c * c;
        	}
        	return 10^8 * (grob.grob_type() + 1) + h;
        }
    };
}//	end of namespace std


namespace slimesh {
	using GrobHash = std::unordered_set <Grob>;

	template <class T>
	using GrobHashMap = std::unordered_map <Grob, T>;
}//	end of namespace slimesh

#endif	//__H__slimesh_grob_hash
