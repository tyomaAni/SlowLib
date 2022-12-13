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
#ifndef __SL_SLOWLIBBASERAY_H__
#define __SL_SLOWLIBBASERAY_H__

#include "slowlib.base/math/slMath.h"

class slRay
{
public:
	slRay();
	~slRay();

	slVec4 m_origin;
	slVec4 m_end;
	slVec4 m_direction;
	slVec4 m_invDir;

	int32_t m_kz = 0;
	int32_t m_kx = 0;
	int32_t m_ky = 0;

	real_t m_Sx = 0;
	real_t m_Sy = 0;
	real_t m_Sz = 0;

	void CreateFrom2DCoords(const slPoint& coord, const slRect& rc, const slPoint& rc_sz, 
		const slMat4& VPinv, const slVec3f& depthRange);
	

	real_t DistanceToLine(const slVec4& lineP0, const slVec4& lineP1);
	void Update();

	void GetIntersectionPoint(real_t t, slVec4& ip);
	bool PlaneIntersection(const slVec4& planePoint, const slVec4& planeNormal, real_t& T);
};

#endif
