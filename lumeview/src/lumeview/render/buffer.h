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

#pragma once

#include <glad/glad.h>  // include before other OpenGL related includes

#include <lumeview/lumeview_error.h>
#include <lumeview/render/bind_scope.h>

namespace lumeview::render
{

class Buffer {
public:
    using BindScope = lumeview::render::BindScope <Buffer>;

    /** generates an OpenGL buffer object.
     *
     * \param type  Either 'GL_ARRAY_BUFFER' or 'GL_ELEMENT_ARRAY_BUFFER'
     *
     * \param memHint Either 'GL_STATIC_DRAW' (default) or 'GL_DYNAMIC_DRAW'
     */
    Buffer (GLenum type, GLenum memHint = GL_STATIC_DRAW) :
        m_bufferType (type),
        m_memHint (memHint)
    {
        glGenBuffers (1, &m_bufferId);
    }

    ~Buffer ()
    {
        glDeleteBuffers (1, &m_bufferId);
    }

    BindScope bind_scope () const
    {
        return BindScope (this);
    }

    GLsizeiptr size () const       {return m_size;}
    GLsizeiptr capacity () const   {return m_capacity;}

    /// makes sure that a buffer of the specified size is allocated.
    /** \note this also binds the buffer 
     * \warning this may clear the buffer contents!
     */
    void set_size (const GLsizeiptr size)
    {
        m_size = size;
        if (size > m_capacity) {
            bind ();
            glBufferData (m_bufferType, size, NULL, m_memHint);
            m_capacity = size;
        }
    }

    /// allocates a new buffer storage if necessary and transfers the specified data
    /** \note this also binds the buffer
     * \{ */
    template <class Container>
    void set_data (const Container& container, GLenum dataType, const GLint tupleSize = 1)
    {
        set_data (container.data (),
                  container.size () * sizeof (Container::value_type),
                  dataType,
                  tupleSize);
    }

    void set_data (const void* data, const GLsizeiptr size, GLenum dataType, const GLint tupleSize = 1)
    {
        m_size = size;
        m_dataType = dataType,
        m_tupleSize = tupleSize;
        if (size > m_capacity)
        {
            bind ();
            glBufferData (m_bufferType, size, data, m_memHint);
            m_capacity = size;
        }
        else {
            set_sub_data (0, data, size);
        }
    }
    /** \} */

    /// transfers the specified data to an existing buffer region
    /** \note this also binds the buffer */
    void set_sub_data (const GLintptr offset, const void* data, const GLsizeiptr size)
    {
        if (offset + size > m_size) {
            throw OutOfBoundsError () << "Specified buffer region expands over buffer boundary";
        }

        bind ();
        glBufferSubData (m_bufferType, offset, size, data);
    }

    GLint  tuple_size () const  {return m_tupleSize;}
    GLenum data_type () const   {return m_dataType;}
    
private:
    friend class BindScope;

    void bind () const
    {
        glBindBuffer (m_bufferType, m_bufferId);
    }

    void unbind () const
    {
        glBindBuffer (m_bufferType, 0);
    }

private:
    GLuint     m_bufferId {0};
    GLsizeiptr m_size {0};
    GLsizeiptr m_capacity {0};
    GLint      m_tupleSize {0};
    GLenum     m_dataType {0};
    GLenum     m_bufferType;
    GLenum     m_memHint;
    int        m_boundStage {-1};
};

}// end of namespace lumeview::render
