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


#include "neighborhoods.h"
#include "topology.h"
#include <limits>

using namespace std;

namespace lume {

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


}//	end of namespace lume
