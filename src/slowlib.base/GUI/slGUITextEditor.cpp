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

slGUITextEditorTextDrawCallback::slGUITextEditorTextDrawCallback()
{
	m_defaultFont = slFramework::GetDefaultFont(slGUIDefaultFont::Text);
	m_currFont = m_defaultFont;
}
slGUIFont* slGUITextEditorTextDrawCallback::OnFont(uint32_t r, char32_t c)
{
	return m_currFont;
}
slColor* slGUITextEditorTextDrawCallback::OnColor(uint32_t r, char32_t c)
{
	switch (r)
	{
	case slGUIDrawTextCallback::Reason_selected:
		return &m_edtr->GetStyle()->m_textEditorSelectedTextColor;
		break;
	}
	return &m_edtr->GetStyle()->m_textEditorTextColor;
}

slGUITextEditor::slGUITextEditor(slGUIWindow* w, const slVec2f& position, const slVec2f& size)
	:
	slGUIElement::slGUIElement(w, position, size)
{
	m_flags |= slGUICommon::flag_wheelScroll;
	m_textEditorTextDrawCallback.m_edtr = this;
	m_textDrawCallback = dynamic_cast<slGUIDrawTextCallback*>(&m_textEditorTextDrawCallback);
	Clear(true);
	findTextCursorRect();
}

slGUITextEditor::~slGUITextEditor()
{
	if (m_textBuffer)
		slMemory::free(m_textBuffer);
}

void slGUITextEditor::reallocate(size_t newSize)
{
	char32_t* newBuffer = (char32_t*)slMemory::malloc((newSize + 1) * sizeof(char32_t));
	for (uint32_t i = 0; i < m_textBufferLen; ++i)
	{
		newBuffer[i] = m_textBuffer[i];
	}
	newBuffer[m_textBufferLen] = 0;
	m_textBufferAllocated = newSize;
	slMemory::free(m_textBuffer);
	m_textBuffer = newBuffer;
}

void slGUITextEditor::findNumberOfLines()
{
	m_numberOfLines = 1; // minimum 1 line even if there is no text
	m_lines.clear();
	m_lines.push_back(LineInfo(0, 1, 0));

	LineInfo* li = &m_lines.m_data[0];
	++li->m_size;

	size_t ln = 1;
	for (uint32_t i = 0; i < m_textBufferLen; ++i)
	{
		// if we have \n then next character is first in new line
		if (m_textBuffer[i] == U'\n')
		{
			++m_numberOfLines;
			m_lines.push_back(LineInfo(i + 1, ++ln, 0));
			li = &m_lines.m_data[m_lines.m_size - 1];
		}
		++li->m_size;
	}
}

void slGUITextEditor::Clear(bool freeMemory)
{
	if (freeMemory)
	{
		if (m_textBuffer)
			slMemory::free(m_textBuffer);

		m_textBufferAllocated = 10;
		m_textBuffer = (char32_t*)slMemory::malloc((m_textBufferAllocated + 1) * sizeof(char32_t));
	}

	m_textBufferLen = 0;
	m_textBuffer[0] = 0;
	findNumberOfLines();
	findTextCursorRect();
	findHScroll();
	findVScroll();
}

void slGUITextEditor::SetText(const slString& text)
{
	size_t len = text.size();
	if (len)
	{
		if (len > m_textBufferAllocated)
			reallocate(len);
		Clear(false);
		for (uint32_t i = 0; i < len; ++i)
		{
			m_textBuffer[i] = text.c_str()[i];
		}
		m_textBufferLen = len;
		m_textBuffer[len] = 0;
	}
	else
	{
		m_textBufferLen = 0;
		m_textBuffer[0] = 0;
	}
	findNumberOfLines();
	findTextCursorRect();
	findHScroll();
	findVScroll();
}

void slGUITextEditor::GoLeft()
{
	drawTextCursor();
	if (m_textCursor)
	{
		--m_textCursor;
		if (m_col > 1)
			--m_col;
		else
		{
			if (m_line > 1)
			{
				--m_line;
				m_col = m_lines[m_line - 1].m_size;
			}
		}
	}

	m_colFix = m_col;
	findTextCursorRect();
	findVScroll();
	findHScroll();
}

