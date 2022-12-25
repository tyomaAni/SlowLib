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
#ifndef __SL_SLOWLIBBASEGUISTYLE_H__
#define __SL_SLOWLIBBASEGUISTYLE_H__

#include "slowlib.base/common/slColor.h"

enum class slGUIStyleTheme
{
	Light,
	Dark
};

struct slGUIStyle
{
	slColor m_colorWhite = ColorWhite;

	slColor m_windowActiveBGColor1;
	slColor m_windowActiveBGColor2;
	slColor m_windowActiveBorderColor;
	slColor m_windowActiveTitleBGColor1;
	slColor m_windowActiveTitleBGColor2;
	slColor m_windowActiveTitleTextColor;

	slColor m_buttonBGColor1;
	slColor m_buttonBGColor2;
	slColor m_buttonBorderColor;
	slColor m_buttonTextColor;
	slColor m_buttonDisabledBGColor1;
	slColor m_buttonDisabledBGColor2;
	slColor m_buttonDisabledBorderColor;
	slColor m_buttonDisabledTextColor;
	slColor m_buttonMouseHoverBGColor1;
	slColor m_buttonMouseHoverBGColor2;
	slColor m_buttonMouseHoverTextColor;
	slColor m_buttonMousePressBGColor1;
	slColor m_buttonMousePressBGColor2;
	slColor m_buttonMousePressTextColor;

	slColor m_chkradioTextColor;
	slColor m_chkradioDisabledTextColor;
	slColor m_chkradioMouseHoverTextColor;
	slColor m_chkradioMousePressTextColor;
};

#endif
