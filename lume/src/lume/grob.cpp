// This file is part of lume, a C++ library for lightweight unstructured meshes
//
// Copyright (C) 2018, 2019 Sebastian Reiter
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

#include <lume/grob.h>

namespace lume
{
ConstGrob::ConstGrob (GrobType grobType, index_t const* corners) :
  m_globCornerInds (corners),
  m_cornerOffsets (impl::Array_16_4::ascending_order ()),
  m_desc (grobType)
{}

ConstGrob::ConstGrob (ConstGrob const& other)
  : m_globCornerInds {other.m_globCornerInds}
  , m_cornerOffsets {other.m_cornerOffsets}
  , m_desc {other.m_desc}
{}

ConstGrob::ConstGrob (Grob const& other)
  : m_globCornerInds {other.global_corner_array ()}
  , m_cornerOffsets {other.corner_offsets ()}
  , m_desc {other.desc ()}
{}

ConstGrob::ConstGrob (GrobType grobType, index_t const* globCornerInds, const impl::Array_16_4& cornerOffsets) :
  m_globCornerInds (globCornerInds),
  m_cornerOffsets (cornerOffsets),
  m_desc (grobType)
{}

void ConstGrob::reset (Grob const& other)
{
  reset (ConstGrob (other));
}

void ConstGrob::reset (ConstGrob const& other)
{
  m_globCornerInds = other.m_globCornerInds;
  m_cornerOffsets = other.m_cornerOffsets;
  m_desc = other.m_desc;
}

void ConstGrob::set_global_corner_array (index_t const* cornerArray)
{
  m_globCornerInds = cornerArray;
}

/// only compares corners, ignores order and orientation.
bool ConstGrob::operator == (const ConstGrob& g) const
{
  if (m_desc.grob_type() != g.m_desc.grob_type())
    return false;

  CornerIndexContainer gCorners;
  g.collect_corners (gCorners);

  const index_t numCorners = num_corners ();
  for(index_t i = 0; i < numCorners; ++i) {
    bool gotOne = false;
    const index_t c = corner(i);
    for(index_t j = 0; j < numCorners; ++j) {
      if(c == gCorners [j]){
        gotOne = true;
        break;
      }
    }

    if (!gotOne)
      return false;
  }

  return true;
}

bool ConstGrob::operator != (const ConstGrob& g) const
{
  return !((*this) == g);
}

index_t ConstGrob::operator [] (const index_t i) const
{
  return corner (i);
}

index_t ConstGrob::dim () const        {return m_desc.dim ();}

GrobType ConstGrob::grob_type () const {return m_desc.grob_type ();}

GrobDesc ConstGrob::desc () const      {return m_desc;}

index_t const* ConstGrob::global_corner_array () const
{
  return m_globCornerInds;
}

index_t ConstGrob::num_corners () const         {return m_desc.num_corners();}

/// returns the global index of the i-th corner
index_t ConstGrob::corner (const index_t i) const
{
  assert (m_globCornerInds != nullptr);
  return m_globCornerInds [m_cornerOffsets.get(i)];
}

/// collects the global indices of corners
/** \param cornersOut Array of size `Grob::max_num_corners(). Only the first
   *                      `Grob::num_corners()` entries are filled
 *
 * \returns The number of corners of the specified side
 */
index_t ConstGrob::collect_corners (CornerIndexContainer& cornersOut) const
{
  assert (m_globCornerInds != nullptr);
  const index_t numCorners = num_corners();
  for(index_t i = 0; i < numCorners; ++i)
    cornersOut[i] = static_cast<index_t> (m_globCornerInds [m_cornerOffsets.get(i)]);
  return numCorners;
}

index_t ConstGrob::num_sides (const index_t sideDim) const
{
  return m_desc.num_sides(sideDim);
}

GrobDesc ConstGrob::side_desc (const index_t sideDim, const index_t sideIndex) const
{
  return m_desc.side_desc (sideDim, sideIndex);
}

ConstGrob ConstGrob::side (const index_t sideDim, const index_t sideIndex) const
{
  assert (m_globCornerInds != nullptr);
  impl::Array_16_4 cornerOffsets;
  const index_t numCorners = m_desc.side_desc(sideDim, sideIndex).num_corners();
  const index_t* locCorners = m_desc.local_side_corners (sideDim, sideIndex);
  // LOGT(side, "side " << sideIndex << "(dim: " << sideDim << ", num corners: " << numCorners << "): ");
  for(index_t i = 0; i < numCorners; ++i){
    // LOG(locCorners[i] << " ");
    cornerOffsets.set (i, m_cornerOffsets.get(locCorners[i]));
  }
  // LOG("\n");

  return ConstGrob (m_desc.side_type (sideDim, sideIndex), m_globCornerInds, cornerOffsets);
}

/// returns the index of the side which corresponds to the given grob
/** if no such side was found, 'lume::NO_INDEX' is returned.*/
index_t ConstGrob::find_side (const ConstGrob& sideGrob) const
{
  const index_t sideDim = sideGrob.dim();
  const index_t numSides = num_sides (sideDim);
  for(index_t iside = 0; iside < numSides; ++iside) {
    if (sideGrob == side (sideDim, iside))
      return iside;
  }
  return NO_INDEX;
}

const impl::Array_16_4& ConstGrob::corner_offsets () const
{
  return m_cornerOffsets;
}

Grob::Grob (GrobType grobType, index_t* corners)
  : m_constGrob (grobType, corners)
{}

Grob::Grob (Grob const& other)
  : m_constGrob (other)
{}

Grob::Grob (GrobType grobType, index_t* globCornerInds, const impl::Array_16_4& cornerOffsets)
  : m_constGrob (grobType, globCornerInds, cornerOffsets)
{}

void Grob::reset (Grob const& other)
{
  m_constGrob.reset (other);
}

void Grob::set_global_corner_array (index_t* cornerArray)
{
  m_constGrob.set_global_corner_array (cornerArray);
}

Grob& Grob::operator = (Grob const& other)
{
  return operator = (ConstGrob (other));
}

Grob& Grob::operator = (ConstGrob const& other)
{
  assert (global_corner_array () != nullptr);
  assert (other.global_corner_array () != nullptr);
  assert (other.grob_type () == grob_type ());
  assert (other.num_corners () == num_corners ());

  index_t const numCorners = num_corners ();
  for (index_t i = 0; i < numCorners; ++i)
  {
    set_corner (i, other [i]);
  }

  return *this;
}

/// only compares corners, ignores order and orientation.
bool Grob::operator == (const Grob& other) const
{
  return m_constGrob == other;
}

bool Grob::operator == (const ConstGrob& other) const
{
  return m_constGrob == other;
}

bool Grob::operator != (const Grob& other) const
{
  return m_constGrob != other;
}

bool Grob::operator != (const ConstGrob& other) const
{
  return m_constGrob != other;
}

index_t Grob::operator [] (const index_t i) const
{
  return corner (i);
}

index_t Grob::dim () const
{
  return m_constGrob.dim ();
}

GrobType Grob::grob_type () const
{
  return m_constGrob.grob_type ();
}

GrobDesc Grob::desc () const
{
  return m_constGrob.desc ();
}

index_t const* Grob::global_corner_array () const
{
  return m_constGrob.global_corner_array ();
}

index_t* Grob::global_corner_array ()
{
  return const_cast <index_t*> (m_constGrob.global_corner_array ());
}

index_t Grob::num_corners () const
{
  return m_constGrob.num_corners ();
}

/// returns the global index of the i-th corner
index_t Grob::corner (const index_t i) const
{
  return m_constGrob.corner (i);
}

/// sets the point index of the i-th corner
void Grob::set_corner (const index_t cornerIndex, const index_t pointIndex)
{
  assert (global_corner_array () != nullptr);
  assert (cornerIndex < num_corners ());
  global_corner_array () [m_constGrob.corner_offsets ().get(cornerIndex)] = pointIndex;
}

/// collects the global indices of corners
/** \param cornersOut Array of size `Grob::max_num_corners(). Only the first
   *                      `Grob::num_corners()` entries are filled
 *
 * \returns The number of corners of the specified side
 */
index_t Grob::collect_corners (CornerIndexContainer& cornersOut) const
{
  return m_constGrob.collect_corners (cornersOut);
}

index_t Grob::num_sides (const index_t sideDim) const
{
  return m_constGrob.num_sides(sideDim);
}

GrobDesc Grob::side_desc (const index_t sideDim, const index_t sideIndex) const
{
  return m_constGrob.side_desc (sideDim, sideIndex);
}

Grob Grob::side (const index_t sideDim, const index_t sideIndex) const
{
  auto const cgrob = m_constGrob.side (sideDim, sideIndex);
  return Grob (cgrob.grob_type (),
               const_cast <index_t*> (cgrob.global_corner_array ()),
               cgrob.corner_offsets ());
}

/// returns the index of the side which corresponds to the given grob
/** if no such side was found, 'lume::NO_INDEX' is returned.*/
index_t Grob::find_side (const Grob& sideGrob) const
{
  return m_constGrob.find_side (sideGrob);
}

const impl::Array_16_4& Grob::corner_offsets () const
{
  return m_constGrob.corner_offsets ();
}

}// end of namespace
