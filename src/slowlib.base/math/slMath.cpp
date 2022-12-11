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

#include "math_private.h"

static float g_acosf[] = {
#include "acosf.inl"
};
static double g_acos[] = {
#include "acos.inl"
};
static float g_asinf[] = {
#include "asinf.inl"
};
static double g_asin[] = {
#include "asin.inl"
};
static float g_atanf[] = {
#include "atanf.inl"
};
static double g_atan[] = {
#include "atan.inl"
};
static double g_atan2[] = {
#include "atan2.inl"
};
static float g_atan2f[] = {
#include "atan2f.inl"
};
static double g_cos[] = {
#include "cos.inl"
};
static float g_cosf[] = {
#include "cosf.inl"
};
static double g_sin[] = {
#include "sin.inl"
};
static float g_sinf[] = {
#include "sinf.inl"
};
static double g_tan[] = {
#include "tan.inl"
};
static float g_tanf[] = {
#include "tanf.inl"
};

float slMath::DegToRad(float degrees)
{
	return degrees * (PIf / 180.f);
}

double slMath::DegToRad(double degrees)
{
	return degrees * (PI / 180.0);
}

float slMath::RadToDeg(float radians)
{
	return radians * (180.f / PIf);
}

double slMath::RadToDeg(double radians)
{
	return radians * (180.0 / PI);
}

float slMath::abs(float x)
{
	uint32_t ix;
	GET_FLOAT_WORD(ix, x);
	SET_FLOAT_WORD(x, ix & 0x7fffffff);
	return x;
}

double slMath::abs(double x)
{
	uint32_t high;
	GET_HIGH_WORD(high, x);
	SET_HIGH_WORD(x, high & 0x7fffffff);
	return x;
}

int32_t slMath::abs(int32_t v)
{
	return(v < 0 ? -v : v);
}

float slMath::acos(float v)
{
	if (v >= -1.f && v <= 1.f)
	{
		uint32_t ind = (uint32_t)roundf((v + 1.f)*1000.f);
		SL_ASSERT_ST(ind < 2001);
		return g_acosf[ind];
	}
	return 0;
}

double slMath::acos(double v)
{
	if (v >= -1.0 && v <= 1.0)
	{
		uint32_t ind = (uint32_t)round((v + 1.0) * 1000.0);
		SL_ASSERT_ST(ind < 2001);
		return g_acos[ind];
	}
	return 0;
}

float slMath::asin(float v)
{
	if (v >= -1.f && v <= 1.f)
	{
		uint32_t ind = (uint32_t)roundf((v + 1.f) * 1000.f);
		SL_ASSERT_ST(ind < 2001);
		return g_asinf[ind];
	}
	return 0;
}

double slMath::asin(double v)
{
	if (v >= -1.0 && v <= 1.0)
	{
		uint32_t ind = (uint32_t)round((v + 1.0) * 1000.0);
		SL_ASSERT_ST(ind < 2001);
		return g_asin[ind];
	}
	return 0;
}

float slMath::atan(float v)
{
	if (v >= -2.f && v <= 2.f)
	{
		uint32_t ind = (uint32_t)roundf((v + 2.f) * 1000.f);
		SL_ASSERT_ST(ind < 4001);
		return g_atanf[ind];
	}
	return 0;
}

double slMath::atan(double v)
{
	if (v >= -2.f && v <= 2.f)
	{
		uint32_t ind = (uint32_t)round((v + 2.0) * 1000.0);
		SL_ASSERT_ST(ind < 4001);
		return g_atan[ind];
	}
	return 0;
}

float slMath::atan2(float y, float x)
{
	if (y >= -1.0f && y <= 1.0f)
	{
		if (x >= -1.0f && x <= 1.0f)
		{
			uint32_t ix = (uint32_t)roundf((x + 1.0f) * 100.0f);
			uint32_t iy = (uint32_t)roundf((y + 1.0f) * 100.0f);
			uint32_t ind = (uint32_t)(((float)iy * 201.f) + (float)ix);
			SL_ASSERT_ST(ind < 40401);

			if (ind > 40400)
			{
				slLog::PrintWarning("ind > 40400\n");
				return 0.f;
			}

			return g_atan2f[ind];
		}
	}
	return 0.f;
}

