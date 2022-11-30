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

#include "../framework/slFrameworkImpl.h"

extern slFrameworkImpl* g_framework;




// masks
enum : uint64_t {
	KBIT_NONE = 0,
	KBIT_BACKSPACE = 0x1,
	KBIT_TAB = 0x2,
	KBIT_ENTER = 0x4,
	KBIT_SHIFT = 0x8,
	KBIT_CTRL = 0x10,
	KBIT_ALT = 0x20,
	KBIT_PAUSE = 0x40,
	KBIT_CAPS_LOCK = 0x80,
	KBIT_ESCAPE = 0x100,
	KBIT_SPACE = 0x200,
	KBIT_PGUP = 0x400,
	KBIT_PGDOWN = 0x800,
	KBIT_END = 0x1000,
	KBIT_HOME = 0x2000,
	KBIT_LEFT = 0x4000,
	KBIT_UP = 0x8000,
	KBIT_RIGHT = 0x10000,
	KBIT_DOWN = 0x20000,
	KBIT_PRTSCR = 0x40000,
	KBIT_INSERT = 0x80000,
	KBIT_DELETE = 0x100000,
	KBIT_0 = 0x200000,
	KBIT_1 = 0x400000,
	KBIT_2 = 0x800000,
	KBIT_3 = 0x1000000,
	KBIT_4 = 0x2000000,
	KBIT_5 = 0x4000000,
	KBIT_6 = 0x8000000,
	KBIT_7 = 0x10000000,
	KBIT_8 = 0x20000000,
	KBIT_9 = 0x40000000,
	KBIT_A = 0x80000000,
	KBIT_B = 0x100000000,
	KBIT_C = 0x200000000,
	KBIT_D = 0x400000000,
	KBIT_E = 0x800000000,
	KBIT_F = 0x1000000000,
	KBIT_G = 0x2000000000,
	KBIT_H = 0x4000000000,
	KBIT_I = 0x8000000000,
	KBIT_J = 0x10000000000,
	KBIT_K = 0x20000000000,
	KBIT_L = 0x40000000000,
	KBIT_M = 0x80000000000,
	KBIT_N = 0x100000000000,
	KBIT_O = 0x200000000000,
	KBIT_P = 0x400000000000,
	KBIT_Q = 0x800000000000,
	KBIT_R = 0x1000000000000,
	KBIT_S = 0x2000000000000,
	KBIT_T = 0x4000000000000,
	KBIT_U = 0x8000000000000,
	KBIT_V = 0x10000000000000,
	KBIT_W = 0x20000000000000,
	KBIT_X = 0x40000000000000,
	KBIT_Y = 0x80000000000000,
	KBIT_Z = 0x100000000000000,
	KBIT_WIN = 0x200000000000000,
	KBIT_CONTEXT = 0x400000000000000,
	KBIT_NUM_0 = 0x800000000000000,
	KBIT_NUM_1 = 0x1,
	KBIT_NUM_2 = 0x2,
	KBIT_NUM_3 = 0x4,
	KBIT_NUM_4 = 0x8,
	KBIT_NUM_5 = 0x10,
	KBIT_NUM_6 = 0x20,
	KBIT_NUM_7 = 0x40,
	KBIT_NUM_8 = 0x80,
	KBIT_NUM_9 = 0x100,
	KBIT_NUM_MUL = 0x200, // *
	KBIT_NUM_ADD = 0x400, // +
	KBIT_NUM_SUB = 0x800, // -
	KBIT_NUM_DOT = 0x1000, // .
	KBIT_NUM_SLASH = 0x2000, // /
	KBIT_F1 = 0x4000,
	KBIT_F2 = 0x8000,
	KBIT_F3 = 0x10000,
	KBIT_F4 = 0x20000,
	KBIT_F5 = 0x40000,
	KBIT_F6 = 0x80000,
	KBIT_F7 = 0x100000,
	KBIT_F8 = 0x200000,
	KBIT_F9 = 0x400000,
	KBIT_F10 = 0x800000,
	KBIT_F11 = 0x1000000,
	KBIT_F12 = 0x2000000,
	KBIT_NUM_LOCK = 0x4000000,
	KBIT_SCROLL_LOCK = 0x8000000,
	KBIT_LSHIFT = 0x10000000,
	KBIT_RSHIFT = 0x20000000,
	KBIT_LCTRL = 0x40000000,
	KBIT_RCTRL = 0x80000000,
	KBIT_LALT = 0x100000000,
	KBIT_RALT = 0x200000000,
	KBIT_FUNC_MY_COMP = 0x400000000, // Function key. open My computer
	KBIT_FUNC_CALC = 0x800000000, // Function key. open Calculator
	KBIT_COLON = 0x1000000000, //;:
	KBIT_ADD = 0x2000000000, //=+
	KBIT_COMMA = 0x4000000000, //,<
	KBIT_SUB = 0x8000000000, //-_
	KBIT_DOT = 0x10000000000, //.>
	KBIT_SLASH = 0x20000000000, // /?
	KBIT_TILDE = 0x40000000000, //`~
	KBIT_FIGURE_OPEN = 0x80000000000, //[{
	KBIT_BACKSLASH = 0x100000000000, //\|
	KBIT_FIGURE_CLOSE = 0x200000000000, //]}
	KBIT_QUOTE = 0x400000000000, //'"
	KBIT_end__,
};

