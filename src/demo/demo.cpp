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

SL_LINK_LIBRARY("slowlib.base");

#include <stdio.h>
#include <Windows.h>

bool g_isRun = true;
struct App
{
	slWindow* m_window = 0;
	slGS* m_gs = 0;
	slCamera* m_camera = 0;
	slGUIWindow* m_GUIWindow = 0;
};

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
	
	virtual void OnSizing(slWindow* w) override 
	{
	}

	virtual void OnSize(slWindow* w) override
	{
		App* app = (App*)w->GetUserData();
		if (app)
		{
			if (w->GetCurrentSize()->x && w->GetCurrentSize()->y)
				app->m_camera->m_aspect = w->GetCurrentSize()->x / w->GetCurrentSize()->y;
			app->m_gs->UpdateMainRenderTarget(slVec3f(w->GetCurrentSize()->x * 0.25f, w->GetCurrentSize()->y * 0.25f, 0.f));
			app->m_gs->SetViewport(0, 0, (uint32_t)w->GetCurrentSize()->x, (uint32_t)w->GetCurrentSize()->y);
			app->m_gs->SetScissorRect(slVec4f(0.f, 0.f, w->GetCurrentSize()->x, w->GetCurrentSize()->y), 0);
			app->m_GUIWindow->SetPositionAndSize(slVec2f(100.f), slVec2f(w->GetCurrentSize()->x - 500.f, w->GetCurrentSize()->y - 300.f));
			app->m_GUIWindow->Rebuild();
		}
	}

	virtual void OnClose(slWindow* w) override 
	{
		w->SetVisible(false);
		g_isRun = false;
	}
};

class MeshLoaderCallback : public slMeshLoaderCallback
{
public:
	MeshLoaderCallback() {}
	virtual ~MeshLoaderCallback() {}

	virtual void OnMaterial(slMaterial* m, slString* name) override 
	{
	}

	virtual void OnMesh(slMesh* newMesh, slString* name, slString* materialName) override 
	{
		if (newMesh)
		{
			slStringA stra;
			if (name)
				name->to_utf8(stra);

			printf("MESH [%s] verts [%i] inds [%i]\n", stra.m_data, newMesh->m_info.m_vCount,
				newMesh->m_info.m_iCount);
			slDestroy(newMesh);
		}
	}
};


class MyModel {
	slGS* m_gs = 0;
	slArray<slGPUMesh*> m_meshBuffers;
	slMaterial m_material;
	slTexture* m_texture = 0;
	slMat4 m_W;
	slMat4 m_WVP;
public:
	MyModel(slGS* gs):m_gs(gs) {}
	~MyModel() 
	{
		if (m_texture)
			slDestroy(m_texture);

		for (size_t i = 0; i < m_meshBuffers.m_size; ++i)
		{
			slDestroy(m_meshBuffers.m_data[i]);
		}
	}

	class cb : public slMeshLoaderCallback
	{
	public:
		cb(){}
		virtual ~cb() {}

		virtual void OnMaterial(slMaterial* m, slString* name) override{}
		virtual void OnMesh(slMesh* newMesh, slString* name, slString* materialName) override{
			if (newMesh)
			{
				slGPUMesh* gpumesh = m_model->m_gs->SummonMesh(newMesh);
				if(gpumesh)
					m_model->m_meshBuffers.push_back(gpumesh);
				slDestroy(newMesh);
			}
		}

		MyModel* m_model = 0;
	}
	m_cb;

	bool Load(const char* p)
	{
		m_cb.m_model = this;
		slFramework::SummonMesh(p, &m_cb);

		slImage* image = slFramework::SummonImage(slFramework::GetPathA("../data/textures/tga.tga").c_str());
		if (image)
		{
			printf("Image %ix%i\n", image->m_info.m_width, image->m_info.m_height);
			slTextureInfo ti;
			m_texture = m_gs->SummonTexture(image, ti);
			m_material.m_maps[0].m_texture = m_texture;
			slDestroy(image);
		}

		return m_meshBuffers.m_size > 0;
	}

	void Draw(slCamera* camera)
	{
		m_WVP = camera->m_projectionMatrix * camera->m_viewMatrix * m_W;
		m_material.m_sunPosition.set(0.f, 1.f, 0.f);

		slFramework::SetMatrix(slMatrixType::World, &m_W);
		slFramework::SetMatrix(slMatrixType::WorldViewProjection, &m_WVP);
		for (size_t i = 0; i < m_meshBuffers.m_size; ++i)
		{			
			m_gs->SetMesh(m_meshBuffers.m_data[i]);
			m_gs->SetMaterial(&m_material);
			m_gs->Draw();
		}
	}

	void SetPosition(const slVec3& p) 
	{
		m_W.m_data[3] = p;
		m_W.m_data[3].w = 1.f;
	}
};

