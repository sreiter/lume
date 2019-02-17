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

#ifndef __H__lume_annex_table
#define __H__lume_annex_table

#include <array>
#include "grob.h"
#include "mesh.h"

namespace lume {

template <class TAnnex>
class AnnexTable {
public:

    template <class ... AnnexConstructorArgs>
	AnnexTable (SPMesh mesh, const std::string& annexName, GrobSet grobSet)
	{
		m_mesh = mesh;
		for(auto gt : grobSet)
        {
            const TypedAnnexKey <TAnnex> annexKey (annexName, gt);
            if (mesh->has_annex (annexKey))
            {
                m_annexes [gt] = &mesh->annex (annexKey);
            }
            else
            {
                m_annexes [gt] = nullptr;
            }
        }
	}

    bool has_annex (const grob_t grobType) const
    {
        return m_annexes [grobType] != nullptr;
    }

	TAnnex& annex (const grob_t grobType)
    {
        assert (m_annexes [grobType] != nullptr);
        return *m_annexes [grobType];
    }

	const TAnnex& annex (const grob_t grobType) const
    {
        assert (m_annexes [grobType] != nullptr);
        return *m_annexes [grobType];
    }

	SPMesh mesh ()			{return m_mesh;}
	CSPMesh mesh () const	{return m_mesh;}

private:
	SPMesh m_mesh;
	std::array <TAnnex*, NUM_GROB_TYPES> m_annexes;
};


template <class TAnnex>
class ArrayAnnexTable {
public:
	ArrayAnnexTable (SPMesh mesh, const std::string& annexName, GrobSet grobSet) :
		m_annexTable (mesh, annexName, grobSet)
	{}

    bool has_annex (const grob_t grobType) const        {return m_annexTable.has_annex (grobType);}
	Annex& annex (const grob_t grobType)			    {return m_annexTable.annex (grobType);}
	const Annex& annex (const grob_t grobType) const	{return m_annexTable.annex (grobType);}

	typename TAnnex::value_type& operator [] (const GrobIndex& gi)				{return m_annexTable.annex (gi.grob_type ()) [gi.index ()];}
	const typename TAnnex::value_type& operator [] (const GrobIndex& gi) const	{return m_annexTable.annex (gi.grob_type ()) [gi.index ()];}

private:
	AnnexTable <TAnnex>	m_annexTable;
};

	
}//	end of namespace lume

#endif	//__H__lume_annex_table
