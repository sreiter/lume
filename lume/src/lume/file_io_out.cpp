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
#include <array>
#include <fstream>
#include <string>
#include <lume/file_io.h>

namespace
{
using namespace lume;

enum class FileType
{
  UGX
};

FileType GetFileTypeFromSuffix (std::string const& filename)
{
  std::string suffix = filename.substr (filename.size() - 4, 4);
  transform (suffix.begin (), suffix.end (), suffix.begin (), ::tolower);

  if (suffix == ".ugx" ){
    return FileType::UGX;
  }
  else {
    throw FileSuffixError () << filename;
  }
}

const char* GetUGXElementLabelFromGrobType (GrobType grobType)
{
  static const std::array <const char*, 4> labels {"vertices",
                                                   "edges",
                                                   "triangles",
                                                   "quadrilaterals"};

  return labels.at (grobType);
}

const char* GetUGXElementSetLabelFromGrobSetType (GrobSetType grobSetType)
{
  static const std::array <const char*, 11> labels {
    "vertices",
    "edges",
    "faces",
    "faces",
    "volumes",
    "volumes",
    "volumes",
    "volumes",
    "no_grob_set",
    "faces",
    "volumes"};

  return labels.at (grobSetType);
}

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

void WriteGrobsToUGX (std::ostream& out,
                      GrobArray const& grobs)
{
  if (grobs.empty ())
    return;

  auto const label = GetUGXElementLabelFromGrobType (grobs.grob_type ());

  out << "\t<" << label << ">";
  WriteArray (out, grobs.underlying_array ());
  out << "</" << label << ">\n";
}

void WriteGrobsToUGX (std::ostream& out,
                      Mesh const& mesh,
                      GrobType grobType)
{
  WriteGrobsToUGX (out, mesh.grobs (grobType));
}

void WriteConsecutiveSubsetIndicesToUGX (std::ostream& out,
                                         size_t baseIndex,
                                         size_t numIndices,
                                         const char* label)
{
  if (numIndices == 0)
    return;

  out << "\t\t<" << label << ">";
  out << baseIndex;
  for(size_t i = 1; i < numIndices; ++i) {
    out << " " << baseIndex + i;
  }
  out << "</" << label << ">\n";
}

void WriteSubsetGrobsToUGX (std::ostream& out, GrobArray const& grobs)
{
  auto const label = GetUGXElementSetLabelFromGrobSetType (static_cast <GrobSetType> (grobs.grob_type ()));
  WriteConsecutiveSubsetIndicesToUGX (out, 0, grobs.size (), label);  
}

void WriteSubsetGrobsToUGX (std::ostream& out, Mesh const& mesh, GrobSetType grobSetType)
{
  auto const label = GetUGXElementSetLabelFromGrobSetType (grobSetType);
  WriteConsecutiveSubsetIndicesToUGX (out, 0, mesh.num (grobSetType), label);
}

void WriteVerticesToUGX (std::ostream& out, RealArrayAnnex const& vertices)
{
  out << "\t<vertices coords=\"" << vertices.tuple_size () << "\">";
  WriteArray (out, vertices);
  out << "</vertices>\n";
}

void WriteUGXHeader (std::ostream& out)
{
  out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
}

void SaveMeshToUGX (Mesh const& mesh,
                    std::string filename,
                    TypedAnnexKey <RealArrayAnnex> const& vertexCoordsKey)
{
  std::ofstream out (filename);
  if (!out) throw CannotOpenFileError () << "'" << filename << "' for writing.";

  auto const& coords = mesh.annex (vertexCoordsKey);

  WriteUGXHeader (out);
  out << "<grid name=\"defGrid\">\n";
  WriteVerticesToUGX (out, coords);

  WriteGrobsToUGX (out, mesh, EDGE);
  WriteGrobsToUGX (out, mesh, TRI);
  WriteGrobsToUGX (out, mesh, QUAD);

  out << "<subset_handler name=\"defSH\">\n";
  out << "\t<subset name=\"all\">\n";
  WriteSubsetGrobsToUGX (out, mesh, VERTICES);
  WriteSubsetGrobsToUGX (out, mesh, EDGES);
  WriteSubsetGrobsToUGX (out, mesh, FACES);
  out << "\t</subset>\n";
  out << "</subset_handler>\n";

  out << "</grid>\n\n";
}

void SaveGrobsToUGX (GrobArray const& grobs,
                     std::string const& filename,
                     RealArrayAnnex const& coords)
{
  std::ofstream out (filename);
  if (!out) throw CannotOpenFileError () << "'" << filename << "' for writing.";

  WriteUGXHeader (out);
  out << "<grid name=\"defGrid\">\n";
  WriteVerticesToUGX (out, coords);
  WriteGrobsToUGX (out, grobs);

  out << "<subset_handler name=\"defSH\">\n";
  out << "\t<subset name=\"all\">\n";
  WriteSubsetGrobsToUGX (out, grobs);
  out << "\t</subset>\n";
  out << "</subset_handler>\n";
  
  out << "</grid>\n\n";
}

}// end of namespace

namespace lume
{

void SaveMeshToFile (Mesh const& mesh,
                     std::string filename,
                     TypedAnnexKey <RealArrayAnnex> const& vertexCoordsKey)
{
  auto const fileType = GetFileTypeFromSuffix (filename);
  switch (fileType)
  {
    case FileType::UGX:
      SaveMeshToUGX (mesh, std::move (filename), vertexCoordsKey);
      break;
  }
}

void SaveGrobsToFile (GrobArray const& grobs,
                      std::string const& filename,
                      RealArrayAnnex const& coordinates)
{
  auto const fileType = GetFileTypeFromSuffix (filename);
  switch (fileType)
  {
    case FileType::UGX:
      SaveGrobsToUGX (grobs, filename, coordinates);
      break;
  }
}                     

}// end of namespace lume
