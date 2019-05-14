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

#pragma once

#include <cmath>
#include "lume/types.h"

namespace lume::math::raw
{

template <class real_t>
real_t* VecCopy (real_t* vOut, const size_t n, const real_t* v)
{
	for (size_t i = 0; i < n; ++i, ++vOut, ++v)
		*vOut = *v;
	return vOut;
}

template <class real_t, class value_t>
real_t* VecSet (real_t* vOut, const size_t n, const value_t v)
{
	const real_t tv = static_cast <real_t> (v);
	for (size_t i = 0; i < n; ++i, ++vOut)
		*vOut = tv;
	return vOut;
}

template <class real_t>
real_t* VecAddInplace (real_t* vOut, const size_t n, const real_t* v)
{
	for (size_t i = 0; i < n; ++i, ++vOut, ++v)
		*vOut += *v;
	return vOut;
}

template <class real_t>
real_t* VecAdd (real_t* vOut, const size_t n, const real_t* v0, const real_t* v1)
{
	for (size_t i = 0; i < n; ++i, ++vOut, ++v0, ++v1)
		*vOut = *v0 + *v1;
	return vOut;
}

template <class real_t>
real_t* VecSubtractInplace (real_t* vOut, const size_t n, const real_t* v)
{
    for (size_t i = 0; i < n; ++i, ++vOut, ++v)
        *vOut -= *v;
    return vOut;
}

template <class real_t>
real_t* VecSubtract (real_t* vOut, const size_t n, const real_t* v0, const real_t* v1)
{
	for (size_t i = 0; i < n; ++i, ++vOut, ++v0, ++v1)
		*vOut = *v0 - *v1;
	return vOut;
}

template <class real_t, class value_t>
real_t* VecScale (real_t* vOut, const size_t n, const real_t* v, const value_t s)
{
	const real_t ts = static_cast <real_t> (s);
	for (size_t i = 0; i < n; ++i, ++vOut, ++v)
		*vOut = *v * ts;
	return vOut;
}

template <class real_t, class value_t>
real_t* VecScaleInplace (real_t* vInOut, const size_t n, const value_t s)
{
	const real_t ts = static_cast <real_t> (s);
	for (size_t i = 0; i < n; ++i, ++vInOut)
		*vInOut *= ts;
	return vInOut;
}

template <class real_t>
real_t VecDot (const real_t* v0, const size_t n, const real_t* v1)
{
	real_t d = 0;
	for (size_t i = 0; i < n; ++i, ++v0, ++v1)
		d += *v0 * *v1;
	return d;
}

template <class real_t>
real_t VecLenSq (const real_t* v, const size_t n)
{
	return VecDot (v, n, v);
}

template <class real_t>
real_t VecLen (const real_t* v, const size_t n)
{
	return static_cast<real_t> (sqrt(VecDot (v, n, v)));
}

template <class real_t>
real_t VecDistSq (const real_t* v0, const size_t n, const real_t* v1)
{
	real_t d = 0;
	for (size_t i = 0; i < n; ++i, ++v0, ++v1){
		const real_t dif = *v0 - *v1;
		d += dif*dif;
	}
	return d;
}

template <class real_t>
real_t VecDist (const real_t* v0, const size_t n, const real_t* v1)
{
	return static_cast<real_t> (VecDistSq (v0, n, v1));
}


template <class real_t>
real_t* VecNormalize (real_t* vOut, const size_t n, const real_t* v)
{
	const real_t l = VecLen (v, n);
	if (l != 0) {
		for (size_t i = 0; i < n; ++i, ++vOut, ++v)
			*vOut = *v / l;
	}
	else
		VecCopy (vOut, n, v);
	return vOut;
}

template <class real_t>
real_t* VecNormalizeInplace (real_t* v, const size_t n)
{
	const real_t l = VecLen (v, n);
	if (l != 0) {
		for (size_t i = 0; i < n; ++i, ++v)
			*v /= l;
	}
	return v;
}

///	Adds the tuple `vtup` to each tuple in `vInOut`
/** Considers the contents of `v` to be a sequence of tuples of size `ntup`.
 * \param vInOut	resulting vector of size `n`
 * \param vtup		vector of size `ntup`
 * \param n			number of entries in `v`
 * \param ntup		number of entries of one tuple
 */
template <class real_t>
real_t* VecTupAddInplace (real_t* vInOut, const size_t n, const size_t ntup, const real_t* vtup)
{
	for(size_t i = 0; i < n; i += ntup)
		VecAppend (vInOut + i, ntup, vtup);
	return vInOut;
}

///	Adds the tuple `vtup` to each tuple in `v`
/** Considers the contents of `v` to be a sequence of tuples of size `ntup`.
 * \param vOut		resulting vector of size `n`
 * \param v			vector of size `n`
 * \param n			number of entries in `v`
 * \param ntup		number of entries of one tuple
 * \param vtup		vector of size `ntup`
 */
template <class real_t>
real_t* VecTupAdd (real_t* vOut, const real_t* v, const size_t n, const size_t ntup, const real_t* vtup)
{
	for(size_t i = 0; i < n; i += ntup)
		VecAdd (vOut + i, ntup, v + i, vtup);
	return vOut;
}

///	Subtracts the tuple `vtup` from each tuple in `v`
/** Considers the contents of `v` to be a sequence of tuples of size `ntup`.
 * \param vOut		resulting vector of size `n`
 * \param v			vector of size `n`
 * \param n			number of entries in `v`
 * \param ntup		number of entries of one tuple
 * \param vtup		vector of size `ntup`
 */
template <class real_t>
real_t* VecTupSubtract (real_t* vOut, const real_t* v, const size_t n, const size_t ntup, const real_t* vtup)
{
	for(size_t i = 0; i < n; i += ntup)
		VecSub (vOut + i, ntup, v + i, vtup);
	return vOut;
}

///	Normalizes each tuple in `v`
/** Considers the contents of `v` to be a sequence of tuples of size `ntup`.
 * \param vOut		resulting vector of size `n`
 * \param v			vector of size `n`
 * \param n			number of entries in `v`
 * \param ntup		number of entries of one tuple
 */
template <class real_t>
real_t* VecTupNormalize (real_t* vOut, const size_t n, const size_t ntup, const real_t* v)
{
	for(size_t i = 0; i < n; i += ntup)
		VecNormalize (vOut + i, ntup, v + i);
	return vOut;
}

///	Normalizes each tuple in `vInOut`
/** Considers the contents of `vInOut` to be a sequence of tuples of size `ntup`.
 * \param vInOut	resulting vector of size `n`
 * \param n			number of entries in `v`
 * \param ntup		number of entries of one tuple
 */
template <class real_t>
real_t* VecTupNormalizeInplace (real_t* vInOut, const size_t n, const size_t ntup)
{
	for(size_t i = 0; i < n; i += ntup)
		VecNormalizeInplace (vInOut + i, ntup);
	return vInOut;
}

///	Computes the sum of all tuples in `v`.
/** Considers the contents of `v` to be a sequence of tuples of size `ntup`.
 * \param tupOut	the resulting tuple. Array of size `ntup`
 * \param v			vector of size `n`
 * \param n			number of entries in `v`
 * \param ntup		number of entries of one tuple
 */
template <class real_t>
real_t* VecTupSum (real_t* tupOut, const real_t* v, const size_t n, const size_t ntup)
{
	VecSet (tupOut, ntup, 0);
	for(size_t i = 0; i < n; i += ntup)
		VecAddInplace (tupOut, ntup, v + i);
	return tupOut;
}

///	Computes the average of all tuples in `v`.
/** Considers the contents of `v` to be a sequence of tuples of size `ntup`.
 * \param tupOut	the resulting tuple. Array of size `ntup`
 * \param v			vector of size `n`
 * \param n			number of entries in `v`
 * \param ntup		number of entries of one tuple
 */
template <class real_t>
real_t* VecTupAverage (real_t* tupOut, const real_t* v, const size_t n, const size_t ntup)
{
	VecTupSum (tupOut, v, n, ntup);
	if (ntup > 0 && n >= ntup)
		VecScaleInplace (tupOut, ntup, real_t(1) / (n / ntup));
	return tupOut;
}


template <class real_t>
real_t* VecCross3 (real_t* vOut, const real_t* v0, const real_t* v1)
{
	vOut[0] = v0[1] * v1[2] - v1[1] * v0[2];
	vOut[1] = v0[2] * v1[0] - v1[2] * v0[0];
	vOut[2] = v0[0] * v1[1] - v1[0] * v0[1];
	return vOut;
}

}
