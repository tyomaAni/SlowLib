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
#ifndef __SL_SLOWLIBBASEPOLYMESH_H__
#define __SL_SLOWLIBBASEPOLYMESH_H__

#include "slowlib.base/containers/slList.h"
#include "slowlib.base/containers/slArray.h"
#include "slowlib.base/geometry/slMesh.h"
#include "slowlib.base/geometry/slPolyTriangle.h"

#include <map>
#include <string>

struct slPolygonCreatorVertex{
	slVertexTriangle baseData;

	bool weld = false;
};

struct slPolygonCreatorData
{
	slPolygonCreatorVertex curr;
	slArray<slPolygonCreatorVertex> array;
};

class SL_API slPolygonCreator
{
	slPolygonCreatorData m_data;
public:
	slPolygonCreator();
	~slPolygonCreator();

	// set vertex data using this
	void SetPosition(const slVec3f&);
	void SetNormal(const slVec3f&);
	void SetBinormal(const slVec3f&);
	void SetTangent(const slVec3f&);
	void SetColor(const slVec4f&);
	void SetUV(const slVec2f&);
	// or this
	void SetVertex(const slVertexTriangle&);
	// then call this

	void AddVertex(bool weld);
	uint32_t Size();

	slArray<slPolygonCreatorVertex>* GetArray() { return &m_data.array; }
};

class slPolyVertex
{
public:

	slPolyVertex() {}
	~slPolyVertex() {}

	slPolyVertex* m_left = 0;
	slPolyVertex* m_right = 0;

	slPolygonCreatorVertex m_data;

	uint32_t m_flags = 0;
	enum {
		flag_isSelected = 0x1,
		flag_User1 = 0x2,
		flag_User2 = 0x4,
	};

	bool IsSelected() { return (m_flags & flag_isSelected) == flag_isSelected; }
	void Select() { m_flags |= flag_isSelected; }
	void Deselect() { m_flags &= ~flag_isSelected; }

	void CopyData(slPolyVertex* other)
	{
		m_data = other->m_data;
		m_flags = other->m_flags;
	}


	slList<slPolygon*> m_polygons;
	slList<slPolyEdge*> m_edges;

	bool IsOnEdge();
};

class slPolyEdge
{
public:
	slPolyEdge() {}
	~slPolyEdge() {}

	uint32_t m_flags = 0;
	enum
	{
		flag_isSelected = 0x1,
		flag_User1 = 0x2,
	};

	bool IsSelected() { return (m_flags & flag_isSelected) == flag_isSelected; }
	void Select() { m_flags |= flag_isSelected; }
	void Deselect() { m_flags &= ~flag_isSelected; }

	void CopyData(slPolyEdge* other) {
		m_flags = other->m_flags;
	}

	slPolyEdge* m_left = 0;
	slPolyEdge* m_right = 0;

	slPolyVertex* m_vertex1 = 0;
	slPolyVertex* m_vertex2 = 0;

	slPolygon* m_polygon1 = 0;
	slPolygon* m_polygon2 = 0;
};

SL_FORCEINLINE
bool slPolyVertex::IsOnEdge()
{
	auto c = m_edges.m_head;
	auto l = c->m_left;
	while (true)
	{
		if (!c->m_data->m_polygon1)
			return true;

		if (!c->m_data->m_polygon2)
			return true;

		if (c == l)
			break;
		c = c->m_right;
	}
	return false;
}

struct slPolygonVertexData
{
	slPolygonVertexData() :m_vertex(0), m_flags(0) {}
	slPolygonVertexData(slPolyVertex* v) :m_vertex(v), m_flags(0) {}
	slPolygonVertexData(slPolyVertex* v, const slVec2f& uv, const slVec3f& normal, uint32_t flags)
		:
		m_vertex(v),
		m_flags(flags)
	{
		m_baseData.UV = uv;
		m_baseData.Normal = normal;
	}

	bool operator==(const slPolygonVertexData& other) {
		return m_vertex == other.m_vertex;
	}

	enum { flag_isSelected = 1, };

	slPolyVertex* m_vertex;
	
	// slPolyVertex is vertex for editor, and this vertex can contain many slMesh\GPU vertices
	// slPolygonVertexData have data for each unique slMesh\GPU vertex
	slVertexTriangle m_baseData;

	uint32_t m_flags;
};

class SL_API slPolygon
{
	void* m_data = 0; //hide all in .cpp
public:
	slPolygon();
	~slPolygon();

	slPolygon* m_left = 0;
	slPolygon* m_right = 0;

	uint32_t GetFlags();

	void SelectUVs();
	void DeselectUVs();
	slListNode<slPolygonVertexData>* FindVertex(slPolyVertex* v);

	slList<slPolygonVertexData>* GetVertices();
	slList<slPolyEdge*>* GetEdges();

	bool IsVisible();
	void CalculateNormal();

	slVec3f GetFaceNormal();
	slVec3f GetFaceNormalCalculateNew();

	void Flip();

	// not guaranteed but better than nothing
	void FixOrder(float lineLineCollisionLen);
};

struct slSkeleton {
	int maybe_one_day = 0;
};

// m_mesh = slCreate<slPolygonMesh>();
// slDestroy(m_mesh);
class SL_API slPolygonMesh
{
	void DeleteMesh();
	void DeleteEdges();
	// add\remove to\from list
	void listAddEdge(slPolyEdge* newEdge);
	void listAddVertex(slPolyVertex* newVertex);
	void listAddPolygon(slPolygon* newPolygon);
	void listRemoveEdge(slPolyEdge* o);
	void listRemoveVertex(slPolyVertex* o);
	void listRemovePolygon(slPolygon* o);
public:
	slPolygonMesh();
	~slPolygonMesh();

	slPolygon* m_first_polygon = 0;
	slPolyEdge* m_first_edge = 0;
	slPolyVertex* m_first_vertex = 0;
	slSkeleton* m_skeleton = 0;

	uint32_t m_vertexCount = 0;
	uint32_t m_edgeCount = 0;
	uint32_t m_polygonCount = 0;
	uint32_t m_uvCount = 0;
	
	slAabb m_aabb;

	std::map<std::string, slPolyVertex*> m_weldMap;

	void UpdateCounts();

	// must be for each polygon.
	// array contain first miUVChannel* for each channel
	//miArray<miUVChannelNode*> m_UVChannels;
	
	void AddPolygon(slPolygonCreator*, bool genNormals);
	slMesh* CreateMesh();

	void CreateEdges();
};

#endif
