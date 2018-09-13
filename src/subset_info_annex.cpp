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


#include "lume/subset_info_annex.h"

using namespace std;

namespace lume {

SubsetInfoAnnex::ImGuiExecutor SubsetInfoAnnex::s_imguiExecutor = nullptr;

SubsetInfoAnnex::
SubsetInfoAnnex(const std::string& name) :
	m_name (name)
{}

SubsetInfoAnnex::
~SubsetInfoAnnex()
{}

void SubsetInfoAnnex::
set_name (const std::string& name)
{
	m_name = name;
}

const std::string& SubsetInfoAnnex::
name () const
{
	return m_name;
}

void SubsetInfoAnnex::
add_subset (const SubsetProperties& p)
{
	m_subsetProps.push_back (p);
}

void SubsetInfoAnnex::
add_subset (SubsetProperties&& p)
{
	m_subsetProps.push_back (std::move (p));
}


void SubsetInfoAnnex::
set_subset (const size_t i, const SubsetProperties& p)
{
	if (m_subsetProps.size() <= i)
		m_subsetProps.resize (i+1);
	m_subsetProps [i] = p;
}

void SubsetInfoAnnex::
set_subset (const size_t i, SubsetProperties&& p)
{
	if (m_subsetProps.size() <= i)
		m_subsetProps.resize (i+1);
	m_subsetProps [i] = std::move (p);
}
	
}//	end of namespace lume
