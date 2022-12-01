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

slVec3::slVec3(){}
slVec3::slVec3(double _x, double _y, double _z): x(_x),y(_y),z(_z){}
slVec3::slVec3(float v):x(v),y(v),z(v) {}
slVec3::slVec3(double v) : x(v), y(v), z(v) {}
slVec3::slVec3(int32_t v) : x(v), y(v), z(v) {}
slVec3::slVec3(const slVec3& v) : x(v.x), y(v.y), z(v.z) {}
slVec3::slVec3(const slVec3f& v) : x(v.x), y(v.y), z(v.z) {}
slVec3::slVec3(const slVec4& v) : x(v.x), y(v.y), z(v.z) {}
slVec3::slVec3(const slVec4f& v) : x(v.x), y(v.y), z(v.z) {}
double slVec3::operator[](uint32_t index) const { SL_ASSERT_ST((index >= 0) && (index < 3)); return (&x)[index]; }
double& slVec3::operator[](uint32_t index) { SL_ASSERT_ST((index >= 0) && (index < 3)); return (&x)[index]; }
slVec3 slVec3::operator+(const slVec3& v)const { slVec3 r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; return r;}
slVec3 slVec3::operator+(const slVec3f& v)const { slVec3 r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; return r;}
slVec3 slVec3::operator+(const slVec4& v)const { slVec3 r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; return r;}
slVec3 slVec3::operator+(const slVec4f& v)const { slVec3 r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; return r;}
slVec3 slVec3::operator-(const slVec3& v)const { slVec3 r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; return r;}
slVec3 slVec3::operator-(const slVec3f& v)const { slVec3 r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; return r;}
slVec3 slVec3::operator-(const slVec4& v)const { slVec3 r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; return r;}
slVec3 slVec3::operator-(const slVec4f& v)const { slVec3 r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; return r;}
slVec3 slVec3::operator*(const slVec3& v)const { slVec3 r; r.x = x * v.x; r.y = y * v.y; r.z = z * v.z; return r;}
slVec3 slVec3::operator*(const slVec3f& v)const { slVec3 r; r.x = x * v.x; r.y = y * v.y; r.z = z * v.z; return r;}
slVec3 slVec3::operator*(const slVec4& v)const { slVec3 r; r.x = x * v.x; r.y = y * v.y; r.z = z * v.z; return r;}
slVec3 slVec3::operator*(const slVec4f& v)const { slVec3 r; r.x = x * v.x; r.y = y * v.y; r.z = z * v.z; return r;}
slVec3 slVec3::operator/(const slVec3& v)const { slVec3 r; r.x = x / v.x; r.y = y / v.y; r.z = z / v.z; return r;}
slVec3 slVec3::operator/(const slVec3f& v)const { slVec3 r; r.x = x / v.x; r.y = y / v.y; r.z = z / v.z; return r;}
slVec3 slVec3::operator/(const slVec4& v)const { slVec3 r; r.x = x / v.x; r.y = y / v.y; r.z = z / v.z; return r;}
slVec3 slVec3::operator/(const slVec4f& v)const { slVec3 r; r.x = x / v.x; r.y = y / v.y; r.z = z / v.z; return r;}
slVec3 slVec3::operator-()const { slVec3 r; r.x = -x; r.y = -y; r.z = -z; return r; }
slVec3& slVec3::operator=(const slVec3& v) { x = v.x; y = v.y; z = v.z; return *this; }
slVec3& slVec3::operator=(const slVec3f& v) { x = v.x; y = v.y; z = v.z; return *this; }
slVec3& slVec3::operator=(const slVec4& v) { x = v.x; y = v.y; z = v.z; return *this; }
slVec3& slVec3::operator=(const slVec4f& v) { x = v.x; y = v.y; z = v.z; return *this; }
bool slVec3::operator==(const slVec3& v)const { if (x != v.x)return false; if (y != v.y)return false; if (z != v.z)return false; return true; }
bool slVec3::operator!=(const slVec3& v)const { if (x != v.x)return true; if (y != v.y)return true; if (z != v.z)return true; return false; }