void slGUITextEditor::GoRight()
{
	drawTextCursor();

	auto prev = m_textCursor;

	++m_textCursor;
	if (m_textCursor > m_textBufferLen)
	{
		m_textCursor = m_textBufferLen;
	}
	else
	{
		++m_col;
		if (m_textBuffer[prev] == U'\n')
		{
			m_col = 1;
			++m_line;
		}
	}

	m_colFix = m_col;
	findTextCursorRect();
	findVScroll();
	findHScroll();
}

void slGUITextEditor::GoHome()
{
	drawTextCursor();
	if (m_col > 1)
	{
		m_textCursor = m_lines[m_line - 1].m_index;
		m_col = 1;
	}
	m_colFix = m_col;
	findTextCursorRect();
	findHScroll();
}

void slGUITextEditor::GoEnd()
{
	drawTextCursor();
	m_col = m_lines[m_line - 1].m_size;
	m_textCursor = m_lines[m_line - 1].m_index + (m_col-1);
	m_colFix = m_col;
	findTextCursorRect();
	findHScroll();
}

void slGUITextEditor::GoUp()
{
	drawTextCursor();

	if (m_line > 1)
	{
		--m_line;
		printf("sz %i col %i colFix %i\n", m_lines[m_line - 1].m_size, m_col, m_colFix);

		if (m_lines[m_line - 1].m_size < m_colFix)
			m_col = m_lines[m_line - 1].m_size;
		else
			m_col = m_colFix;
		m_textCursor = m_lines[m_line - 1].m_index + (m_col -1);
	}
	findTextCursorRect();
	findVScroll();
	findHScroll();
}

void slGUITextEditor::GoDown()
{
	drawTextCursor();
	if (m_line - 1 < m_lines.m_size-1)
	{
		++m_line;
		
		printf("sz %i col %i colFix %i\n", m_lines[m_line - 1].m_size, m_col, m_colFix);

		if (m_lines[m_line - 1].m_size < m_colFix)
			m_col = m_lines[m_line - 1].m_size;
		else 
			m_col = m_colFix;
		m_textCursor = m_lines[m_line - 1].m_index + (m_col - 1);
	}
	findTextCursorRect();
	findVScroll();
	findHScroll();
}

void slGUITextEditor::UpdateContentSize()
{
	m_contentSize.x = m_baseRect.z - m_baseRect.x;
	m_contentSize.y = 0.f;

	if (m_numberOfLines)
	{
		m_contentSize.y = (float)(m_numberOfLines)*m_lineHeight;

		// + when scroll to bottom there is must 1 line be visible
		m_contentSize.y += (m_baseRect.w - m_baseRect.y) - m_lineHeight;
	}

	UpdateScrollLimit();
}

void slGUITextEditor::Rebuild()
{
	slGUIElement::Rebuild();

	m_lineHeight = 0.f;
	const char32_t b[] = { U"X#$@WHM_" };
	for (int i = 0; i < 9; ++i)
	{
		slGUIFont* font = m_textDrawCallback->OnFont(0, m_textBuffer[i]);
		if (font->GetMaxSize().y > m_lineHeight)
			m_lineHeight = (float)font->GetMaxSize().y;
	}

	m_numberOfVisibleLines = 0;
	if (m_lineHeight && (m_buildRect.w - m_buildRect.y))
	{
		m_numberOfVisibleLines = (size_t)ceilf((m_buildRect.w - m_buildRect.y) / m_lineHeight);
	}
	//findVScroll();
	//findHScroll();
	printf("Num of lines: %i\n", m_numberOfLines);
	printf("Num of visible lines: %i\n", m_numberOfVisibleLines);
}

