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

slPolygonMesh::slPolygonMesh() {}
slPolygonMesh::~slPolygonMesh() 
{
	DeleteMesh();
}

void slPolygonMesh::UpdateCounts()
{
	m_vertexCount = 0;
	m_edgeCount = 0;
	m_polygonCount = 0;
	m_uvCount = 0;
	{
		auto c = m_first_vertex;
		if (c)
		{
			auto l = c->m_left;
			while (true)
			{
				++m_vertexCount;
				if (c == l)
					break;
				c = c->m_right;
			}
		}
	}
	{
		auto c = m_first_edge;
		if (c)
		{
			auto l = c->m_left;
			while (true)
			{
				++m_edgeCount;
				if (c == l)
					break;
				c = c->m_right;
			}
		}
	}
	{
		auto c = m_first_polygon;
		if (c)
		{
			auto l = c->m_left;
			while (true)
			{
				++m_polygonCount;
				auto cv = c->GetVertices()->m_head;
				auto lv = cv->m_left;
				while (true)
				{
					++m_uvCount;
					if (cv == lv)
						break;
					cv = cv->m_right;
				}
				if (c == l)
					break;
				c = c->m_right;
			}
		}
	}
}

void slPolygonMesh::CreateEdges()
{
	if (m_first_edge)
		DeleteEdges();
	if (!m_first_polygon)
		return;

	auto current_polygon = m_first_polygon;
	auto last_polygon = current_polygon->m_left;
	while (true) {
		auto next_polygon = current_polygon->m_right;

		auto current_vertex = current_polygon->GetVertices()->m_head;
		auto next_vertex = current_vertex->m_right;
		auto last_vertex = current_vertex->m_left;
		while (true) {
			slPolyVertex* v1 = current_vertex->m_data.m_vertex;
			slPolyVertex* v2 = next_vertex->m_data.m_vertex;

			// пусть вершина с адресом значение которого меньше
			//  будет на первом месте.
			if (v2 < v1)
			{
				v1 = next_vertex->m_data.m_vertex;
				v2 = current_vertex->m_data.m_vertex;
			}

			slPolyEdge* newEdge = nullptr;
			if (v1->m_edges.m_head) // find exist edge
			{
				// я ищу рёбра в списке у вершины, так как v1 и v2
				//  отсортированы по значению адреса, и v1 всегда
				//   будет иметь возможность хранить уже ранее 
				//    созданное ребро.
				auto current_edge = v1->m_edges.m_head;
				auto last_edge = current_edge->m_left;
				while (true) {

					if (current_edge->m_data->m_vertex1 == v1
						&& current_edge->m_data->m_vertex2 == v2)
					{
						newEdge = current_edge->m_data;
						break;
					}

					if (current_edge == last_edge)
						break;
					current_edge = current_edge->m_right;
				}
			}

			if (!newEdge)
			{
				newEdge = slCreate<slPolyEdge>();

				newEdge->m_vertex1 = v1;
				newEdge->m_vertex2 = v2;

				v1->m_edges.push_back(newEdge);
				v2->m_edges.push_back(newEdge);

				/*{
					auto n1 = v1->m_edges.find(newEdge);
					auto n2 = v2->m_edges.find(newEdge);
					printf("E[%u] N1[%u] N2[%u]\n", (u32)newEdge, (u32)n1, (u32)n2);
				}*/

				listAddEdge(newEdge);
			}

			if (!newEdge->m_polygon1)
				newEdge->m_polygon1 = current_polygon;
			else if (!newEdge->m_polygon2)
				newEdge->m_polygon2 = current_polygon;
			if (newEdge->m_polygon1 == newEdge->m_polygon2)
				newEdge->m_polygon2 = 0;

			current_polygon->GetEdges()->push_back(newEdge);


			if (current_vertex == last_vertex)
				break;

			current_vertex = next_vertex;
			next_vertex = next_vertex->m_right;
		}


		if (current_polygon == last_polygon)
			break;
		current_polygon = next_polygon;
	}

	// Try to sort edges in vertices
	{
		slArray<slPolyEdge*> sortedEdges;
		slArray<slPolyEdge*> sortedEdges2;

		auto current_vertex = m_first_vertex;
		auto last_vertex = current_vertex->m_left;
		while (true)
		{
			sortedEdges.clear();
			sortedEdges2.clear();

			uint32_t count = 0;
			auto current_edge = current_vertex->m_edges.m_head;
			if (current_edge)
			{

				auto last_edge = current_edge->m_left;
				while (true)
				{

					if (current_edge->m_data->m_flags & slPolyEdge::flag_User1)
						current_edge->m_data->m_flags ^= slPolyEdge::flag_User1;

					sortedEdges.push_back(current_edge->m_data);

					if (current_edge == last_edge)
						break;
					current_edge = current_edge->m_right;
				}

				slPolyEdge* lastEdge = 0;
				for (uint32_t i = 0, sz = sortedEdges.size(); i < sz; ++i)
				{
					if (i == 0)
					{
						sortedEdges2.push_back(sortedEdges.m_data[i]);
						sortedEdges.m_data[i]->m_flags |= slPolyEdge::flag_User1;
						lastEdge = sortedEdges.m_data[i];
					}
					else
					{
						slPolyEdge* thatEdge = 0;
						for (uint32_t i2 = 1, sz2 = sortedEdges.size(); i2 < sz2; ++i2)
						{
							auto e = sortedEdges.m_data[i2];
							if ((e->m_flags & slPolyEdge::flag_User1) == 0)
							{
								if (lastEdge->m_polygon1) {
									if (e->m_polygon1) {
										if (e->m_polygon1 == lastEdge->m_polygon1) {
											thatEdge = e;
											break;
										}
									}
									if (e->m_polygon2) {
										if (e->m_polygon2 == lastEdge->m_polygon1) {
											thatEdge = e;
											break;
										}
									}
								}
								if (lastEdge->m_polygon2) {
									if (e->m_polygon1) {
										if (e->m_polygon1 == lastEdge->m_polygon2) {
											thatEdge = e;
											break;
										}
									}
									if (e->m_polygon2) {
										if (e->m_polygon2 == lastEdge->m_polygon2) {
											thatEdge = e;
											break;
										}
									}
								}
							}
						}
						if (thatEdge)
						{
							sortedEdges2.push_back(thatEdge);
							thatEdge->m_flags |= slPolyEdge::flag_User1;
							lastEdge = thatEdge;
						}
						else
						{
							sortedEdges2.push_back(sortedEdges.m_data[i]);
							sortedEdges.m_data[i]->m_flags |= slPolyEdge::flag_User1;
							lastEdge = sortedEdges.m_data[i];
						}
					}
				}

				current_edge = current_vertex->m_edges.m_head;
				for (size_t i = 0, sz = sortedEdges2.size(); i < sz; ++i)
				{
					current_edge->m_data = sortedEdges2.m_data[i];
					current_edge = current_edge->m_right;
				}
			}

			if (current_vertex == last_vertex)
				break;
			current_vertex = current_vertex->m_right;
		}
	}
}

