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

static const slMatrix3f g_emptyMatrix3f = { 
	slVec3f(1.f, 0.f, 0.f),
	slVec3f(0.f, 1.f, 0.f),
	slVec3f(0.f, 0.f, 1.f)
};
static const slMatrix3 g_emptyMatrix3 = {
	slVec3(1.0, 0.0, 0.0),
	slVec3(0.0, 1.0, 0.0),
	slVec3(0.0, 0.0, 1.0)
};
static const slMatrix4f g_emptyMatrix4f = {
	slVec4f(1.f, 0.f, 0.f, 0.f),
	slVec4f(0.f, 1.f, 0.f, 0.f),
	slVec4f(0.f, 0.f, 1.f, 0.f),
	slVec4f(0.f, 0.f, 0.f, 1.f)
};
static const slMatrix4 g_emptyMatrix4 = {
	slVec4f(1.0, 0.0, 0.0, 0.0),
	slVec4f(0.0, 1.0, 0.0, 0.0),
	slVec4f(0.0, 0.0, 1.0, 0.0),
	slVec4f(0.0, 0.0, 0.0, 1.0)
};

// ===========================================================
//   FLOAT 3                                                 =
// ===========================================================
slMatrix3f::slMatrix3f() { *this = g_emptyMatrix3f; }
slMatrix3f::slMatrix3f(const slMatrix3f& m) {
	m_data[0] = m.m_data[0];
	m_data[1] = m.m_data[1];
	m_data[2] = m.m_data[2];
}
slMatrix3f::slMatrix3f(const slMatrix4f& m) { copy_basis(m); }
slMatrix3f::slMatrix3f(const slMatrix3& m) { 
	m_data[0] = m.m_data[0];
	m_data[1] = m.m_data[1];
	m_data[2] = m.m_data[2];
}
slMatrix3f::slMatrix3f(const slMatrix4& m) { copy_basis(m); }
slMatrix3f::slMatrix3f(const slVec3f& r1, const slVec3f& r2, const slVec3f& r3) {
	m_data[0] = r1;
	m_data[1] = r2;
	m_data[2] = r3;
}
slVec3f slMatrix3f::operator[](uint32_t index) const {
	SL_ASSERT_ST((index >= 0) && (index < 3));
	return m_data[index];
}
slVec3f& slMatrix3f::operator[](uint32_t index) {
	SL_ASSERT_ST((index >= 0) && (index < 3));
	return m_data[index];
}
void slMatrix3f::identity() { *this = g_emptyMatrix3f; }
void slMatrix3f::copy_basis(const slMatrix4f& m) {
	m_data[0] = m.m_data[0];
	m_data[1] = m.m_data[1];
	m_data[2] = m.m_data[2];
}
void slMatrix3f::copy_basis(const slMatrix4& m) {
	m_data[0] = m.m_data[0];
	m_data[1] = m.m_data[1];
	m_data[2] = m.m_data[2];
}

// ===========================================================
//   DOUBLE 3                                                =
// ===========================================================
slMatrix3::slMatrix3() { *this = g_emptyMatrix3; }
slMatrix3::slMatrix3(const slMatrix3f& m) {
	m_data[0] = m.m_data[0];
	m_data[1] = m.m_data[1];
	m_data[2] = m.m_data[2];
}
slMatrix3::slMatrix3(const slMatrix4f& m) { copy_basis(m); }
slMatrix3::slMatrix3(const slMatrix3& m) {
	m_data[0] = m.m_data[0];
	m_data[1] = m.m_data[1];
	m_data[2] = m.m_data[2];
}
slMatrix3::slMatrix3(const slMatrix4& m) { copy_basis(m); }
slMatrix3::slMatrix3(const slVec3& r1, const slVec3& r2, const slVec3& r3) {
	m_data[0] = r1;
	m_data[1] = r2;
	m_data[2] = r3;
}
slVec3 slMatrix3::operator[](uint32_t index) const {
	SL_ASSERT_ST((index >= 0) && (index < 3));
	return m_data[index];
}
slVec3& slMatrix3::operator[](uint32_t index) {
	SL_ASSERT_ST((index >= 0) && (index < 3));
	return m_data[index];
}
void slMatrix3::identity() { *this = g_emptyMatrix3; }
void slMatrix3::copy_basis(const slMatrix4f& m) {
	m_data[0] = m.m_data[0];
	m_data[1] = m.m_data[1];
	m_data[2] = m.m_data[2];
}
void slMatrix3::copy_basis(const slMatrix4& m) {
	m_data[0] = m.m_data[0];
	m_data[1] = m.m_data[1];
	m_data[2] = m.m_data[2];
}