double slMath::atan2(double y, double x)
{
	if (y >= -1.0 && y <= 1.0)
	{
		if (x >= -1.0 && x <= 1.0)
		{
			uint32_t ix = (uint32_t)round((x + 1.0) * 100.0);
			uint32_t iy = (uint32_t)round((y + 1.0) * 100.0);
			uint32_t ind = (uint32_t)(((float)iy * 201.f) + (float)ix);
			SL_ASSERT_ST(ind < 40401);

			if (ind > 40400)
			{
				slLog::PrintWarning("ind > 40400\n");
				return 0;
			}

			return g_atan2[ind];
		}
	}
	return 0.f;
}

float slMath::clamp(float v, float mn, float mx)
{
	if (v < mn)
		return mn;
	else if (v > mx)
		return mx;
	return v;
}

double slMath::clamp(double v, double mn, double mx)
{
	if (v < mn)
		return mn;
	else if (v > mx)
		return mx;
	return v;
}

float slMath::cos(float v)
{
	if (v < -3.141f) v = -3.141f;
	if (v < 0.f) v = slMath::abs(v);
	if (v > 3.141f) v = 3.141f;

	uint32_t ind = (uint32_t)roundf(v * 1000.0f);

	if(ind < 3143)
		return g_cosf[ind];
	return 0;
}

double slMath::cos(double v)
{
	if (v < -3.141) v = -3.141;
	if (v < 0.) v = slMath::abs(v);
	if (v > 3.141) v = 3.141;

	uint32_t ind = (uint32_t)round(v * 1000.0);

	if (ind < 3143)
		return g_cos[ind];
	return 0;
}

float slMath::sin(float v)
{
	bool neg = false;
	if (v < -3.141f) v = -3.141f;
	if (v < 0.f)
	{
		neg = true;
		v = slMath::abs(v);
	}
	if (v > 3.141f) v = 3.141f;

	uint32_t ind = (uint32_t)roundf(v * 1000.0f);

	if (ind < 3143)
		return neg ? -g_sinf[ind] : g_sinf[ind];
	return 0;
}

double slMath::sin(double v)
{
	bool neg = false;
	if (v < -3.141) v = -3.141;
	if (v < 0.)
	{
		neg = true;
		v = slMath::abs(v);
	}
	if (v > 3.141) v = 3.141;

	uint32_t ind = (uint32_t)round(v * 1000.0);

	if (ind < 3143)
		return neg ? -g_sin[ind] : g_sin[ind];
	return 0;
}

float slMath::tan(float v)
{
	bool neg = false;
	if (v < -3.141f) v = -3.141f;
	if (v < 0.f)
	{
		neg = true;
		v = slMath::abs(v);
	}
	if (v > 3.141f) v = 3.141f;

	uint32_t ind = (uint32_t)roundf(v * 1000.0f);

	if (ind < 3143)
		return neg ? -g_tanf[ind] : g_tanf[ind];
	return 0;
}

double slMath::tan(double v)
{
	bool neg = false;
	if (v < -3.141) v = -3.141;
	if (v < 0.)
	{
		neg = true;
		v = slMath::abs(v);
	}
	if (v > 3.141) v = 3.141;

	uint32_t ind = (uint32_t)round(v * 1000.0);

	if (ind < 3143)
		return neg ? -g_tan[ind] : g_tan[ind];
	return 0;
}

void slMath::cross(const slVec3& v1, const slVec3& v2, slVec3& r)
{
	r.x = (v1.y * v2.z) - (v1.z * v2.y);
	r.y = (v1.z * v2.x) - (v1.x * v2.z);
	r.z = (v1.x * v2.y) - (v1.y * v2.x);
}