void slPolygonMesh::DeleteMesh()
{
	DeleteEdges();
	if (m_first_polygon)
	{
		auto p = m_first_polygon;
		auto last = p->m_left;
		while (true) {
			auto next = p->m_right;
			slDestroy(p);
			if (p == last)
				break;
			p = next;
		}
	}


	if (m_first_vertex)
	{
		auto v = m_first_vertex;
		auto last = v->m_left;
		while (true) {
			auto next = v->m_right;
			slDestroy(v);
			if (v == last)
				break;
			v = next;
		}
	}
}

void slPolygonMesh::DeleteEdges()
{
	if (m_first_polygon)
	{
		auto p = m_first_polygon;
		auto last = p->m_left;
		while (true) {
			auto next = p->m_right;
			p->GetEdges()->clear();
			if (p == last)
				break;
			p = next;
		}
	}
	if (m_first_vertex)
	{
		auto v = m_first_vertex;
		auto last = v->m_left;
		while (true) {
			auto next = v->m_right;
			v->m_edges.clear();
			if (v == last)
				break;
			v = next;
		}
	}
	if (m_first_edge)
	{
		auto e = m_first_edge;
		auto last = e->m_left;
		while (true) {
			auto next = e->m_right;
			slDestroy(e);

			if (e == last)
				break;
			e = next;
		}
		m_first_edge = 0;
	}
}

