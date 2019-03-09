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

#ifndef __H__lume_arguments
#define __H__lume_arguments

#include <vector>
#include "lume/lume_error.h"
#include "lume/commands/types.h"

namespace lume {
namespace commands {

DECLARE_CUSTOM_EXCEPTION (BadArgumentError, LumeError);
DECLARE_CUSTOM_EXCEPTION (ArgumentsInitializationError, LumeError);
DECLARE_CUSTOM_EXCEPTION (BadNumberOfArgumentsError, LumeError);

class ArgumentDesc
{
public:
    ArgumentDesc (Type type, std::string name, std::string description)
        : m_type (type)
        , m_name (std::move (name))
        , m_description (std::move (description))
    {}

    Type                type ()        const   {return m_type;}
    const std::string&  name ()        const   {return m_name;}
    const std::string&  description () const   {return m_description;}

private:
    Type         m_type;
    std::string  m_name;
    std::string  m_description;
};

/// Provides function arguments. Only r-value instances supported.
class Arguments
{
public:
    static Arguments create (const std::vector <ArgumentDesc>& argDescs,
                             const std::vector <Variant>& argValues)
    {
        if (argDescs.size () != argValues.size())
        {
            throw ArgumentsInitializationError () << "argDesc and argValues have different size.";
        }

        return Arguments (argDescs, argValues);
    }

    template <class T>
    const T& get (const char* name) const
    {
        for(size_t i = 0; i < m_argDescs.size (); ++i)
        {
            if (m_argDescs [i].name ().compare (name) == 0)
            {
                return get <T> (i);
            }
        }

        throw BadArgumentError () << "Argument '" << name << "' not supported.";
    }

    template <class T>
    const T& get (size_t index) const
    {
        return std::get <T> (m_argValues.at (index));
    }

private:
    Arguments () = delete;
    Arguments (const Arguments&) = delete;
    Arguments (Arguments&&) = delete;
    Arguments& operator = (const Arguments&) = delete;
    Arguments& operator = (Arguments&&) = delete;

    Arguments (const std::vector <ArgumentDesc>& argDescs,
               const std::vector <Variant>& argValues)
        : m_argDescs (argDescs)
        , m_argValues (argValues)
    {}

private:
    const std::vector <ArgumentDesc>&   m_argDescs;
    const std::vector <Variant>&        m_argValues;
};

/// Translates the given set of strings to arguments corresponding to the specified argument descs
std::vector <Variant> TranslateArguments (const std::vector <ArgumentDesc>& argDescs, int argc, char** argv);

}// end of namespace commands
}// end of namespace lume

#endif    //__H__lume_arguments