void slMath::cross(const slVec3f& v1, const slVec3f& v2, slVec3f& r)
{
	r.x = (v1.y * v2.z) - (v1.z * v2.y);
	r.y = (v1.z * v2.x) - (v1.x * v2.z);
	r.z = (v1.x * v2.y) - (v1.y * v2.x);
}

void slMath::cross(const slVec4& v1, const slVec4& v2, slVec4& r)
{
	r.x = (v1.y * v2.z) - (v1.z * v2.y);
	r.y = (v1.z * v2.x) - (v1.x * v2.z);
	r.z = (v1.x * v2.y) - (v1.y * v2.x);
}

void slMath::cross(const slVec4f& v1, const slVec4f& v2, slVec4f& r)
{
	r.x = (v1.y * v2.z) - (v1.z * v2.y);
	r.y = (v1.z * v2.x) - (v1.x * v2.z);
	r.z = (v1.x * v2.y) - (v1.y * v2.x);
}

void slMath::cross(const slVec3& v1, const slVec4& v2, slVec3& r)
{
	r.x = (v1.y * v2.z) - (v1.z * v2.y);
	r.y = (v1.z * v2.x) - (v1.x * v2.z);
	r.z = (v1.x * v2.y) - (v1.y * v2.x);
}

void slMath::cross(const slVec3f& v1, const slVec4f& v2, slVec3f& r)
{
	r.x = (v1.y * v2.z) - (v1.z * v2.y);
	r.y = (v1.z * v2.x) - (v1.x * v2.z);
	r.z = (v1.x * v2.y) - (v1.y * v2.x);
}

real_t slMath::distance(const slVec3& v1, const slVec3& v2)
{
	real_t xx = v2.x - v1.x;
	real_t yy = v2.y - v1.y;
	real_t zz = v2.z - v1.z;

	return sqrt((xx * xx) + (yy * yy) + (zz * zz));
}

real_t slMath::distance(const slVec4& v1, const slVec4& v2)
{
	real_t xx = v2.x - v1.x;
	real_t yy = v2.y - v1.y;
	real_t zz = v2.z - v1.z;

	return sqrt((xx * xx) + (yy * yy) + (zz * zz));
}

real_t slMath::distance(const slVec3& v1, const slVec4& v2)
{
	real_t xx = v2.x - v1.x;
	real_t yy = v2.y - v1.y;
	real_t zz = v2.z - v1.z;

	return sqrt((xx * xx) + (yy * yy) + (zz * zz));
}

float slMath::distance(const slVec3f& v1, const slVec3f& v2)
{
	float xx = v2.x - v1.x;
	float yy = v2.y - v1.y;
	float zz = v2.z - v1.z;

	return sqrtf((xx * xx) + (yy * yy) + (zz * zz));
}

float slMath::distance(const slVec4f& v1, const slVec4f& v2)
{
	float xx = v2.x - v1.x;
	float yy = v2.y - v1.y;
	float zz = v2.z - v1.z;

	return sqrtf((xx * xx) + (yy * yy) + (zz * zz));
}

float slMath::distance(const slVec3f& v1, const slVec4f& v2)
{
	float xx = v2.x - v1.x;
	float yy = v2.y - v1.y;
	float zz = v2.z - v1.z;

	return sqrtf((xx * xx) + (yy * yy) + (zz * zz));
}

real_t slMath::dot(const slVec3& v1, const slVec3& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

real_t slMath::dot(const slVec4& v1, const slVec4& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
}

real_t slMath::dot(const slVec3& v1, const slVec4& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

float slMath::dot(const slVec3f& v1, const slVec3f& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

float slMath::dot(const slVec4f& v1, const slVec4f& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
}

float slMath::dot(const slVec3f& v1, const slVec4f& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

float slMath::dot(const slQuaternion& v1, const slQuaternion& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
}

real_t slMath::length(const slVec3& v)
{
	return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

real_t slMath::length(const slVec4& v)
{
	return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w));
}

float slMath::length(const slVec3f& v)
{
	return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

float slMath::length(const slVec4f& v)
{
	return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w));
}

float slMath::length(const slQuaternion& v)
{
	return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w));
}

