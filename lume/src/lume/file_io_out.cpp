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

#include <algorithm>
#include <fstream>
#include <lume/file_io.h>

namespace
{
using namespace lume;

template <class Array>
void WriteArray (std::ostream& out, Array const& array)
{
    if (array.empty ()) {
        return;
    }

    out << array [0];
    for(size_t i = 1; i < array.size (); ++i) {
        out << " " << array [i];
    }
}

void WriteGrobsToUGX (std::ostream& out, Mesh const& mesh, GrobType grobType, const char* label)
{
    if (mesh.num (grobType) == 0) {
        return;
    }

    out << "\t<" << label << ">";
    WriteArray (out, mesh.grobs (grobType).underlying_array ());
    out << "</" << label << ">\n";
}

void WriteSubsetGrobsToUGX (std::ostream& out, Mesh const& mesh, GrobSetType grobSetType, const char* label)
{
    auto const numGrobs = mesh.num (grobSetType);
    if (numGrobs == 0)
        return;

    out << "\t\t<" << label << ">";
    out << "0";
    for(size_t i = 0; i < numGrobs; ++i) {
        out << " " << i;
    }
    out << "</" << label << ">\n";
}

void SaveMeshToUGX (Mesh const& mesh,
                    std::string filename,
                    TypedAnnexKey <RealArrayAnnex> const& vertexCoordsKey)
{
    using namespace std;
    ofstream out (filename);
    if (!out) throw CannotOpenFileError () << "'" << filename << "' for writing.";

    auto const& coords = mesh.annex (vertexCoordsKey);

    out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    out << "<grid name=\"defGrid\">\n";
    out << "\t<vertices coords=\"" << coords.tuple_size () << "\">";
    WriteArray (out, coords);
    out << "</vertices>\n";

    WriteGrobsToUGX (out, mesh, EDGE, "edges");
    WriteGrobsToUGX (out, mesh, TRI, "triangles");
    WriteGrobsToUGX (out, mesh, QUAD, "quadrilaterals");

    out << "<subset_handler name=\"defSH\">\n";
    out << "\t<subset name=\"all\">\n";
    WriteSubsetGrobsToUGX (out, mesh, VERTICES, "vertices");
    WriteSubsetGrobsToUGX (out, mesh, EDGES, "edges");
    WriteSubsetGrobsToUGX (out, mesh, FACES, "faces");
    out << "\t</subset>\n";
    out << "</subset_handler>\n";

    out << "</grid>\n\n";
}

}

namespace lume
{

void SaveMeshToFile (Mesh const& mesh,
                     std::string filename,
                     TypedAnnexKey <RealArrayAnnex> const& vertexCoordsKey)
{
    std::string suffix = filename.substr (filename.size() - 4, 4);
    transform (suffix.begin (), suffix.end (), suffix.begin (), ::tolower);

    if (suffix == ".ugx" ){
        SaveMeshToUGX (mesh, std::move (filename), vertexCoordsKey);
    }
    else {
        throw FileSuffixError () << filename;
    }
}

}// end of namespace lume
