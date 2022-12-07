﻿/*
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

#include "slowlib.d3d11impl.h"
#include "slowlib.base/system/slWindowWin32.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

// {0FE89B14-923C-4F2E-BA22-B18694D6F1ED}
slDEFINE_UID(g_uid_d3d11, 0xfe89b14, 0x923c, 0x4f2e, 0xba22, 0xb1, 0x86, 0x94, 0xd6, 0xf1, 0xed);

#define SLD3DSAFE_RELEASE(x) if(x){x->Release();x=0;}

slGSD3D11::slGSD3D11()
{
	
}

slGSD3D11::~slGSD3D11()
{
	Shutdown();
}

void slGSD3D11::Shutdown()
{
	SLD3DSAFE_RELEASE(m_depthStencilView);
	SLD3DSAFE_RELEASE(m_blendStateAlphaDisabled);
	SLD3DSAFE_RELEASE(m_blendStateAlphaEnabled);
	SLD3DSAFE_RELEASE(m_RasterizerWireframeNoBackFaceCulling);
	SLD3DSAFE_RELEASE(m_RasterizerWireframe);
	SLD3DSAFE_RELEASE(m_RasterizerSolidNoBackFaceCulling);
	SLD3DSAFE_RELEASE(m_RasterizerSolid);
	SLD3DSAFE_RELEASE(m_depthStencilStateDisabled);
	SLD3DSAFE_RELEASE(m_depthStencilStateEnabled);
	SLD3DSAFE_RELEASE(m_depthStencilBuffer);
	SLD3DSAFE_RELEASE(m_MainTargetView);
	SLD3DSAFE_RELEASE(m_d3d11DevCon);
	SLD3DSAFE_RELEASE(m_SwapChain);
	SLD3DSAFE_RELEASE(m_d3d11Device);
}

bool slGSD3D11::Init(slWindow* w, const char* parameters)
{
	SL_ASSERT_ST(w);

	slLog::PrintInfo("Init video driver - D3D11...\n");
	m_activeWindow = w;
	m_activeWindowSize = w->GetCurrentSize();
	m_mainTargetSize.x = (float)m_activeWindowSize->x;
	m_mainTargetSize.y = (float)m_activeWindowSize->y;

	DXGI_MODE_DESC	bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.Width = (UINT)m_activeWindowSize->x;
	bufferDesc.Height = (UINT)m_activeWindowSize->y;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	auto hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_SINGLETHREADED,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&m_d3d11Device,
		&featureLevel,
		&m_d3d11DevCon);
	if (FAILED(hr))
	{
		slLog::PrintError("Can't create Direct3D 11 Device : code %u\n", hr);
		return false;
	}

	IDXGIDevice* dxgiDevice = nullptr;
	IDXGIAdapter* dxgiAdapter = nullptr;
	IDXGIFactory1* dxgiFactory = nullptr;
	hr = m_d3d11Device->QueryInterface(IID_IDXGIDevice, (void**)&dxgiDevice);
	if (FAILED(hr))
	{
		slLog::PrintError("Can't QueryInterface : IID_IDXGIDevice, code %u\n", hr);
		return false;
	}

	hr = dxgiDevice->GetParent(IID_IDXGIAdapter, (void**)&dxgiAdapter);
	if (FAILED(hr))
	{
		slLog::PrintError("Can't get DXGI adapter, code %u\n", hr);
		return false;
	}

	hr = dxgiAdapter->GetParent(IID_IDXGIFactory, (void**)&dxgiFactory);
	if (FAILED(hr))
	{
		slLog::PrintError("Can't get DXGI factory, code %u\n", hr);
		return false;
	}

	slWindowWin32* w32 = (slWindowWin32*)w->GetData()->m_implementation;

	DXGI_SWAP_CHAIN_DESC	swapChainDesc;
	memset(&swapChainDesc, 0, sizeof(swapChainDesc));
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = w32->m_hWnd;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.Windowed = true/*m_params.m_fullScreen ? false : true*/;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	hr = dxgiFactory->CreateSwapChain(m_d3d11Device, &swapChainDesc, &m_SwapChain);
	if (FAILED(hr))
	{
		slLog::PrintError("Can't create Swap Chain : code %u\n", hr);
		return false;
	}

	dxgiFactory->MakeWindowAssociation(w32->m_hWnd, DXGI_MWA_NO_ALT_ENTER);
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	createBackBuffer((float)m_activeWindowSize->x, (float)m_activeWindowSize->y);

	D3D11_DEPTH_STENCIL_DESC	depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if (FAILED(m_d3d11Device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStateEnabled)))
	{
		slLog::PrintError("Can't create Direct3D 11 depth stencil state\n");
		return false;
	}

	m_d3d11DevCon->OMSetDepthStencilState(this->m_depthStencilStateEnabled, 0);

	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.DepthEnable = false;
	if (FAILED(m_d3d11Device->CreateDepthStencilState(&depthStencilDesc, &this->m_depthStencilStateDisabled)))
	{
		slLog::PrintError("Can't create Direct3D 11 depth stencil state\n");
		return false;
	}

	D3D11_RASTERIZER_DESC	rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = true;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	if (FAILED(m_d3d11Device->CreateRasterizerState(&rasterDesc, &m_RasterizerSolid)))
	{
		slLog::PrintError("Can not create rasterizer state\n");
		return false;
	}

	rasterDesc.CullMode = D3D11_CULL_NONE;
	m_d3d11Device->CreateRasterizerState(&rasterDesc, &m_RasterizerSolidNoBackFaceCulling);
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	m_d3d11Device->CreateRasterizerState(&rasterDesc, &m_RasterizerWireframeNoBackFaceCulling);
	rasterDesc.CullMode = D3D11_CULL_BACK;
	m_d3d11Device->CreateRasterizerState(&rasterDesc, &m_RasterizerWireframe);

	m_d3d11DevCon->RSSetState(m_RasterizerSolid);

	D3D11_BLEND_DESC  bd;
	memset(&bd, 0, sizeof(bd));
	bd.AlphaToCoverageEnable = 0;
	bd.RenderTarget[0].BlendEnable = TRUE;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(m_d3d11Device->CreateBlendState(&bd, &m_blendStateAlphaEnabled)))
	{
		slLog::PrintError("Can't create Direct3D 11 blend state\n");
		return false;
	}
	bd.RenderTarget[0].BlendEnable = FALSE;
	if (FAILED(m_d3d11Device->CreateBlendState(&bd, &m_blendStateAlphaDisabled)))
	{
		slLog::PrintError("Can't create Direct3D 11 blend state\n");
		return false;
	}
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	m_d3d11DevCon->OMSetBlendState(m_blendStateAlphaEnabled, blendFactor, 0xffffffff);

	D3D11_RECT sr;
	sr.left = 0;
	sr.top = 0;
	sr.right = (LONG)m_activeWindowSize->x;
	sr.bottom = (LONG)m_activeWindowSize->y;
	m_d3d11DevCon->RSSetScissorRects(1, &sr);

	D3D11_VIEWPORT viewport;
	viewport.Width = (float)m_activeWindowSize->x;
	viewport.Height = (float)m_activeWindowSize->y;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	m_d3d11DevCon->RSSetViewports(1, &viewport);

	// shaders here

	m_d3d11DevCon->OMSetDepthStencilState(m_depthStencilStateEnabled, 0);

	return true;
}