float slMath::lerp1(float x, float y, float t)
{
	return x + t * (y - x);
}

double slMath::lerp1(double x, double y, double t)
{
	return x + t * (y - x);
}

void slMath::lerp1(const slVec3& x, const slVec3& y, double t, slVec3& r)
{
	r.x = x.x + t * (y.x - x.x);
	r.y = x.y + t * (y.y - x.y);
	r.z = x.z + t * (y.z - x.z);
}

void slMath::lerp1(const slVec3& x, const slVec4& y, double t, slVec3& r)
{
	r.x = x.x + t * (y.x - x.x);
	r.y = x.y + t * (y.y - x.y);
	r.z = x.z + t * (y.z - x.z);
}

void slMath::lerp1(const slVec4& x, const slVec4& y, double t, slVec4& r)
{
	r.x = x.x + t * (y.x - x.x);
	r.y = x.y + t * (y.y - x.y);
	r.z = x.z + t * (y.z - x.z);
	r.w = x.w + t * (y.w - x.w);
}

void slMath::lerp1(const slVec3f& x, const slVec3f& y, float t, slVec3f& r)
{
	r.x = x.x + t * (y.x - x.x);
	r.y = x.y + t * (y.y - x.y);
	r.z = x.z + t * (y.z - x.z);
}

void slMath::lerp1(const slVec3f& x, const slVec4f& y, float t, slVec3f& r)
{
	r.x = x.x + t * (y.x - x.x);
	r.y = x.y + t * (y.y - x.y);
	r.z = x.z + t * (y.z - x.z);
}

void slMath::lerp1(const slVec4f& x, const slVec4f& y, float t, slVec4f& r)
{
	r.x = x.x + t * (y.x - x.x);
	r.y = x.y + t * (y.y - x.y);
	r.z = x.z + t * (y.z - x.z);
	r.w = x.w + t * (y.w - x.w);
}

void slMath::lerp1(const slQuaternion& x, const slQuaternion& y, float t, slQuaternion& r)
{
	r.x = x.x + t * (y.x - x.x);
	r.y = x.y + t * (y.y - x.y);
	r.z = x.z + t * (y.z - x.z);
	r.w = x.w + t * (y.w - x.w);
}

float slMath::lerp2(float x, float y, float t)
{
	return (1.f - t) * x + t * y;
}

double slMath::lerp2(double x, double y, double t)
{
	return (1.0 - t) * x + t * y;
}

void slMath::lerp2(const slVec3& x, const slVec3& y, double t, slVec3& r)
{
	r.x = (1.0 - t) * x.x + t * y.x;
	r.y = (1.0 - t) * x.y + t * y.y;
	r.z = (1.0 - t) * x.z + t * y.z;
}

void slMath::lerp2(const slVec3& x, const slVec4& y, double t, slVec3& r)
{
	r.x = (1.0 - t) * x.x + t * y.x;
	r.y = (1.0 - t) * x.y + t * y.y;
	r.z = (1.0 - t) * x.z + t * y.z;
}

void slMath::lerp2(const slVec4& x, const slVec4& y, double t, slVec4& r)
{
	r.x = (1.0 - t) * x.x + t * y.x;
	r.y = (1.0 - t) * x.y + t * y.y;
	r.z = (1.0 - t) * x.z + t * y.z;
	r.w = (1.0 - t) * x.w + t * y.w;
}

void slMath::lerp2(const slVec3f& x, const slVec3f& y, float t, slVec3f& r)
{
	r.x = (1.0f - t) * x.x + t * y.x;
	r.y = (1.0f - t) * x.y + t * y.y;
	r.z = (1.0f - t) * x.z + t * y.z;
}

