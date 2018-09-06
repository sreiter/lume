#ifndef __H__slimesh__normals
#define __H__slimesh__normals

#include <memory>
#include "grob.h"
#include "mesh.h"
#include "types.h"

namespace slimesh {


///	computes the normal of a single triangle
/**
 * \param normalOut	array of length 3. The resulting normal will be written to this array.
 * \param c0		array of length 3. coordinate of first corner.
 * \param c1		array of length 3. coordinate of second corner.
 * \param c2		array of length 3. coordinate of third corner.
 */
real_t* TriangleNormal3 (real_t* normalOut,
                         const real_t* c0,
                         const real_t* c1,
                         const real_t* c2);


///	computes the vertex normals of a mesh and stores them in the specified data array
void
ComputeFaceVertexNormals3 (Mesh& meshInOut,
                           const std::string& normalId = "normals");

}// end of namespace slimesh

#endif	//__H__slimesh__normals
