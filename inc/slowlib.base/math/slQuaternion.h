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
#ifndef __SL_SLOWLIBBASEMTHQUAT_H__
#define __SL_SLOWLIBBASEMTHQUAT_H__

class SL_API slQuaternion
{
public:
	slQuaternion();
	slQuaternion(const slQuaternion&);
	slQuaternion(float x, float y, float z, float w);

	void identity();
	float length();

	slQuaternion& operator=(const slQuaternion& o);
	slQuaternion operator*(const slQuaternion& q)const;
	slQuaternion operator*(float s) const;
	void operator*=(const slQuaternion& q);
	bool operator!=(const slQuaternion& q)const;
	bool operator==(const slQuaternion& q)const;
	slQuaternion operator+(const slQuaternion& o) const;
	slQuaternion operator-(const slQuaternion& o) const;
	slQuaternion operator-();
	float operator[](uint32_t index) const;
	float& operator[](uint32_t index);

	float x, y, z, w;
	float* data() { return &x; }
};

#endif