void slMath::lerp2(const slVec3f& x, const slVec4f& y, float t, slVec3f& r)
{
	r.x = (1.0f - t) * x.x + t * y.x;
	r.y = (1.0f - t) * x.y + t * y.y;
	r.z = (1.0f - t) * x.z + t * y.z;
}

void slMath::lerp2(const slVec4f& x, const slVec4f& y, float t, slVec4f& r)
{
	r.x = (1.0f - t) * x.x + t * y.x;
	r.y = (1.0f - t) * x.y + t * y.y;
	r.z = (1.0f - t) * x.z + t * y.z;
	r.w = (1.0f - t) * x.w + t * y.w;
}

void slMath::lerp2(const slQuaternion& x, const slQuaternion& y, float t, slQuaternion& r)
{
	r.x = (1.0f - t) * x.x + t * y.x;
	r.y = (1.0f - t) * x.y + t * y.y;
	r.z = (1.0f - t) * x.z + t * y.z;
	r.w = (1.0f - t) * x.w + t * y.w;
}

void slMath::normalize(slVec3& v)
{
	double len = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	if (len > 0)
		len = 1.0f / len;
	v.x *= len;
	v.y *= len;
	v.z *= len;
}

void slMath::normalize(slVec3f& v)
{
	float len = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	if (len > 0)
		len = 1.0f / len;
	v.x *= len;
	v.y *= len;
	v.z *= len;
}

void slMath::normalize(slVec4& v)
{
	double len = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w));
	if (len > 0)
		len = 1.0f / len;
	v.x *= len;
	v.y *= len;
	v.z *= len;
	v.w *= len;
}

void slMath::normalize(slVec4f& v)
{
	float len = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w));
	if (len > 0)
		len = 1.0f / len;
	v.x *= len;
	v.y *= len;
	v.z *= len;
	v.w *= len;
}

void slMath::normalize(slQuaternion& v)
{
	float len = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w));
	if (len > 0)
		len = 1.0f / len;
	v.x *= len;
	v.y *= len;
	v.z *= len;
	v.w *= len;
}

void slMath::set_rotation(slQuaternion& q, float x, float y, float z)
{
	x *= 0.5f;
	y *= 0.5f;
	z *= 0.5f;
	float c1 = ::cosf(x);
	float c2 = ::cosf(y);
	float c3 = ::cosf(z);
	float s1 = ::sinf(x);
	float s2 = ::sinf(y);
	float s3 = ::sinf(z);
	q.w = (c1 * c2 * c3) + (s1 * s2 * s3);
	q.x = (s1 * c2 * c3) - (c1 * s2 * s3);
	q.y = (c1 * s2 * c3) + (s1 * c2 * s3);
	q.z = (c1 * c2 * s3) - (s1 * s2 * c3);
}

void slMath::set_rotation(slQuaternion& q, const slVec3& axis, const real_t& _angle)
{
	real_t d = (real_t)length(axis);
	real_t s = std::sin(_angle * 0.5f) / d;
	q.x = (float)(axis.x * s);
	q.y = (float)(axis.y * s);
	q.z = (float)(axis.z * s);
	q.w = (float)std::cos(_angle * 0.5);
}

void slMath::set_rotation(slQuaternion& q, const slVec3f& axis, const float& _angle)
{
	float d = length(axis);
	float s = std::sin(_angle * 0.5f) / d;
	q.x = axis.x * s;
	q.y = axis.y * s;
	q.z = axis.z * s;
	q.w = std::cos(_angle * 0.5f);
}

void slMath::set_rotation(slQuaternion& q, const slVec4& axis, const real_t& _angle)
{
	real_t d = (real_t)length(axis);
	real_t s = std::sin(_angle * 0.5f) / d;
	q.x = (float)(axis.x * s);
	q.y = (float)(axis.y * s);
	q.z = (float)(axis.z * s);
	q.w = (float)std::cos(_angle * 0.5f);
}