enum {
	MBFL_LMBDOWN = 0x1,
	MBFL_LMBUP = 0x2,
	MBFL_RMBDOWN = 0x4,
	MBFL_RMBUP = 0x8,
	MBFL_MMBDOWN = 0x10,
	MBFL_MMBUP = 0x20,
	MBFL_X1MBDOWN = 0x40,
	MBFL_X1MBUP = 0x80,
	MBFL_X2MBDOWN = 0x100,
	MBFL_X2MBUP = 0x200,

	MBFL_LMBHOLD = 0x400,
	MBFL_RMBHOLD = 0x800,
	MBFL_MMBHOLD = 0x1000,
	MBFL_X1MBHOLD = 0x2000,
	MBFL_X2MBHOLD = 0x4000,
};

enum {
	KEYFL_HOLD = 0x1,
	KEYFL_HIT = 0x2,
	KEYFL_RELEASE = 0x4,
};

enum {
	KBMOD_clear = 0x0,
	KBMOD_CTRL = 0x1,
	KBMOD_SHIFT = 0x2,
	KBMOD_ALT = 0x4,
	KBMOD_CTRLSHIFT = 0x8,
	KBMOD_CTRLALT = 0x10,
	KBMOD_SHIFTALT = 0x20,
	KBMOD_CTRLSHIFTALT = 0x40
};

