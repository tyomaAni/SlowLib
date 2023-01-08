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

#include "slowlib.d3d11impl.h"
#include "slowlib.base/system/slWindowWin32.h"
#include "slowlib.base/gs/slMaterial.h"
#include "slowlib.base/GUI/slGUI.h"
#include "slowlib.base/scene/slSprite.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// {0FE89B14-923C-4F2E-BA22-B18694D6F1ED}
slDEFINE_UID(g_uid_d3d11, 0xfe89b14, 0x923c, 0x4f2e, 0xba22, 0xb1, 0x86, 0x94, 0xd6, 0xf1, 0xed);


slGSD3D11::slGSD3D11()
{	
}

slGSD3D11::~slGSD3D11()
{
	Shutdown();
}

void slGSD3D11::SetActiveShader(slGSD3D11ShaderBase* shader)
{
	m_activeShader = shader;
	m_d3d11DevCon->IASetInputLayout(shader->m_vLayout);
	m_d3d11DevCon->VSSetShader(shader->m_vShader, 0, 0);
	m_d3d11DevCon->GSSetShader(shader->m_gShader, 0, 0);
	m_d3d11DevCon->PSSetShader(shader->m_pShader, 0, 0);
}

bool slGSD3D11::CreateShaders()
{
	m_shaderLine3D = slCreate<slD3D11ShaderLine3D>(this);
	if (!m_shaderLine3D->init())
		return false;

	m_shaderSolid = slCreate<slD3D11ShaderSolid>(this);
	if (!m_shaderSolid->init())
		return false;

	m_shaderMainTarget = slCreate<slD3D11ShaderMainTarget>(this);
	if (!m_shaderMainTarget->init())
		return false;

	m_shaderGUIRectangle = slCreate<slD3D11ShaderGUIRectangle>(this);
	if (!m_shaderGUIRectangle->init())
		return false;

	m_shaderSprite = slCreate<slD3D11ShaderSprite>(this);
	if (!m_shaderSprite->init())
		return false;

	return true;
}

