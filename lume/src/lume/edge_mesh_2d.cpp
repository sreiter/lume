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

#include <fstream>
#include <lume/edge_mesh_2d.h>

namespace lume
{

namespace
{
  using Boundary = EdgeMesh2d::Boundary;

  Boundary InverseBoundary (Boundary const boundary)
  {
    switch (boundary)
    {
      case Boundary::None: return Boundary::None;
      case Boundary::Left: return Boundary::Right;
      case Boundary::Right: return Boundary::Left;
      default:
        assert (false);
        return Boundary::None;
    }
  }
}

EdgeMesh2d::EdgeMesh2d (std::function <std::tuple <double, double> (index_t)> coordinateCallback)
  : m_coordinateCallback (coordinateCallback)
{
}

EdgeMesh2d::EdgeMesh2d (EdgeMesh2d const& other)
  : m_connections (other.m_connections)
  , m_coordinateCallback (other.m_coordinateCallback)
{
}

EdgeMesh2d::EdgeMesh2d (EdgeMesh2d&& other)
  : m_connections (std::move (other.m_connections))
  , m_coordinateCallback (std::move (other.m_coordinateCallback))
{
}

EdgeMesh2d& EdgeMesh2d::operator = (EdgeMesh2d const& other)
{
  m_connections = other.m_connections;
  m_coordinateCallback = other.m_coordinateCallback;
  return *this;
}

EdgeMesh2d& EdgeMesh2d::operator = (EdgeMesh2d&& other)
{
  m_connections = std::move (other.m_connections);
  m_coordinateCallback = std::move (other.m_coordinateCallback);
  return *this;
}

bool EdgeMesh2d::add_edge (Edge const& edge, Boundary boundary)
{
  if (edge.from == edge.to) {
    assert (false);
    return false;
  }

  bool success = true;
  success &= insert_connection (edge.from, edge.to, boundary);
  success &= insert_connection (edge.to, edge.from, InverseBoundary (boundary));

  return success;
}

bool EdgeMesh2d::has_edge (Edge const& edge) const
{
  auto const& connections = vertex_connections (edge.from);
  return find_connection (connections, edge.to) != connections.end ();
}

bool EdgeMesh2d::swap_edge (Edge const& edge)
{
  assert (is_valid (edge));
  
  auto const swapCandidate = swap_candidate (edge);
  
  if (!swapCandidate ||
      has_edge (*swapCandidate) ||
      !edges_intersect (edge, *swapCandidate))
  {
    return false;
  }

  remove_edge (edge);
  add_edge (*swapCandidate);

  return true;
}

void EdgeMesh2d::remove_edge (Edge const& edge)
{
  remove_connection (edge.from, edge.to);
  remove_connection (edge.to, edge.from);
}

void EdgeMesh2d::remove_edges_with_vertex (index_t vertex, bool addBoundaryMarkers)
{
  auto& connections = vertex_connections (vertex);
  auto const p = coordinate (vertex);

  if (addBoundaryMarkers &&
      connections.size () > 1)
  {
    for (size_t i = 0; i < connections.size (); ++i)
    {
      size_t const j = (i + 1) % connections.size ();

      auto const iTo = connections [i].to;
      auto const jTo = connections [j].to;

      if (connections [i].boundary == Boundary::Left ||
          connections [j].boundary == Boundary::Right ||
          !triangle_is_ccw (p, coordinate (iTo), coordinate (jTo)))
      {
        continue;
      }

      auto& iCons = vertex_connections (iTo);
      auto jIter = find_connection (iCons, jTo);
      if (jIter != iCons.end ())
        jIter->boundary = Boundary::Left;

      auto& jCons = vertex_connections (jTo);
      auto iIter = find_connection (jCons, iTo);
      if (iIter != jCons.end ())
        iIter->boundary = Boundary::Right;
    }
  }

  // remove connections from connected vertices to this one
  for (auto const& c : connections) {
    assert (c.to != vertex);
    remove_connection (c.to, vertex);
  }

  connections.clear ();
}

size_t EdgeMesh2d::num_vertices () const
{
  return m_connections.size ();
}

auto EdgeMesh2d::vertex_connections (index_t vertex) -> Connections&
{
  if (vertex >= m_connections.size ())
    m_connections.resize (vertex + 1);
  return m_connections [vertex];
}

auto EdgeMesh2d::vertex_connections (index_t vertex) const -> Connections const&
{
  assert (vertex < m_connections.size ());
  return m_connections [vertex];
}

auto EdgeMesh2d::connections (index_t vertex) const -> Connections const&
{
  return vertex_connections (vertex);
}

GrobArray EdgeMesh2d::create_triangles () const
{
  GrobArray tris {TRI};
  for (index_t iVrt = 0; iVrt < static_cast <index_t> (m_connections.size ()); ++iVrt)
  {
    auto const p = coordinate (iVrt);
    auto const connections = m_connections [iVrt];
    auto const numConnections = connections.size ();
    for (size_t iCon = 0; iCon < numConnections; ++iCon)
    {
      if (connections [iCon].boundary == Boundary::Left)
        continue;

      auto const to0 = connections [iCon].to;
      auto const to1 = connections [(iCon + 1) % numConnections].to;

      if (iVrt < to0 &&
          iVrt < to1 &&
          triangle_is_ccw (p, coordinate (to0), coordinate (to1)))
      {
        tris.push_back ({iVrt, to0, to1});
      }
    }
  }

  return tris;
}

void EdgeMesh2d::save_connections (std::string const& filename) const
{
  std::ofstream out (filename);
  if (!out)
    return;

  for (index_t i = 0; i < num_vertices (); ++i)
  {
    auto c = coordinate (i);
    out << "v " << c [0] << " " << c [1] << " 0" << std::endl;
  }

  std::array <const char*, 3> subsetNames {"inner", "leftBnd", "rightBnd"};

  for (size_t iSubset = 0; iSubset < subsetNames.size (); ++iSubset)
  {
    out << "o " << subsetNames [iSubset] << std::endl;
    out << "usemtl (null)" << std::endl;

    Boundary boundary = static_cast <Boundary> (iSubset);
    for (size_t iVrt = 0; iVrt < num_vertices (); ++iVrt)
    {
      for (auto const& c : m_connections [iVrt])
      {
        if (c.boundary == boundary)
        {
          out << "f " << iVrt + 1 << " " << c.to + 1 << std::endl;
        }
      }
    }
  }
}

bool EdgeMesh2d::insert_connection (index_t const from, index_t const to, Boundary boundary)
{
  auto const pseudoAngle = compute_pseudo_angle (from, to);
  auto& connections = vertex_connections (from);
  auto const nextIter = find_connection_lower_bound (connections, pseudoAngle);
  if (nextIter == connections.end ()) {
    connections.push_back ({to, pseudoAngle, boundary});
    return true;
  }
  else if (nextIter->to != to) {
    connections.insert (nextIter, {to, pseudoAngle, boundary});
    return true;
  }
  return false;
}

void EdgeMesh2d::remove_connection (index_t const from, index_t const to)
{
  auto& connections = vertex_connections (from);
  auto const iter = find_connection (connections, to);
  if (iter != connections.end ())
    connections.erase (iter);
  return;
}

double EdgeMesh2d::compute_pseudo_angle (index_t const iFrom, index_t const iTo) const
{
  return compute_pseudo_angle (coordinate (iFrom), coordinate (iTo));
}

double EdgeMesh2d::compute_pseudo_angle (Coordinate const& from, Coordinate const& to) const
{
  auto dir = to - from;
  dir.normalize ();

  if (dir [1] >= 0)
    return 0.25 - 0.25 * dir [0];
  else
    return 0.75 + 0.25 * dir [0];
}

double EdgeMesh2d::pseudo_angle_distance_ccw (double fromAngle, double toAngle) const
{
  auto const t = toAngle - fromAngle;
  if (t < 0)
    return 1.0 - t;
  return t;
}

bool EdgeMesh2d::is_left_of (Coordinate const& a, Coordinate const& b, Coordinate const& c) const
{
  static constexpr double threshold = 1.e-12;
  auto const angleBA = compute_pseudo_angle (b, a);
  auto const angleBC = compute_pseudo_angle (b, c);
  auto const angleCB = compute_pseudo_angle (c, b);

  return pseudo_angle_distance_ccw (angleBC, angleBA) > threshold &&
         pseudo_angle_distance_ccw (angleBA, angleCB) > threshold;
}

bool EdgeMesh2d::triangle_is_ccw (Coordinate const& a, Coordinate const& b, Coordinate const& c) const
{
  return is_left_of (a, b, c) &&
         is_left_of (b, c, a) &&
         is_left_of (c, a, b);
}

bool EdgeMesh2d::is_valid (Edge const& edge) const
{
  return edge.from < m_connections.size () &&
         edge.to < m_connections.size ();
}

bool EdgeMesh2d::has_connection (index_t const from, index_t const to) const
{
  auto& connections = vertex_connections (from);
  return find_connection (connections, to) != connections.end ();
}

auto EdgeMesh2d::find_connection (Connections& connections, index_t const to) -> Connections::iterator
{
  return std::find_if (connections.begin (), connections.end (),
                       [to] (auto const& c) {return c.to == to;});
}

auto EdgeMesh2d::find_connection (Connections const& connections, index_t const to) const -> Connections::const_iterator
{
  return std::find_if (connections.begin (), connections.end (),
                       [to] (auto const& c) {return c.to == to;});
}

auto EdgeMesh2d::find_connection_lower_bound (Connections& connections,
                                              double const pseudoAngle)
-> Connections::iterator
{
  return std::find_if (connections.begin (), connections.end (),
                       [pseudoAngle] (auto const& c) {return c.pseudoAngle >= pseudoAngle;});
}

auto EdgeMesh2d::swap_candidate (Edge const& edge) const -> std::optional <Edge>
{
  assert (is_valid (edge));

  std::array <index_t, 2>  swapCandidates;
  size_t numSwapCandidates = 0;

  auto const& connections = vertex_connections (edge.from);
  for (auto const& c : connections) {
    if (c.to == edge.to) {
      if (c.boundary != Boundary::None)
        return {}; // boundary edges cannot be swapped
    }
    else if (has_connection (c.to, edge.to)) {
      if (numSwapCandidates < 2)
        swapCandidates [numSwapCandidates] = c.to;
      ++numSwapCandidates;
    }
  }

  if (numSwapCandidates == 2)
    return Edge {swapCandidates [0], swapCandidates [1]};
  return {};
}

auto EdgeMesh2d::coordinate (index_t vertex) const -> Coordinate
{
  auto c = Coordinate::uninitialized (2);
  std::tie (c [0], c [1]) = m_coordinateCallback (vertex);
  return c;
}

auto EdgeMesh2d::direction (index_t const from, index_t const to) const -> Coordinate
{
  return coordinate (to) - coordinate (from);
}
  
auto EdgeMesh2d::normal (index_t const from, index_t const to) const -> Coordinate
{
  auto d = direction (from, to);
  std::swap (d [0], d [1]);
  d [0] *= -1.;
  return d;
}

bool EdgeMesh2d::edges_intersect (Edge const& edge1, Edge const& edge2) const
{
  return edge_separates_vertices (edge1, edge2.from, edge2.to) &&
         edge_separates_vertices (edge2, edge1.from, edge1.to);
}

bool EdgeMesh2d::edge_separates_vertices (Edge const& edge,
                                          index_t const vertex1,
                                          index_t const vertex2) const
{
  auto const edgeFrom = coordinate (edge.from);
  auto const n = normal (edge.from, edge.to);

  return n.dot (coordinate (vertex1) - edgeFrom) *
         n.dot (coordinate (vertex2) - edgeFrom) < 0;
}

}// end of namespace lume