slVec3f::slVec3f() {}
slVec3f::slVec3f(double _x, double _y, double _z) : x((float)_x), y((float)_y), z((float)_z) {}
slVec3f::slVec3f(float v) : x(v), y(v), z(v) {}
slVec3f::slVec3f(double v) : x((float)v), y((float)v), z((float)v) {}
slVec3f::slVec3f(int32_t v) : x((float)v), y((float)v), z((float)v) {}
slVec3f::slVec3f(const slVec3& v) : x((float)v.x), y((float)v.y), z((float)v.z) {}
slVec3f::slVec3f(const slVec3f& v) : x(v.x), y(v.y), z(v.z) {}
slVec3f::slVec3f(const slVec4& v) : x((float)v.x), y((float)v.y), z((float)v.z) {}
slVec3f::slVec3f(const slVec4f& v) : x(v.x), y(v.y), z(v.z) {}
float slVec3f::operator[](uint32_t index) const { SL_ASSERT_ST((index >= 0) && (index < 3)); return (&x)[index]; }
float& slVec3f::operator[](uint32_t index) { SL_ASSERT_ST((index >= 0) && (index < 3)); return (&x)[index]; }
slVec3f slVec3f::operator+(const slVec3& v)const { slVec3f r; r.x = x + (float)v.x; r.y = y + (float)v.y; r.z = z + (float)v.z; return r; }
slVec3f slVec3f::operator+(const slVec3f& v)const { slVec3f r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; return r; }
slVec3f slVec3f::operator+(const slVec4& v)const { slVec3f r; r.x = x + (float)v.x; r.y = y + (float)v.y; r.z = z + (float)v.z; return r; }
slVec3f slVec3f::operator+(const slVec4f& v)const { slVec3f r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; return r; }
slVec3f slVec3f::operator-(const slVec3& v)const { slVec3f r; r.x = x - (float)v.x; r.y = y - (float)v.y; r.z = z - (float)v.z; return r; }
slVec3f slVec3f::operator-(const slVec3f& v)const { slVec3f r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; return r; }
slVec3f slVec3f::operator-(const slVec4& v)const { slVec3f r; r.x = x - (float)v.x; r.y = y - (float)v.y; r.z = z - (float)v.z; return r; }
slVec3f slVec3f::operator-(const slVec4f& v)const { slVec3f r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; return r; }
slVec3f slVec3f::operator*(const slVec3& v)const { slVec3f r; r.x = x * (float)v.x; r.y = y * (float)v.y; r.z = z * (float)v.z; return r; }
slVec3f slVec3f::operator*(const slVec3f& v)const { slVec3f r; r.x = x * v.x; r.y = y * v.y; r.z = z * v.z; return r; }
slVec3f slVec3f::operator*(const slVec4& v)const { slVec3f r; r.x = x * (float)v.x; r.y = y * (float)v.y; r.z = z * (float)v.z; return r; }
slVec3f slVec3f::operator*(const slVec4f& v)const { slVec3f r; r.x = x * v.x; r.y = y * v.y; r.z = z * v.z; return r; }
slVec3f slVec3f::operator/(const slVec3& v)const { slVec3f r; r.x = x / (float)v.x; r.y = y / (float)v.y; r.z = z / (float)v.z; return r; }
slVec3f slVec3f::operator/(const slVec3f& v)const { slVec3f r; r.x = x / v.x; r.y = y / v.y; r.z = z / v.z; return r; }
slVec3f slVec3f::operator/(const slVec4& v)const { slVec3f r; r.x = x / (float)v.x; r.y = y / (float)v.y; r.z = z / (float)v.z; return r; }
slVec3f slVec3f::operator/(const slVec4f& v)const { slVec3f r; r.x = x / v.x; r.y = y / v.y; r.z = z / v.z; return r; }
slVec3f slVec3f::operator-()const { slVec3f r; r.x = -x; r.y = -y; r.z = -z; return r; }
slVec3f& slVec3f::operator=(const slVec3& v) { x = (float)v.x; y = (float)v.y; z = (float)v.z; return *this; }
slVec3f& slVec3f::operator=(const slVec3f& v) { x = v.x; y = v.y; z = v.z; return *this; }
slVec3f& slVec3f::operator=(const slVec4& v) { x = (float)v.x; y = (float)v.y; z = (float)v.z; return *this; }
slVec3f& slVec3f::operator=(const slVec4f& v) { x = v.x; y = v.y; z = v.z; return *this; }
bool slVec3f::operator==(const slVec3f& v)const { if (x != v.x)return false; if (y != v.y)return false; if (z != v.z)return false; return true; }
bool slVec3f::operator!=(const slVec3f& v)const { if (x != v.x)return true; if (y != v.y)return true; if (z != v.z)return true; return false; }

