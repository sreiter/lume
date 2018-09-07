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


#ifndef __H__lume_topology_impl
#define __H__lume_topology_impl

#include "topology.h"

namespace lume {

template <class TIndexVector>
void GrobHashToIndexArray (TIndexVector& indArrayInOut,
                           const GrobHash& hash)
{
	using iter_t = GrobHash::const_iterator;
	const iter_t iend = hash.end();
	for (iter_t igrob = hash.begin(); igrob != iend; ++igrob) {
		const Grob& grob = *igrob;
		for(index_t i = 0; i < grob.num_corners(); ++i) {
			indArrayInOut.push_back (grob.corner (i));
		}
	}
}

template <class TIndexVector>
void GrobHashToTypeArray (TIndexVector& typeArrayInOut,
                          const GrobHash& hash)
{
	using iter_t = GrobHash::const_iterator;
	const iter_t iend = hash.end();
	for (iter_t igrob = hash.begin(); igrob != iend; ++igrob) {
		typeArrayInOut.push_back (igrob->grob_type());
	}
}

template <class TIndexVector>
void GrobHashToIndexArray (TIndexVector& indArrayInOut,
                           const GrobHash& hash,
                           grob_t grobType)
{
	using iter_t = GrobHash::const_iterator;
	const iter_t iend = hash.end();
	for (iter_t igrob = hash.begin(); igrob != iend; ++igrob) {
		const Grob& grob = *igrob;
		if (grob.grob_type () == grobType) {
			for(index_t i = 0; i < grob.num_corners(); ++i) {
				indArrayInOut.push_back (grob.corner (i));
			}
		}
	}
}

template <class TIndexVector>
void UniqueSidesToIndexArray (TIndexVector& indArrayInOut,
                              const index_t* cornerInds,
                              const index_t numCornerInds,
                              const grob_t grobType,
                              const index_t sideDim)
{
	GrobHash hash;
	FindUniqueSides (hash, cornerInds, numCornerInds, grobType, sideDim);
	GrobHashToIndexArray (indArrayInOut, hash);
}


template <class TIndexVector>
void CreateAssociatedElemMap (TIndexVector& elemMapOut,
                        	  TIndexVector& offsetsOut,
                        	  index_t* grobBaseIndsOut,
                        	  Mesh& mesh,
                        	  GrobSet elemTypes,
                        	  GrobSet assElemTypes)
{
	elemMapOut.clear ();
	offsetsOut.clear ();

	
	const index_t elemDim = elemTypes.dim();
	const index_t assElemDim = assElemTypes.dim();

	const index_t numElems = mesh.num (elemTypes);
	offsetsOut.resize (numElems + 1, 0);

	// We need a hash map which tells us the index of each elem of type elemType
	GrobHashMap <index_t> elemIndHash;
	FillElemIndexMap (elemIndHash, grobBaseIndsOut, mesh, elemTypes);

	// Count how many associated elements each element has
	if (assElemDim > elemDim) {
		for (auto assElemType : assElemTypes) {
			for(auto grob : *mesh.inds (assElemType)) {
				for(index_t iside = 0; iside < grob.num_sides(elemDim); ++iside) {
					++offsetsOut[elemIndHash[grob.side (elemDim, iside)]];
				}
			}
		}
	}
	else {
		throw LumeError ("CreateAssociatedElemMap: Currently, nbr dimension > elem dimension has to hold true");
	}

	// Compute an offset into elemMapOut for each element (convert count -> offset)
	index_t offset = 0;
	for(index_t i = 0; i < offsetsOut.size(); ++i) {
		const index_t curOffset = offset;
		offset += offsetsOut[i];
		offsetsOut[i] = curOffset;
	}

	elemMapOut.resize (offsetsOut.back() * 2, NO_GROB);

	for (auto assElemType : assElemTypes) {
		index_t assGrobIndex = 0;
		for(auto assGrob : *mesh.inds (assElemType)) {
			for(index_t iside = 0; iside < assGrob.num_sides(elemDim); ++iside) {
				const index_t eind = elemIndHash[assGrob.side (elemDim, iside)];
				const index_t offset = 2 * offsetsOut [eind];
				const index_t numAss = offsetsOut [eind + 1] - offsetsOut [eind];
				for(index_t j = offset; j < offset + 2 * numAss; j+=2) {
					if (elemMapOut [j] == NO_GROB){
						elemMapOut [j] = assElemType;
						elemMapOut [j+1] = assGrobIndex;
						break;
					}
				}
			}
			++assGrobIndex;
		}
	}
}

}//	end of namespace lume

#endif	//__H__lume_topology_impl
