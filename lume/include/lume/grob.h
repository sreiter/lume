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


#ifndef __H__lume__grob
#define __H__lume__grob

#include <array>
#include <cassert>
#include <cstdint>
#include <string>
#include "grob_desc.h"
#include "types.h"
#include "impl/array_16_4.h"

namespace lume {

class Grob;

// TODO: CREATE CONST GROB
/// A ConstGrob represents a const reference to a **grid object**, specified by its corner indices.
/** To create a `ConstGrob`, pass the `grobType` and an array of corner-indices
 * (`corners`) to the constructor. `corners`has to have at least length
 * `GridDesc(grobType).num_corners()`.
 *
 * \warning Since a `ConstGrob` instance stores a pointer to the specified array
 *      of corners, an instance is invalidated once the pointer does no longer
 *      point to a valid memory location (e.g. because the underlying corner
 *      array has been resized). A `ConstGrob` thus should only be used as a
 *      temporary object and should only be used if it is clear that the
 *      underlying coner-array is still valid.
 */   
class ConstGrob
{
public:
  /// Defines the maximum number of corners that any `Grob` may have.
  /** For all Grobs `g` holds: `g.num_grobs() <= Grob::maxNumCorners`.
      \note This limitation arises from the use of Array_16_4 which is used to
            store local corner indices*/
  static constexpr index_t maxNumCorners = 16;

  using CornerIndexContainer = std::array <index_t, maxNumCorners>;
public:
  ConstGrob (GrobType grobType, index_t const* corners = nullptr);
  ConstGrob (ConstGrob const& other);
  ConstGrob (Grob const& other);

  /// Creates a `Grob` from an indexArray and a table of offsets for each corner into that array.
  ConstGrob (GrobType grobType, index_t const* globCornerInds, const impl::Array_16_4& cornerOffsets);

  /// Resets the grob so that it now operates as a reference to the data of `other`.
  /** \{ */
  void reset (Grob const& other);
  void reset (ConstGrob const& other);
  /** \} */

  void set_global_corner_array (index_t const* cornerArray);

  ///  only compares corners, ignores order and orientation.
  bool operator == (const ConstGrob& g) const;
  bool operator != (const ConstGrob& g) const;

  index_t operator [] (const index_t i) const;

  index_t dim () const;
  
  GrobType grob_type () const;
  
  GrobDesc desc () const;

  index_t const* global_corner_array () const;

  index_t num_corners () const;

  /// returns the global index of the i-th corner
  index_t corner (const index_t i) const;

  /// collects the global indices of corners
  /** \param cornersOut  Array of size `Grob::max_num_corners(). Only the first
     *                      `Grob::num_corners()` entries are filled
   *
   * \returns  The number of corners of the specified side
   */
  index_t collect_corners (CornerIndexContainer& cornersOut) const;

  index_t num_sides (const index_t sideDim) const;
  
  GrobDesc side_desc (const index_t sideDim, const index_t sideIndex) const;

  ConstGrob side (const index_t sideDim, const index_t sideIndex) const;

  /// returns the index of the side which corresponds to the given grob
  /** if no such side was found, 'lume::NO_INDEX' is returned.*/
  index_t find_side (const ConstGrob& sideGrob) const;

  /** Returns a special array object which contains for each corner the offset into the
    underlying globalCornerInds array.
    \note This method is typically not relevant for users of the library.*/
  const impl::Array_16_4& corner_offsets () const;

private:
  index_t const*   m_globCornerInds {nullptr};
  impl::Array_16_4 m_cornerOffsets;
  GrobDesc         m_desc {lume::VERTEX};
};

/// A Grob represents a reference to a **grid object**, specified by its corner indices.
/** To create a `Grob`, pass the `grobType` and an array of corner-indices
 * (`corners`) to the constructor. `corners`has to have at least length
 * `GridDesc(grobType).num_corners()`.
 *
 * A Grob behaves like a reference object, i.e., assigning corners (or another grob)
 * will change the content of the underlying corner array.
 *
 * \warning Since a `Grob` instance stores a pointer to the specified array
 *      of corners, an instance is invalidated once the pointer does no longer
 *      point to a valid memory location (e.g. because the underlying corner
 *      array has been resized). A `Grob` thus should only be used as a
 *      temporary object and should only be used if it is clear that the
 *      underlying coner-array is still valid.
 */ 
class Grob
{
public:
  /// Defines the maximum number of corners that any `Grob` may have.
  /** For all Grobs `g` holds: `g.num_grobs() <= Grob::maxNumCorners`.
      \note This limitation arises from the use of Array_16_4 which is used to
            store local corner indices*/
  static constexpr index_t maxNumCorners = 16;

  using CornerIndexContainer = std::array <index_t, maxNumCorners>;

public:
  Grob (GrobType grobType, index_t* corners = nullptr);
  Grob (Grob const& other);

  /// Creates a `Grob` from an indexArray and a table of offsets for each corner into that array.
  Grob (GrobType grobType, index_t* indexArray, const impl::Array_16_4& cornerOffsets);

  /// Resets the grob so that it now operates as a reference to the data of `other`.
  void reset (Grob const& other);

  void set_global_corner_array (index_t* cornerArray);

  Grob& operator = (Grob const& other);
  Grob& operator = (ConstGrob const& other);

  /// only compares corners, ignores order and orientation.
  bool operator == (const Grob& g) const;
  bool operator == (const ConstGrob& g) const;
  bool operator != (const Grob& g) const;
  bool operator != (const ConstGrob& g) const;

  index_t operator [] (const index_t i) const;

  index_t dim () const;
  
  GrobType grob_type () const;
  
  GrobDesc desc () const;

  const index_t* global_corner_array () const;

  index_t* global_corner_array ();

  index_t num_corners () const;

  /// returns the global index of the i-th corner
  index_t corner (const index_t i) const;

  /// sets the point index of the i-th corner
  void set_corner (const index_t cornerIndex, const index_t pointIndex);

  /// collects the global indices of corners
  /** \param cornersOut Array of size `Grob::max_num_corners(). Only the first
     *                      `Grob::num_corners()` entries are filled
   *
   * \returns The number of corners of the specified side
   */
  index_t collect_corners (CornerIndexContainer& cornersOut) const;

  index_t num_sides (const index_t sideDim) const;
  
  GrobDesc side_desc (const index_t sideDim, const index_t sideIndex) const;

  Grob side (const index_t sideDim, const index_t sideIndex) const;

  /// returns the index of the side which corresponds to the given grob
  /** if no such side was found, 'lume::NO_INDEX' is returned.*/
  index_t find_side (const Grob& sideGrob) const;

  const impl::Array_16_4& corner_offsets () const;

private:
  /** The data stored in the ConstGrob will be used here even for non-const operations.
    This reduces code duplication and is save, since the internal ConstGrob is solely used
    in this non-const environment.*/ 
  ConstGrob m_constGrob;
};
}//  end of namespace lume

#endif  //__H__lume__grob
