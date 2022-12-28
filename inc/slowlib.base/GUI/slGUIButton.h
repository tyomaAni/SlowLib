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
#ifndef __SL_SLOWLIBBASEGUIBTN_H__
#define __SL_SLOWLIBBASEGUIBTN_H__

#include "slowlib.base/string/slString.h"

class slGUIButtonTextDrawCallback : public slGUIDrawTextCallback
{
	friend class slGUIButton;
	slGUIButton* m_button = 0;
public:
	slGUIButtonTextDrawCallback() {}
	virtual ~slGUIButtonTextDrawCallback() {}

	virtual slGUIFont* OnFont(uint32_t r, char32_t) override;
	virtual slColor* OnColor(uint32_t r, char32_t) override;
};

class slGUIButton : public slGUIElement
{
protected:
	slString m_text;
	slVec2f m_textPosition;
	void UpdateTextPosition();
	slGUIButtonTextDrawCallback m_defaultTextDrawCallback;
public:
	slGUIButton(slGUIWindow*, const slVec2f& position, const slVec2f& size);
	virtual ~slGUIButton();
	virtual void Rebuild() override;
	virtual void Update() override;
	virtual void Draw(slGS* gs, float dt) override;
	
	virtual void SetText(const slString&);
	slGUIButtonTextDrawCallback* GetDefaultButtonTextDrawCallback() { return &m_defaultTextDrawCallback; }
};

#endif