bool slGSD3D11::InitWindow(slWindow* w)
{
	SL_ASSERT_ST(w);
	return true;
}

bool slGSD3D11::updateMainTarget()
{
	m_d3d11DevCon->OMSetRenderTargets(0, 0, 0);

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}
	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}
	if (m_MainTargetView)
	{
		m_MainTargetView->Release();
		m_MainTargetView = 0;
	}

	m_SwapChain->ResizeBuffers(0, (UINT)m_activeWindowSize->x, (UINT)m_activeWindowSize->y, DXGI_FORMAT_UNKNOWN, 0);
	createBackBuffer((float)m_activeWindowSize->x, (float)m_activeWindowSize->y);
	return true;
}

bool slGSD3D11::createBackBuffer(float x, float y)
{
	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}
	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}
	if (m_MainTargetView)
	{
		m_MainTargetView->Release();
		m_MainTargetView = 0;
	}

	ID3D11Texture2D* BackBuffer = 0;
	if (FAILED(m_SwapChain->GetBuffer(
		0,
		IID_ID3D11Texture2D,
		(void**)&BackBuffer)))
	{
		slLog::PrintError("Can't create Direct3D 11 back buffer\n");
		return false;
	}
	if (FAILED(this->m_d3d11Device->CreateRenderTargetView(
		BackBuffer, 0, &m_MainTargetView)))
	{
		slLog::PrintError("Can't create Direct3D 11 render target\n");
		if (BackBuffer) BackBuffer->Release();
		return false;
	}
	if (BackBuffer) BackBuffer->Release();
	D3D11_TEXTURE2D_DESC	DSD;
	ZeroMemory(&DSD, sizeof(DSD));
	DSD.Width = (UINT)x;
	DSD.Height = (UINT)y;
	DSD.MipLevels = 1;
	DSD.ArraySize = 1;
	DSD.Format = DXGI_FORMAT_D32_FLOAT;
	DSD.SampleDesc.Count = 1;
	DSD.SampleDesc.Quality = 0;
	DSD.Usage = D3D11_USAGE_DEFAULT;
	DSD.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DSD.CPUAccessFlags = 0;
	DSD.MiscFlags = 0;
	if (FAILED(m_d3d11Device->CreateTexture2D(&DSD, 0, &m_depthStencilBuffer)))
	{
		slLog::PrintError("Can't create Direct3D 11 depth stencil buffer\n");
		return false;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	memset(&depthStencilViewDesc, 0, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	if (FAILED(m_d3d11Device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView)))
	{
		slLog::PrintError("Can't create Direct3D 11 depth stencil view\n");
		return false;
	}
	m_d3d11DevCon->OMSetRenderTargets(1, &m_MainTargetView, m_depthStencilView);
	return true;
}

