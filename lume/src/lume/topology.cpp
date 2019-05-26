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
#include "lume/math/vector_math.h"

//todo: remove this include
#include "lume/neighborhoods.h"

using namespace std;

namespace lume {

TotalToGrobIndexMap::
TotalToGrobIndexMap (Mesh& mesh, const GrobSet& gs)
{
	m_grobTypes.reserve (gs.size());
	for(auto grobType : gs)
		m_grobTypes.push_back (grobType);
	
	generate_base_inds (mesh);
}


TotalToGrobIndexMap::
TotalToGrobIndexMap (Mesh& mesh, const std::vector <GrobType>& gs) :
	m_grobTypes (gs)
{
	generate_base_inds (mesh);
}

TotalToGrobIndexMap::
TotalToGrobIndexMap (Mesh& mesh, std::vector <GrobType>&& gs) :
	m_grobTypes (std::move (gs))
{
	generate_base_inds (mesh);
}


void TotalToGrobIndexMap::
generate_base_inds (Mesh& mesh)
{
	m_baseInds.resize(m_grobTypes.size() + 1);
    m_baseInds[0] = 0;
    const size_t numGrobTypes = m_grobTypes.size();
    for(size_t i = 0; i < numGrobTypes; ++i)
     	m_baseInds [i+1] = m_baseInds [i] + static_cast <index_t> (mesh.num (m_grobTypes [i]));
}

GrobIndex TotalToGrobIndexMap::
operator () (const index_t ind) const
{
	const size_t numGrobTypes = m_grobTypes.size();
    for(size_t i = 0; i < numGrobTypes; ++i) {
      	if (ind >= m_baseInds [i] && ind < m_baseInds [i+1])
        	return GrobIndex(m_grobTypes[i], ind - m_baseInds[i]);
    }

    throw LumeError () << "TotalToGrobIndexMap: Couldn't map index " << ind;
    return GrobIndex ();
}



void FillGrobToIndexMap (GrobHashMap <index_t>& indexMapInOut,
                       index_t* grobBaseIndsOut,
                       const Mesh& mesh,
                       const GrobSet grobSet)
{
	math::raw::VecSet (grobBaseIndsOut, NUM_GROB_TYPES, NO_INDEX);

	index_t counter = 0;
	
	indexMapInOut.reserve (mesh.num (grobSet));
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
	indexMapInOut.reserve (mesh.num (grobSet));
	
	for (auto grobType : grobSet) {
		if (!mesh.has (grobType))
			continue;
		
		index_t counter = 0;

		for(auto grob : mesh.grobs (grobType)) {
			indexMapInOut.insert (make_pair (grob, GrobIndex (grobType, counter++)));
		}
	}
}


GrobHashMap <index_t> ComputeGrobValences (const Mesh& mesh,
                     	                   GrobSet grobs,
                     	                   GrobSet nbrGrobs)
{
	GrobHashMap <index_t> valences;

	const index_t grobDim = grobs.dim();
	const index_t nbrGrobDim = nbrGrobs.dim();

	if (grobDim < nbrGrobDim) {
		// initialize all grob valences with 0
		for(auto gt : grobs) {
			for(auto grob : mesh.grobs (gt)) {
				valences [grob] = 0;
			}
		}

		for(auto nbrGT : nbrGrobs) {
			for(auto nbrGrob : mesh.grobs (nbrGT)) {
				for(index_t iside = 0; iside < nbrGrob.num_sides (grobDim); ++iside) {
					++valences [nbrGrob.side (grobDim, iside)];
				}
			}
		}
	}
	else if (grobDim > nbrGrobDim) {
		for(auto gt : grobs) {
			for(auto grob : mesh.grobs (gt)) {
				valences [grob] = grob.num_sides (nbrGrobDim);
			}
		}
	}
	else
		throw LumeError () << "ComputeGrobValences is currently not implemented for grobs.dim() == nbrGrobs.dim(). Sorry.";
    
    return valences;
}

std::vector <index_t> ValenceHistogram (const Mesh& mesh, GrobSet grobs, GrobSet nbrGrobs)
{
    GrobHashMap <index_t> valences = ComputeGrobValences (mesh, grobs, nbrGrobs);

    std::vector <index_t> histogram;

    for (const auto& entry : valences)
    {
        const index_t valence = entry.second;

        if (valence >= static_cast <index_t> (histogram.size ()))
        {
            histogram.resize (valence + 1, 0);
        }

        ++histogram [valence];
    }
    
    return histogram;
}

index_t FindUniqueSides (GrobHash& sideHashInOut,
                         const Mesh& mesh,
                         const GrobSet grobSet,
                         const index_t sideDim)
{
	index_t numInsertions = 0;

	for(auto grobType : grobSet) {
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

index_t FindUniqueSidesNumbered (GrobHashMap <index_t>& hashMapInOut,
                                 const Mesh& mesh,
                                 const GrobSet grobSet,
                                 const index_t sideDim,
                                 const index_t indexOffset)
{
    index_t numInsertions = 0;
    index_t const startIndex = hashMapInOut.size () + indexOffset;
    for(auto grobType : grobSet) {
        const index_t numSides = GrobDesc (grobType).num_sides (sideDim);

        for(auto grob : mesh.grobs (grobType)) {
            for(index_t iside = 0; iside < numSides; ++iside) {
                auto const entry = std::make_pair (grob.side (sideDim, iside), startIndex + numInsertions);
                const auto r = hashMapInOut.insert(entry);
                numInsertions += static_cast<index_t> (r.second);
            }
        }
    }

    return numInsertions;
}

index_t InsertGrobsNumbered (GrobHashMap <index_t>& hashMapInOut,
                             Mesh& mesh,
                             const GrobSet grobSet,
                             const index_t indexOffset)
{
    index_t numInsertions = 0;
    index_t const startIndex = hashMapInOut.size () + indexOffset;
    for(auto grobType : grobSet) {
        for(auto grob : mesh.grobs (grobType)) {
            auto const entry = std::make_pair (grob, startIndex + numInsertions);
            const auto r = hashMapInOut.insert(entry);
            numInsertions += static_cast<index_t> (r.second);
        }
    }

    return numInsertions;
}

void CreateSideGrobs (Mesh& mesh, const index_t sideDim)
{
	const std::vector<GrobType> grobs = mesh.grob_types();

	GrobHash hash;
	for(auto gt : grobs) {
		if(GrobDesc(gt).dim() > sideDim)
			FindUniqueSides (hash, mesh, gt, sideDim);
	}
	
	mesh.clear (GrobSetTypeByDim (sideDim));
	mesh.insert_grobs (hash.begin(), hash.end());
}


}//	end of namespace
