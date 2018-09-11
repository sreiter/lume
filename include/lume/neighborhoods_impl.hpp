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
void FillHigherDimNeighborOffsetMap (TIndexVector& offsetsOut,
			                         Mesh& mesh,
			                     	 GrobSet grobSet,
			                     	 GrobSet nbrGrobSet,
			                     	 const GrobHashMap <index_t>& grobToIndexMap)
{
	offsetsOut.clear ();
	offsetsOut.resize (mesh.num (grobSet) + 1, 0);

	const index_t grobSetDim = grobSet.dim();
	const index_t nbrGrobSetDim = nbrGrobSet.dim();

	if (nbrGrobSetDim <= grobSetDim)
		throw LumeError ("neighbor dimension has to be higher than central grob set dimension");

	for (auto nbrGrobType : nbrGrobSet) {
		for(auto nbrGrob : mesh.grobs (nbrGrobType)) {
			for(index_t iside = 0; iside < nbrGrob.num_sides(grobSetDim); ++iside) {
				++offsetsOut[grobToIndexMap.at(nbrGrob.side (grobSetDim, iside))];
			}
		}
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
void FillHigherDimNeighborMap (TIndexVector& nbrMapOut,
                        	   TIndexVector& offsetsOut,
                        	   index_t* grobBaseIndsOut,
                        	   Mesh& mesh,
                        	   GrobSet grobSet,
                        	   GrobSet nbrGrobSet)
{
	const index_t grobSetDim = grobSet.dim();
	const index_t nbrGrobSetDim = nbrGrobSet.dim();

	if (nbrGrobSetDim <= grobSetDim)
		throw LumeError ("neighbor dimension has to be higher than central grob set dimension");

	GrobHashMap <index_t> grobToIndexMap;
	FillGrobToIndexMap (grobToIndexMap, grobBaseIndsOut, mesh, grobSet);

	FillHigherDimNeighborOffsetMap (offsetsOut, mesh, grobSet, nbrGrobSet, grobToIndexMap);

	nbrMapOut.clear ();
	nbrMapOut.resize (offsetsOut.back() * 2, NO_GROB);
	
	for (auto nbrGrobType : nbrGrobSet) {
		index_t nbrGrobIndex = 0;
		for(auto nbrGrob : mesh.grobs (nbrGrobType)) {
			for(index_t iside = 0; iside < nbrGrob.num_sides(grobSetDim); ++iside) {
				const index_t eind = grobToIndexMap.at (nbrGrob.side (grobSetDim, iside));
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


template <class TIndexVector>
void FillLowerDimNeighborOffsetMap (TIndexVector& offsetsOut,
			                        Mesh& mesh,
			                     	GrobSet grobSet,
			                     	GrobSet nbrGrobSet)
{
	offsetsOut.clear ();
	offsetsOut.resize (mesh.num (grobSet) + 1, 0);

	const index_t grobSetDim = grobSet.dim();
	const index_t nbrGrobSetDim = nbrGrobSet.dim();

	if (nbrGrobSetDim >= grobSetDim)
		throw LumeError ("neighbor dimension has to be lower than central grob set dimension");

	offsetsOut [0] = 0;
	index_t counter = 1;
	for(auto grobType : grobSet) {
		for(auto grob : mesh.grobs (grobType)) {
			offsetsOut [counter] = offsetsOut [counter - 1] + grob.num_sides (nbrGrobSetDim);
			++counter;
		}
	}
}

template <class TIndexVector>
void FillLowerDimNeighborMap (TIndexVector& nbrMapOut,
                        	   TIndexVector& offsetsOut,
                        	   index_t* grobBaseIndsOut,
                        	   Mesh& mesh,
                        	   GrobSet grobSet,
                        	   GrobSet nbrGrobSet)
{
	const index_t grobSetDim = grobSet.dim();
	const index_t nbrGrobSetDim = nbrGrobSet.dim();

	if (nbrGrobSetDim >= grobSetDim)
		throw LumeError ("neighbor dimension has to be lower than central grob set dimension");

	FillLowerDimNeighborOffsetMap (offsetsOut, mesh, grobSet, nbrGrobSet);

	nbrMapOut.clear ();
	nbrMapOut.resize (offsetsOut.back() * 2, NO_GROB);

	index_t nbrGrobBaseInds [NUM_GROB_TYPES];
	GrobHashMap <index_t> nbrGrobToIndexMap;
	FillGrobToIndexMap (nbrGrobToIndexMap, nbrGrobBaseInds, mesh, nbrGrobSet);

	index_t counter = 0;
	VecSet (grobBaseIndsOut, 0, NUM_GROB_TYPES);
	for(auto grobType : grobSet) {
		grobBaseIndsOut [grobType] = counter;
		for(auto grob : mesh.grobs (grobType)) {
			const index_t offset = 2 * offsetsOut [counter];
			const index_t numNbrs = grob.num_sides (nbrGrobSetDim);
			
			for(index_t inbr = 0; inbr < numNbrs; ++inbr) {
				const Grob nbrGrob = grob.side (nbrGrobSetDim, inbr);
				nbrMapOut [offset + 2*inbr] = nbrGrob.grob_type();
				nbrMapOut [offset + 2*inbr + 1] = nbrGrobToIndexMap.at (nbrGrob) - nbrGrobBaseInds [nbrGrob.grob_type()];
			}
			++counter;
		}
	}
}


template <class TIndexVector>
void FillNeighborMap (TIndexVector& nbrMapOut,
                      TIndexVector& offsetsOut,
                      index_t* grobBaseIndsOut,
                      Mesh& mesh,
                      GrobSet grobSet,
                      GrobSet nbrGrobSet)
{
	const index_t grobSetDim = grobSet.dim();
	const index_t nbrGrobSetDim = nbrGrobSet.dim();

	if (nbrGrobSetDim > grobSetDim) {
		FillHigherDimNeighborMap (nbrMapOut, offsetsOut, grobBaseIndsOut, mesh, grobSet, nbrGrobSet);
	}
	else if (nbrGrobSetDim < grobSetDim) {
		FillLowerDimNeighborMap (nbrMapOut, offsetsOut, grobBaseIndsOut, mesh, grobSet, nbrGrobSet);
	}
	else {
		throw LumeError ("FillNeighborMap: Currently, nbr dimension == elem dimension is not supported");
	}
}

}// end of namespace impl	
}//	end of namespace lume

#endif	//__H__lume_neighborhoods_impl
