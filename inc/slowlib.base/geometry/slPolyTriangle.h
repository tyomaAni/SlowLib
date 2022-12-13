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
#ifndef __SL_SLOWLIBBASEPOLYTRI_H__
#define __SL_SLOWLIBBASEPOLYTRI_H__

class slPolyTriangle
{
public:
	slPolyTriangle();
	slPolyTriangle(const slVec4f& _v1, const slVec4f& _v2, const slVec4f& _v3);
	slPolyTriangle(const slVec3f& _v1, const slVec3f& _v2, const slVec3f& _v3);

	slVec4f v1;
	slVec4f v2;
	slVec4f v3;
	//v4f faceNormal;
	slVec4f normal1;
	slVec4f normal2;
	slVec4f normal3;
	slVec4f e1;
	slVec4f e2;
	slVec4f t1;
	slVec4f t2;
	slVec4f t3;

	void update();
	void center(slVec4f& out);
	bool rayTest_MT(const slRay& ray, bool withBackFace, real_t& T, real_t& U, real_t& V, real_t& W);
	bool rayTest_Watertight(const slRay& ray, bool withBackFace, real_t& T, real_t& U, real_t& V, real_t& W);
};

#endif
