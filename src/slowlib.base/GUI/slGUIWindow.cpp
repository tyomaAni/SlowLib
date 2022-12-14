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

slGUIWindow::slGUIWindow(const slVec2f& position, const slVec2f& size)
	:
	slGUICommon(position, size)
{
	m_rootElement = dynamic_cast<slGUIElement*>(slCreate<slGUIRootElement>(this, position, size));
}

slGUIWindow::~slGUIWindow()
{
}

void slGUIWindow::SetTitle(const char32_t* t)
{
	m_title = t;
}

void slGUIWindow::SetPositionAndSize(const slVec2f& p, const slVec2f& sz)
{
	m_position = p;
	m_size = sz;
}

void _slGUIWindow_RebuildElement(slGUIElement* e)
{
	e->Rebuild();

	if (e->GetChildren()->m_head)
	{
		auto children = e->GetChildren();
		if (children->m_head)
		{
			auto curr = children->m_head;
			auto last = curr->m_left;
			while (1)
			{
				_slGUIWindow_RebuildElement(dynamic_cast<slGUIElement*>(curr->m_data));
				if (curr == last)
					break;
				curr = curr->m_right;
			}
		}
	}

	e->UpdateContentSize();
	e->UpdateScroll();
}

void slGUIWindow::Rebuild()
{
	// calculate rects
	m_baseRect.x = m_position.x;
	m_baseRect.y = m_position.y;
	m_baseRect.z = m_baseRect.x + m_size.x;
	m_baseRect.w = m_baseRect.y + m_size.y;
	if (m_baseRect.x > m_baseRect.z)
		m_baseRect.x = m_baseRect.z;
	if (m_baseRect.y > m_baseRect.w)
		m_baseRect.y = m_baseRect.w;

	m_clipRect = m_baseRect;
	m_activeRect = m_clipRect;

	// rebuild root here
	m_rootElement->m_baseRect.x = m_baseRect.x;
	m_rootElement->m_baseRect.y = m_baseRect.y;
	m_rootElement->m_baseRect.z = m_baseRect.x + m_size.x;
	m_rootElement->m_baseRect.w = m_baseRect.y + m_size.y;
	m_rootElement->m_clipRect = m_rootElement->m_baseRect;
	m_rootElement->m_activeRect = m_rootElement->m_clipRect;

	// then rebuild other elements
	_slGUIWindow_RebuildElement(m_rootElement);
}

void _slGUIWindow_UpdateElement(slGUIElement* e)
{
	e->Update();

	if (e->GetChildren()->m_head)
	{
		auto children = e->GetChildren();
		if (children->m_head)
		{
			auto last = children->m_head;
			auto curr = last->m_left;
			while (1)
			{
				_slGUIWindow_UpdateElement(dynamic_cast<slGUIElement*>(curr->m_data));
				if (curr == last)
					break;
				curr = curr->m_left;
			}
		}
	}
	e->UpdateScroll();
}

void slGUIWindow::Update()
{
	if (slMath::pointInRect(g_framework->m_input.mousePosition, m_activeRect))
		g_framework->m_GUIState.m_windowUnderCursor = this;

	_slGUIWindow_UpdateElement(m_rootElement);
}

void _slGUIWindow_DrawElement(slGS* gs, slGUIElement* e, float dt)
{

	if (e->IsVisible())
	{
		if ((e->m_clipRect.x < e->m_clipRect.z) && (e->m_clipRect.y < e->m_clipRect.w))
			e->Draw(gs, dt);

		if (e->GetChildren()->m_head)
		{
			auto children = e->GetChildren();
			if (children->m_head)
			{
				auto curr = children->m_head;
				auto last = curr->m_left;
				while (1)
				{
					_slGUIWindow_DrawElement(gs, dynamic_cast<slGUIElement*>(curr->m_data), dt);
					if (curr == last)
						break;
					curr = curr->m_right;
				}
			}
		}
	}
}

void slGUIWindow::Draw(slGS* gs, float dt)
{
	if(IsDrawBG())
		gs->DrawGUIRectangle(m_baseRect, m_style->m_windowActiveBGColor1, m_style->m_windowActiveBGColor2, 0, 0);

	_slGUIWindow_DrawElement(gs, m_rootElement, dt);
}

