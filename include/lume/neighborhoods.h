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


/// Gives access to the neighbors of all entities of the mesh
class Neighborhoods {
	friend class NeighborIndices;
	friend class NeighborGrobs;

public:

    Neighborhoods ();

    /// Creates a neighborhood between grobs of different type and dimension
    /** \note it has to hold true: centerGrobTypes != neighborGrobTypes
     * \{ */
    Neighborhoods (SPMesh mesh, GrobSet centerGrobTypes, GrobSet neighborGrobTypes);
    // void refresh (SPMesh mesh, GrobSet centerGrobTypes, GrobSet neighborGrobTypes);
    /** \} */

	/// Creates a neighborhood between grobs of the same type using the specified grobConnections as links
	/** To create a neighborhoods between faces, where two faces are neighbors if
	 * they are connected by a vertex, one could execute the following code:
	 * \code
	 * Neighborhoods faceNbrs (mesh, FACES, Neighborhoods (mesh, VERTICES, FACES));
	 * \endcode
	 *
	 * To receive a neighborhood between faces connected by their edges, this code would work:
	 * \code
	 * Neighborhoods faceNbrs (mesh, FACES, Neighborhoods (mesh, EDGES, FACES));
	 * \endcode
	 *
	 * \param neighborGrobTypes	the following properties have to hold true:
	 *									- `grobConnections.center_grob_types() != grobTypes`
	 *									- `grobConnections.neighbor_grob_types() == grobTypes`
	 * \{ */
    Neighborhoods (SPMesh mesh, GrobSet grobTypes, const Neighborhoods& grobConnections);
	// void refresh (SPMesh mesh, GrobSet grobTypes, const Neighborhoods& grobConnections);
	/** \} */

    SPMesh mesh ();

    NeighborIndices neighbor_indices (const GrobIndex gi) const;
    NeighborGrobs neighbor_grobs (const GrobIndex gi) const;

    index_t num_neighbors (const GrobIndex gi) const;

    GrobSet center_grob_set () const	{return m_centerGrobTypes;}
    GrobSet neighbor_grob_set () const	{return m_neighborGrobTypes;}
    
private:
	index_t base_index (const GrobIndex gi) const;
	index_t offset_index (const GrobIndex& gi) const;
	const index_t* first_neighbor (const GrobIndex& gi) const;

    IndexArrayAnnex m_offsets;
    IndexArrayAnnex m_nbrs;
    index_t         m_grobBaseInds [NUM_GROB_TYPES];
    SPMesh			m_mesh;
    GrobSet			m_centerGrobTypes;
    GrobSet			m_neighborGrobTypes;
};


namespace impl {
	/// Computes the offset for each grob in `grobs` into an neighborhood array
	/** The results are stored in the specified index array, based on the indices
	 * given in the `grobToIndexMap`. The `grobToIndexMap` can e.g. be created using
	 * the method `FillGrobToIndexMap` on the `grobs` grob set.
	 *
	 * \returns	an array of the size `mesh.num (grobs)`. For each grob, it stores
	 *			an offset into an array in which indices of neighboring grobs from
	 *			the set `nbrGrobs` can be stored.
	 *				- hint: `offsetsOut[i+1] - offsetsOut[i]` corresponds to the
	 *				  number of neighbors of the i'th grob.
	 *				- hint: `offsetsOut[mesh.num(grobs)]` or `offsetsOut.back()`
	 *				  corresponds to the sum of the neighborhood sizes from all
	 *				  grobs of type `grobs`.
	 *
	 * \warning	it is assumed that the highest index provided by `grobToIndexMap`
	 *			is smaller than `mesh.num(grobs)`.
	 *			
	 * \sa FillGrobToIndexMap */
	template <class TIndexVector>
	void FillHigherDimNeighborOffsetMap (TIndexVector& offsetsOut,
				                         Mesh& mesh,
				                     	 GrobSet grobSet,
				                     	 GrobSet nbrGrobSet,
				                     	 const GrobHashMap <index_t>& grobToIndexMap);

	template <class TIndexVector>
	void FillLowerDimNeighborOffsetMap (TIndexVector& offsetsOut,
				                        Mesh& mesh,
				                     	GrobSet grobSet,
				                     	GrobSet nbrGrobSet);

	template <class TIndexVector>
	void FillHigherDimNeighborMap (TIndexVector& nbrMapOut,
	                        	   TIndexVector& offsetsOut,
	                        	   index_t* grobBaseIndsOut,
	                        	   Mesh& mesh,
	                        	   GrobSet grobSet,
	                        	   GrobSet nbrGrobSet);

	template <class TIndexVector>
	void FillLowerDimNeighborMap (TIndexVector& nbrMapOut,
	                        	   TIndexVector& offsetsOut,
	                        	   index_t* grobBaseIndsOut,
	                        	   Mesh& mesh,
	                        	   GrobSet grobSet,
	                        	   GrobSet nbrGrobSet);

	/**
	 * \param grobBaseIndsOut Array of size `NUM_GROB_TYPES`.
	 */
	template <class TIndexVector>
	void FillNeighborMap (TIndexVector& elemMapOut,
                            TIndexVector& offsetsOut,
                            index_t* grobBaseIndsOut,
                            Mesh& mesh,
                            GrobSet elemSet,
                            GrobSet assElemSet);

	/**
	 * \param grobBaseIndsOut Array of size `NUM_GROB_TYPES`.
	 */
	template <class TIndexVector>
	void FillNeighborMap (TIndexVector& elemMapOut,
                            TIndexVector& offsetsOut,
                            index_t* grobBaseIndsOut,
                            Mesh& mesh,
                            GrobSet elemSet,
                            const Neighborhoods& grobConnections);
}

}//	end of namespace lume

////////////////////////////////
// include implementation
#include "neighborhoods_impl.hpp"

#endif	//__H__lume_neighborhoods