void slPolygonMesh::listAddEdge(slPolyEdge* newEdge)
{
	if (!m_first_edge)
	{
		m_first_edge = newEdge;
		m_first_edge->m_right = m_first_edge;
		m_first_edge->m_left = m_first_edge;
	}
	else
	{
		auto last = m_first_edge->m_left;
		last->m_right = newEdge;
		newEdge->m_left = last;
		newEdge->m_right = m_first_edge;
		m_first_edge->m_left = newEdge;
	}
}

void slPolygonMesh::listAddVertex(slPolyVertex* newVertex) {
	if (!m_first_vertex)
	{
		m_first_vertex = newVertex;
		m_first_vertex->m_right = m_first_vertex;
		m_first_vertex->m_left = m_first_vertex;
	}
	else
	{
		auto last = m_first_vertex->m_left;
		last->m_right = newVertex;
		newVertex->m_left = last;
		newVertex->m_right = m_first_vertex;
		m_first_vertex->m_left = newVertex;
	}
}

void slPolygonMesh::listAddPolygon(slPolygon* newPolygon)
{
	if (!m_first_polygon)
	{
		m_first_polygon = newPolygon;
		m_first_polygon->m_right = m_first_polygon;
		m_first_polygon->m_left = m_first_polygon;
	}
	else
	{
		auto last = m_first_polygon->m_left;
		last->m_right = newPolygon;
		newPolygon->m_left = last;
		newPolygon->m_right = m_first_polygon;
		m_first_polygon->m_left = newPolygon;
	}
}

void slPolygonMesh::listRemoveEdge(slPolyEdge* o)
{
	auto l = o->m_left;
	auto r = o->m_right;
	l->m_right = r;
	r->m_left = l;

	if (o == m_first_edge)
		m_first_edge = o->m_right;
	if (o == m_first_edge)
		m_first_edge = 0;
}

void slPolygonMesh::listRemoveVertex(slPolyVertex* o)
{
	auto l = o->m_left;
	auto r = o->m_right;
	l->m_right = r;
	r->m_left = l;

	if (o == m_first_vertex)
		m_first_vertex = o->m_right;
	if (o == m_first_vertex)
		m_first_vertex = 0;
}

void slPolygonMesh::listRemovePolygon(slPolygon* o)
{
	auto l = o->m_left;
	auto r = o->m_right;
	l->m_right = r;
	r->m_left = l;

	if (o == m_first_polygon)
		m_first_polygon = o->m_right;
	if (o == m_first_polygon)
		m_first_polygon = 0;
}

