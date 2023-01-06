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


#ifndef SL_D3D11_SHADER_GUIRCT_H__
#define SL_D3D11_SHADER_GUIRCT_H__

class slGSD3D11;
class slD3D11ShaderGUIRectangle : public slGSD3D11ShaderBase
{
public:
	slD3D11ShaderGUIRectangle(slGSD3D11* gs);
	virtual ~slD3D11ShaderGUIRectangle();

	slGSD3D11* m_gs = 0;

	ID3D11Buffer* m_cbFrame = 0;
	ID3D11Buffer* m_cbElement = 0;

	struct cbFrame
	{
		slMatrix4_t<float> ProjMtx;
	}m_cbDataFrame;
	
	struct cbElement
	{
		slVec4f Corners;
		slColor Color1;
		slColor Color2;
		slVec4f UVs;
	}m_cbDataElement;

	virtual void SetConstants(slMaterial* material);

	void SetData();
	bool init();

	void SetOnFrame();
	void SetOnElement(slGSD3D11Texture*);
};

#endif