void slMath::set_rotation(slQuaternion& q, const slVec4f& axis, const float& _angle)
{
	float d = length(axis);
	float s = std::sin(_angle * 0.5f) / d;
	q.x = axis.x * s;
	q.y = axis.y * s;
	q.z = axis.z * s;
	q.w = std::cos(_angle * 0.5f);
}

void slMath::set_rotation(slMat4& m, const slQuaternion& q)
{
	real_t d = length(q);
	real_t s = 2.0f / d;
	real_t xs = q.x * s, ys = q.y * s, zs = q.z * s;
	real_t wx = q.w * xs, wy = q.w * ys, wz = q.w * zs;
	real_t xx = q.x * xs, xy = q.x * ys, xz = q.x * zs;
	real_t yy = q.y * ys, yz = q.y * zs, zz = q.z * zs;
	m.set(
		1.0f - (yy + zz), xy - wz, xz + wy,
		xy + wz, 1.0f - (xx + zz), yz - wx,
		xz - wy, yz + wx, 1.0f - (xx + yy));
}

// irrlicht?
void slMath::slerp(slQuaternion& q1, slQuaternion& q2, float time, float threshold, slQuaternion& r)
{
	float angle = dot(q1, q2);
	// make sure we use the short rotation
	if (angle < 0.0f)
	{
		q1 = q1 * -1.0f;
		angle *= -1.0f;
	}

	if (angle <= (1 - threshold)) // spherical interpolation
	{
		const float theta = ::acosf(angle);
		const float invsintheta = 1.f / (::sinf(theta));
		const float scale = ::sinf(theta * (1.0f - time)) * invsintheta;
		const float invscale = ::sinf(theta * time) * invsintheta;
		r = (q1 * scale) + (q2 * invscale);
	}
	else // linear interploation
		lerp1(q1, q2, time, r);
}

void slMath::mul(const slMat4& m1, const slMat4& m2, slMat4& r)
{
	r.m_data[0].x = m1.m_data[0].x * m2.m_data[0].x + m1.m_data[1].x * m2.m_data[0].y + m1.m_data[2].x * m2.m_data[0].z + m1.m_data[3].x * m2.m_data[0].w;
	r.m_data[0].y = m1.m_data[0].y * m2.m_data[0].x + m1.m_data[1].y * m2.m_data[0].y + m1.m_data[2].y * m2.m_data[0].z + m1.m_data[3].y * m2.m_data[0].w;
	r.m_data[0].z = m1.m_data[0].z * m2.m_data[0].x + m1.m_data[1].z * m2.m_data[0].y + m1.m_data[2].z * m2.m_data[0].z + m1.m_data[3].z * m2.m_data[0].w;
	r.m_data[0].w = m1.m_data[0].w * m2.m_data[0].x + m1.m_data[1].w * m2.m_data[0].y + m1.m_data[2].w * m2.m_data[0].z + m1.m_data[3].w * m2.m_data[0].w;

	r.m_data[1].x = m1.m_data[0].x * m2.m_data[1].x + m1.m_data[1].x * m2.m_data[1].y + m1.m_data[2].x * m2.m_data[1].z + m1.m_data[3].x * m2.m_data[1].w;
	r.m_data[1].y = m1.m_data[0].y * m2.m_data[1].x + m1.m_data[1].y * m2.m_data[1].y + m1.m_data[2].y * m2.m_data[1].z + m1.m_data[3].y * m2.m_data[1].w;
	r.m_data[1].z = m1.m_data[0].z * m2.m_data[1].x + m1.m_data[1].z * m2.m_data[1].y + m1.m_data[2].z * m2.m_data[1].z + m1.m_data[3].z * m2.m_data[1].w;
	r.m_data[1].w = m1.m_data[0].w * m2.m_data[1].x + m1.m_data[1].w * m2.m_data[1].y + m1.m_data[2].w * m2.m_data[1].z + m1.m_data[3].w * m2.m_data[1].w;

	r.m_data[2].x = m1.m_data[0].x * m2.m_data[2].x + m1.m_data[1].x * m2.m_data[2].y + m1.m_data[2].x * m2.m_data[2].z + m1.m_data[3].x * m2.m_data[2].w;
	r.m_data[2].y = m1.m_data[0].y * m2.m_data[2].x + m1.m_data[1].y * m2.m_data[2].y + m1.m_data[2].y * m2.m_data[2].z + m1.m_data[3].y * m2.m_data[2].w;
	r.m_data[2].z = m1.m_data[0].z * m2.m_data[2].x + m1.m_data[1].z * m2.m_data[2].y + m1.m_data[2].z * m2.m_data[2].z + m1.m_data[3].z * m2.m_data[2].w;
	r.m_data[2].w = m1.m_data[0].w * m2.m_data[2].x + m1.m_data[1].w * m2.m_data[2].y + m1.m_data[2].w * m2.m_data[2].z + m1.m_data[3].w * m2.m_data[2].w;

	r.m_data[3].x = m1.m_data[0].x * m2.m_data[3].x + m1.m_data[1].x * m2.m_data[3].y + m1.m_data[2].x * m2.m_data[3].z + m1.m_data[3].x * m2.m_data[3].w;
	r.m_data[3].y = m1.m_data[0].y * m2.m_data[3].x + m1.m_data[1].y * m2.m_data[3].y + m1.m_data[2].y * m2.m_data[3].z + m1.m_data[3].y * m2.m_data[3].w;
	r.m_data[3].z = m1.m_data[0].z * m2.m_data[3].x + m1.m_data[1].z * m2.m_data[3].y + m1.m_data[2].z * m2.m_data[3].z + m1.m_data[3].z * m2.m_data[3].w;
	r.m_data[3].w = m1.m_data[0].w * m2.m_data[3].x + m1.m_data[1].w * m2.m_data[3].y + m1.m_data[2].w * m2.m_data[3].z + m1.m_data[3].w * m2.m_data[3].w;
}

