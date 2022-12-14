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
#pragma once
#ifndef _SL_FRMIMPL_H_
#define _SL_FRMIMPL_H_

#include "slowlib.base/GUI/slGUI.h"
#include "slowlib.base/containers/slList.h"

#include <vector>

void slInputUpdatePre(slInputData* id);
void slInputUpdatePost(slInputData* id);

class DefaultGUIDrawTextCallback : public slGUIDrawTextCallback
{
public:
	DefaultGUIDrawTextCallback() {}
	virtual ~DefaultGUIDrawTextCallback() {}

	virtual slGUIFont* OnFont(uint32_t, char32_t) final;
	virtual slColor* OnColor(uint32_t, char32_t) final;
};

class slFrameworkImpl
{
public:
	slFrameworkImpl() {}
	~slFrameworkImpl() {}

	bool m_imageLoaderConverToRGBA8 = true;

	slString m_appPath;
	slStringA m_fileExistString;
	slStringA m_fileSizeString;

	
	std::vector<slGS*> m_gss;
	std::vector<slMeshLoader*> m_meshLoaders;
	std::vector<slImageLoader*> m_imageLoaders;

	bool _compress_fastlz(slCompressionInfo* info);
	bool _decompress_fastlz(slCompressionInfo* info);
	std::vector<slArchiveZipFile*> m_zipFiles;
	void _onDestroy_archive();

	slMat4* m_matrixPtrs[(uint32_t)slMatrixType::_count];

	slFrameworkCallback* m_callback = 0;
	slInputData m_input;
	float m_deltaTime = 0.f;
	void OnDestroy();

	slGUIStyle m_GUIStyleThemeLight;
	slGUIStyle m_GUIStyleThemeDark;
	slList<slGUIWindow*> m_GUIWindows;
	slGUIState m_GUIState;
	void UpdateGUI();

	slArray<slGUIFont*> m_defaultFonts;
	slArray<slTexture*> m_texturesForDestroy;
	DefaultGUIDrawTextCallback m_defaultDrawTextCallback;
	slColor m_colorWhite;

	slGUITextEditor* m_activeTextEditor = 0;
};

#endif
