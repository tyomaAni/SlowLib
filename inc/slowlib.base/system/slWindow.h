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
#ifndef __SL_SLOWLIBBASEWINDOW_H__
#define __SL_SLOWLIBBASEWINDOW_H__

class slWindowCallback
{
public:
	slWindowCallback() {}
	virtual ~slWindowCallback() {}

	virtual void OnActivate(slWindow*) {}
	virtual void OnDeactivate(slWindow*) {}
	virtual void OnSize(slWindow*) {}
	virtual void OnMinimize(slWindow*) {}
	virtual void OnMaximize(slWindow*) {}
	virtual void OnRestore(slWindow*) {}
	virtual void OnDraw(slWindow*) {}
	virtual void OnMove(slWindow*) {}
	virtual void OnClose(slWindow*) {}
};

struct slWindowCommonData
{
	slWindowCallback* m_cb = 0;
	slPoint m_borderSize;
	slPoint m_borderSizeCurrent;
	slPoint m_sizeMinimum;
	slPoint m_currentSize;
	slPoint m_creationSize;
	slPoint m_sizePreFullscreen;
	bool m_isVisible = false;
	bool m_isFullscreen = false;

	void* m_implementation = 0;
};

class slWindow
{
	slWindowCommonData m_data;
public:
	slWindow(slWindowCallback* cb, int sx, int sy);
	~slWindow();

	void SetTitle(const char*);
	void SetVisible(bool v);
	bool IsVisible();
	void Maximize();
	void Minimize();
	void Restore();
	void SetBorderless(bool);
	void SetNoResize(bool);
	void SetNoMinimize(bool);
	slPoint* GetSizeMinimum();
	slPoint* GetBorderSize();
	slPoint* GetCurrentSize();
	
	void ToFullscreenMode();
	void ToWindowMode();

	// it will call callback functions
	void OnActivate();
	void OnDeactivate();
	void OnSize();
	void OnMinimize();
	void OnMaximize();
	void OnRestore();
	void OnDraw();
	void OnMove();
	void OnClose();

	slWindowCommonData* GetData() { return &m_data; }
};

#endif
