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

#include "slQuaternion.h"

template<typename T>
class slMatrix4_t
{
public:
	slMatrix4_t() { identity(); }

	template<typename T2>
	slMatrix4_t(const slMatrix4_t<T2>& m) 
	{
		m_data[0] = m.m_data[0];
		m_data[1] = m.m_data[1];
		m_data[2] = m.m_data[2];
		m_data[3] = m.m_data[3];
	}

	slMatrix4_t(const slQuaternion& q)
	{
		identity();
		set_rotation(q);
	}

	slMatrix4_t(const slVec4_t<T>& x, const slVec4_t<T>& y, const slVec4_t<T>& z, const slVec4_t<T>& w) {
		m_data[0u] = x;
		m_data[1u] = y;
		m_data[2u] = z;
		m_data[3u] = w;
	}

	void identity() {
		auto* p = this->data();
		p[0] = static_cast<T>(1.f);
		p[1] = static_cast<T>(0.f);
		p[2] = static_cast<T>(0.f);
		p[3] = static_cast<T>(0.f);

		p[4] = static_cast<T>(0.f);
		p[5] = static_cast<T>(1.f);
		p[6] = static_cast<T>(0.f);
		p[7] = static_cast<T>(0.f);

		p[8] = static_cast<T>(0.f);
		p[9] = static_cast<T>(0.f);
		p[10] = static_cast<T>(1.f);
		p[11] = static_cast<T>(0.f);

		p[12] = static_cast<T>(0.f);
		p[13] = static_cast<T>(0.f);
		p[14] = static_cast<T>(0.f);
		p[15] = static_cast<T>(1.f);
	}

	void set(
		T xx, T xy, T xz,
		T yx, T yy, T yz,
		T zx, T zy, T zz)
	{
		m_data[0].set(xx, xy, xz, 0.f);
		m_data[1].set(yx, yy, yz, 0.f);
		m_data[2].set(zx, zy, zz, 0.f);
	}

	void set_rotation(const slQuaternion& q)
	{
		T d = q.length();
		T s = 2.0f / d;
		T xs = q.x * s, ys = q.y * s, zs = q.z * s;
		T wx = q.w * xs, wy = q.w * ys, wz = q.w * zs;
		T xx = q.x * xs, xy = q.x * ys, xz = q.x * zs;
		T yy = q.y * ys, yz = q.y * zs, zz = q.z * zs;
		set(
			1.0f - (yy + zz), xy - wz, xz + wy,
			xy + wz, 1.0f - (xx + zz), yz - wx,
			xz - wy, yz + wx, 1.0f - (xx + yy));
	}

	slVec4_t<T>& operator[](uint32_t i) { assert(i <= 3); return m_data[i]; }
	const slVec4_t<T>& operator[](uint32_t i) const { assert(i <= 3); return m_data[i]; }

	slMatrix4_t<T> operator*(const slMatrix4_t<T>& m) const
	{
		return slMatrix4_t<T>(
			m_data[0] * m.m_data[0].x + m_data[1] * m.m_data[0].y + m_data[2] * m.m_data[0].z + m_data[3] * m.m_data[0].w,
			m_data[0] * m.m_data[1].x + m_data[1] * m.m_data[1].y + m_data[2] * m.m_data[1].z + m_data[3] * m.m_data[1].w,
			m_data[0] * m.m_data[2].x + m_data[1] * m.m_data[2].y + m_data[2] * m.m_data[2].z + m_data[3] * m.m_data[2].w,
			m_data[0] * m.m_data[3].x + m_data[1] * m.m_data[3].y + m_data[2] * m.m_data[3].z + m_data[3] * m.m_data[3].w
		);
	}

	slVec4_t<T> operator*(const slVec4_t<T>& v) const {
		return slVec4_t<T>
		(
			v.x * m_data[0].x + v.y * m_data[1].x + v.z * m_data[2].x + v.w * m_data[2].x,
			v.x * m_data[0].y + v.y * m_data[1].y + v.z * m_data[2].y + v.w * m_data[2].y,
			v.x * m_data[0].z + v.y * m_data[1].z + v.z * m_data[2].z + v.w * m_data[2].z,
			v.x * m_data[0].w + v.y * m_data[1].w + v.z * m_data[2].w + v.w * m_data[2].w
		);
	}

	slMatrix4_t<T>& operator*=(const slMatrix4_t<T>& m) {
		(*this) = (*this) * m;
		return *this;
	}

	void transpose()
	{
		slMatrix4_t<T> tmp;
		tmp[0].x = this->m_data[0].x; //0
		tmp[0].y = this->m_data[1].x; //1
		tmp[0].z = this->m_data[2].x; //2
		tmp[0].w = this->m_data[3].x; //3

		tmp[1].x = this->m_data[0].y; //4
		tmp[1].y = this->m_data[1].y; //5
		tmp[1].z = this->m_data[2].y; //6
		tmp[1].w = this->m_data[3].y; //7

		tmp[2].x = this->m_data[0].z; //8
		tmp[2].y = this->m_data[1].z; //9
		tmp[2].z = this->m_data[2].z; //10
		tmp[2].w = this->m_data[3].z; //11

		tmp[3].x = this->m_data[0].w; //12
		tmp[3].y = this->m_data[1].w; //13
		tmp[3].z = this->m_data[2].w; //14
		tmp[3].w = this->m_data[3].w; //15
		this->m_data[0] = tmp[0];
		this->m_data[1] = tmp[1];
		this->m_data[2] = tmp[2];
		this->m_data[3] = tmp[3];
	}

	//https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/matrix-inverse
	bool invert()
	{
		slMatrix4_t<T> mat;
		auto ptr = this->data();
		for (unsigned column = 0; column < 4; ++column)
		{
			// Swap row in case our pivot point is not working
			auto column_data = m_data[column].data();
			if (column_data[column] == 0)
			{
				unsigned big = column;
				for (unsigned row = 0; row < 4; ++row)
				{
					auto row_data = m_data[row].data();
					auto big_data = m_data[big].data();
					if (fabs(row_data[column]) > fabs(big_data[column]))
						big = row;
				}
				// Print this is a singular matrix, return identity ?
				if (big == column)
					fprintf(stderr, "Singular matrix\n");
				// Swap rows                               
				else for (unsigned j = 0; j < 4; ++j)
				{
					auto big_data = m_data[big].data();
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
					auto row_data = m_data[row].data();
					T coeff = row_data[column] / column_data[column];
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
				auto row_data = m_data[row].data();
				other_row_data[column] /= row_data[row];
			}
		}

		*this = mat;
		return true;
	}

	slMatrix4_t<T> get_basis() {
		slMatrix4_t<T> other;
		other.m_data[0].x = m_data[0].x;
		other.m_data[0].y = m_data[0].y;
		other.m_data[0].z = m_data[0].z;
		other.m_data[1].x = m_data[1].x;
		other.m_data[1].y = m_data[1].y;
		other.m_data[1].z = m_data[1].z;
		other.m_data[2].x = m_data[2].x;
		other.m_data[2].y = m_data[2].y;
		other.m_data[2].z = m_data[2].z;
		return other;
	}

	slVec4_t<T> m_data[4];
	T* data() { return &m_data[0].x; }
};



#endif