static const uint64_t g_keyToBin[] =
{
	0, //KEY_NONE = 0,
	0, 0, 0, 0, 0, 0, 0,
	KBIT_BACKSPACE,	//KEY_BACKSPACE = 8,
	KBIT_TAB,	//KEY_TAB = 9,
	0, 0, 0,
	KBIT_ENTER, //KEY_ENTER = 13,
	0, 0,
	KBIT_SHIFT, //KEY_SHIFT = 16,
	KBIT_CTRL, //KEY_CTRL = 17,
	KBIT_ALT, //KEY_ALT = 18,
	KBIT_PAUSE,//KEY_PAUSE = 19,
	KBIT_CAPS_LOCK,//KEY_CAPS_LOCK = 20,
	0, 0, 0, 0, 0, 0,
	KBIT_ESCAPE, //KEY_ESCAPE = 27,
	0, 0, 0, 0,
	KBIT_SPACE, //KEY_SPACE = 32,
	KBIT_PGUP, //KEY_PGUP = 33,
	KBIT_PGDOWN, //KEY_PGDOWN = 34,
	KBIT_END, //KEY_END = 35,
	KBIT_HOME, //KEY_HOME = 36,
	KBIT_LEFT, //KEY_LEFT = 37,
	KBIT_UP, //KEY_UP = 38,
	KBIT_RIGHT, //KEY_RIGHT = 39,
	KBIT_DOWN, //KEY_DOWN = 40,
	0, 0, 0,
	KBIT_PRTSCR, //KEY_PRTSCR = 44,
	KBIT_INSERT, //KEY_INSERT = 45,
	KBIT_DELETE, //KEY_DELETE = 46,
	0,
	KBIT_0, //KEY_0 = 48,
	KBIT_1, //KEY_1 = 49,
	KBIT_2, //KEY_2 = 50,
	KBIT_3, //KEY_3 = 51,
	KBIT_4, //KEY_4 = 52,
	KBIT_5, //KEY_5 = 53,
	KBIT_6, //KEY_6 = 54,
	KBIT_7, //KEY_7 = 55,
	KBIT_8, //KEY_8 = 56,
	KBIT_9, //KEY_9 = 57,
	0, 0, 0, 0, 0, 0, 0,
	KBIT_A, //KEY_A = 65,
	KBIT_B, //KEY_B = 66,
	KBIT_C, //KEY_C = 67,
	KBIT_D, //KEY_D = 68,
	KBIT_E, //KEY_E = 69,
	KBIT_F, //KEY_F = 70,
	KBIT_G, //KEY_G = 71,
	KBIT_H, //KEY_H = 72,
	KBIT_I, //KEY_I = 73,
	KBIT_J, //KEY_J = 74,
	KBIT_K, //KEY_K = 75,
	KBIT_L, //KEY_L = 76,
	KBIT_M, //KEY_M = 77,
	KBIT_N, //KEY_N = 78,
	KBIT_O, //KEY_O = 79,
	KBIT_P, //KEY_P = 80,
	KBIT_Q, //KEY_Q = 81,
	KBIT_R, //KEY_R = 82,
	KBIT_S, //KEY_S = 83,
	KBIT_T, //KEY_T = 84,
	KBIT_U, //KEY_U = 85,
	KBIT_V, //KEY_V = 86,
	KBIT_W, //KEY_W = 87,
	KBIT_X, //KEY_X = 88,
	KBIT_Y, //KEY_Y = 89,
	KBIT_Z, //KEY_Z = 90,
	KBIT_WIN, //KEY_WIN = 91,
	0,
	KBIT_CONTEXT, //KEY_CONTEXT = 93,
	0, 0,
	KBIT_NUM_0, //KEY_NUM_0 = 96,
	KBIT_NUM_1, //KEY_NUM_1 = 97,
	KBIT_NUM_2, //KEY_NUM_2 = 98,
	KBIT_NUM_3, //KEY_NUM_3 = 99,
	KBIT_NUM_4, //KEY_NUM_4 = 100,
	KBIT_NUM_5, //KEY_NUM_5 = 101,
	KBIT_NUM_6, //KEY_NUM_6 = 102,
	KBIT_NUM_7, //KEY_NUM_7 = 103,
	KBIT_NUM_8, //KEY_NUM_8 = 104,
	KBIT_NUM_9, //KEY_NUM_9 = 105,
	KBIT_NUM_MUL,//	KEY_NUM_MUL = 106, // *
	KBIT_NUM_ADD, //KEY_NUM_ADD = 107, // +
	0,
	KBIT_NUM_SUB, //KEY_NUM_SUB = 109, // -
	KBIT_NUM_DOT, //KEY_NUM_DOT = 110, // .
	KBIT_NUM_SLASH, //KEY_NUM_SLASH = 111, // /
	KBIT_F1, //KEY_F1 = 112,
	KBIT_F2, //KEY_F2 = 113,
	KBIT_F3, //	KEY_F3 = 114,
	KBIT_F4, //KEY_F4 = 115,
	KBIT_F5, //KEY_F5 = 116,
	KBIT_F6, //KEY_F6 = 117,
	KBIT_F7, //KEY_F7 = 118,
	KBIT_F8, //KEY_F8 = 119,
	KBIT_F9, //KEY_F9 = 120,
	KBIT_F10, //KEY_F10 = 121,
	KBIT_F11, //KEY_F11 = 122,
	KBIT_F12, //KEY_F12 = 123,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		KBIT_NUM_LOCK,//	KEY_NUM_LOCK = 144,
		KBIT_SCROLL_LOCK, //KEY_SCROLL_LOCK = 145,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		KBIT_LSHIFT, //KEY_LSHIFT = 160,
		KBIT_RSHIFT, //KEY_RSHIFT = 161,
		KBIT_LCTRL, //KEY_LCTRL = 162,
		KBIT_RCTRL, //KEY_RCTRL = 163,
		KBIT_LALT, //KEY_LALT = 164,
		KBIT_RALT, //KEY_RALT = 165,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		KBIT_FUNC_MY_COMP, //KEY_FUNC_MY_COMP = 182, // Function key. open My computer
		KBIT_FUNC_CALC, //KEY_FUNC_CALC = 183, // Function key. open Calculator
		0, 0,
		KBIT_COLON, //KEY_COLON = 186, //;:
		KBIT_ADD, //KEY_ADD = 187, //=+
		KBIT_COMMA, //KEY_COMMA = 188, //,<
		KBIT_SUB, //KEY_SUB = 189, //-_
		KBIT_DOT, //KEY_DOT = 190, //.>
		KBIT_SLASH, //KEY_SLASH = 191, // /?
		KBIT_TILDE, //KEY_TILDE = 192, //`~
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		KBIT_FIGURE_OPEN, //KEY_FIGURE_OPEN = 219, //[{
		KBIT_BACKSLASH, //KEY_BACKSLASH = 220, //\|
		KBIT_FIGURE_CLOSE, //KEY_FIGURE_CLOSE = 221, //]}
		KBIT_QUOTE, //KEY_QUOTE = 222,
};

