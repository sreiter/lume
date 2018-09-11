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

#include <map>
#include <memory>
#include <vector>
#include <string>

#include "annex.h"
#include "annex_storage.h"
#include "array_annex.h"
#include "grob.h"
#include "grob_array.h"
#include "grob_hash.h"
#include "grob_index.h"
#include "types.h"

namespace lume {

DECLARE_CUSTOM_EXCEPTION (AnnexTypeError, AnnexError);

///	A mesh holds index arrays to define a net and provides annexes to store associtated data
class Mesh {
public:

	struct AnnexKey {
		AnnexKey ()									: name (""), grobId (NO_GROB) {}
		// AnnexKey (const char* _name)					: name (_name), grobId (NO_GROB) {}
		// AnnexKey (std::string _name)					: name (std::move(_name)), grobId (NO_GROB) {}
		AnnexKey (std::string _name, grob_t _grobId)	: name (std::move(_name)), grobId (_grobId) {}
		bool operator < (const AnnexKey& key) const			{return grobId < key.grobId || (grobId == key.grobId && name < key.name);}
		// std::ostream& operator << (std::ostream& out) const	{out << name; return out;}
		
		std::string name;
		grob_t		grobId;
	};

	using annex_iterator_t = std::map <AnnexKey, SPAnnex>::iterator;
	using const_annex_iterator_t = std::map <AnnexKey, SPAnnex>::const_iterator;

	Mesh () :
		m_coords (std::make_shared <RealArrayAnnex> ())
	{
		for(index_t i = 0; i < NUM_GROB_TYPES; ++i) {
			const grob_t grobType = static_cast<grob_t>(i);
			m_grobArrays [grobType] = std::make_unique <GrobArray> (grobType);
		}

		set_annex (AnnexKey ("coords", VERTEX), m_coords);
	}
	
	Mesh (std::initializer_list <GrobSet> supportedGrobSets) :
		m_coords (std::make_shared <RealArrayAnnex> ())
	{
		for(auto grobSet : supportedGrobSets) {
			for(auto grobType : grobSet)
				m_grobArrays [grobType] = std::make_unique <GrobArray> (static_cast<grob_t>(grobType));
		}
		set_annex (AnnexKey ("coords", VERTEX), m_coords);
	}

	~Mesh () {}
	
	// COORDINATES
	SPRealArrayAnnex coords ()						{return m_coords;}
	CSPRealArrayAnnex coords () const				{return m_coords;}
	index_t num_coords () const						{return m_coords->size();}

	void set_coords (const SPRealArrayAnnex& coords)
	{
		m_coords = coords;
	}


	// INDICES
	GrobArray& grobs (const grob_t grobType)
	{
		return *m_grobArrays [grobType];
	}

	const GrobArray& grobs (const grob_t grobType) const
	{
		return *m_grobArrays [grobType];
	}

	Grob grob (const GrobIndex& grobIndex) const
	{
		return grobs (grobIndex.grobType) [grobIndex.index];
	}

	bool grobs_allocated (const grob_t grobType) const
	{
		return m_grobArrays [grobType].get() != nullptr;
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

	index_t num (grob_t grobType)
	{
		if (grobs_allocated (grobType))
			return grobs (grobType).size ();
		return 0;
	}

	index_t num (const GrobSet& grobSet)
	{
		index_t counter = 0;
		for(auto grobType : grobSet)
			counter += num (grobType);
		return counter;
	}

	index_t num_indices (grob_t grobType)
	{
		if (grobs_allocated (grobType))
			return grobs (grobType).num_indices ();
		return 0;
	}

	index_t num_indices (const GrobSet& grobSet)
	{
		index_t counter = 0;
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

	// ANNEX
	bool has_annex (const AnnexKey& key) const						{return m_annexStorage.has_annex (key);}

	bool has_annex (const std::string& name, grob_t gt) const		{return has_annex (AnnexKey (name, gt));}

	template <class T>
	bool has_annex (const AnnexKey& key) const						{return m_annexStorage.has_annex (key) && annex <T> (key);}

	template <class T>
	bool has_annex (const std::string& name, grob_t gt) const		{return has_annex <T> (AnnexKey (name, gt));}


	///	returns the annex array for the given id. If none was present, a new one will be created.
	template <class T>
	std::shared_ptr <T>
	annex (const AnnexKey& key)							{return std::dynamic_pointer_cast<T> (m_annexStorage.annex <T> (key));}

	template <class T>
	std::shared_ptr <T>
	annex (const std::string& name, grob_t gt)			{return annex <T> (AnnexKey (name, gt));}

	template <class T>
	std::shared_ptr <const T>
	annex (const AnnexKey& key) const						{return std::dynamic_pointer_cast<const T> (m_annexStorage.annex (key));}

	template <class T>
	std::shared_ptr <const T>
	annex (const std::string& name, grob_t gt) const		{return annex <T> (AnnexKey (name, gt));}


	///	explicitly set an annex for a mesh (old one will be replaced)
	void set_annex (const AnnexKey& key,
	               const SPAnnex& annex)
	{
		m_annexStorage.set_annex (key, annex);
		if (key.name == "coords")
			set_coords (annex);
	}

	void set_annex (const std::string& name, grob_t gt,
	               const SPAnnex& annex)
	{
		set_annex (AnnexKey (name, gt), annex);
	}

	///	removes an annex from a mesh.
	/** This will decrement the shared_ptr but not necessarily delete the annex.*/
	void remove_annex (const AnnexKey& key)					{m_annexStorage.remove_annex (key);}

	void remove_annex (const std::string& name, grob_t gt)	{remove_annex (AnnexKey (name, gt));}


	void share_annexes_with (Mesh& target) const
	{
		auto& annexMap = m_annexStorage.annex_map();
		for (auto& entry : annexMap)
			target.set_annex (entry.first, entry.second);
	}

	void share_annexes_with (Mesh& target, grob_t gt) const
	{
		auto& annexMap = m_annexStorage.annex_map();
		for (auto& entry : annexMap) {
			if (entry.first.grobId == gt) {
				target.set_annex (entry.first, entry.second);
			}
		}
	}

	annex_iterator_t annex_begin ()		{return m_annexStorage.annex_map().begin();}
	annex_iterator_t annex_end ()		{return m_annexStorage.annex_map().end();}

	const_annex_iterator_t annex_begin () const	{return m_annexStorage.annex_map().begin();}
	const_annex_iterator_t annex_end () const	{return m_annexStorage.annex_map().end();}

private:
	template <class T>
	void set_coords (const std::shared_ptr<T>& coords) {
		if (auto t = std::dynamic_pointer_cast <RealArrayAnnex> (coords))
			set_coords (t);
		else
			throw AnnexTypeError ("Mesh::set_coords only supported for type real_t");
	}

	using mesh_annex_storage_t	= AnnexStorage <AnnexKey, Annex>;

	//	MEMBER VARIABLES
	SPRealArrayAnnex			m_coords;
	/** \todo	think about different storage with faster access (e.g. plain array)*/
	std::unique_ptr<GrobArray>	m_grobArrays [NUM_GROB_TYPES];
	mesh_annex_storage_t		m_annexStorage;
};

inline std::ostream& operator<< (std::ostream& out, const Mesh::AnnexKey& v) {
    out << v.name;
    return out;
}


using SPMesh = std::shared_ptr <Mesh>;
using CSPMesh = std::shared_ptr <const Mesh>;

}// end of namespace lume


namespace std {
	inline string to_string (const lume::Mesh::AnnexKey& v) {
		return v.name;
	}
}
#endif	//__H__lume__mesh
