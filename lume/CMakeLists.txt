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

cmake_minimum_required(VERSION 3.8)
project (lume LANGUAGES CXX)

set (sources
        src/lume/commands/arguments.cpp
        src/lume/commands/types.cpp
        src/lume/edge_mesh_2d.cpp
        src/lume/file_io_in.cpp
        src/lume/file_io_out.cpp
        src/lume/grob.cpp
        src/lume/grob_desc.cpp
        src/lume/grob_set.cpp
        src/lume/grob_set_types.cpp
        src/lume/grob_types.cpp
        src/lume/mesh.cpp
        src/lume/neighborhoods.cpp
        src/lume/neighbors.cpp
        src/lume/normals.cpp
        src/lume/refinement.cpp
        src/lume/rim_mesh.cpp
        src/lume/subset_info_annex.cpp
        src/lume/surface_analytics.cpp
        src/lume/topology.cpp
    )

set (headers
        include/lume/impl/array_16_4.h
        include/lume/annex.h
        include/lume/annex_key.h
        include/lume/array_annex.h
        include/lume/array_iterator.h
        include/lume/file_io.h
        include/lume/grob.h
        include/lume/grob_array.h
        include/lume/grob_hash.h
        include/lume/grob_index.h
        include/lume/grob_iterator.h
        include/lume/grob_set.h
        include/lume/grob_set_types.h
        include/lume/grob_types.h
        include/lume/lume_error.h
        include/lume/mesh.h
        include/lume/neighborhoods.h
        include/lume/neighborhoods_impl.hpp
        include/lume/neighbors.h
        include/lume/normals.h
        include/lume/parallel_for.h
        include/lume/rim_mesh.h
        include/lume/subset_info_annex.h
        include/lume/topology.h
        include/lume/topology_impl.h
        include/lume/tuple_vector.h
        include/lume/types.h
        include/lume/unpack.h

        include/lume/math/geometry.h
        include/lume/math/tuple.h
        include/lume/math/tuple_view.h
        include/lume/math/vector_math.h
        include/lume/math/raw/vector_math_raw.h)

set (privateHeaders)

add_library (lume STATIC ${sources} ${headers} ${privateHeaders})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(lume PUBLIC Threads::Threads)

target_include_directories(lume
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${CMAKE_CURRENT_SOURCE_DIR}/deps
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/rapidxml-1.13
    )

target_compile_features(lume PUBLIC cxx_std_17)
