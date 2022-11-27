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

#ifdef SL_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

class slFrameworkImpl
{
public:
	slFrameworkImpl() {}
	~slFrameworkImpl() 
	{
	}

	slFrameworkCallback* m_callback = 0;

	void OnDestroy();
};
slFrameworkImpl* g_framework = 0;

void slFrameworkImpl::OnDestroy()
{
}


void slFramework::Start(slFrameworkCallback* cb)
{
	SL_ASSERT_ST(cb);
	SL_ASSERT_ST(g_framework == 0);
	if (!g_framework)
	{
		g_framework = slCreate<slFrameworkImpl>();
		g_framework->m_callback = cb;
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

void slFrameworkCallback::OnMessage()
{
	printf("ON MESSAGE\n");
}
