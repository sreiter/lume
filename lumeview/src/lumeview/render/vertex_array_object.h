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

#include <cassert>
#include <glad/glad.h>  // include before other OpenGL related includes
#include <lumeview/render/buffer.h>
#include <lumeview/render/bind_scope.h>

namespace lumeview::render
{

class VertexArrayObject {
public:
    using BindScope = lumeview::render::BindScope <VertexArrayObject>;

public:
    VertexArrayObject ()
    {
        glGenVertexArrays (1, &m_id);
    }

    ~VertexArrayObject ()
    {
        glDeleteVertexArrays (1, &m_id);
    }

    BindScope bind_scope () const
    {
        return BindScope (this);
    }

    /// Binds the given vertex buffer to the specified stage and enables the stage.
    void bind_to_stage (const Buffer& buffer, GLuint stage)
    {
        int boundVao;
        glGetIntegerv (GL_VERTEX_ARRAY_BINDING, &boundVao);
        
        if (boundVao != m_id) {
            glBindVertexArray (m_id);
        }
        
        auto bufferScope = buffer.bind_scope ();

        glVertexAttribPointer (stage, buffer.tuple_size (), buffer.data_type (), GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray (stage);
        
        if (boundVao != m_id) {
            glBindVertexArray (boundVao);
        }
    }

    void enable_stage (const GLuint stage, const bool enable)
    {
        int boundVao;
        glGetIntegerv (GL_VERTEX_ARRAY_BINDING, &boundVao);
        
        if (boundVao != m_id) {
            glBindVertexArray (m_id);
        }
        
        if (enable) {
            glEnableVertexAttribArray (stage);
        }
        else {
            glDisableVertexAttribArray (stage);
        }

        if (boundVao != m_id) {
            glBindVertexArray (boundVao);
        }
    }

private:
    friend class lumeview::render::BindScope <VertexArrayObject>;

    void bind () const
    {
        glBindVertexArray (m_id);
    }

    void unbind () const
    {
        glBindVertexArray (0);
    }

private:
    GLuint m_id    {0};
};

}// end of namespace lumeview::render
