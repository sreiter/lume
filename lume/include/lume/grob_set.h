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

#ifndef __H__lume_grob_set
#define __H__lume_grob_set

#include "grob_set_types.h"

namespace lume {

class GrobSet {
public:
    struct iterator {
        using value_type = GrobType;
        iterator ()                                 : index (0), set (nullptr) {}
        iterator (index_t i, const GrobSet* gs )    : index (i), set (gs)      {}
        GrobType operator * () const                {return set->grob_type (index);}
        bool operator == (const iterator& it) const {return index == it.index && set == it.set;}
        bool operator != (const iterator& it) const {return index != it.index || set != it.set;}
        iterator& operator ++ ()                    {++index; return *this;}
      private:
        int index;
        const GrobSet* set;
    };

    GrobSet ();
    GrobSet (const GrobSetType grobSet);
    GrobSet (const GrobType grobType);

    bool operator == (const GrobSet& gs) const          {return m_rawDesc == gs.m_rawDesc;}
    bool operator != (const GrobSet& gs) const          {return m_rawDesc != gs.m_rawDesc;}

    GrobSetType grob_set_type () const                  {return static_cast <GrobSetType> (m_rawDesc [0]);}
    index_t dim () const                                {return m_rawDesc [1];}
    const std::string& name () const                    {return GrobSetTypeName (grob_set_type ());}
    index_t size () const                               {return m_rawDesc [2];}
    GrobType grob_type (const index_t i) const          {return static_cast <GrobType> (m_rawDesc [3 + i]);}

    GrobSetType side_set (const index_t sideDim) const  {return static_cast <GrobSetType> (m_rawDesc [3 + size() + sideDim]);}
    GrobSetType side_set () const                       {return side_set (dim() - 1);}

    iterator begin () const     {return iterator (0, this);}
    iterator end () const       {return iterator (size(), this);}

private:
    const index_t* m_rawDesc;
};

}//    end of namespace lume

#endif    //__H__lume_grob_set
