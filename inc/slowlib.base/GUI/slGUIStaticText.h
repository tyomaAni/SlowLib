﻿/*
BSD 2-Clause License

Copyright (c) 2023, tyomaAni
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
#ifndef __SL_SLOWLIBBASEGUISTATICTXT_H_
#define __SL_SLOWLIBBASEGUISTATICTXT_H_

class slGUIStaticTextDrawCallback : public slGUIDrawTextCallback
{
	friend class slGUIStaticText;
	slGUIStaticText* m_staticText = 0;
	slGUIFont* m_defaultFont = 0;
	slGUIFont* m_currFont = 0;
public:
	slGUIStaticTextDrawCallback();
	virtual ~slGUIStaticTextDrawCallback() {}

	virtual slGUIFont* OnFont(uint32_t r, char32_t) override;
	virtual slColor* OnColor(uint32_t r, char32_t) override;
};

class slGUIStaticText : public slGUIElement
{
	slGUIStaticTextDrawCallback m_staticTextTextDrawCallback;
	
	slString m_text;
	bool m_useDrawLimit = false;
	size_t m_drawLimit = 0;
	float m_lineHeight = 0.f;
public:
	slGUIStaticText(slGUIWindow*, const slVec2f& position, const slVec2f& size);
	virtual ~slGUIStaticText();

	virtual void Rebuild() final;
	virtual void Update() final;
	virtual void Draw(slGS* gs, float dt) final;	

	virtual void SetText(const slString&);
	virtual void UseDrawLimit(bool use, size_t limit)
	{
		m_useDrawLimit = use;
		m_drawLimit = limit;
	}
};

#endif
