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

#include "DemoApp.h"
#include "examples/scene/ExampleSceneSprite.h"
#include "examples/basics/ExampleBasics3DLineAndCamera.h"
#include "examples/basics/ExampleBasicsScreenResolution.h"

SL_LINK_LIBRARY("slowlib.base");

class MyStaticText : public slGUIStaticText
{
public:
	MyStaticText(slGUIWindow* w, const slVec2f& position, const slVec2f& size) :
		slGUIStaticText(w, position, size)
	{
	}
	virtual ~MyStaticText() {}
};

DemoExample::DemoExample(DemoApp* app) 
	:
	m_app(app), 
	m_gs(app->GetGS()) 
{}

DemoApp::DemoApp()
{
}

DemoApp::~DemoApp()
{
	for (size_t i = 0; i < m_allExamples.m_size; ++i)
	{
		delete m_allExamples.m_data[i];
	}

	SLSAFE_DESTROY(m_textDrawCallback);
	SLSAFE_DESTROY(m_frameworkCallback);
	SLSAFE_DESTROY(m_windowCallback);
	SLSAFE_DESTROY(m_window);
	slFramework::Stop();
}

bool DemoApp::Init()
{
	m_frameworkCallback = slCreate<FrameworkCallback>();
	m_windowCallback = slCreate<WindowCallback>();
	
	slFramework::Start(m_frameworkCallback);
	
	m_window = slFramework::SummonWindow(m_windowCallback, 800, 600);
	m_window->SetVisible(true);	

	slArchiveSystem::ZipAdd(slFramework::GetPathA("..\\data\\demo.zip").c_str());
	m_inputData = slInput::GetData();

	// m_gs will be auto destroyed, do not call slDestroy(m_gs);
	m_gs = slFramework::SummonGS(slFramework::GetGSUID(0));
	if (m_gs)
	{
		if (!m_gs->Init(m_window, 0))
			return false;
		m_gs->SetClearColor(0.f, 0.f, 1.f, 1.f);
		m_window->SetUserData(this);
	}
	else
	{
		return false;
	}

	slFramework::InitDefaultFonts(m_gs);
	m_fontDefault = slFramework::GetDefaultFont(slGUIDefaultFont::Text);

	m_textDrawCallback = slCreate<GUIDrawTextCallback>(m_fontDefault);

	m_GUIWindow = slFramework::SummonGUIWindow(slVec2f(300.f, 0.f),
		slVec2f(300.f, 400.f));
	m_GUIWindow->SetDrawBG(false);
	auto staticText = slCreate<MyStaticText>(m_GUIWindow, slVec2f(0.f, 0.f), slVec2f(300.f, 100.f));
	m_staticTextDescription = staticText;
	m_staticTextDescription->SetText(U" ");

	static slGUIStyle styleInMenu;
	styleInMenu = *slFramework::GetGUIStyle(slGUIStyleTheme::Light);
	styleInMenu.m_staticTextTextColor = ColorWhite;
	
	staticText->SetStyle(&styleInMenu);
	staticText->SetDrawBG(false);

	slFramework::RebuildGUI();

	m_dt = slFramework::GetDeltaTime();

	m_gs->SetClearColor(0.2f, 0.2f, 0.2f, 1.f);


	m_currentCategory = &m_rootCategory;
	AddExample(new ExampleBasics3DLineAndCamera(this), U"3D line and camera", "basics/", U"Basic thing. Add camera and draw something. Use WASDQE");
	AddExample(new ExampleBasicsScreenResolution(this), U"Screen Resolution", "basics/", U"Change main target size. Use WASDQE");
	AddExample(new ExampleSceneSprite(this), U"Sprite", "scene/", U"Basic sprite");
	//AddExample(new ExampleSprite, U"Physics", "physics/kinematic");
	//AddExample(new ExampleSprite, U"Demo", "/");
	m_rootCategory.findElements();

	m_windowCallback->OnSize(m_window);
	return true;
}

void DemoApp::Run()
{
	float* dt = slFramework::GetDeltaTime();
	while (m_isRun)
	{
		slFramework::Update();

		Sleep(1);

		if (m_activeExample)
			m_activeExample->OnDraw();
		else
			OnDraw();
	}
}

void DemoApp::AddExample(DemoExample* e, const char32_t* name, const char* category,
	const char32_t* description)
{
	m_allExamples.push_back(e);
	e->m_name = name;
	e->m_description = description;
	auto cat = GetCategory(category);
	if (cat)
	{
		cat->m_examples.push_back(e);
		cat->findElements();
	}
}

