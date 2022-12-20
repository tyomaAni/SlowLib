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
#ifndef __SL_SLOWLIBBASEGUIFONT_H__
#define __SL_SLOWLIBBASEGUIFONT_H__

#include "slowlib.base/containers/slArray.h"

struct slGUIFontGlyph
{
	char32_t m_symbol = U' ';
	slVec4f m_UV;
	slRect m_rect;
	int32_t m_underhang = 0;
	int32_t m_overhang = 0;
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	uint32_t m_textureSlot = 0;
};

class slGUIFont
{
	uint32_t m_textureNum = 0;
	slArray<slTexture*> m_textures;

	slArray<slGUIFontGlyph*> m_glyphs;

	slPoint m_maxSize;

	slGUIFontGlyph* m_glyphMap[0x32000];
public:
	slGUIFont();
	~slGUIFont();

	void AddTexture(slTexture*);
	void AddGlyph(const slGUIFontGlyph&);
	void AddGlyph(char32_t ch, const slVec2f& leftTop, const slPoint& charSz, uint32_t texture, const slPoint& texSz)
	{
		slGUIFontGlyph glyph;
		glyph.m_symbol = ch;
		glyph.m_width = charSz.x;
		glyph.m_height = charSz.y;
		glyph.m_textureSlot = texture;
		glyph.m_UV.x = slMath::coordToUV(leftTop.x, (float)texSz.x);
		glyph.m_UV.y = slMath::coordToUV(leftTop.y, (float)texSz.y);
		glyph.m_UV.z = slMath::coordToUV(leftTop.x + (float)charSz.x, (float)texSz.x);
		glyph.m_UV.w = slMath::coordToUV(leftTop.y + (float)charSz.y, (float)texSz.y);
		AddGlyph(glyph);
	}

	const slPoint& GetMaxSize() { return m_maxSize; }
	slGUIFontGlyph** GetGlyphMap() { return m_glyphMap; }
	slTexture* GetTexture(uint32_t i) { return m_textures.m_data[i]; }

	int32_t m_characterSpacing = 1;
	int32_t m_lineSpacing = 3;
	int32_t m_spaceSize = 4;
	int32_t m_tabSize = 16;
};


#endif
