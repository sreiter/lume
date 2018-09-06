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
//     * Neither the name of the copyright holders, the authors nor the
//       names of the contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
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


#ifndef __H__lume_grob_hash
#define __H__lume_grob_hash

#include <unordered_set>
#include <unordered_map>
#include "grob.h"

namespace std
{
    template<> struct hash<lume::Grob>
    {
        typedef lume::Grob argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& grob) const noexcept
        {
        	using namespace lume;
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


namespace lume {
	using GrobHash = std::unordered_set <Grob>;

	template <class T>
	using GrobHashMap = std::unordered_map <Grob, T>;
}//	end of namespace lume

#endif	//__H__lume_grob_hash
