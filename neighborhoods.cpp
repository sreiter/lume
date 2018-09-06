// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2017 Sebastian Reiter, G-CSC Frankfurt <s.b.reiter@gmail.com>

#include "neighborhoods.h"
#include "topology.h"
#include <limits>

using namespace std;

namespace slimesh {

Neighborhoods::
Neighborhoods ()
{
	for(index_t i = 0; i < MAX_GROB_SET_SIZE; ++i)
		m_grobBaseInds[i] = 0;
}

Neighborhoods::
Neighborhoods (SPMesh mesh, GrobSet centerGrobTypes, GrobSet neighborGrobTypes)
{
	refresh (mesh, centerGrobTypes, neighborGrobTypes);
}


void Neighborhoods::
refresh ()
{
	if (m_mesh) {
		m_nbrs.set_tuple_size (2);
		CreateAssociatedElemMap (m_nbrs, m_offsets, m_grobBaseInds, *m_mesh,
		                         m_centerGrobTypes, m_neighborGrobTypes);
	}
	else {
		m_nbrs.clear();
		m_offsets.clear();
		m_centerGrobTypes = NO_GROB_SET;
		m_neighborGrobTypes = NO_GROB_SET;
	}
}

void Neighborhoods::
refresh (SPMesh mesh, GrobSet centerGrobTypes, GrobSet neighborGrobTypes)
{
	m_mesh = mesh;
	m_centerGrobTypes = centerGrobTypes;
	m_neighborGrobTypes = neighborGrobTypes;
	refresh();
}


SPMesh Neighborhoods::
mesh ()
{
	return m_mesh;
}


Neighbors Neighborhoods::
neighbors (const GrobIndex gi) const
{
	const index_t baseIndex = base_index (gi);

	if (baseIndex >= m_offsets.size())
		return Neighbors (0, nullptr);

	const index_t numNbrs = m_offsets [baseIndex + gi.index + 1]
							- m_offsets [baseIndex + gi.index];

	const index_t* firstNbr = m_nbrs.raw_ptr() + baseIndex * 2;

	return Neighbors (numNbrs, firstNbr);
}


index_t Neighborhoods::
base_index (const GrobIndex gi) const
{
	return m_grobBaseInds[gi.grobType];
}


}//	end of namespace slimesh
