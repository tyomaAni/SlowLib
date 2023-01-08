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
#include "ExampleBasicsMouseAndKeyboard.h"

ExampleBasicsMouseAndKeyboard::ExampleBasicsMouseAndKeyboard(DemoApp*app):DemoExample(app)
{
}

ExampleBasicsMouseAndKeyboard::~ExampleBasicsMouseAndKeyboard()
{
}


bool ExampleBasicsMouseAndKeyboard::Init()
{
	return true;
}

void ExampleBasicsMouseAndKeyboard::Shutdown()
{
}

void ExampleBasicsMouseAndKeyboard::OnDraw()
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

	slVec2f textPosition;
	float lineHeight = 14.f;

	m_app->GetTextDrawCallback()->m_curColor = &m_app->GetTextDrawCallback()->m_colorWhite;

	auto inputData = slInput::GetData();

	m_text = U"Mouse position: ";
	m_text += inputData->mousePosition.x;
	m_text += U" : ";
	m_text += inputData->mousePosition.y;
	m_gs->DrawGUIText(m_text.c_str(), m_text.size(), textPosition, m_app->GetTextDrawCallback());
	textPosition.y += lineHeight;

	m_text = U"Mouse move delta: ";
	m_text += inputData->mouseMoveDelta.x;
	m_text += U" : ";
	m_text += inputData->mouseMoveDelta.y;
	m_gs->DrawGUIText(m_text.c_str(), m_text.size(), textPosition, m_app->GetTextDrawCallback());
	textPosition.y += lineHeight;

	m_text = U"Mouse wheel delta: ";
	m_text += inputData->mouseWheelDelta;
	m_gs->DrawGUIText(m_text.c_str(), m_text.size(), textPosition, m_app->GetTextDrawCallback());
	textPosition.y += lineHeight;

	m_text = U"Mouse flags: ";
	m_text.append_hex(inputData->mouseButtonFlags);
	m_text += U" Last was: ";
	if(m_lastMouseFlags & inputData->MBFL_LMBDOWN)
		m_text += U"MBFL_LMBDOWN ";
	if (m_lastMouseFlags & inputData->MBFL_RMBDOWN)
		m_text += U"MBFL_RMBDOWN ";
	if (m_lastMouseFlags & inputData->MBFL_MMBDOWN)
		m_text += U"MBFL_MMBDOWN ";
	if (m_lastMouseFlags & inputData->MBFL_X1MBDOWN)
		m_text += U"MBFL_X1MBDOWN ";
	if (m_lastMouseFlags & inputData->MBFL_X2MBDOWN)
		m_text += U"MBFL_X2MBDOWN ";
	if (m_lastMouseFlags & inputData->MBFL_LMBUP)
		m_text += U"MBFL_LMBUP ";
	if (m_lastMouseFlags & inputData->MBFL_RMBUP)
		m_text += U"MBFL_RMBUP ";
	if (m_lastMouseFlags & inputData->MBFL_MMBUP)
		m_text += U"MBFL_MMBUP ";
	if (m_lastMouseFlags & inputData->MBFL_X1MBUP)
		m_text += U"MBFL_X1MBUP ";
	if (m_lastMouseFlags & inputData->MBFL_X2MBUP)
		m_text += U"MBFL_X2MBUP ";
	if (m_lastMouseFlags & inputData->MBFL_LMBHOLD)
		m_text += U"MBFL_LMBHOLD ";
	if (m_lastMouseFlags & inputData->MBFL_RMBHOLD)
		m_text += U"MBFL_RMBHOLD ";
	if (m_lastMouseFlags & inputData->MBFL_MMBHOLD)
		m_text += U"MBFL_MMBHOLD ";
	if (m_lastMouseFlags & inputData->MBFL_X1MBHOLD)
		m_text += U"MBFL_X1MBHOLD ";
	if (m_lastMouseFlags & inputData->MBFL_X2MBHOLD)
		m_text += U"MBFL_X2MBHOLD ";
	m_gs->DrawGUIText(m_text.c_str(), m_text.size(), textPosition, m_app->GetTextDrawCallback());
	textPosition.y += lineHeight;
	
	m_text = U"Keyboard character: ";
	m_text.push_back(m_lastCharacter);
	m_gs->DrawGUIText(m_text.c_str(), m_text.size(), textPosition, m_app->GetTextDrawCallback());
	textPosition.y += lineHeight;
	if (inputData->character)
		m_lastCharacter = inputData->character;

	m_text = U"Keyboard modifier: ";
	if (inputData->keyboardModifier == inputData->KBMOD_ALT)
		m_text += U"Alt";
	if (inputData->keyboardModifier == inputData->KBMOD_CTRL)
		m_text += U"Ctrl";
	if (inputData->keyboardModifier == inputData->KBMOD_CTRLALT)
		m_text += U"Ctrl+Alt";
	if (inputData->keyboardModifier == inputData->KBMOD_CTRLSHIFT)
		m_text += U"Ctrl+Shift";
	if (inputData->keyboardModifier == inputData->KBMOD_CTRLSHIFTALT)
		m_text += U"Ctrl+Shift+Alt";
	if (inputData->keyboardModifier == inputData->KBMOD_SHIFT)
		m_text += U"Shift";
	if (inputData->keyboardModifier == inputData->KBMOD_SHIFTALT)
		m_text += U"Shift+Alt";
	m_gs->DrawGUIText(m_text.c_str(), m_text.size(), textPosition, m_app->GetTextDrawCallback());
	textPosition.y += lineHeight;

	if (inputData->mouseButtonFlags)
		m_lastMouseFlags = inputData->mouseButtonFlags;

	if(slInput::IsKeyHold(slInput::KEY_UP))
	{
		m_text = U"UP";
		m_text.push_back(m_lastCharacter);
		m_gs->DrawGUIText(m_text.c_str(), m_text.size(), textPosition, m_app->GetTextDrawCallback());
		textPosition.y += lineHeight;
	}
	if (slInput::IsKeyHold(slInput::KEY_DOWN))
	{
		m_text = U"DOWN";
		m_text.push_back(m_lastCharacter);
		m_gs->DrawGUIText(m_text.c_str(), m_text.size(), textPosition, m_app->GetTextDrawCallback());
		textPosition.y += lineHeight;
	}

	m_gs->EndGUI();

	m_gs->BeginDraw();
	m_gs->ClearAll();
	
	m_gs->EndDraw();
	m_gs->SwapBuffers();
}

