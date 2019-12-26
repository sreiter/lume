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

#pragma once

#include <array>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <typeinfo>
#include <vector>

#include "annex.h"
#include "annex_key.h"
#include "grob.h"
#include "grob_array.h"
#include "grob_hash.h"
#include "grob_index.h"
#include "grob_set.h"
#include "types.h"

namespace lume
{

/** A mesh holds index arrays to define a net and provides annexes to store associtated data.
    \note   The 'const' interface is thread safe. The non-const interface is not thread save.
*/
class Mesh
{
public:
  using size_type = size_t;
    
  ~Mesh () {}

  void clear_grobs ()
  {
    const auto grobTypes = grob_types();
    for(auto grobType : grobTypes)
    {
      if (grobs_allocated (grobType))
      {
        grob_array (grobType).clear();
        annex_update (grobType);
      }
    }
  }

  void clear (const GrobSet grobSet)
  {
    for(auto grobType : grobSet)
    {
      if (grobs_allocated (grobType))
      {
        grob_array (grobType).clear();
        annex_update (grobType);
      }
    }
  }

  void resize_vertices (const size_t s)
  {
    auto& vertices = grob_array (VERTEX);
    
    if (s == vertices.size ())
      return;

    if (s < vertices.size ())
    {
      vertices.resize (s);
    }
    else
    {
      vertices.reserve (s);
      while (vertices.size () < s)
      {
        vertices.push_back ({static_cast <index_t> (vertices.size ())});
      }
    }

    annex_update (VERTEX);
  }

  void insert_grob (const Grob& grob)
  {
    grob_array (grob.grob_type ()).push_back (grob);
    annex_update (grob.grob_type ());
  }

  template <class iter_t>
  void insert_grobs (const iter_t begin, const iter_t end)
  {
    std::array <bool, NUM_GROB_TYPES> wasInserted {};

    for (iter_t i = begin; i != end; ++i)
    {
      const GrobType grobType = i->grob_type ();
      grob_array (grobType).push_back (*i);
      wasInserted [grobType] = true;
    }

    for(size_t i = 0; i < NUM_GROB_TYPES; ++i)
    {
      if (wasInserted [i])
          annex_update (static_cast <GrobType> (i));
    }
  }

  void set_grobs (GrobArray&& grobs)
  {
    const GrobType grobType = grobs.grob_desc ().grob_type ();
    
    if (m_linkedMeshes [grobType] != nullptr)
      m_linkedMeshes [grobType]->set_grobs (std::move (grobs));

    m_grobArrays [grobType].reset ();
    m_grobArrays [grobType] = std::make_unique <GrobArray> (std::move (grobs));
    annex_update (grobType);
  }

  GrobArray& grobs (const GrobType grobType)
  {
    return grob_array (grobType);
  }

  const GrobArray& grobs (const GrobType grobType) const
  {
    return grob_array (grobType);
  }

  Grob grob (const GrobIndex& grobIndex)
  {
    return grobs (grobIndex.grob_type ()) [grobIndex.index ()];
  }

  ConstGrob grob (const GrobIndex& grobIndex) const
  {
    return grobs (grobIndex.grob_type ()) [grobIndex.index ()];
  }

  bool has (const GrobType grobType) const
  {
    return grobs_allocated (grobType) && grobs (grobType).size();
  }

  bool has (const GrobSet grobSet) const
  {
    for (auto grobType : grobSet) {
      if (has (grobType))
        return true;
    }
    return false;
  }

  std::vector <GrobType> grob_types() const
  {
    std::vector <GrobType> grobTypes;
    for(index_t i = 0; i < NUM_GROB_TYPES; ++i) {
      const GrobType grobType = static_cast<GrobType> (i);
      if (has (grobType))
        grobTypes.push_back (grobType);
    }
    return grobTypes;
  }

  size_type num (GrobType grobType) const
  {
    if (grobs_allocated (grobType))
      return grobs (grobType).size ();
    return 0;
  }

  size_type num (const GrobSet& grobSet) const
  {
    size_type counter = 0;
    for(auto grobType : grobSet)
      counter += num (grobType);
    return counter;
  }

  size_type num_indices (GrobType grobType) const
  {
    if (grobs_allocated (grobType))
      return grobs (grobType).num_indices ();
    return 0;
  }

  size_type num_indices (const GrobSet& grobSet) const
  {
    size_type counter = 0;
    for(auto grobType : grobSet)
      counter += num_indices (grobType);
    return counter;
  }

  GrobSetType grob_set_type_of_highest_dim () const
  {
    for(int dim = (int)MAX_GROB_DIM; dim >= 0; --dim) {
      GrobSetType gst = GrobSetTypeByDim ((index_t)dim);
      if (has (gst))
        return gst;
    }
    return NO_GROB_SET;
  }

  void link_mesh (const std::shared_ptr <Mesh>& targetMesh, std::optional <GrobType> grobType = {})
  {
    linked_mesh (grobType) = targetMesh;
  }