void slInputUpdatePre(slInputData* id)
{
	id->mousePositionOld = id->mousePosition;
	id->mouseButtonFlags1 = 0;
	id->mouseWheelDelta = 0;
	id->character = 0;
	slInput::DisableLMBDown();
	slInput::DisableRMBDown();
	slInput::DisableMMBDown();
	slInput::DisableX1MBDown();
	slInput::DisableX2MBDown();
	slInput::DisableLMBUp();
	slInput::DisableRMBUp();
	slInput::DisableMMBUp();
	slInput::DisableX1MBUp();
	slInput::DisableX2MBUp();
	id->keyFlagsHit[0] = 0;
	id->keyFlagsHit[1] = 0;
	id->keyFlagsRelease[0] = 0;
	id->keyFlagsRelease[1] = 0;
}

void slInputUpdatePost(slInputData* id)
{
	unsigned int ctrl_shift_alt = 0;
	if (slInput::IsKeyHold(slInput::KEY_ALT) || slInput::IsKeyHold(slInput::KEY_RALT))
		ctrl_shift_alt |= 1;

	if (slInput::IsKeyHold(slInput::KEY_LSHIFT) || slInput::IsKeyHold(slInput::KEY_RSHIFT))
		ctrl_shift_alt |= 2;

	if (slInput::IsKeyHold(slInput::KEY_LCTRL) || slInput::IsKeyHold(slInput::KEY_RCTRL))
		ctrl_shift_alt |= 4;

	switch (ctrl_shift_alt)
	{
	default:
	case 0:  id->keyboardModifier = KBMOD_clear;         break;
	case 1:  id->keyboardModifier = KBMOD_ALT;           break;
	case 2:  id->keyboardModifier = KBMOD_SHIFT;         break;
	case 3:  id->keyboardModifier = KBMOD_SHIFTALT;      break;
	case 4:  id->keyboardModifier = KBMOD_CTRL;          break;
	case 5:  id->keyboardModifier = KBMOD_CTRLALT;       break;
	case 6:  id->keyboardModifier = KBMOD_CTRLSHIFT;     break;
	case 7:  id->keyboardModifier = KBMOD_CTRLSHIFTALT;  break;
	}

	id->mouseMoveDelta.x = id->mousePosition.x - id->mousePositionOld.x;
	id->mouseMoveDelta.y = id->mousePosition.y - id->mousePositionOld.y;
}

slInputData* slInput::GetData()
{
	SL_ASSERT_ST(g_framework);
	return &g_framework->m_input;
}

void slInput::SetKeyHit(uint32_t k, bool v)
{
	SL_ASSERT_ST(g_framework);

	uint64_t bit = g_keyToBin[k];

	if (k < slInput::KEY_NUM_1)
	{
		if (v)
			g_framework->m_input.keyFlagsHit[0] |= bit;
		else
			g_framework->m_input.keyFlagsHit[0] &= ~bit;
	}
	else
	{
		if (v)
			g_framework->m_input.keyFlagsHit[1] |= bit;
		else
			g_framework->m_input.keyFlagsHit[1] &= ~bit;
	}
}

