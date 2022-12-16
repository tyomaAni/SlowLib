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

#include <cmath>

template<typename T>
class slVec2_t
{
public:
	slVec2_t() {}
	slVec2_t(T X, T Y) :x(X),y(Y){}
	
	template <typename T2>
	slVec2_t(T2 v) :x(static_cast<T>(v)),y(static_cast<T>(v)){}

	template <typename T2>
	slVec2_t(const slVec2_t<T2>& v) :x(static_cast<T>(v.x)),y(static_cast<T>(v.y)){}

	void set(T X, T Y) { x = X; y = Y; }
	void set(T v){x = v;y = v;}

	template <typename T2>
	void set(const slVec2_t<T2>& v){x = static_cast<T>(v.x);y = static_cast<T>(v.y);}

	T operator[](uint32_t index) const{SL_ASSERT_ST(index < 2);return (&x)[index];}
	T& operator[](uint32_t index){SL_ASSERT_ST(index < 2);return (&x)[index];}

	template<typename T2>
	slVec2_t<T2> operator*(T2 v)const{slVec2_t<T2> r;r.x = x * v;r.y = y * v;return r;}

	template<typename T2>
	slVec2_t<T2> operator+(const slVec2_t<T2>& v)const{slVec2_t<T2> r;r.x = x + v.x;r.y = y + v.y;return r;}

	template<typename T2>
	slVec2_t<T2> operator-(const slVec2_t<T2>& v)const{slVec2_t<T2> r;r.x = x - v.x;r.y = y - v.y;return r;}

	template<typename T2>
	slVec2_t<T2> operator*(const slVec2_t<T2>& v)const{slVec2_t<T2> r;r.x = x * v.x;r.y = y * v.y;return r;}

	template<typename T2>
	slVec2_t<T2> operator/(const slVec2_t<T2>& v)const{slVec2_t<T2> r;r.x = x / v.x;r.y = y / v.y;return r;}

	slVec2_t<T> operator-()const{slVec2_t<T> r;r.x = -x;r.y = -y;return r;}

	template<typename T2>
	slVec2_t<T>& operator=(const slVec2_t<T2>& v){x = static_cast<T>(v.x);y = static_cast<T>(v.y);}

	bool operator==(const slVec2_t<T>& v)const{
		if (x != v.x)return false;
		if (y != v.y)return false;
		return true;
	}

	bool operator!=(const slVec2_t<T>& v)const{
		if (x != v.x)return true;
		if (y != v.y)return true;
		return false;
	}

	template<typename T2>
	void operator+=(const slVec2_t<T2>& v){x += static_cast<T>(v.x);y += static_cast<T>(v.y);}

	template<typename T2>
	void operator-=(const slVec2_t<T2>& v){x -= static_cast<T>(v.x);y -= static_cast<T>(v.y);}

	template<typename T2>
	void operator*=(const slVec2_t<T2>& v){x *= static_cast<T>(v.x);y *= static_cast<T>(v.y);}

	template<typename T2>
	void operator/=(const slVec2_t<T2>& v){x /= static_cast<T>(v.x);y /= static_cast<T>(v.y);}

	template<typename T2>
	void operator+=(T2 v){x += static_cast<T>(v);y += static_cast<T>(v);}

	template<typename T2>
	void operator-=(T2 v){x -= static_cast<T>(v);y -= static_cast<T>(v);}

	template<typename T2>
	void operator/=(T2 v){x /= static_cast<T>(v);y /= static_cast<T>(v);}

	template<typename T2>
	void operator*=(T2 v){x *= static_cast<T>(v);y *= static_cast<T>(v);}

	void normalize(){
		T len = std::sqrt(dot());
		if (len > 0)
			len = 1.0 / len;
		x *= static_cast<T>(len);
		y *= static_cast<T>(len);
	}

	T dot()const { return (x * x) + (y * y); }
	T length() const { return std::sqrt((x * x) + (y * y)); }

	T x = static_cast<T>(0);
	T y = static_cast<T>(0);
	T* data() { return &x; }
};

template<typename T>
class slVec3_t
{
public:
	slVec3_t() {}
	
	slVec3_t(T X, T Y, T Z) : 
		x(X), 
		y(Y), 
		z(Z)
	{}

