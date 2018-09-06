// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2017 Sebastian Reiter, G-CSC Frankfurt <s.b.reiter@gmail.com>

#include "subset_info_annex.h"

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
