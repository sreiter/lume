// This file is part of lume, a C++ library for lightweight unstructured meshes
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

#include <stdexcept>
#include <string>
#include <lume/to_string.h>

/// Declares an exception class. baseClass should derive be LumeError or a derived class.
#define DECLARE_CUSTOM_EXCEPTION(className, baseClass) \
    class className : public baseClass {\
    public:\
        className () : baseClass (#className) {}\
    protected:\
        className (const char* derivedClassName) : baseClass (derivedClassName) {}\
    }; 

namespace lume {

/// The base class for all exceptions thrown by slimesh
class LumeError : public std::runtime_error
{
public:
    LumeError ()
        : std::runtime_error ("")
        , m_what ("LumeError: ")
    {}

    const char* what () const noexcept override {return m_what.c_str();}

    template <class T>
    LumeError& operator << (const T& t) {m_what.append (lume::to_string (t)); return *this;}
    LumeError& operator << (const char* t) {m_what.append (t); return *this;}

protected:
    LumeError (const char* derivedClassName)
        : std::runtime_error ("")
        , m_what (derivedClassName)
    {
        m_what.append (": ");
    }

private:
    std::string m_what;
};

DECLARE_CUSTOM_EXCEPTION (BadNumberOfIndices, LumeError);

DECLARE_CUSTOM_EXCEPTION (BadTupleSizeError, LumeError);

DECLARE_CUSTOM_EXCEPTION (AnnexError, LumeError);
DECLARE_CUSTOM_EXCEPTION (NoSuchAnnexError, AnnexError)
DECLARE_CUSTOM_EXCEPTION (AnnexTypeError, AnnexError);

DECLARE_CUSTOM_EXCEPTION (FileIOError, LumeError);
DECLARE_CUSTOM_EXCEPTION (FileSuffixError, FileIOError);
DECLARE_CUSTOM_EXCEPTION (FileNotFoundError, FileIOError);
DECLARE_CUSTOM_EXCEPTION (FileParseError, FileIOError);
DECLARE_CUSTOM_EXCEPTION (CannotOpenFileError, FileIOError);

}// end of namespace lume