void slGUITextEditor::Update()
{
	slGUIElement::Update();
	m_isLMBHit = false;

	if (!m_lineHeight)
		return;

	if (IsActivated())
	{
		if (slInput::IsKeyHit(slInput::KEY_LEFT))
			GoLeft();
		if (slInput::IsKeyHit(slInput::KEY_RIGHT))
			GoRight();
		if (slInput::IsKeyHit(slInput::KEY_UP))
			GoUp();
		if (slInput::IsKeyHit(slInput::KEY_DOWN))
			GoDown();
		if (slInput::IsKeyHit(slInput::KEY_HOME))
			GoHome();
		if (slInput::IsKeyHit(slInput::KEY_END))
			GoEnd();
	}

	if (IsCursorInRect())
	{
		if (slInput::IsLMBHit())
		{
			m_isLMBHit = true;
			m_skipDraw = true;
			Draw(0, 0.f);
		}

		if (!IsActivated())
		{
			if (IsClickedLMB()
				|| IsClickedRMB()
				|| IsClickedMMB()
				|| IsClickedX1MB()
				|| IsClickedX2MB())
				Activate(true);
		}

		if (g_framework->m_input.mouseWheelDelta < 0.f)
		{
			m_v_scroll += 10.f;
			if (m_v_scroll > m_scrollLimit.y)
				m_v_scroll = m_scrollLimit.y;
		}else if (g_framework->m_input.mouseWheelDelta > 0.f)
		{
			m_v_scroll -= 10.f;
			if (m_v_scroll < 0.f)
				m_v_scroll = 0.f;
		}
		
	}
	else
	{
		if (IsActivated())
		{
			if (slInput::IsLMBHit()
				|| slInput::IsRMBHit()
				|| slInput::IsMMBHit()
				|| slInput::IsX1MBHit()
				|| slInput::IsX2MBHit())
			{
				Activate(false);
			}
		}
	}

	// find m_firstItemIndexForDraw, use only m_v_scroll
	slVec2f pos;
	pos.x = m_buildRect.x;
	pos.y = m_buildRect.y - m_v_scroll;
	pos.y += (m_lineHeight * (float)m_firstItemIndexForDraw);
	uint32_t itemsSize = m_numberOfLines + m_numberOfVisibleLines;


	if (pos.y < (m_buildRect.y - m_lineHeight - m_lineHeight))
	{
		float d = (m_buildRect.y - m_lineHeight - m_lineHeight) - pos.y;
		int dd = (int)ceilf(d / m_lineHeight);

		//++m_firstItemIndexForDraw;
		m_firstItemIndexForDraw += dd;

		uint32_t lastIndex = itemsSize - m_numberOfLines;
		if (m_numberOfVisibleLines > lastIndex)
			m_numberOfVisibleLines = lastIndex;
	}
	else if (pos.y > (m_buildRect.y - m_lineHeight - m_lineHeight))
	{
		if (m_firstItemIndexForDraw)
		{
			float d = pos.y - (m_buildRect.y - m_lineHeight - m_lineHeight);
			int dd = (int)ceilf(d / m_lineHeight);

			//--m_firstItemIndexForDraw;
			if ((size_t)dd < m_firstItemIndexForDraw)
				m_firstItemIndexForDraw -= dd;
			else
				m_firstItemIndexForDraw = 0;
		}
	}
	
	slGUIElement* parent = dynamic_cast<slGUIElement*>(GetParent());

	if (parent->m_scrollDelta.y != 0.f)
	{
		findTextCursorRect();
	}
}

void slGUITextEditor::Activate(bool v)
{
	if (g_framework->m_activeTextEditor && g_framework->m_activeTextEditor != this)
	{
		g_framework->m_activeTextEditor->Activate(false);
	}

	if (v)
	{
		g_framework->m_activeTextEditor = this;
		m_textEditorFlags |= textEditorFlag_isActivated;
		drawTextCursor();
		findTextCursorRect();
		findVScroll();
		findHScroll();
	}
	else
	{
		g_framework->m_activeTextEditor = 0;
		m_textEditorFlags &= ~textEditorFlag_isActivated;
		m_drawTextCursor = false;
	}
}

void slGUITextEditor::findHScroll()
{
	if (IsActivated())
	{
		slVec4f buildRectWithScroll = m_buildRect;
		buildRectWithScroll.x += m_h_scroll;
		buildRectWithScroll.z += m_h_scroll;
		buildRectWithScroll.y += m_v_scroll;
		buildRectWithScroll.w += m_v_scroll;

		if (m_textCursorRect.x < buildRectWithScroll.x)
			m_h_scroll -= buildRectWithScroll.x - m_textCursorRect.x;
		else if (m_textCursorRect.z > buildRectWithScroll.z)
			m_h_scroll += m_textCursorRect.z - buildRectWithScroll.z;

		if (m_h_scroll < 0.f)
			m_h_scroll = 0.f;
	}
}

