// This file is part of lume, a C++ library for lightweight unstructured meshes
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

#ifndef __H__lume_command
#define __H__lume_command

#include <string>
#include <vector>

#include "arguments.h"

namespace lume {
namespace commands {

class Command
{
public:
    virtual ~Command () = default;

    const std::string& name () const            {return m_name;}
    const std::string& description () const     {return m_description;}

    virtual int        version () const         {return 1;}
    
    virtual std::vector <ArgumentDesc> argument_descs () const   {return m_argDesc;}

    void execute (const Arguments& args)
    {
        run (args);
    }

protected:
    Command (std::string name, std::string description)
        : m_name (std::move (name))
        , m_description (std::move (description))
    {}

    Command (std::string name,
             std::string description,
             std::initializer_list <ArgumentDesc> argDesc)
        : m_name (std::move (name))
        , m_description (std::move (description))
        , m_argDesc (argDesc)
    {}

    virtual void run (const Arguments& args) = 0;

private:
    std::string                m_name;
    std::string                m_description;
    std::vector <ArgumentDesc> m_argDesc;
};

}// end of namespace commands
}// end of namespace lume

#endif    //__H__lume_command