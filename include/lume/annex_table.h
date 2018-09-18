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

#include "grob.h"
#include "mesh.h"

namespace lume {

template <class TAnnex>
class AnnexTable {
public:
	
	using SPTAnnex = std::shared_ptr <TAnnex>;
	using CSPTAnnex = std::shared_ptr <const TAnnex>;

	AnnexTable (SPMesh mesh, const std::string& annexName, GrobSet grobSet, bool createMissing)
	{
		for(auto gt : grobSet) {
			if (createMissing || mesh->has_annex <TAnnex> (annexName))
				m_annexes [gt] = mesh->annex <TAnnex> (annexName);
		}
	}

	SPTAnnex annex (const grob_t grobType)			{return m_annexes [grobType];}
	CSPTAnnex annex (const grob_t grobType) const	{return m_annexes [grobType];}

private:
	SPTAnnex m_annexes [NUM_GROB_TYPES];
};


template <class TAnnex>
class ArrayAnnexTable {
public:
	
	using SPTAnnex = std::shared_ptr <TAnnex>;
	using CSPTAnnex = std::shared_ptr <const TAnnex>;

	ArrayAnnexTable (SPMesh mesh, const std::string& annexName, GrobSet grobSet, bool createMissing) :
		m_annexTable (mesh, annexName, grobSet, createMissing)
	{}

	SPTAnnex annex (const grob_t grobType)			{return m_annexTable.annex (grobType);}
	CSPTAnnex annex (const grob_t grobType) const	{return m_annexTable.annex (grobType);}

	auto& operator [] (const GrobIndex& gi)				{return (*m_annexTable.annex (gi.grobType))[gi.index];}
	const auto& operator [] (const GrobIndex& gi) const	{return (*m_annexTable.annex (gi.grobType))[gi.index];}

private:
	AnnexTable <TAnnex>	m_annexTable;
};

	
}//	end of namespace lume

#endif	//__H__lume_annex_table
