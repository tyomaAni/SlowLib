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

slPolygonCreator::slPolygonCreator()
{
}

slPolygonCreator::~slPolygonCreator()
{
}

void slPolygonCreator::SetPosition(const slVec3f& v)
{
	m_data.curr.baseData.Position = v;
}

void slPolygonCreator::SetNormal(const slVec3f& v)
{
	m_data.curr.baseData.Normal = v;
}

void slPolygonCreator::SetBinormal(const slVec3f& v)
{
	m_data.curr.baseData.Binormal = v;
}

void slPolygonCreator::SetTangent(const slVec3f& v)
{
	m_data.curr.baseData.Tangent = v;
}

void slPolygonCreator::SetColor(const slVec4f& v)
{
	m_data.curr.baseData.Color = v;
}

void slPolygonCreator::SetUV(const slVec2f& v)
{
	m_data.curr.baseData.UV = v;
}

void slPolygonCreator::SetVertex(const slVertexTriangle& v)
{
	m_data.curr.baseData.Binormal = v.Binormal;
	m_data.curr.baseData.Color = v.Color;
	m_data.curr.baseData.Normal = v.Normal;
	m_data.curr.baseData.Position = v.Position;
	m_data.curr.baseData.Tangent = v.Tangent;
	m_data.curr.baseData.UV = v.UV;
}

void slPolygonCreator::AddVertex(bool weld)
{
	m_data.curr.weld = weld;
	m_data.array.push_back(m_data.curr);
}

uint32_t slPolygonCreator::Size()
{
	return m_data.array.size();
}


