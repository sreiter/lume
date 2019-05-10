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

#ifndef __H__lume__topology
#define __H__lume__topology

#include <utility>
#include "array_annex.h"
#include "grob.h"
#include "grob_hash.h"
#include "grob_index.h"

namespace lume {

class Mesh;

/// Counts the number of 

/// Maps consecutively indexed grid objects of different types to their respective grob indices
/** While slimesh indices grid objects for each type starting from zero, other
 * indexing schemata are possible. E.g. first indexing all triangles and afterwards
 * all quadrilaterals in a consecutive way (the first quad index is thus equal to
 * the number of triangles). If such an indexing scheme is encountered, this class
 * allows to map those indices to slimesh's indexing scheme.*/
class TotalToGrobIndexMap {
public:
	TotalToGrobIndexMap (Mesh& mesh, const GrobSet& gs);
	TotalToGrobIndexMap (Mesh& mesh, const std::vector <GrobType>& gs);
	TotalToGrobIndexMap (Mesh& mesh, std::vector <GrobType>&& gs);

	GrobIndex operator () (const index_t ind) const;

private:
	void generate_base_inds (Mesh& mesh);

	std::vector <index_t>	m_baseInds;
	std::vector <GrobType>	m_grobTypes;
};


/// Fills a map which associates grobs, each specified by a sequence of vertex indices with consecutive indices
/**
* \param grobBaseIndsOut Array of size `NUM_GROB_TYPES`.
*
* \note   It is assumed, that `cornerInds` holds the corner indices of one or
*         more *grobs* of the same `grobType` in sequential order.
*
* \note `indexMapInOut is not cleared during this function. It is thus possible
*        call this method repeatedly on different `cornerInds` and `grobType` to
*        find fill all element indices of a hybrid grid.
* \{ */
void FillGrobToIndexMap (GrobHashMap <index_t>& indexMapInOut,
                       index_t* grobBaseIndsOut,
                       const Mesh& mesh,
                       const GrobSet grobSet);
/** \} */


/// Fills a map which associates grobs, each specified by a sequence of vertex indices with GrobIndices
/**
* \note `indexMapInOut is not cleared during this function. It is thus possible
*        call this method repeatedly on different `grobSets` to
*        find fill all element indices of a hybrid grid.
* \{ */
void FillGrobToIndexMap (GrobHashMap <GrobIndex>& indexMapInOut,
                       const Mesh& mesh,
                       const GrobSet grobSet);
/** \} */


/// Creates a hash map which stores the number of `nbrGrobs` for each `grob`.
GrobHashMap <index_t> ComputeGrobValences (const Mesh& mesh,
                     	                   GrobSet grobs,
                     	                   GrobSet nbrGrobs);

/// Returns a vector which stores at position `i` the number of `grobs` with `i` `nbrGrobs`.
std::vector <index_t> ValenceHistogram (const Mesh& mesh, GrobSet grobs, GrobSet nbrGrobs);

/// Collects all sides of the specified *sideDim* of the *grobs* in the specified grob set.
/** \note  `sideHashInOut` is not cleared during this function. It is thus possible to
*		     call this method repeatedly on different grobSets to
*		     find all sides of a hybrid grid.
* \returns  The number of newly inserted grobs.*/
index_t FindUniqueSides (GrobHash& sideHashInOut,
                         Mesh& mesh,
                         const GrobSet grobSet,
                         const index_t sideDim);

/// Collects all sides of the specified *sideDim* of the *grobs* in the specified grob set.
/** The grobs are numbered in sequential order according to when they were first encountered
* during iteration over the grob set. The index is derivied from the size of the provided
* *hashMapInOut* at the start of the algorithm plus the provided indexOffset.
*
* \note  `hashMapInOut` is not cleared during this function. It is thus possible to
*         call this method repeatedly on different grobSets to
*         find all sides of a hybrid grid.
* \returns  The number of newly inserted grobs.*/
index_t FindUniqueSidesNumbered (GrobHashMap <index_t>& hashMapInOut,
                                 Mesh& mesh,
                                 const GrobSet grobSet,
                                 const index_t sideDim,
                                 const index_t indexOffset = 0);

/// Inserts all *grobs* in the specified grob set into the provided *hashMapInOut*.
/** The grobs are numbered in sequential order according to when they were first encountered
* during iteration over the grob set. The index is derivied from the size of the provided
* *hashMapInOut* at the start of the algorithm plus the provided indexOffset.
*
* \note  `hashMapInOut` is not cleared during this function. It is thus possible to
*         call this method repeatedly on different grobSets to
*         find all sides of a hybrid grid.
* \returns  The number of newly inserted grobs.*/
index_t InsertGrobsNumbered (GrobHashMap <index_t>& hashMapInOut,
                             Mesh& mesh,
                             const GrobSet grobSet,
                             const index_t indexOffset = 0);

template <class TIndexVector>
void GrobHashToIndexArray (TIndexVector& indArrayInOut,
                           const GrobHash& hash);

template <class TIndexVector>
void GrobHashToTypeArray (TIndexVector& typeArrayInOut,
                          const GrobHash& hash);

template <class TIndexVector>
void GrobHashToIndexArray (TIndexVector& indArrayInOut,
                           const GrobHash& hash,
                           GrobType grobType);


/// Creates grobs for all sides of the specified dimension
void CreateSideGrobs (Mesh& mesh, const index_t sideDim);


}//	end of namespace lume


// INCLUDE IMPLEMENTATION
#include "topology_impl.h"

#endif	//__H__lume__topology