bool slGSD3D11::createShaders(
	const char* vertexTarget,
	const char* pixelTarget,
	const char* vertexShader,
	const char* pixelShader,
	const char* vertexEntryPoint,
	const char* pixelEntryPoint,
	slMeshVertexType vertexType,
	ID3D11VertexShader** vs,
	ID3D11PixelShader** ps,
	ID3D11InputLayout** il)
{
	ID3D10Blob* m_VsBlob = nullptr;
	ID3D10Blob* m_PsBlob = nullptr;
	ID3D10Blob* m_errorBlob = nullptr;

	HRESULT hr = D3DCompile(
		vertexShader,
		strlen(vertexShader),
		0, 0, 0,
		vertexEntryPoint,
		vertexTarget,
		0,
		0,
		&m_VsBlob,
		&m_errorBlob
	);

	if (FAILED(hr))
	{
		char* message = (char*)m_errorBlob->GetBufferPointer();
		slLog::PrintError("Vertex shader compile error: %s\n", message);
		return false;
	}

	hr = D3DCompile(
		pixelShader,
		strlen(pixelShader),
		0, 0, 0,
		pixelEntryPoint,
		pixelTarget,
		0,
		0,
		&m_PsBlob,
		&m_errorBlob
	);

	if (FAILED(hr))
	{
		char* message = (char*)m_errorBlob->GetBufferPointer();
		slLog::PrintError("Pixel shader compile error: %s\n", message);
		return false;
	}

	hr = m_d3d11Device->CreateVertexShader(
		m_VsBlob->GetBufferPointer(),
		m_VsBlob->GetBufferSize(),
		0,
		vs);
	if (FAILED(hr))
	{
		slLog::PrintError("Can't create vertex shader. Error code [%u]\n", hr);
		return false;
	}

	hr = m_d3d11Device->CreatePixelShader(
		m_PsBlob->GetBufferPointer(),
		m_PsBlob->GetBufferSize(),
		0,
		ps);
	if (FAILED(hr))
	{
		slLog::PrintError("Can't create pixel shader. Error code [%u]\n", hr);
		return false;
	}

	if (vertexType != slMeshVertexType::Null)
	{
		D3D11_INPUT_ELEMENT_DESC vertexLayout[8];
		uint32_t vertexLayoutSize = 0;
		/*
		LPCSTR SemanticName;
		UINT SemanticIndex;
		DXGI_FORMAT Format;
		UINT InputSlot;
		UINT AlignedByteOffset;
		D3D11_INPUT_CLASSIFICATION InputSlotClass;
		UINT InstanceDataStepRate;
		*/

		int ind = 0;
		switch (vertexType)
		{
		case slMeshVertexType::Triangle:
			ind = 0;
			vertexLayout[ind].SemanticName = "POSITION";
			vertexLayout[ind].SemanticIndex = 0;
			vertexLayout[ind].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			vertexLayout[ind].InputSlot = 0;
			vertexLayout[ind].AlignedByteOffset = 0;
			vertexLayout[ind].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			vertexLayout[ind].InstanceDataStepRate = 0;

			ind++;
			vertexLayout[ind].SemanticName = "TEXCOORD";
			vertexLayout[ind].SemanticIndex = 0;
			vertexLayout[ind].Format = DXGI_FORMAT_R32G32_FLOAT;
			vertexLayout[ind].InputSlot = 0;
			vertexLayout[ind].AlignedByteOffset = 12;
			vertexLayout[ind].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			vertexLayout[ind].InstanceDataStepRate = 0;

			ind++;
			vertexLayout[ind].SemanticName = "NORMAL";
			vertexLayout[ind].SemanticIndex = 0;
			vertexLayout[ind].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			vertexLayout[ind].InputSlot = 0;
			vertexLayout[ind].AlignedByteOffset = 20;
			vertexLayout[ind].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			vertexLayout[ind].InstanceDataStepRate = 0;

			ind++;
			vertexLayout[ind].SemanticName = "BINORMAL";
			vertexLayout[ind].SemanticIndex = 0;
			vertexLayout[ind].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			vertexLayout[ind].InputSlot = 0;
			vertexLayout[ind].AlignedByteOffset = 32;
			vertexLayout[ind].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			vertexLayout[ind].InstanceDataStepRate = 0;

			ind++;
			vertexLayout[ind].SemanticName = "TANGENT";
			vertexLayout[ind].SemanticIndex = 0;
			vertexLayout[ind].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			vertexLayout[ind].InputSlot = 0;
			vertexLayout[ind].AlignedByteOffset = 44;
			vertexLayout[ind].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			vertexLayout[ind].InstanceDataStepRate = 0;

			ind++;
			vertexLayout[ind].SemanticName = "COLOR";
			vertexLayout[ind].SemanticIndex = 0;
			vertexLayout[ind].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			vertexLayout[ind].InputSlot = 0;
			vertexLayout[ind].AlignedByteOffset = 56;
			vertexLayout[ind].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			vertexLayout[ind].InstanceDataStepRate = 0;
			break;
		}
		vertexLayoutSize = ind + 1;

		hr = m_d3d11Device->CreateInputLayout(
			vertexLayout,
			vertexLayoutSize,
			m_VsBlob->GetBufferPointer(),
			m_VsBlob->GetBufferSize(),
			il);
		if (FAILED(hr))
		{
			slLog::PrintError("Can't create input layout. Error code [%u]\n", hr);
			return false;
		}
	}


	if (m_VsBlob)    m_VsBlob->Release();
	if (m_PsBlob)    m_PsBlob->Release();
	if (m_errorBlob) m_errorBlob->Release();

	return true;
}

bool slGSD3D11::createConstantBuffer(uint32_t byteSize, ID3D11Buffer** cb)
{
	D3D11_BUFFER_DESC mbd;
	memset(&mbd, 0, sizeof(mbd));
	mbd.Usage = D3D11_USAGE_DYNAMIC;
	mbd.ByteWidth = byteSize;
	mbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mbd.MiscFlags = 0;
	mbd.StructureByteStride = 0;

	HRESULT hr = m_d3d11Device->CreateBuffer(&mbd, 0, cb);
	if (FAILED(hr))
	{
		slLog::PrintError("Can't create constant buffer. Error code [%u]\n", hr);
		return false;
	}
	return true;
}