	template <typename T2>
	slVec3_t(T2 v):
		x(static_cast<T>(v)),
		y(static_cast<T>(v)),
		z(static_cast<T>(v))
	{}

	template <typename T2>
	slVec3_t(const slVec3_t<T2>& v) :
		x(static_cast<T>(v.x)),
		y(static_cast<T>(v.y)),
		z(static_cast<T>(v.z))
	{}

	void set(T X, T Y, T Z) 
	{
		x = X;
		y = Y;
		z = Z;
	}
	
	void set(T v) { x = v; y = v; z = v; }

	template <typename T2>
	void set(const slVec3_t<T2>& v)
	{
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		z = static_cast<T>(v.z);
	}

	T operator[](uint32_t index) const
	{
		SL_ASSERT_ST((index >= 0) && (index < 3)); 
		return (&x)[index];
	}

	T& operator[](uint32_t index)
	{
		SL_ASSERT_ST((index >= 0) && (index < 3)); 
		return (&x)[index];
	}
	template<typename T2>
	slVec3_t<T2> operator*(T2 v)const
	{
		slVec3_t<T2> r;
		r.x = x * v;
		r.y = y * v;
		r.z = z * v;
		return r;
	}

	template<typename T2>
	slVec3_t<T2> operator+(const slVec3_t<T2>& v)const 
	{
		slVec3_t<T2> r;
		r.x = x + v.x;
		r.y = y + v.y;
		r.z = z + v.z;
		return r;
	}

	template<typename T2>
	slVec3_t<T2> operator-(const slVec3_t<T2>& v)const
	{
		slVec3_t<T2> r;
		r.x = x - v.x;
		r.y = y - v.y;
		r.z = z - v.z;
		return r;
	}

	template<typename T2>
	slVec3_t<T2> operator*(const slVec3_t<T2>& v)const
	{
		slVec3_t<T2> r;
		r.x = x * v.x;
		r.y = y * v.y;
		r.z = z * v.z;
		return r;
	}

	template<typename T2>
	slVec3_t<T2> operator/(const slVec3_t<T2>& v)const
	{
		slVec3_t<T2> r;
		r.x = x / v.x;
		r.y = y / v.y;
		r.z = z / v.z;
		return r;
	}

	slVec3_t<T> operator-()const
	{
		slVec3_t<T> r;
		r.x = -x;
		r.y = -y;
		r.z = -z;
		return r;
	}

	template<typename T2>
	slVec3_t<T>& operator=(const slVec3_t<T2>& v)
	{
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		z = static_cast<T>(v.z);
	}

	bool operator==(const slVec3_t<T>& v)const
	{
		if (x != v.x)
			return false; 
		if (y != v.y)
			return false; 
		if (z != v.z)
			return false; 
		return true;
	}

	bool operator!=(const slVec3_t<T>& v)const
	{
		if (x != v.x)
			return true; 
		if (y != v.y)
			return true; 
		if (z != v.z)
			return true; 
		return false;
	}

	template<typename T2>
	void operator+=(const slVec3_t<T2>& v)
	{
		x += static_cast<T>(v.x);
		y += static_cast<T>(v.y);
		z += static_cast<T>(v.z);
	}
	template<typename T2>
	void operator-=(const slVec3_t<T2>& v)
	{
		x -= static_cast<T>(v.x);
		y -= static_cast<T>(v.y);
		z -= static_cast<T>(v.z);
	}
	template<typename T2>
	void operator*=(const slVec3_t<T2>& v)
	{
		x *= static_cast<T>(v.x);
		y *= static_cast<T>(v.y);
		z *= static_cast<T>(v.z);
	}
	template<typename T2>
	void operator/=(const slVec3_t<T2>& v)
	{
		x /= static_cast<T>(v.x);
		y /= static_cast<T>(v.y);
		z /= static_cast<T>(v.z);
	}

	template<typename T2>
	void operator+=(T2 v)
	{
		x += static_cast<T>(v);
		y += static_cast<T>(v);
		z += static_cast<T>(v);
	}
	template<typename T2>
	void operator-=(T2 v)
	{
		x -= static_cast<T>(v);
		y -= static_cast<T>(v);
		z -= static_cast<T>(v);
	}
	template<typename T2>
	void operator/=(T2 v)
	{
		x /= static_cast<T>(v);
		y /= static_cast<T>(v);
		z /= static_cast<T>(v);
	}
	template<typename T2>
	void operator*=(T2 v)
	{
		x *= static_cast<T>(v);
		y *= static_cast<T>(v);
		z *= static_cast<T>(v);
	}

