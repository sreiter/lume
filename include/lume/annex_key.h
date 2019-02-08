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

#ifndef __H__lume_mesh_annex_key
#define __H__lume_mesh_annex_key

#include <optional>
#include <string>
#include "grob.h"
#include "annex_storage.h"

namespace lume {

class AnnexKey {
public:
    using storage_key_t = AnnexStorage::key_t;

    AnnexKey ()
        : m_storageKey (-1, "")
    {}
    
    AnnexKey (const char* name)
        : m_storageKey (NUM_GROB_TYPES, name)
    {}

    AnnexKey (const std::string& name)
        : m_storageKey (NUM_GROB_TYPES, name)
    {}

    AnnexKey (grob_t grobType, const char* name)
        : m_storageKey (grobType, name)
    {}

    AnnexKey (grob_t grobType, const std::string& name)
        : m_storageKey (grobType, name)
    {}

    virtual ~AnnexKey () = default;

    bool operator < (const AnnexKey& key) const
    {
        return m_storageKey < key.m_storageKey;
    }

    const std::string& name () const
    {
        return m_storageKey.second;
    }

    std::optional <grob_t> grob_type () const
    {
        if (m_storageKey.first < NUM_GROB_TYPES)
            return static_cast <grob_t> (m_storageKey.first);

        return {};
    }

    const storage_key_t& storage_key () const
    {
        return m_storageKey;
    }

private:
    storage_key_t m_storageKey;
};

template <class T>
class TypedAnnexKey : public AnnexKey
{
public:
    using type = T;

    TypedAnnexKey () = default;
    
    TypedAnnexKey (const char* name)
        : AnnexKey (name)
    {}

    TypedAnnexKey (const std::string& name)
        : AnnexKey (name)
    {}

    TypedAnnexKey (grob_t grobType, const char* name)
        : AnnexKey (grobType, name)
    {}

    TypedAnnexKey (grob_t grobType, const std::string& name)
        : AnnexKey (grobType, name)
    {}
};

template <class T> class ArrayAnnex;

namespace keys
{
    const TypedAnnexKey <ArrayAnnex <real_t>> vertexCoords (VERTEX, "coords");
    const TypedAnnexKey <ArrayAnnex <real_t>> vertexNormals (VERTEX, "normal");
}

}//    end of namespace lume

namespace std {
    inline string to_string (const lume::AnnexKey& v) {
        return v.name ();
    }
}

#endif    //__H__lume_mesh_annex_key
