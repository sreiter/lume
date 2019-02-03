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


#ifndef __H__lume_annex_storage
#define __H__lume_annex_storage

#include <map>
#include <memory>
#include <vector>
#include <string>
#include "annex.h"
#include "annex_key.h"
#include "custom_exception.h"

namespace lume {

DECLARE_CUSTOM_EXCEPTION (NoSuchAnnexError, AnnexError)

class AnnexStorage
{
public:
    using key_t         = std::pair <int, std::string>;
	using value_t		= std::shared_ptr <Annex>;
	using const_value_t	= std::shared_ptr <const Annex>;
	using annex_map_t	= std::map <key_t, value_t>;

	bool has_annex (const key_t& id) const
	{
		auto i = m_annexMap.find (id);
		return i != m_annexMap.end ();
	}

    template <class T>
    bool has_annex (const key_t& id) const
    {
        auto i = m_annexMap.find (id);
        return i != m_annexMap.end ()
               && dynamic_cast <const T*> (i->second.get ()) != nullptr;
    }

    template <class T, class ... ConstructorArgs>
    value_t annex (const key_t& id, Mesh& mesh, ConstructorArgs&& ... args)
    {
        auto& annex = m_annexMap [id];
        if (!annex)
            annex = std::make_shared <T> (std::forward <ConstructorArgs> (args)...);
        return annex;
    }

	const_value_t annex (const key_t& id) const
	{
		auto i = m_annexMap.find (id);
		if (i == m_annexMap.end())
			throw NoSuchAnnexError (id.second);
		return i->second;
	}

	value_t optional_annex (const key_t& id)
	{
		auto i = m_annexMap.find (id);
		if (i == m_annexMap.end())
			return std::shared_ptr<Annex>();
		return i->second;
	}

	const_value_t optional_annex (const key_t& id) const
	{
		auto i = m_annexMap.find (id);
		if (i == m_annexMap.end())
			return std::shared_ptr<const Annex>();
		return i->second;
	}

	void set_annex (const key_t& id, const value_t& annex, Mesh& mesh)
	{
        if (!annex)
            return;
		m_annexMap[id] = annex;
        if (id.first >= 0 && id.first < NUM_GROB_TYPES)
            annex->grobs_changed (static_cast <grob_t> (id.first), mesh);
	}

	void remove_annex (const key_t& id)
	{
		m_annexMap.erase (id);
	}

    void grobs_changed (const grob_t gt, Mesh& mesh)
    {
        for (auto& e: m_annexMap)
        {
            if (e.first.first == static_cast <int> (gt))
                e.second->grobs_changed (gt, mesh);
        }
    }
	
private:
	annex_map_t	m_annexMap;
};

}//	end of namespace lume

#endif	//__H__lume_annex_storage
