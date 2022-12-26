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
#include "slowlib.base/gs/slTexture.h"
#include "slowlib.base/gs/slImageLoader.h"
#include "slowlib.base/geometry/slMeshLoader.h"
#include "slowlib.base/geometry/slPolyMesh.h"
#include "slowlib.base/scene/slCamera.h"
#include "slowlib.base/archive/slArchive.h"
#include "slowlib.base/GUI/slGUI.h"

static uint8_t g_defaultFontPNG[] = {
	#include "../data/font.inl"
};
static uint8_t g_defaultIconsPNG[] = {
	#include "../data/defaultIcons.inl"
};

#include <stdio.h>
#include <time.h>
#include <filesystem>

#include <algorithm>

//
//  Lowercases string
//
template <typename T>
std::basic_string<T> lowercase(const std::basic_string<T>& s)
{
	std::basic_string<T> s2 = s;
	std::transform(s2.begin(), s2.end(), s2.begin(), tolower);
	return s2;
}

//
// Uppercases string
//
template <typename T>
std::basic_string<T> uppercase(const std::basic_string<T>& s)
{
	std::basic_string<T> s2 = s;
	std::transform(s2.begin(), s2.end(), s2.begin(), toupper);
	return s2;
}

#ifdef SL_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "slFrameworkImpl.h"

extern "C"
{
	slGS* SL_CDECL slGSD3D11_create();
	slMeshLoader* SL_CDECL slMeshLoaderOBJ_create();
	slImageLoader* SL_CDECL slImageLoaderDefault_create();
}
SL_LINK_LIBRARY("slowlib.d3d11");
SL_LINK_LIBRARY("slowlib.meshloader");
SL_LINK_LIBRARY("slowlib.imageloader");
SL_LINK_LIBRARY("zlib");
SL_LINK_LIBRARY("minizip");
SL_LINK_LIBRARY("libpng");
SL_LINK_LIBRARY("libjpeg");


slFrameworkImpl* g_framework = 0;

void slFrameworkImpl::OnDestroy()
{
	for (size_t i = 0; i < m_defaultFonts.m_size; ++i)
	{
		SLSAFE_DESTROY(m_defaultFonts.m_data[i]);
	}

	for (size_t i = 0; i < m_texturesForDestroy.m_size; ++i)
	{
		SLSAFE_DESTROY(m_texturesForDestroy.m_data[i]);
	}

	if (g_framework->m_GUIWindows.m_head)
	{
		auto cw = g_framework->m_GUIWindows.m_head;
		auto lw = cw->m_left;

		while (1)
		{
			auto nw = cw->m_right;

			slFramework::DestroyGUIWindow(cw->m_data);
			if (cw == lw)
				break;
			cw = nw;
		}
		g_framework->m_GUIWindows.clear();
	}
	_onDestroy_archive();
	if (g_framework->m_imageLoaders.size())
	{
		for (auto o : g_framework->m_imageLoaders)
		{
			slDestroy(o);
		}
		g_framework->m_imageLoaders.clear();
	}
	if (g_framework->m_meshLoaders.size())
	{
		for (auto o : g_framework->m_meshLoaders)
		{
			slDestroy(o);
		}
		g_framework->m_meshLoaders.clear();
	}
	if (g_framework->m_gss.size())
	{
		for (auto o : g_framework->m_gss)
		{
			o->Shutdown();
			slDestroy(o);
		}
		g_framework->m_gss.clear();
	}
}