void DemoApp::OnDraw()
{
	m_gs->BeginGUI();
	slFramework::DrawGUI(m_gs);
	m_gs->SetScissorRect(slVec4f(0.f, 0.f,
		m_window->GetCurrentSize()->x, m_window->GetCurrentSize()->y), 0);

	float Y = 0.f;
	const float lineHeight = 14.f;

	if (m_currentCategory->m_elements.m_size)
	{
		if (slInput::IsKeyHit(slInput::KEY_UP))
		{
			if (m_currentCategory->m_cursor)
				--m_currentCategory->m_cursor;
			else
				m_currentCategory->m_cursor = m_currentCategory->m_elements.m_size - 1;
			findDescription();
		}
		else if (slInput::IsKeyHit(slInput::KEY_DOWN))
		{
			++m_currentCategory->m_cursor;
			if (m_currentCategory->m_cursor == m_currentCategory->m_elements.m_size)
				m_currentCategory->m_cursor = 0;
			findDescription();
		}
		else if (slInput::IsKeyHit(slInput::KEY_ENTER))
		{
			auto & el = m_currentCategory->m_elements.m_data[m_currentCategory->m_cursor];
			switch (el.type)
			{
			case DemoCategory::Element::type_parentDir:
				m_currentCategory = m_currentCategory->m_parentCategory;
				findDescription();
				return;
			case DemoCategory::Element::type_category:
				m_currentCategory = el.m_cat;
				findDescription();
				return;
			case DemoCategory::Element::type_example:
				StartExample(el.m_ex);
				findDescription();
				return;
			}
		}
		else if (slInput::IsKeyHit(slInput::KEY_RIGHT))
		{
			auto& el = m_currentCategory->m_elements.m_data[m_currentCategory->m_cursor];
			switch (el.type)
			{
			case DemoCategory::Element::type_parentDir:
				break;
			case DemoCategory::Element::type_category:
				m_currentCategory = el.m_cat;
				findDescription();
				return;
			case DemoCategory::Element::type_example:
				break;
			}
		}
		else if (slInput::IsKeyHit(slInput::KEY_LEFT))
		{
			if(m_currentCategory->m_parentCategory)
				m_currentCategory = m_currentCategory->m_parentCategory;
			findDescription();
			return;
		}
		

		for (size_t i = 0; i < m_currentCategory->m_elements.m_size; ++i)
		{
			m_textDrawCallback->m_curColor = &m_textDrawCallback->m_colorWhite;
			if (i == m_currentCategory->m_cursor)
				m_textDrawCallback->m_curColor = &m_textDrawCallback->m_colorYellow;

			m_drawingText.clear();
			switch (m_currentCategory->m_elements.m_data[i].type)
			{
				case DemoCategory::Element::type_parentDir:
					m_drawingText.append(U"[...]");
					break;
				case DemoCategory::Element::type_category:
					m_drawingText.append(U"[");
					m_drawingText.append(m_currentCategory->m_elements.m_data[i].m_cat->m_name);
					m_drawingText.append(U"]");
					break;
				case DemoCategory::Element::type_example:
					m_drawingText.append(m_currentCategory->m_elements.m_data[i].m_ex->m_name);
					//m_drawingText.append(U"*");
					break;
			}
			m_gs->DrawGUIText(m_drawingText.c_str(), m_drawingText.size(), slVec2f(0.f, Y), m_textDrawCallback);
			Y += lineHeight;
		}
	}
	m_gs->EndGUI();

	m_gs->BeginDraw();
	m_gs->ClearAll();

	m_gs->EndDraw();
	m_gs->SwapBuffers();
}

DemoCategory* DemoApp::GetCategory(const char* category)
{
	slStringA stra;
	stra = category;

	const char* delimiters = "/";

	DemoCategory* curCat = &m_rootCategory;
	
	char* next_token1 = NULL;

	char* token = strtok_s((char*)stra.m_data, delimiters, &next_token1);
	while (token) 
	{
		auto findCat = FindCategory(curCat, token);
		if (findCat)
		{
			curCat = findCat;
		}
		else
		{
			DemoCategory* newCategory = new DemoCategory;
			newCategory->m_parentCategory = curCat;
			newCategory->m_name = token;
			curCat->m_subCategories.push_back(newCategory);
			curCat->findElements();
			curCat = newCategory;
		}
		curCat->findElements();

		token = strtok_s(nullptr, delimiters, &next_token1);
	}
	curCat->findElements();
	return curCat;
}

DemoCategory* DemoApp::FindCategory(DemoCategory* cat, const char* name)
{
	slString N;
	N = name;
	for (size_t i = 0; i < cat->m_subCategories.m_size; ++i)
	{
		if (cat->m_subCategories.m_data[i]->m_name == N)
			return cat->m_subCategories.m_data[i];
	}

	return nullptr;
}

void DemoApp::StopExample()
{
	if (m_activeExample)
	{
		m_activeExample->Shutdown();
		m_activeExample = 0;
	}
	m_GUIWindow->SetVisible(true);
}

void DemoApp::StartExample(DemoExample* ex)
{
	StopExample();

	if (ex->Init())
		m_activeExample = ex;

	m_GUIWindow->SetVisible(false);
}

void DemoApp::findDescription()
{
	auto i = m_currentCategory->m_cursor;

	switch (m_currentCategory->m_elements.m_data[i].type)
	{
	case DemoCategory::Element::type_parentDir:
	case DemoCategory::Element::type_category:
		m_staticTextDescription->SetText(U" ");
		break;
	case DemoCategory::Element::type_example:
		m_staticTextDescription->SetText(m_currentCategory->m_elements.m_data[i].m_ex->m_description.c_str());
		break;
	}
}