bool slGSD3D11::createGeometryShaders(const char* target,
	const char* shaderText,
	const char* entryPoint,
	ID3D11GeometryShader** gs)
{
	ID3D10Blob* m_GsBlob = nullptr;
	ID3D10Blob* m_errorBlob = nullptr;
	HRESULT hr = D3DCompile(
		shaderText,
		strlen(shaderText),
		0, 0, 0,
		entryPoint,
		target,
		0,
		0,
		&m_GsBlob,
		&m_errorBlob
	);
	if (FAILED(hr))
	{
		char* message = (char*)m_errorBlob->GetBufferPointer();
		slLog::PrintError("Geometry shader compile error: %s\n", message);
		return false;
	}

	hr = m_d3d11Device->CreateGeometryShader(
		m_GsBlob->GetBufferPointer(),
		m_GsBlob->GetBufferSize(),
		0,
		gs);
	if (FAILED(hr))
	{
		slLog::PrintError("Can't create geometry shader. Error code [%u]\n", hr);
		return false;
	}

	return true;
}

void slGSD3D11::Shutdown()
{
	SLSAFE_DESTROY(m_whiteTexture);
	SLSAFE_DESTROY(m_mainTargetRTT);
	SLSAFE_DESTROY(m_GUIRTT);

	SLSAFE_DESTROY(m_shaderSprite);
	SLSAFE_DESTROY(m_shaderSolid);
	SLSAFE_DESTROY(m_shaderLine3D);
	SLSAFE_DESTROY(m_shaderMainTarget);
	SLSAFE_DESTROY(m_shaderGUIRectangle);

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
	m_mainTargetSize.x = m_activeWindowSize->x;
	m_mainTargetSize.y = m_activeWindowSize->y;

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

	DXGI_MODE_DESC	bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.Width = (UINT)m_activeWindowSize->x;
	bufferDesc.Height = (UINT)m_activeWindowSize->y;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;

	//DXGI_FORMAT_R16G16B16A16_FLOAT
	//DXGI_FORMAT_R10G10B10A2_UNORM
	//DXGI_FORMAT_R8G8B8A8_UNORM
	//DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
	//DXGI_FORMAT_B8G8R8A8_UNORM
	//DXGI_FORMAT_B8G8R8A8_UNORM_SRGB
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

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

	m_d3d11DevCon->OMSetDepthStencilState(m_depthStencilStateEnabled, 0);

	if (!CreateShaders())
	{
		slLog::PrintError("Can't create Direct3D 11 shader\n");
		return false;
	}

	if (!updateMainTarget())
	{
		slLog::PrintError("Can't create main target\n");
		return false;
	}

	{
		slImage img;
		img.Create(2, 2);
		img.Fill(ColorWhite);
		slTextureInfo tinf;
		m_whiteTexture = (slGSD3D11Texture*)SummonTexture(&img, tinf);
	}

	return true;
}

bool slGSD3D11::InitWindow(slWindow* w)
{
	SL_ASSERT_ST(w);
	return true;
}

