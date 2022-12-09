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

slQuaternion::slQuaternion() 
{
	identity(); 
}

slQuaternion::slQuaternion(const slQuaternion& o) 
	:
	x(o.x),
	y(o.y),
	z(o.z),
	w(o.w)
{}

slQuaternion::slQuaternion(float x, float y, float z, float w) 
	:
	x(x),
	y(y),
	z(z),
	w(w)
{}

void slQuaternion::identity() 
{
	x = y = z = 0.f; 
	w = 1.f;
}

float slQuaternion::length()
{
	return (x * x) + (y * y) + (z * z) + (w * w);
}

slQuaternion& slQuaternion::operator=(const slQuaternion& o)
{
	x = o.x;
	y = o.y;
	z = o.z;
	w = o.w;
	return *this;
}

slQuaternion slQuaternion::operator*(const slQuaternion& q)const
{
	return slQuaternion(
		w * q.x + x * q.w + y * q.z - z * q.y,
		w * q.y + y * q.w + z * q.x - x * q.z,
		w * q.z + z * q.w + x * q.y - y * q.x,
		w * q.w - x * q.x - y * q.y - z * q.z);
}

slQuaternion slQuaternion::operator*(float s) const
{
	return slQuaternion(s * x, s * y, s * z, s * w);
}

void slQuaternion::operator*=(const slQuaternion& q)
{
	x = w * q.x + x * q.w + y * q.z - z * q.y;
	y = w * q.y + y * q.w + z * q.x - x * q.z;
	z = w * q.z + z * q.w + x * q.y - y * q.x;
	w = w * q.w - x * q.x - y * q.y - z * q.z;
}

bool slQuaternion::operator!=(const slQuaternion& q)const 
{
	if (x != q.x) return true;
	if (y != q.y) return true;
	if (z != q.z) return true;
	if (w != q.w) return true;
	return false;
}

bool slQuaternion::operator==(const slQuaternion& q)const
{
	if (x != q.x) return false;
	if (y != q.y) return false;
	if (z != q.z) return false;
	if (w != q.w) return false;
	return true;
}

slQuaternion slQuaternion::operator+(const slQuaternion& o) const
{
	return slQuaternion(
		x + o.x,
		y + o.y,
		z + o.z,
		w + o.w);
}

slQuaternion slQuaternion::operator-(const slQuaternion& o) const 
{
	return slQuaternion(
		x - o.x,
		y - o.y,
		z - o.z,
		w - o.w);
}

slQuaternion slQuaternion::operator-()
{
	x = -x;
	y = -y;
	z = -z;
	return slQuaternion(x, y, z, w);
}

float slQuaternion::operator[](uint32_t index) const
{
	SL_ASSERT_ST((index >= 0) && (index < 4));
	return (&x)[index];
}

float& slQuaternion::operator[](uint32_t index)
{
	SL_ASSERT_ST((index >= 0) && (index < 4));
	return (&x)[index];
}