void slGUITextEditor::findVScroll()
{
	if (IsActivated())
	{
		slVec4f buildRectWithScroll = m_buildRect;
		buildRectWithScroll.x += m_h_scroll;
		buildRectWithScroll.z += m_h_scroll;
		buildRectWithScroll.y += m_v_scroll;
		buildRectWithScroll.w += m_v_scroll;

		if (m_textCursorRect.y < buildRectWithScroll.y)
			m_v_scroll -= buildRectWithScroll.y - m_textCursorRect.y;
		else if (m_textCursorRect.w > buildRectWithScroll.w)
			m_v_scroll += m_textCursorRect.w - buildRectWithScroll.w;

		if (m_v_scroll < 0.f)
			m_v_scroll = 0.f;
	}
}

void slGUITextEditor::findTextCursorRect()
{
	m_textCursorRect.set(0.f);
	slVec2f pos;
	pos.x = m_buildRect.x;
	pos.y = m_buildRect.y;
		
	slVec2f textPosition = pos;
	float fontMaxSizeY = 0.f;
	for (size_t i = 0; i < m_textBufferLen; ++i)
	{
		char32_t ch = m_textBuffer[i];
		slGUIFont* font = m_textDrawCallback->OnFont(0, ch);
		slGUIFontGlyph* g = font->GetGlyphMap()[ch];
		if (font->GetMaxSize().y > fontMaxSizeY)
			fontMaxSizeY = (float)font->GetMaxSize().y;
		if (i == m_textCursor)
		{
			m_textCursorRect.x = textPosition.x;
			m_textCursorRect.y = textPosition.y;
			m_textCursorRect.z = m_textCursorRect.x + 2.f;
			m_textCursorRect.w = m_textCursorRect.y + fontMaxSizeY;
			return;
		}
		textPosition.x += g->m_width + g->m_overhang + g->m_underhang + font->m_characterSpacing;
		switch (m_textBuffer[i])
		{
		case U' ':
			textPosition.x += font->m_spaceSize;
			break;
		case U'\t':
			textPosition.x += font->m_tabSize;
			break;
		case U'\n':
			textPosition.y += m_lineHeight;
			textPosition.x = pos.x;
			break;
		}
	}

	if (m_textCursor == m_textBufferLen)
	{
		m_textCursorRect.x = textPosition.x;
		m_textCursorRect.y = textPosition.y;
		m_textCursorRect.z = m_textCursorRect.x + 2.f;
		m_textCursorRect.w = m_textCursorRect.y + fontMaxSizeY;
	}
}

