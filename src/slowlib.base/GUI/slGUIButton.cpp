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

slGUIFont* slGUIButtonTextDrawCallback::OnFont(uint32_t r, char32_t)
{
	return slFramework::GetDefaultFont(slGUIDefaultFont::Text);
}

slColor* slGUIButtonTextDrawCallback::OnColor(uint32_t r, char32_t)
{
	switch (r)
	{
	case slGUIDrawTextCallback::Reason_pressed:
		return &m_button->GetStyle()->m_buttonMousePressTextColor;
	case slGUIDrawTextCallback::Reason_mouseAbove:
		return &m_button->GetStyle()->m_buttonMouseHoverTextColor;
	case slGUIDrawTextCallback::Reason_disabled:
		return &m_button->GetStyle()->m_buttonDisabledTextColor;
	}
	return &m_button->GetStyle()->m_buttonTextColor;
}

slGUIButton::slGUIButton(slGUIWindow* w, const slVec2f& position, const slVec2f& size)
	: 
	slGUIElement::slGUIElement(w, position, size) 
{
	m_defaultTextDrawCallback.m_button = this;
	m_textDrawCallback = dynamic_cast<slGUIDrawTextCallback*>(&m_defaultTextDrawCallback);
}

slGUIButton::~slGUIButton(){}

void slGUIButton::Rebuild()
{
	slGUIElement::Rebuild();
	UpdateTextPosition();
}

void slGUIButton::Update(slInputData* id)
{
	slGUIElement::Update(id);
}

void slGUIButton::Draw(slGS* gs, float dt)
{
	gs->SetScissorRect(m_clipRect, 0);
	if (IsEnabled())
	{
		if (IsClickedLMB())
		{
			if (IsDrawBG())
				gs->DrawGUIRectangle(m_buildRect, m_style->m_buttonMousePressBGColor1, m_style->m_buttonMousePressBGColor2, 0, 0);
			m_textDrawCallback->m_reason = slGUIDrawTextCallback::Reason_pressed;
			gs->DrawGUIText(m_text.c_str(), m_text.size(), m_textPosition, m_textDrawCallback);
		}
		else
		{
			if (IsCursorInRect())
			{
				if (IsDrawBG())
					gs->DrawGUIRectangle(m_buildRect, m_style->m_buttonMouseHoverBGColor1, m_style->m_buttonMouseHoverBGColor2, 0, 0);
				m_textDrawCallback->m_reason = slGUIDrawTextCallback::Reason_mouseAbove;
				gs->DrawGUIText(m_text.c_str(), m_text.size(), m_textPosition, m_textDrawCallback);
			}
			else
			{
				if (IsDrawBG())
					gs->DrawGUIRectangle(m_buildRect, m_style->m_buttonBGColor1, m_style->m_buttonBGColor2, 0, 0);
				m_textDrawCallback->m_reason = slGUIDrawTextCallback::Reason_default;
				gs->DrawGUIText(m_text.c_str(), m_text.size(), m_textPosition, m_textDrawCallback);
			}
		}
	}
	else
	{
		if (IsDrawBG())
			gs->DrawGUIRectangle(m_buildRect, m_style->m_buttonDisabledBGColor1, m_style->m_buttonDisabledBGColor2, 0, 0);
		m_textDrawCallback->m_reason = slGUIDrawTextCallback::Reason_disabled;
		gs->DrawGUIText(m_text.c_str(), m_text.size(), m_textPosition, m_textDrawCallback);
	}
}

void slGUIButton::UpdateTextPosition()
{
	m_textPosition.x = m_buildRect.x;
	m_textPosition.y = m_buildRect.y;
	
	auto tsz = m_textDrawCallback->GetTextSize(m_text.c_str());
	m_textPosition.y = m_buildRect.y + tsz.y;

	slVec2f buttonCenter;
	buttonCenter.x = (m_buildRect.z - m_buildRect.x) * 0.5f;
	buttonCenter.y = (m_buildRect.w - m_buildRect.y) * 0.5f;

	slVec2f textHalfLen;
	textHalfLen.x = tsz.x * 0.5f;
	textHalfLen.y = tsz.y * 0.5f;

	m_textPosition.x = m_buildRect.x + buttonCenter.x - textHalfLen.x;
	m_textPosition.y = m_buildRect.y + buttonCenter.y - textHalfLen.y;
}

void slGUIButton::SetText(const slString& s)
{
	m_text = s;
	UpdateTextPosition();
}
