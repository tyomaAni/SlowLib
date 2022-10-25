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
#ifndef __SL_SLOWLIBBASEUNICODECONVERTER_H__
#define __SL_SLOWLIBBASEUNICODECONVERTER_H__

class SL_API slUnicodeConverter
{
public:
	slUnicodeConverter();
	~slUnicodeConverter();

	void CharToWchar(slStringA&, slStringW&);
	void CharToU8(slStringA&, slStringU8&);
	void CharToU16(slStringA&, slStringU16&);

	void WcharToChar(slStringW&, slStringA&);
	void WcharToU8(slStringW&, slStringU8&);
	void WcharToU16(slStringW&, slStringU16&);

	void U8ToChar(slStringU8&, slStringA&);
	void U8ToWchar(slStringU8&, slStringW&);
	void U8ToU16(slStringU8&, slStringU16&);

	void U16ToChar(slStringU16&, slStringA&);
	void U16ToWchar(slStringU16&, slStringW&);
	void U16ToU8(slStringU16&, slStringU8&);

	char m_defaultCharacter = ' ';
};

#endif