bool slGSD3D11::updateMainTarget()
{
	updateGUIMatrix();

	SLSAFE_DESTROY(m_mainTargetRTT);

	slTextureInfo tinf;
	m_mainTargetRTT = (slGSD3D11Texture*)SummonRenderTargetTexture(
		slPoint((uint32_t)m_mainTargetSize.x, (uint32_t)m_mainTargetSize.y), 
		tinf);

	m_GUIRTT = (slGSD3D11Texture*)SummonRenderTargetTexture(
		slPoint((uint32_t)m_activeWindowSize->x, (uint32_t)m_activeWindowSize->y),
		tinf);

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
		m_currentBlendState = m_blendStateAlphaEnabled;
	else
		m_currentBlendState = m_blendStateAlphaDisabled;

	m_d3d11DevCon->OMSetBlendState(m_currentBlendState, blend_factor, 0xffffffff);

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
	m_currentTargetView = m_mainTargetRTT->m_RTV;
	m_d3d11DevCon->OMSetRenderTargets(1, &m_currentTargetView, m_depthStencilView);
	SetViewport(0, 0, (uint32_t)m_mainTargetSize.x, (uint32_t)m_mainTargetSize.y);
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
	m_d3d11DevCon->OMSetRenderTargets(1, &m_MainTargetView, m_depthStencilView);
	m_currentTargetView = m_MainTargetView;
	SetViewport(0, 0, (uint32_t)m_activeWindowSize->x, (uint32_t)m_activeWindowSize->y);
	SetScissorRect(slVec4f(0.f, 0.f, (float)m_activeWindowSize->x, (float)m_activeWindowSize->y), 0);
	ClearColor();

	SetActiveShader(m_shaderMainTarget);
	m_shaderMainTarget->SetConstants(0);

	m_d3d11DevCon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	m_d3d11DevCon->Draw(1, 0);
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

void slGSD3D11::DrawLine3D(const slVec3& p1, const slVec3& p2, const slColor& c)
{
	m_shaderLine3D->SetData(p1, p2, c, *slFramework::GetMatrix(slMatrixType::ViewProjection));
	m_shaderLine3D->SetConstants(0);

	m_d3d11DevCon->Draw(2, 0);
}

slGPUMesh* slGSD3D11::SummonMesh(slMesh* m)
{
	SL_ASSERT_ST(m);
	SL_ASSERT_ST(m->m_vertices);
	SL_ASSERT_ST(m->m_indices);
	SL_ASSERT_ST(m->m_info.m_iCount);
	SL_ASSERT_ST(m->m_info.m_vCount);

	slMesh* oldm = m;

	slVertexTriangle V[6];
	V[0].Position.set(1.f, 1.f, 0.f);
	V[1].Position.set(-1.f, -1.f, 0.f);
	V[2].Position.set(-1.f, 1.f, 0.f);

	V[3].Position.set(1.f, 1.f, 0.f);
	V[4].Position.set(1.f, -1.f, 0.f);
	V[5].Position.set(-1.f, -1.f, 0.f);
	uint16_t I[6];
	I[0] = 0;
	I[1] = 1;
	I[2] = 2;
	I[3] = 3;
	I[4] = 4;
	I[5] = 5;

	slMesh M;
	M.m_indices = (uint8_t*)(&I[0]);
	M.m_vertices = (uint8_t*)(&V[0]);
	M.m_info.m_iCount = 6;
	M.m_info.m_vCount = 6;
	M.m_info.m_stride = sizeof(slVertexTriangle);
	M.m_info.m_indexType = slMeshIndexType::u16;
	M.m_info.m_vertexType = slMeshVertexType::Triangle;

	//m = &M;
	slVertexTriangle* vertArr = (slVertexTriangle*)m->m_vertices;
	uint16_t* indsArr = (uint16_t*)m->m_indices;


	slGSD3D11Mesh* newMesh = slCreate<slGSD3D11Mesh>();
	newMesh->m_meshInfo = m->m_info;

	slZeroDecl(D3D11_BUFFER_DESC, vbd);
	slZeroDecl(D3D11_BUFFER_DESC, ibd);

	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	slZeroDecl(D3D11_SUBRESOURCE_DATA, vData);
	slZeroDecl(D3D11_SUBRESOURCE_DATA, iData);

	vbd.ByteWidth = m->m_info.m_stride * m->m_info.m_vCount;
	vData.pSysMem = &m->m_vertices[0];

	auto hr = m_d3d11Device->CreateBuffer(&vbd, &vData, &newMesh->m_vBuffer);
	if (FAILED(hr))
	{
		slLog::PrintError("Can't create Direct3D 11 vertex buffer [%u]\n", hr);
		return nullptr;
	}

	uint32_t index_sizeof = sizeof(uint16_t);
	newMesh->m_indexType = DXGI_FORMAT_R16_UINT;
	if (m->m_info.m_indexType == slMeshIndexType::u32)
	{
		newMesh->m_indexType = DXGI_FORMAT_R32_UINT;
		index_sizeof = sizeof(uint32_t);
	}
	ibd.ByteWidth = index_sizeof * newMesh->m_meshInfo.m_iCount;
	iData.pSysMem = &m->m_indices[0];

	hr = m_d3d11Device->CreateBuffer(&ibd, &iData, &newMesh->m_iBuffer);
	if (FAILED(hr))
	{
		slLog::PrintError("Can't create Direct3D 11 index buffer [%u]\n", hr);
		return nullptr;
	}

	M.m_indices = 0;
	M.m_vertices = 0;

	m = oldm;
	return newMesh;
}

void slGSD3D11::SetShader(slShaderType st, uint32_t userIndex)
{
	switch (st)
	{
	case slShaderType::Solid:
		SetActiveShader(m_shaderSolid);
		break;
	case slShaderType::BumpMap:
		break;
	case slShaderType::SphereMap:
		break;
	case slShaderType::Line3D:
		SetActiveShader(m_shaderLine3D);
		m_d3d11DevCon->IASetInputLayout(NULL);
		m_d3d11DevCon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	case slShaderType::Sprite:
		SetActiveShader(m_shaderSprite);
		break;
	case slShaderType::User:
		break;
	default:
		slLog::PrintWarning("Uknown shader\n");
		break;
	}
}

void slGSD3D11::SetMesh(slGPUMesh* m)
{
	m_currMesh = (slGSD3D11Mesh*)m;
}

void slGSD3D11::SetMaterial(slMaterial* m)
{
	m_currMaterial = m;
}

void slGSD3D11::Draw()
{
	SL_ASSERT_ST(m_currMesh);
	SL_ASSERT_ST(m_currMaterial);

	if (m_currMaterial->m_wireframe)
	{
		if (m_currMaterial->m_cullBackFace)
			m_d3d11DevCon->RSSetState(m_RasterizerWireframe);
		else
			m_d3d11DevCon->RSSetState(m_RasterizerWireframeNoBackFaceCulling);
	}
	else
	{
		if (m_currMaterial->m_cullBackFace)
			m_d3d11DevCon->RSSetState(m_RasterizerSolid);
		else
			m_d3d11DevCon->RSSetState(m_RasterizerSolidNoBackFaceCulling);
	}

	switch (m_currMesh->m_meshInfo.m_vertexType)
	{
	case slMeshVertexType::Triangle:
	{
		switch (m_currMaterial->m_shader)
		{
		case slShaderType::Solid:
			m_shaderSolid->SetData(*slFramework::GetMatrix(slMatrixType::WorldViewProjection),
				*slFramework::GetMatrix(slMatrixType::World));
			m_shaderSolid->SetConstants(m_currMaterial);
			break;
		}
	}break;
	}
	/*{
		m_d3d11DevCon->PSSetShaderResources(0, 1, &m_currentTextures[0]->m_textureResView);
		m_d3d11DevCon->PSSetSamplers(0, 1, &m_currentTextures[0]->m_samplerState);
	}*/

	uint32_t offset = 0u;
	m_d3d11DevCon->IASetVertexBuffers(0, 1, &m_currMesh->m_vBuffer, &m_currMesh->m_meshInfo.m_stride, &offset);

	switch (m_currMesh->m_meshInfo.m_vertexType)
	{
	default:
	case slMeshVertexType::Triangle:
		m_d3d11DevCon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_d3d11DevCon->IASetIndexBuffer(m_currMesh->m_iBuffer, m_currMesh->m_indexType, 0);
		m_d3d11DevCon->DrawIndexed(m_currMesh->m_meshInfo.m_iCount, 0, 0);
		break;
	}
}

HRESULT	slGSD3D11::createSamplerState(D3D11_FILTER filter,
	D3D11_TEXTURE_ADDRESS_MODE addressMode,
	uint32_t anisotropic_level,
	ID3D11SamplerState** samplerState,
	D3D11_COMPARISON_FUNC cmpFunc)
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = filter;
	samplerDesc.MipLODBias = 0.0f;

	samplerDesc.AddressU = addressMode;
	samplerDesc.AddressV = addressMode;
	samplerDesc.AddressW = addressMode;

	samplerDesc.ComparisonFunc = cmpFunc; //D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	samplerDesc.MaxAnisotropy = anisotropic_level;


	return m_d3d11Device->CreateSamplerState(&samplerDesc, samplerState);
}

