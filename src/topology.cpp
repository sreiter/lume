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


#include "lume/mesh.h"
#include "lume/topology.h"
#include "lume/vec_math_raw.h"

//todo: remove this include
#include "lume/neighborhoods.h"

using namespace std;

namespace lume {

namespace impl {
	void GenerateVertexIndicesFromCoords (Mesh& mesh)
	{
		GrobArray& vrts = mesh.grobs (VERTEX);
		const index_t oldNumVrts = vrts.size();
		const index_t newNumVrts = mesh.coords()->num_tuples();
		if (newNumVrts > oldNumVrts){
			vrts.reserve (newNumVrts);
			for(index_t i = oldNumVrts; i < newNumVrts; ++i)
				vrts.push_back ({i});
		}
		else
			vrts.resize (newNumVrts);
	}
}// end of namespace impl

TotalToGrobIndexMap::
TotalToGrobIndexMap (Mesh& mesh, const GrobSet& gs) :
    m_grobSet (gs)
{
    if (gs.size() > MAX_GROB_SET_SIZE)
    	LumeError ("Internal error: MAX_GROB_SET_SIZE is wrong!");

    m_baseInds[0] = 0;
    for(index_t i = 0; i < gs.size(); ++i) {
     	m_baseInds [i+1] = m_baseInds [i] + mesh.num (gs.grob_type (i));
    }
}

GrobIndex TotalToGrobIndexMap::
operator () (const index_t ind) const
{
    for(size_t i = 0; i < m_grobSet.size(); ++i) {
      	if (ind >= m_baseInds [i] && ind < m_baseInds [i+1])
        	return GrobIndex(m_grobSet.grob_type(index_t(i)), ind - m_baseInds[i]);
    }

    throw LumeError (string("TotalToGrobIndexMap: Couldn't map index ").
    					append (to_string(ind)).
    					append (" to GrobSet ").
    					append (m_grobSet.name()));
    return GrobIndex (NO_GROB, 0);
}



void FillGrobToIndexMap (GrobHashMap <index_t>& indexMapInOut,
                       index_t* grobBaseIndsOut,
                       const Mesh& mesh,
                       const GrobSet grobSet)
{
	VecSet (grobBaseIndsOut, NUM_GROB_TYPES, NO_INDEX);

	index_t counter = 0;
	
	for (auto grobType : grobSet) {
		grobBaseIndsOut [grobType] = counter;

		if (!mesh.has (grobType))
			continue;

		for(auto grob : mesh.grobs (grobType)) {
			indexMapInOut.insert (make_pair (grob, counter++));
		}
	}
}


void FillGrobToIndexMap (GrobHashMap <GrobIndex>& indexMapInOut,
                       const Mesh& mesh,
                       const GrobSet grobSet)
{
	for (auto grobType : grobSet) {
		if (!mesh.has (grobType))
			continue;
		
		index_t counter = 0;

		for(auto grob : mesh.grobs (grobType)) {
			indexMapInOut.insert (make_pair (grob, GrobIndex (grobType, counter++)));
		}
	}
}


void ComputeGrobValences (GrobHashMap <index_t>& valencesOut,
                          Mesh& mesh,
                     	  GrobSet grobs,
                     	  GrobSet nbrGrobs)
{
	valencesOut.clear ();

	const index_t grobDim = grobs.dim();
	const index_t nbrGrobDim = nbrGrobs.dim();

	if (grobDim < nbrGrobDim) {
		// initialize all grob valences with 0
		for(auto gt : grobs) {
			for(auto grob : mesh.grobs (gt)) {
				valencesOut [grob] = 0;
			}
		}

		for(auto nbrGT : nbrGrobs) {
			for(auto nbrGrob : mesh.grobs (nbrGT)) {
				for(index_t iside = 0; iside < nbrGrob.num_sides (grobDim); ++iside) {
					++valencesOut [nbrGrob.side (grobDim, iside)];
				}
			}
		}
	}
	else if (grobDim > nbrGrobDim) {
		for(auto gt : grobs) {
			for(auto grob : mesh.grobs (gt)) {
				valencesOut [grob] = grob.num_sides (nbrGrobDim);
			}
		}
	}
	else
		throw LumeError ("ComputeGrobValences is currently not implemented for grobs.dim() == nbrGrobs.dim(). Sorry.");
}


index_t FindUniqueSides (GrobHash& sideHashInOut,
                         Mesh& mesh,
                         const GrobSet grobSet,
                         const index_t sideDim)
{
	index_t numInsertions = 0;

	for(auto grobType : grobSet) {
		const index_t numCorners = GrobDesc (grobType).num_corners ();
		const index_t numSides = GrobDesc (grobType).num_sides (sideDim);

		for(auto grob : mesh.grobs (grobType)) {
			for(index_t iside = 0; iside < numSides; ++iside) {
				const auto r = sideHashInOut.insert(grob.side (sideDim, iside));
				numInsertions += static_cast<index_t> (r.second);
			}
		}
	}

	return numInsertions;
}


void CreateSideGrobs (Mesh& mesh, const index_t sideDim)
{
	const std::vector<grob_t> grobs = mesh.grob_types();

	GrobHash hash;
	for(auto gt : grobs) {
		if(GrobDesc(gt).dim() > sideDim)
			FindUniqueSides (hash, mesh, gt, sideDim);
	}
	
	mesh.clear (GrobSetTypeByDim (sideDim));
	mesh.insert (hash.begin(), hash.end());
}


}//	end of namespace
