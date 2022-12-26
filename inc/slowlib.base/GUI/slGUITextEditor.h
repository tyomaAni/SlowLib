﻿/*
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
#ifndef __SL_SLOWLIBBASEGUITXTEDTR_H__
#define __SL_SLOWLIBBASEGUITXTEDTR_H__

class slGUITextEditorTextDrawCallback : public slGUIDrawTextCallback
{
	friend class slGUITextEditor;
	slGUITextEditor* m_edtr = 0;
	slGUIFont* m_defaultFont = 0;
	slGUIFont* m_currFont = 0;
public:
	slGUITextEditorTextDrawCallback();
	virtual ~slGUITextEditorTextDrawCallback() {}

	virtual slGUIFont* OnFont(uint32_t r, char32_t) override;
	virtual slColor* OnColor(uint32_t r, char32_t) override;
};

class slGUITextEditor : public slGUIElement
{
	slGUITextEditorTextDrawCallback m_textEditorTextDrawCallback;
	
	char32_t* m_textBuffer = 0;
	size_t m_textBufferLen = 0;
	size_t m_textBufferAllocated = 0; //in char32_t
	void reallocate(size_t newSize);

	size_t m_textBeginDrawIndex = 0;

	// this points to place where to put new char.
	// from 0 to m_textBufferLen
	// 0 - put in front
	// m_textBufferLen - push_back
	size_t m_textCursor = 0;
	float m_textCursorTimer = 0.f;
	float m_textCursorTimerLimit = 0.5f;
	slVec2f m_textCursorPosition; // save it in Draw
	bool m_drawTextCursor = false;

	void GoLeft();
	void GoRight();
	void GoUp();
	void GoDown();
	void GoHome();
	void GoEnd();

	enum
	{
		textEditorFlag_isActivated = 0x1
	};
	uint32_t m_textEditorFlags = 0;

public:
	slGUITextEditor(slGUIWindow*, const slVec2f& position, const slVec2f& size);
	virtual ~slGUITextEditor();

	bool IsActivated() { return (m_textEditorFlags & textEditorFlag_isActivated); }
	void Activate(bool);

	virtual void Rebuild() final;
	virtual void Update(slInputData*) final;
	virtual void Draw(slGS* gs, float dt) final;	

	virtual void Clear(bool freeMemory);
	virtual void SetText(const slString&);
};

#endif