slTexture* slGSD3D11::SummonTexture(slImage* img, const slTextureInfo& inf)
{
	SL_ASSERT_ST(img);
	slGSD3D11Texture* newTexture = 0;

	ID3D11Texture2D* _texture = 0;
	ID3D11ShaderResourceView* _textureResView = 0;
	ID3D11SamplerState* _samplerState = 0;
	ID3D11RenderTargetView* _RTV = 0;
	D3D11_FILTER filter;
	slTextureFilter tf = inf.m_filter;
	switch (tf)
	{
	case slTextureFilter::PPP:
		filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	case slTextureFilter::PPL:
		filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		break;
	case slTextureFilter::PLP:
		filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
		break;
	case slTextureFilter::PLL:
		filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		break;
	case slTextureFilter::LPP:
		filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
		break;
	case slTextureFilter::LPL:
		filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
		break;
	case slTextureFilter::LLP:
		filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		break;
	case slTextureFilter::LLL:
		filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case slTextureFilter::ANISOTROPIC:
		filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		break;
	case slTextureFilter::CMP_PPP:
		filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		break;
	case slTextureFilter::CMP_PPL:
		filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
		break;
	case slTextureFilter::CMP_PLP:
		filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
		break;
	case slTextureFilter::CMP_PLL:
		filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
		break;
	case slTextureFilter::CMP_LPP:
		filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
		break;
	case slTextureFilter::CMP_LPL:
		filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
		break;
	case slTextureFilter::CMP_LLP:
		filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		break;
	case slTextureFilter::CMP_LLL:
		filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		break;
	case slTextureFilter::CMP_ANISOTROPIC:
		filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_ANISOTROPIC;
		break;
	default:
		break;
	}

	HRESULT hr = 0;
	if (inf.m_type == slTextureType::Texture2D)
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = img->m_info.m_width;
		desc.Height = img->m_info.m_height;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		bool isGenMips = inf.m_generateMipmaps;

		switch (img->m_info.m_format)
		{
		case slImageFormat::r8g8b8a8:
		{
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			desc.MiscFlags = 0;
			desc.MipLevels = 1;

			desc.ArraySize = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.CPUAccessFlags = 0;

			if (isGenMips)
			{
				desc.MipLevels = 0;
				desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
				hr = m_d3d11Device->CreateTexture2D(&desc, 0, &_texture);
				if (FAILED(hr))
				{
					slLog::PrintError("Can't create 2D texture\n");
					goto fail;
				}
				m_d3d11DevCon->UpdateSubresource(_texture, 0, NULL, img->m_data, img->m_info.m_pitch, 0);
			}
			else
			{
				D3D11_SUBRESOURCE_DATA initData;
				ZeroMemory(&initData, sizeof(initData));
				initData.pSysMem = img->m_data;
				initData.SysMemPitch = img->m_info.m_pitch;
				initData.SysMemSlicePitch = img->m_dataSize;
				hr = m_d3d11Device->CreateTexture2D(&desc, &initData, &_texture);
				if (FAILED(hr))
				{
					slLog::PrintError("Can't create 2D texture\n");
					goto fail;
				}
			}


			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
			ZeroMemory(&SRVDesc, sizeof(SRVDesc));
			SRVDesc.Format = desc.Format;
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MostDetailedMip = 0;
			SRVDesc.Texture2D.MipLevels = 1;
			if (isGenMips)
				SRVDesc.Texture2D.MipLevels = -1;

			hr = m_d3d11Device->CreateShaderResourceView(_texture,
				&SRVDesc, &_textureResView);
			if (FAILED(hr))
			{
				slLog::PrintError("Can't create shader resource view\n");
				goto fail;
			}
		}break;
		default:
			slLog::PrintError("Unsupported texture format\n");
			goto fail;
		}
		if (isGenMips)
			m_d3d11DevCon->GenerateMips(_textureResView);
	}
	else
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = img->m_info.m_width;
		desc.Height = img->m_info.m_height;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.MiscFlags = 0;
		desc.ArraySize = 1;
		desc.MipLevels = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;

		hr = m_d3d11Device->CreateTexture2D(&desc, NULL, &_texture);
		if (FAILED(hr))
		{
			slLog::PrintError("Can't create render target texture\n");
			goto fail;
		}
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = desc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		hr = m_d3d11Device->CreateRenderTargetView(_texture, &renderTargetViewDesc, &_RTV);
		if (FAILED(hr))
		{
			slLog::PrintError("Can't create render target view\n");
			goto fail;
		}



		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(SRVDesc));
		SRVDesc.Format = desc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MostDetailedMip = 0;
		SRVDesc.Texture2D.MipLevels = 1;

		hr = m_d3d11Device->CreateShaderResourceView(_texture,
			&SRVDesc, &_textureResView);
		if (FAILED(hr))
		{
			slLog::PrintError("Can't create shader resource view\n");
			goto fail;
		}

		//goto success;
	}

	D3D11_TEXTURE_ADDRESS_MODE tam;
	switch (inf.m_adrMode)
	{
	case slTextureAddressMode::Wrap:
	default:
		tam = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	case slTextureAddressMode::Mirror:
		tam = D3D11_TEXTURE_ADDRESS_MIRROR;
		break;
	case slTextureAddressMode::Clamp:
		tam = D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	case slTextureAddressMode::Border:
		tam = D3D11_TEXTURE_ADDRESS_BORDER;
		break;
	case slTextureAddressMode::MirrorOnce:
		tam = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		break;
	}

	D3D11_COMPARISON_FUNC cmpFunc;
	switch (inf.m_cmpFnc)
	{
	case slTextureComparisonFunc::Never:
		cmpFunc = D3D11_COMPARISON_NEVER;
		break;
	case slTextureComparisonFunc::Less:
		cmpFunc = D3D11_COMPARISON_LESS;
		break;
	case slTextureComparisonFunc::Equal:
		cmpFunc = D3D11_COMPARISON_EQUAL;
		break;
	case slTextureComparisonFunc::LessEqual:
		cmpFunc = D3D11_COMPARISON_LESS_EQUAL;
		break;
	case slTextureComparisonFunc::Greater:
		cmpFunc = D3D11_COMPARISON_GREATER;
		break;
	case slTextureComparisonFunc::NotEqual:
		cmpFunc = D3D11_COMPARISON_NOT_EQUAL;
		break;
	case slTextureComparisonFunc::GreaterEqual:
		cmpFunc = D3D11_COMPARISON_GREATER_EQUAL;
		break;
	case slTextureComparisonFunc::Always:
	default:
		cmpFunc = D3D11_COMPARISON_ALWAYS;
		break;
	}

	hr = createSamplerState(filter, tam, inf.m_anisotropicLevel,
		&_samplerState, cmpFunc);
	if (FAILED(hr))
	{
		slLog::PrintError("Can't create sampler state\n");
		goto fail;
	}

	newTexture = slCreate<slGSD3D11Texture>();
	newTexture->m_RTV = _RTV;
	newTexture->m_samplerState = _samplerState;
	newTexture->m_textureResView = _textureResView;
	newTexture->m_texture = _texture;
	newTexture->m_textureInfo = inf;
	newTexture->m_textureInfo.m_imageInfo = img->m_info;
	return newTexture;

