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

#include "lume/rim_mesh.h"
#include "lume/neighborhoods.h"

namespace lume {

void CreateRimMesh (SPMesh rimMeshOut,
                      SPMesh mesh,
                      GrobSet grobSet,
                      const std::function <bool (const GrobIndex& gi)>& visFunc,
                      const std::function <void (const GrobIndex& rimGrob, const GrobIndex& srcGrob)>& gotRimGrobFunc,
                      const Neighborhoods* nbrhds)
{
	rimMeshOut->set_coords (mesh->coords());

	GrobSet rimGrobSet = grobSet.side_set ();

	Neighborhoods localNbrhds;
	if (!nbrhds) {
		localNbrhds.refresh (mesh, rimGrobSet, grobSet);
		nbrhds = &localNbrhds;
	}
	else if (nbrhds->center_grob_set() != rimGrobSet || nbrhds->neighbor_grob_set() != grobSet)
		throw LumeError ("CreateRimMesh can't operate on provided neighborhoods instance. ");
	
	const Neighborhoods& neighborhoods = *nbrhds;

	for(auto rimGrobType : rimGrobSet) {
		index_t counter = 0;
		for(auto rimGrob : mesh->grobs (rimGrobType)) {
			const GrobIndex rgi (rimGrobType, counter++);
			NeighborIndices nbrs = neighborhoods.neighbor_indices (rgi);
			index_t numVis = 0;
			GrobIndex visNbrGrobIndex;
			for(auto nbrGI : nbrs) {
				if (visFunc (nbrGI)){
					visNbrGrobIndex = nbrGI;
					++numVis;
				}
			}
			if (numVis == 1) {
				rimMeshOut->insert (rimGrob);
				gotRimGrobFunc (GrobIndex(rimGrobType, rimMeshOut->num (rimGrobType)),
				                visNbrGrobIndex);
			}
		}
	}
}

void CreateRimMesh (SPMesh rimMeshOut,
                      SPMesh mesh,
                      GrobSet grobSet,
                      const std::function <void (const GrobIndex& rimGrob, const GrobIndex& srcGrob)>& gotRimGrobFunc,
                      const Neighborhoods* nbrhds)
{
	CreateRimMesh (rimMeshOut, mesh, grobSet, [](const GrobIndex&){return true;},
	                 gotRimGrobFunc, nbrhds);
}


SPMesh CreateRimMesh (SPMesh mesh,
                      GrobSet grobSet,
                      const std::function <bool (const GrobIndex& gi)>& visFunc,
                      const std::function <void (const GrobIndex& rimGrob, const GrobIndex& srcGrob)>& gotRimGrobFunc,
                      const Neighborhoods* nbrhds)
{
	auto rimMesh = std::make_shared <Mesh> ();
	CreateRimMesh (rimMesh, mesh, grobSet, visFunc, gotRimGrobFunc, nbrhds);
	return rimMesh;
}


SPMesh CreateRimMesh (SPMesh mesh,
                      GrobSet grobSet,
                      const std::function <void (const GrobIndex& rimGrob, const GrobIndex& srcGrob)>& gotRimGrobFunc,
                      const Neighborhoods* nbrhds)
{
	auto rimMesh = std::make_shared <Mesh> ();
	CreateRimMesh (rimMesh, mesh, grobSet, gotRimGrobFunc, nbrhds);
	return rimMesh;
}



}//	end of namespace lume
