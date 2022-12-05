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

#include <stdio.h>
#include <time.h>

#ifdef SL_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "slFrameworkImpl.h"

slGS* SL_API slGSD3D11_create();
SL_LINK_LIBRARY("slowlib.d3d11");


slFrameworkImpl* g_framework = 0;

void slFrameworkImpl::OnDestroy(){}

void slFramework::Start(slFrameworkCallback* cb)
{
	SL_ASSERT_ST(cb);
	SL_ASSERT_ST(g_framework == 0);
	if (!g_framework)
	{
		g_framework = slCreate<slFrameworkImpl>();
		g_framework->m_callback = cb;
		slLog::PrintInfo("Init SlowLib...\n");
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

// =========== GS
uint32_t slFramework::GetGSCount()
{
	return 0;
}

slString slFramework::GetGSName(uint32_t)
{
	return slString("-");
}

slUID slFramework::GetGSUID(uint32_t)
{
	slUID s;
	s.d1 = 0;
	return s;
}

slGS* slFramework::SummonGS(slUID)
{
	return 0;
}

slGS* slFramework::SummonGS(const char*)
{
	return slGSD3D11_create();
}

slGS* slFramework::SummonGS(slUID, const char*)
{
	return 0;
}
