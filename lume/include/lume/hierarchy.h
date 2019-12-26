// This file is part of lume, a C++ library for lightweight unstructured meshes
//
// Copyright (C) 2019 Sebastian Reiter <s.b.reiter@gmail.com>
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

#include <lume/grob.h>
#include <lume/grob_index.h>
#include <lume/mesh.h>

namespace lume
{

class Hierarchy
{
public:
  struct Relation
  {
    class ChildIterator
    {
    public:
      ChildIterator (index_t index) : m_index (index) {}

      bool           operator != (const ChildIterator& i) const {return m_index != i.m_index;}
      index_t        operator *  () const                       {return m_index;}
      ChildIterator& operator ++ ()                             {++m_index; return *this;}

    private:
      index_t m_index;
    };

    ConstGrob parent;
    index_t firstChild;
    index_t numChildren;

    ChildIterator begin () const {return ChildIterator (firstChild);}
    ChildIterator end   () const {return ChildIterator (firstChild + numChildren);}
  };

public:
  Hierarchy (CSPMesh   parentMesh,
             SPMesh    childMesh)
    : m_parentMesh (std::move (parentMesh))
    , m_childMesh  (std::move (childMesh))
  {}

  Hierarchy (Hierarchy& other) = delete;

  Hierarchy (Hierarchy&& other)
    : m_parentMesh (std::move (other.m_parentMesh))
    , m_childMesh  (std::move (other.m_childMesh))
  {
    for(size_t i = 0; i < m_relationsByChildType.size (); ++i) {
      m_relationsByChildType [i] = std::move (other.m_relationsByChildType [i]);
    }
  }

  Mesh const& parent_mesh () const {return *m_parentMesh;}
  Mesh&       child_mesh  ()       {return *m_childMesh;}

  void reserve (GrobType childType, size_t const numParents)
  {
    m_relationsByChildType [childType].reserve (numParents);
  }

  /** Returns an array of relations between parents and their consecutive children.*/
  std::vector <Relation> const& relationsForChildType (GrobType childType)
  {
    return m_relationsByChildType [childType];
  }

  void add_relation (const ConstGrob& parent,
                     GrobType const childType,
                     index_t const firstChild,
                     index_t const numChildren)
  {
    m_relationsByChildType [childType].emplace_back (Relation {parent, firstChild, numChildren});
  }

private:
  CSPMesh   m_parentMesh;
  SPMesh    m_childMesh;
  std::array <std::vector <Relation>, NUM_GROB_TYPES> m_relationsByChildType;
};

}// end of namespace lume