void slInput::SetKeyHold(uint32_t k, bool v)
{
	SL_ASSERT_ST(g_framework);

	uint64_t bit = g_keyToBin[k];

	if (k < slInput::KEY_NUM_1)
	{
		if (v)
			g_framework->m_input.keyFlagsHold[0] |= bit;
		else
			g_framework->m_input.keyFlagsHold[0] &= ~bit;
	}
	else
	{
		if (v)
			g_framework->m_input.keyFlagsHold[1] |= bit;
		else
			g_framework->m_input.keyFlagsHold[1] &= ~bit;
	}
}

void slInput::SetKeyRelease(uint32_t k, bool v)
{
	SL_ASSERT_ST(g_framework);

	uint64_t bit = g_keyToBin[k];

	if (k < slInput::KEY_NUM_1)
	{
		if (v)
			g_framework->m_input.keyFlagsRelease[0] |= bit;
		else
			g_framework->m_input.keyFlagsRelease[0] &= ~bit;
	}
	else
	{
		if (v)
			g_framework->m_input.keyFlagsRelease[1] |= bit;
		else
			g_framework->m_input.keyFlagsRelease[1] &= ~bit;
	}
}

bool slInput::IsKeyHit(uint32_t i)
{
	SL_ASSERT_ST(g_framework);

	uint64_t bit = g_keyToBin[i];

	if (i < slInput::KEY_NUM_1) // первый uint64_t
		return (g_framework->m_input.keyFlagsHit[0] & bit);
	else
		return (g_framework->m_input.keyFlagsHit[1] & bit);
}

bool slInput::IsKeyHold(uint32_t i)
{
	SL_ASSERT_ST(g_framework);

	uint64_t bit = g_keyToBin[i];

	if (i < slInput::KEY_NUM_1) // первый uint64_t
		return (g_framework->m_input.keyFlagsHold[0] & bit);
	else
		return (g_framework->m_input.keyFlagsHold[1] & bit);
}

bool slInput::IsKeyRelease(uint32_t i)
{
	SL_ASSERT_ST(g_framework);

	uint64_t bit = g_keyToBin[i];

	if (i < slInput::KEY_NUM_1) // первый uint64_t
		return (g_framework->m_input.keyFlagsRelease[0] & bit);
	else
		return (g_framework->m_input.keyFlagsRelease[1] & bit);
}

void slInput::EnableLMBDown() { g_framework->m_input.mouseButtonFlags1 |= MBFL_LMBDOWN; }
void slInput::EnableLMBHold() { g_framework->m_input.mouseButtonFlags2 |= MBFL_LMBHOLD; }
void slInput::EnableLMBUp() { g_framework->m_input.mouseButtonFlags1 |= MBFL_LMBUP; }
void slInput::EnableRMBDown() { g_framework->m_input.mouseButtonFlags1 |= MBFL_RMBDOWN; }
void slInput::EnableRMBHold() { g_framework->m_input.mouseButtonFlags2 |= MBFL_RMBHOLD; }
void slInput::EnableRMBUp() { g_framework->m_input.mouseButtonFlags1 |= MBFL_RMBUP; }
void slInput::EnableMMBDown() { g_framework->m_input.mouseButtonFlags1 |= MBFL_MMBDOWN; }
void slInput::EnableMMBHold() { g_framework->m_input.mouseButtonFlags2 |= MBFL_MMBHOLD; }
void slInput::EnableMMBUp() { g_framework->m_input.mouseButtonFlags1 |= MBFL_MMBUP; }
void slInput::EnableX1MBDown() { g_framework->m_input.mouseButtonFlags1 |= MBFL_X1MBDOWN; }
void slInput::EnableX1MBHold() { g_framework->m_input.mouseButtonFlags2 |= MBFL_X1MBHOLD; }
void slInput::EnableX1MBUp() { g_framework->m_input.mouseButtonFlags1 |= MBFL_X1MBUP; }
void slInput::EnableX2MBDown() { g_framework->m_input.mouseButtonFlags1 |= MBFL_X2MBDOWN; }
void slInput::EnableX2MBHold() { g_framework->m_input.mouseButtonFlags2 |= MBFL_X2MBHOLD; }
void slInput::EnableX2MBUp() { g_framework->m_input.mouseButtonFlags1 |= MBFL_X2MBUP; }

