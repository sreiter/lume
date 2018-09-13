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


#include <algorithm>
#include <iostream>
#include "lume/grob.h"

using namespace std;

namespace lume {
namespace impl {

void PrintGrobDescs ()
{
	cout << "Overview over all available grid objects:\n\n";
	for(index_t igrob = 0; igrob < NO_GROB; ++igrob) {
		GrobDesc desc (static_cast<grob_t> (igrob));

		cout << igrob + 1 << ": " << desc.name().c_str() << endl;
		cout << "dim: " << desc.dim() << endl;

		for(index_t sideDim = 0; sideDim < desc.dim(); ++sideDim) {
			const index_t numSides = desc.num_sides (sideDim);
			cout << "side set of dim " << sideDim << ": " << GrobSetName (desc.side_set_type (sideDim)).c_str() << endl;
			cout << "sides of dim " << sideDim << ": " << numSides << endl;
			for(index_t iside = 0; iside < numSides; ++iside) {
				cout << "  side " << iside << ": type = " <<
				    desc.side_desc (sideDim, iside).name().c_str() << ", on corners:";

				const index_t* corners = desc.local_side_corners (sideDim, iside);
				for(index_t i = 0; i < desc.num_side_corners (sideDim, iside); ++i)
				{
					cout << " " << corners[i];
				}
				cout << endl;
			}
		}
		cout << endl;
	}
}

void PrintGrobSetDescs ()
{
	cout << "Overview over all available grid objects sets:\n\n";
	for(index_t igrobSet = 0; igrobSet <= CELLS; ++igrobSet) {
		GrobSet gs (static_cast<grob_set_t> (igrobSet));

		cout << igrobSet + 1 << ": " << gs.name().c_str() << endl;
		cout << "dim:  " << gs.dim() << endl;
		cout << "size: " << gs.size() << endl;

		cout << "grobs:";
		
		for (auto gt : gs)
			cout << " " << GrobName (gt).c_str();

		cout << endl;

		for(index_t sideDim = 0; sideDim < gs.dim(); ++sideDim) 
			cout << "side set " << sideDim << "D: " << GrobSetName (gs.side_set(sideDim)).c_str() << endl;

		cout << endl;
	}
}

}// end of namespace impl

const std::string& GrobName (grob_t grob)
{
	static const std::string names [] = {
		std::string ("vertex"),
		std::string ("edge"),
		std::string ("tri"),
		std::string ("quad"),
		std::string ("tet"),
		std::string ("hex"),
		std::string ("pyra"),
		std::string ("prism"),
		std::string ("invalid")
	};

	return names[grob];
}

const std::string& GrobSetName (grob_set_t grobSet)
{
	static const std::string names [] = {
		std::string ("vertices"),
		std::string ("edges"),
		std::string ("tris"),
		std::string ("quads"),
		std::string ("tets"),
		std::string ("hexs"),
		std::string ("pyras"),
		std::string ("prisms"),
		std::string ("invalid"),
		std::string ("faces"),
		std::string ("cells"),
	};

	return names[grobSet];
}


grob_set_t GrobSetTypeByDim (index_t dim)
{
	if (dim > MAX_GROB_DIM)
		return NO_GROB_SET;

	return impl::GROB_SET_TYPE_BY_DIM [dim];
}

}// end of namespace lume