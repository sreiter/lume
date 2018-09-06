// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2018 Sebastian Reiter <s.b.reiter@gmail.com>

#ifndef __H__slimesh_unpack
#define __H__slimesh_unpack

#include "types.h"

///	Unpacks a buffer into arguments `data` and `size`.
/** This macro is useful if a function takes a data pointer and a size argument.
 * Instead of manually specifying `buf.raw_ptr(), buf.size()`, or in case of
 * std::vector `&buf.front(), &buf.size()`, one can call
 * the macro `UNPACK_DS(buf)` which automatically extends to the above arguments.
 *
 * 
 * The macro uses the functions slimesh::impl::buf_data_ptr and slimesh::impl::buf_size
 * which can be adapted to different containers/buffers. A specialization for std::vector
 * exists.*/
#define UNPACK_DS(buf) slimesh::impl::buf_data_ptr(buf), slimesh::impl::buf_size(buf)

#define UNPACK_DST(buf) slimesh::impl::buf_data_ptr(buf), slimesh::impl::buf_size(buf), slimesh::impl::buf_tuple_size(buf)

namespace slimesh {
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
}//	end of namespace slimesh

#endif	//__H__slimesh_unpack
