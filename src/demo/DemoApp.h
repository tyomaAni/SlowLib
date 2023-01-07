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
#include "slowlib.base/gs/slGS.h"
#include "slowlib.base/gs/slMaterial.h"
#include "slowlib.base/gs/slImage.h"
#include "slowlib.base/gs/slImageLoader.h"
#include "slowlib.base/gs/slTexture.h"
#include "slowlib.base/scene/slCamera.h"
#include "slowlib.base/geometry/slGeometry.h"
#include "slowlib.base/containers/slArray.h"
#include "slowlib.base/archive/slArchive.h"
#include "slowlib.base/GUI/slGUI.h"

#include <stdio.h>
#include <Windows.h>

class FrameworkCallback;
class WindowCallback; 
class GUIDrawTextCallback;



class DemoExample
{
	friend class DemoApp;
	slString m_name;
public:
	DemoExample() {}
	virtual ~DemoExample() {}

	virtual bool Init() = 0;
	virtual void Shutdown() = 0;
};

class DemoCategory
{
	friend class DemoApp;
	slString m_name;

	uint32_t m_cursor = 0;

	DemoCategory* m_parentCategory = 0;
	slArray<DemoCategory*> m_subCategories;
	slArray<DemoExample*> m_examples;
	
	struct Element
	{
		enum
		{
			type_category,
			type_example,
			type_parentDir
		};
		uint32_t type = type_category;
		DemoCategory* m_cat = 0;
		DemoExample* m_ex = 0;
	};
	slArray<Element> m_elements; // draw menu using this
	void findElements()
	{
		m_elements.clear();
		Element el;
		if (m_parentCategory)
		{
			el.type = el.type_parentDir;
			m_elements.push_back(el);
		}

		for (size_t i = 0; i < m_subCategories.m_size; ++i)
		{
			el.type = el.type_category;
			el.m_cat = m_subCategories.m_data[i];
			m_elements.push_back(el);
		}

		for (size_t i = 0; i < m_examples.m_size; ++i)
		{
			el.type = el.type_example;
			el.m_ex = m_examples.m_data[i];
			m_elements.push_back(el);
		}
	}
public:
	DemoCategory() {}
	virtual ~DemoCategory() 
	{
		for (size_t i = 0; i < m_subCategories.m_size; ++i)
		{
			delete m_subCategories.m_data[i];
		}
	}
};

class DemoApp
{
	friend class WindowCallback;

	slString m_drawingText;

	FrameworkCallback* m_frameworkCallback = 0;
	WindowCallback* m_windowCallback = 0;
	GUIDrawTextCallback* m_textDrawCallback = 0;
	bool m_isRun = true;
	
	slWindow* m_window = 0;
	slGS* m_gs = 0;
	//slCamera* m_camera = 0;
	slGUIWindow* m_GUIWindow = 0;
	slInputData* m_inputData = 0;
	slGUIFont* m_fontDefault = 0;
	float* m_dt = 0;

	DemoCategory m_rootCategory;
	DemoCategory* m_currentCategory = 0;

	slArray<DemoExample*> m_allExamples;

public:
	DemoApp();
	~DemoApp();
	bool Init();
	void Run();
	void Quit() { m_isRun = false; }

	void AddExample(DemoExample*, const char32_t* name, const char* category);
	void OnDraw();
	DemoCategory* GetCategory(const char* category);
	DemoCategory* FindCategory(DemoCategory*, const char* name);
};

class FrameworkCallback : public slFrameworkCallback
{
public:
	FrameworkCallback() {}
	virtual ~FrameworkCallback() {}
	
	virtual void OnMessage() override
	{
	}
};

class WindowCallback : public slWindowCallback
{
public:
	WindowCallback() {}
	virtual ~WindowCallback() {}
	
	virtual void OnSizing(slWindow* w) override 
	{
	}

	virtual void OnSize(slWindow* w) override
	{
		DemoApp* app = (DemoApp*)w->GetUserData();
		if (app)
		{
			if (w->GetCurrentSize()->x && w->GetCurrentSize()->y)
				//app->m_camera->m_aspect = w->GetCurrentSize()->x / w->GetCurrentSize()->y;
			app->m_gs->UpdateMainRenderTarget(slVec3f(w->GetCurrentSize()->x * 0.25f, w->GetCurrentSize()->y * 0.25f, 0.f));
			app->m_gs->SetViewport(0, 0, (uint32_t)w->GetCurrentSize()->x, (uint32_t)w->GetCurrentSize()->y);
			app->m_gs->SetScissorRect(slVec4f(0.f, 0.f, w->GetCurrentSize()->x, w->GetCurrentSize()->y), 0);
		//	app->m_GUIWindow->SetPositionAndSize(slVec2f(100.f), slVec2f(w->GetCurrentSize()->x - 400.f, w->GetCurrentSize()->y - 300.f));
		//	app->m_GUIWindow->Rebuild();
		}
	}

	virtual void OnClose(slWindow* w) override 
	{
		DemoApp* app = (DemoApp*)w->GetUserData();
		if (app)
		{
			w->SetVisible(false);
			app->Quit();
		}
	}
};

class GUIDrawTextCallback : public slGUIDrawTextCallback
{
public:
	GUIDrawTextCallback(slGUIFont* f):m_font(f) {}
	virtual ~GUIDrawTextCallback() {}
	virtual slGUIFont* OnFont(uint32_t reason, char32_t c) final
	{
		return m_font;
	}
	virtual slColor* OnColor(uint32_t reason, char32_t c) final
	{
		return m_curColor;
	}

	slGUIFont* m_font = 0;
	slColor m_colorWhite = ColorWhite;
	slColor m_colorRed = ColorRed;
	slColor m_colorYellow = ColorGold;
	slColor* m_curColor = &m_colorYellow;
};

