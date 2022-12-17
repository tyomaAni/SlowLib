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
#ifndef __SL_SLOWLIBBASETEXTURE_H__
#define __SL_SLOWLIBBASETEXTURE_H__

#include "slowlib.base/gs/slImage.h"

enum class slTextureType : uint32_t
{
	Texture2D,
	RTT
};

enum class slTextureComparisonFunc : uint32_t
{
	Never,
	Less,
	Equal,
	LessEqual,
	Greater,
	NotEqual,
	GreaterEqual,
	Always
};

enum class slTextureAddressMode : uint32_t
{
	Wrap,
	Mirror,
	Clamp,
	Border,
	MirrorOnce
};

enum class slTextureFilter : uint32_t
{
	// min mag mip / point linear
	PPP,
	PPL,
	PLP,
	PLL,
	LPP,
	LPL,
	LLP,
	LLL,
	ANISOTROPIC,
	// comparison
	CMP_PPP,
	CMP_PPL,
	CMP_PLP,
	CMP_PLL,
	CMP_LPP,
	CMP_LPL,
	CMP_LLP,
	CMP_LLL,
	CMP_ANISOTROPIC,
};

struct slTextureInfo
{
	slImageInfo m_imageInfo;
	slTextureType m_type = slTextureType::Texture2D;
	slTextureComparisonFunc m_cmpFnc = slTextureComparisonFunc::Always;
	slTextureAddressMode m_adrMode = slTextureAddressMode::Wrap;
	slTextureFilter m_filter = slTextureFilter::PPP;
	uint32_t m_anisotropicLevel = 1;
	bool m_generateMipmaps = false;
};

class slTexture
{
protected:
	slTextureInfo m_info;
public:
	slTexture() {}
	virtual ~slTexture() {}

	const slTextureInfo& GetInfo() { return m_info; }
};

#endif
