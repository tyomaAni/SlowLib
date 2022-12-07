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
	slWindow * window = slFramework::SummonWindow(&windowCallback, 800, 600);
	window->SetVisible(true);

	slQuaternion qX40;
	slMath::set_rotation(qX40, slMath::DegToRad(40.f), 0.f, 0.f);

	slQuaternion qY20;
	slMath::set_rotation(qY20, 0.f, slMath::DegToRad(20.f), 0.f);


	slMatrix4 m1;
	slMath::set_rotation(m1, qX40);

	slMatrix4 m2;
	slMath::set_rotation(m2, qY20);

	slMatrix4 m3;
	slMath::mul(m1, m2, m3);

	slVec3 pos(0.f, 1.f, 0.f);
	slVec3 posRotated;
	slMath::mul(m3, pos, posRotated);

	auto inputData = slInput::GetData();

	double dd = 0.0;

	slGS* gs = slFramework::SummonGS(slFramework::GetGSUID(0));
	if (gs)
	{
		gs->Init(window, 0);
		gs->SetClearColor(0.f, 0.f, 1.f, 1.f);
	}

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

		if (slInput::IsKeyHit(slInput::KEY_ESCAPE))
			windowCallback.OnClose(window);

		if (slInput::IsKeyRelease(slInput::KEY_HOME))
			window->SetBorderless(true);
		if (slInput::IsKeyRelease(slInput::KEY_END))
			window->SetBorderless(false);
		
		if (slInput::IsKeyRelease(slInput::KEY_INSERT))
			window->SetNoResize(true);
		if (slInput::IsKeyRelease(slInput::KEY_DELETE))
			window->SetNoResize(false);

		if (slInput::IsKeyRelease(slInput::KEY_PGUP))
			window->SetNoMinimize(true);
		if (slInput::IsKeyRelease(slInput::KEY_PGDOWN))
			window->SetNoMinimize(false);

		if (slInput::IsKeyRelease(slInput::KEY_F1))
			window->ToFullscreenMode();
		if (slInput::IsKeyRelease(slInput::KEY_F2))
			window->ToWindowMode();

		static double x = 0;
		static double y = 0;
		if (slInput::IsKeyRelease(slInput::KEY_A))
		{
			x += 0.1f;
			printf("%f %f : %f\n",x, y, slMath::distance(slVec3(x), slVec3(y)));
		}
		if (slInput::IsKeyRelease(slInput::KEY_Z))
		{
			x -= 0.1f;
			printf("%f %f : %f\n", x, y, slMath::distance(slVec3(x), slVec3(y)));
		}
		if (slInput::IsKeyRelease(slInput::KEY_S))
		{
			y += 0.1f;
			printf("%f %f : %f\n", x, y, slMath::distance(slVec3(x), slVec3(y)));
		}
		if (slInput::IsKeyRelease(slInput::KEY_X))
		{
			y -= 0.1f;
			printf("%f %f : %f\n", x, y, slMath::distance(slVec3(x), slVec3(y)));
		}

		if (slInput::IsKeyRelease(slInput::KEY_Q))
			printf("%f %f : %f\n", x, y, slMath::atan2(-1.0, -1.0));
		if (slInput::IsKeyRelease(slInput::KEY_W))
			printf("%f %f : %f\n", x, y, slMath::atan2(1.0, 1.0));
			
		/*printf("%f\n",slMath::tan(3.141));
		printf("%f %f %f : %f %f %f\n", 
			slMath::sin(1.0),
			slMath::sin(0.11),
			slMath::sin(3.141),
			slMath::sin(-1.0f),
			slMath::sin(0.63f),
			slMath::sin(-3.140)
		);*/

		if (gs)
		{
			gs->BeginDraw();
			gs->ClearAll();
			gs->EndDraw();
			gs->SwapBuffers();
		}
	}

	if (window)
		slDestroy(window);

	slFramework::Stop();
	return EXIT_SUCCESS;
}