	template<typename T2>
	void cross(const slVec3_t<T2>& a, slVec3_t<T>& out)const {
		out.x = (y * static_cast<T>(a.z)) - (z * static_cast<T>(a.y));
		out.y = (z * static_cast<T>(a.x)) - (x * static_cast<T>(a.z));
		out.z = (x * static_cast<T>(a.y)) - (y * static_cast<T>(a.x));
	}

	void normalize()
	{
		T len = std::sqrt(dot());
		if (len > 0)
			len = static_cast<T>(1.0) / len;
		x *= static_cast<T>(len);
		y *= static_cast<T>(len);
		z *= static_cast<T>(len);
	}

	T dot()const { return (x * x) + (y * y) + (z * z); }
	T dot(const slVec3_t<T>& v)const { return (x * v.x) + (y * v.y) + (z * v.z); }
	T length() const { return std::sqrt((x * x) + (y * y) + (z * z)); }

	T x = static_cast<T>(0);
	T y = static_cast<T>(0);
	T z = static_cast<T>(0);
	T* data() { return &x; }
};

template<typename T>
class slVec4_t
{
public:
	typedef T this_type;

	slVec4_t() {}

	slVec4_t(T X, T Y, T Z, T W) :
		x(X),
		y(Y),
		z(Z),
		w(W)
	{}

	template <typename T2>
	slVec4_t(T2 v) :
		x(static_cast<T>(v)),
		y(static_cast<T>(v)),
		z(static_cast<T>(v)),
		w(static_cast<T>(v))
	{}

	template <typename T2>
	slVec4_t(const slVec4_t<T2>& v) :
		x(static_cast<T>(v.x)),
		y(static_cast<T>(v.y)),
		z(static_cast<T>(v.z)),
		w(static_cast<T>(v.w))
	{}

	template <typename T2>
	slVec4_t(const slVec3_t<T2>& v) :
		x(static_cast<T>(v.x)),
		y(static_cast<T>(v.y)),
		z(static_cast<T>(v.z))
	{}

	void set(T X, T Y, T Z, T W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}

	void set(T v) { x = v; y = v; z = v; w = v; }

	template <typename T2>
	void set(const slVec4_t<T2>& v)
	{
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		z = static_cast<T>(v.z);
		w = static_cast<T>(v.w);
	}

	T operator[](uint32_t index) const
	{
		SL_ASSERT_ST((index >= 0) && (index < 4));
		return (&x)[index];
	}

	T& operator[](uint32_t index)
	{
		SL_ASSERT_ST((index >= 0) && (index < 4));
		return (&x)[index];
	}

	template<typename T2>
	slVec4_t<T2> operator*(T2 v)const
	{
		slVec4_t<T2> r;
		r.x = x * v;
		r.y = y * v;
		r.z = z * v;
		r.w = w * v;
		return r;
	}

	template<typename T2>
	slVec4_t<T2> operator+(const slVec4_t<T2>& v)const
	{
		slVec4_t<T2> r;
		r.x = x + v.x;
		r.y = y + v.y;
		r.z = z + v.z;
		r.w = w + v.w;
		return r;
	}

	template<typename T2>
	slVec4_t<T2> operator-(const slVec4_t<T2>& v)const
	{
		slVec4_t<T2> r;
		r.x = x - v.x;
		r.y = y - v.y;
		r.z = z - v.z;
		r.w = w - v.w;
		return r;
	}

	template<typename T2>
	slVec4_t<T2> operator*(const slVec4_t<T2>& v)const
	{
		slVec4_t<T2> r;
		r.x = x * v.x;
		r.y = y * v.y;
		r.z = z * v.z;
		r.w = w * v.w;
		return r;
	}

	template<typename T2>
	slVec4_t<T2> operator/(const slVec4_t<T2>& v)const
	{
		slVec4_t<T2> r;
		r.x = x / v.x;
		r.y = y / v.y;
		r.z = z / v.z;
		r.w = w / v.w;
		return r;
	}

