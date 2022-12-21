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

#pragma once
#ifndef __SL_SLOWLIBBASEGUIELMNT_H__
#define __SL_SLOWLIBBASEGUIELMNT_H__

#include "slowlib.base/common/slHierarchy.h"
#include "slowlib.base/GUI/slGUICommon.h"

class slGUIElement : public slUserData, public slHierarchy, public slGUICommon
{
protected:
	slGUIWindow* m_window = 0;
public:
	slGUIElement();
	virtual ~slGUIElement();

	slGUIWindow* GetWindow() { return m_window; }

	slVec4f m_margin;

	slVec2f m_scroll = 0.f;
	slVec2f m_scrollLimit = 0.f;

	enum Alignment
	{
		LeftTop,
		Top,
		RightTop,
		Right,
		RightBottom,
		Bottom,
		LeftBottom,
		Left,
		Center
	};
	Alignment m_alignment = Alignment::LeftTop;

	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual void OnClickLMB();
	virtual void OnClickRMB();
	virtual void OnClickMMB();
	virtual void OnClickX1MB();
	virtual void OnClickX2MB();
	virtual void OnReleaseLMB();
	virtual void OnReleaseRMB();
	virtual void OnReleaseMMB();
	virtual void OnReleaseX1MB();
	virtual void OnReleaseX2MB();

	// Make this element last for drawing, first for input.
	// It will be like on top of all other elements.
	// It's just changing the order.
	virtual void ToTop();

};

#endif