void slGSD3D11::SetActiveWindow(slWindow* w)
{
	SL_ASSERT_ST(w);
	m_activeWindow = w;
}

slString slGSD3D11::GetName()
{
	return slString(U"D3D11");
}

slString slGSD3D11::GetTextInfo()
{
	slString info;
	info += U"GS D3D11\r\n";


	return info;
}

slUID slGSD3D11::GetUID()
{
	return g_uid_d3d11;
}

void slGSD3D11::UseVSync(bool v)
{
	m_vsync = v;
}

bool slGSD3D11::UseDepth(bool v)
{
	v ? m_d3d11DevCon->OMSetDepthStencilState(m_depthStencilStateEnabled, 0)
		: m_d3d11DevCon->OMSetDepthStencilState(m_depthStencilStateDisabled, 0);

	static bool old = true;
	bool n = old;
	old = v;
	return n;
}

bool slGSD3D11::UseBlend(bool v)
{
	const float blend_factor[4] = { 1.f, 1.f, 1.f, 1.f };
	if (v)
		m_d3d11DevCon->OMSetBlendState(m_blendStateAlphaEnabled, blend_factor, 0xffffffff);
	else
		m_d3d11DevCon->OMSetBlendState(m_blendStateAlphaDisabled, blend_factor, 0xffffffff);

	static bool old = true;
	bool n = old;
	old = v;
	return n;
}

void slGSD3D11::GetDepthRange(slVec3f* d)
{
	SL_ASSERT_ST(d);
	d->x = 0.f;
	d->y = 1.f;
}

void slGSD3D11::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	D3D11_VIEWPORT viewport;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = (float)x;
	viewport.TopLeftY = (float)y;
	m_d3d11DevCon->RSSetViewports(1, &viewport);
}

void slGSD3D11::SetScissorRect(const slVec4f& rect, slVec4f* o)
{
	D3D11_RECT r;
	r.left = (LONG)rect.x;
	r.top = (LONG)rect.y;
	r.right = (LONG)rect.z;
	r.bottom = (LONG)rect.w;
	m_d3d11DevCon->RSSetScissorRects(1, &r);

	static slVec4f old;

	if (o)
	{
		o->x = old.x;
		o->x = old.y;
		o->x = old.z;
		o->x = old.w;
	}

	old = rect;
}

void slGSD3D11::SetClearColor(float r, float g, float b, float a)
{
	m_clearColor.set(r, g, b, a);
}

void slGSD3D11::BeginDraw()
{
	m_currentTargetView = m_MainTargetView;
	m_d3d11DevCon->OMSetRenderTargets(1, &m_currentTargetView, m_depthStencilView);
	//SetViewport(0, 0, m_mainTargetSize.x, m_mainTargetSize.y);
	SetViewport(0, 0, m_activeWindowSize->x, m_activeWindowSize->y);

}

void slGSD3D11::ClearDepth()
{
	m_d3d11DevCon->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void slGSD3D11::ClearColor()
{
	m_d3d11DevCon->ClearRenderTargetView(m_currentTargetView, m_clearColor.data());
}

void slGSD3D11::ClearAll()
{
	m_d3d11DevCon->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_d3d11DevCon->ClearRenderTargetView(m_currentTargetView, m_clearColor.data());
}

void slGSD3D11::EndDraw()
{

}

void slGSD3D11::SwapBuffers()
{
	m_vsync ? m_SwapChain->Present(1, 0) : m_SwapChain->Present(0, 0);
}

void slGSD3D11::UpdateMainRenderTarget(const slVec3f& v)
{
	m_mainTargetSize = v;
	updateMainTarget();
}


