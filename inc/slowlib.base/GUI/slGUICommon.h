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
#ifndef __SL_SLOWLIBBASEGUICMN_H__
#define __SL_SLOWLIBBASEGUICMN_H__

class slGUICommon
{
public:
	enum
	{
		flag_visible = 0x1,
		flag_enabled = 0x2,
		flag_drawBG = 0x4,
	};

protected:
	slGUIStyle* m_style = 0;

	uint32_t m_flags = flag_visible | flag_enabled | flag_drawBG;
public:
	slGUICommon() {}
	virtual ~slGUICommon() {}

	virtual void SetStyle(slGUIStyle* s)
	{
		s ? m_style = s : m_style = slFramework::GetGUIStyle(slGUIStyleTheme::Light);
	}

	virtual slGUIStyle* GetStyle() 
	{
		return m_style; 
	}

	// Set visibility. It will set to children too.
	virtual void SetVisible(bool v) {
		if (v)
			m_flags |= slGUICommon::flag_visible;
		else
			m_flags &= ~slGUICommon::flag_visible;
	}
	virtual bool IsVisible() { return (m_flags & flag_visible); }

	virtual void SetEnabled(bool v) {
		if (v)
			m_flags |= slGUICommon::flag_enabled;
		else
			m_flags &= ~slGUICommon::flag_enabled;
	}
	virtual bool IsEnabled() { return (m_flags & flag_enabled); }

	virtual void SetDrawBG(bool v) {
		if (v)
			m_flags |= slGUICommon::flag_drawBG;
		else
			m_flags &= ~slGUICommon::flag_drawBG;
	}
	virtual bool IsDrawBG() { return (m_flags & flag_drawBG); }
	
	// Size of element + all children. It like AABB. find maximum x and y.
	// Size of window with all elements.
	// Use it for scrolling
	slVec2f m_contentSize;

	uint32_t m_id = 0;

	// calculate rects.
	// Better to call this after GUI creation and after changing widow size
	virtual void Rebuild() = 0;
	
	// all mouse things, click things
	virtual void Update(slInputData*) = 0;
	
	// draw it
	virtual void Draw(slGS* gs, float dt) = 0;

	slVec4f m_buildRect;  // build element using this
	slVec4f m_clipRect;   // clip using this
	slVec4f m_activeRect; // mouse sensor area
};

#endif