void slInput::DisableLMBDown() { g_framework->m_input.mouseButtonFlags1 &= ~MBFL_LMBDOWN; }
void slInput::DisableLMBHold() { g_framework->m_input.mouseButtonFlags2 &= ~MBFL_LMBHOLD; }
void slInput::DisableLMBUp()   { g_framework->m_input.mouseButtonFlags1 &= ~MBFL_LMBUP; }
void slInput::DisableRMBDown() { g_framework->m_input.mouseButtonFlags1 &= ~MBFL_RMBDOWN; }
void slInput::DisableRMBHold() { g_framework->m_input.mouseButtonFlags2 &= ~MBFL_RMBHOLD; }
void slInput::DisableRMBUp()   { g_framework->m_input.mouseButtonFlags1 &= ~MBFL_RMBUP; }
void slInput::DisableMMBDown() { g_framework->m_input.mouseButtonFlags1 &= ~MBFL_MMBDOWN; }
void slInput::DisableMMBHold() { g_framework->m_input.mouseButtonFlags2 &= ~MBFL_MMBHOLD; }
void slInput::DisableMMBUp()   { g_framework->m_input.mouseButtonFlags1 &= ~MBFL_MMBUP; }
void slInput::DisableX1MBDown() { g_framework->m_input.mouseButtonFlags1 &= ~MBFL_X1MBDOWN; }
void slInput::DisableX1MBHold() { g_framework->m_input.mouseButtonFlags2 &= ~MBFL_X1MBHOLD; }
void slInput::DisableX1MBUp()   { g_framework->m_input.mouseButtonFlags1 &= ~MBFL_X1MBUP; }
void slInput::DisableX2MBDown() { g_framework->m_input.mouseButtonFlags1 &= ~MBFL_X2MBDOWN; }
void slInput::DisableX2MBHold() { g_framework->m_input.mouseButtonFlags2 &= ~MBFL_X2MBHOLD; }
void slInput::DisableX2MBUp()   { g_framework->m_input.mouseButtonFlags1 &= ~MBFL_X2MBUP; }

bool slInput::IsLMBHit() { return (g_framework->m_input.mouseButtonFlags1 & MBFL_LMBDOWN) == MBFL_LMBDOWN; }
bool slInput::IsLMBHold() { return (g_framework->m_input.mouseButtonFlags2 & MBFL_LMBHOLD) == MBFL_LMBHOLD; }
bool slInput::IsLMBRelease() { return (g_framework->m_input.mouseButtonFlags1 & MBFL_LMBUP) == MBFL_LMBUP; }
bool slInput::IsRMBHit() { return (g_framework->m_input.mouseButtonFlags1 & MBFL_RMBDOWN) == MBFL_RMBDOWN; }
bool slInput::IsRMBHold() { return (g_framework->m_input.mouseButtonFlags2 & MBFL_RMBHOLD) == MBFL_RMBHOLD; }
bool slInput::IsRMBRelease() { return (g_framework->m_input.mouseButtonFlags1 & MBFL_RMBUP) == MBFL_RMBUP; }
bool slInput::IsMMBHit() { return (g_framework->m_input.mouseButtonFlags1 & MBFL_MMBDOWN) == MBFL_MMBDOWN; }
bool slInput::IsMMBHold() { return (g_framework->m_input.mouseButtonFlags2 & MBFL_MMBHOLD) == MBFL_MMBHOLD; }
bool slInput::IsMMBRelease() { return (g_framework->m_input.mouseButtonFlags1 & MBFL_MMBUP) == MBFL_MMBUP; }
bool slInput::IsX1MBHit() { return (g_framework->m_input.mouseButtonFlags1 & MBFL_X1MBDOWN) == MBFL_X1MBDOWN; }
bool slInput::IsX1MBHold() { return (g_framework->m_input.mouseButtonFlags2 & MBFL_X1MBHOLD) == MBFL_X1MBHOLD; }
bool slInput::IsX1MBRelease() { return (g_framework->m_input.mouseButtonFlags1 & MBFL_X1MBUP) == MBFL_X1MBUP; }
bool slInput::IsX2MBHit() { return (g_framework->m_input.mouseButtonFlags1 & MBFL_X2MBDOWN) == MBFL_X2MBDOWN; }
bool slInput::IsX2MBHold() { return (g_framework->m_input.mouseButtonFlags2 & MBFL_X2MBHOLD) == MBFL_X2MBHOLD; }
bool slInput::IsX2MBRelease() { return (g_framework->m_input.mouseButtonFlags1 & MBFL_X2MBUP) == MBFL_X2MBUP; }
