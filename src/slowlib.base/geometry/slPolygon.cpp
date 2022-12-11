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
#include "slowlib.base/geometry/slPolyMesh.h"
#include "slowlib.base/containers/slArray.h"
#include "slowlib.base/containers/slList.h"
#include "slowlib.base/containers/slPair.h"

struct slPolygonData
{
	uint32_t m_flags = 0;
	enum {
		flag_isSelected = 0x1,
		flag_User1 = 0x2,
	};

	void CopyData(slPolygon* other) 
	{
		m_flags = other->GetFlags();
	}

	slList<slPolygonVertexData> m_verts;

	slList<slPolyEdge*> m_edges;

	bool IsSelected() { return (m_flags & flag_isSelected) == flag_isSelected; }
	void Select() { m_flags |= flag_isSelected; }
	void Deselect() { m_flags &= ~flag_isSelected; }
};

slPolygon::slPolygon() 
{
	m_data = slCreate<slPolygonData>();
}

slPolygon::~slPolygon() 
{
	if (m_data)
		slDestroy((slPolygonData*)m_data);
}

slList<slPolyEdge*>* slPolygon::GetEdges()
{
	slPolygonData* data = (slPolygonData*)m_data;
	return &data->m_edges;
}

slList<slPolygonVertexData>* slPolygon::GetVertices()
{
	slPolygonData* data = (slPolygonData*)m_data;
	return &data->m_verts;
}

uint32_t slPolygon::GetFlags() 
{
	slPolygonData* data = (slPolygonData*)m_data;
	return data->m_flags;
}

void slPolygon::SelectUVs()
{
	slPolygonData* data = (slPolygonData*)m_data;

	auto curV = data->m_verts.m_head;
	auto lastV = curV->m_left;
	while (true)
	{
		curV->m_data.m_flags |= slPolygonVertexData::flag_isSelected;
		if (curV == lastV)
			break;
		curV = curV->m_right;
	}
}

void slPolygon::DeselectUVs()
{
	slPolygonData* data = (slPolygonData*)m_data;

	auto curV = data->m_verts.m_head;
	auto lastV = curV->m_left;
	while (true)
	{
		curV->m_data.m_flags &= ~slPolygonVertexData::flag_isSelected;
		if (curV == lastV)
			break;
		curV = curV->m_right;
	}
}

slListNode<slPolygonVertexData>* slPolygon::FindVertex(slPolyVertex* v)
{
	slPolygonData* data = (slPolygonData*)m_data;

	auto curV = data->m_verts.m_head;
	auto lastV = curV->m_left;
	while (true)
	{
		if (curV->m_data.m_vertex == v)
			break;
		if (curV == lastV)
			break;
		curV = curV->m_right;
	}
	return curV;
}

bool slPolygon::IsVisible()
{
	slPolygonData* data = (slPolygonData*)m_data;

	auto vertex_1 = data->m_verts.m_head;
	auto vertex_3 = vertex_1->m_right;
	auto vertex_2 = vertex_3->m_right;
	while (true)
	{
		auto a = vertex_2->m_data.m_vertex->m_position - vertex_1->m_data.m_vertex->m_position;
		auto b = vertex_3->m_data.m_vertex->m_position - vertex_1->m_data.m_vertex->m_position;

		slVec3f n;
		a.cross(b, n);

		float area = 0.5f * sqrt(n.dot());

		if (area > 0.001)
			return true;

		vertex_2 = vertex_2->m_right;
		vertex_3 = vertex_3->m_right;
		if (vertex_3 == vertex_1)
			break;
	}
	return false;
}

void slPolygon::CalculateNormal()
{
	slPolygonData* data = (slPolygonData*)m_data;
	{
		auto vertex_1 = data->m_verts.m_head;
		vertex_1->m_data.m_normal.set(0.f);

		auto vertex_3 = vertex_1->m_right;
		auto vertex_2 = vertex_3->m_right;
		while (true) 
		{
			vertex_2->m_data.m_normal.set(0.f);
			vertex_3->m_data.m_normal.set(0.f);
			// ===============================
			vertex_2 = vertex_2->m_right;
			vertex_3 = vertex_3->m_right;

			if (vertex_2 == vertex_1)
				break;
		}
	}
	{
		auto vertex_1 = data->m_verts.m_head;
		auto vertex_3 = vertex_1->m_right;
		auto vertex_2 = vertex_3->m_right;
		while (true) {
			auto e1 = vertex_2->m_data.m_vertex->m_position - vertex_1->m_data.m_vertex->m_position;
			auto e2 = vertex_3->m_data.m_vertex->m_position - vertex_1->m_data.m_vertex->m_position;
			slVec3f n;
			e1.cross(e2, n);
			vertex_1->m_data.m_normal -= n;
			vertex_2->m_data.m_normal -= n;
			vertex_3->m_data.m_normal -= n;

			// ===============================
			vertex_2 = vertex_2->m_right;
			vertex_3 = vertex_3->m_right;

			if (vertex_2 == vertex_1)
				break;
		}
	}
	{
		auto vertex_1 = data->m_verts.m_head;
		vertex_1->m_data.m_normal.normalize();

		auto vertex_3 = vertex_1->m_right;
		auto vertex_2 = vertex_3->m_right;
		while (true) {
			vertex_2->m_data.m_normal.normalize();
			vertex_3->m_data.m_normal.normalize();
			// ===============================
			vertex_2 = vertex_2->m_right;
			vertex_3 = vertex_3->m_right;

			if (vertex_2 == vertex_1)
				break;
		}
	}
}

