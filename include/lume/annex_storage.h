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
#include "custom_exception.h"

namespace lume {

DECLARE_CUSTOM_EXCEPTION (NoSuchAnnexError, AnnexError)

template <class TKey, class T>
class AnnexStorage
{
public:
	using value_t		= std::shared_ptr <T>;
	using const_value_t	= std::shared_ptr <const T>;
	using annex_map_t	= std::map <TKey, value_t>;

	annex_map_t& annex_map ()						{return m_annexMap;}

	const annex_map_t& annex_map () const			{return m_annexMap;}

	bool has_annex (const TKey& id) const
	{
		auto i = m_annexMap.find (id);
		return i != m_annexMap.end ();
	}

	template <class TConstruct = T>
	value_t annex (const TKey& id)
	{
		auto d = m_annexMap[id];
		if(!d)
			m_annexMap[id] = d = std::make_shared <TConstruct> ();
		return d;
	}

	const_value_t annex (const TKey& id) const
	{
		auto i = m_annexMap.find (id);
		if (i == m_annexMap.end())
			throw NoSuchAnnexError (std::to_string (id));
		            
		return i->second;
	}

	template <class TConcreteType>
	value_t optional_annex (const TKey& id)
	{
		auto i = m_annexMap.find (id);
		if (i == m_annexMap.end())
			return std::shared_ptr<TConcreteType>();
		return i->second;
	}

	template <class TConcreteType>
	const_value_t optional_annex (const TKey& id) const
	{
		auto i = m_annexMap.find (id);
		if (i == m_annexMap.end())
			return std::shared_ptr<const TConcreteType>();
		return i->second;
	}

	void set_annex (const TKey& id, const value_t& annex)
	{
		m_annexMap[id] = annex;
	}

	void remove_annex (const TKey& id)
	{
		m_annexMap.erase (id);
	}

	std::vector <TKey> collect_keys () const
	{
		std::vector <TKey> keys;
		for (auto& e : m_annexMap)
			keys.push_back (e.first);
		return keys;
	}
	
private:
	annex_map_t	m_annexMap;
};

}//	end of namespace lume

#endif	//__H__lume_annex_storage