class GUIDrawTextCallback : public slGUIDrawTextCallback
{
	slGUIFont* m_font = 0;
	slColor m_colorWhite = ColorWhite;
	slColor m_colorRed = ColorRed;
public:
	GUIDrawTextCallback(slGUIFont* f):m_font(f) {}
	virtual ~GUIDrawTextCallback() {}
	virtual slGUIFont* OnFont(uint32_t reason, char32_t c) final
	{
		return m_font;
	}
	virtual slColor* OnColor(uint32_t reason, char32_t c) final
	{
		switch (c)
		{
		case U'A':
		case U'E':
		case U'O':
		case U'U':
		case U'Y':
		case U'I':
			return &m_colorRed;
		}

		return &m_colorWhite;
	}
};

class MyButton : public slGUIButton
{
public:
	MyButton(slGUIWindow* w, const slVec2f& position, const slVec2f& size)
		:
		slGUIButton(w, position, size)
	{}
	virtual ~MyButton() {}

	virtual void OnMouseEnter() final
	{
		slLog::PrintInfo("%s\n", SL_FUNCTION);
	}

	virtual void OnMouseLeave() final
	{
		slLog::PrintInfo("%s\n", SL_FUNCTION);
	}

	virtual void OnClickLMB() final
	{
		slLog::PrintInfo("%s\n", SL_FUNCTION);
	}

	virtual void OnClickRMB() final
	{
		slLog::PrintInfo("%s\n", SL_FUNCTION);
	}

	virtual void OnClickMMB() final
	{
		slLog::PrintInfo("%s\n", SL_FUNCTION);
	}

	virtual void OnReleaseLMB() final
	{
		slLog::PrintInfo("%s\n", SL_FUNCTION);
	}
	virtual void OnReleaseRMB() final
	{
		slLog::PrintInfo("%s\n", SL_FUNCTION);
	}
	virtual void OnReleaseMMB() final
	{
		slLog::PrintInfo("%s\n", SL_FUNCTION);
	}
};

