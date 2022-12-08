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
#include "slowlib.base/scene/slCamera.h"

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

	slMatrix3 m4(m3);
	slMath::invert(m4);

	slMatrix3f p;
	slMath::perspectiveLH(p, 1.f, 800.f / 600.f, 0.1f, 60000.f);

	auto inputData = slInput::GetData();

	double dd = 0.0;

	slGS* gs = slFramework::SummonGS(slFramework::GetGSUID(0));
	if (gs)
	{
		gs->Init(window, 0);
		gs->SetClearColor(0.f, 0.f, 1.f, 1.f);
	}

	slVec3 globalPosition;
	globalPosition.set(99990000.f, 0.f, 0.f);
	
	slCamera* camera = slFramework::SummonCamera();
	camera->m_position = globalPosition + slVec3(10.f, 10.f, 10.f);
	camera->m_target = globalPosition;
	camera->Update();
	slFramework::SetMatrix(slMatrixType::ViewProjection, &camera->m_viewProjectionMatrix);

	float* dt = slFramework::GetDeltaTime();

	while (g_isRun)
	{
		slFramework::Update();
		camera->Update();

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

		if (slInput::IsKeyHold(slInput::KEY_A))
		{
			double d = 13.33 * (double)(*dt);
			camera->m_position.x += d;
			wprintf(L"%f %f %f\n", camera->m_position.x, camera->m_position.y, camera->m_position.z);
		}
		if (slInput::IsKeyHold(slInput::KEY_D))
		{
			camera->m_position.x -= 10.0 * (double)(*dt);
			wprintf(L"%f %f %f\n", camera->m_position.x, camera->m_position.y, camera->m_position.z);
		}
		if (slInput::IsKeyHold(slInput::KEY_W))
		{
			camera->m_position.z += 10.0 * (double)(*dt);
			wprintf(L"%f %f %f\n", camera->m_position.x, camera->m_position.y, camera->m_position.z);
		}
		if (slInput::IsKeyHold(slInput::KEY_S))
		{
			camera->m_position.z -= 10.0 * (double)(*dt);
			wprintf(L"%f %f %f\n", camera->m_position.x, camera->m_position.y, camera->m_position.z);
		}
		if (slInput::IsKeyHold(slInput::KEY_Q))
		{
			camera->m_position.y += 10.0 * (double)(*dt);
			wprintf(L"%f %f %f\n", camera->m_position.x, camera->m_position.y, camera->m_position.z);
		}
		if (slInput::IsKeyHold(slInput::KEY_E))
		{
			camera->m_position.y -= 10.0 * (double)(*dt);
			wprintf(L"%f %f %f\n", camera->m_position.x, camera->m_position.y, camera->m_position.z);
		}


		if (gs)
		{
			gs->BeginDraw();
			gs->ClearAll();

			gs->DrawLine3D(slVec3(globalPosition.x + 1.f, globalPosition.y, globalPosition.z), slVec3(globalPosition.x -1.f, globalPosition.y, globalPosition.z), ColorRed);
			gs->DrawLine3D(slVec3(globalPosition.x, globalPosition.y + 1.f, globalPosition.z), slVec3(globalPosition.x, globalPosition .y -1.f, globalPosition.z), ColorYellow);
			gs->DrawLine3D(slVec3(globalPosition.x, globalPosition.y, globalPosition.z + 1.f), slVec3(globalPosition.x, globalPosition.y, globalPosition.z -1.f), ColorLime);

			gs->EndDraw();
			gs->SwapBuffers();
		}
	}

	if (window)
		slDestroy(window);

	slFramework::Stop();
	return EXIT_SUCCESS;
}
