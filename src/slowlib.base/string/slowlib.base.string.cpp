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

static uint8_t g_stringWordSize = 16;

slStringBase::slStringBase(uint32_t csz) :m_charSize(csz) 
{

}

slStringBase::~slStringBase() 
{
	if (m_data)
	{
		slFree(m_data);
		m_data = nullptr;
	}
}

void slStringBase::reallocate(size_t new_allocated)
{
	uint8_t* new_data = (uint8_t*)slMalloc(new_allocated * m_charSize);
	if (m_data)
	{
		memcpy(new_data, m_data, m_size * m_charSize);
		slFree(m_data);
	}
	else
	{
		memset(new_data, 0, new_allocated);
	}
	m_data = new_data;
	m_allocated = new_allocated;
}


slStringA::slStringA() :slStringBase(1) { reallocate(g_stringWordSize); }
slStringA::slStringA(const char* str) : slStringBase(1) 
{
	reallocate(g_stringWordSize);
}
slStringA::~slStringA() {}
const char* slStringA::c_str() { return (const char*)m_data; }

slStringW::slStringW() : slStringBase(2) { reallocate(g_stringWordSize); }
slStringW::slStringW(const wchar_t* str) : slStringBase(2)
{
	reallocate(g_stringWordSize);
}
slStringW::~slStringW() {}
const wchar_t* slStringW::c_str() { return (const wchar_t*)m_data; }