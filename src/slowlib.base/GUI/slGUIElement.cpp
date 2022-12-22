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

slGUIElement::slGUIElement(slGUIWindow* w) 
{
	SL_ASSERT_ST(w);
	SetStyle(slFramework::GetGUIStyle(slGUIStyleTheme::Light));
}
slGUIElement::~slGUIElement() {}

void slGUIElement::ToTop() {}

void slGUIElement::Rebuild()
{
	slGUIElement* parent = dynamic_cast<slGUIElement*>(GetParent());
	if (parent)
	{
		// build buildRect using margin
		m_buildRect.x = parent->m_buildRect.x + m_margin.x;
		m_buildRect.y = parent->m_buildRect.y + m_margin.y;
		
		// if width or height != 0 then do not use m_margin
		if (m_size.x != 0.f)
			m_buildRect.z = m_buildRect.x + m_size.x;
		else
			m_buildRect.z = parent->m_buildRect.z - m_margin.z;

		if (m_size.y != 0.f)
			m_buildRect.w = m_buildRect.y + m_size.y;
		else
			m_buildRect.w = parent->m_buildRect.w - m_margin.w;

		if (m_buildRect.x > m_buildRect.z)
			m_buildRect.x = m_buildRect.z;
		if (m_buildRect.y > m_buildRect.w)
			m_buildRect.y = m_buildRect.w;

		// scrolling here somewhere
		
		// usuially m_clipRect is == m_buildRect;
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

		m_activeRect = m_clipRect;


		// maybe here callback
		// OnRects(); or OnRebuild();
	}
	// else
	// only root element without parent
	// rebuild root in slGUIWindow::Rebuild();
}

void slGUIElement::Update(slInputData* id)
{
	slGUICommon::Update(id);
}