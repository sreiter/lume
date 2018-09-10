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


#ifndef __H__lume_unpack
#define __H__lume_unpack

#include "types.h"

///	Unpacks a buffer into arguments `data` and `size`.
/** This macro is useful if a function takes a data pointer and a size argument.
 * Instead of manually specifying `buf.raw_ptr(), buf.size()`, or in case of
 * std::vector `&buf.front(), &buf.size()`, one can call
 * the macro `UNPACK_DS(buf)` which automatically extends to the above arguments.
 *
 * 
 * The macro uses the functions lume::impl::buf_data_ptr and lume::impl::buf_size
 * which can be adapted to different containers/buffers. A specialization for std::vector
 * exists.*/
#define UNPACK_DS(buf) lume::impl::buf_data_ptr(buf), lume::impl::buf_size(buf)

#define UNPACK_DST(buf) lume::impl::buf_data_ptr(buf), lume::impl::buf_size(buf), lume::impl::buf_tuple_size(buf)

namespace lume {
namespace impl {

template <class buf_t>
struct buf_traits {
	using value_t			= typename buf_t::value_type;
	using value_ptr_t		= typename buf_t::value_type*;
	using const_value_ptr_t	= const typename buf_t::value_type*;
	using size_t			= index_t;//typename buf_t::size_type;
};

// buf_data_ptr
template <class buf_t>
typename buf_traits <buf_t>::value_ptr_t
buf_data_ptr (buf_t& b) {
	return b.raw_ptr();
}

template <class T>
T* buf_data_ptr (std::vector<T>& b) {
	return &b.front();
}


// const buf_data_ptr
template <class buf_t>
typename buf_traits <buf_t>::const_value_ptr_t
buf_data_ptr (const buf_t& b) {
	return b.raw_ptr();
}

template <class T>
const T* buf_data_ptr (const std::vector<T>& b) {
	return &b.front();
}


// buf_size
template <class buf_t>
typename buf_traits <buf_t>::size_t
buf_size (const buf_t& b) {
	return static_cast <typename buf_traits <buf_t>::size_t> (b.size());
}


// buf_tuple_size
template <class buf_t>
typename buf_traits <buf_t>::size_t
buf_tuple_size (const buf_t& b) {
	return static_cast <typename buf_traits <buf_t>::size_t> (b.tuple_size());
}

}// end of namespace impl
}//	end of namespace lume

#endif	//__H__lume_unpack
