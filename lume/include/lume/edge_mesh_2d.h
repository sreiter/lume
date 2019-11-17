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

#include <functional>
#include <vector>

#include <lume/mesh.h>
#include <lume/math/tuple.h>

namespace lume
{

class EdgeMesh2d
{
public:
  struct Edge {
    index_t from;
    index_t to;
  };

  enum class Boundary
  {
    None,
    Left, ///< Adacent triangle is on the right of the edge
    Right ///< Adjacent triangle is on the left of the edge
  };

  struct Connection {
    index_t to;
    double pseudoAngle;
    Boundary boundary;
  };

  using Connections = std::vector <Connection>;

public:
  /** Constructs an edge mesh from the given array of counterclockwise oriented triangle and the
    given vector of positions.
    Positions::value_type is expected to be 2-tuples (e.g. through lume::TupleView) or
    compatible types (e.g. glm::vec2).

    Note: positions has to be a valid reference for the whole lifetime of the created instance.
  */
  template <class Positions>
  static EdgeMesh2d fromTrianglesCCW (GrobArray const& triangles, Positions const& positions);

  /** The specified callback has to provide 2d coordinates given a vertex index.*/
  EdgeMesh2d (std::function <std::tuple <double, double> (index_t)> coordinateCallback);
  EdgeMesh2d (EdgeMesh2d const& other);
  EdgeMesh2d (EdgeMesh2d&& other);

  EdgeMesh2d& operator = (EdgeMesh2d const& other);
  EdgeMesh2d& operator = (EdgeMesh2d&& other);

  /** If an edge is already present in the edge mesh, no action is performed and false is returned.*/
  bool add_edge (Edge const& edge, Boundary boundary = Boundary::None);
  bool has_edge (Edge const& edge) const;
  void remove_edge (Edge const& edge);
  void remove_edges_with_vertex (index_t vertex);
  
  bool swap_edge (Edge const& edge);
  
  Connections const& vertex_connections (index_t vertex) const;

  GrobArray create_triangles () const;

private:
  using CoordinateCallback = std::function <std::tuple <double, double> (index_t)>;
  using Coordinate = math::TupleWithStorage <double>;

private:
  bool insert_connection (index_t const from, index_t const to, Boundary boundary);
  void remove_connection (index_t const from, index_t const to);
  double compute_pseudo_angle (index_t const from, index_t const to) const;
  bool is_valid (Edge const& edge) const;

  Connections& vertex_connections (index_t vertex);

  bool has_connection (index_t const from, index_t const to) const;

  Connections::iterator find_connection (Connections& connections, index_t const to);
  Connections::const_iterator find_connection (Connections const& connections, index_t const to) const;

  Connections::iterator find_connection_lower_bound (Connections& connections,
                                                     double const pseudoAngle);
  
  std::optional <Edge> swap_candidate (Edge const& edge) const;

  Coordinate coordinate (index_t vertex) const;

  Coordinate direction (index_t const from, index_t const to) const;

  Coordinate normal (index_t const from, index_t const to) const;

  bool edges_intersect (Edge const& edge1, Edge const& edge2) const;
  
  bool edge_separates_vertices (Edge const& edge,
                                index_t const vertex1,
                                index_t const vertex2) const;

private:
  std::vector <Connections> m_connections;
  CoordinateCallback m_coordinateCallback;
};

}// end of namespace lume

#include "edge_mesh_2d_impl.h"