slVec3f slPolygon::GetFaceNormal()
{
	slPolygonData* data = (slPolygonData*)m_data;
	slVec3f n;
	auto curV = data->m_verts.m_head;
	auto lastV = curV->m_left;
	while (true)
	{
		n += curV->m_data.m_normal;
		if (curV == lastV)
			break;
		curV = curV->m_right;
	}
	n.normalize();
	return n;
}

slVec3f slPolygon::GetFaceNormalCalculateNew()
{
	slPolygonData* data = (slPolygonData*)m_data;
	slVec3f _n;
	auto vertex_1 = data->m_verts.m_head;
	auto vertex_3 = vertex_1->m_right;
	auto vertex_2 = vertex_3->m_right;
	while (true) {
		auto e1 = vertex_2->m_data.m_vertex->m_position - vertex_1->m_data.m_vertex->m_position;
		auto e2 = vertex_3->m_data.m_vertex->m_position - vertex_1->m_data.m_vertex->m_position;
		slVec3f n;
		e1.cross(e2, n);
		_n -= n;

		// ===============================
		vertex_2 = vertex_2->m_right;
		vertex_3 = vertex_3->m_right;

		if (vertex_2 == vertex_1)
			break;
	}
	_n.normalize();
	return _n;
}

void slPolygon::Flip() {
	slPolygonData* data = (slPolygonData*)m_data;
	data->m_verts.reverse();
}

void slPolygon::FixOrder(float lineLineCollisionLen)
{
	slPolygonData* data = (slPolygonData*)m_data;

	// only if 4 or more vertices
	uint32_t num = 0;
	auto curV = data->m_verts.m_head;
	auto lastV = curV->m_left;
	while (true)
	{
		++num;
		if (curV == lastV)
			break;
		curV = curV->m_right;
	}
	if (num < 4)
		return;

	for (uint32_t i = 0; i < num * 4; ++i)
	{
		curV = data->m_verts.m_head;
		lastV = curV->m_left;
		while (true)
		{
			slPair<slListNode<slPolygonVertexData>*, slListNode<slPolygonVertexData>*> current;
			slPair<slListNode<slPolygonVertexData>*, slListNode<slPolygonVertexData>*> prev;

			current.m_first = curV;
			current.m_second = current.m_first->m_right;

			prev.m_second = current.m_first;
			prev.m_first = prev.m_second->m_left;

			auto tgCurr = current.m_second->m_right;
			auto tgLast = tgCurr->m_left;
			while (true)
			{
				slPair<slListNode<slPolygonVertexData>*, slListNode<slPolygonVertexData>*> target;
				target.m_first = tgCurr;
				target.m_second = tgCurr->m_right;

				if (target.m_first == prev.m_first)
					break;

				/*miRay r;
				r.m_origin = current.m_first->m_data.m_vertex->m_position;
				r.m_end = current.m_second->m_data.m_vertex->m_position;

				f32 d = r.distanceToLine(target.m_first->m_data.m_vertex->m_position, target.m_second->m_data.m_vertex->m_position);
				if (d < lineLineCollisionLen)*/
				slVec4f normal;
				slPolyTriangle tri1;
				slPolyTriangle tri2;
				{
					slPolyTriangle tri3;
					auto vertex = current.m_first;

					tri3.v1 = vertex->m_data.m_vertex->m_position;
					vertex = vertex->m_right;

					tri3.v2 = vertex->m_data.m_vertex->m_position;
					vertex = vertex->m_right;

					tri3.v3 = vertex->m_data.m_vertex->m_position;

					tri3.update();
					tri3.e1.cross(tri3.e2, normal);
					normal.normalize();
				}

				tri1.v1 = current.m_first->m_data.m_vertex->m_position;
				tri1.v2 = current.m_first->m_right->m_data.m_vertex->m_position;
				tri1.v3 = current.m_first->m_data.m_vertex->m_position;

				tri2.v1 = current.m_first->m_data.m_vertex->m_position;
				tri2.v2 = current.m_first->m_right->m_data.m_vertex->m_position;
				tri2.v3 = current.m_first->m_right->m_data.m_vertex->m_position;

				//ip = m_origin + t * m_direction;
				tri1.v1 = tri1.v1 + 1.f * normal;
				tri1.v2 = tri1.v2 + 1.f * normal;
				tri1.v3 = tri1.v3 + 1.f * -normal;

				tri2.v1 = tri2.v1 + 1.f * -normal;
				tri2.v2 = tri2.v2 + 1.f * -normal;
				tri2.v3 = tri2.v3 + 1.f * normal;

				//{ // scale
				//	f32 sz = 100.f;
				//	v4f center;
				//	tri.center(center);

				//	tri.v1 = ((tri.v1 - center) * sz) + center;
				//	tri.v2 = ((tri.v2 - center) * sz) + center;
				//	tri.v3 = ((tri.v3 - center) * sz) + center;
				//}
				tri1.update();
				tri2.update();

				slRay r;
				r.m_origin = target.m_first->m_data.m_vertex->m_position;
				r.m_end = target.m_second->m_data.m_vertex->m_position;
				r.Update();

				real_t T = 0.f;
				real_t U = 0.f;
				real_t V = 0.f;
				real_t W = 0.f;

				bool good = tri1.rayTest_MT(r, true, T, U, V, W);

				if (!good)
					good = tri2.rayTest_MT(r, true, T, U, V, W);

				if (good)
				{
					auto tmp = target.m_first->m_data.m_vertex;
					target.m_first->m_data.m_vertex = current.m_second->m_data.m_vertex;
					current.m_second->m_data.m_vertex = tmp;
					goto end;
				}

				if (tgCurr == tgLast)
					break;

				tgCurr = tgCurr->m_right;
			}


			if (curV == lastV)
				break;
			curV = curV->m_right;
		}
	end:;
	}
}
