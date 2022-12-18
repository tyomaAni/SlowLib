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
#include "slowlib.base/gs/slImageLoader.h"
#include "slowlib.base/geometry/slMeshLoader.h"
#include "slowlib.base/geometry/slPolyMesh.h"
#include "slowlib.base/scene/slCamera.h"
#include "slowlib.base/archive/slArchive.h"

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


slFrameworkImpl* g_framework = 0;

void slFrameworkImpl::OnDestroy()
{
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
#else
#error OMG
#endif

		g_framework->m_gss.push_back(slGSD3D11_create());
		g_framework->m_meshLoaders.push_back(slMeshLoaderOBJ_create());
		g_framework->m_imageLoaders.push_back(slImageLoaderDefault_create());
	}
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
	return stra;
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
