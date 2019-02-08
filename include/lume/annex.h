// This file is part of lume, a C++ library for lightweight unstructured meshes
//
// Copyright (C) 2018 Sebastian Reiter
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


#ifndef __H__lume_annex
#define __H__lume_annex

#include <optional>
#include "custom_exception.h"
#include "grob.h"

namespace lume {

DECLARE_CUSTOM_EXCEPTION (AnnexError, LumeError);

class Mesh;

///	Base class for annexes, which can e.g. be annexed to an instance of Mesh
class Annex {
public:
	virtual ~Annex ()
    {
        if (bound_mesh () == nullptr)
            unbind_from_mesh ();
    };

	virtual const char* class_name () const = 0;

    void bind_to_mesh (const Mesh& mesh, std::optional <grob_t> grobType = {})
    {
        m_boundMesh = &mesh;
        m_boundGrobType = grobType;
        on_bind_to_mesh ();
    }
    
    void unbind_from_mesh ()
    {
        m_boundMesh = nullptr;
        m_boundGrobType = {};
        on_unbind_from_mesh ();
    }

    virtual void grobs_changed (grob_t grobType, const Mesh& mesh);

    const Mesh* bound_mesh () const
    {
        return m_boundMesh;
    }

    std::optional <grob_t> bound_grob_type () const
    {
        return m_boundGrobType;
    }

    virtual void do_imgui ()
    {
    }

    virtual bool has_imgui () const
    {
        return false;
    }
    
protected:
    virtual void on_bind_to_mesh ()
    {
    }

    virtual void on_unbind_from_mesh ()
    {
    }

    virtual void on_grobs_changed ()
    {
    }

private:
    const Mesh*             m_boundMesh {nullptr};
    std::optional <grob_t>  m_boundGrobType;
};

}//	end of namespace lume

#endif	//__H__lume_annex
