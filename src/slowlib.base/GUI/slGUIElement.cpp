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

slGUIElement::slGUIElement(slGUIWindow* w, const slVec2f& position, const slVec2f& size):
	slGUICommon(position, size)
{
	SL_ASSERT_ST(w);
	m_window = w;
	SetStyle(slFramework::GetGUIStyle(slGUIStyleTheme::Light));
}
slGUIElement::~slGUIElement() {}

void slGUIElement::ToTop() {}

void slGUIElement::Rebuild()
{
	slGUIElement* parent = dynamic_cast<slGUIElement*>(GetParent());
	if (parent)
	{
		m_baseRect.x = parent->m_baseRect.x + m_position.x;
		m_baseRect.y = parent->m_baseRect.y + m_position.y;
		m_baseRect.z = m_baseRect.x + m_size.x;
		m_baseRect.w = m_baseRect.y + m_size.y;

		if (m_baseRect.x > m_baseRect.z)
			m_baseRect.x = m_baseRect.z;
		if (m_baseRect.y > m_baseRect.w)
			m_baseRect.y = m_baseRect.w;

		slVec2f parentSize(parent->m_baseRect.z - parent->m_baseRect.x, parent->m_baseRect.w - parent->m_baseRect.y);
		if (parentSize.x && parentSize.y)
		{
			float parentRectSizeX_1 = 1.f / parentSize.x;
			float parentRectSizeY_1 = 1.f / parentSize.y;

			slVec2f parentCreationCenter;
			parentCreationCenter.x = parent->m_buildRectOnCreation.x + ((parent->m_buildRectOnCreation.z - parent->m_buildRectOnCreation.x) * 0.5f);
			parentCreationCenter.y = parent->m_buildRectOnCreation.y + ((parent->m_buildRectOnCreation.w - parent->m_buildRectOnCreation.y) * 0.5f);

			slVec2f parentCurrentCenter;
			parentCurrentCenter.x = parent->m_baseRect.x + ((parent->m_baseRect.z - parent->m_baseRect.x) * 0.5f);
			parentCurrentCenter.y = parent->m_baseRect.y + ((parent->m_baseRect.w - parent->m_baseRect.y) * 0.5f);

			float parentRectSizeDiff_X = parentCurrentCenter.x - parentCreationCenter.x;
			float parentRectSizeDiff_Y = parentCurrentCenter.y - parentCreationCenter.y;

			// alignment
			switch (m_alignment)
			{
			case slGUIElement::Left:
				m_baseRect.y += parentRectSizeDiff_Y;
				m_baseRect.w += parentRectSizeDiff_Y;
				break;
			case slGUIElement::Top:
				m_baseRect.x += parentRectSizeDiff_X;
				m_baseRect.z += parentRectSizeDiff_X;
				break;
			case slGUIElement::Center:
				m_baseRect.y += parentRectSizeDiff_Y;
				m_baseRect.w += parentRectSizeDiff_Y;
				m_baseRect.x += parentRectSizeDiff_X;
				m_baseRect.z += parentRectSizeDiff_X;
				break;
			case slGUIElement::Right:
				m_baseRect.y += parentRectSizeDiff_Y;
				m_baseRect.w += parentRectSizeDiff_Y;

				m_baseRect.x = parent->m_baseRect.z -
					(parent->m_buildRectOnCreation.z - m_buildRectOnCreation.x) + parent->m_buildRectOnCreation.x;

				m_baseRect.z = parent->m_baseRect.z -
					(parent->m_buildRectOnCreation.z - m_buildRectOnCreation.z) + parent->m_buildRectOnCreation.x;
				break;
			case slGUIElement::Bottom:
				m_baseRect.x += parentRectSizeDiff_X;
				m_baseRect.z += parentRectSizeDiff_X;

				m_baseRect.y = parent->m_baseRect.w -
					(parent->m_buildRectOnCreation.w - m_buildRectOnCreation.y) + parent->m_buildRectOnCreation.y;

				m_baseRect.w = parent->m_baseRect.w -
					(parent->m_buildRectOnCreation.w - m_buildRectOnCreation.w) + parent->m_buildRectOnCreation.y;
				break;
			case slGUIElement::LeftTop:
				break;
			case slGUIElement::RightTop:
				m_baseRect.x = parent->m_baseRect.z -
					(parent->m_buildRectOnCreation.z - m_buildRectOnCreation.x) + parent->m_buildRectOnCreation.x;

				m_baseRect.z = parent->m_baseRect.z -
					(parent->m_buildRectOnCreation.z - m_buildRectOnCreation.z) + parent->m_buildRectOnCreation.x;
				break;
			case slGUIElement::LeftBottom:
				m_baseRect.y = parent->m_baseRect.w -
					(parent->m_buildRectOnCreation.w - m_buildRectOnCreation.y) + parent->m_buildRectOnCreation.y;

				m_baseRect.w = parent->m_baseRect.w -
					(parent->m_buildRectOnCreation.w - m_buildRectOnCreation.w) + parent->m_buildRectOnCreation.y;
				break;
			case slGUIElement::RightBottom:
				m_baseRect.x = parent->m_baseRect.z -
					(parent->m_buildRectOnCreation.z - m_buildRectOnCreation.x) + parent->m_buildRectOnCreation.x;

				m_baseRect.z = parent->m_baseRect.z -
					(parent->m_buildRectOnCreation.z - m_buildRectOnCreation.z) + parent->m_buildRectOnCreation.x;
				m_baseRect.y = parent->m_baseRect.w -
					(parent->m_buildRectOnCreation.w - m_buildRectOnCreation.y) + parent->m_buildRectOnCreation.y;

				m_baseRect.w = parent->m_baseRect.w -
					(parent->m_buildRectOnCreation.w - m_buildRectOnCreation.w) + parent->m_buildRectOnCreation.y;
				break;
			default:
				break;
			}
		}
		// scrolling here somewhere
	}

		
	// else
	// only root element without parent
	// rebuild root in slGUIWindow::Rebuild();

}

