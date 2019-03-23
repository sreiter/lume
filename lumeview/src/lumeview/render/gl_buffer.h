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

namespace lumeview::render
{

class GLBuffer {
public:
    /** generates an OpenGL buffer object.
     *
     * \param type  Either 'GL_ARRAY_BUFFER' or 'GL_ELEMENT_ARRAY_BUFFER'
     *
     * \param memHint Either 'GL_STATIC_DRAW' (default) or 'GL_DYNAMIC_DRAW'
     */
    GLBuffer (GLenum type, GLenum memHint = GL_STATIC_DRAW) :
        m_id (0),
        m_size (0),
        m_capacity (0),
        m_type (type),
        m_memHint (memHint)
    {
        glGenBuffers (1, &m_id);
    }

    ~GLBuffer ()
    {
        glDeleteBuffers (1, &m_id);
    }

    void bind () {
        glBindBuffer (m_type, m_id);
    }

    index_t size () const       {return m_size;}
    index_t capacity () const   {return m_capacity;}

    /// makes sure that a buffer of the specified size is allocated.
    /** \note this also binds the buffer 
     * \warning this may clear the buffer contents!
     */
    void set_size (const uint size) {
        m_size = size;
        if (size > m_capacity){
            bind ();
            glBufferData (m_type, size, NULL, m_memHint);
            m_capacity = size;
        }
    }

    /// allocates a new buffer storage if necessary and transfers the specified data
    /** \note this also binds the buffer
     * \{ */
    template <class Container>
    void set_data (const Container& container)
    {
        set_data (container.data (), container.size () * sizeof (Container::value_type));
    }

    void set_data (const void* data, const uint size) {
        m_size = size;
        if (size > m_capacity) {
            bind ();
            glBufferData (m_type, size, data, m_memHint);
            m_capacity = size;
        }
        else
            set_sub_data (0, data, size);
    }
    /** \} */

    /// transfers the specified data to an existing buffer region
    /** \note this also binds the buffer */
    void set_sub_data (const uint offset, const void* data, const uint size) {
        COND_THROW (offset + size > m_size,
                    "GLBuffer::set_sub_data: Specified buffer region expands over buffer boundary");
        bind ();
        glBufferSubData (m_type, offset, size, data);
    }

private:
    uint    m_id;
    uint    m_size;
    uint    m_capacity;
    GLenum  m_type;
    GLenum  m_memHint;
};

}// end of namespace lumeview