slVec4::slVec4() {}
slVec4::slVec4(double _x, double _y, double _z, double _w) : x(_x), y(_y), z(_z), w(_w) {}
slVec4::slVec4(float v) : x(v), y(v), z(v), w(v) {}
slVec4::slVec4(double v) : x(v), y(v), z(v), w(v) {}
slVec4::slVec4(int32_t v) : x(v), y(v), z(v), w(v) {}
slVec4::slVec4(const slVec3& v) : x(v.x), y(v.y), z(v.z), w(0.0) {}
slVec4::slVec4(const slVec3f& v) : x(v.x), y(v.y), z(v.z), w(0.0) {}
slVec4::slVec4(const slVec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
slVec4::slVec4(const slVec4f& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
double slVec4::operator[](uint32_t index) const { SL_ASSERT_ST((index >= 0) && (index < 4)); return (&x)[index]; }
double& slVec4::operator[](uint32_t index){ SL_ASSERT_ST((index >= 0) && (index < 4)); return (&x)[index]; }
slVec4 slVec4::operator+(const slVec3& v)const { slVec4 r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; return r; }
slVec4 slVec4::operator+(const slVec3f& v)const { slVec4 r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; return r; }
slVec4 slVec4::operator+(const slVec4& v)const { slVec4 r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; r.w = w + v.w; return r; }
slVec4 slVec4::operator+(const slVec4f& v)const { slVec4 r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; r.w = w + v.w; return r; }
slVec4 slVec4::operator-(const slVec3& v)const { slVec4 r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; return r; }
slVec4 slVec4::operator-(const slVec3f& v)const { slVec4 r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; return r; }
slVec4 slVec4::operator-(const slVec4& v)const { slVec4 r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; r.w = w - v.w; return r; }
slVec4 slVec4::operator-(const slVec4f& v)const { slVec4 r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; r.w = w - v.w; return r; }
slVec4 slVec4::operator*(const slVec3& v)const { slVec4 r; r.x = x * v.x; r.y = y * v.y; r.z = z * v.z; return r; }
slVec4 slVec4::operator*(const slVec3f& v)const { slVec4 r; r.x = x * v.x; r.y = y * v.y; r.z = z * v.z; return r; }
slVec4 slVec4::operator*(const slVec4& v)const { slVec4 r; r.x = x * v.x; r.y = y * v.y; r.z = z * v.z; r.w = w * v.w; return r; }
slVec4 slVec4::operator*(const slVec4f& v)const { slVec4 r; r.x = x * v.x; r.y = y * v.y; r.z = z * v.z; r.w = w * v.w; return r; }
slVec4 slVec4::operator/(const slVec3& v)const { slVec4 r; r.x = x / v.x; r.y = y / v.y; r.z = z / v.z; return r; }
slVec4 slVec4::operator/(const slVec3f& v)const { slVec4 r; r.x = x / v.x; r.y = y / v.y; r.z = z / v.z; return r; }
slVec4 slVec4::operator/(const slVec4& v)const { slVec4 r; r.x = x / v.x; r.y = y / v.y; r.z = z / v.z; r.w = w / v.w; return r; }
slVec4 slVec4::operator/(const slVec4f& v)const { slVec4 r; r.x = x / v.x; r.y = y / v.y; r.z = z / v.z; r.w = w / v.w; return r; }
slVec4 slVec4::operator-()const { slVec4 r; r.x = -x; r.y = -y; r.z = -z; r.w = -w; return r; }
slVec4& slVec4::operator=(const slVec3& v) { x = v.x; y = v.y; z = v.z; return *this; }
slVec4& slVec4::operator=(const slVec3f& v) { x = v.x; y = v.y; z = v.z; return *this; }
slVec4& slVec4::operator=(const slVec4& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
slVec4& slVec4::operator=(const slVec4f& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
bool slVec4::operator==(const slVec4& v)const { if (x != v.x)return false; if (y != v.y)return false; if (z != v.z)return false; if (w != v.w)return false; return true; }
bool slVec4::operator!=(const slVec4& v)const { if (x != v.x)return true; if (y != v.y)return true; if (z != v.z)return true; if (w != v.w)return true; return false; }

slVec4f::slVec4f() {}
slVec4f::slVec4f(double _x, double _y, double _z, double _w) : x((float)_x), y((float)_y), z((float)_z), w((float)_w) {}
slVec4f::slVec4f(float v) : x(v), y(v), z(v), w(v) {}
slVec4f::slVec4f(double v) : x((float)v), y((float)v), z((float)v), w((float)v) {}
slVec4f::slVec4f(int32_t v) : x((float)v), y((float)v), z((float)v), w((float)v) {}
slVec4f::slVec4f(const slVec3& v) : x((float)v.x), y((float)v.y), z((float)v.z), w(0.0) {}
slVec4f::slVec4f(const slVec3f& v) : x(v.x), y(v.y), z(v.z), w(0.0) {}
slVec4f::slVec4f(const slVec4& v) : x((float)v.x), y((float)v.y), z((float)v.z), w((float)v.w) {}
slVec4f::slVec4f(const slVec4f& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
float slVec4f::operator[](uint32_t index) const { SL_ASSERT_ST((index >= 0) && (index < 4)); return (&x)[index]; }
float& slVec4f::operator[](uint32_t index) { SL_ASSERT_ST((index >= 0) && (index < 4)); return (&x)[index]; }
slVec4f slVec4f::operator+(const slVec3& v)const { slVec4f r; r.x = x + (float)v.x; r.y = y + (float)v.y; r.z = z + (float)v.z; return r; }
slVec4f slVec4f::operator+(const slVec3f& v)const { slVec4f r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; return r; }
slVec4f slVec4f::operator+(const slVec4& v)const { slVec4f r; r.x = x + (float)v.x; r.y = y + (float)v.y; r.z = z + (float)v.z; r.w = w + (float)v.w; return r; }
slVec4f slVec4f::operator+(const slVec4f& v)const { slVec4f r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; r.w = w + v.w; return r; }
slVec4f slVec4f::operator-(const slVec3& v)const { slVec4f r; r.x = x - (float)v.x; r.y = y - (float)v.y; r.z = z - (float)v.z; return r; }
slVec4f slVec4f::operator-(const slVec3f& v)const { slVec4f r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; return r; }
slVec4f slVec4f::operator-(const slVec4& v)const { slVec4f r; r.x = x - (float)v.x; r.y = y - (float)v.y; r.z = z - (float)v.z; r.w = w - (float)v.w; return r; }
slVec4f slVec4f::operator-(const slVec4f& v)const { slVec4f r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; r.w = w - v.w; return r; }
slVec4f slVec4f::operator*(const slVec3& v)const { slVec4f r; r.x = x * (float)v.x; r.y = y * (float)v.y; r.z = z * (float)v.z; return r; }
slVec4f slVec4f::operator*(const slVec3f& v)const { slVec4f r; r.x = x * v.x; r.y = y * v.y; r.z = z * v.z; return r; }
slVec4f slVec4f::operator*(const slVec4& v)const { slVec4f r; r.x = x * (float)v.x; r.y = y * (float)v.y; r.z = z * (float)v.z; r.w = w * (float)v.w; return r; }
slVec4f slVec4f::operator*(const slVec4f& v)const { slVec4f r; r.x = x * v.x; r.y = y * v.y; r.z = z * v.z; r.w = w * v.w; return r; }
slVec4f slVec4f::operator/(const slVec3& v)const { slVec4f r; r.x = x / (float)v.x; r.y = y / (float)v.y; r.z = z / (float)v.z; return r; }
slVec4f slVec4f::operator/(const slVec3f& v)const { slVec4f r; r.x = x / v.x; r.y = y / v.y; r.z = z / v.z; return r; }
slVec4f slVec4f::operator/(const slVec4& v)const { slVec4f r; r.x = x / (float)v.x; r.y = y / (float)v.y; r.z = z / (float)v.z; r.w = w / (float)v.w; return r; }
slVec4f slVec4f::operator/(const slVec4f& v)const { slVec4f r; r.x = x / v.x; r.y = y / v.y; r.z = z / v.z; r.w = w / v.w; return r; }
slVec4f slVec4f::operator-()const { slVec4 r; r.x = -x; r.y = -y; r.z = -z; r.w = -w; return r; }
slVec4f& slVec4f::operator=(const slVec3& v) { x = (float)v.x; y = (float)v.y; z = (float)v.z; return *this; }
slVec4f& slVec4f::operator=(const slVec3f& v) { x = v.x; y = v.y; z = v.z; return *this; }
slVec4f& slVec4f::operator=(const slVec4& v) { x = (float)v.x; y = (float)v.y; z = (float)v.z; w = (float)v.w; return *this; }
slVec4f& slVec4f::operator=(const slVec4f& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
bool slVec4f::operator==(const slVec4f& v)const { if (x != v.x)return false; if (y != v.y)return false; if (z != v.z)return false; if (w != v.w)return false; return true; }
bool slVec4f::operator!=(const slVec4f& v)const { if (x != v.x)return true; if (y != v.y)return true; if (z != v.z)return true; if (w != v.w)return true; return false; }
