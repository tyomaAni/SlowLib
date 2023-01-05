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
#include "slowlib.base/GUI/slGUI.h"
#include "slowlib.base/gs/slGS.h"

#include "../framework/slFrameworkImpl.h"
extern slFrameworkImpl* g_framework;

slGUIStaticTextDrawCallback::slGUIStaticTextDrawCallback()
{
	m_defaultFont = slFramework::GetDefaultFont(slGUIDefaultFont::Text);
	m_currFont = m_defaultFont;
}
slGUIFont* slGUIStaticTextDrawCallback::OnFont(uint32_t r, char32_t c)
{
	return m_currFont;
}
slColor* slGUIStaticTextDrawCallback::OnColor(uint32_t r, char32_t c)
{
	return &m_staticText->GetStyle()->m_staticTextTextColor;
}

slGUIStaticText::slGUIStaticText(slGUIWindow* w, const slVec2f& position, const slVec2f& size)
	:
	slGUIElement::slGUIElement(w, position, size)
{
	m_staticTextTextDrawCallback.m_staticText = this;
	m_textDrawCallback = dynamic_cast<slGUIDrawTextCallback*>(&m_staticTextTextDrawCallback);
}

slGUIStaticText::~slGUIStaticText()
{
}

void slGUIStaticText::SetText(const slString& text)
{
	m_text = text;
}

void slGUIStaticText::Rebuild()
{
	slGUIElement::Rebuild();
	m_lineHeight = 0.f;
	const char32_t b[] = { U"X#$@WHM_" };
	for (int i = 0; i < 9; ++i)
	{
		slGUIFont* font = m_textDrawCallback->OnFont(0, b[i]);
		if (font->GetMaxSize().y > m_lineHeight)
			m_lineHeight = (float)font->GetMaxSize().y;
	}
}

void slGUIStaticText::Update()
{
	slGUIElement::Update();
}

void slGUIStaticText::Draw(slGS* gs, float dt)
{
	gs->SetScissorRect(m_clipRect, 0);
	if (IsDrawBG())
		gs->DrawGUIRectangle(m_buildRect, m_style->m_staticTextBGColor, m_style->m_staticTextBGColor, 0, 0);

	auto sz = m_text.size();
	if (sz)
	{
		slVec2f pos;
		pos.x = m_buildRect.x;
		pos.y = m_buildRect.y;

		slVec2f textPosition = pos;

		slVec4f r;
		r.x = pos.x;
		r.y = pos.y;
		r.z = m_buildRect.z;
		r.w = r.y;

		bool draw = true;
		for (size_t i = 0; i < sz; ++i)
		{
			if (m_useDrawLimit)
			{
				if (i >= m_drawLimit)
					draw = false;
			}

			char32_t ch = m_text[i];
			slGUIFont* font = m_textDrawCallback->OnFont(0, ch);
			slGUIFontGlyph* g = font->GetGlyphMap()[ch];

			slVec4f chrct;
			chrct.x = textPosition.x;
			chrct.y = textPosition.y;
			chrct.z = chrct.x + g->m_width + g->m_overhang + g->m_underhang + font->m_characterSpacing;
			chrct.w = chrct.y + m_lineHeight;

			if (chrct.z > m_buildRect.z)
			{
				textPosition.y += m_lineHeight;
				textPosition.x = pos.x;
			}

			
			if (draw)
			{
				gs->DrawGUICharacter(
					ch,
					font,
					textPosition,
					*m_textDrawCallback->OnColor(0, ch));
			}

			textPosition.x += g->m_width + g->m_overhang + g->m_underhang + font->m_characterSpacing;

			switch (ch)
			{
			case U' ':
				textPosition.x += font->m_spaceSize;
				break;
			case U'\t':
				textPosition.x += font->m_tabSize;
				break;
			case U'\n':
				textPosition.x = pos.x;
				textPosition.y += m_lineHeight;
				break;
			}
		}
	}
}

