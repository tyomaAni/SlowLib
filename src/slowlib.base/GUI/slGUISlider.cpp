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

slGUISliderTextDrawCallback::slGUISliderTextDrawCallback()
{
	m_defaultFont = slFramework::GetDefaultFont(slGUIDefaultFont::Text);
	m_icons = slFramework::GetDefaultFont(slGUIDefaultFont::Icons);
	m_currFont = m_defaultFont;
}

slGUIFont* slGUISliderTextDrawCallback::OnFont(uint32_t r, char32_t c)
{
	return m_currFont;
}

slColor* slGUISliderTextDrawCallback::OnColor(uint32_t r, char32_t c)
{
	return &m_slider->GetStyle()->m_sliderTextColor;
}

slGUISlider::slGUISlider(slGUIWindow* w, const slVec2f& position, const slVec2f& size)
	:
	slGUIElement(w, position, size)
{
	m_sliderTextDrawCallback.m_slider = this;
	m_textDrawCallback = dynamic_cast<slGUIDrawTextCallback*>(&m_sliderTextDrawCallback);
	SetDrawBG(false);
}

slGUISlider::~slGUISlider(){}


void slGUISlider::Update()
{
	slGUIElement::Update();
	if (m_window->GetRootElement()->m_scrollDelta.y)
		Rebuild();

	if (!m_isClicked)
	{
		if (slMath::pointInRect(slInput::GetData()->mousePosition, m_controlRect))
		{
			if (slInput::IsLMBHit())
				m_isClicked = true;
		}
	}
	else
	{
		auto id = slInput::GetData();

		if (slInput::IsKeyHit(slInput::KEY_ESCAPE))
			m_isClicked = false;
		else if (slInput::IsLMBRelease())
			m_isClicked = false;
		else if(id->mouseMoveDelta.x != 0.f)
		{
			float value = 0.f;
			if (id->mousePosition.x <= m_axisRect.x)
				value = 0.f;
			else if (id->mousePosition.x >= m_axisRect.z)
				value = 1.f;
			else
			{
				float mouseWidth = id->mousePosition.x - m_axisRect.x;
				if (m_axisWidth != 0.f)
				{
					float M = 1.f / m_axisWidth;
					value = mouseWidth * M;
				}
			}

			//printf("%f\n", value);
			if (m_morePrecise)
			{
				auto V = (double)slInput::GetData()->mouseMoveDelta.x * m_multiplerNormal;
				
				if (id->keyboardModifier == id->KBMOD_ALT)
					V *= m_multiplerSlow;
				if (id->keyboardModifier == id->KBMOD_SHIFT)
					V *= m_multiplerFast;

				m_valueFloat += V;
				m_valueInt += (int32_t)slInput::GetData()->mouseMoveDelta.x;
				m_valueUint += (uint32_t)slInput::GetData()->mouseMoveDelta.x;
			}
			else
				m_valueFloat = slMath::lerp1(m_valueMinFloat, m_valueMaxFloat, (double)value);
			
			this->OnChangeValue();


			if (m_valueFloat < m_valueMinFloat) m_valueFloat = m_valueMinFloat;
			if (m_valueFloat > m_valueMaxFloat) m_valueFloat = m_valueMaxFloat;

			if (m_valueInt < m_valueMinInt) m_valueInt = m_valueMinInt;
			if (m_valueInt > m_valueMaxInt) m_valueInt = m_valueMaxInt;

			if (m_valueUint < m_valueMinUint) m_valueUint = m_valueMinUint;
			if (m_valueUint > m_valueMaxUint) m_valueUint = m_valueMaxUint;

			findAxisRectFill();
		}
	}
}


void slGUISlider::Rebuild()
{
	slGUIElement::Rebuild();
	
	auto H = m_buildRect.w - m_buildRect.y;

	m_axisRect.x = m_buildRect.x + m_axisIndent;
	m_axisRect.z = m_buildRect.z - m_axisIndent;
	
	m_axisRect.y = m_buildRect.y + (H * 0.5f);
	m_axisRect.y -= m_axisHeight * 0.5f;
	m_axisRect.w = m_axisRect.y + m_axisHeight;

	m_axisWidth = m_axisRect.z - m_axisRect.x;

	findAxisRectFill();
}

void slGUISlider::Draw(slGS* gs, float dt)
{
	gs->SetScissorRect(m_clipRect, 0);
	
	gs->DrawGUIRectangle(m_axisRect, m_style->m_sliderAxisEmtpyColor, m_style->m_sliderAxisEmtpyColor, 0, 0);
	gs->DrawGUIRectangle(m_axisRectFill, m_style->m_sliderAxisFillColor, m_style->m_sliderAxisFillColor, 0, 0);
	gs->DrawGUIRectangle(m_controlRect, m_style->m_sliderControlColor, m_style->m_sliderControlColor, 0, 0);

	if (m_drawText)
	{
		slGUIElement* parent = dynamic_cast<slGUIElement*>(GetParent());
		if (parent && m_text.size())
		{
			gs->SetScissorRect(parent->m_clipRect, 0);

			slVec2f textPosition;
			textPosition.x = m_buildRect.z;
			textPosition.y = m_axisRect.y - m_axisHeight;

			gs->DrawGUIText(
				m_text.c_str(),
				m_text.size(),
				textPosition,
				m_textDrawCallback);
		}
	}
}

void slGUISlider::findAxisRectFill()
{
	m_axisRectFill = m_axisRect;

	m_axisWidth = m_axisRect.z - m_axisRect.x;

	float D = 0.f;

	m_text.clear();

	switch (m_valueType)
	{
	case slGUISlider::ValueType::Float:
	{
		m_text.append(m_valueFloat);
		for (uint32_t i = 0; i < m_truncateFloatByThisNumOfChars; ++i)
		{
			m_text.pop_back();
		}
		double len = m_valueMaxFloat - m_valueMinFloat;
		if (len != 0.0)
		{
			double M = 1.f / len;
			double V = abs(m_valueMinFloat - m_valueFloat);
			D = V * M;
		}
	}break;
	case slGUISlider::ValueType::Uint:
	{
		m_text.append(m_valueUint);
		uint32_t len = m_valueMaxUint - m_valueMinUint;

	}break;
	case slGUISlider::ValueType::Int:
	{
		m_text.append(m_valueInt);
		int32_t len = m_valueMaxInt - m_valueMinInt;

	}break;
	}
	
	m_axisRectFill.z = m_axisRectFill.x + (m_axisWidth * D);

	m_controlRect.x = m_buildRect.x + m_axisIndent - (m_controlWidth *0.5f);
	m_controlRect.y = m_buildRect.y;
	m_controlRect.z = m_controlRect.x + m_controlWidth;
	m_controlRect.w = m_buildRect.w;

	m_controlRect.x += m_axisRectFill.z - m_axisRectFill.x;
	m_controlRect.z += m_axisRectFill.z - m_axisRectFill.x;
}

void slGUISlider::SetMinMaxFloat(double mn, double mx) 
{
	m_valueMinFloat = mn; 
	m_valueMaxFloat = mx; 
	m_valueFloat = m_valueMinFloat;
	Rebuild();
}

void slGUISlider::SetMinMaxUint(uint32_t mn, uint32_t mx) 
{
	m_valueMinUint = mn; 
	m_valueMaxUint = mx; 
	m_valueUint = m_valueMinUint;
	Rebuild();
}

void slGUISlider::SetMinMaxInt(int32_t mn, int32_t mx) 
{
	m_valueMinInt = mn;
	m_valueMaxInt = mx;
	m_valueInt = m_valueMinInt;
	Rebuild();
}
