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

#ifndef __H__lume_parallel_for
#define __H__lume_parallel_for

#include <algorithm>
#include <functional>
#include <future>
#include <iterator>
#include <thread>
#include <iostream>

namespace lume {

namespace impl {

template<typename T, typename = void>
struct is_iterator
{
   static constexpr bool value = false;
};

template<typename T>
struct is_iterator<T, typename std::enable_if<!std::is_same<typename std::iterator_traits<T>::value_type, void>::value>::type>
{
   static constexpr bool value = true;
};


template <class T, bool isIter>
struct call_with_ref_or_value_impl {
};

template <class T>
struct call_with_ref_or_value_impl <T, true> {
	template <class TFunc>
	static
	void call (const TFunc& f, T& t) {f(*t);}
};

template <class T>
struct call_with_ref_or_value_impl <T, false> {
	template <class TFunc>
	static
	void call (const TFunc& f, const T& t) {f(t);}
};

template <class T>
struct call_with_ref_or_value {
	template <class TFunc>
	static 
	void call (const TFunc& f, T& t)
	{
		call_with_ref_or_value_impl <T, is_iterator<T>::value>::call (f, t);
	}
};
}// end of namespace impl


///	Executes a parallel for and calls `func` for each encountered value.
/**
 * Usage:
 * The following code demonstrates how index based iteration is used to fill
 * a vector with a sequence of indices and iterator based variants are used to
 * first set all entries to '0' and then to '1'.
 *
 * \code
 * vector <int> v (10, 0);
 * parallel_for (0, v.size(), [&v] (int i) {v[i] = i;})
 * // v == {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
 *
 * parallel_for (v, [] (int& e) {e = 0});
 * // v == {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
 *
 * parallel_for (v.begin(), v.end(), [] (int& e) {e = 1});
 * // v == {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
 * \endcode
 *
 * In the code above, as many threads as the hardware supports are used and the
 * iteration squence is cut into corresponding blocks, so that each thread executes
 * a sequential iteration on one block.
 *
 * If, however, the function that you pass to parallel_for does heavy work, or if
 * the runtime of that function varies depending on the current iterate, you may want
 * to schedule more threads than there are hardware threads. To this end you may
 * specify the size of the blocks, which are processed by one thread. This can
 * be done specifying the optional parameter `blockSize` when calling `parallel_for`.
 * The maximum number of threads will be scheduled so that
 * each thread operates on a block of at least size `blockSize`. An example:
 *
 * \code
 * void SomeHeavyFunction (shared_ptr<SomeClass>&);
 * // ...
 * vector<shared_ptr<SomeClass>> v;
 * parallel_for (v, &SomeHeavyFunction, 1)
 * \endcode
 *
 * This would start one thread for each entry in `v`.
 *
 * \warning when using `parallel_for`, please be aware that serious issues may
 *			arise, if common data-types are accessed during a loop. The following
 *			code would lead to serious problems and unexpected results:
 *			\code
 *			// WARNING: BAD CODE WITH SERIOUS ISSUES
 *			vector v;
 *			parallel_for (0, 10, [&v](int i){v.push_back(i);})
 *			\endcode
 *
 * \param begin 	Iterator to the beginning of a sequence or an integer with the
 *					start value for the iterate.
 *
 * \param end 		Iterator to the end of a sequence or an integer value which denotes
 *					the value directly after the last value in the sequence.
 *
 * \param container	An overload exists which takes a container instead of a pair
 *					of begin/end iterators. Such a container has to feature methods
 *					`iter_t begin()` and `iter_t end()`, where `iter_t` is an iterator
 *					type compatible with `std::iterator_traits`.
 *
 * \param func		If iterators have been passed to the method, then TFunc should
 *					have a signature compatible to `void (*)(T&)`, where `T`
 *					corresponds to the value type of the iterator.
 *					If an integer range has been specified, then the signature should
 *					be `void (*)(T)`, where `T` is of the same integer type as the
 *					ones used to define the range.
 *
 * \param blockSize	(optional, default = 0) defines the minimal size of an
 *					iteration block which shall be processed by one thread.
 *					The maximum number of threads will be scheduled so that each
 *					thread operates on a block of the provided sequence which has
 *					at least size `blockSize`. 
 *					If not specified or 0, the block size will be determined
 *					automatically, so that each hardware thread operates on
 *					one part of the sequence.
 * \{
 */
template <class TRandAccIter1, class TRandAccIter2, class TFunc>
void parallel_for (TRandAccIter1 begin,
                   TRandAccIter2 end,
                   const TFunc& func,
                   const int blockSize = 0)
{
	using iter_t = TRandAccIter1;

	const auto len = end - begin;
	if(len <= 0)
		return;

	const size_t numBlocks = blockSize ?
							 std::max <size_t> (1, static_cast<size_t> (len / blockSize)) :
							 std::max<int> (1, std::thread::hardware_concurrency());

	std::vector <std::future<void>> futures;
	futures.reserve (numBlocks);

	for(size_t iblock = 0; iblock < numBlocks; ++iblock) {
		const auto restLen = (end - begin);
		const auto restBlocks = (numBlocks - iblock);
		auto blockSize = restLen / restBlocks;

	//	process one additional entry until (blockSize * restBlocks == restLen)
		if (blockSize * restBlocks < restLen)
			++blockSize;

		auto tend = begin + blockSize;
		auto f = std::async (std::launch::async,
	                         [begin, tend, func] () {
	                          	for (iter_t i = begin; i < tend; ++i)
	                          		impl::call_with_ref_or_value <iter_t>::call (func, i);
	                         });
		begin = tend;
		futures.push_back (std::move(f));
	}

	for(auto& f : futures)
		f.wait();
}


template <class TRandAccContainer, class TFunc>
void parallel_for (TRandAccContainer& container, const TFunc& func, const int blockSize = 0)
{
	parallel_for (container.begin(), container.end(), func, blockSize);
}
/** \} */

}//	end of namespace lume

#endif	//__H__lume_parallel_for

