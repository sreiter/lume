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

#ifndef __H__lume_grob_desc
#define __H__lume_grob_desc

#include "grob_set_types.h"
#include "grob_types.h"
#include "types.h"

namespace lume {

/// Describes a class of **grid objects** in terms of local corner indices and sides.
/** A `GrobDesc` is a descriptor object for a given type of *grid object*.
 * An instance can be queried for different properties common to a type of
 * **grid object**.
 *
 * \note    An instance of `GrobDesc` corresponds to the **class** of *grid objects*
 *          of a certain type, not to an individual *grid object*.
 *
 * \note    *Local corner indices* always start from 0 and go up to `num_corners()`.
 */
class GrobDesc {
public:
    GrobDesc (GrobType grobType);

    inline GrobType grob_type () const              {return static_cast<GrobType>(m_rawDesc [0]);}
    inline const std::string& name () const         {return GrobTypeName (grob_type());}
    inline index_t dim () const                     {return m_rawDesc [1];}
    inline index_t num_corners () const             {return (grob_type() == VERTEX) ? 1 : m_rawDesc [side_offset (0)];}

    inline index_t local_corner (const index_t cornerIndex) const
    {
        return *local_side_corners (0, cornerIndex);
    }

    inline index_t num_sides (const index_t sideDim) const
    {
        if (sideDim >= dim ())
            return 0;
        return m_rawDesc [side_offset (sideDim)];
    }

    inline GrobSetType side_set_type (const index_t sideDim) const
    {
        return GrobSetType (m_rawDesc [2 + sideDim]);
    }

    inline GrobType side_type (const index_t sideDim, const index_t sideIndex) const
    {
        return static_cast<GrobType> (m_rawDesc [side_desc_offset (sideDim, sideIndex)]);
    }
    
    inline GrobDesc side_desc (const index_t sideDim, const index_t sideIndex) const
    {
        return GrobDesc (side_type (sideDim, sideIndex));
    }

    inline index_t num_side_corners (const index_t sideDim, const index_t sideIndex) const
    {
        return side_desc (sideDim, sideIndex).num_corners();
    }

    /// returns the local indices of corners of the specified side
    inline const index_t* local_side_corners (const index_t sideDim, const index_t sideIndex) const
    {
        return m_rawDesc + side_desc_offset (sideDim, sideIndex) + 1;
    }

private:
    inline index_t side_offset (const index_t sideDim) const
    {
        return 2 + dim() + sideDim + m_rawDesc [2 + dim() + sideDim];
    }

    inline index_t side_desc_offset (const index_t sideDim, const index_t sideIndex) const
    {
        const index_t t = side_offset (sideDim) + 1 + sideIndex;
        return t + m_rawDesc [t];
    }

    const index_t* m_rawDesc;
};

}//    end of namespace lume

#endif    //__H__lume_grob_desc
