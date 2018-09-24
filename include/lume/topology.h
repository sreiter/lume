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

namespace impl {
	void GenerateVertexIndicesFromCoords (Mesh& mesh);
}// end of namespace impl


/// Maps consecutively indexed grid objects of different types to their respective grob indices
/** While slimesh indices grid objects for each type starting from zero, other
 * indexing schemata are possible. E.g. first indexing all triangles and afterwards
 * all quadrilaterals in a consecutive way (the first quad index is thus equal to
 * the number of triangles). If such an indexing scheme is encountered, this class
 * allows to map those indices to slimesh's indexing scheme.*/
class TotalToGrobIndexMap {
public:
  TotalToGrobIndexMap (Mesh& mesh, const GrobSet& gs);
  GrobIndex operator () (const index_t ind) const;

private:
  index_t m_baseInds [MAX_GROB_SET_SIZE + 1];
  const GrobSet m_grobSet;
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


/// Computes the number of neighbors of type `nbrGrobs` for each grob in `grobs`
/** The results are stored in the GrobHashMap `valencesOut`*/
void ComputeGrobValences (GrobHashMap <index_t>& valencesOut,
                          Mesh& mesh,
                     	  GrobSet grobs,
                     	  GrobSet nbrGrobs);

/// Collects all sides of the *grobs* specified by `cornerInds` and `grobType`.
/** \note It is assumed, that `cornerInds` holds the corner indices of one or
*			    more *grobs* of the same `grobType` in sequential order.
* \note  `sideHashInOut is not cleared during this function. It is thus possible
*		     call this method repeatedly on different grobSets to
*		     find all sides of a hybrid grid.*/
index_t FindUniqueSides (GrobHash& sideHashInOut,
                         Mesh& mesh,
                         const GrobSet grobSet,
                         const index_t sideDim);


template <class TIndexVector>
void GrobHashToIndexArray (TIndexVector& indArrayInOut,
                           const GrobHash& hash);

template <class TIndexVector>
void GrobHashToTypeArray (TIndexVector& typeArrayInOut,
                          const GrobHash& hash);

template <class TIndexVector>
void GrobHashToIndexArray (TIndexVector& indArrayInOut,
                           const GrobHash& hash,
                           grob_t grobType);

template <class TIndexVector>
void UniqueSidesToIndexArray (TIndexVector& indArrayInOut,
                              const index_t* cornerInds,
                              const index_t numCornerInds,
                              const grob_t grobType,
                              const index_t sideDim);

/// Creates grobs for all sides of the specified dimension
void CreateSideGrobs (Mesh& mesh, const index_t sideDim);


}//	end of namespace lume


// INCLUDE IMPLEMENTATION
#include "topology_impl.h"

#endif	//__H__lume__topology