void slFramework::Start(slFrameworkCallback* cb)
{
	SL_ASSERT_ST(cb);
	SL_ASSERT_ST(g_framework == 0);
	if (!g_framework)
	{
		slLog::PrintInfo("Init SlowLib...\n");

		g_framework = slCreate<slFrameworkImpl>();
		g_framework->m_callback = cb;
		
#ifdef SL_PLATFORM_WINDOWS
		wchar_t pth[1000];
		GetModuleFileName(0, pth, 1000);
		g_framework->m_appPath = pth;
		g_framework->m_appPath.pop_back_before(U'\\');
		g_framework->m_appPath.replace(U'\\', U'/');
#else
#error OMG
#endif

		g_framework->m_gss.push_back(slGSD3D11_create());
		g_framework->m_meshLoaders.push_back(slMeshLoaderOBJ_create());
		g_framework->m_imageLoaders.push_back(slImageLoaderDefault_create());
	}

	g_framework->m_GUIStyleThemeLight.m_windowActiveBGColor1 = 0xE1E6F7;
	g_framework->m_GUIStyleThemeLight.m_windowActiveBGColor2 = 0xEFEFFF;
	g_framework->m_GUIStyleThemeLight.m_windowActiveBorderColor = 0xE1E6F7;
	g_framework->m_GUIStyleThemeLight.m_windowActiveTitleBGColor1 = 0xB5CCFF;
	g_framework->m_GUIStyleThemeLight.m_windowActiveTitleBGColor2 = 0xBFCFFF;
	g_framework->m_GUIStyleThemeLight.m_windowActiveTitleTextColor = 0x0;
	g_framework->m_GUIStyleThemeLight.m_buttonBGColor1 = 0x999999;
	g_framework->m_GUIStyleThemeLight.m_buttonBGColor2 = 0x666666;
	g_framework->m_GUIStyleThemeLight.m_buttonBorderColor = 0x999999;
	g_framework->m_GUIStyleThemeLight.m_buttonTextColor = 0x0;
	g_framework->m_GUIStyleThemeLight.m_buttonDisabledBGColor1 = 0x999999;
	g_framework->m_GUIStyleThemeLight.m_buttonDisabledBGColor2 = 0x666666;
	g_framework->m_GUIStyleThemeLight.m_buttonDisabledBorderColor = 0x999999;
	g_framework->m_GUIStyleThemeLight.m_buttonDisabledTextColor = 0x555555;
	g_framework->m_GUIStyleThemeLight.m_buttonMouseHoverBGColor1 = 0xAAAAAA;
	g_framework->m_GUIStyleThemeLight.m_buttonMouseHoverBGColor2 = 0x777777;
	g_framework->m_GUIStyleThemeLight.m_buttonMouseHoverTextColor = 0x222222;
	g_framework->m_GUIStyleThemeLight.m_buttonMousePressBGColor1 = 0x777777;
	g_framework->m_GUIStyleThemeLight.m_buttonMousePressBGColor2 = 0x444444;
	g_framework->m_GUIStyleThemeLight.m_buttonMousePressTextColor = 0xFF0000;
	
	g_framework->m_GUIStyleThemeLight.m_chkradioDisabledTextColor = 0xFF0000;
	g_framework->m_GUIStyleThemeLight.m_chkradioMouseHoverTextColor = ColorYellow;
	g_framework->m_GUIStyleThemeLight.m_chkradioMousePressTextColor = 0xFF55FF;
	g_framework->m_GUIStyleThemeLight.m_chkradioTextColor = 0x0;

	g_framework->m_GUIStyleThemeLight.m_textEditorBGColor = 0xFFFFFF;
	g_framework->m_GUIStyleThemeLight.m_textEditorSelectedTextBGColor = 0x333333;
	g_framework->m_GUIStyleThemeLight.m_textEditorTextColor = 0x0;
	g_framework->m_GUIStyleThemeLight.m_textEditorSelectedTextColor = 0x0;
	g_framework->m_GUIStyleThemeLight.m_textEditorCursorColor = 0x0;

	g_framework->m_GUIStyleThemeDark = g_framework->m_GUIStyleThemeLight;
}

void slFramework::Stop()
{
	SL_ASSERT_ST(g_framework);
	if (g_framework)
	{
		g_framework->OnDestroy();
		slDestroy(g_framework);
		g_framework = 0;
	}
}

slString slFramework::GetAppPath()
{
	return g_framework->m_appPath;
}

slStringA slFramework::GetPathA(const slString& v)
{
	slString p = g_framework->m_appPath;
	p.append(v);
	slStringA stra;
	p.to_utf8(stra);

	if (!std::filesystem::exists(stra.c_str()))
	{
		p.assign(v);

		while(p.size())
		{
			if (p[0] == U'.'
				|| p[0] == U'\\'
				|| p[0] == U'/')
				p.pop_front();
			else
				break;
		}

		p.to_utf8(stra);
	}

	return stra;
}

void slFramework::SetImageLoaderConvertToRGBA8(bool v)
{
	g_framework->m_imageLoaderConverToRGBA8 = v;
}

bool slFramework::GetImageLoaderConvertToRGBA8()
{
	return g_framework->m_imageLoaderConverToRGBA8;
}

