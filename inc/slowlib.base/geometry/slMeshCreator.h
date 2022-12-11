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
#ifndef __SL_SLOWLIBBASEMESHCR_H__
#define __SL_SLOWLIBBASEMESHCR_H__

#include "slowlib.base/containers/slArray.h"

struct slPolygonCreatorVertex
{
	slVec3f Position;
	slVec2f UV;
	slVec3f Normal;
	slVec3f Binormal;
	slVec3f Tangent;
	slVec4f Color;

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
};

class SL_API slMeshCreator
{
public:
	slMeshCreator();
	~slMeshCreator();

	// Create and return slMesh
	slMesh* CreateMesh(slPolygonMesh*);
};

#endif
