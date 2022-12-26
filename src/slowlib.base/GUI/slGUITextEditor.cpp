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
	m_textEditorTextDrawCallback.m_edtr = this;
	m_textDrawCallback = dynamic_cast<slGUIDrawTextCallback*>(&m_textEditorTextDrawCallback);
	Clear(true);
}

slGUITextEditor::~slGUITextEditor()
{
	if(m_textBuffer)
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
}

void slGUITextEditor::GoLeft()
{
	m_drawTextCursor = true;
	if(m_textCursor)
		--m_textCursor;
}

void slGUITextEditor::GoRight()
{
	m_drawTextCursor = true;
	++m_textCursor;
	if (m_textCursor > m_textBufferLen)
		m_textCursor = m_textBufferLen;
}

void slGUITextEditor::GoHome()
{
	m_drawTextCursor = true;
	if (m_textCursor && m_textBuffer)
	{
		for (size_t i = m_textCursor; i >= 0; --i)
		{
			if (i == 0)
			{
				m_textCursor = 0;
				return;
			}

			if (m_textBuffer[i] == U'\n' && i != m_textCursor)
			{
				m_textCursor = i + 1;
				return;
			}
		}
	}
}

void slGUITextEditor::GoEnd()
{
	m_drawTextCursor = true;
	if (m_textBuffer)
	{
		for (; m_textCursor < m_textBufferLen; )
		{
			if (m_textBuffer[m_textCursor] == U'\n')
				return;
			++m_textCursor;
		}
	}
}

void slGUITextEditor::GoUp()
{
	m_drawTextCursor = true;
	GoHome();
	GoLeft();
	GoHome();

	// now find index
	slVec2f textPosition;
	textPosition.x = m_buildRect.x;
	for (size_t k = m_textCursor; k < m_textBufferLen; ++k)
	{
		if (m_textBuffer[k] == U'\n') // empty line
		{
			m_textCursor = k;
			return;
		}

		slGUIFont* font = m_textDrawCallback->OnFont(0, m_textBuffer[k]);
		slGUIFontGlyph* g = font->GetGlyphMap()[m_textBuffer[k]];

		slVec4f rct;
		rct.x = textPosition.x;

		textPosition.x += g->m_width + g->m_overhang + g->m_underhang + font->m_characterSpacing;

		switch (m_textBuffer[k])
		{
		case U' ':
			textPosition.x += font->m_spaceSize;
			break;
		case U'\t':
			textPosition.x += font->m_tabSize;
			break;
		}

		rct.z = textPosition.x;
		if (m_textCursorPosition.x >= rct.x && m_textCursorPosition.x <= rct.z)
		{
			// find half width
			float h = (rct.z - rct.x) * 0.5f;

			if (m_textCursorPosition.x >= rct.x + h)
			{
				m_textCursor = k + 1;
				return;
			}
			else
			{
				m_textCursor = k;
				return;
			}
		}
	}
}

void slGUITextEditor::GoDown()
{
	m_drawTextCursor = true;
	GoEnd();
	GoRight();
	// now find index
	slVec2f textPosition;
	textPosition.x = m_buildRect.x;
	for (size_t k = m_textCursor; k < m_textBufferLen; ++k)
	{
		if (m_textBuffer[k] == U'\n') // empty line
		{
			m_textCursor = k;
			return;
		}

		slGUIFont* font = m_textDrawCallback->OnFont(0, m_textBuffer[k]);
		slGUIFontGlyph* g = font->GetGlyphMap()[m_textBuffer[k]];

		slVec4f rct;
		rct.x = textPosition.x;

		textPosition.x += g->m_width + g->m_overhang + g->m_underhang + font->m_characterSpacing;

		switch (m_textBuffer[k])
		{
		case U' ':
			textPosition.x += font->m_spaceSize;
			break;
		case U'\t':
			textPosition.x += font->m_tabSize;
			break;
		}

		rct.z = textPosition.x;
		if (m_textCursorPosition.x >= rct.x && m_textCursorPosition.x <= rct.z)
		{
			// find half width
			float h = (rct.z - rct.x) * 0.5f;

			if (m_textCursorPosition.x >= rct.x + h)
			{
				m_textCursor = k + 1;
				return;
			}
			else
			{
				m_textCursor = k;
				return;
			}
		}
	}
}


void slGUITextEditor::Rebuild()
{
	slGUIElement::Rebuild();
}

void slGUITextEditor::Update(slInputData* id)
{
	slGUIElement::Update(id);

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
		if (!IsActivated())
		{
			if(IsClickedLMB()
				|| IsClickedRMB()
				|| IsClickedMMB()
				|| IsClickedX1MB()
				|| IsClickedX2MB())
			Activate(true);
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
		m_drawTextCursor = true;
	}
	else
	{
		g_framework->m_activeTextEditor = 0;
		m_textEditorFlags &= ~textEditorFlag_isActivated;
		m_drawTextCursor = false;
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

	gs->SetScissorRect(m_clipRect, 0);
	if (IsEnabled())
	{
		if (IsDrawBG())
			gs->DrawGUIRectangle(m_buildRect, m_style->m_textEditorBGColor, m_style->m_textEditorBGColor, 0, 0);
	}

	if (m_textBufferLen)
	{
		slVec2f textPosition;
		textPosition.x = m_buildRect.x;
		textPosition.y = m_buildRect.y;

	//	printf("[%u]\n", (uint32_t)m_textBuffer[m_textCursor]);

	//	size_t numOfLines = 0;
		for (size_t i = m_textBeginDrawIndex; i < m_textBufferLen; ++i)
		{
			slGUIFont* font = m_textDrawCallback->OnFont(0, m_textBuffer[i]);

			gs->DrawGUICharacter(
				m_textBuffer[i],
				font,
				textPosition,
				*m_textDrawCallback->OnColor(0, m_textBuffer[i]));

			slGUIFontGlyph* g = font->GetGlyphMap()[m_textBuffer[i]];

			if (m_drawTextCursor && (i == m_textCursor))
			{
				slVec4f rct;
				rct.x = textPosition.x;
				rct.y = textPosition.y;
				rct.z = rct.x + 2.f;
				rct.w = rct.y + font->GetMaxSize().y;
				m_textCursorPosition = textPosition;
				gs->DrawGUIRectangle(rct, GetStyle()->m_textEditorCursorColor, GetStyle()->m_textEditorCursorColor, 0, 0);
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
				textPosition.y += font->m_lineSpacing + font->GetMaxSize().y;
				textPosition.x = m_buildRect.x;
		//		++numOfLines;
				break;
			}

			if (textPosition.y > m_buildRect.w)
				break;

			if (textPosition.x > m_buildRect.z)
			{
				for (; i < m_textBufferLen; ++i)
				{
					if (m_textBuffer[i] == U'\n')
					{
						textPosition.x = m_buildRect.x;
						textPosition.y += font->m_lineSpacing + font->GetMaxSize().y;
						break;
					}
				}
			}
		}

	//	printf("%u\n", numOfLines);
	}
}