void slGUIElement::Update(slInputData* id)
{
	slGUICommon::Update(id);

	// maybe here callback
	// OnRects(); or OnRebuild();

	m_buildRect = m_baseRect;
	// usuially m_clipRect is == m_baseRect;
	m_clipRect = m_buildRect;

	slGUIElement* parent = dynamic_cast<slGUIElement*>(GetParent());
	if (parent)
	{
		m_buildRect.x -= parent->m_scroll.x;
		m_buildRect.y -= parent->m_scroll.y;
		m_buildRect.z -= parent->m_scroll.x;
		m_buildRect.w -= parent->m_scroll.y;
		
		m_clipRect = m_buildRect;

		// but parent has own clip rect
		if (m_clipRect.x < parent->m_clipRect.x)
			m_clipRect.x = parent->m_clipRect.x;
		if (m_clipRect.y < parent->m_clipRect.y)
			m_clipRect.y = parent->m_clipRect.y;
		if (m_clipRect.z > parent->m_clipRect.z)
			m_clipRect.z = parent->m_clipRect.z;
		if (m_clipRect.w > parent->m_clipRect.w)
			m_clipRect.w = parent->m_clipRect.w;
	}
	m_activeRect = m_clipRect;
	
	if (IsCursorInRect())
	{
		if (id->mouseWheelDelta < 0.f)
		{
			m_scrollTarget.y += 10.f;
			if (m_scrollTarget.y > m_scrollLimit.y)
				m_scrollTarget.y = m_scrollLimit.y;
		}
		else if (id->mouseWheelDelta > 0.f)
		{
			m_scrollTarget.y -= 10.f;

			if (m_scrollTarget.y < 0.f)
				m_scrollTarget.y = 0.f;
		}
	}
	m_scroll.y = slMath::lerp1(m_scroll.y, m_scrollTarget.y, 0.1f);
	
	Rebuild();
}