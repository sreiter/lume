// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2018 Sebastian Reiter <s.b.reiter@gmail.com>

#ifndef __H__slimesh_annex
#define __H__slimesh_annex

#include <memory>
#include "custom_exception.h"

namespace slimesh {

DECLARE_CUSTOM_EXCEPTION (AnnexError, SlimeshError);

///	Base class for annexes, which can e.g. be annexed to an instance of Mesh
class Annex {
public:
	virtual ~Annex () {};
	virtual const char* class_name () const = 0;
	virtual void do_imgui () {};
	virtual bool has_imgui () const	{return false;}
};

using SPAnnex	= std::shared_ptr <Annex>;
using CSPAnnex	= std::shared_ptr <const Annex>;

}//	end of namespace slimesh

#endif	//__H__slimesh_annex
