// This file is part of lume, a C++ library for lightweight unstructured meshes
//
// Copyright (C) 2019 Sebastian Reiter <s.b.reiter@gmail.com>
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

#include <cassert>
#include <lume/math/raw/vector_math_raw.h>

namespace lume::math
{

template <class Vec1, class Vec2>
bool
SizesMatch (Vec1 const& a, Vec2 const& b)
{
    return a.size () == b.size ();
}

template <class Vec1, class Vec2, class Vec3>
bool
SizesMatch (Vec1 const& a, Vec2 const& b, Vec3 const& c)
{
    return a.size () == b.size () &&
           a.size () == c.size ();
}

template <class Vec1, class Vec2, class Vec3, class Vec4>
bool
SizesMatch (Vec1 const& a, Vec2 const& b, Vec3 const& c, Vec4 const& d)
{
    return a.size () == b.size () &&
           a.size () == c.size () &&
           a.size () == d.size ();
}

template <class VecOut, class VecIn>
VecOut&
VecCopy (VecOut& out, const VecIn& in)
{
    assert (SizesMatch (out, in));
    raw::VecCopy (out.data (), out.size (), in.data ());
    return out;
}

template <class VecInOut>
VecInOut&
VecSet (VecInOut& vOut, typename VecInOut::value_type const v)
{
    raw::VecSet (vOut.data (), vOut.size (), v);
    return vOut;
}

template <class VecInOut, class VecIn>
VecInOut&
VecAddInplace (VecInOut& out, const VecIn& in)
{
    assert (SizesMatch (out, in));
    raw::VecAddInplace (out.data (), out.size (), in.data ());
    return out;
}

template <class VecOut, class VecIn1, class VecIn2>
VecOut&
VecAdd (VecOut& out, const VecIn1& a, const VecIn2& b)
{
    assert (SizesMatch (out, a, b));
    raw::VecAdd (out.data (), out.size (), a.data (), b.data ());
    return out;
}

template <class VecInOut, class VecIn>
VecInOut&
VecSubtractInplace (VecInOut& out, const VecIn& in)
{
    assert (SizesMatch (out, in));
    raw::VecSubtractInplace (out.data (), out.size (), in.data ());
    return out;
}

template <class VecOut, class VecIn1, class VecIn2>
VecOut&
VecSubtract (VecOut& out, const VecIn1& a, const VecIn2& b)
{
    assert (SizesMatch (a, b));
    raw::VecSubtract (out.data (), out.size (), a.data (), b.data ());
    return out;
}

template <class VecInOut>
VecInOut&
VecScaleInplace (VecInOut& out, typename VecInOut::value_type const s)
{
    raw::VecScaleInplace (out.data (), out.size (), s);
    return out;
}

template <class VecOut, class VecIn>
VecOut&
VecScale (VecOut& out, const VecIn& in, typename VecIn::value_type const s)
{
    assert (SizesMatch (out, in));
    raw::VecScale (out.data (), out.size (), in.data (), s);
    return out;
}

template <class Vec1, class Vec2>
typename Vec1::value_type
VecDot (const Vec1& a, const Vec2& b)
{
    assert (SizesMatch (a, b));
    return raw::VecDot (a.data (), a.size (), b.data ());
}

template <class Vec>
typename Vec::value_type
VecLenSq (const Vec& v)
{
    return raw::VecLenSq (v.data (), v.size ());
}

template <class Vec>
typename Vec::value_type
VecLen (const Vec& v)
{
    return raw::VecLen (v.data (), v.size ());
}

template <class Vec1, class Vec2>
typename Vec1::value_type
VecDistSq (const Vec1& a, const Vec2& b)
{
    assert (SizesMatch (a, b));
    return raw::VecDistSq (a.data (), a.size (), b.data ());
}

template <class Vec1, class Vec2>
typename Vec1::value_type
VecDist (const Vec1& a, const Vec2& b)
{
    assert (SizesMatch (a, b));
    return raw::VecDist (a.data (), a.size (), b.data ());
}

template <class VecOut, class VecIn>
VecOut&
VecNormalize (VecOut& out, const VecIn& in)
{
    assert (SizesMatch (out, in));
    raw::VecNormalize (out.data (), out.size (), in.data ());
    return out;
}

template <class VecInOut>
VecInOut&
VecNormalizeInplace (VecInOut& out)
{
    raw::VecNormalizeInplace (out.data (), out.size ());
    return out;
}

/// Adds the tuple `tup` to each tuple in `inOut`
/** Considers the contents of `inOut` to be a sequence of tuples of size `tup.size ()`.*/
template <class VecInOut, class Tuple>
VecInOut& VecTupAddInplace (VecInOut& inOut, const Tuple& tup)
{
    raw::VecTupAddInplace (inOut.data (), inOut.size (), tup.size (), tup.data ());
    return inOut;
}

/// Adds the tuple `tup` to each tuple in `in` and stores the result in `out`
/** Considers the contents of `out` to be a sequence of tuples, each of size `tup.size ()`.*/
template <class VecOut, class VecIn, class Tuple>
VecOut& VecTupAdd (VecOut& out, VecIn const& in, Tuple const& tup)
{
    raw::VecTupAdd (out.data (), in.data (), in.size (), tup.size (), tup.data ());
    return out;
}

/// Subtracts the tuple `tup` from each tuple in `in` and stores the result in `out`
/** Considers the contents of `out` to be a sequence of tuples, each of size `tup.size ()`.*/
template <class VecOut, class VecIn, class Tuple>
VecOut& VecTupSubtract (VecOut& out, VecIn const& in, Tuple const& tup)
{
    raw::VecTupSubtract (out.data (), in.data (), in.size (), tup.size (), tup.data ());
    return out;
}

/// Normalizes each tuple in `in` and writes the result to `out`
template <class TupleVecOut, class TupleVecIn>
TupleVecOut& VecTupNormalize (TupleVecOut& out, TupleVecIn const& in)
{
    assert (SizesMatch (out, in));
    raw::VecTupNormalize (out.data (), out.size (), out.tuple_size (), in.data ());
    return out;
}

/// Normalizes each tuple in `inOut`
template <class TupleVecInOut>
TupleVecInOut& VecTupNormalizeInplace (TupleVecInOut& inOut)
{
    raw::VecTupNormalizeInplace (inOut.data (), inOut.size (), inOut.tuple_size ());
    return inOut;
}

/// Computes the sum of all tuples in `in`.
/** Considers the contents of `in` to be a sequence of tuples, each of size `out.size ()`.*/
template <class Tuple, class VecIn>
Tuple& VecTupSum (Tuple& out, VecIn const& in)
{
    raw::VecTupSum (out.data (), in.data (), in.size (), out.size ());
    return out;
}

/// Computes the sum of all tuples in `in`.
/** Considers the contents of `in` to be a sequence of tuples, each of size `out.size ()`.*/
template <class Tuple, class VecIn>
Tuple& VecTupAverage (Tuple& out, VecIn const& in)
{
    raw::VecTupAverage (out.data (), in.data (), in.size (), out.size ());
    return out;
}


template <class VecOut, class VecIn1, class VecIn2>
VecOut& VecCross3 (VecOut& out, VecIn1 const& v0, VecIn2 const& v1)
{
    assert (SizesMatch (out, v0, v1));
    assert (out.size () == 3);
    raw::VecCross3 (out.data (), v0.data (), v1.data ());
    return out;
}

}// end of namespace lume
