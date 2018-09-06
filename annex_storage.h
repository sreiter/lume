// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2018 Sebastian Reiter <s.b.reiter@gmail.com>

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

	void set_annex (const TKey& id, const value_t& annex)
	{
		m_annexMap[id] = annex;
	}

	const_value_t annex (const TKey& id) const
	{
		auto i = m_annexMap.find (id);
		if (i == m_annexMap.end())
			throw NoSuchAnnexError (std::to_string (id));
		            
		return i->second;
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
