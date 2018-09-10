# This file is part of lume, a C++ library for lightweight unstructured meshes
#
# Copyright (C) 2018 Sebastian Reiter
# Copyright (C) 2018 G-CSC, Goethe University Frankfurt
# Author: Sebastian Reiter <s.b.reiter@gmail.com>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 14)
project (lume)

set (sources
        src/subset_info_annex.cpp
        src/file_io.cpp
        src/grob.cpp
        src/mesh.cpp
        src/neighborhoods.cpp
        src/neighbors.cpp
        src/normals.cpp
        src/tests.cpp
        src/topology.cpp
    )

set (headers
     	include/lume/annex.h
     	include/lume/annex_storage.h
     	include/lume/array_annex.h
     	include/lume/custom_exception.h
     	include/lume/file_io.h
     	include/lume/grob.h
     	include/lume/grob_array.h
     	include/lume/grob_hash.h
     	include/lume/grob_index.h
     	include/lume/grob_iterator.h
     	include/lume/mesh.h
     	include/lume/neighborhoods.h
     	include/lume/neighborhoods_impl.hpp
     	include/lume/neighbors.h
     	include/lume/normals.h
     	include/lume/subset_info_annex.h
     	include/lume/tests.h
     	include/lume/topology.h
     	include/lume/topology_impl.h
     	include/lume/types.h
     	include/lume/unpack.h
     	include/lume/vec_math_raw.h)

include_directories (deps/rapidxml-1.13)
include_directories (deps)
include_directories (include/lume)

file(COPY "test_meshes" DESTINATION "${CMAKE_BINARY_DIR}")

add_library (lume ${sources} ${headers})

add_executable (lume_tests src/tests_main.cpp)
target_link_libraries(lume_tests lume)
