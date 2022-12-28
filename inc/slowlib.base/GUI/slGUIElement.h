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

class slGUIElement : public slGUICommon
{
protected:
	slGUIWindow* m_window = 0;

public:
	slGUIElement(slGUIWindow*, const slVec2f& position, const slVec2f& size);
	virtual ~slGUIElement();

	slGUIWindow* GetWindow() { return m_window; }

	// basic rebuild, common for all elements
	virtual void Rebuild() override;
	// basic things for OnMouseEnter OnClickLMB and other
	virtual void Update() override;

	enum Alignment
	{
		Left = 0x1,
		Top = 0x2,
		Right = 0x4,
		Bottom = 0x8,
		LeftTop = Left | Top,
		RightTop = Top | Right,
		LeftBottom = Left | Bottom,
		RightBottom = Right | Bottom,
		Center = Left | Top | Right | Bottom
	};
	Alignment m_alignment = Alignment::Center;

	// Make this element last for drawing, first for input.
	// It will be like on top of all other elements.
	// It's just changing the order.
	virtual void ToTop();
};

#endif
