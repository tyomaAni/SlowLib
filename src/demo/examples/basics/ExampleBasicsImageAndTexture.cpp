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
#include "ExampleBasicsImageAndTexture.h"

ExampleBasicsImageAndTexture::ExampleBasicsImageAndTexture(DemoApp*app):DemoExample(app)
{
}

ExampleBasicsImageAndTexture::~ExampleBasicsImageAndTexture()
{
}


bool ExampleBasicsImageAndTexture::Init()
{
	auto image = slFramework::SummonImage(slFramework::GetPathA("../data/textures/N.png").c_str());
	if (image)
	{
		slTextureInfo ti;
		m_texture = m_gs->SummonTexture(image, ti);

		slDestroy(image);

		if (!m_texture)
		{
			slLog::PrintError("%s: can't create texture\n", SL_FUNCTION);
			return false;
		}
	}

	return true;
}

void ExampleBasicsImageAndTexture::Shutdown()
{
	SLSAFE_DESTROY(m_texture);
}

void ExampleBasicsImageAndTexture::OnDraw()
{
	if (slInput::IsKeyHit(slInput::KEY_ESCAPE))
	{
		m_app->StopExample();
		return;
	}

	m_gs->BeginGUI();
	slFramework::DrawGUI(m_gs);
	m_gs->SetScissorRect(slVec4f(0.f, 0.f,
		m_app->GetWindow()->GetCurrentSize()->x, 
		m_app->GetWindow()->GetCurrentSize()->y), 0);

	if (m_texture)
		m_gs->DrawGUIRectangle(slVec4f(0.f, 0.f, 512.f, 512.f), ColorWhite, ColorWhite, m_texture, 0);

	m_gs->EndGUI();

	m_gs->BeginDraw();
	m_gs->ClearAll();
	
	m_gs->EndDraw();
	m_gs->SwapBuffers();
}