fail:;
	SLD3DSAFE_RELEASE(_RTV);
	SLD3DSAFE_RELEASE(_samplerState);
	SLD3DSAFE_RELEASE(_textureResView);
	SLD3DSAFE_RELEASE(_texture);
	return NULL;
}

void slGSD3D11::SetRenderTarget(slTexture* t)
{
	if (t)
	{
		slGSD3D11Texture* gst = (slGSD3D11Texture*)t;
		m_d3d11DevCon->OMSetRenderTargets(1, &gst->m_RTV, m_depthStencilView);
		m_currentTargetView = gst->m_RTV;
	}
	else
	{
		m_d3d11DevCon->OMSetRenderTargets(1, &m_mainTargetRTT->m_RTV, m_depthStencilView);
		m_currentTargetView = m_mainTargetRTT->m_RTV;
	}
}

slTexture* slGSD3D11::SummonRenderTargetTexture(const slPoint& size, const slTextureInfo& ti)
{
	slImage img;
	img.m_info.m_width = size.x;
	img.m_info.m_height = size.y;

	slTextureInfo inf;
	inf = ti;
	inf.m_type = slTextureType::RTT;

	return SummonTexture(&img, inf);
}

void slGSD3D11::updateGUIMatrix()
{
	m_shaderGUIRectangle->m_cbDataFrame.ProjMtx.m_data[0].set(2.0f / m_activeWindowSize->x, 0.0f, 0.0f, 0.0f);
	m_shaderGUIRectangle->m_cbDataFrame.ProjMtx.m_data[1].set(0.0f, 2.0f / -m_activeWindowSize->y, 0.0f, 0.0f);
	m_shaderGUIRectangle->m_cbDataFrame.ProjMtx.m_data[2].set(0.0f, 0.0f, 0.5f, 0.0f);
	m_shaderGUIRectangle->m_cbDataFrame.ProjMtx.m_data[3].set(-1.f, 1.f, 0.5f, 1.0f);
}

