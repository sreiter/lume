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
#include "types.h"

namespace lume {

DECLARE_CUSTOM_EXCEPTION (AnnexTypeError, AnnexError);

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
		set_annex (AnnexKey ("coords", VERTEX), m_coords);
	}
	
	~Mesh () {}
	
	// COORDINATES
	SPRealArrayAnnex coords ()						{return m_coords;}
	CSPRealArrayAnnex coords () const				{return m_coords;}
	index_t num_coords () const						{return m_coords->size();}

	void set_coords (const SPRealArrayAnnex& coords)		{m_coords = coords;}


	// INDICES
	SPGrobArray inds (const grob_t gt)
	{
		if (!inds_allocated (gt)) {
			auto t = std::make_shared <GrobArray> (gt);
			m_grobStorage.set_annex (gt, t);
			return t;
		}
		else
			return m_grobStorage.annex(gt);
	}

	CSPGrobArray inds (const grob_t gt) const
	{
		return m_grobStorage.annex(gt);
	}

	bool inds_allocated (const grob_t gt) const
	{
		return m_grobStorage.has_annex(gt);
	}

	bool has (const grob_t gt) const
	{
		return inds_allocated (gt) && inds (gt)->size();
	}

	bool has (const GrobSet gs) const
	{
		for (auto gt : gs) {
			if (has (gt))
				return true;
		}
		return false;
	}

	void remove_inds (const grob_t gt)
	{
		m_grobStorage.remove_annex (gt);
	}

	std::vector <grob_t> grob_types() const
	{
		return m_grobStorage.collect_keys();
	}

	index_t num_inds (grob_t grob)
	{
		if (inds_allocated (grob))
			return inds (grob)->num_indices();
		return 0;
	}

	index_t num_inds (const GrobSet& gs)
	{
		index_t num = 0;
		for(index_t i = 0; i < gs.size(); ++i)
			num += num_inds (gs.grob_type (i));
		return num;
	}

	index_t num (grob_t grob)
	{
		if (inds_allocated (grob))
			return inds (grob)->size ();
		return 0;
	}

	index_t num (const GrobSet& gs)
	{
		index_t counter = 0;
		for(index_t i = 0; i < gs.size(); ++i)
			counter += num (gs.grob_type (i));
		return counter;
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

	using index_annex_storage_t	= AnnexStorage <grob_t, GrobArray>;
	using mesh_annex_storage_t	= AnnexStorage <AnnexKey, Annex>;

	//	MEMBER VARIABLES
	SPRealArrayAnnex			m_coords;
	index_annex_storage_t	m_grobStorage;
	mesh_annex_storage_t	m_annexStorage;
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
