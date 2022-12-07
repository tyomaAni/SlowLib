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

#ifdef SL_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

slDLLHandle slDLL::load(const char* libraryName)
{
	SL_ASSERT(libraryName);
#ifdef SL_PLATFORM_WINDOWS
	return (slDLLHandle)LoadLibraryA(libraryName);
#else
#error Please, write code for other platform
#endif
}

void slDLL::free(slDLLHandle library)
{
	SL_ASSERT_ST(library);
#ifdef SL_PLATFORM_WINDOWS
	FreeLibrary((HMODULE)library);
#else
#error Please, write code for other platform
#endif
}

slDLLFunction slDLL::get_proc(slDLLHandle library, const char* functionName)
{
	SL_ASSERT_ST(library);
	SL_ASSERT_ST(functionName);
#ifdef SL_PLATFORM_WINDOWS
	return (slDLLFunction)GetProcAddress((HMODULE)library, functionName);
#else
#error Please, write code for other platform
#endif
}
