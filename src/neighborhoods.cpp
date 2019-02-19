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


#include "lume/neighborhoods.h"
#include "lume/topology.h"
#include "pettyprof/pettyprof.h"
#include <limits>

using namespace std;

namespace lume {

Neighborhoods::
Neighborhoods ()
{
	for(index_t i = 0; i < NUM_GROB_TYPES; ++i)
		m_grobBaseInds[i] = 0;

	m_centerGrobTypes = NO_GROB_SET;
	m_neighborGrobTypes = NO_GROB_SET;
}


Neighborhoods::
Neighborhoods (SPMesh mesh, GrobSet centerGrobTypes, GrobSet neighborGrobTypes)
{
	refresh (mesh, centerGrobTypes, neighborGrobTypes);
}


Neighborhoods::
Neighborhoods (SPMesh mesh, GrobSet grobTypes, const Neighborhoods& grobConnections)
{
	refresh (mesh, grobTypes, grobConnections);
}

void Neighborhoods::
refresh (SPMesh mesh, GrobSet centerGrobTypes, GrobSet neighborGrobTypes)
{
	PEPRO_BEGIN(Neighborhoods__refresh);
	
	m_mesh = mesh;
	m_centerGrobTypes = centerGrobTypes;
	m_neighborGrobTypes = neighborGrobTypes;

	impl::FillNeighborMap (m_nbrs, m_offsets, m_grobBaseInds, *m_mesh,
	                       m_centerGrobTypes, m_neighborGrobTypes);
}


void Neighborhoods::
refresh (SPMesh mesh, GrobSet grobTypes, const Neighborhoods& grobConnections)
{
	PEPRO_BEGIN(Neighborhoods__refresh);

	m_mesh = mesh;
	m_centerGrobTypes = grobTypes;
	m_neighborGrobTypes = grobTypes;

	impl::FillNeighborMap (m_nbrs, m_offsets, m_grobBaseInds, *m_mesh,
	                       grobTypes, grobConnections);
}


SPMesh Neighborhoods::
mesh ()
{
	return m_mesh;
}


NeighborIndices Neighborhoods::
neighbor_indices (const GrobIndex gi) const
{
	const index_t baseIndex = base_index (gi);

	if (baseIndex >= m_offsets.size())
		throw LumeError (std::string("This Neighborhoods instance doesn't provide "
		                             "neighbors for grobs of type ")
						.append (GrobTypeName (gi.grob_type ())));

	return NeighborIndices (gi, this);
}

NeighborGrobs Neighborhoods::
neighbor_grobs (const GrobIndex gi) const
{
	return NeighborGrobs (neighbor_indices (gi));
}

index_t Neighborhoods::
num_neighbors (const GrobIndex gi) const
{
	const index_t oi = offset_index(gi);
	return m_offsets [oi + 1] - m_offsets [oi];
}

index_t Neighborhoods::
base_index (const GrobIndex gi) const
{
	return m_grobBaseInds[gi.grob_type ()];
}

index_t Neighborhoods::
offset_index (const GrobIndex& gi) const
{
	return base_index (gi) + gi.index ();
}

const index_t* Neighborhoods::first_neighbor (const GrobIndex& gi) const
{
	const index_t o = m_offsets [offset_index (gi)] * 2;
	return m_nbrs.data() +  o;
}


}//	end of namespace lume
