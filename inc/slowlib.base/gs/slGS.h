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
#ifndef __SL_SLOWLIBBASEGS_H__
#define __SL_SLOWLIBBASEGS_H__

#include "slowlib.base/common/slUserData.h"
#include "slowlib.base/common/slColor.h"
#include "slowlib.base/gs/slGPUMesh.h"

// Graphics System
class slGS : public slUserData
{
public:
	slGS() {}
	virtual ~slGS() {}

	// Init GPU things.
	// `parameters` is optional thing, it depends on library implementation.
	virtual bool Init(slWindow*, const char* parameters) = 0;
	virtual void Shutdown() = 0;

	// Init more window. For drawing in many windows.
	virtual bool InitWindow(slWindow*) = 0;
	// For drawing in many windows.
	virtual void SetActiveWindow(slWindow*) = 0;

	// Get name
	virtual slString GetName() = 0;
	// Get all other info in text.
	virtual slString GetTextInfo() = 0;
	// Get unique ID
	virtual slUID GetUID() = 0;

	virtual void UseVSync(bool) = 0;
	virtual bool UseDepth(bool) = 0; // return old
	virtual bool UseBlend(bool) = 0; // return old

	// use x and y
	virtual void GetDepthRange(slVec3f*) = 0;

	// Color for `canvas`
	virtual void SetClearColor(float r, float g, float b, float a) = 0;

	// Call before drawing
	virtual void BeginDraw() = 0;
	virtual void ClearDepth() = 0;
	virtual void ClearColor() = 0;
	virtual void ClearAll() = 0;    // ClearDepth и ClearColor
	// Call after drawing
	virtual void EndDraw() = 0;
	virtual void SwapBuffers() = 0;

	// Call this when window resized.
	// x and y
	virtual void UpdateMainRenderTarget(const slVec3f&) = 0;

	virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
	virtual void SetScissorRect(const slVec4f& rect, slVec4f* old) = 0;

	virtual void DrawLine3D(const slVec3& p1, const slVec3& p2, const slColor& c) = 0;

	virtual slGPUMesh* SummonMesh(slMesh*) = 0;

	virtual void SetMesh(slGPUMesh*) = 0;
	virtual void SetMaterial(slMaterial*) = 0;
	virtual void Draw() = 0;

	virtual slTexture* SummonTexture(slImage*, const slTextureInfo&) = 0;

	virtual void SetRenderTarget(slTexture*) = 0;
	virtual slTexture* SummonRenderTargetTexture(const slPoint& size, const slTextureInfo&) = 0;

	// GUI drawing must be outside BeginDraw EndDraw
	virtual void BeginGUI() = 0;
	virtual void DrawGUI() = 0;
	// t and UVs is optional
	virtual void DrawGUIRectangle(const slRect& rect, const slColor& color1, const slColor& color2,
		slTexture* t, slVec4f* UVs) = 0;
	virtual void EndGUI() = 0;
};

#endif
