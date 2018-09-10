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

#ifndef __H__lume_neighborhoods_impl
#define __H__lume_neighborhoods_impl

#include "neighborhoods.h"
#include "topology.h"

namespace lume {
namespace impl {

template <class TIndexVector>
void FillNeighborOffsetMap (TIndexVector& offsetsOut,
	                        Mesh& mesh,
	                     	GrobSet grobSet,
	                     	GrobSet nbrGrobSet,
	                     	const GrobHashMap <index_t>& grobToIndexMap)
{
	offsetsOut.clear ();
	offsetsOut.resize (mesh.num (grobSet) + 1, 0);

	const index_t grobSetDim = grobSet.dim();
	const index_t nbrGrobSetDim = nbrGrobSet.dim();

	// Count how many associated elements each element has
	if (nbrGrobSetDim > grobSetDim) {
		for (auto nbrGrobType : nbrGrobSet) {
			for(auto nbrGrob : mesh.grobs (nbrGrobType)) {
				for(index_t iside = 0; iside < nbrGrob.num_sides(grobSetDim); ++iside) {
					++offsetsOut[grobToIndexMap.at(nbrGrob.side (grobSetDim, iside))];
				}
			}
		}
	}
	else {
		throw LumeError ("FillNeighborOffsetMap: Currently, nbr dimension > elem dimension has to hold true");
	}

	// Compute an offset into a neighbor map for each element (convert count -> offset)
	index_t offset = 0;
	for(index_t i = 0; i < offsetsOut.size(); ++i) {
		const index_t curOffset = offset;
		offset += offsetsOut[i];
		offsetsOut[i] = curOffset;
	}
}


template <class TIndexVector>
void FillNeighborMap (TIndexVector& nbrMapOut,
                        TIndexVector& offsetsOut,
                        index_t* grobBaseIndsOut,
                        Mesh& mesh,
                        GrobSet grobs,
                        GrobSet nbrGrobs)
{
	if (nbrGrobs.dim() <= grobs.dim())
		throw LumeError ("FillNeighborMap: Currently only nbrGrobs.dim() > grobs.dim() supported");
	
	GrobHashMap <index_t> grobToIndexMap;
	FillGrobToIndexMap (grobToIndexMap, grobBaseIndsOut, mesh, grobs);

	FillNeighborOffsetMap (offsetsOut, mesh, grobs, nbrGrobs, grobToIndexMap);

	nbrMapOut.clear ();
	nbrMapOut.resize (offsetsOut.back() * 2, NO_GROB);
	
	const index_t elemDim = grobs.dim();

	for (auto nbrGrobType : nbrGrobs) {
		index_t nbrGrobIndex = 0;
		for(auto nbrGrob : mesh.grobs (nbrGrobType)) {
			for(index_t iside = 0; iside < nbrGrob.num_sides(elemDim); ++iside) {
				const index_t eind = grobToIndexMap.at (nbrGrob.side (elemDim, iside));
				const index_t offset = 2 * offsetsOut [eind];
				const index_t numAss = offsetsOut [eind + 1] - offsetsOut [eind];
				for(index_t j = offset; j < offset + 2 * numAss; j+=2) {
					if (nbrMapOut [j] == NO_GROB){
						nbrMapOut [j] = nbrGrobType;
						nbrMapOut [j+1] = nbrGrobIndex;
						break;
					}
				}
			}
			++nbrGrobIndex;
		}
	}
}

}// end of namespace impl	
}//	end of namespace lume

#endif	//__H__lume_neighborhoods_impl
