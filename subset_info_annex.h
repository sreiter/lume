// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2018 Sebastian Reiter <s.b.reiter@gmail.com>

#ifndef __H__slimesh_subset_info_annex
#define __H__slimesh_subset_info_annex

#include <string>
#include <vector>
#include "annex.h"
#include "types.h"

namespace slimesh {

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

}//	end of namespace slimesh

#endif	//__H__slimesh_subset_info_annex
