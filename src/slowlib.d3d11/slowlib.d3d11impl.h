/*
BSD 2-Clause License

Copyright (c) 2023, tyomaAni
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

#include "slowlib.base/geometry/slGeometry.h"
#include "slowlib.base/gs/slGS.h"
#include "slowlib.base/gs/slGPUMesh.h"
#include "slowlib.base/gs/slTexture.h"

#include "slowlib.d3d11.mesh.h"
#include "slowlib.d3d11.texture.h"
#include "shaders/slowlib.d3d11.shader.h"
#include "shaders/slowlib.d3d11.shader.Line3D.h"
#include "shaders/slowlib.d3d11.shader.Solid.h"
#include "shaders/slowlib.d3d11.shader.MainTarget.h"
#include "shaders/slowlib.d3d11.shader.GUIRectangle.h"
#include "shaders/slowlib.d3d11.shader.Sprite.h"

#define SLD3DSAFE_RELEASE(x) if(x){x->Release();x=0;}

class slGSD3D11 : public slGS
{
	friend class slD3D11ShaderLine3D;
	friend class slD3D11ShaderSolid;
	friend class slD3D11ShaderMainTarget;
	friend class slD3D11ShaderGUIRectangle;
	friend class slD3D11ShaderSprite;

	slWindow* m_activeWindow = 0;
	slVec2f* m_activeWindowSize = 0;
	
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
	
	ID3D11BlendState* m_currentBlendState = 0;

	// it for this: you have fullscreen window but you can use lower `resolution`
	// I need to use Render Target Texture for this
	slVec3f m_mainTargetSize;
	slColor m_clearColor;

	bool m_vsync = false;

	slD3D11ShaderSprite* m_shaderSprite = 0;
	slD3D11ShaderLine3D* m_shaderLine3D = 0;
	slD3D11ShaderSolid* m_shaderSolid = 0;
	slD3D11ShaderMainTarget* m_shaderMainTarget = 0;
	slD3D11ShaderGUIRectangle* m_shaderGUIRectangle = 0;
	slGSD3D11ShaderBase* m_activeShader = 0;

	bool createBackBuffer(float x, float y);
	bool updateMainTarget();


	slGSD3D11Mesh* m_currMesh = 0;
	slMaterial* m_currMaterial = 0;

	slGSD3D11Texture* m_mainTargetRTT = 0;
	slGSD3D11Texture* m_whiteTexture = 0;
	slGSD3D11Texture* m_GUIRTT = 0;
	
	void updateGUIMatrix();

public:
	slGSD3D11();
	virtual ~slGSD3D11();

	void SetActiveShader(slGSD3D11ShaderBase* shader);
	bool CreateShaders();

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
	virtual void DrawLine3D(const slVec3& p1, const slVec3& p2, const slColor& c) final;
	virtual slGPUMesh* SummonMesh(slMesh*) final;
	virtual void SetShader(slShaderType, uint32_t userIndex) final;
	virtual void SetMesh(slGPUMesh*) final;
	virtual void SetMaterial(slMaterial*) final;
	virtual void Draw() final;
	virtual slTexture* SummonTexture(slImage*, const slTextureInfo&) final;
	virtual void SetRenderTarget(slTexture*) final;
	virtual slTexture* SummonRenderTargetTexture(const slPoint& size, const slTextureInfo&) final;
	virtual void BeginGUI() final;
	virtual void EndGUI() final;
	virtual void DrawGUIRectangle(const slRect& rect, const slColor& color1, const slColor& color2,
		slTexture* t, slVec4f* UVs) final;
	virtual void DrawGUIRectangle(const slVec4f& rect, const slColor& color1, const slColor& color2,
		slTexture* t, slVec4f* UVs) final;
	virtual void DrawGUICharacter(char32_t, slGUIFont*, const slVec2f& position, const slColor&) final;
	virtual void DrawGUIText(const char32_t* text, uint32_t textSz, const slVec2f& position,
		slGUIDrawTextCallback*) final;
	virtual void DrawSprite(slSprite*) final;

	bool createShaders(
		const char* vertexTarget,
		const char* pixelTarget,
		const char* vertexShader,
		const char* pixelShader,
		const char* vertexEntryPoint,
		const char* pixelEntryPoint,
		slMeshVertexType vertexType,
		ID3D11VertexShader** vs,
		ID3D11PixelShader** ps,
		ID3D11InputLayout** il);
	bool createConstantBuffer(uint32_t byteSize, ID3D11Buffer**);
	bool createGeometryShaders(const char* target,
		const char* shaderText,
		const char* entryPoint,
		ID3D11GeometryShader** gs);
	HRESULT	createSamplerState(D3D11_FILTER filter,
		D3D11_TEXTURE_ADDRESS_MODE addressMode,
		uint32_t anisotropic_level,
		ID3D11SamplerState** samplerState,
		D3D11_COMPARISON_FUNC cmpFunc);
};

#endif
