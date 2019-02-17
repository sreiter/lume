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


#ifndef __H__lume_grob_index
#define __H__lume_grob_index

#include <limits>
#include "grob.h"

namespace lume {

/// Pairs an index with a grob type
/** \todo	grobType and index should be private and only availablle through method calls.
 * \todo	store grobType and index in one 64-bit unsigned integer.
 */
class GrobIndex {
public:
	GrobIndex () :
        m_grobType (VERTEX),
        m_index (invalid_index ())
	{}

	GrobIndex (GrobType _grobType, index_t _index) :
		m_grobType (_grobType),
		m_index (_index)
	{}

    bool valid () const             {return m_index != invalid_index ();}
    
    static index_t invalid_index () {return std::numeric_limits <index_t>::max ();}

    GrobType grob_type () const       {return m_grobType;}
    index_t index () const          {return m_index;}

private:
	GrobType	m_grobType;
	index_t	m_index;
};

}//	end of namespace lume

#endif	//__H__lume_grob_index
