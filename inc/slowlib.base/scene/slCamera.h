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

#pragma once
#ifndef __SL_SLOWLIBBASESCCAM_H__
#define __SL_SLOWLIBBASESCCAM_H__

#include "slowlib.base/math/slMatrix.h"

struct slCameraFrustum
{
	slVec4 m_planes[6u];

	bool PointInFrustum(const slVec4& point);
	bool SphereInFrustum(float radius, const slVec4& position);
	//bool AABBInFrustum(const Aabb& aabb, const v4f& position);

	enum FrustumSide
	{
		RIGHT = 0,		// The RIGHT side of the frustum
		LEFT = 1,		// The LEFT	 side of the frustum
		BOTTOM = 2,		// The BOTTOM side of the frustum
		TOP = 3,		// The TOP side of the frustum
		BACK = 4,		// The BACK	side of the frustum
		FRONT = 5			// The FRONT side of the frustum
	};

	void NormalizePlane(slVec4& plane)
	{
		// Here we calculate the magnitude of the normal to the plane (point A B C)
		// Remember that (A, B, C) is that same thing as the normal's (X, Y, Z).
		// To calculate magnitude you use the equation:  magnitude = sqrt( x^2 + y^2 + z^2)
		float magnitude = (float)sqrt(plane.x * plane.x +
			plane.y * plane.y +
			plane.z * plane.z);

		// Then we divide the plane's values by it's magnitude.
		// This makes it easier to work with.
		plane.x /= magnitude;
		plane.y /= magnitude;
		plane.z /= magnitude;
		plane.w /= magnitude;
	}

	// I don't know original author. I saw this in NeHe tutorials.
	void CalculateFrustum(slMat4& P, slMat4& V)
	{
		double* proj = P.data();
		double* modl = V.data();
		double   clip[16]; //clipping planes

		clip[0] = modl[0] * proj[0] + modl[1] * proj[4] + modl[2] * proj[8] + modl[3] * proj[12];
		clip[1] = modl[0] * proj[1] + modl[1] * proj[5] + modl[2] * proj[9] + modl[3] * proj[13];
		clip[2] = modl[0] * proj[2] + modl[1] * proj[6] + modl[2] * proj[10] + modl[3] * proj[14];
		clip[3] = modl[0] * proj[3] + modl[1] * proj[7] + modl[2] * proj[11] + modl[3] * proj[15];

		clip[4] = modl[4] * proj[0] + modl[5] * proj[4] + modl[6] * proj[8] + modl[7] * proj[12];
		clip[5] = modl[4] * proj[1] + modl[5] * proj[5] + modl[6] * proj[9] + modl[7] * proj[13];
		clip[6] = modl[4] * proj[2] + modl[5] * proj[6] + modl[6] * proj[10] + modl[7] * proj[14];
		clip[7] = modl[4] * proj[3] + modl[5] * proj[7] + modl[6] * proj[11] + modl[7] * proj[15];

		clip[8] = modl[8] * proj[0] + modl[9] * proj[4] + modl[10] * proj[8] + modl[11] * proj[12];
		clip[9] = modl[8] * proj[1] + modl[9] * proj[5] + modl[10] * proj[9] + modl[11] * proj[13];
		clip[10] = modl[8] * proj[2] + modl[9] * proj[6] + modl[10] * proj[10] + modl[11] * proj[14];
		clip[11] = modl[8] * proj[3] + modl[9] * proj[7] + modl[10] * proj[11] + modl[11] * proj[15];

		clip[12] = modl[12] * proj[0] + modl[13] * proj[4] + modl[14] * proj[8] + modl[15] * proj[12];
		clip[13] = modl[12] * proj[1] + modl[13] * proj[5] + modl[14] * proj[9] + modl[15] * proj[13];
		clip[14] = modl[12] * proj[2] + modl[13] * proj[6] + modl[14] * proj[10] + modl[15] * proj[14];
		clip[15] = modl[12] * proj[3] + modl[13] * proj[7] + modl[14] * proj[11] + modl[15] * proj[15];


		//RIGHT 
		m_planes[RIGHT].x = (clip[3] - clip[0]);
		m_planes[RIGHT].y = (clip[7] - clip[4]);
		m_planes[RIGHT].z = (clip[11u] - clip[8]);
		m_planes[RIGHT].w = (clip[15u] - clip[12u]);
		NormalizePlane(m_planes[RIGHT]);

		//LEFT  
		m_planes[LEFT].x = (clip[3] + clip[0]);
		m_planes[LEFT].y = (clip[7] + clip[4]);
		m_planes[LEFT].z = (clip[11u] + clip[8]);
		m_planes[LEFT].w = (clip[15u] + clip[12u]);
		NormalizePlane(m_planes[LEFT]);

		//BOTTOM  
		m_planes[BOTTOM].x = (clip[3] + clip[1]);
		m_planes[BOTTOM].y = (clip[7] + clip[5]);
		m_planes[BOTTOM].z = (clip[11u] + clip[9]);
		m_planes[BOTTOM].w = (clip[15u] + clip[13u]);
		NormalizePlane(m_planes[BOTTOM]);

		//TOP  
		m_planes[TOP].x = (clip[3] - clip[1]);
		m_planes[TOP].y = (clip[7] - clip[5]);
		m_planes[TOP].z = (clip[11u] - clip[9]);
		m_planes[TOP].w = (clip[15u] - clip[13u]);
		NormalizePlane(m_planes[TOP]);

		//FAR  
		m_planes[BACK].x = (clip[3] - clip[2]);
		m_planes[BACK].y = (clip[7] - clip[6]);
		m_planes[BACK].z = (clip[11u] - clip[10u]);
		m_planes[BACK].w = (clip[15u] - clip[14u]);
		NormalizePlane(m_planes[BACK]);

		//NEAR  
		m_planes[FRONT].x = (clip[3] + clip[2]);
		m_planes[FRONT].y = (clip[7] + clip[6]);
		m_planes[FRONT].z = (clip[11u] + clip[10u]);
		m_planes[FRONT].w = (clip[15u] + clip[14u]);
		NormalizePlane(m_planes[FRONT]);
	}
};


class slCamera : public slUserData
{
public:
	slCamera();
	virtual ~slCamera();

	/*enum class Direction : uint32_t
	{
		North,
		NorthEast,
		East,
		SouthEast,
		South,
		SouthWest,
		West,
		NorthWest
	};*/

	slMat4 m_projectionMatrix;

	slMat4 m_viewMatrix;
	slMat4 m_viewMatrixInvert;

	slMat4 m_viewProjectionMatrix;
	slMat4 m_viewProjectionInvertMatrix;

	float m_near = 0.01f;
	float m_far = 2500.f;
	float m_fov = 0.683264f;
	float m_aspect = 1.3333333f;

	slCameraFrustum m_frust;

	slVec3 m_position;
	slVec3 m_target;
	slVec3 m_up;

	void Update();
};

#endif
