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

#include "slowlib.h"

SL_LINK_LIBRARY("slowlib.base");

#include <stdio.h>
#include <Windows.h>

bool g_isRun = true;

class FrameworkCallback : public slFrameworkCallback
{
public:
	FrameworkCallback() {}
	virtual ~FrameworkCallback() {}
	
	virtual void OnMessage() override
	{
		printf("OnMessage\n");
	}
};

class WindowCallback : public slWindowCallback
{
public:
	WindowCallback() {}
	virtual ~WindowCallback() {}
	
	virtual void OnClose(slWindow* w) override 
	{
		w->SetVisible(false);
		g_isRun = false;
	}
};

int main(int argc, char * argv[])
{
	FrameworkCallback frameworkCallback;
	WindowCallback windowCallback;

	slFramework::Start(&frameworkCallback);
	slWindow * window = slFramework::SummonWindow(&windowCallback);
	window->SetVisible(true);

	auto inputData = slInput::GetData();

	while (g_isRun)
	{
		slFramework::Update();

		Sleep(1);
		//frameworkCallback.OnMessage();

		if(slInput::IsLMBHold())
			wprintf(L"%i %i %c[%u]\n", inputData->mousePosition.x, inputData->mousePosition.y, (wchar_t)inputData->character, (uint32_t)inputData->character);

		if (slInput::IsLMBHit())
			Beep(400, 100);
		if (slInput::IsLMBRelease())
			Beep(600, 100);

		if (slInput::IsKeyHit(slInputData::KEY_ESCAPE))
			windowCallback.OnClose(window);

		if (slInput::IsKeyRelease(slInputData::KEY_BACKSPACE))
			windowCallback.OnClose(window);
	}

	if (window)
		slDestroy(window);

	slFramework::Stop();

	return EXIT_SUCCESS;
}