void slPolygonMesh::AddPolygon(slPolygonCreator* pc, bool genNormals)
{
	auto polygonVertexCount = pc->Size();
	if (polygonVertexCount < 3)
		return;

	slPolygon* newPolygon = slCreate<slPolygon>();

	if (!m_first_polygon)
	{
		m_first_polygon = newPolygon;
		m_first_polygon->m_right = m_first_polygon;
		m_first_polygon->m_left = m_first_polygon;
	}
	else
	{
		auto last = m_first_polygon->m_left;
		last->m_right = newPolygon;
		newPolygon->m_left = last;
		newPolygon->m_right = m_first_polygon;
		m_first_polygon->m_left = newPolygon;
	}

	auto verts = pc->GetArray();

	for (uint32_t i = 0; i < polygonVertexCount; ++i)
	{
		slPolyVertex* newVertex = 0;
		m_aabb.Add(verts->m_data[i].baseData.Position);

		newVertex = slCreate<slPolyVertex>();
		newVertex->m_data = verts->m_data[i];

		listAddVertex(newVertex);

		/*if (positions[i].m_second & miPolygonCreator::flag_selected)
			newVertex->m_flags |= miVertex::flag_isSelected;*/

		// add newPolygon to vertex and newVertex to polygon
		// only once
		if (newVertex->m_polygons.find(newPolygon) == 0)
		{
			newVertex->m_polygons.push_back(newPolygon);
			slPolygonVertexData newVD;
			newVD.m_vertex = newVertex;
			newVD.m_baseData = verts->m_data[i].baseData;
			newPolygon->GetVertices()->push_back(newVD);
		}
	}

	if (genNormals)
		newPolygon->CalculateNormal();
}

slMesh* slPolygonMesh::CreateMesh()
{
	UpdateCounts();
	if (m_polygonCount)
	{
		slMesh* newMesh = slCreate<slMesh>();

		newMesh->m_info.m_stride = sizeof(slVertexTriangle);

		uint32_t index = 0;

		slArray<slVertexTriangle> vertArr;
		slArray<uint32_t> indsArr;

		vertArr.reserve(5000);
		indsArr.reserve(5000);

		slVertexTriangle currVert;

		auto current_polygon = m_first_polygon;
		auto last_polygon = current_polygon->m_left;
		while (true)
		{

			auto vertex_1 = current_polygon->GetVertices()->m_head;
			auto vertex_3 = vertex_1->m_right;
			auto vertex_2 = vertex_3->m_right;
			while (true)
			{
				currVert = vertex_1->m_data.m_baseData;
				vertArr.push_back(currVert);
				indsArr.push_back(index);

				++index;

				currVert = vertex_2->m_data.m_baseData;
				vertArr.push_back(currVert);
				indsArr.push_back(index);

				++index;

				currVert = vertex_3->m_data.m_baseData;
				vertArr.push_back(currVert);
				indsArr.push_back(index);

				++index;

				newMesh->m_info.m_vCount += 3;
				newMesh->m_info.m_iCount += 3;

				vertex_2 = vertex_2->m_right;
				vertex_3 = vertex_3->m_right;

				if (vertex_2 == vertex_1)
					break;
			}

			if (current_polygon == last_polygon)
				break;
			current_polygon = current_polygon->m_right;
		}

		newMesh->m_vertices = (uint8_t*)vertArr.m_data;
		newMesh->m_indices = (uint8_t*)indsArr.m_data;

		/*for (size_t i = 0; i < vertArr.m_size; ++i)
		{
			printf("P: %f %f %f\n", vertArr.m_data[i].Position.x, vertArr.m_data[i].Position.y, vertArr.m_data[i].Position.z);
		}
		for (size_t i = 0; i < indsArr.m_size; ++i)
		{
			printf("I: %i\n", indsArr.m_data[i]);
		}*/

		if (newMesh->m_info.m_iCount < 0xFFFF)
		{
			newMesh->m_info.m_indexType = slMeshIndexType::u16;

			uint32_t* oldIndices = (uint32_t*)newMesh->m_indices;

			newMesh->m_indices = (uint8_t*)slMemory::malloc(newMesh->m_info.m_iCount * sizeof(uint16_t));
			uint16_t* i16 = (uint16_t*)newMesh->m_indices;

			for (uint32_t i = 0; i < newMesh->m_info.m_iCount; ++i)
			{
				*i16 = *oldIndices;
				++oldIndices;
				++i16;
			}

			indsArr.free_memory();
		}

		vertArr.m_data = 0;
		indsArr.m_data = 0;

		newMesh->m_aabb = m_aabb;

		return newMesh;
	}
	return 0;
}