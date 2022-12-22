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

slGUICommon::slGUICommon() {}
slGUICommon::~slGUICommon() {}
void slGUICommon::OnMouseEnter() {}
void slGUICommon::OnMouseLeave() {}
void slGUICommon::OnClickLMB() {}
void slGUICommon::OnClickRMB() {}
void slGUICommon::OnClickMMB() {}
void slGUICommon::OnClickX1MB() {}
void slGUICommon::OnClickX2MB() {}
void slGUICommon::OnReleaseLMB() {}
void slGUICommon::OnReleaseRMB() {}
void slGUICommon::OnReleaseMMB() {}
void slGUICommon::OnReleaseX1MB() {}
void slGUICommon::OnReleaseX2MB() {}

void slGUICommon::Update(slInputData* id)
{
	if (slMath::pointInRect(id->mousePosition, m_activeRect))
	{
		if ((m_flags & slGUICommon::flag_cursorInRect) == 0)
			OnMouseEnter();

		m_flags |= slGUICommon::flag_cursorInRect;
	}
	else
	{
		if(IsCursorInRect())
			OnMouseLeave();

		m_flags &= ~slGUICommon::flag_cursorInRect;
	}

	if (IsEnabled() && IsCursorInRect())
	{
		if (slInput::IsLMBHit())
		{
			m_flags |= slGUICommon::flag_clickedLMB;
			OnClickLMB();
		}
		else if (slInput::IsLMBRelease())
		{
			if (IsClickedLMB())
				OnReleaseLMB();
		}

		if (slInput::IsRMBHit())
		{
			m_flags |= slGUICommon::flag_clickedRMB;
			OnClickRMB();
		}
		else if (slInput::IsRMBRelease())
		{
			if (IsClickedRMB())
				OnReleaseRMB();
		}

		if (slInput::IsMMBHit())
		{
			m_flags |= slGUICommon::flag_clickedMMB;
			OnClickMMB();
		}
		else if (slInput::IsMMBRelease())
		{
			if (IsClickedMMB())
				OnReleaseMMB();
		}

		if (slInput::IsX1MBHit())
		{
			m_flags |= slGUICommon::flag_clickedX1MB;
			OnClickX1MB();
		}
		else if (slInput::IsX1MBRelease())
		{
			if (IsClickedX1MB())
				OnReleaseX1MB();
		}

		if (slInput::IsX2MBHit())
		{
			m_flags |= slGUICommon::flag_clickedX2MB;
			OnClickX2MB();
		}
		else if (slInput::IsX2MBRelease())
		{
			if (IsClickedX2MB())
				OnReleaseX2MB();
		}
	}

	// I need to remove flags when cursor outside of rect
	if (slInput::IsLMBRelease())
		m_flags &= ~slGUICommon::flag_clickedLMB;
	if (slInput::IsRMBRelease())
		m_flags &= ~slGUICommon::flag_clickedRMB;
	if (slInput::IsMMBRelease())
		m_flags &= ~slGUICommon::flag_clickedMMB;
	if (slInput::IsX1MBRelease())
		m_flags &= ~slGUICommon::flag_clickedX1MB;
	if (slInput::IsX2MBRelease())
		m_flags &= ~slGUICommon::flag_clickedX2MB;
}

