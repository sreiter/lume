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
project (lume LANGUAGES CXX)

option (BUILD_LUME_TESTS "Build 'lume_test' executable and configure test environment" OFF)
message (STATUS "BUILD_LUME_TESTS: " ${BUILD_LUME_TESTS} "    (enable/disable with cmake option -DBUILD_LUME_TESTS=ON/OFF)")

set (sources
        src/subset_info_annex.cpp
        src/file_io.cpp
        src/grob.cpp
        src/mesh.cpp
        src/neighborhoods.cpp
        src/neighbors.cpp
        src/normals.cpp
        src/rim_mesh.cpp
        src/topology.cpp
    )

set (headers
     	include/lume/annex.h
     	include/lume/annex_storage.h
     	include/lume/array_annex.h
     	include/lume/array_iterator.h
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
     	include/lume/parallel_for.h
     	include/lume/rim_mesh.h
     	include/lume/subset_info_annex.h
     	include/lume/topology.h
     	include/lume/topology_impl.h
     	include/lume/types.h
     	include/lume/unpack.h
     	include/lume/vec_math_raw.h)

add_library (lume ${sources} ${headers})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(lume Threads::Threads)

target_include_directories(lume
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/deps
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/rapidxml-1.13
)

target_compile_features(lume PUBLIC cxx_range_for
                        			cxx_variadic_templates
									cxx_lambdas
									#cxx_return_type_deduction
									)


if (BUILD_LUME_TESTS)
	file(COPY "test/test_meshes" DESTINATION "${CMAKE_BINARY_DIR}")

	set (testSources
	     	test/tests.cpp
	     	test/tests_main.cpp)

	add_executable (lume_tests ${testSources})
	target_link_libraries(lume_tests lume)
endif (BUILD_LUME_TESTS)
