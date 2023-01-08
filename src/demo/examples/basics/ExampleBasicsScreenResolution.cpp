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

#include "../../DemoApp.h"
#include "ExampleBasicsScreenResolution.h"

class ExampleBasicsScreenResolution_sliderSize : public slGUISlider
{
public:
	ExampleBasicsScreenResolution_sliderSize(slGUIWindow* w, const slVec2f& position, const slVec2f& size)
		:
		slGUISlider(w, position, size)
	{

	}
	virtual ~ExampleBasicsScreenResolution_sliderSize() {}

	virtual void OnChangeValue() override 
	{
		ExampleBasicsScreenResolution* example = (ExampleBasicsScreenResolution*)GetUserData();
		example->OnChangeScreenResolution((float)m_valueFloat);
	}
};

ExampleBasicsScreenResolution::ExampleBasicsScreenResolution(DemoApp*app):DemoExample(app)
{
}

ExampleBasicsScreenResolution::~ExampleBasicsScreenResolution()
{
}


bool ExampleBasicsScreenResolution::Init()
{
	m_camera = slFramework::SummonCamera();
	m_camera->m_position = slVec3(10.f, 10.f, 10.f);
	m_camera->m_aspect = (float)m_app->GetWindow()->GetCurrentSize()->x / (float)m_app->GetWindow()->GetCurrentSize()->y;
	m_camera->Update();

	// Set View Projection matrix for 3D line
	slFramework::SetMatrix(slMatrixType::ViewProjection, &m_camera->m_viewProjectionMatrix);

	m_GUIWindow = slFramework::SummonGUIWindow(slVec2f(0.f, 0.f), slVec2f(220.f, 40.f));
	m_GUIWindow->SetDrawBG(false);

	ExampleBasicsScreenResolution_sliderSize* slider = slCreate<ExampleBasicsScreenResolution_sliderSize>(m_GUIWindow,
		slVec2f(0.f, 15.f), slVec2f(180.f, 20.f));
	m_sliderSize = slider;
	m_sliderSize->SetMinMaxFloat(0.125, 1.0);
	*m_sliderSize->GetFloat() = 1.0;
	m_sliderSize->SetUserData(this);

	m_GUIWindow->Rebuild();

	return true;
}

void ExampleBasicsScreenResolution::Shutdown()
{
	if (m_GUIWindow)
	{
		// I can't call SLSAFE_DESTROY(m_GUIWindow); because I need to remove
		//  window from framework.
		slFramework::DestroyGUIWindow(m_GUIWindow);
		m_GUIWindow = 0;
	}
	SLSAFE_DESTROY(m_camera);
}

void ExampleBasicsScreenResolution::OnDraw()
{
	if (slInput::IsKeyHit(slInput::KEY_ESCAPE))
	{
		m_app->StopExample();
		return;
	}

	m_camera->Update();

	if (slInput::IsKeyHold(slInput::KEY_A))
		m_camera->m_position.x += 10.0 * (double)(*m_app->m_dt);
	if (slInput::IsKeyHold(slInput::KEY_D))
		m_camera->m_position.x -= 10.0 * (double)(*m_app->m_dt);
	if (slInput::IsKeyHold(slInput::KEY_W))
		m_camera->m_position.z += 10.0 * (double)(*m_app->m_dt);
	if (slInput::IsKeyHold(slInput::KEY_S))
		m_camera->m_position.z -= 10.0 * (double)(*m_app->m_dt);
	if (slInput::IsKeyHold(slInput::KEY_Q))
		m_camera->m_position.y += 10.0 * (double)(*m_app->m_dt);
	if (slInput::IsKeyHold(slInput::KEY_E))
		m_camera->m_position.y -= 10.0 * (double)(*m_app->m_dt);

	m_gs->BeginGUI();
	slFramework::DrawGUI(m_gs);
	m_gs->SetScissorRect(slVec4f(0.f, 0.f,
		m_app->GetWindow()->GetCurrentSize()->x, 
		m_app->GetWindow()->GetCurrentSize()->y), 0);

	m_app->GetTextDrawCallback()->m_curColor = &m_app->GetTextDrawCallback()->m_colorWhite;
	m_gs->DrawGUIText(U"Size multipler", 14, slVec2f(), m_app->GetTextDrawCallback());

	m_gs->EndGUI();

	m_gs->BeginDraw();
	m_gs->ClearAll();
	
	m_gs->SetShader(slShaderType::Line3D, 0);
	m_gs->DrawLine3D(slVec3(-1.f, 0.f, 0.f), slVec3(1.f, 0.f, 0.f), ColorRed);
	m_gs->DrawLine3D(slVec3(0.f, -1.f, 0.f), slVec3(0.f, 1.f, 0.f), ColorYellow);
	m_gs->DrawLine3D(slVec3(0.f, 0.f, -1.f), slVec3(0.f, 0.f, 1.f), ColorLime);

	m_gs->EndDraw();
	m_gs->SwapBuffers();
}

void ExampleBasicsScreenResolution::OnChangeScreenResolution(float m)
{
	slVec3f sz;

	sz.x = m_app->GetWindow()->GetCurrentSize()->x * m;
	sz.y = m_app->GetWindow()->GetCurrentSize()->y * m;

	m_gs->UpdateMainRenderTarget(sz);
}
