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
#ifndef __SL_SLOWLIBBASESTRING_H__
#define __SL_SLOWLIBBASESTRING_H__

class SL_API slStringBase
{
protected:
	size_t m_size = 0;
	size_t m_allocated = 0;
	uint32_t m_charSize = 0;
	uint8_t* m_data = nullptr;

	void reallocate(size_t new_allocated);
public:
	slStringBase(uint32_t csz);
	virtual ~slStringBase();
	
	size_t size();
};

class SL_API slStringA : public slStringBase
{
public:
	slStringA();
	slStringA(const char*);
	~slStringA();
	
	const char* c_str();
};

class SL_API slStringW : public slStringBase
{
public:
	slStringW();
	slStringW(const wchar_t*);
	~slStringW();
	
	const wchar_t* c_str();
};

#endif
