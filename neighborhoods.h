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


#ifndef __H__lume_neighborhoods
#define __H__lume_neighborhoods

#include "grob_index.h"
#include "mesh.h" // use forward instead
#include "neighbors.h"

namespace lume {


/// Gives access to the neighbors of all enteties of the mesh
class Neighborhoods {
public:
    Neighborhoods ();
    Neighborhoods (SPMesh mesh, GrobSet centerGrobTypes, GrobSet neighborGrobTypes);

    void refresh ();
    void refresh (SPMesh mesh, GrobSet centerGrobTypes, GrobSet neighborGrobTypes);

    SPMesh mesh ();

    Neighbors neighbors (const GrobIndex gi) const;

private:
	index_t base_index (const GrobIndex gi) const;

    IndexArrayAnnex m_offsets;
    IndexArrayAnnex m_nbrs;
    index_t         m_grobBaseInds [NUM_GROB_TYPES];
    SPMesh			m_mesh;
    GrobSet			m_centerGrobTypes;
    GrobSet			m_neighborGrobTypes;
};

}//	end of namespace lume

#endif	//__H__lume_neighborhoods
