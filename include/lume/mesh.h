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


#ifndef __H__lume__mesh
#define __H__lume__mesh

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
#include "annex_storage.h"
#include "grob.h"
#include "grob_array.h"
#include "grob_hash.h"
#include "grob_index.h"
#include "types.h"

namespace lume {

DECLARE_CUSTOM_EXCEPTION (AnnexTypeError, AnnexError);

/** A mesh holds index arrays to define a net and provides annexes to store associtated data.
    \note   The 'const' interface is thread safe. The non-const interface is not thread save.
*/
class Mesh {
public:
    using size_type = size_t;
    
	~Mesh () {}

	void clear_grobs ()
	{
		const auto grobTypes = grob_types();
		for(auto grobType : grobTypes) {
            if (grobs_allocated (grobType))
                grob_array (grobType).clear();
        }
	}

	void clear (const GrobSet grobSet)
	{
		for(auto grobType : grobSet) {
			if (grobs_allocated (grobType))
				grob_array (grobType).clear();
		}
	}

    void resize_vertices (const size_t s)
    {
        auto& vertices = grob_array (VERTEX);
        if (s < vertices.size ())
        {
            vertices.resize (s);
            return;
        }

        vertices.reserve (s);
        while (vertices.size () < s)
        {
            vertices.push_back ({static_cast <index_t> (vertices.size ())});
        }
    }

	void insert_grob (const Grob& grob)
	{
		grob_array (grob.grob_type()).push_back (grob);
	}

	template <class iter_t>
	void insert_grobs (const iter_t begin, const iter_t end)
	{
		for (iter_t i = begin; i != end; ++i)
            grob_array (i->grob_type()).push_back (*i);
	}

    void set_grobs (GrobArray&& grobs)
    {
        const grob_t grobType = grobs.grob_desc ().grob_type ();
        
        if (m_grobLinks [grobType] != nullptr)
            m_grobLinks [grobType]->set_grobs (std::move (grobs));

        m_grobArrays [grobType].reset ();
        m_grobArrays [grobType] = std::unique_ptr <GrobArray> (new GrobArray (std::move (grobs)));
    }

	const GrobArray& grobs (const grob_t grobType) const
	{
		return grob_array (grobType);
	}

	Grob grob (const GrobIndex& grobIndex) const
	{
		return grobs (grobIndex.grobType) [grobIndex.index];
	}

	bool has (const grob_t grobType) const
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

	std::vector <grob_t> grob_types() const
	{
		std::vector <grob_t> grobTypes;
		for(index_t i = 0; i < NUM_GROB_TYPES; ++i) {
			const grob_t grobType = static_cast<grob_t> (i);
			if (has (grobType))
				grobTypes.push_back (grobType);
		}
		return grobTypes;
	}

	size_type num (grob_t grobType) const
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

	size_type num_indices (grob_t grobType) const
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

	grob_set_t grob_set_type_of_highest_dim () const
	{
		for(int dim = (int)MAX_GROB_DIM; dim >= 0; --dim) {
			grob_set_t gst = GrobSetTypeByDim ((index_t)dim);
			if (has (gst))
				return gst;
		}
		return NO_GROB_SET;
	}

    void link_grobs_and_annexes (const std::shared_ptr <Mesh>& targetMesh, std::optional <grob_t> grobType = {})
    {
        if (grobType)
            m_grobLinks.at (*grobType) = targetMesh;
        else
            m_grobLinks.at (NUM_GROB_TYPES) = targetMesh;
    }

    void link_grobs_and_annexes (const GrobSet& grobSet, const std::shared_ptr <Mesh>& targetMesh)
    {
        for (auto const grobType : grobSet)
            link_grobs_and_annexes (grobType, targetMesh);
    }

    void remove_all_links ()
    {
        for (auto& link : m_grobLinks)
            link.reset ();
    }

	bool has_annex (const AnnexKey& key) const
    {
        return m_annexStorage.has_annex (key.storage_key ());
    }

	template <class T>
	bool has_annex (const AnnexKey& key) const
    {
        return m_annexStorage.has_annex <T> (key.storage_key ());
    }

    template <class T>
    bool has_annex (const TypedAnnexKey <T>& key) const
    {
        return m_annexStorage.has_annex <T> (key.storage_key ());
    }

    template <class T>
    void set_annex (const AnnexKey& key, T&& annex)
    {
        m_annexStorage.set_annex (key.storage_key (), std::make_shared <T> (std::move (annex)), *this);
    }

    template <class T>
    void set_annex (const TypedAnnexKey <T>& key, T&& annex)
    {
        m_annexStorage.set_annex (key.storage_key (), std::make_shared <T> (std::move (annex)), *this);
    }

    /// removes an annex from a mesh.
    void remove_annex (const AnnexKey& key)
    {
        m_annexStorage.remove_annex (key.storage_key ());
    }

	template <class T>
	const T&
	annex (const AnnexKey& key) const
    {
        assert (link_index (key) >= 0 && link_index (key) < m_grobLinks.size ());

        const Annex* basePtr = m_annexStorage.optional_annex (key.storage_key ()).get ();

        if (!basePtr
            && m_grobLinks [link_index (key)] != nullptr)
        {
            basePtr = m_grobLinks [link_index (key)]->m_annexStorage.optional_annex (key.storage_key ()).get ();
        }

        const T* ptr = dynamic_cast<const T*> (basePtr);

        if (!ptr)
        {
            throw AnnexTypeError (std::string ("incompatible type '")
                                  .append (typeid (T).name ())
                                  .append ("' requested for annex key '")
                                  .append (key.name ())
                                  .append ("'."));
        }
        return *ptr;
    }

    template <class T>
    const typename TypedAnnexKey <T>::type&
    annex (const TypedAnnexKey <T>& key) const
    {
        return annex <typename T> (static_cast <const AnnexKey&> (key));
    }

    /** returns the annex array for the given id.*/
    template <class T>
    T&
    annex (const AnnexKey& key)
    {
        return const_cast <T&> (const_cast <const Mesh*> (this)->annex <T> (key));
    }

    template <class T>
    typename TypedAnnexKey <T>::type&
    annex (const TypedAnnexKey <T>& key)
    {
        return annex <T> (static_cast <const AnnexKey&> (key));
    }

private:
    int link_index (const AnnexKey& key) const
    {
        auto const optionalGrobType = key.grob_type ();
        return optionalGrobType ? *optionalGrobType : NUM_GROB_TYPES;
    }

    GrobArray& grob_array (const grob_t grobType)
    {
        if (m_grobLinks [grobType] != nullptr)
            return m_grobLinks [grobType]->grob_array (grobType);

        if (m_grobArrays [grobType] == nullptr)
            m_grobArrays [grobType] = std::unique_ptr <GrobArray> (new GrobArray (grobType));

        return *m_grobArrays [grobType];
    }

    const GrobArray& grob_array (const grob_t grobType) const
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

    bool grobs_allocated (const grob_t grobType) const
    {
        if (m_grobLinks [grobType] != nullptr)
            return m_grobLinks [grobType]->grobs_allocated (grobType);
        return m_grobArrays [grobType].get() != nullptr;
    }

	std::array <std::unique_ptr <GrobArray>, NUM_GROB_TYPES> m_grobArrays;
    std::array <std::shared_ptr <Mesh>, NUM_GROB_TYPES + 1>  m_grobLinks;
	AnnexStorage		                                     m_annexStorage;
};

using SPMesh = std::shared_ptr <Mesh>;
using CSPMesh = std::shared_ptr <const Mesh>;

}// end of namespace lume

#endif	//__H__lume__mesh
