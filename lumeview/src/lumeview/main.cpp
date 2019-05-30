// This file is part of lumeview, a lightweight viewer for unstructured meshes
//
// Copyright (C) 2018, 2019 Sebastian Reiter
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

#include <iostream>

#include <glad/glad.h>  // include before other OpenGL related includes
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <imgui/imgui.h>
#include <lumeview/lumeview.h>
#include <lumeview/camera/cmd/focus_nodes.h>
#include <lumeview/cmd/active_command_queues.h>
#include <lumeview/mesh/mesh_content.h>
#include <lumeview/mesh/cmd/load_from_file.h>
#include <lumeview/mesh/cmd/refine.h>

using namespace lumeview;
using std::cout;
using std::endl;
using SPLumeview = std::shared_ptr <Lumeview>;

static glm::vec2                g_pixelScale (1);
static std::weak_ptr <Lumeview> g_lumeview;

DECLARE_CUSTOM_EXCEPTION (GLFWError, LumeviewError);

void HandleGLFWError (int error, const char* description)
{
    throw GLFWError () << "(code " << error << "): " << description;
}

void FramebufferResized (GLFWwindow* window, int width, int height)
{
    int winWidth, winHeight;
    glfwGetWindowSize (window, &winWidth, &winHeight);
    if(winWidth > 0 && winHeight > 0 && width > 0 && height > 0)
        g_pixelScale = glm::vec2 ((float) width / (float) winWidth,
                                  (float) height / (float) winHeight);

    SPLumeview (g_lumeview)->set_viewport (camera::Viewport (0, 0, width, height));
}

void CursorPositionCallback(GLFWwindow* window, double x, double y)
{
    SPLumeview (g_lumeview)->mouse_move (glm::vec2(x, y) * g_pixelScale);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    SPLumeview (g_lumeview)->mouse_button (button, action, mods);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    SPLumeview (g_lumeview)->mouse_scroll (glm::vec2 (xoffset, yoffset));
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}

void CharCallback(GLFWwindow* window, unsigned int c)
{
    // ImGui_ImplGlfw_CharCallback(window, c);
}

static const char* ImGui_GetClipboardText(void* user_data)
{
    return glfwGetClipboardString((GLFWwindow*)user_data);
}

static void ImGui_SetClipboardText(void* user_data, const char* text)
{
    glfwSetClipboardString((GLFWwindow*)user_data, text);
}

void InitImGui (GLFWwindow* window)
{
    // ImGui_SetClipboardCallbacks (ImGui_GetClipboardText,
    //                              ImGui_SetClipboardText,
    //                              window);
}

int main (int argc, char** argv)
{
    int retVal = 0;

    try {
        glfwSetErrorCallback (HandleGLFWError);
        glfwInit ();
        glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, USE_GL_VERSION_MAJOR);
        glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, USE_GL_VERSION_MINOR);
        if((USE_GL_VERSION_MAJOR >= 3) && (USE_GL_VERSION_MINOR >= 2))
            glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //  add for OSX:
        #ifdef __APPLE__
            glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

    //  Set up window
        GLFWwindow* window = glfwCreateWindow (1200, 800, "lumeview", NULL, NULL);
        if (window == NULL) {
            throw InitializationError () << "Failed to create glfw window.";
        }

        glfwMakeContextCurrent (window);
        glfwSwapInterval(1);

        auto lumeview = std::make_shared <Lumeview> ();
        g_lumeview = lumeview;

    //  load specified meshes
        // auto barrier = std::make_shared <cmd::Barrier> ();
        std::vector <std::shared_ptr <scene::Node>> nodes;

        if (argc >= 2) {
            for (int i = 1; i < argc; ++i) {
                auto meshContent = std::make_shared <mesh::MeshContent> (argv [i]);
                meshContent->schedule (std::make_shared <mesh::cmd::LoadFromFile> (meshContent, argv [i]));
                meshContent->schedule (std::make_shared <mesh::cmd::Refine> (meshContent));
                meshContent->schedule (std::make_shared <mesh::cmd::Refine> (meshContent));
                meshContent->schedule (std::make_shared <mesh::cmd::Refine> (meshContent));
                meshContent->schedule (std::make_shared <mesh::cmd::Refine> (meshContent));
                meshContent->schedule (std::make_shared <mesh::cmd::Refine> (meshContent));
                meshContent->schedule (std::make_shared <mesh::cmd::Refine> (meshContent));

                auto node = std::make_shared <scene::Node> (meshContent);
                lumeview->scene ().add_child (node);
                nodes.emplace_back (std::move (node));
                // meshContent->schedule (barrier);
            }
        }

        // if (!nodes.empty ())
        // {
        //     // lumeview->schedule_camera_command (barrier);
        //     lumeview->schedule_camera_command (
        //         std::make_shared <camera::cmd::FocusNodes> (lumeview->camera (), nodes, 0.5));
        // }

        glfwSetCursorPosCallback (window, CursorPositionCallback);
        glfwSetMouseButtonCallback (window, MouseButtonCallback);
        glfwSetScrollCallback (window, ScrollCallback);
        glfwSetKeyCallback (window, KeyCallback);
        glfwSetCharCallback (window, CharCallback);

    //  setup view
        int frmBufWidth, frmBufHeight;
        glfwGetFramebufferSize (window, &frmBufWidth, &frmBufHeight);
        FramebufferResized (window, frmBufWidth, frmBufHeight);
        glfwSetFramebufferSizeCallback (window, FramebufferResized);

        while (!glfwWindowShouldClose (window))
        {
            cmd::ActiveCommandQueues::tick ();
            lumeview->process_gui ();
            lumeview->render ();
            
            glfwSwapBuffers (window);
            glfwPollEvents ();
        }
    }
    catch (std::exception& e) {
        cout << "\nAn unhandled ERROR occurred during execution:\n";
        cout << e.what() << endl << endl;
        retVal = 1;
    }
    catch (...)
    {
        cout << "\nAn unknown error occurred during execution.\n";
        retVal = 1;
    }

    glfwTerminate ();
    return retVal;
}

