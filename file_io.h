// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2018 Sebastian Reiter <s.b.reiter@gmail.com>

#ifndef __H__lume_file_io
#define __H__lume_file_io

#include <string>
#include <exception>
#include "custom_exception.h"
#include "mesh.h"

namespace lume {


DECLARE_CUSTOM_EXCEPTION (FileIOError, LumeError);

DECLARE_CUSTOM_EXCEPTION (FileSuffixError, FileIOError);
DECLARE_CUSTOM_EXCEPTION (FileNotFoundError, FileIOError);
DECLARE_CUSTOM_EXCEPTION (FileParseError, FileIOError);

SPMesh CreateMeshFromFile (std::string filename);

}//	end of namespace lume

#endif	//__H__lume_file_io
