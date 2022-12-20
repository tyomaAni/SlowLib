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
#ifndef __SL_SLOWLIBBASEMATH_H__
#define __SL_SLOWLIBBASEMATH_H__

#include <cmath>
#include <limits>

#define slInfinity std::numeric_limits<real_t>::infinity()
#define slEpsilon std::numeric_limits<real_t>::epsilon()

const float PIf = static_cast<float>(3.14159265358979323846);
const float PIfHalf = static_cast<float>(3.14159265358979323846 * 0.5);
const float PIfPI = 6.2831853f;
const double PI = 3.14159265358979323846;
const double PIHalf = 3.14159265358979323846 * 0.5;
const double PIPI = 6.283185307179586476925286766559;

#include "slowlib.base/math/slVector.h"
#include "slowlib.base/math/slMatrix.h"
#include "slowlib.base/math/slQuaternion.h"

const slVec4 slVec4fFltMax = slVec4(DBL_MAX);
const slVec4 slVec4fFltMaxNeg = slVec4(-DBL_MAX);

class slMath
{
public:
	static float DegToRad(float degrees);
	static double DegToRad(double degrees);
	static float RadToDeg(float radians);
	static double RadToDeg(double radians);

	static float abs(float);
	static double abs(double);
	static int32_t abs(int32_t);

	// Expect value form -1 to +1
	static float acos(float);
	static double acos(double);

	// Expect value form -1 to +1
	static float asin(float);
	static double asin(double);

	// Expect value form -2 to +2
	static float atan(float);
	static double atan(double);

	// Expect value form -1 to +1
	static float atan2(float y, float x);
	static double atan2(double y, double x);
	
	static float clamp(float, float mn, float mx);
	static double clamp(double, double mn, double mx);

	// Expect value from -PI to +PI
	// It will do this
	// if (v > 3.141f) v = 3.141f;
	// if (v < -3.141f) v = -3.141f;
	static float cos(float);
	static double cos(double);
	static float sin(float);
	static double sin(double);
	static float tan(float);
	static double tan(double);

	static void cross(const slVec3& v1, const slVec3& v2, slVec3& r);
	static void cross(const slVec3f& v1, const slVec3f& v2, slVec3f& r);
	static void cross(const slVec4& v1, const slVec4& v2, slVec4& r);
	static void cross(const slVec4f& v1, const slVec4f& v2, slVec4f& r);
	static void cross(const slVec3& v1, const slVec4& v2, slVec3& r);
	static void cross(const slVec3f& v1, const slVec4f& v2, slVec3f& r);
	
	static real_t distance(const slVec3& v1, const slVec3& v2);
	static real_t distance(const slVec4& v1, const slVec4& v2);
	static real_t distance(const slVec3& v1, const slVec4& v2);
	static float distance(const slVec3f& v1, const slVec3f& v2);
	static float distance(const slVec4f& v1, const slVec4f& v2);
	static float distance(const slVec3f& v1, const slVec4f& v2);

	static real_t dot(const slVec3& v1, const slVec3& v2);
	static real_t dot(const slVec4& v1, const slVec4& v2);
	static real_t dot(const slVec3& v1, const slVec4& v2);
	static float dot(const slVec3f& v1, const slVec3f& v2);
	static float dot(const slVec4f& v1, const slVec4f& v2);
	static float dot(const slVec3f& v1, const slVec4f& v2);
	static float dot(const slQuaternion& v1, const slQuaternion& v2);

	static real_t length(const slVec3& v);
	static real_t length(const slVec4& v);
	static float length(const slVec3f& v);
	static float length(const slVec4f& v);
	static float length(const slQuaternion& v);

	// wikipedia:
	// Imprecise method, which does not guarantee v = v1 when t = 1, due to floating-point arithmetic error.
	// This method is monotonic. This form may be used when the hardware has a native fused multiply-add instruction.
	// 
	//  ... it has only 1 multiplication. lerp2 has 2 multiplications.
	static float lerp1(float x, float y, float t);
	static double lerp1(double x, double y, double t);
	static void lerp1(const slVec3& x, const slVec3& y, real_t t, slVec3& r);
	static void lerp1(const slVec3& x, const slVec4& y, real_t t, slVec3& r);
	static void lerp1(const slVec4& x, const slVec4& y, real_t t, slVec4& r);
	static void lerp1(const slVec3f& x, const slVec3f& y, float t, slVec3f& r);
	static void lerp1(const slVec3f& x, const slVec4f& y, float t, slVec3f& r);
	static void lerp1(const slVec4f& x, const slVec4f& y, float t, slVec4f& r);
	static void lerp1(const slQuaternion& x, const slQuaternion& y, float t, slQuaternion& r);

	// wikipedia:
	// Precise method, which guarantees v = v1 when t = 1. This method is monotonic only when v0 * v1 < 0.
	// Lerping between same values might not produce the same value
	static float lerp2(float x, float y, float t);
	static double lerp2(double x, double y, double t);
	static void lerp2(const slVec3& x, const slVec3& y, real_t t, slVec3& r);
	static void lerp2(const slVec3& x, const slVec4& y, real_t t, slVec3& r);
	static void lerp2(const slVec4& x, const slVec4& y, real_t t, slVec4& r);
	static void lerp2(const slVec3f& x, const slVec3f& y, float t, slVec3f& r);
	static void lerp2(const slVec3f& x, const slVec4f& y, float t, slVec3f& r);
	static void lerp2(const slVec4f& x, const slVec4f& y, float t, slVec4f& r);
	static void lerp2(const slQuaternion& x, const slQuaternion& y, float t, slQuaternion& r);
	
	static void normalize(slVec3& v);
	static void normalize(slVec3f& v);
	static void normalize(slVec4& v);
	static void normalize(slVec4f& v);
	static void normalize(slQuaternion& v);

	static void set_rotation(slQuaternion&, float x, float y, float z);
	static void set_rotation(slQuaternion&, const slVec3& axis, const real_t& _angle);
	static void set_rotation(slQuaternion&, const slVec3f& axis, const float& _angle);
	static void set_rotation(slQuaternion&, const slVec4& axis, const real_t& _angle);
	static void set_rotation(slQuaternion&, const slVec4f& axis, const float& _angle);
	static void set_rotation(slMat4&, const slQuaternion&);
	// from irrlicht?
	static void slerp(slQuaternion& q1, slQuaternion& q2, float time, float threshold, slQuaternion& r);

	static void mul(const slMat4&, const slMat4&, slMat4&);
	static void mul(const slMat4&, const slVec3&, slVec3&);
	static void mul(const slMat4&, const slVec4&, slVec4&);
	static void mul(const slQuaternion&, const slQuaternion&, slQuaternion&);
	
	static void transpose(slMat4&);

	static void invert(slMat4&);

	static void perspectiveLH(slMat4&, real_t FOV, real_t aspect, real_t Near, real_t Far);
	static void lookAtLH(slMat4&, const slVec4& eye, const slVec4& center, const slVec4& up);

	static float coordToUV(float value, float textureSz);
};

#endif
