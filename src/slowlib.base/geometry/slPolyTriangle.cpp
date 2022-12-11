/*
BSD 2-Clause License

Copyright (c) 2022, tyomaAni
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "slowlib.h"
#include "slowlib.base/geometry/slPolyTriangle.h"
#include "slowlib.base/geometry/slRay.h"

slPolyTriangle::slPolyTriangle() {}
slPolyTriangle::slPolyTriangle(const slVec4f& _v1, const slVec4f& _v2, const slVec4f& _v3)
	:
	v1(_v1),
	v2(_v2),
	v3(_v3)
{
	update();
}

slPolyTriangle::slPolyTriangle(const slVec3f& _v1, const slVec3f& _v2, const slVec3f& _v3)
	:
	v1(_v1),
	v2(_v2),
	v3(_v3)
{
	update();
}

void slPolyTriangle::update()
{
	e1 = slVec4f(v2.x - v1.x,
		v2.y - v1.y,
		v2.z - v1.z,
		0.f);
	e2 = slVec4f(v3.x - v1.x,
		v3.y - v1.y,
		v3.z - v1.z,
		0.f);
	//	e1.cross(e2, faceNormal);
}

void slPolyTriangle::center(slVec4f& out)
{
	out = (v1 + v2 + v3) * 0.3333333f;
}

bool slPolyTriangle::rayTest_MT(const slRay& ray, bool withBackFace, real_t& T, real_t& U, real_t& V, real_t& W)
{
	slVec4  pvec;
	ray.m_direction.cross(e2, pvec);
	float det = e1.dot(pvec);

	if (withBackFace)
	{
		if (std::fabs(det) < slEpsilon)
			return false;
	}
	else
	{
		if (det < slEpsilon && det > -slEpsilon)
			return false;
	}

	slVec4 tvec(
		ray.m_origin.x - static_cast<real_t>(v1.x),
		ray.m_origin.y - static_cast<real_t>(v1.y),
		ray.m_origin.z - static_cast<real_t>(v1.z),
		0.f);

	float inv_det = 1.f / det;
	U = tvec.dot(pvec) * inv_det;

	if (U < 0.f || U > 1.f)
		return false;

	slVec4  qvec;
	tvec.cross(e1, qvec);
	V = ray.m_direction.dot(qvec) * inv_det;

	if (V < 0.f || U + V > 1.f)
		return false;

	T = e2.dot(qvec) * inv_det;

	if (T < slEpsilon) return false;

	W = 1.f - U - V;
	return true;
}

bool slPolyTriangle::rayTest_Watertight(const slRay& ray, bool withBackFace, real_t& T, real_t& U, real_t& V, real_t& W)
{
	v1.w = 1.f;
	v2.w = 1.f;
	v3.w = 1.f;
	const auto A = v2 - ray.m_origin;
	const auto B = v3 - ray.m_origin;
	const auto C = v1 - ray.m_origin;

	const real_t Ax = A[ray.m_kx] - (ray.m_Sx * A[ray.m_kz]);
	const real_t Ay = A[ray.m_ky] - (ray.m_Sy * A[ray.m_kz]);
	const real_t Bx = B[ray.m_kx] - (ray.m_Sx * B[ray.m_kz]);
	const real_t By = B[ray.m_ky] - (ray.m_Sy * B[ray.m_kz]);
	const real_t Cx = C[ray.m_kx] - (ray.m_Sx * C[ray.m_kz]);
	const real_t Cy = C[ray.m_ky] - (ray.m_Sy * C[ray.m_kz]);

	U = (Cx * By) - (Cy * Bx);
	V = (Ax * Cy) - (Ay * Cx);
	W = (Bx * Ay) - (By * Ax);

	if (U == 0.f || V == 0.f || W == 0.f)
	{
		double CxBy = (double)Cx * (double)By;
		double CyBx = (double)Cy * (double)Bx;
		U = (float)(CxBy - CyBx);

		double AxCy = (double)Ax * (double)Cy;
		double AyCx = (double)Ay * (double)Cx;
		V = (float)(AxCy - AyCx);

		double BxAy = (double)Bx * (double)Ay;
		double ByAx = (double)By * (double)Ax;
		W = (float)(BxAy - ByAx);
	}

	if (withBackFace)
	{
		if ((U < 0.f || V < 0.f || W < 0.f) &&
			(U > 0.f || V > 0.f || W > 0.f))
			return false;
	}
	else
	{
		if (U < 0.f || V < 0.f || W < 0.f)
			return false;
	}

	real_t det = U + V + W;

	if (det == 0.f)
		return false;

	const real_t Az = ray.m_Sz * A[ray.m_kz];
	const real_t Bz = ray.m_Sz * B[ray.m_kz];
	const real_t Cz = ray.m_Sz * C[ray.m_kz];
	const real_t Ts = (U * Az) + (V * Bz) + (W * Cz);

	if (!withBackFace) // CULL
	{
		if (Ts < 0.f || Ts > slInfinity * det)
			return false;
	}
	else
	{
		if (det < 0.f && (Ts >= 0.f || Ts < slInfinity * det))
			return false;
		else if (det > 0.f && (Ts <= 0.f || Ts > slInfinity * det))
			return false;
	}

	const real_t invDet = 1.f / det;
	U = U * invDet;
	V = V * invDet;
	W = W * invDet;
	T = Ts * invDet;
	if (T < slEpsilon)
		return false;
	return true;
}
