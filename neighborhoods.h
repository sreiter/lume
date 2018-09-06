// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2018 Sebastian Reiter <s.b.reiter@gmail.com>

#ifndef __H__slimesh_neighborhoods
#define __H__slimesh_neighborhoods

#include "grob_index.h"
#include "mesh.h" // use forward instead
#include "neighbors.h"

namespace slimesh {


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

}//	end of namespace slimesh

#endif	//__H__slimesh_neighborhoods
