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
#include "slowlib.base/geometry/slAabb.h"
#include "slowlib.base/geometry/slRay.h"

slAabb::slAabb()
	:
	m_min(slVec4fFltMax),
	m_max(slVec4fFltMaxNeg)
{}

slAabb::slAabb(const slVec4& min, const slVec4& max) 
	:
	m_min(min), 
	m_max(max) 
{}

/*
	Transforming Axis-Aligned Bounding Boxes
	by Jim Arvo
	from "Graphics Gems", Academic Press, 1990
*/
void slAabb::Transform(slAabb* original, slMat4* matrix, slVec4* position) 
{
	real_t  a, b;
	real_t  Amin[3], Amax[3];
	real_t  Bmin[3], Bmax[3];
	int32_t    i, j;

	/*Copy box A into a min array and a max array for easy reference.*/
	Amin[0] = original->m_min.x;  Amax[0] = original->m_max.x;
	Amin[1] = original->m_min.y;  Amax[1] = original->m_max.y;
	Amin[2] = original->m_min.z;  Amax[2] = original->m_max.z;

	/* Take care of translation by beginning at T. */
	Bmin[0] = Bmax[0] = position->x;
	Bmin[1] = Bmax[1] = position->y;
	Bmin[2] = Bmax[2] = position->z;

	/* Now find the extreme points by considering the product of the */
	/* min and max with each component of M.  */
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			// mimi: I swapped j and i
			a = (matrix->m_data[j][i] * Amin[j]);
			b = (matrix->m_data[j][i] * Amax[j]);
			if (a < b)
			{
				Bmin[i] += a;
				Bmax[i] += b;
			}
			else
			{
				Bmin[i] += b;
				Bmax[i] += a;
			}
		}
	}
	/* Copy the result into the new box. */
	m_min.x = Bmin[0];  m_max.x = Bmax[0];
	m_min.y = Bmin[1];  m_max.y = Bmax[1];
	m_min.z = Bmin[2];  m_max.z = Bmax[2];
}

void slAabb::Add(const slVec4& point)
{
	if (point.x < m_min.x) m_min.x = point.x;
	if (point.y < m_min.y) m_min.y = point.y;
	if (point.z < m_min.z) m_min.z = point.z;

	if (point.x > m_max.x) m_max.x = point.x;
	if (point.y > m_max.y) m_max.y = point.y;
	if (point.z > m_max.z) m_max.z = point.z;
}

void slAabb::Add(const slVec3& point)
{
	if (point.x < m_min.x) m_min.x = point.x;
	if (point.y < m_min.y) m_min.y = point.y;
	if (point.z < m_min.z) m_min.z = point.z;

	if (point.x > m_max.x) m_max.x = point.x;
	if (point.y > m_max.y) m_max.y = point.y;
	if (point.z > m_max.z) m_max.z = point.z;
}

void slAabb::Add(const slVec4f& point)
{
	if (point.x < m_min.x) m_min.x = point.x;
	if (point.y < m_min.y) m_min.y = point.y;
	if (point.z < m_min.z) m_min.z = point.z;

	if (point.x > m_max.x) m_max.x = point.x;
	if (point.y > m_max.y) m_max.y = point.y;
	if (point.z > m_max.z) m_max.z = point.z;
}

void slAabb::Add(const slVec3f& point)
{
	if (point.x < m_min.x) m_min.x = point.x;
	if (point.y < m_min.y) m_min.y = point.y;
	if (point.z < m_min.z) m_min.z = point.z;

	if (point.x > m_max.x) m_max.x = point.x;
	if (point.y > m_max.y) m_max.y = point.y;
	if (point.z > m_max.z) m_max.z = point.z;
}

void slAabb::Add(const slAabb& box)
{
	if (box.m_min.x < m_min.x) m_min.x = box.m_min.x;
	if (box.m_min.y < m_min.y) m_min.y = box.m_min.y;
	if (box.m_min.z < m_min.z) m_min.z = box.m_min.z;

	if (box.m_max.x > m_max.x) m_max.x = box.m_max.x;
	if (box.m_max.y > m_max.y) m_max.y = box.m_max.y;
	if (box.m_max.z > m_max.z) m_max.z = box.m_max.z;
}

bool slAabb::RayTest(const slRay& r)
{
	real_t t1 = (m_min.x - r.m_origin.x) * r.m_invDir.x;
	real_t t2 = (m_max.x - r.m_origin.x) * r.m_invDir.x;
	real_t t3 = (m_min.y - r.m_origin.y) * r.m_invDir.y;
	real_t t4 = (m_max.y - r.m_origin.y) * r.m_invDir.y;
	real_t t5 = (m_min.z - r.m_origin.z) * r.m_invDir.z;
	real_t t6 = (m_max.z - r.m_origin.z) * r.m_invDir.z;

	real_t tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	real_t tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	if (tmax < 0 || tmin > tmax) return false;

	return true;
}

void slAabb::Center(slVec4& v) const
{
	v = slVec4(m_min + m_max);
	v *= 0.5f;
}

real_t slAabb::Radius() 
{
	return slMath::distance(m_min, m_max) * 0.5f;
}

void slAabb::Extent(slVec4& v) 
{
	v = slVec4(m_max - m_min);
}

bool slAabb::IsEmpty() const 
{
	return ((m_min == slVec4fFltMax) && (m_max == slVec4fFltMaxNeg))
		|| (m_min == m_max);
}

void slAabb::Reset() 
{
	m_min = slVec4fFltMax;
	m_max = slVec4fFltMaxNeg; 
}