  void link_mesh (const std::shared_ptr <Mesh>& targetMesh, const GrobSet& grobSet)
  {
    for (auto const grobType : grobSet)
      linked_mesh (grobType) = targetMesh;
  }

  void remove_all_links ()
  {
    for (auto& link : m_linkedMeshes)
      link.reset ();
  }

  bool has_annex (const AnnexKey& key) const
  {
    return m_annexMap.find (key) != m_annexMap.end ();
  }

  template <class T>
  bool has_annex (const TypedAnnexKey <T>& key) const
  {
    auto i = m_annexMap.find (key);
    return (i != m_annexMap.end () &&
            dynamic_cast <const T*> (i->second.get ()) != nullptr);
  }

  template <class T>
  T& set_annex (const AnnexKey& key, T&& annex)
  {
    const auto ret = m_annexMap.insert_or_assign (key, std::make_unique <T> (std::move (annex)));
    T& newAnnex = *static_cast <T*> (ret.first->second.get ());
    newAnnex.update (*this, key.grob_type ());
    return newAnnex;
  }

  void remove_annex (const AnnexKey& key)
  {
    m_annexMap.erase (key);
  }

  template <class T>
  const typename TypedAnnexKey <T>::type&
  annex (const TypedAnnexKey <T>& key) const
  {
    const Annex* annex = nullptr;
    auto annexIter = m_annexMap.find (key);
    if (annexIter != m_annexMap.end ())
    {
      annex = annexIter->second.get ();
    }
    else if (linked_mesh (key.grob_type ()) != nullptr)
    {
      annexIter = linked_mesh (key.grob_type ())->m_annexMap.find (key);
      if (annexIter != linked_mesh (key.grob_type ())->m_annexMap.end ())
      {
        annex = annexIter->second.get ();
      }
    }

    if (annex == nullptr)
    {
      throw NoSuchAnnexError () << "no annex found for the given key '" << key.name () << "'.";
    }

    const T* typedAnnex = dynamic_cast<const T*> (annex);

    if (typedAnnex == nullptr)
    {
      throw AnnexTypeError () << "incompatible type '" << typeid (T).name ()
                              << "' requested for annex key '" << key.name () << "'.";
    }
    return *typedAnnex;
  }

  template <class T>
  typename TypedAnnexKey <T>::type&
  annex (const TypedAnnexKey <T>& key)
  {
    return const_cast <T&> (const_cast <const Mesh*> (this)->annex <T> (key));
  }

private:
  std::shared_ptr <Mesh>& linked_mesh (std::optional <GrobType> grobType)
  {
    const int index = grobType ? *grobType : NUM_GROB_TYPES;
    return m_linkedMeshes [index];
  }

  const std::shared_ptr <Mesh>& linked_mesh (std::optional <GrobType> grobType) const
  {
    const int index = grobType ? *grobType : NUM_GROB_TYPES;
    return m_linkedMeshes [index];
  }

  GrobArray& grob_array (const GrobType grobType)
  {
    if (m_linkedMeshes [grobType] != nullptr)
      return m_linkedMeshes [grobType]->grob_array (grobType);

    if (m_grobArrays [grobType] == nullptr)
      m_grobArrays [grobType] = std::unique_ptr <GrobArray> (new GrobArray (grobType));

    return *m_grobArrays [grobType];
  }

  const GrobArray& grob_array (const GrobType grobType) const
  {
    if (!grobs_allocated (grobType))
    {
      static std::mutex allocationMutex;
      std::lock_guard <std::mutex> lockGuard (allocationMutex);

      // we have to check again, since another thread may already have allocated the grob array
      if (!grobs_allocated (grobType))
      {
        return const_cast <Mesh*> (this)->grob_array (grobType);
      }
    }

    return *m_grobArrays [grobType];
  }

  bool grobs_allocated (const GrobType grobType) const
  {
    if (linked_mesh (grobType) != nullptr)
        return linked_mesh (grobType)->grobs_allocated (grobType);
    return m_grobArrays [grobType].get() != nullptr;
  }

  void annex_update (std::optional <GrobType> grobType)
  {
    if (linked_mesh (grobType) != nullptr)
      linked_mesh (grobType)->annex_update (grobType);
      
    for (auto& e: m_annexMap)
    {
      if (e.first.grob_type () == grobType)
        e.second->update (*this, grobType);
    }
  }

  std::array <std::unique_ptr <GrobArray>, NUM_GROB_TYPES>   m_grobArrays;
  std::array <std::shared_ptr <Mesh>, NUM_GROB_TYPES + 1>    m_linkedMeshes;
  std::map <AnnexKey, std::unique_ptr <Annex>>               m_annexMap;
};

using SPMesh = std::shared_ptr <Mesh>;
using CSPMesh = std::shared_ptr <const Mesh>;

}// end of namespace lume
