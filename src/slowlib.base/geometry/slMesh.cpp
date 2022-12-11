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
#include "slowlib.base/geometry/slGeometry.h"

slMesh::slMesh(){}
slMesh::~slMesh()
{
	if (m_vertices) slMemory::free(m_vertices);
	if (m_indices) slMemory::free(m_indices);
}

void slMesh::CalculateTangents(slVec3f& normal, slVec3f& tangent, slVec3f& binormal,
	const slVec3f& vt1, const slVec3f& vt2, const slVec3f& vt3, // vertices, in float
	const slVec2f& tc1, const slVec2f& tc2, const slVec2f& tc3) // texture coords
{
	slVec3f e1 = vt2 - vt1;
	slVec3f e2 = vt3 - vt1;
	e1.cross(e2, normal);
	normal.normalize();

	float deltaX1 = tc1.x - tc2.x;
	float deltaX2 = tc3.x - tc1.x;
	binormal = (e1 * deltaX2) - (e2 * deltaX1);
	binormal.normalize();

	float deltaY1 = tc1.y - tc2.y;
	float deltaY2 = tc3.y - tc1.y;
	tangent = (e1 * deltaY2) - (e2 * deltaY1);
	tangent.normalize();

	slVec3f txb;
	tangent.cross(binormal, txb);
	if (txb.dot(normal) < static_cast<real_t>(0.0))
	{
		tangent *= static_cast<real_t>(-1.0);
		binormal *= static_cast<real_t>(-1.0);
	}
}

void slMesh::GenerateTangents_u16()
{
	uint16_t* idx = (uint16_t*)m_indices;
	slVertexTriangle* v = (slVertexTriangle*)m_vertices;

	slVec3f localNormal;
	for (uint32_t i = 0; i < m_info.m_iCount; i += 3)
	{
		CalculateTangents(
			localNormal,
			v[idx[i + 0]].Tangent,
			v[idx[i + 0]].Binormal,
			v[idx[i + 0]].Position,
			v[idx[i + 1]].Position,
			v[idx[i + 2]].Position,
			v[idx[i + 0]].UV,
			v[idx[i + 1]].UV,
			v[idx[i + 2]].UV);
		//if (recalculateNormals)
		//v[idx[i+0]].Normal=localNormal;

		CalculateTangents(
			localNormal,
			v[idx[i + 1]].Tangent,
			v[idx[i + 1]].Binormal,
			v[idx[i + 1]].Position,
			v[idx[i + 2]].Position,
			v[idx[i + 0]].Position,
			v[idx[i + 1]].UV,
			v[idx[i + 2]].UV,
			v[idx[i + 0]].UV);
		//if (recalculateNormals)
		//v[idx[i+1]].Normal=localNormal;

		CalculateTangents(
			localNormal,
			v[idx[i + 2]].Tangent,
			v[idx[i + 2]].Binormal,
			v[idx[i + 2]].Position,
			v[idx[i + 0]].Position,
			v[idx[i + 1]].Position,
			v[idx[i + 2]].UV,
			v[idx[i + 0]].UV,
			v[idx[i + 1]].UV);
		//if (recalculateNormals)
		//v[idx[i+2]].Normal=localNormal;
	}
}

void slMesh::GenerateTangents_u32()
{
	uint32_t* idx = (uint32_t*)m_indices;
	slVertexTriangle* v = (slVertexTriangle*)m_vertices;

	slVec3f localNormal;
	for (uint32_t i = 0; i < m_info.m_iCount; i += 3)
	{
		CalculateTangents(
			localNormal,
			v[idx[i + 0]].Tangent,
			v[idx[i + 0]].Binormal,
			v[idx[i + 0]].Position,
			v[idx[i + 1]].Position,
			v[idx[i + 2]].Position,
			v[idx[i + 0]].UV,
			v[idx[i + 1]].UV,
			v[idx[i + 2]].UV);
		//if (recalculateNormals)
		//v[idx[i+0]].Normal=localNormal;

		CalculateTangents(
			localNormal,
			v[idx[i + 1]].Tangent,
			v[idx[i + 1]].Binormal,
			v[idx[i + 1]].Position,
			v[idx[i + 2]].Position,
			v[idx[i + 0]].Position,
			v[idx[i + 1]].UV,
			v[idx[i + 2]].UV,
			v[idx[i + 0]].UV);
		//if (recalculateNormals)
		//v[idx[i+1]].Normal=localNormal;

		CalculateTangents(
			localNormal,
			v[idx[i + 2]].Tangent,
			v[idx[i + 2]].Binormal,
			v[idx[i + 2]].Position,
			v[idx[i + 0]].Position,
			v[idx[i + 1]].Position,
			v[idx[i + 2]].UV,
			v[idx[i + 0]].UV,
			v[idx[i + 1]].UV);
		//if (recalculateNormals)
		//v[idx[i+2]].Normal=localNormal;
	}
}

void slMesh::GenerateTangents()
{
	if (m_info.m_vertexType != slMeshVertexType::Triangle)
		return;

	switch (m_info.m_indexType)
	{
	case slMeshIndexType::u16:
	default:
		GenerateTangents_u16();
		break;
	case slMeshIndexType::u32:
		GenerateTangents_u32();
		break;
	}
}


