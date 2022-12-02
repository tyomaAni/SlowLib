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

