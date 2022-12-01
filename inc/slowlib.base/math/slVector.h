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
#ifndef __SL_SLOWLIBBASEMTHVEC_H__
#define __SL_SLOWLIBBASEMTHVEC_H__

class SL_API slVec3
{
public:
	slVec3();
	slVec3(double x, double y, double z);
	slVec3(float v);
	slVec3(double v);
	slVec3(int32_t v);
	slVec3(const slVec3&);
	slVec3(const slVec3f&);
	slVec3(const slVec4&);
	slVec3(const slVec4f&);
	double operator[](uint32_t index) const;
	double& operator[](uint32_t index);
	slVec3 operator+(const slVec3& v)const;
	slVec3 operator+(const slVec3f& v)const;
	slVec3 operator+(const slVec4& v)const;
	slVec3 operator+(const slVec4f& v)const;
	slVec3 operator-(const slVec3& v)const;
	slVec3 operator-(const slVec3f& v)const;
	slVec3 operator-(const slVec4& v)const;
	slVec3 operator-(const slVec4f& v)const;
	slVec3 operator*(const slVec3& v)const;
	slVec3 operator*(const slVec3f& v)const;
	slVec3 operator*(const slVec4& v)const;
	slVec3 operator*(const slVec4f& v)const;
	slVec3 operator/(const slVec3& v)const;
	slVec3 operator/(const slVec3f& v)const;
	slVec3 operator/(const slVec4& v)const;
	slVec3 operator/(const slVec4f& v)const;
	slVec3 operator-()const;
	slVec3& operator=(const slVec3& v);
	slVec3& operator=(const slVec3f& v);
	slVec3& operator=(const slVec4& v);
	slVec3& operator=(const slVec4f& v);
	bool operator==(const slVec3& v)const;
	bool operator!=(const slVec3& v)const;

	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
	double* data() { return &x; }
};

class SL_API slVec3f
{
public:
	slVec3f();
	slVec3f(double x, double y, double z);
	slVec3f(float v);
	slVec3f(double v);
	slVec3f(int32_t v);
	slVec3f(const slVec3&);
	slVec3f(const slVec3f&);
	slVec3f(const slVec4&);
	slVec3f(const slVec4f&);
	float operator[](uint32_t index) const;
	float& operator[](uint32_t index);
	slVec3f operator+(const slVec3& v)const;
	slVec3f operator+(const slVec3f& v)const;
	slVec3f operator+(const slVec4& v)const;
	slVec3f operator+(const slVec4f& v)const;
	slVec3f operator-(const slVec3& v)const;
	slVec3f operator-(const slVec3f& v)const;
	slVec3f operator-(const slVec4& v)const;
	slVec3f operator-(const slVec4f& v)const;
	slVec3f operator*(const slVec3& v)const;
	slVec3f operator*(const slVec3f& v)const;
	slVec3f operator*(const slVec4& v)const;
	slVec3f operator*(const slVec4f& v)const;
	slVec3f operator/(const slVec3& v)const;
	slVec3f operator/(const slVec3f& v)const;
	slVec3f operator/(const slVec4& v)const;
	slVec3f operator/(const slVec4f& v)const;
	slVec3f operator-()const;
	slVec3f& operator=(const slVec3& v);
	slVec3f& operator=(const slVec3f& v);
	slVec3f& operator=(const slVec4& v);
	slVec3f& operator=(const slVec4f& v);
	bool operator==(const slVec3f& v)const;
	bool operator!=(const slVec3f& v)const;

	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	float* data() { return &x; }
};

class SL_API slVec4
{
public:
	slVec4();
	slVec4(double x, double y, double z, double w);
	slVec4(float v);
	slVec4(double v);
	slVec4(int32_t v);
	slVec4(const slVec3&);
	slVec4(const slVec3f&);
	slVec4(const slVec4&);
	slVec4(const slVec4f&);
	double operator[](uint32_t index) const;
	double& operator[](uint32_t index);
	slVec4 operator+(const slVec3& v)const;
	slVec4 operator+(const slVec3f& v)const;
	slVec4 operator+(const slVec4& v)const;
	slVec4 operator+(const slVec4f& v)const;
	slVec4 operator-(const slVec3& v)const;
	slVec4 operator-(const slVec3f& v)const;
	slVec4 operator-(const slVec4& v)const;
	slVec4 operator-(const slVec4f& v)const;
	slVec4 operator*(const slVec3& v)const;
	slVec4 operator*(const slVec3f& v)const;
	slVec4 operator*(const slVec4& v)const;
	slVec4 operator*(const slVec4f& v)const;
	slVec4 operator/(const slVec3& v)const;
	slVec4 operator/(const slVec3f& v)const;
	slVec4 operator/(const slVec4& v)const;
	slVec4 operator/(const slVec4f& v)const;
	slVec4 operator-()const;
	slVec4& operator=(const slVec3& v);
	slVec4& operator=(const slVec3f& v);
	slVec4& operator=(const slVec4& v);
	slVec4& operator=(const slVec4f& v);
	bool operator==(const slVec4& v)const;
	bool operator!=(const slVec4& v)const;

	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
	double w = 0.0;
	double* data() { return &x; }
};

class SL_API slVec4f
{
public:
	slVec4f();
	slVec4f(double x, double y, double z, double w);
	slVec4f(float v);
	slVec4f(double v);
	slVec4f(int32_t v);
	slVec4f(const slVec3&);
	slVec4f(const slVec3f&);
	slVec4f(const slVec4&);
	slVec4f(const slVec4f&);
	float operator[](uint32_t index) const;
	float& operator[](uint32_t index);
	slVec4f operator+(const slVec3& v)const;
	slVec4f operator+(const slVec3f& v)const;
	slVec4f operator+(const slVec4& v)const;
	slVec4f operator+(const slVec4f& v)const;
	slVec4f operator-(const slVec3& v)const;
	slVec4f operator-(const slVec3f& v)const;
	slVec4f operator-(const slVec4& v)const;
	slVec4f operator-(const slVec4f& v)const;
	slVec4f operator*(const slVec3& v)const;
	slVec4f operator*(const slVec3f& v)const;
	slVec4f operator*(const slVec4& v)const;
	slVec4f operator*(const slVec4f& v)const;
	slVec4f operator/(const slVec3& v)const;
	slVec4f operator/(const slVec3f& v)const;
	slVec4f operator/(const slVec4& v)const;
	slVec4f operator/(const slVec4f& v)const;
	slVec4f operator-()const;
	slVec4f& operator=(const slVec3& v);
	slVec4f& operator=(const slVec3f& v);
	slVec4f& operator=(const slVec4& v);
	slVec4f& operator=(const slVec4f& v);
	bool operator==(const slVec4f& v)const;
	bool operator!=(const slVec4f& v)const;

	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	float w = 0.0;
	float* data() { return &x; }
};

#endif
