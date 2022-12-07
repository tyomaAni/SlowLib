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
#ifndef _SL_D3D11IMPL_H_
#define _SL_D3D11IMPL_H_

#include <d3d11.h>

#include "slowlib.base/gs/slGS.h"

class slGSD3D11 : public slGS
{
	slWindow* m_activeWindow = 0;
	slPoint* m_activeWindowSize = 0;
	
	IDXGISwapChain* m_SwapChain = 0;
	ID3D11Device* m_d3d11Device = 0;
	ID3D11DeviceContext* m_d3d11DevCon = 0;
	ID3D11DepthStencilState* m_depthStencilStateEnabled = 0;
	ID3D11DepthStencilState* m_depthStencilStateDisabled = 0;
	ID3D11RasterizerState* m_RasterizerSolid = 0;
	ID3D11RasterizerState* m_RasterizerSolidNoBackFaceCulling = 0;
	ID3D11RasterizerState* m_RasterizerWireframeNoBackFaceCulling = 0;
	ID3D11RasterizerState* m_RasterizerWireframe = 0;
	ID3D11BlendState* m_blendStateAlphaEnabled = 0;
	ID3D11BlendState* m_blendStateAlphaDisabled = 0;
	ID3D11DepthStencilView* m_depthStencilView = 0;
	ID3D11Texture2D* m_depthStencilBuffer = 0;
	ID3D11RenderTargetView* m_MainTargetView = 0;
	ID3D11RenderTargetView* m_currentTargetView = 0;

	// it for this: you have fullscreen window but you can use lower `resolution`
	// I need to use Render Target Texture for this
	slVec3f m_mainTargetSize;
	slColor m_clearColor;

	bool m_vsync = false;

	bool createBackBuffer(float x, float y);
	bool updateMainTarget();
public:
	slGSD3D11();
	virtual ~slGSD3D11();

	virtual bool Init(slWindow*, const char* parameters) final;
	virtual void Shutdown() final;
	virtual bool InitWindow(slWindow*) final;
	virtual void SetActiveWindow(slWindow*) final;
	virtual slString GetName() final;
	virtual slString GetTextInfo() final;
	virtual slUID GetUID() final;
	virtual void UseVSync(bool) final;
	virtual bool UseDepth(bool) final;
	virtual bool UseBlend(bool) final;
	virtual void GetDepthRange(slVec3f*) final;
	virtual void SetClearColor(float r, float g, float b, float a) final;
	virtual void BeginDraw() final;
	virtual void ClearDepth() final;
	virtual void ClearColor() final;
	virtual void ClearAll() final;    // ClearDepth и ClearColor
	virtual void EndDraw() final;
	virtual void SwapBuffers() final;
	virtual void UpdateMainRenderTarget(const slVec3f&) final;
	virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) final;
	virtual void SetScissorRect(const slVec4f& rect, slVec4f* old) final;
};

#endif
