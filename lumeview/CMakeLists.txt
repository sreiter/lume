# This file is part of lume, a C++ library for lightweight unstructured meshes
#
# Copyright (C) 2018, 2019 Sebastian Reiter
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
project (lumeview LANGUAGES C CXX)

set (GL_VERSION_MAJOR "3")
set (GL_VERSION_MINOR "3")
set (GLAD_PATH  "deps/glad-${GL_VERSION_MAJOR}.${GL_VERSION_MINOR}")

set (sources
        src/lumeview/lumeview.cpp
        src/lumeview/main.cpp

        src/lumeview/camera/camera.cpp

        src/lumeview/cmd/active_command_queues.cpp
        src/lumeview/cmd/group.cpp
        src/lumeview/cmd/widgets/command_menu.cpp

        src/lumeview/editor/meshed/meshed.cpp
        # src/lumeview/editor/isodraw/isodraw.cpp

        src/lumeview/gui/arc_ball.cpp
        src/lumeview/gui/arc_ball_control.cpp
        src/lumeview/gui/imgui_binding.cpp
        src/lumeview/gui/window_event_listener.cpp

        src/lumeview/mesh/mesh_content.cpp
        src/lumeview/mesh/mesh_factory.cpp
        src/lumeview/mesh/status_messages.cpp
        src/lumeview/mesh/cmd/register_mesh_commands.cpp

        src/lumeview/scene/node.cpp

        src/lumeview/util/file_util.cpp
        src/lumeview/util/shapes.cpp
    )

set (headers)

set (depSources
        deps/imgui/imgui.cpp
        deps/imgui/imgui_demo.cpp
        deps/imgui/imgui_draw.cpp
        deps/imgui/imgui_impl_glfw_gl3.cpp
        deps/imgui/imgui_stdlib.cpp
        deps/imgui/imgui_widgets.cpp
        ${GLAD_PATH}/src/glad.c
    )

find_package(OpenGL REQUIRED)

set (GLFW_PATH  "deps/glfw-3.2.1")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW examples" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW tests" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build GLFW docs" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "Build GLFW install" FORCE)
add_subdirectory(${GLFW_PATH})
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

add_executable (lumeview ${sources} ${headers} ${depSources})
target_compile_features(lumeview PUBLIC cxx_std_17)
add_dependencies(lumeview glfw)

target_include_directories(lumeview
    PRIVATE
        src
        deps/glm-0.9.9.3
        ${GLAD_PATH}/include
        ${GLFW_PATH}/include
        deps
    )

target_link_libraries(lumeview glfw3 ${OPENGL_LIBRARIES} lume )

if (UNIX)
    if(APPLE)
        # macOS
        find_library(COCOA Cocoa)
        find_library(COREVIDEO CoreVideo)
        find_library(IOKIT IOKit)
        target_link_libraries(lumeview dl ${COCOA} ${COREVIDEO} ${IOKIT})
    else(APPLE)
        # Linux, BSD etc.
        target_link_libraries(lumeview dl X11 Xcursor Xxf86vm Xrandr Xinerama)
    endif(APPLE)
endif (UNIX)

add_definitions (-DGLM_ENABLE_EXPERIMENTAL)
add_definitions (-DUSE_GL_VERSION_MAJOR=${GL_VERSION_MAJOR})
add_definitions (-DUSE_GL_VERSION_MINOR=${GL_VERSION_MINOR})

# lumeview shaders
add_custom_target (copyShaders ALL COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)
