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


#ifndef __H__lume__grob
#define __H__lume__grob

#include <array>
#include <cstdint>
#include <string>
#include "grob_desc.h"
#include "types.h"
#include "impl/array_16_4.h"

namespace lume {

/// A Grob represents a **grid object**, specified by its corner indices.
/** To create a `Grob`, pass the `grobType` and an array of corner-indices
 * (`corners`) to the constructor. `corners`has to have at least length
 * `GridDesc(grobType).num_corners()`.
 *
 * \warning	Since a `Grob` instance stores a pointer to the specified array
 *			of corners, an instance is invalidated once the pointer does no longer
 *			point to a valid memory location (e.g. because the underlying corner
 *			array has been resized). A `Grob` thus should only be used as a
 *			temporary object and should only be used if it is clear that the
 *			underlying coner-array is still valid.
 */
class Grob {
public:
	Grob (GrobType grobType, const index_t* corners = nullptr) :
		m_globCornerInds (corners),
		m_cornerOffsets (impl::Array_16_4::ascending_order ()),
		m_desc (grobType)
	{}

    /// Returns the maximum number of corners that any `Grob` may have.
    /** For all Grobs `g` holds: `g.num_grobs() <= Grob::max_num_grobs()`.
        \note This limitation arises from the use of Array_16_4 which is used to
              store local corner indices*/
    static constexpr index_t maxNumCorners = 16;

	///	only compares corners, ignores order and orientation.
	bool operator == (const Grob& g) const
	{
		if (m_desc.grob_type() != g.m_desc.grob_type())
			return false;

		std::array <index_t, maxNumCorners> gCorners;
		g.corners (gCorners);

		const index_t numCorners = num_corners ();
		for(index_t i = 0; i < numCorners; ++i) {
			bool gotOne = false;
			const index_t c = corner(i);
			for(index_t j = 0; j < numCorners; ++j) {
				if(c == gCorners [j]){
					gotOne = true;
					break;
				}
			}

			if (!gotOne)
				return false;
		}

		return true;
	}

	bool operator != (const Grob& g) const
	{
		return !((*this) == g);
	}

	inline index_t dim () const							{return m_desc.dim ();}
	
	inline GrobType grob_type () const					{return m_desc.grob_type ();}
	
	inline GrobDesc desc () const 						{return m_desc;}

	inline void set_global_corner_array (const index_t* corners)
	{
		m_globCornerInds = corners;
	}

	inline const index_t* global_corner_array () const
	{
		return m_globCornerInds;
	}

	inline index_t num_corners () const					{return m_desc.num_corners();}

	/// returns the global index of the i-th corner
	inline index_t corner (const index_t i) const
	{
		return m_globCornerInds [m_cornerOffsets.get(i)];
	}

	/// collects the global indices of corners
	/** \param cornersOut	Array of size `Grob::max_num_corners(). Only the first
     *                      `Grob::num_corners()` entries are filled
	 *
	 * \returns	The number of corners of the specified side
	 */
	inline index_t corners (std::array <index_t, maxNumCorners>& cornersOut) const
	{
		const index_t numCorners = num_corners();
		for(index_t i = 0; i < numCorners; ++i)
			cornersOut[i] = static_cast<index_t> (m_globCornerInds [m_cornerOffsets.get(i)]);
		return numCorners;
	}

	inline index_t num_sides (const index_t sideDim) const
	{
		return m_desc.num_sides(sideDim);
	}
	
	inline GrobDesc side_desc (const index_t sideDim, const index_t sideIndex) const
	{
		return m_desc.side_desc (sideDim, sideIndex);
	}

	Grob side (const index_t sideDim, const index_t sideIndex) const
	{
		impl::Array_16_4 cornerOffsets;
		const index_t numCorners = m_desc.side_desc(sideDim, sideIndex).num_corners();
		const index_t* locCorners = m_desc.local_side_corners (sideDim, sideIndex);
		// LOGT(side, "side " << sideIndex << "(dim: " << sideDim << ", num corners: " << numCorners << "): ");
		for(index_t i = 0; i < numCorners; ++i){
			// LOG(locCorners[i] << " ");
			cornerOffsets.set (i, m_cornerOffsets.get(locCorners[i]));
		}
		// LOG("\n");

		return Grob (m_desc.side_type (sideDim, sideIndex), m_globCornerInds, cornerOffsets);
	}

	/// returns the index of the side which corresponds to the given grob
	/** if no such side was found, 'lume::NO_INDEX' is returned.*/
	index_t find_side (const Grob& sideGrob) const
	{
		const index_t sideDim = sideGrob.dim();
		const index_t numSides = num_sides (sideDim);
		for(index_t iside = 0; iside < numSides; ++iside) {
			if (sideGrob == side (sideDim, iside))
				return iside;
		}
		return NO_INDEX;
	}

private:
	Grob (GrobType grobType, const index_t* globCornerInds, const impl::Array_16_4& cornerOffsets) :
		m_globCornerInds (globCornerInds),
		m_cornerOffsets (cornerOffsets),
		m_desc (grobType)
	{}

	const index_t*		m_globCornerInds;
	impl::Array_16_4	m_cornerOffsets;
	GrobDesc			m_desc;
};

}//	end of namespace lume

#endif	//__H__lume__grob