void slFramework::Update()
{
	SL_ASSERT_ST(g_framework);
	slInputUpdatePre(&g_framework->m_input);

#ifdef SL_PLATFORM_WINDOWS
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		GetMessage(&msg, NULL, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#endif

	slInputUpdatePost(&g_framework->m_input);

	g_framework->UpdateGUI();

	static clock_t then = 0;
	clock_t now = clock();

	g_framework->m_deltaTime = (float)(now - then) / CLOCKS_PER_SEC;
	then = now;
}

void slFrameworkCallback::OnMessage()
{
	printf("ON MESSAGE\n");
}

slWindow* slFramework::SummonWindow(slWindowCallback* cb, int sx, int sy)
{
	SL_ASSERT_ST(cb);

	return slCreate<slWindow>(cb, sx, sy);
}

float* slFramework::GetDeltaTime()
{
	SL_ASSERT_ST(g_framework);
	return &g_framework->m_deltaTime;
}

bool slFramework::PointInRect(slRect* r, slPoint* p)
{
	SL_ASSERT_ST(r);
	SL_ASSERT_ST(p);
	if (p->x >= r->left) { if (p->x <= r->right) { if (p->y >= r->top) { if (p->y <= r->bottom) { return true; } } } }
	return false;
}

void slFramework::PointSet(slPoint* p, int32_t x, int32_t y)
{
	SL_ASSERT_ST(p);
	p->x = x;
	p->y = y;
}

void slFramework::RectSet(slRect* rct, int32_t l, int32_t t, int32_t r, int32_t b)
{
	SL_ASSERT_ST(rct);
	rct->left = l;
	rct->top = t;
	rct->right = r;
	rct->bottom = b;
}

void slFramework::RectfSet(slRectf* rct, float* f)
{
	SL_ASSERT_ST(rct);
	rct->left = f[0];
	rct->top = f[1];
	rct->right = f[2];
	rct->bottom = f[3];
}

bool slFramework::CompareUIDs(const slUID& id1, const slUID& id2)
{
	const uint8_t* b1 = (const uint8_t*)&id1.d1;
	const uint8_t* b2 = (const uint8_t*)&id2.d1;
	for (int i = 0; i < 16; ++i)
	{
		if (b1[i] != b2[i])
			return false;
	}
	return true;
}

// =========== GS
uint32_t slFramework::GetGSNum()
{
	return (uint32_t)g_framework->m_gss.size();
}

slString slFramework::GetGSName(uint32_t i)
{
	SL_ASSERT_ST(i < g_framework->m_gss.size());
	return g_framework->m_gss[i]->GetName();
}

slUID slFramework::GetGSUID(uint32_t i)
{
	SL_ASSERT_ST(i < g_framework->m_gss.size());
	return g_framework->m_gss[i]->GetUID();
}

slGS* slFramework::SummonGS(slUID id)
{
	for (auto o : g_framework->m_gss)
	{
		if (CompareUIDs(o->GetUID(), id))
			return o;
	}
	return 0;
}

slGS* slFramework::SummonGS(const char* _name)
{
	slString name(_name);
	for (auto o : g_framework->m_gss)
	{
		slString o_name = o->GetName();
		if (name == o_name)
			return o;
	}
	return 0;
}

slGS* slFramework::SummonGS(slUID id, const char* _name)
{
	slString name(_name);
	for (auto o : g_framework->m_gss)
	{
		if (CompareUIDs(o->GetUID(), id))
		{
			slString o_name = o->GetName();
			if (name == o_name)
				return o;
		}
	}
	return 0;
}

uint32_t slFramework::GetMeshLoadersNum()
{
	return (uint32_t)g_framework->m_meshLoaders.size();
}

slMeshLoader* slFramework::GetMeshLoader(uint32_t i)
{
	SL_ASSERT_ST(i < g_framework->m_meshLoaders.size());
	return g_framework->m_meshLoaders[i];
}

void slFramework::SummonMesh(const char* path, slMeshLoaderCallback* cb)
{
	slStringA stra;
	std::filesystem::path p = path;
	auto e = p.extension();
	uint32_t mln = GetMeshLoadersNum();
	for (uint32_t i = 0; i < mln; ++i)
	{
		auto ml = GetMeshLoader(i);
		auto sfc = ml->GetSupportedFilesCount();
		for (uint32_t o = 0; o < sfc; ++o)
		{
			slString sfe = ml->GetSupportedFileExtension(o);
			sfe.insert(U".", 0);
			sfe.to_lower();
			sfe.to_utf8(stra);
			auto stre = lowercase(e.generic_string());
			if (strcmp((const char*)stra.m_data, stre.c_str()) == 0)
			{
				ml->Load(path, cb);
				return;
			}
		}
	}
}

slMat4* slFramework::GetMatrix(slMatrixType t)
{
	return g_framework->m_matrixPtrs[(uint32_t)t];
}

void slFramework::SetMatrix(slMatrixType t, slMat4* m)
{
	g_framework->m_matrixPtrs[(uint32_t)t] = m;
}

slCamera* slFramework::SummonCamera()
{
	return slCreate<slCamera>();
}

bool slFramework::FileExist(const char* p)
{
	return std::filesystem::exists(p);
}

bool slFramework::FileExist(const slString& p)
{
	g_framework->m_fileExistString.clear();
	p.to_utf8(g_framework->m_fileExistString);
	return std::filesystem::exists(g_framework->m_fileExistString.m_data);
}

uint64_t slFramework::FileSize(const char* p)
{
	return (uint64_t)std::filesystem::file_size(p);
}

uint64_t slFramework::FileSize(const slString& p)
{
	g_framework->m_fileSizeString.clear();
	p.to_utf8(g_framework->m_fileSizeString);
	return (uint64_t)std::filesystem::file_size(g_framework->m_fileSizeString.m_data);
}

slPolygonMesh* slFramework::SummonPolygonMesh()
{
	return slCreate<slPolygonMesh>();
}

uint32_t slFramework::GetImageLoadersNum()
{
	return (uint32_t)g_framework->m_imageLoaders.size();
}

slImageLoader* slFramework::GetImageLoader(uint32_t i)
{
	SL_ASSERT_ST(i < g_framework->m_imageLoaders.size());
	return g_framework->m_imageLoaders[i];
}

slImage* slFramework::SummonImage(const char* path)
{
	slStringA stra;
	std::filesystem::path p = path;
	auto e = p.extension();
	uint32_t mln = GetImageLoadersNum();
	for (uint32_t i = 0; i < mln; ++i)
	{
		auto il = GetImageLoader(i);
		auto sfc = il->GetSupportedFilesCount();
		for (uint32_t o = 0; o < sfc; ++o)
		{
			slString sfe = il->GetSupportedFileExtension(o);
			sfe.insert(U".", 0);
			sfe.to_lower();
			sfe.to_utf8(stra);
			auto stre = lowercase(e.generic_string());
			if (strcmp((const char*)stra.m_data, stre.c_str()) == 0)
			{
				return il->Load(path);
			}
		}
	}
	return NULL;
}

uint8_t* slFramework::SummonFileBuffer(const char* path, uint32_t* szOut, bool isText)
{
	SL_ASSERT_ST(path);
	SL_ASSERT_ST(szOut);

	*szOut = 0;
	
	std::filesystem::path p = path;

	if (std::filesystem::exists(p))
	{
		*szOut = (uint32_t)std::filesystem::file_size(p);
		if (*szOut)
		{
			FILE* f = 0;
			fopen_s(&f, path, "rb");
			if (f)
			{
				if (isText)
					*szOut += 2;

				uint8_t* data = (uint8_t*)slMemory::malloc(*szOut);
				fread(data, *szOut, 1, f);
				fclose(f);

				if (isText)
				{
					data[*szOut - 2] = ' ';
					data[*szOut - 1] = 0;
				}

				return data;
			}
			else
			{
				slLog::PrintError("Unable to open file in %s : %i\n", SL_FUNCTION, SL_LINE);
			}
		}
	}

	return slArchiveSystem::ZipUnzip(path, szOut, 0);
}

slGUIFont* slFramework::SummonFont()
{
	return slCreate<slGUIFont>();
}

slGUIStyle* slFramework::GetGUIStyle(const slGUIStyleTheme& theme)
{
	switch (theme)
	{
	case slGUIStyleTheme::Light:
		return &g_framework->m_GUIStyleThemeLight;
	case slGUIStyleTheme::Dark:
		return &g_framework->m_GUIStyleThemeDark;
	}
	return &g_framework->m_GUIStyleThemeLight;
}

slGUIWindow* slFramework::SummonGUIWindow(const slVec2f& position, const slVec2f& size)
{
	slGUIWindow* newWindow = slCreate<slGUIWindow>(position, size);
	newWindow->SetStyle(slFramework::GetGUIStyle(slGUIStyleTheme::Light));
	g_framework->m_GUIWindows.push_back(newWindow);
	return newWindow;
}


void DestroyGUIElement_internal(slGUIElement* e)
{
	if (e->GetChildren()->m_head)
	{
		auto children = e->GetChildren();
		if (children->m_head)
		{
			auto curr = children->m_head;
			auto last = curr->m_left;
			while (1)
			{
				DestroyGUIElement_internal(dynamic_cast<slGUIElement*>(curr->m_data));
				if (curr == last)
					break;
				curr = curr->m_right;
			}
		}
	}

	slDestroy(e);
}

void _DestroyGUIElement(slGUIElement* e)
{
	e->SetParent(0);
	DestroyGUIElement_internal(e);
}
void slFramework::DestroyGUIElement(slGUIElement* e)
{
	SL_ASSERT_ST(e);
	if (e->GetWindow()->GetRootElement() == e)
		return;
	_DestroyGUIElement(e);
}

void slFramework::DestroyGUIWindow(slGUIWindow* w)
{
	SL_ASSERT_ST(w);
	_DestroyGUIElement(w->GetRootElement());
	g_framework->m_GUIWindows.erase_first(w);
	slDestroy(w);
}

void slFrameworkImpl::UpdateGUI()
{
	if (m_GUIWindows.m_head)
	{
		// reset it here, it will set in Update if cursor in window rect
		m_GUIState.m_windowUnderCursor = 0;

		auto last = m_GUIWindows.m_head;
		auto curr = last->m_left;
		while (1)
		{
			if(curr->m_data->IsVisible() && !m_GUIState.m_windowUnderCursor)
			{
				curr->m_data->Update(&m_input);
			}

			if (curr == last)
				break;
			curr = curr->m_left;
		}
	}
}

slGUIState* slFramework::GetGUIState()
{
	return &g_framework->m_GUIState;
}

void slFramework::DrawGUI(slGS* gs)
{
	if (g_framework->m_GUIWindows.m_head)
	{
		auto last = g_framework->m_GUIWindows.m_head;
		auto curr = last->m_left;
		while (1)
		{
			if (curr->m_data->IsVisible())
			{
				curr->m_data->Draw(gs, g_framework->m_deltaTime);
			}

			if (curr == last)
				break;
			curr = curr->m_left;
		}
	}
}

void slFramework::RebuildGUI()
{
	if (g_framework->m_GUIWindows.m_head)
	{
		auto last = g_framework->m_GUIWindows.m_head;
		auto curr = last->m_left;
		while (1)
		{
			curr->m_data->Rebuild();

			if (curr == last)
				break;
			curr = curr->m_left;
		}
	}
}

slGUIFont* slFramework::GetDefaultFont(const slGUIDefaultFont& t)
{
	SL_ASSERT_ST(g_framework->m_defaultFonts.m_size);
	
	switch (t)
	{
	case slGUIDefaultFont::Icons:
	case slGUIDefaultFont::Text:
		return g_framework->m_defaultFonts.m_data[(uint32_t)t];
	default:
		slLog::PrintWarning("%s : not implemented\n", SL_FUNCTION);
		break;
	}
	return g_framework->m_defaultFonts.m_data[0];
}

void slFramework::InitDefaultFonts(slGS* gs)
{
	static bool isInit = false;

	if (!isInit)
	{
		auto getImage = [](uint8_t* buf, uint32_t sz)->slImage* {
			for (uint32_t i = 0; i < slFramework::GetImageLoadersNum(); ++i)
			{
				auto il = slFramework::GetImageLoader(i);
				for (uint32_t o = 0; o < il->GetSupportedFilesCount(); ++o)
				{
					auto str = il->GetSupportedFileExtension(o);
					if (str == U"png")
					{
						return il->Load("something/file.png", buf, sz);
					}
				}
			}
			return 0;
		};

		auto getTexture = [gs](slImage* img)->slTexture* {
			slTextureInfo ti;
			slTexture* t = gs->SummonTexture(img, ti);
			SLSAFE_DESTROY(img);
			return t;
		};

		slImage* img = getImage(g_defaultFontPNG, 9065);

		if (!img)
			return;		

		slTexture* myFontTexture = getTexture(img);

		if (!myFontTexture)
			return;

		g_framework->m_texturesForDestroy.push_back(myFontTexture);

		slGUIFont* myFont = slFramework::SummonFont();
		myFont->AddTexture(myFontTexture);
		myFont->AddGlyph(U'A', slVec2f(0, 0), slPoint(11, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'B', slVec2f(10, 0), slPoint(9, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'C', slVec2f(19, 0), slPoint(9, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'D', slVec2f(27, 0), slPoint(11, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'E', slVec2f(37, 0), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'F', slVec2f(45, 0), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'G', slVec2f(52, 0), slPoint(11, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'H', slVec2f(62, 0), slPoint(10, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'I', slVec2f(72, 0), slPoint(4, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'J', slVec2f(76, 0), slPoint(6, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'K', slVec2f(82, 0), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'L', slVec2f(90, 0), slPoint(7, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'M', slVec2f(97, 0), slPoint(13, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'N', slVec2f(110, 0), slPoint(10, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'O', slVec2f(120, 0), slPoint(11, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'P', slVec2f(131, 0), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'Q', slVec2f(139, 0), slPoint(12, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'R', slVec2f(151, 0), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'S', slVec2f(159, 0), slPoint(7, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'T', slVec2f(166, 0), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'U', slVec2f(175, 0), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'V', slVec2f(184, 0), slPoint(9, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'W', slVec2f(193, 0), slPoint(14, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'X', slVec2f(207, 0), slPoint(9, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'Y', slVec2f(216, 0), slPoint(7, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'Z', slVec2f(224, 0), slPoint(7, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'0', slVec2f(231, 0), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'1', slVec2f(239, 0), slPoint(7, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'2', slVec2f(247, 0), slPoint(7, 15), 0, slPoint(256, 256));

		myFont->AddGlyph(U'3', slVec2f(1, 20), slPoint(7, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'4', slVec2f(9, 20), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'5', slVec2f(17, 20), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'6', slVec2f(25, 20), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'7', slVec2f(33, 20), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'8', slVec2f(41, 20), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'9', slVec2f(49, 20), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'a', slVec2f(57, 20), slPoint(8, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'b', slVec2f(65, 20), slPoint(8, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'c', slVec2f(73, 20), slPoint(8, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'd', slVec2f(80, 20), slPoint(8, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'e', slVec2f(89, 20), slPoint(8, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'f', slVec2f(96, 20), slPoint(6, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'g', slVec2f(102, 20), slPoint(6, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'h', slVec2f(109, 20), slPoint(7, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'i', slVec2f(117, 20), slPoint(3, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'j', slVec2f(120, 20), slPoint(4, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'k', slVec2f(125, 20), slPoint(7, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'l', slVec2f(132, 20), slPoint(3, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'm', slVec2f(136, 20), slPoint(12, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'n', slVec2f(148, 20), slPoint(8, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'o', slVec2f(157, 20), slPoint(8, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'p', slVec2f(165, 20), slPoint(8, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'q', slVec2f(173, 20), slPoint(8, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'r', slVec2f(182, 20), slPoint(6, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U's', slVec2f(188, 20), slPoint(5, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U't', slVec2f(194, 20), slPoint(5, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'u', slVec2f(199, 20), slPoint(7, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'v', slVec2f(207, 20), slPoint(7, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'w', slVec2f(214, 20), slPoint(12, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'x', slVec2f(226, 20), slPoint(7, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'y', slVec2f(233, 20), slPoint(7, 16), 0, slPoint(256, 256));
		myFont->AddGlyph(U'z', slVec2f(240, 20), slPoint(7, 16), 0, slPoint(256, 256));

		myFont->AddGlyph(U'~', slVec2f(1, 39), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'`', slVec2f(9, 39), slPoint(4, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'!', slVec2f(14, 39), slPoint(4, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'?', slVec2f(19, 39), slPoint(7, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'\'', slVec2f(26, 39), slPoint(4, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'\"', slVec2f(30, 39), slPoint(5, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'@', slVec2f(37, 39), slPoint(13, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'#', slVec2f(50, 39), slPoint(9, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'№', slVec2f(59, 39), slPoint(15, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U';', slVec2f(75, 39), slPoint(3, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U':', slVec2f(80, 39), slPoint(3, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'%', slVec2f(84, 39), slPoint(11, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'&', slVec2f(95, 39), slPoint(10, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'^', slVec2f(106, 39), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'<', slVec2f(114, 39), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'>', slVec2f(121, 39), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'(', slVec2f(130, 39), slPoint(5, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U')', slVec2f(134, 39), slPoint(5, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'[', slVec2f(140, 39), slPoint(5, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U']', slVec2f(144, 39), slPoint(5, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'{', slVec2f(150, 39), slPoint(5, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'}', slVec2f(154, 39), slPoint(5, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'*', slVec2f(160, 39), slPoint(6, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'.', slVec2f(167, 39), slPoint(3, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U',', slVec2f(171, 39), slPoint(3, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'/', slVec2f(174, 39), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'\\', slVec2f(181, 39), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'|', slVec2f(189, 39), slPoint(4, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'+', slVec2f(195, 39), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'-', slVec2f(203, 39), slPoint(5, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'=', slVec2f(208, 39), slPoint(7, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'_', slVec2f(216, 39), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U'$', slVec2f(233, 39), slPoint(8, 15), 0, slPoint(256, 256));
		myFont->AddGlyph(U' ', slVec2f(225, 39), slPoint(8, 15), 0, slPoint(256, 256));

		g_framework->m_defaultFonts.push_back(myFont);

		img = getImage(g_defaultIconsPNG, 4825);
		if (img)
		{
			myFontTexture = getTexture(img);
			if (myFontTexture)
			{
				g_framework->m_texturesForDestroy.push_back(myFontTexture);

				myFont = slFramework::SummonFont();
				myFont->AddTexture(myFontTexture);
				myFont->AddGlyph((uint32_t)slGUIDefaultIconID::CheckboxUncheck, slVec2f(0, 0), slPoint(14, 14), 0, slPoint(512, 512));
				myFont->AddGlyph((uint32_t)slGUIDefaultIconID::RadioUncheck, slVec2f(0, 0), slPoint(14, 14), 0, slPoint(512, 512));
				myFont->AddGlyph((uint32_t)slGUIDefaultIconID::CheckboxCheck, slVec2f(28, 0), slPoint(14, 14), 0, slPoint(512, 512));
				myFont->AddGlyph((uint32_t)slGUIDefaultIconID::RadioCheck, slVec2f(14, 0), slPoint(14, 14), 0, slPoint(512, 512));
				myFont->AddGlyph((uint32_t)slGUIDefaultIconID::Minus, slVec2f(43, 0), slPoint(14, 14), 0, slPoint(512, 512));
				myFont->AddGlyph((uint32_t)slGUIDefaultIconID::Plus, slVec2f(57, 0), slPoint(14, 14), 0, slPoint(512, 512));
				myFont->AddGlyph((uint32_t)slGUIDefaultIconID::ArrowUp, slVec2f(71, 0), slPoint(14, 14), 0, slPoint(512, 512));
				myFont->AddGlyph((uint32_t)slGUIDefaultIconID::ArrowDonw, slVec2f(85, 0), slPoint(14, 14), 0, slPoint(512, 512));
				myFont->AddGlyph((uint32_t)slGUIDefaultIconID::ArrowRight, slVec2f(97, 0), slPoint(14, 14), 0, slPoint(512, 512));
				myFont->AddGlyph((uint32_t)slGUIDefaultIconID::ArrowLeft, slVec2f(109, 0), slPoint(14, 14), 0, slPoint(512, 512));

				g_framework->m_defaultFonts.push_back(myFont);
			}
		}

		isInit = true;
	}
}

slGUIFont* DefaultGUIDrawTextCallback::OnFont(uint32_t, char32_t i) {
	SL_ASSERT_ST(g_framework->m_defaultFonts.m_size);
	return g_framework->m_defaultFonts[0];
}

slColor* DefaultGUIDrawTextCallback::OnColor(uint32_t, char32_t i) {
	return &g_framework->m_colorWhite;
}


slGUIDrawTextCallback* slFramework::GetDefaultDrawTextCallback()
{
	return &g_framework->m_defaultDrawTextCallback;
}

