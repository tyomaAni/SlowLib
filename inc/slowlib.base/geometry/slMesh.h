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
#ifndef __SL_SLOWLIBBASEMESH_H__
#define __SL_SLOWLIBBASEMESH_H__

enum class slMeshVertexType : uint32_t
{
	Null, // 0
	Triangle
};

enum class slMeshIndexType : uint32_t
{
	u16,
	u32
};

struct slVertexTriangle
{
	slVec3f Position;
	slVec2f UV;
	slVec3f Normal;
	slVec3f Binormal;
	slVec3f Tangent;
	slVec4f Color;
};

struct slMeshInfo
{
	slMeshVertexType m_vertexType = slMeshVertexType::Triangle;
	slMeshIndexType m_indexType = slMeshIndexType::u16;
	uint32_t m_vCount = 0;
	uint32_t m_iCount = 0;
	uint32_t m_stride = 0;
};

class SL_API slMesh
{
	void CalculateTangents(slVec3f& normal, slVec3f& tangent, slVec3f& binormal,
		const slVec3f& vt1, const slVec3f& vt2, const slVec3f& vt3, // vertices, in float
		const slVec2f& tc1, const slVec2f& tc2, const slVec2f& tc3); // texture coords
	void GenerateTangents_u16();
	void GenerateTangents_u32();
public:
	slMesh();
	~slMesh();

	slAabb m_aabb;

	uint8_t* m_vertices = nullptr;
	uint8_t* m_indices = nullptr;
	
	slMeshInfo m_info;

	void GenerateTangents();
};

#endif
