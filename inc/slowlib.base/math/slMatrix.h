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
#ifndef __SL_SLOWLIBBASEMTHMAT_H__
#define __SL_SLOWLIBBASEMTHMAT_H__

class SL_API slMatrix3f
{
public:
	slMatrix3f();
	slMatrix3f(const slMatrix3f&);
	slMatrix3f(const slMatrix4f&);
	slMatrix3f(const slMatrix3&);
	slMatrix3f(const slMatrix4&);
	slMatrix3f(const slVec3f&, const slVec3f&, const slVec3f&);
	slVec3f operator[](uint32_t index) const;
	slVec3f& operator[](uint32_t index);
	void identity();
	void copy_basis(const slMatrix4f&);
	void copy_basis(const slMatrix4&);

	slVec3f m_data[3];
	float* data() { return &m_data[0].x; }
};

class SL_API slMatrix3
{
public:
	slMatrix3();
	slMatrix3(const slMatrix3f&);
	slMatrix3(const slMatrix4f&);
	slMatrix3(const slMatrix3&);
	slMatrix3(const slMatrix4&);
	slMatrix3(const slVec3&, const slVec3&, const slVec3&);
	slVec3 operator[](uint32_t index) const;
	slVec3& operator[](uint32_t index);
	void identity();
	void copy_basis(const slMatrix4f&);
	void copy_basis(const slMatrix4&);

	slVec3 m_data[3];
	double* data() { return &m_data[0].x; }
};

class SL_API slMatrix4f
{
public:
	slMatrix4f();
	slMatrix4f(const slMatrix4f&);
	slMatrix4f(const slMatrix3f&);
	slMatrix4f(const slMatrix3&);
	slMatrix4f(const slMatrix4&);
	slMatrix4f(const slVec4f&, const slVec4f&, const slVec4f&, const slVec4f&);
	slVec4f operator[](uint32_t index) const;
	slVec4f& operator[](uint32_t index);
	void identity();
	void copy_basis(const slMatrix3f&);
	void copy_basis(const slMatrix3&);
	void copy_basis(const slMatrix4f&);
	void copy_basis(const slMatrix4&);

	slVec4f m_data[4];
	float* data() { return &m_data[0].x; }
};

class SL_API slMatrix4
{
public:
	slMatrix4();
	slMatrix4(const slMatrix4f&);
	slMatrix4(const slMatrix3f&);
	slMatrix4(const slMatrix3&);
	slMatrix4(const slMatrix4&);
	slMatrix4(const slVec4&, const slVec4&, const slVec4&, const slVec4&);
	slVec4 operator[](uint32_t index) const;
	slVec4& operator[](uint32_t index);
	void identity();
	void copy_basis(const slMatrix3f&);
	void copy_basis(const slMatrix3&);
	void copy_basis(const slMatrix4f&);
	void copy_basis(const slMatrix4&);

	slVec4 m_data[4];
	double* data() { return &m_data[0].x; }
};


#endif
