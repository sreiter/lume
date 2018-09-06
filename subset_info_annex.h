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
//     * Neither the name of the copyright holders, the authors nor the
//       names of the contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
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


#ifndef __H__lume_subset_info_annex
#define __H__lume_subset_info_annex

#include <string>
#include <vector>
#include "annex.h"
#include "types.h"

namespace lume {

class SubsetInfoAnnex : public Annex
{
public:
	class Color {
	public:
		Color () {
			m_data[0] = m_data[1] = m_data[2] = m_data[3] = 1;
		}

		Color (const real_t value) {
			m_data[0] = m_data[1] = m_data[2] = m_data[3] = value;
		}

		Color (const real_t r, const real_t g, const real_t b, const real_t a) {
			m_data[0] = a;
			m_data[1] = g;
			m_data[2] = b;
			m_data[3] = a;
		}

		real_t& operator [] (const index_t i)		{return m_data [i];}
		real_t operator [] (const index_t i) const	{return m_data [i];}

		float& r ()			{return m_data[0];}
		float r () const 	{return m_data[0];}

		float& g ()			{return m_data[1];}
		float g () const 	{return m_data[1];}

		float& b ()			{return m_data[2];}
		float b () const 	{return m_data[2];}

		float& a ()			{return m_data[3];}
		float a () const 	{return m_data[3];}

		real_t* data_ptr ()		 		{return m_data;}
		const real_t* data_ptr () const	{return m_data;}

	private:
		real_t m_data[4];
	};

	struct SubsetProperties {
		SubsetProperties () : name ("_default_"), color (1.f, 1.f, 1.f, 1.f), visible (true) {}
		SubsetProperties (const std::string& _name, const Color& _color, const bool _visible) :
			name (_name),
			color (_color),
			visible (_visible)
		{}

		std::string		name;
		Color			color;
		bool			visible;
	};

	
	SubsetInfoAnnex(const std::string& name);
	virtual ~SubsetInfoAnnex();

	const char* class_name () const override	{return "SubsetInfoAnnex";}
		
	void set_name (const std::string& name);
	const std::string& name () const;

	void add_subset (const SubsetProperties& p);
	void add_subset (SubsetProperties&& p);

	void set_subset (const size_t i, const SubsetProperties& p);
	void set_subset (const size_t i, SubsetProperties&& p);


	using ImGuiExecutor = void (*) (std::vector<SubsetProperties>&);
	static void set_imgui_executor (ImGuiExecutor exec)	{s_imguiExecutor = exec;}

	void do_imgui () override							{if(s_imguiExecutor) s_imguiExecutor (m_subsetProps);}
	bool has_imgui () const override					{return s_imguiExecutor != nullptr;}

private:
	static ImGuiExecutor			s_imguiExecutor;

	std::string						m_name;
	std::vector <SubsetProperties>	m_subsetProps;

};

using SPSubsetInfoAnnex = std::shared_ptr <SubsetInfoAnnex>;
using CSPSubsetInfoAnnex = std::shared_ptr <const SubsetInfoAnnex>;

}//	end of namespace lume

#endif	//__H__lume_subset_info_annex