// ===========================================================
//   FLOAT 4                                                 =
// ===========================================================
slMatrix4f::slMatrix4f() { *this = g_emptyMatrix4f; }
slMatrix4f::slMatrix4f(const slMatrix4f& m) {
	m_data[0] = m.m_data[0];
	m_data[1] = m.m_data[1];
	m_data[2] = m.m_data[2];
	m_data[3] = m.m_data[3];
}
slMatrix4f::slMatrix4f(const slMatrix3f& m) { copy_basis(m); }
slMatrix4f::slMatrix4f(const slMatrix3& m) { copy_basis(m); }
slMatrix4f::slMatrix4f(const slMatrix4& m) {
	m_data[0] = m.m_data[0];
	m_data[1] = m.m_data[1];
	m_data[2] = m.m_data[2];
	m_data[3] = m.m_data[3];
}
slMatrix4f::slMatrix4f(const slVec4f& r1, const slVec4f& r2, const slVec4f& r3, const slVec4f& r4) {
	m_data[0] = r1;
	m_data[1] = r2;
	m_data[2] = r3;
	m_data[3] = r4;
}
slVec4f slMatrix4f::operator[](uint32_t index) const {
	SL_ASSERT_ST((index >= 0) && (index < 4));
	return m_data[index];
}
slVec4f& slMatrix4f::operator[](uint32_t index) {
	SL_ASSERT_ST((index >= 0) && (index < 4));
	return m_data[index];
}
void slMatrix4f::identity() { *this = g_emptyMatrix4f; }
void slMatrix4f::copy_basis(const slMatrix3f& m) {
	m_data[0].x = m.m_data[0].x;
	m_data[0].y = m.m_data[0].y;
	m_data[0].z = m.m_data[0].z;
	m_data[1].x = m.m_data[1].x;
	m_data[1].y = m.m_data[1].y;
	m_data[1].z = m.m_data[1].z;
	m_data[2].x = m.m_data[2].x;
	m_data[2].y = m.m_data[2].y;
	m_data[2].z = m.m_data[2].z;
}
void slMatrix4f::copy_basis(const slMatrix3& m) {
	m_data[0].x = (float)m.m_data[0].x;
	m_data[0].y = (float)m.m_data[0].y;
	m_data[0].z = (float)m.m_data[0].z;
	m_data[1].x = (float)m.m_data[1].x;
	m_data[1].y = (float)m.m_data[1].y;
	m_data[1].z = (float)m.m_data[1].z;
	m_data[2].x = (float)m.m_data[2].x;
	m_data[2].y = (float)m.m_data[2].y;
	m_data[2].z = (float)m.m_data[2].z;
}
void slMatrix4f::copy_basis(const slMatrix4f& m) {
	m_data[0].x = m.m_data[0].x;
	m_data[0].y = m.m_data[0].y;
	m_data[0].z = m.m_data[0].z;
	m_data[1].x = m.m_data[1].x;
	m_data[1].y = m.m_data[1].y;
	m_data[1].z = m.m_data[1].z;
	m_data[2].x = m.m_data[2].x;
	m_data[2].y = m.m_data[2].y;
	m_data[2].z = m.m_data[2].z;
}
void slMatrix4f::copy_basis(const slMatrix4& m) {
	m_data[0].x = (float)m.m_data[0].x;
	m_data[0].y = (float)m.m_data[0].y;
	m_data[0].z = (float)m.m_data[0].z;
	m_data[1].x = (float)m.m_data[1].x;
	m_data[1].y = (float)m.m_data[1].y;
	m_data[1].z = (float)m.m_data[1].z;
	m_data[2].x = (float)m.m_data[2].x;
	m_data[2].y = (float)m.m_data[2].y;
	m_data[2].z = (float)m.m_data[2].z;
}

