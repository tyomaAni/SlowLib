/*
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
#ifndef __SL_SLOWLIBBASEGUILISTBOX_H_
#define __SL_SLOWLIBBASEGUILISTBOX_H_

class slGUIListBoxTextDrawCallback : public slGUIDrawTextCallback
{
	friend class slGUIListBox;
	slGUIListBox* m_lb = 0;
	slGUIFont* m_defaultFont = 0;
	slGUIFont* m_currFont = 0;
public:
	slGUIListBoxTextDrawCallback();
	virtual ~slGUIListBoxTextDrawCallback() {}

	virtual slGUIFont* OnFont(uint32_t r, char32_t) override;
	virtual slColor* OnColor(uint32_t r, char32_t) override;
};

struct slGUIListBoxItem
{
	slString m_text;
	uint32_t m_id = 0;
	void* m_data = 0;

	bool m_isSelected = false;
};

class slGUIListBox : public slGUIElement
{
	slGUIListBoxTextDrawCallback m_listBoxTextDrawCallback;
	slArray<slGUIListBoxItem*> m_items;
	float m_lineHeight = 0.f;
	size_t m_firstItemIndexForDraw = 0;
	size_t m_numberOfVisibleLines = 0;

public:
	slGUIListBox(slGUIWindow*, const slVec2f& position, const slVec2f& size);
	virtual ~slGUIListBox();

	virtual void Rebuild() final;
	virtual void Update() final;
	virtual void Draw(slGS* gs, float dt) final;	
	
	virtual void UpdateContentSize() final;

	slGUIListBoxItem* AddItem(const char32_t*, uint32_t id, void* data);
	void RemoveItem(slGUIListBoxItem*);

	slArray<slGUIListBoxItem*>* GetItems() { return &m_items; }

	bool m_drawItemBG = true;
	bool m_multiSelect = false;

	// return true if need to select
	virtual bool OnSelect(slGUIListBoxItem*) { return true; }
	// return true if need to deselect
	virtual bool OnDeSelect(slGUIListBoxItem*) { return true; }
};

#endif
