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

#include <lume/array_annex.h>
#include <lume/mesh.h>
#include <lumeview/render/buffer.h>
#include <lumeview/camera/camera.h>
#include <lumeview/render/shader.h>
#include <lumeview/render/vertex_array_object.h>

namespace lumeview::render
{

class TriangleRenderer
{
public:
    TriangleRenderer ()
        : m_vertices (GL_ARRAY_BUFFER, GL_STATIC_DRAW)
        , m_indices (GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
    {
    }

    ~TriangleRenderer ()
    {
    }

    void clear ()
    {
        m_numIndices = 0;
    }

    void set_mesh_data (const lume::Mesh& mesh,
                        const lume::TypedAnnexKey <lume::ArrayAnnex <float>>& coordinatesKey = lume::keys::vertexCoords)
    {
        if (!mesh.has (lume::TRIS) ||
            !mesh.has_annex (coordinatesKey))
        {
            // clear ();
            return;
        }

        set_data (mesh.annex (coordinatesKey), mesh.grobs (lume::TRI).underlying_array ());
    }

    template <class CoordinatesContainer, class IndexContainer>
    void set_data (const CoordinatesContainer& coords, const IndexContainer& triInds)
    {
        m_vertices.set_data (coords, GL_FLOAT, static_cast <GLint> (coords.tuple_size ()));
        m_indices.set_data (triInds, GL_UNSIGNED_INT);
        m_numIndices = static_cast <GLsizei> (m_indices.size ());

        m_vertexArrayObject.bind_to_stage (m_vertices, 0);
    }

    void render (const camera::Camera& camera)
    {
        if (m_numIndices == 0) {
            return;
        }

        if (!m_shader) {
            init_shader ();
        }

        auto const& vp = camera.viewport ();
        glViewport (vp.x (), vp.y (), vp.width (), vp.height ());

        m_shader.use ();
        m_shader.set_uniform ("view", camera.view_matrix ());
        m_shader.set_uniform ("projection", camera.projection_matrix ());
        m_shader.set_uniform ("viewport", vp.size_f ());

        auto vaoScope = m_vertexArrayObject.bind_scope ();
        auto indScope = m_indices.bind_scope ();
        
        glDrawElements (GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);
    }

private:
    void init_shader ()
    {
        m_shader.add_source_vs ("shaders/flat_shading.vs");
        m_shader.add_source_gs ("shaders/flat_shading.gs");
        m_shader.add_source_fs ("shaders/flat_shading.fs");
        m_shader.link ();
    }

private:
    Buffer            m_vertices;
    Buffer            m_indices;
    Shader            m_shader;
    VertexArrayObject m_vertexArrayObject;
    GLsizei           m_numIndices {0};
};

}// end of namespace lumeview::render
