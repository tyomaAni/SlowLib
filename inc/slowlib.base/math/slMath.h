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

const float PIf = static_cast<float>(3.14159265358979323846);
const float PIfHalf = static_cast<float>(3.14159265358979323846 * 0.5);
const float PIfPI = 6.2831853f;
const double PI = 3.14159265358979323846;
const double PIHalf = 3.14159265358979323846 * 0.5;
const double PIPI = 6.283185307179586476925286766559;

#include "slowlib.base/math/slVector.h"

class SL_API slMath
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
};

#endif