void slGSD3D11::BeginGUI()
{
	const float cc[4] = { 0.f,0.f,0.f,0.f };
	SetRenderTarget(m_GUIRTT);
	UseDepth(false);
	UseBlend(true);
	m_d3d11DevCon->ClearRenderTargetView(m_currentTargetView, cc);
	SetActiveShader(m_shaderGUIRectangle);
	m_shaderGUIRectangle->SetOnFrame();
	m_d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void slGSD3D11::DrawGUIRectangle(const slVec4f& rect, const slColor& color1, const slColor& color2,
	slTexture* t, slVec4f* UVs)
{
	m_shaderGUIRectangle->m_cbDataElement.Color1 = color1;
	m_shaderGUIRectangle->m_cbDataElement.Color2 = color2;

	m_shaderGUIRectangle->m_cbDataElement.Corners = rect;

	m_shaderGUIRectangle->m_cbDataElement.UVs.x = 0.f;
	m_shaderGUIRectangle->m_cbDataElement.UVs.y = 0.f;
	m_shaderGUIRectangle->m_cbDataElement.UVs.z = 1.f;
	m_shaderGUIRectangle->m_cbDataElement.UVs.w = 1.f;

	if (UVs)
		m_shaderGUIRectangle->m_cbDataElement.UVs = *UVs;

	m_shaderGUIRectangle->SetOnElement(dynamic_cast<slGSD3D11Texture*>(t ? t : m_whiteTexture));

	m_d3d11DevCon->Draw(1, 0);
}

void slGSD3D11::DrawGUIRectangle(const slRect& rct, const slColor& color1, const slColor& color2,
	slTexture* t, slVec4f* UVs)
{
	slVec4f r;
	r.x = (float)rct.left;
	r.y = (float)rct.top;
	r.z = (float)rct.right;
	r.w = (float)rct.bottom;
	DrawGUIRectangle(r, color1, color2, dynamic_cast<slGSD3D11Texture*>(t ? t : m_whiteTexture), UVs);
}

void slGSD3D11::DrawGUICharacter(char32_t ch, slGUIFont* font, const slVec2f& position, const slColor& color)
{
	SL_ASSERT_ST(font);
	SL_ASSERT_ST(ch < 0x32000);

	slGUIFontGlyph* g = font->GetGlyphMap()[ch];

	slVec4f rct;
	rct.x = position.x;
	rct.y = position.y;

	rct.z = rct.x + g->m_width;
	rct.w = rct.y + g->m_height;

	DrawGUIRectangle(rct, color, color, (slGSD3D11Texture*)font->GetTexture(g->m_textureSlot),
		&g->m_UV);
}

void slGSD3D11::DrawGUIText(const char32_t* text, uint32_t textSz, const slVec2f& _position,
	slGUIDrawTextCallback* cb)
{
	SL_ASSERT_ST(cb);
	SL_ASSERT_ST(text);

	slVec2f position = _position;

	for (uint32_t i = 0; i < textSz; ++i)
	{
		slGUIFont* font = cb->OnFont(cb->m_reason, text[i]);
		slColor* color = cb->OnColor(cb->m_reason, text[i]);

		slGUIFontGlyph* g = font->GetGlyphMap()[text[i]];

		slVec4f rct;
		rct.x = position.x;
		rct.y = position.y;

		rct.z = rct.x + g->m_width;
		rct.w = rct.y + g->m_height;

		DrawGUIRectangle(rct, *color, *color, (slGSD3D11Texture*)font->GetTexture(g->m_textureSlot),
			&g->m_UV);

		position.x += g->m_width + g->m_overhang + g->m_underhang + font->m_characterSpacing;

		switch (text[i])
		{
		case U' ':
			position.x += font->m_spaceSize;
			break;
		case U'\t':
			position.x += font->m_tabSize;
			break;
		case U'\n':
			position.y +=  font->GetMaxSize().y;
			position.x = _position.x;
			break;
		}
	}
}

void slGSD3D11::EndGUI()
{
	SetRenderTarget(m_mainTargetRTT);
	UseDepth(true);
}

void slGSD3D11::DrawSprite(slSprite* s)
{
	SL_ASSERT_ST(s);
	SL_ASSERT_ST(s->GetTexture());

	m_shaderSprite->m_cbDataElement.Color1 = ColorWhite;
	m_shaderSprite->m_cbDataElement.Color2 = ColorWhite;

	m_shaderSprite->m_cbDataElement.Corners = s->GetRect();

	m_shaderSprite->m_cbDataElement.UVs.x = 0.f;
	m_shaderSprite->m_cbDataElement.UVs.y = 0.f;
	m_shaderSprite->m_cbDataElement.UVs.z = 1.f;
	m_shaderSprite->m_cbDataElement.UVs.w = 1.f;

	m_shaderSprite->m_cbDataElement.WVP = *slFramework::GetMatrix(slMatrixType::Projection)
		* (*slFramework::GetMatrix(slMatrixType::View))
		* s->GetMatrixWorld();
	m_shaderSprite->m_cbDataElement.WorldMtx = s->GetMatrixWorld();

	m_shaderSprite->SetOnElement(dynamic_cast<slGSD3D11Texture*>(s->GetTexture()));


	m_d3d11DevCon->Draw(1, 0);
}
