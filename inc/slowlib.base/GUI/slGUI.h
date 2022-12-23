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
#ifndef __SL_SLOWLIBBASEGUI_H__
#define __SL_SLOWLIBBASEGUI_H__

#include "slowlib.base/GUI/slGUIStyle.h"
#include "slowlib.base/GUI/slGUIFont.h"

class slGUIDrawTextCallback : public slUserData
{
public:
	slGUIDrawTextCallback() {}
	virtual ~slGUIDrawTextCallback() {}

	enum {
		Reason_default,
		Reason_mouseEnter,
		Reason_mouseLeave,
		Reason_mouseAbove,
		Reason_pressed,
		Reason_active,
		Reason_disabled,
		/*Reason_, // others
		Reason_,*/
	};

	uint32_t m_reason = Reason_default;
	virtual slGUIFont* OnFont(uint32_t, char32_t) = 0;
	virtual slColor* OnColor(uint32_t, char32_t) = 0;

	virtual slVec2f GetTextSize(const char32_t* text)
	{
		slVec2f sz, position;
		slVec4f rct;
		if (text)
		{
			if (text[0] != 0)
			{
				const char32_t* p = &text[0u];
				while ((size_t)*p++)
				{
					auto font = OnFont(Reason_default, *p);
					slGUIFontGlyph* g = font->GetGlyphMap()[*p];

					rct.x = position.x;
					rct.y = position.y;

					rct.z = rct.x + g->m_width;
					rct.w = rct.y + g->m_height;

					position.x += g->m_width + g->m_overhang + g->m_underhang + font->m_characterSpacing;

					switch (*p)
					{
					case U' ':
						position.x += font->m_spaceSize;
						break;
					case U'\t':
						position.x += font->m_tabSize;
						break;
					case U'\n':
						position.y += font->m_lineSpacing + font->GetMaxSize().y;
						position.x = 0.f;
						break;
					}

					if (position.x > sz.x) sz.x = position.x;
					if (position.y > sz.y) sz.y = position.y;
					if (g->m_height > sz.y) sz.y = (float)g->m_height;
				}
			}
		}
		return sz;
	}
};


#include "slowlib.base/GUI/slGUIElement.h"
#include "slowlib.base/GUI/slGUIWindow.h"
#include "slowlib.base/GUI/slGUIButton.h"


struct slGUIState
{
	slGUIWindow* m_windowUnderCursor = 0;
	slGUIWindow* m_activeWindow = 0;
};

class slGUIRootElement : public slGUIElement
{
public:
	slGUIRootElement(slGUIWindow*, const slVec2f& position, const slVec2f& size);
	virtual ~slGUIRootElement();
	virtual void Rebuild() final;
	virtual void Update(slInputData*) final;
	virtual void Draw(slGS* gs, float dt) final;
};

#endif
