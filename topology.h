#ifndef __H__slimesh__topology
#define __H__slimesh__topology

#include <utility>
#include "array_annex.h"
#include "grob.h"
#include "grob_hash.h"
#include "grob_index.h"

namespace slimesh {

class Mesh;

/// Maps consecutively indexed grid objects of different types to their respective grob indices
/** While slimesh indices grid objects for each type starting from zero, other
 * indexing schemata are possible. E.g. first indexing all triangles and afterwards
 * all quadrilaterals in a consecutive way (the first quad index is thus equal to
 * the number of triangles). If such an indexing scheme is encountered, this class
 * allows to map those indices to slimesh's indexing scheme.*/
class TotalToGrobIndexMap {
public:
  TotalToGrobIndexMap (Mesh& mesh, const GrobSet& gs);
  std::pair <index_t, grob_t> operator () (const index_t ind) const;

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
void FillElemIndexMap (GrobHashMap <index_t>& indexMapInOut,
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
void FillElemIndexMap (GrobHashMap <GrobIndex>& indexMapInOut,
                       const Mesh& mesh,
                       const GrobSet grobSet);
/** \} */


void ComputeGrobValences (GrobHashMap <index_t>& valencesOut,
                          Mesh& mesh,
                     	  GrobSet grobs,
                     	  GrobSet nbrGrobs);

/// Collects all sides of the *grobs* specified by `cornerInds` and `grobType`.
/** \note It is assumed, that `cornerInds` holds the corner indices of one or
*			    more *grobs* of the same `grobType` in sequential order.
* \note  `sideHashInOut is not cleared during this function. It is thus possible
*		     call this method repeatedly on different `cornerInds` and `grobType` to
*		     find all sides of a hybrid grid.*/
index_t FindUniqueSides (GrobHash& sideHashInOut,
                        const index_t* cornerInds,
                        const index_t numCornerInds,
                        const grob_t grobType,
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

/// Creates edge indices for a mesh with face or volume elements
void CreateEdgeInds (Mesh& mesh);

/// Creates faces indices for a mesh with volume elements
/** \todo:  add support for elements with quadrilateral sides!*/
void CreateFaceInds (Mesh& mesh);

SPMesh CreateBoundaryMesh (SPMesh mesh, GrobSet grobSet, const bool* visibilities = nullptr);

/**
 * \param grobBaseIndsOut Array of size `NUM_GROB_TYPES`.
 */
template <class TIndexVector>
void CreateAssociatedElemMap (TIndexVector& elemMapOut,
                              TIndexVector& offsetsOut,
                              index_t* grobBaseIndsOut,
                              Mesh& mesh,
                              GrobSet elemSet,
                              GrobSet assElemSet);



}//	end of namespace slimesh


// INCLUDE IMPLEMENTATION
#include "topology_impl.h"

#endif	//__H__slimesh__topology
