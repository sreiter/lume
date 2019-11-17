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

#pragma once

#include "lume/edge_mesh_2d.h"
#include "lume/topology.h"

namespace lume
{
  template <class Positions>
  EdgeMesh2d EdgeMesh2d::fromTrianglesCCW (GrobArray const& triangles, Positions const& positions)
  {
    auto coordinateCallback = [&positions] (index_t i)
                              {
                                return std::make_tuple (static_cast <double> (positions [i][0]),
                                                        static_cast <double> (positions [i][1]));
                              };

    lume::EdgeMesh2d edgeMesh {std::move (coordinateCallback)};

    if (triangles.grob_desc ().grob_type () != TRI ||
        triangles.empty ())
    {
      assert (triangles.grob_desc ().grob_type () == TRI);
      return edgeMesh;
    }

    auto const edgeRefs = lume::FindUniqueSidesRefCounted (triangles, 1);

    for (auto const& edgeRef : edgeRefs)
    {
      auto const& edge = edgeRef.first;
      if (edgeRef.second == 1)
        edgeMesh.add_edge ({edge [0], edge [1]}, lume::EdgeMesh2d::Boundary::Right);
      else
        edgeMesh.add_edge ({edge [0], edge [1]});
    }

    return edgeMesh;
  }
}// end of namespace lume