void slMath::mul(const slMat4& m, const slVec3& v, slVec3& r)
{
	r.x = v.x * m.m_data[0].x + v.y * m.m_data[1].x + v.z * m.m_data[2].x;
	r.y = v.x * m.m_data[0].y + v.y * m.m_data[1].y + v.z * m.m_data[2].y;
	r.z = v.x * m.m_data[0].z + v.y * m.m_data[1].z + v.z * m.m_data[2].z;
}

void slMath::mul(const slMat4& m, const slVec4& v, slVec4& r)
{
	r.x = v.x * m.m_data[0].x + v.y * m.m_data[1].x + v.z * m.m_data[2].x + v.w * m.m_data[3].x;
	r.y = v.x * m.m_data[0].y + v.y * m.m_data[1].y + v.z * m.m_data[2].y + v.w * m.m_data[3].y;
	r.z = v.x * m.m_data[0].z + v.y * m.m_data[1].z + v.z * m.m_data[2].z + v.w * m.m_data[3].z;
	r.w = v.x * m.m_data[0].w + v.y * m.m_data[1].w + v.z * m.m_data[2].w + v.w * m.m_data[3].w;
}

void slMath::mul(const slQuaternion& q1, const slQuaternion& q2, slQuaternion& r)
{
	r.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	r.x = q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z;
	r.x = q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x;
	r.x = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
}

void slMath::transpose(slMat4& m)
{
	slMat4 tmp;
	tmp[0u].x = m.m_data[0u].x;
	tmp[0u].y = m.m_data[1u].x;
	tmp[0u].z = m.m_data[2u].x;
	tmp[0u].w = m.m_data[3u].x;

	tmp[1u].x = m.m_data[0u].y;
	tmp[1u].y = m.m_data[1u].y;
	tmp[1u].z = m.m_data[2u].y;
	tmp[1u].w = m.m_data[3u].y;

	tmp[2u].x = m.m_data[0u].z;
	tmp[2u].y = m.m_data[1u].z;
	tmp[2u].z = m.m_data[2u].z;
	tmp[2u].w = m.m_data[3u].z;

	m.m_data[0u] = tmp[0u];
	m.m_data[1u] = tmp[1u];
	m.m_data[2u] = tmp[2u];
	m.m_data[3u] = tmp[3u];
}

