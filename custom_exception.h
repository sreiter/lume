// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2018 Sebastian Reiter <s.b.reiter@gmail.com>

#ifndef __H__slimesh_custom_exception
#define __H__slimesh_custom_exception

#include <stdexcept>

///	Declares an exception class. baseClass should derive from std::exception or similar.
#define DECLARE_CUSTOM_EXCEPTION(className, baseClass) \
	class className : public baseClass {\
	public:\
		className () : baseClass ("") {}\
		className (const char* what) : baseClass (what) {}\
		className (const std::string& what) : baseClass (what) {}\
	}; 
	
namespace slimesh {

/// The base class for all exceptions thrown by slimesh
DECLARE_CUSTOM_EXCEPTION (SlimeshError, std::runtime_error);

}//	end of namespace slimesh

#endif	//__H__slimesh_custom_exception
