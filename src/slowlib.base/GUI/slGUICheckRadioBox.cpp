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

slGUICheckRadioBoxTextDrawCallback::slGUICheckRadioBoxTextDrawCallback()
{
	m_defaultFont = slFramework::GetDefaultFont(slGUIDefaultFont::Text);
	m_icons = slFramework::GetDefaultFont(slGUIDefaultFont::Icons);
	m_currFont = m_defaultFont;
}

slGUIFont* slGUICheckRadioBoxTextDrawCallback::OnFont(uint32_t r, char32_t c)
{
	return m_currFont;
}

slColor* slGUICheckRadioBoxTextDrawCallback::OnColor(uint32_t r, char32_t c)
{
	switch (r)
	{
	case slGUIDrawTextCallback::Reason_icon:
		return &m_button->GetStyle()->m_colorWhite;
	case slGUIDrawTextCallback::Reason_pressed:
		return &m_button->GetStyle()->m_chkradioMousePressTextColor;
	case slGUIDrawTextCallback::Reason_mouseAbove:
		return &m_button->GetStyle()->m_chkradioMouseHoverTextColor;
	case slGUIDrawTextCallback::Reason_disabled:
		return &m_button->GetStyle()->m_chkradioDisabledTextColor;
	case slGUIDrawTextCallback::Reason_default:
		break;
	}
	return &m_button->GetStyle()->m_chkradioTextColor;
}

slGUICheckRadioBox::slGUICheckRadioBox(slGUIWindow* w, const slVec2f& position, const slVec2f& size)
	:
	slGUIButton(w, position, size)
{
	m_iconVerticalIndent = 2.f;
	m_iconDrawCallback.m_button = this;
	SetDrawBG(false);
}

slGUICheckRadioBox::~slGUICheckRadioBox(){}

void slGUICheckRadioBox::Rebuild()
{
	slGUIButton::Rebuild();
}

void slGUICheckRadioBox::Draw(slGS* gs, float dt)
{
	slGUIDrawTextCallback* prevcb = m_textDrawCallback;
	m_textDrawCallback = dynamic_cast<slGUIDrawTextCallback*>(&m_iconDrawCallback);
	
	m_iconDrawCallback.m_currFont = m_iconDrawCallback.m_defaultFont;
	slGUIButton::Draw(gs, dt);

	m_iconDrawCallback.m_currFont = m_iconDrawCallback.m_icons;
	
	char32_t text[2] = { (char32_t)slGUIDefaultIconID::CheckboxUncheck, 0 };

	if (m_asRadioButton)
	{
		if (m_isChecked)
			text[0] = (char32_t)slGUIDefaultIconID::RadioCheck;
		else
			text[0] = (char32_t)slGUIDefaultIconID::RadioUncheck;
	}
	else
	{
		if (m_isChecked)
			text[0] = (char32_t)slGUIDefaultIconID::CheckboxCheck;
	}

	
	slVec2f textPosition;
	textPosition.x = m_buildRect.x;
	textPosition.y = m_buildRect.y + m_iconVerticalIndent;

	if (IsEnabled())
	{
		if (IsClickedLMB())
		{
			m_textDrawCallback->m_reason = slGUIDrawTextCallback::Reason_pressed;
			gs->DrawGUIText(text, 1, textPosition, m_textDrawCallback);
		}
		else
		{
			if (IsCursorInRect())
			{
				m_textDrawCallback->m_reason = slGUIDrawTextCallback::Reason_mouseAbove;
				gs->DrawGUIText(text, 1, textPosition, m_textDrawCallback);
			}
			else
			{
				m_textDrawCallback->m_reason = slGUIDrawTextCallback::Reason_icon;
				gs->DrawGUIText(text, 1, textPosition, m_textDrawCallback);
			}
		}
	}
	else
	{
		m_textDrawCallback->m_reason = slGUIDrawTextCallback::Reason_disabled;
		gs->DrawGUIText(text, 1, textPosition, m_textDrawCallback);
	}

	m_textDrawCallback = prevcb;
}

void slGUICheckRadioBox::OnClickLMB()
{
	if (m_asRadioButton)
	{
		if (!m_isChecked)
		{
			m_isChecked = true;

			// uncheck other
			slGUIElement* parent = dynamic_cast<slGUIElement*>(GetParent());
			if (parent)
			{
				auto & children = *parent->GetChildren();
				for (auto _child : children)
				{
					auto child = dynamic_cast<slGUICheckRadioBox*>(_child);
					if (child)
					{
						if (child != this && child->m_asRadioButton)
						{
							if (child->m_radiouGroup == m_radiouGroup)
							{
								child->m_isChecked = false;
								child->OnUnCheck();
							}
						}
					}
				}
			}
		}
	}
	else
	{
		m_isChecked = m_isChecked ? false : true;
		m_isChecked ? OnCheck() : OnUnCheck();
	}
}

void slGUICheckRadioBox::OnCheck()
{
}

void slGUICheckRadioBox::OnUnCheck()
{
}
