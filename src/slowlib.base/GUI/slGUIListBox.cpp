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

slGUIListBoxTextDrawCallback::slGUIListBoxTextDrawCallback()
{
	m_defaultFont = slFramework::GetDefaultFont(slGUIDefaultFont::Text);
	m_currFont = m_defaultFont;
}
slGUIFont* slGUIListBoxTextDrawCallback::OnFont(uint32_t r, char32_t c)
{
	return m_currFont;
}
slColor* slGUIListBoxTextDrawCallback::OnColor(uint32_t r, char32_t c)
{
	switch (r)
	{
	case slGUIDrawTextCallback::Reason_selected:
		return &m_lb->GetStyle()->m_textEditorSelectedTextColor;
		break;
	}
	return &m_lb->GetStyle()->m_textEditorTextColor;
}

slGUIListBox::slGUIListBox(slGUIWindow* w, const slVec2f& position, const slVec2f& size)
	:
	slGUIElement::slGUIElement(w, position, size)
{
	m_flags |= slGUICommon::flag_wheelScroll;
	m_listBoxTextDrawCallback.m_lb = this;
	m_textDrawCallback = dynamic_cast<slGUIDrawTextCallback*>(&m_listBoxTextDrawCallback);
}

slGUIListBox::~slGUIListBox()
{
	for (size_t i = 0; i < m_items.m_size; ++i)
	{
		slDestroy(m_items.m_data[i]);
	}
}

void slGUIListBox::Rebuild()
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
	m_numberOfVisibleLines = 0;
	if (m_lineHeight && (m_buildRect.w - m_buildRect.y))
	{
		m_numberOfVisibleLines = (size_t)ceilf((m_buildRect.w - m_buildRect.y) / m_lineHeight);
	}
}

void slGUIListBox::UpdateContentSize()
{
	m_contentSize.x = m_baseRect.z - m_baseRect.x;
	m_contentSize.y = 0.f;

	if (m_items.m_size)
	{
		m_contentSize.y = (float)(m_items.m_size)*m_lineHeight;

		// + when scroll to bottom there is must 1 line be visible
		m_contentSize.y += (m_baseRect.w - m_baseRect.y) - m_lineHeight;
	}

	UpdateScrollLimit();
}

void slGUIListBox::Update()
{
	slGUIElement::Update();

	// find m_firstItemIndexForDraw, use only m_v_scroll
	slVec2f pos;
	pos.x = m_buildRect.x;
	pos.y = m_buildRect.y - 0.f;
	pos.y += (m_lineHeight * (float)m_firstItemIndexForDraw);
	uint32_t itemsSize = m_items.m_size + m_numberOfVisibleLines;

	if (pos.y < (m_buildRect.y - m_lineHeight - m_lineHeight))
	{
		float d = (m_buildRect.y - m_lineHeight - m_lineHeight) - pos.y;
		int dd = (int)ceilf(d / m_lineHeight);

		m_firstItemIndexForDraw += dd;

		uint32_t lastIndex = itemsSize - m_items.m_size;
		if (m_numberOfVisibleLines > lastIndex)
			m_numberOfVisibleLines = lastIndex;
	}
	else if (pos.y > (m_buildRect.y - m_lineHeight - m_lineHeight))
	{
		if (m_firstItemIndexForDraw)
		{
			float d = pos.y - (m_buildRect.y - m_lineHeight - m_lineHeight);
			int dd = (int)ceilf(d / m_lineHeight);

			if ((size_t)dd < m_firstItemIndexForDraw)
				m_firstItemIndexForDraw -= dd;
			else
				m_firstItemIndexForDraw = 0;
		}
	}
}

void slGUIListBox::Draw(slGS* gs, float dt)
{
	if (IsEnabled())
	{
		gs->SetScissorRect(m_clipRect, 0);
		if (IsDrawBG())
			gs->DrawGUIRectangle(m_buildRect, m_style->m_staticTextBGColor, m_style->m_staticTextBGColor, 0, 0);
	}

	if (m_items.m_size)
	{
		uint32_t index = m_firstItemIndexForDraw;

		slVec2f pos;
		pos.x = m_buildRect.x;
		pos.y = m_buildRect.y - m_scroll.y;
		pos.y += (m_lineHeight * (float)index);
		uint32_t itemsSize = m_items.m_size + m_numberOfVisibleLines;

		if (pos.y < (m_baseRect.y - m_lineHeight - m_lineHeight))
		{
			++m_firstItemIndexForDraw;

			uint32_t lastIndex = itemsSize - m_items.m_size;
			if (m_numberOfVisibleLines > lastIndex)
				m_numberOfVisibleLines = lastIndex;
		}
		else if (pos.y > (m_baseRect.y - m_lineHeight))
		{
			if (m_firstItemIndexForDraw)
				--m_firstItemIndexForDraw;
		}

		for (uint32_t i = 0; i < m_numberOfVisibleLines + 4; ++i)
		{
			slVec4f r;
			r.x = pos.x;
			r.y = pos.y;
			r.z = m_buildRect.z;
			r.w = r.y + m_lineHeight;


			slVec4f rClip = r;
			if (rClip.y < m_clipRect.y)
				rClip.y = m_clipRect.y;
			if (m_drawItemBG)
			{
				slColor cc;
				cc = m_style->m_listboxLine1BGColor;
				if (index)
				{
					if ((index % 2) != 0)
						cc = m_style->m_listboxLine2BGColor;
				}
				
				if (m_items.m_data[i]->m_isSelected)
					cc = m_style->m_listboxSelectedLineBGColor;

				gs->DrawGUIRectangle(r, cc, cc, 0, 0);
			}

			if (m_items.m_data[i]->m_text.size())
			{
				gs->DrawGUIText(
					m_items.m_data[i]->m_text.c_str(),
					m_items.m_data[i]->m_text.size(),
					pos,
					m_textDrawCallback);
			}
			
			if (slMath::pointInRect(slInput::GetData()->mousePosition, r))
			{
				if (slInput::IsLMBHit())
				{
					if (m_items.m_data[i]->m_isSelected)
					{
						if (OnDeSelect(m_items.m_data[i]))
						{
							m_items.m_data[i]->m_isSelected = false;
						}
					}
					else
					{
						if (OnSelect(m_items.m_data[i]))
						{
							if (!m_multiSelect)
							{
								for (size_t i2 = 0; i2 < m_items.m_size; ++i2)
								{
									m_items.m_data[i2]->m_isSelected = false;
								}
							}

							m_items.m_data[i]->m_isSelected = true;
						}
					}
				}
			}

			pos.y = pos.y + m_lineHeight;

			++index;
			if (index >= itemsSize)
				break;
		}
	}
	/*auto sz = m_text.size();
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
	}*/
}

slGUIListBoxItem* slGUIListBox::AddItem(const char32_t* t, uint32_t id, void* data)
{
	slGUIListBoxItem* newItem = slCreate<slGUIListBoxItem>();

	newItem->m_text = t;
	newItem->m_data = data;
	newItem->m_id = id;

	m_items.push_back(newItem);

	return newItem;
}

void slGUIListBox::RemoveItem(slGUIListBoxItem* it)
{
}
