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
#include "slowlib.base/gs/slMaterial.h"

#include "../slowlib.d3d11impl.h"


slD3D11ShaderSolid::slD3D11ShaderSolid(slGSD3D11* gs)
	:
	m_gs(gs)
{}

slD3D11ShaderSolid::~slD3D11ShaderSolid() 
{
	if (m_cbV)m_cbV->Release();
	if (m_cbP)m_cbP->Release();
}

bool slD3D11ShaderSolid::init(){
	const char* text =
		"Texture2D tex2d_1;\n"
		"SamplerState tex2D_sampler_1;\n"
		"struct VSIn{\n"
		"   float3 position : POSITION;\n"
		"	float2 uv : TEXCOORD;\n"
		"   float3 normal : NORMAL;\n"
		"   float3 binormal : BINORMAL;\n"
		"   float3 tangent : TANGENT;\n"
		"   float4 color : COLOR;\n"
		"};\n"
		"cbuffer cbVertex{\n"
		"	double4x4 WVP;\n"
		"	double4x4 W;\n"
		"};\n"
		"cbuffer cbPixel{\n"
		"	double4 SunPosition;\n"
		"	float4 BaseColor;\n"
		"	float alphaDiscard;\n"
		"};\n"
		"#define FLAG_NOBLEND 0x1\n"
		"struct VSOut{\n"
		"   float4 pos : SV_POSITION;\n"
		"	float2 uv : TEXCOORD0;\n"
		"	float4 vColor : COLOR0;\n"
		"	float3 normal : NORMAL0;\n"
		"	float4 fragPos : NORMAL1;\n"
		"};\n"
		"struct PSOut{\n"
		"    float4 color : SV_Target;\n"
		"};\n"
		"VSOut VSMain(VSIn input){\n"
		"   VSOut output;\n"
		"	output.pos   = mul(WVP, double4(input.position.x, input.position.y, input.position.z, 1.0));\n"
		"	output.uv.x    = input.uv.x;\n"
		"	output.uv.y    = input.uv.y;\n"
		"	output.vColor    = input.color;\n"
		"	output.normal    = normalize(mul((double3x3)W, input.normal));\n"
		"	output.fragPos    = mul(W, double4(input.position.x, input.position.y, input.position.z, 1.0));\n"
		"	return output;\n"
		"}\n"
		"PSOut PSMain(VSOut input){\n"
		"	float3 lightDir = normalize(-SunPosition.xyz);\n"
		"	float diff = max(dot(input.normal, -lightDir), 0.0);\n"

		"   PSOut output;\n"
		"   output.color = tex2d_1.Sample(tex2D_sampler_1, input.uv) * BaseColor;\n"
		//"   output.color = BaseColor;\n"
		//"	output.color.w += input.vColor.w;\n"
		//"	if(output.color.w>1.f) output.color.w = 1.f;\n"
		//"	output.color.xyz = lerp(output.color.xyz, input.vColor.xyz, input.vColor.www);\n"
		"	if(diff>1.f) diff = 1.f;\n"
		"	output.color.xyz *= diff;\n"
		//"	if((flags & FLAG_NOBLEND) == FLAG_NOBLEND){\n"
		"	if(output.color.w < alphaDiscard) discard;\n"
		//"	}\n"
		//"	if(output.color.w == 0.f) if(input.vColor.w!=1.f)discard;\n"
		"    return output;\n"
		"}\n";
	if (!m_gs->createShaders(
		"vs_5_0",
		"ps_5_0",
		text,
		text,
		"VSMain",
		"PSMain",
		slMeshVertexType::Triangle,
		&this->m_vShader,
		&this->m_pShader,
		&this->m_vLayout))
		return false;

	if (!m_gs->createConstantBuffer(sizeof(cbV), &m_cbV))
		return false;
	if (!m_gs->createConstantBuffer(sizeof(cbP), &m_cbP))
		return false;

	return true;
}

void slD3D11ShaderSolid::SetData(const slMat4& WVP, const slMat4& W)
{
	m_cbDataV.W = W;
	m_cbDataV.WVP = WVP;
}

void slD3D11ShaderSolid::SetConstants(slMaterial* material) {

	slGSD3D11Texture* _t = m_gs->m_whiteTexture;
	if (material->m_maps[0].m_texture)
		_t = (slGSD3D11Texture*)material->m_maps[0].m_texture;

	m_gs->m_d3d11DevCon->PSSetShaderResources(0, 1, &_t->m_textureResView);
	m_gs->m_d3d11DevCon->PSSetSamplers(0, 1, &_t->m_samplerState);

	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D11_BUFFER_DESC d;
		m_gs->m_d3d11DevCon->Map(m_cbV, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		m_cbV->GetDesc(&d);
		memcpy(mappedResource.pData, &m_cbDataV, d.ByteWidth);
		m_gs->m_d3d11DevCon->Unmap(m_cbV, 0);
		m_gs->m_d3d11DevCon->VSSetConstantBuffers(0, 1, &m_cbV);
	}

	m_cbDataP.alphaDiscard = material->m_alphaDiscard;
	
	m_cbDataP.BaseColor = material->m_colorDiffuse;
	m_cbDataP.SunPosition = material->m_sunPosition;

	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D11_BUFFER_DESC d;
		m_gs->m_d3d11DevCon->Map(m_cbP, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		m_cbP->GetDesc(&d);
		memcpy(mappedResource.pData, &m_cbDataP, d.ByteWidth);
		m_gs->m_d3d11DevCon->Unmap(m_cbP, 0);
		m_gs->m_d3d11DevCon->PSSetConstantBuffers(0, 1, &m_cbP);
	}
}