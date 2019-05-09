// This file is part of lumeview, a lightweight viewer for unstructured meshes
//
// Copyright (C) 2019 Sebastian Reiter <s.b.reiter@gmail.com>
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

#pragma once

#include <memory>
#include <string>

#include <glad/glad.h>  // include before other OpenGL related includes

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <lumeview/lumeview_error.h>
#include <lumeview/util/file_util.h>

namespace lumeview::render
{

class Shader
{
public:
    Shader ()
    {
        m_shaderData = std::make_shared <ShaderData> ();
    }

    explicit operator bool () const {return data().m_shaderProg != 0;}
    bool operator ! () const        {return data().m_shaderProg == 0;}
    
    void use () {
        if (!data().m_linked)
            link ();
        glUseProgram (data().m_shaderProg);
    }

    void use () const {
        if (!data().m_linked) {
            throw ShaderError () << "The shader has not been linked yet, and thus can't be used.";
        }
        glUseProgram (data().m_shaderProg);
    }

    GLuint get_uniform_location (const char* name) const
    {
        return glGetUniformLocation(data().m_shaderProg, name);
    }

    void add_source (const std::string& filename, GLenum shaderType) {
        if (!data().m_shaderProg)
            data().m_shaderProg = glCreateProgram ();

    //  load the sources from a file
        std::string shaderSrc = util::LoadStringFromFile (filename);
        const char* shaderSrcCStr = shaderSrc.c_str();

    //  create the shader object and compile the sources
        GLuint shader;
        shader = glCreateShader (shaderType);
        glShaderSource (shader, 1, &shaderSrcCStr, NULL);
        glCompileShader (shader);

    //  check for errors
        int success;
        glGetShaderiv (shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog (shader, 512, NULL, infoLog);
            throw ShaderError () << "Compilation failed in file '" << filename << "':\n" << infoLog;
        }

        glAttachShader (data().m_shaderProg, shader);
        data().m_linked = false;
        data().m_shaders.push_back (shader);
        data().m_shaderNames.push_back (filename);
        data().m_shaderTypes.push_back (shaderType);
    }

    void add_source_vs (const std::string& filename)
    {
        add_source (filename, GL_VERTEX_SHADER);
    }

    void add_source_gs (const std::string& filename)
    {
        add_source (filename, GL_GEOMETRY_SHADER);
    }

    void add_source_fs (const std::string& filename)
    {
        add_source (filename, GL_FRAGMENT_SHADER);
    }

    void link () {
        if (data().m_linked)
            return;

        data().m_linked = true;

        if (!data().m_shaderProg)
            data().m_shaderProg = glCreateProgram ();

        glLinkProgram (data().m_shaderProg);

        int success;
        glGetProgramiv (data().m_shaderProg, GL_LINK_STATUS, &success);

        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog (data().m_shaderProg, 512, NULL, infoLog);
            throw ShaderError () << "SHADER_PROGRAM::LINK_FAILED for shaders\n"
                                 << data().m_shaderNames << "\n"
                                 << infoLog;
        }
    }

    void set_uniform (const char* name, const float v) const
    {
        glUniform1f(glGetUniformLocation(data().m_shaderProg, name), v);
    }

    void set_uniform (const char* name, const glm::vec2& v) const
    {
        glUniform2fv(glGetUniformLocation(data().m_shaderProg, name),
                     1, glm::value_ptr(v));
    }

    void set_uniform (const char* name, const glm::vec3& v) const
    {
        glUniform3fv(glGetUniformLocation(data().m_shaderProg, name),
                     1, glm::value_ptr(v));
    }

    void set_uniform (const char* name, const glm::vec4& v) const
    {
        glUniform4fv(glGetUniformLocation(data().m_shaderProg, name),
                     1, glm::value_ptr(v));
    }

    void set_uniform_4fv (const char* name, const float* v) const
    {
        glUniform4fv(glGetUniformLocation(data().m_shaderProg, name),
                     1, v);
    }

    void set_uniform (const char* name, const glm::mat4& m) const
    {
        glUniformMatrix4fv(glGetUniformLocation(data().m_shaderProg, name),
                           1, GL_FALSE, glm::value_ptr(m));
    }

private:
    struct ShaderData {
        ShaderData () : m_shaderProg (0), m_linked (false)
        {}

        // ShaderData (ShaderData&& s) :
        //  m_shaderProg (std::exchange (s.m_shaderProg, 0)),
        //  m_shaders (std::exchange (s.m_shaders, std::vector<GLuint>())),
        //  m_shaderNames (std::move (s.m_shaderNames)),
        //  m_shaderTypes (std::move (s.m_shaderTypes)),
        //  m_linked (std::move (s.m_linked))
        // {}

        ~ShaderData()
        {
            if (m_shaderProg)
                glDeleteProgram (m_shaderProg);

            for(auto shader : m_shaders) {
                glDeleteShader (shader);
            }
        }

        GLuint m_shaderProg;
        std::vector<GLuint>         m_shaders;
        std::vector<std::string>    m_shaderNames;
        std::vector<GLenum>         m_shaderTypes;
        bool m_linked;
    };

    ShaderData& data ()                 {return *m_shaderData;}
    const ShaderData& data () const     {return *m_shaderData;}

    std::shared_ptr <ShaderData>    m_shaderData;
};

}// end of namespace lumeview::render