//https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/matrix-inverse
void slMath::invert(slMat4& m)
{
	slMat4 mat;
	auto ptr = m.data();
	for (unsigned column = 0; column < 4; ++column)
	{
		// Swap row in case our pivot point is not working
		auto column_data = m.m_data[column].data();
		if (column_data[column] == 0)
		{
			unsigned big = column;
			for (unsigned row = 0; row < 4; ++row)
			{
				auto row_data = m.m_data[row].data();
				auto big_data = m.m_data[big].data();
				if (fabs(row_data[column]) > fabs(big_data[column]))
					big = row;
			}
			// Print this is a singular matrix, return identity ?
			if (big == column)
				fprintf(stderr, "Singular matrix\n");
			// Swap rows                               
			else for (unsigned j = 0; j < 4; ++j)
			{
				auto big_data = m.m_data[big].data();
				std::swap(column_data[j], big_data[j]);

				auto other_column_data = mat.m_data[column].data();
				auto other_big_data = mat.m_data[big].data();
				std::swap(other_column_data[j], other_big_data[j]);
			}
		}

		// Set each row in the column to 0  
		for (unsigned row = 0; row < 4; ++row)
		{
			if (row != column)
			{
				auto row_data = m.m_data[row].data();
				real_t coeff = row_data[column] / column_data[column];
				if (coeff != 0)
				{
					for (unsigned j = 0; j < 4; ++j)
					{
						row_data[j] -= coeff * column_data[j];

						auto other_row_data = mat.m_data[row].data();
						auto other_column_data = mat.m_data[column].data();
						other_row_data[j] -= coeff * other_column_data[j];
					}
					// Set the element to 0 for safety
					row_data[column] = 0;
				}
			}
		}
	}

	// Set each element of the diagonal to 1
	for (unsigned row = 0; row < 4; ++row)
	{
		for (unsigned column = 0; column < 4; ++column)
		{
			auto other_row_data = mat.m_data[row].data();
			auto row_data = m.m_data[row].data();
			other_row_data[column] /= row_data[row];
		}
	}
	m = mat;
}

void slMath::perspectiveLH(slMat4& m, real_t FOV, real_t aspect, real_t Near, real_t Far)
{
	real_t S = ::sin(0.5 * FOV);
	real_t C = ::cos(0.5 * FOV);
	real_t H = C / S;
	real_t W = H / aspect;
	m.m_data[0] = slVec4(W, 0., 0.,0.);
	m.m_data[1] = slVec4(0., H, 0.,0.);
	m.m_data[2] = slVec4(0., 0., Far / (Far - Near), 1.);
	m.m_data[3] = slVec4(0., 0., -m.m_data[2].z * Near, 0.);
}

void slMath::lookAtLH(slMat4& m, const slVec4& eye, const slVec4& center, const slVec4& up)
{
	slVec4 f(center - eye);
	normalize(f);

	slVec4 s;
	cross(f, up, s);
	normalize(s);

	slVec4 u;
	cross(s, f, u);

	m.m_data[0].x = s.x;
	m.m_data[1].x = s.y;
	m.m_data[2].x = s.z;
	m.m_data[0].y = u.x;
	m.m_data[1].y = u.y;
	m.m_data[2].y = u.z;
	m.m_data[0].z = f.x;
	m.m_data[1].z = f.y;
	m.m_data[2].z = f.z;
	m.m_data[3].x = -dot(s, eye);
	m.m_data[3].y = -dot(u, eye);
	m.m_data[3].z = -dot(f, eye);
}