void slGUITextEditor::Draw(slGS* gs, float dt)
{
	if (IsActivated())
	{
		m_textCursorTimer += dt;
		if (m_textCursorTimer > m_textCursorTimerLimit)
		{
			m_textCursorTimer = 0.f;
			m_drawTextCursor = m_drawTextCursor ? false : true;
		}
	}

	if (IsEnabled() && !m_skipDraw)
	{
		gs->SetScissorRect(m_clipRect, 0);
		if (IsDrawBG())
			gs->DrawGUIRectangle(m_buildRect, m_style->m_textEditorBGColor, m_style->m_textEditorBGColor, 0, 0);
	}

	if (m_textBufferLen)
	{
		uint32_t index = m_firstItemIndexForDraw;

		slVec2f pos;
		pos.x = m_buildRect.x - m_h_scroll;
		pos.y = m_buildRect.y - m_v_scroll;
		pos.y += (m_lineHeight * (float)index);

		uint32_t itemsSize = m_numberOfLines;
		
		// when click LMB. get m_textCursor index.
		//size_t clickChar = 0;
		// when click in row but not in char (cursorPosition.x > last char rect)
		//size_t clickCharNotInRect = 0;
		//bool probablyClickWasBelowText = false;
		//bool cursorInLine = false;
		
		// or not under. if cursor is out of last char in the line (cursor.x > charRect.z)
		//   then index must be last char in this line
		// if cursor is not in the line (below text), then index is m_textBufferLen
		size_t charIndexUnderCursor = 0;
		bool probablyClickWasBelowText = true;

		for (uint32_t i = 0; i < m_numberOfVisibleLines + 4; ++i)
		{
			slVec2f textPosition = pos;

			slVec4f r;
			r.x = pos.x;
			r.y = pos.y;
			r.z = m_buildRect.z;
			r.w = r.y + m_lineHeight;


			if (IsDrawBG() && !m_skipDraw)
			{
				slColor cc;
				cc = GetStyle()->m_textEditorLine1BGColor;
				if (index)
				{
					if ((index % 2) != 0)
						cc = GetStyle()->m_textEditorLine2BGColor;
				}

				gs->DrawGUIRectangle(r, cc, cc, 0, 0);
			}

			float fontMaxSizeY = 0.f;


			size_t last = m_textBufferLen + 1;
			for (size_t o = m_lines.m_data[index].m_index; o < last; ++o)
			{
				char32_t ch = m_textBuffer[o];
				slGUIFont* font = m_textDrawCallback->OnFont(0, ch);
				slGUIFontGlyph* g = font->GetGlyphMap()[ch];

				if (font->GetMaxSize().y > fontMaxSizeY)
					fontMaxSizeY = (float)font->GetMaxSize().y;

				slVec4f chrct;
				chrct.x = textPosition.x;
				chrct.y = textPosition.y;
				chrct.z = chrct.x + g->m_width + g->m_overhang + g->m_underhang + font->m_characterSpacing;
				chrct.w = chrct.y + m_lineHeight;
				if (!g->m_height)
					chrct.w += font->GetMaxSize().y;

				auto& mp = slInput::GetData()->mousePosition;
				if ((mp.y >= chrct.y) && (mp.y <= chrct.w))
				{
					probablyClickWasBelowText = false;
					if (mp.x >= chrct.x)
					{
						charIndexUnderCursor = o;
					}
				}


				if (ch && !m_skipDraw)
					gs->DrawGUICharacter(
						m_textBuffer[o],
						font,
						textPosition,
						*m_textDrawCallback->OnColor(0, m_textBuffer[o]));

				if (m_drawTextCursor)
				{
					if (o == m_textCursor)
					{
						if (!m_skipDraw)
						{
							auto fnl = m_textCursorRect;
							fnl.y -= m_v_scroll;
							fnl.w -= m_v_scroll;
							fnl.x -= m_h_scroll;
							fnl.z -= m_h_scroll;

							gs->DrawGUIRectangle(fnl, GetStyle()->m_textEditorCursorColor, GetStyle()->m_textEditorCursorColor, 0, 0);
						}
					}
				}

				textPosition.x += g->m_width + g->m_overhang + g->m_underhang + font->m_characterSpacing;

				switch (m_textBuffer[o])
				{
				case U' ':
					textPosition.x += font->m_spaceSize;
					break;
				case U'\t':
					textPosition.x += font->m_tabSize;
					break;
				case U'\n':
					textPosition.x = pos.x;

					o = m_textBufferLen;
					break;
				}

			}

			pos.y = pos.y + m_lineHeight;

			++index;
			if (index >= itemsSize)
				break;
		}

		if (m_isLMBHit)
		{
			if (probablyClickWasBelowText)
			{
				m_textCursor = m_textBufferLen;
			}
			else
			{
				m_textCursor = charIndexUnderCursor;
			}

			findTextCursorRect();
			findColAndLineByTextCursor();
			drawTextCursor();
			findHScroll();
			findVScroll();
		}

		//	printf("[%u]\n", (uint32_t)m_textCursor);
		//	printf("m_textBeginDrawIndex: %u\n", m_textBeginDrawIndex);
	}

	m_skipDraw = false;
}

void slGUITextEditor::findColAndLineByTextCursor()
{
	m_line = 1;
	m_col = 1;

	for (uint32_t i = 0; i < m_textBufferLen; ++i)
	{
		if (i == m_textCursor)
			break;

		++m_col;

		if (m_textBuffer[i] == U'\n')
		{
			m_col = 1;
			++m_line;
		}
		
	}

	m_colFix = m_col;
}