int main(int argc, char * argv[])
{
	FrameworkCallback frameworkCallback;
	WindowCallback windowCallback;

	slFramework::Start(&frameworkCallback);
	App app;

	app.m_window = slFramework::SummonWindow(&windowCallback, 800, 600);
	app.m_window->SetVisible(true);

	slArchiveSystem::ZipAdd(slFramework::GetPathA("..\\data\\demo.zip").c_str());

	auto inputData = slInput::GetData();

	// m_gs will be auto destroyed, do not call slDestroy(m_gs);
	app.m_gs = slFramework::SummonGS(slFramework::GetGSUID(0));
	if (app.m_gs)
	{
		app.m_gs->Init(app.m_window, 0);
		app.m_gs->SetClearColor(0.f, 0.f, 1.f, 1.f);
		app.m_window->SetUserData(&app);
	}

	slVec3 globalPosition;
	globalPosition.set(99990000.f, 0.f, 0.f);
	
	app.m_camera = slFramework::SummonCamera();
	app.m_camera->m_position = globalPosition + slVec3(10.f, 10.f, 10.f);
	app.m_camera->m_target = globalPosition;
	app.m_camera->m_aspect = (float)app.m_window->GetCurrentSize()->x / (float)app.m_window->GetCurrentSize()->y;
	app.m_camera->Update();
	slFramework::SetMatrix(slMatrixType::ViewProjection, &app.m_camera->m_viewProjectionMatrix);

	MyModel* mm = slCreate<MyModel>(app.m_gs);
	if (mm->Load(slFramework::GetPathA("../data/models/box.obj").c_str()))
	{
		printf("LOADED!\n");
	}
	mm->SetPosition(globalPosition);

	slFramework::InitDefaultFonts(app.m_gs);
	slGUIFont* myFont = slFramework::GetDefaultFont(0);
	
	auto guiWindow = slFramework::SummonGUIWindow(slVec2f(100.f, 100.f), slVec2f(app.m_window->GetCurrentSize()->x - 500.f, app.m_window->GetCurrentSize()->y - 300.f));
	app.m_GUIWindow = guiWindow;
	guiWindow->SetDrawBG(true);
	//auto btn = slFramework::SummonGUIButton(guiWindow);
	auto btn = slCreate<MyButton>(guiWindow, slVec2f(0.f, 400.f), slVec2f(100.f, 100.f));
	btn->m_alignment = slGUIElement::Alignment::Right;
	btn->SetText(U"Button");
	
	slFramework::RebuildGUI();
	
	GUIDrawTextCallback textDrawCallback(myFont);

	windowCallback.OnSize(app.m_window);

	float* dt = slFramework::GetDeltaTime();
	while (g_isRun)
	{
		slFramework::Update();
		app.m_camera->Update();

		Sleep(1);
		//frameworkCallback.OnMessage();

		if (slInput::IsKeyHit(slInput::KEY_ESCAPE))
			windowCallback.OnClose(app.m_window);

		if (slInput::IsKeyRelease(slInput::KEY_HOME))
			app.m_window->SetBorderless(true);
		if (slInput::IsKeyRelease(slInput::KEY_END))
			app.m_window->SetBorderless(false);
		
		if (slInput::IsKeyRelease(slInput::KEY_INSERT))
			app.m_window->SetNoResize(true);
		if (slInput::IsKeyRelease(slInput::KEY_DELETE))
			app.m_window->SetNoResize(false);

		if (slInput::IsKeyRelease(slInput::KEY_PGUP))
			app.m_window->SetNoMinimize(true);
		if (slInput::IsKeyRelease(slInput::KEY_PGDOWN))
			app.m_window->SetNoMinimize(false);

		if (slInput::IsKeyRelease(slInput::KEY_F1))
			app.m_window->ToFullscreenMode();
		if (slInput::IsKeyRelease(slInput::KEY_F2))
			app.m_window->ToWindowMode();

		if (slInput::IsKeyHold(slInput::KEY_A))
		{
			double d = 13.33 * (double)(*dt);
			app.m_camera->m_position.x += d;
			wprintf(L"%f %f %f\n", app.m_camera->m_position.x, app.m_camera->m_position.y, app.m_camera->m_position.z);
		}
		if (slInput::IsKeyHold(slInput::KEY_D))
		{
			app.m_camera->m_position.x -= 10.0 * (double)(*dt);
			wprintf(L"%f %f %f\n", app.m_camera->m_position.x, app.m_camera->m_position.y, app.m_camera->m_position.z);
		}
		if (slInput::IsKeyHold(slInput::KEY_W))
		{
			app.m_camera->m_position.z += 10.0 * (double)(*dt);
		}
		if (slInput::IsKeyHold(slInput::KEY_S))
		{
			app.m_camera->m_position.z -= 10.0 * (double)(*dt);
		}
		if (slInput::IsKeyHold(slInput::KEY_Q))
		{
			app.m_camera->m_position.y += 10.0 * (double)(*dt);
		}
		if (slInput::IsKeyHold(slInput::KEY_E))
		{
			app.m_camera->m_position.y -= 10.0 * (double)(*dt);
		}
		if (slInput::IsKeyHit(slInput::KEY_END))
		{
			printf("try to load\n");
			MeshLoaderCallback mlcb;
			/*slString path = slFramework::GetAppPath() + "4_objs.obj";
			slStringA stra;
			path.to_utf8(stra);
			slFramework::SummonMesh(stra.data(), &mlcb);*/
			slFramework::SummonMesh(slFramework::GetPathA("..\\data\\4_objs.obj").c_str(), &mlcb);
		}


		if (app.m_gs)
		{
			app.m_gs->BeginGUI();
			slFramework::DrawGUI(app.m_gs);
			app.m_gs->SetScissorRect(slVec4f(0.f,0.f, 
				app.m_window->GetCurrentSize()->x, app.m_window->GetCurrentSize()->y), 0);
			app.m_gs->DrawGUIRectangle(slRect(0, 0, 100, 30), ColorRed, ColorBlue, 0, 0);
			app.m_gs->DrawGUICharacter(U'A', myFont, slVec2f(10.f, 10.f), ColorWhite);
			app.m_gs->DrawGUICharacter(U'B', myFont, slVec2f(20.f, 10.f), ColorLime);
			app.m_gs->DrawGUICharacter(U'C', myFont, slVec2f(30.f, 10.f), ColorCornsilk);
			app.m_gs->DrawGUIText(U"Hello world!\nNew LINE\nABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_)(abcdefghijklmnop<>", 79, slVec2f(0.f, 20.f), &textDrawCallback);
			app.m_gs->EndGUI();	

			app.m_gs->BeginDraw();
			app.m_gs->ClearAll();

			app.m_gs->DrawLine3D(slVec3(globalPosition.x + 1.f, globalPosition.y, globalPosition.z), slVec3(globalPosition.x -1.f, globalPosition.y, globalPosition.z), ColorRed);
			app.m_gs->DrawLine3D(slVec3(globalPosition.x, globalPosition.y + 1.f, globalPosition.z), slVec3(globalPosition.x, globalPosition .y -1.f, globalPosition.z), ColorYellow);
			app.m_gs->DrawLine3D(slVec3(globalPosition.x, globalPosition.y, globalPosition.z + 1.f), slVec3(globalPosition.x, globalPosition.y, globalPosition.z -1.f), ColorLime);

			mm->Draw(app.m_camera);

			app.m_gs->EndDraw();
			app.m_gs->SwapBuffers();
		}
	}
	
	SLSAFE_DESTROY(mm);

	SLSAFE_DESTROY(app.m_window);

	slFramework::Stop();
	return EXIT_SUCCESS;
}
