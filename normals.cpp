#include "normals.h"
#include "mesh.h"
#include "vec_math_raw.h"

namespace slimesh {

real_t* TriangleNormal3 (real_t* normalOut,
                         const real_t* c0,
                         const real_t* c1,
                         const real_t* c2)
{
	real_t d0[3];
	real_t d1[3];

	VecSub (d0, 3, c1, c0);
	VecSub (d1, 3, c2, c0);

	return VecNormalize (VecCross3 (normalOut, d0, d1), 3);
}


void
ComputeFaceVertexNormals3 (Mesh& mesh,
						  const std::string& normalId)
{
	if (mesh.coords()->tuple_size() != 3)
		throw BadTupleSizeError (std::to_string (mesh.coords()->tuple_size()));

	auto& normalArray = *mesh.annex<RealArrayAnnex> (normalId, VERTEX);
	normalArray.set_tuple_size (3);
	normalArray.resize (mesh.num_coords());
	VecSet (UNPACK_DS(normalArray), 0);

	const real_t*	coords		= mesh.coords()->raw_ptr();
	real_t* 		normals		= normalArray.raw_ptr();
	
	for(auto gt : GrobSet (FACES)) {
		const index_t*	inds		= mesh.inds (gt)->raw_ptr();
		const index_t	numInds		= mesh.inds (gt)->num_indices();
		const index_t	numCorners	= mesh.inds (gt)->grob_desc ().num_corners ();
		const index_t	offset = numCorners / 2;

		for (index_t i = 0; i < numInds; i += numCorners) {
			const index_t* elem = inds + i;

			real_t d0[3];
			real_t d1[3];

			VecSub (d0, 3, coords + elem[offset] * 3, coords + elem[0] * 3);
			VecSub (d1, 3, coords + elem[1 + offset] * 3, coords + elem[1] * 3);

			real_t n[3];
			VecNormalize (VecCross3 (n, d0, d1), 3);

			for(index_t j = 0; j < numCorners; ++j)
				VecAppend (normals + elem [j] * 3, 3, n);
		}
	}

	VecTupNormalize (UNPACK_DST(normalArray));
}

}// end of namespace slimesh
