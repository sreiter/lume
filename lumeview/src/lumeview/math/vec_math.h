// This file is part of lumeview, a lightweight viewer for unstructured meshes
//
// Copyright (C) 2018 Sebastian Reiter
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

#ifndef __H__glm__vec_math
#define __H__glm__vec_math

#include <cmath>
#include <glm/geometric.hpp>

namespace glm {

///	uniform distributed random numbers in [lowerBound, upperBound[. Use srand to set a seed.
// template <class TNumber>
// TNumber
// urand(TNumber lowerBound, TNumber upperBound)
// {
// 	long t = rand();
// 	if(t == RAND_MAX)
// 		t -= 1;
// 	return lowerBound + (TNumber)((upperBound - lowerBound) * ((double)t / (double)RAND_MAX));
// }


// template <class number_t>
// inline
// number_t
// sq(number_t a)
// {
// 	return a * a;
// }


// template <class number_t>
// inline number_t
// DegToRad(number_t deg)
// {
// 	return deg * constants<number_t>::pi / 180.;
// }


// template <class number_t>
// inline number_t
// RadToDeg(number_t rad)
// {
// 	return rad * 180. / constants<number_t>::pi;
// }

template <class vector_t>
typename vector_t::value_type
lengthSq (const vector_t& v)
{
	return dot (v, v);
}


template <class vector_t>
vector_t
safeNormalize(
	const vector_t& v)
{
	const typename vector_t::value_type lenSq = lengthSq(v);
	if(lenSq > 0)
		return v / sqrt(lenSq);
	return v;
}


template <class vector_t>
typename vector_t::value_type
distancePointToPoint(
	const vector_t& p0,
	const vector_t& p1)
{
	return length(p1-p0);
}


template <class vector_t>
typename vector_t::value_type
distancePointToPointSq(
	const vector_t& p0,
	const vector_t& p1)
{
	const vector_t v = p1-p0;
	return dot(v,v);
}


template <class vector_t>
vector_t
pointOnRay(
	const vector_t& from,
	const typename vector_t::value_type s,
	const vector_t& dir)
{
	return from + s * dir;
}


template <class vector_t>
typename vector_t::value_type
projectPointToRay(
	const vector_t& v,
	const vector_t& from,
	const vector_t& dir)
{
	using scalar = typename vector_t::value_type;

	scalar d2 = lengthSq(dir);
	
//	avoid division by zero
	if(d2 != 0){
	//	calculate the projection p'
		return dot(dir, v - from) / d2;
	}

//	projection failed since direction has zero length
	return 0;
}


template <class vector_t>
typename vector_t::value_type
projectPointToRay(
	vector_t& vOut,
	const vector_t& v,
	const vector_t& from,
	const vector_t& dir)
{
	typedef typename vector_t::value_type	scalar;

	scalar d2 = lengthSq(dir);
	
//	avoid division by zero
	if(d2 != 0){
	//	calculate the projection p'
		scalar s = dot(dir, v - from) / d2;
		vOut = from + s * dir;
		return s;
	}

//	projection failed since direction has zero length
	vOut = from;
	return 0;
}


template <class vector_t>
typename vector_t::value_type
distancePointToRay(
	const vector_t& v,
	const vector_t& from,
	const vector_t& dir)
{
	vector_t tmp;
	projectPointToRay(tmp, v, from, dir);
	return distancePointToPoint(v, tmp);
}


template <class vector_t>
typename vector_t::value_type
projectPointToPlane(vector_t& vOut, const vector_t& v,
					const vector_t& p, const vector_t& n)
{
	const vector_t t = v - p; 
	const vector_t norm = normalize (n);
	const typename vector_t::value_type s = dot(norm, t);
	vOut = v - norm * s;
	return s;
}


template <class vector_t>
typename vector_t::value_type
offsetPointToPlane(const vector_t& v, const vector_t& p, const vector_t& n)
{
	const vector_t t = v - p; 
	const vector_t norm = normalize (n);
	return dot(norm, t);
}

template <class vector_t>
typename vector_t::value_type
distancePointToPlane(const vector_t& v, const vector_t& p, const vector_t& n)
{
	return fabs (offsetPointToPlane (v, p, n));
}


template <class vector_t>
bool rayPlaneIntersection(vector_t& vOut,
                          typename vector_t::value_type& tOut,
						  const vector_t& rayFrom,
						  const vector_t& rayDir,
						  const vector_t& p,
						  const vector_t& n)
{
//	solve: t = (p-rayFrom)*n / rayDir*n
	const typename vector_t::value_type denom = dot(rayDir, n);
	if (denom == 0)
		return false;

//	calculate intersection parameter
	const vector_t v = p - rayFrom;
	tOut = dot(v, n) / denom;
	
//	calculate intersection point
	vOut = rayFrom + rayDir * tOut;
	return true;
}

template <class vector_t>
bool rayPlaneIntersection(vector_t& vOut,
						  const vector_t& rayFrom,
						  const vector_t& rayDir,
						  const vector_t& p,
						  const vector_t& n)
{
	typename vector_t::value_type t;
	return rayPlaneIntersection (vOut, t, rayFrom, rayDir, p, n);
}

// template <class vector_t>
// bool
// RayRayIntersection2d(
// 	typename vector_t::value_type& t0Out,
// 	typename vector_t::value_type& t1Out,
// 	const vector_t& from0,
// 	const vector_t& dir0,
// 	const vector_t& from1,
// 	const vector_t& dir1)
// {
// 	// we search for the intersection of the two rays (from0 + t0Out * dir0)
// 	// (from1 + t1Out * dir1). To this end we set up and solve the system
// 	//   ( dir0[0] , - dir1[0] ) ( t0Out ) = ( (from1 - from0)[0] )
// 	//   ( dir0[1] , - dir1[1] ) ( t1Out ) = ( (from1 - from0)[1] )

// 	typedef typename vector_t::value_type	scalar;
// 	typedef Eigen::Matrix<scalar, 2, 2>	matrix22;

// 	matrix22 A;
// 	A << dir0[0], -dir1[0],
// 		 dir0[1], -dir1[1];

// 	// TODO: This check doesn't treat identical rays correctly
// 	if(A.determinant() == 0)
// 		return false;

// 	vector_t v = A.inverse() * (from1 - from0);

// 	// parameters of the intersection
// 	t0Out = v[0];
// 	t1Out = v[1];

// 	return true;
// }


// template <class vector_t>
// bool
// RayRayIntersection2d(
// 	vector_t& vOut,
// 	typename vector_t::value_type& t0Out,
// 	typename vector_t::value_type& t1Out,
// 	const vector_t& from0,
// 	const vector_t& dir0,
// 	const vector_t& from1,
// 	const vector_t& dir1)
// {
// 	if(RayRayIntersection2d(t0Out, t1Out, from0, dir0, from1, dir1)){
// 		vOut = from0 + t0Out * dir0;
// 		return true;
// 	}

// 	return false;
// }


// /**	Returns false if the rays are parallel or if one of the directions is zero.
//  * Otherwise the method returns true and writes the local coordinates of the
//  * respective closest points to t1Out and t2Out.*/
// template <class vector_t>
// bool
// RayRayProjection(
// 		typename vector_t::value_type& t1Out,
// 		typename vector_t::value_type& t2Out,
// 		const vector_t& from1,
// 		const vector_t& dir1,
// 		const vector_t& from2,
// 		const vector_t& dir2)
// {
// 	typedef typename vector_t::value_type scalar;
// 	const scalar smallSq = constants<scalar>::relSmallSquared;

// 	const vector_t ab = from2 - from1;
// 	const scalar l11 = dir1.dot(dir1);
// 	const scalar l12 = -dir1.dot(dir2);
// 	const scalar l22 = dir2.dot(dir2);
// 	const scalar ra = dir1.dot(ab);
// 	const scalar rb = -dir2.dot(ab);

// //	l11 and l22 are always >= 0
// 	if((l11 < smallSq) || (l22 < smallSq))
// 		return false;

// 	const scalar tmp = l11 * l22 - l12 * l12;
// 	if(fabs(tmp) < smallSq)
// 		return false;

// 	t2Out = (l11*rb - l12*ra) / tmp;
// 	t1Out = (ra - l12*t2Out) / l11;
// 	return true;
// }


// template <class vector_t1, class vector_t2, class vector_t3, class vector_t4>
// bool RayPlaneIntersection(typename vector_t1::Scalar& tOut,
// 						  const Eigen::MatrixBase<vector_t1>& rayFrom,
// 						  const Eigen::MatrixBase<vector_t2>& rayDir,
// 						  const Eigen::MatrixBase<vector_t3>& p,
// 						  const Eigen::MatrixBase<vector_t4>& n)
// {
// 	typedef typename vector_t1::Scalar scalar;
// 	const scalar smallSq = constants<scalar>::relSmallSquared;

// //	solve: t = (p-rayFrom)*n / rayDir*n
// 	const scalar denom = rayDir.dot(n);
// 	if(fabs(denom) < smallSq)
// 		return false;
	
// //	calculate intersection parameter
// 	const vector_t1 v = p - rayFrom;
// 	tOut = v.dot(n) / denom;
	
// 	return true;
// }


// template <class vector_t>
// bool
// RaySphereIntersection(
// 	typename vector_t::value_type& t0Out,
// 	typename vector_t::value_type& t1Out,
// 	const vector_t& from,
// 	const vector_t& dir,
// 	const vector_t& center,
// 	const typename vector_t::value_type radius)
// {
// 	typedef typename vector_t::value_type	number_t;
// 	number_t a = 2 * dir.squaredNorm();
// 	if(a == 0)
// 		return false;

// 	vector_t q = from - center;
// 	number_t b = 2 * dir.dot(q);
// 	number_t c = q.squaredNorm() - sq(radius);

// 	number_t d = sq(b) - 2 * a * c;

// 	if(d < 0)
// 		return false;

// 	number_t r = sqrt(d);
// 	t0Out = (-b - r) / a;
// 	t1Out = (r - b) / a;

// 	return true;
// }


// template <class vector_t>
// vector_t
// TriangleNormalNoNormalize(
// 	const vector_t& v0,
// 	const vector_t& v1,
// 	const vector_t& v2)
// {
// 	return(v2 - v0).cross(v1 - v0);
// }


// template <class vector_t>
// vector_t
// TriangleNormal(
// 	const vector_t& v0,
// 	const vector_t& v1,
// 	const vector_t& v2)
// {
// 	return SafeNormalize (TriangleNormalNoNormalize (v0, v1, v2));
// }


// template <class vector_t>
// bool
// BoxBoundProbe(
// 	const vector_t& v,
// 	const vector_t& boxMin,
// 	const vector_t& boxMax)
// {
// 	for(size_t i = 0; i < vector_t::SizeAtCompileTime; ++i){
// 		if(v[i] < boxMin[i] || v[i] > boxMax[i])
// 			return false;
// 	}
// 	return true;
// }

// template <class vector_t>
// bool
// OpenBoxBoundProbe(
// 	const vector_t& v,
// 	const vector_t& boxMin,
// 	const vector_t& boxMax)
// {
// 	for(size_t i = 0; i < vector_t::SizeAtCompileTime; ++i){
// 		if(v[i] <= boxMin[i] || v[i] >= boxMax[i])
// 			return false;
// 	}
// 	return true;
// }


// template <class vector_t>
// bool
// RayBoxIntersection(
// 	typename vector_t::value_type& t0Out,
// 	typename vector_t::value_type& t1Out,
// 	const vector_t& rayFrom,
// 	const vector_t& rayDir,
// 	const vector_t& boxMin,
// 	const vector_t& boxMax)
// {
// 	typedef typename vector_t::value_type	number_t;

// 	bool bMinMaxSet = false;
// 	t0Out = t1Out = 0;

// 	for(size_t i = 0; i < vector_t::SizeAtCompileTime; ++i){
// 		if(rayDir[i] != 0){
// 			//get near and far
// 			number_t t0, t1;
// 			t0 = (boxMin[i] - rayFrom[i]) / rayDir[i];
// 			t1 = (boxMax[i] - rayFrom[i]) / rayDir[i];

// 			if(t0 > t1)
// 				std::swap(t0, t1);

// 			if(bMinMaxSet){
// 				if((t0 <= t1Out) && (t1 >= t0Out)){
// 					t0Out = std::max(t0, t0Out);
// 					t1Out = std::min(t1, t1Out);
// 				}
// 				else
// 					return false;
// 			}
// 			else{
// 				t0Out = t0;
// 				t1Out = t1;
// 				bMinMaxSet = true;
// 			}
// 		}
// 		else if((rayFrom[i] < boxMin[i]) || (rayFrom[i] > boxMax[i]))
// 			return false;
// 	}
	
// 	return (bMinMaxSet || BoxBoundProbe(rayFrom, boxMin, boxMax));
// }

// template <class vector_t>
// bool
// RayBoxIntersection(
// 	typename vector_t::value_type& t0Out,
// 	typename vector_t::value_type& t1Out,
// 	vector_t& norm0Out,
// 	vector_t& norm1Out,
// 	const vector_t& rayFrom,
// 	const vector_t& rayDir,
// 	const vector_t& boxMin,
// 	const vector_t& boxMax)
// {
// 	using std::swap;

// 	typedef typename vector_t::value_type	number_t;

// 	bool bMinMaxSet = false;
// 	t0Out = t1Out = 0;
// 	norm0Out.setZero();
// 	norm1Out.setZero();

// 	for(size_t i = 0; i < vector_t::SizeAtCompileTime; ++i){
// 		if(rayDir[i] != 0){
// 			//get near and far
// 			number_t t0, t1;
// 			t0 = (boxMin[i] - rayFrom[i]) / rayDir[i];
// 			t1 = (boxMax[i] - rayFrom[i]) / rayDir[i];

// 			number_t up = 1;

// 			if(t0 > t1){
// 				up = -1;
// 				swap(t0, t1);
// 			}

// 			if(bMinMaxSet){
// 				if((t0 <= t1Out) && (t1 >= t0Out)){
// 					if(t0 > t0Out){
// 						t0Out = t0;
// 						norm0Out.setZero();
// 						norm0Out[i] = -up;
// 					}

// 					if(t1 < t1Out){
// 						t1Out = t1;
// 						norm1Out.setZero();
// 						norm1Out[i] = up;
// 					}
// 				}
// 				else
// 					return false;
// 			}
// 			else{
// 				t0Out = t0;
// 				norm0Out[i] = -up;
// 				t1Out = t1;
// 				norm1Out[i] = up;
// 				bMinMaxSet = true;
// 			}
// 		}
// 		else if((rayFrom[i] < boxMin[i]) || (rayFrom[i] > boxMax[i]))
// 			return false;
// 	}
	
// 	return (bMinMaxSet || BoxBoundProbe(rayFrom, boxMin, boxMax));
// }

// template <class vector_t>
// bool RayCylinderIntersection (
// 		typename vector_t::value_type& tMinOut,
// 		typename vector_t::value_type& tMaxOut,
// 		vector_t& n0Out,
// 		vector_t& n1Out,
// 		const vector_t& rayFrom,
// 		const vector_t& rayDir,
// 		const vector_t& cylCenter,
// 		const vector_t& cylAxis,
// 		typename vector_t::value_type cylRadius)
// {
// 	using std::swap;
// 	typedef typename vector_t::value_type	scalar;
// 	const scalar smallSq = constants<scalar>::relSmallSquared;

// //	find the closest points on the ray and the cylinder axis
// 	scalar tRay, tCyl;
// 	if(!RayRayProjection (tRay, tCyl, rayFrom, rayDir, cylCenter, cylAxis)){
// 	//	rays are parallel. Check whether rayFrom lies inside the cylinder.
// 		if(DistancePointToRay(rayFrom, cylCenter, cylAxis) <= cylRadius){
// 			tMinOut = ProjectPointToRay(cylCenter, rayFrom, rayDir);
// 			tMaxOut = ProjectPointToRay(cylCenter + cylAxis, rayFrom, rayDir);
// 			n0Out = -cylAxis;
// 			n1Out = cylAxis;
// 			return true;
// 		}
// 		return false;
// 	}

// 	const vector_t pRay = PointOnRay (rayFrom, tRay, rayDir);
// 	const vector_t pCyl = PointOnRay (cylCenter, tCyl, cylAxis);

// 	const scalar cylRadiusSq = sq(cylRadius);
// 	const scalar rayAxisDistSq = DistancePointToPointSq(pRay, pCyl);

// 	if(rayAxisDistSq > cylRadiusSq)
// 		return false;

// //	parallel and normalized orthogonal component of rayDir regarding cylAxis
// 	const vector_t axisNormized = cylAxis.normalized();
// 	const vector_t dirParallel = axisNormized * axisNormized.dot(rayDir);
// 	const vector_t dirOrtho = rayDir - dirParallel;

// //	calculate the distance from rayFrom, where rayFrom+t*dirOrtho intersects the cylinder
// 	const scalar orthoIntersectionDist = sqrt(cylRadiusSq - rayAxisDistSq);

// 	const scalar orthoLen = dirOrtho.norm();

// //	this should not happen. Otherwise the rays were parallel
// 	if(orthoLen == 0)
// 		return false;

// //	the factor by which rayDir has to be scaled to reach the intersection point
// //	starting from pRay.
// 	const scalar scaleFac = orthoIntersectionDist / orthoLen;

// 	scalar tInf[2] = {tRay - scaleFac, tRay + scaleFac};
// 	const vector_t v = PointOnRay(rayFrom, tMinOut, rayDir);
// 	const vector_t planes[2] = {cylCenter, cylCenter + cylAxis};
// 	scalar t[2];
// 	for(size_t iPlane = 0; iPlane < 2; ++iPlane){
// 		if(!RayPlaneIntersection(t[iPlane], rayFrom, rayDir,
// 								 planes[iPlane], cylAxis))
// 		{
// 		//	ray is orthogonal to axis
// 			vector_t p;
// 			const scalar s = ProjectPointToRay(p, rayFrom, cylCenter, cylAxis);
// 			tMinOut = tInf[0];
// 			tMaxOut = tInf[1];

// 			if(s >= 0 && s <= 1){
// 				{
// 					const vector_t v = PointOnRay(rayFrom, tMinOut, rayDir);
// 					vector_t p;
// 					ProjectPointToRay (p, v, cylCenter, cylAxis);
// 					n0Out = v - p;
// 					n0Out = SafeNormalize(n0Out);
// 				}
// 				{
// 					const vector_t v = PointOnRay(rayFrom, tMaxOut, rayDir);
// 					vector_t p;
// 					ProjectPointToRay (p, v, cylCenter, cylAxis);
// 					n1Out = v - p;
// 					n1Out = SafeNormalize(n1Out);
// 				}

// 				return true;
// 			}
// 			return false;
// 		}
// 	}

// 	scalar normFac = 1;
// 	if(t[0] > t[1]){
// 		swap(t[0], t[1]);
// 		normFac = -1;
// 	}

// 	if(t[0] > tInf[0]){
// 		tMinOut = t[0];
// 		n0Out = - cylAxis * normFac;
// 		n0Out = SafeNormalize(n0Out);
// 	}
// 	else{
// 		tMinOut = tInf[0];
// 		const vector_t v = PointOnRay(rayFrom, tMinOut, rayDir);
// 		vector_t p;
// 		ProjectPointToRay (p, v, cylCenter, cylAxis);
// 		n0Out = v - p;
// 		n0Out = SafeNormalize(n0Out);
// 	}

// 	if(t[1] < tInf[1]){
// 		tMaxOut = t[1];
// 		n1Out = cylAxis * normFac;
// 		n1Out = SafeNormalize(n1Out);
// 	}
// 	else{
// 		tMaxOut = tInf[1];
// 		const vector_t v = PointOnRay(rayFrom, tMaxOut, rayDir);
// 		vector_t p;
// 		ProjectPointToRay (p, v, cylCenter, cylAxis);
// 		n1Out = v - p;
// 		n1Out = SafeNormalize(n1Out);
// 	}

// 	return tMinOut <= tMaxOut;
// }

}//	end of namespace

#endif	//__H__glm__vec_math