	slVec4_t<T> operator-()const
	{
		slVec4_t<T> r;
		r.x = -x;
		r.y = -y;
		r.z = -z;
		r.w = -w;
		return r;
	}

	template<typename T2>
	slVec4_t<T>& operator=(const slVec4_t<T2>& v)
	{
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		z = static_cast<T>(v.z);
		w = static_cast<T>(v.w);
		return *this;
	}

	bool operator==(const slVec4_t<T>& v)const
	{
		if (x != v.x)
			return false;
		if (y != v.y)
			return false;
		if (z != v.z)
			return false;
		if (w != v.w)
			return false;
		return true;
	}

	bool operator!=(const slVec4_t<T>& v)const
	{
		if (x != v.x)
			return true;
		if (y != v.y)
			return true;
		if (z != v.z)
			return true;
		if (w != v.w)
			return true;
		return false;
	}

	template<typename T2>
	void operator+=(const slVec4_t<T2>& v)
	{
		x += static_cast<T>(v.x);
		y += static_cast<T>(v.y);
		z += static_cast<T>(v.z);
		w += static_cast<T>(v.w);
	}
	template<typename T2>
	void operator-=(const slVec4_t<T2>& v)
	{
		x -= static_cast<T>(v.x);
		y -= static_cast<T>(v.y);
		z -= static_cast<T>(v.z);
		w -= static_cast<T>(v.w);
	}
	template<typename T2>
	void operator*=(const slVec4_t<T2>& v)
	{
		x *= static_cast<T>(v.x);
		y *= static_cast<T>(v.y);
		z *= static_cast<T>(v.z);
		w *= static_cast<T>(v.w);
	}
	template<typename T2>
	void operator/=(const slVec4_t<T2>& v)
	{
		x /= static_cast<T>(v.x);
		y /= static_cast<T>(v.y);
		z /= static_cast<T>(v.z);
		w /= static_cast<T>(v.w);
	}

	template<typename T2>
	void operator+=(T2 v)
	{
		x += static_cast<T>(v);
		y += static_cast<T>(v);
		z += static_cast<T>(v);
		w += static_cast<T>(v);
	}
	template<typename T2>
	void operator-=(T2 v)
	{
		x -= static_cast<T>(v);
		y -= static_cast<T>(v);
		z -= static_cast<T>(v);
		w -= static_cast<T>(v);
	}
	template<typename T2>
	void operator/=(T2 v)
	{
		x /= static_cast<T>(v);
		y /= static_cast<T>(v);
		z /= static_cast<T>(v);
		w /= static_cast<T>(v);
	}
	template<typename T2>
	void operator*=(T2 v)
	{
		x *= static_cast<T>(v);
		y *= static_cast<T>(v);
		z *= static_cast<T>(v);
		w *= static_cast<T>(v);
	}

	template<typename T2>
	void cross(const slVec4_t<T2>& a, slVec4_t<T>& out)const {
		out.x = (y * static_cast<T>(a.z)) - (z * static_cast<T>(a.y));
		out.y = (z * static_cast<T>(a.x)) - (x * static_cast<T>(a.z));
		out.z = (x * static_cast<T>(a.y)) - (y * static_cast<T>(a.x));
	}

	void normalize()
	{
		T len = std::sqrt(dot());
		if (len > 0)
			len = static_cast<T>(1.0) / len;
		x *= static_cast<T>(len);
		y *= static_cast<T>(len);
		z *= static_cast<T>(len);
		w *= static_cast<T>(len);
	}

	T dot()const { return (x * x) + (y * y) + (z * z) + (w * w); }
	T dot(const slVec4_t<T>& v)const { return (x * v.x) + (y * v.y) + (z * v.z) + (w * v.w); }
	T length() const { return std::sqrt((x * x) + (y * y) + (z * z) + (w * w)); }

	T x = static_cast<T>(0);
	T y = static_cast<T>(0);
	T z = static_cast<T>(0);
	T w = static_cast<T>(0);
	T* data() { return &x; }
};
SL_FORCEINLINE slVec4f operator*(const real_t& s, const slVec4f& v) { return v * s; }

#endif