// ===========================================================
//   DOUBLE 4                                                 =
// ===========================================================
slMatrix4::slMatrix4() { *this = g_emptyMatrix4; }
slMatrix4::slMatrix4(const slMatrix4f& m) {
	m_data[0] = m.m_data[0];
	m_data[1] = m.m_data[1];
	m_data[2] = m.m_data[2];
	m_data[3] = m.m_data[3];
}
slMatrix4::slMatrix4(const slMatrix3f& m) { copy_basis(m); }
slMatrix4::slMatrix4(const slMatrix3& m) { copy_basis(m); }
slMatrix4::slMatrix4(const slMatrix4& m) {
	m_data[0] = m.m_data[0];
	m_data[1] = m.m_data[1];
	m_data[2] = m.m_data[2];
	m_data[3] = m.m_data[3];
}
slMatrix4::slMatrix4(const slVec4& r1, const slVec4& r2, const slVec4& r3, const slVec4& r4) {
	m_data[0] = r1;
	m_data[1] = r2;
	m_data[2] = r3;
	m_data[3] = r4;
}
slVec4 slMatrix4::operator[](uint32_t index) const {
	SL_ASSERT_ST((index >= 0) && (index < 4));
	return m_data[index];
}
slVec4& slMatrix4::operator[](uint32_t index) {
	SL_ASSERT_ST((index >= 0) && (index < 4));
	return m_data[index];
}
void slMatrix4::identity() { *this = g_emptyMatrix4f; }
void slMatrix4::copy_basis(const slMatrix3f& m) {
	m_data[0].x = m.m_data[0].x;
	m_data[0].y = m.m_data[0].y;
	m_data[0].z = m.m_data[0].z;
	m_data[1].x = m.m_data[1].x;
	m_data[1].y = m.m_data[1].y;
	m_data[1].z = m.m_data[1].z;
	m_data[2].x = m.m_data[2].x;
	m_data[2].y = m.m_data[2].y;
	m_data[2].z = m.m_data[2].z;
}
void slMatrix4::copy_basis(const slMatrix3& m) {
	m_data[0].x = m.m_data[0].x;
	m_data[0].y = m.m_data[0].y;
	m_data[0].z = m.m_data[0].z;
	m_data[1].x = m.m_data[1].x;
	m_data[1].y = m.m_data[1].y;
	m_data[1].z = m.m_data[1].z;
	m_data[2].x = m.m_data[2].x;
	m_data[2].y = m.m_data[2].y;
	m_data[2].z = m.m_data[2].z;
}
void slMatrix4::copy_basis(const slMatrix4f& m) {
	m_data[0].x = m.m_data[0].x;
	m_data[0].y = m.m_data[0].y;
	m_data[0].z = m.m_data[0].z;
	m_data[1].x = m.m_data[1].x;
	m_data[1].y = m.m_data[1].y;
	m_data[1].z = m.m_data[1].z;
	m_data[2].x = m.m_data[2].x;
	m_data[2].y = m.m_data[2].y;
	m_data[2].z = m.m_data[2].z;
}
void slMatrix4::copy_basis(const slMatrix4& m) {
	m_data[0].x = m.m_data[0].x;
	m_data[0].y = m.m_data[0].y;
	m_data[0].z = m.m_data[0].z;
	m_data[1].x = m.m_data[1].x;
	m_data[1].y = m.m_data[1].y;
	m_data[1].z = m.m_data[1].z;
	m_data[2].x = m.m_data[2].x;
	m_data[2].y = m.m_data[2].y;
	m_data[2].z = m.m_data[2].z;
}

