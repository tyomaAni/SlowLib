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

#include <stdio.h>

#ifdef SL_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

void onAssert_default(const char* message, const char* file, uint32_t line, uint32_t flags)
{
#ifdef SL_DEBUG
    if ((flags & sl_internal::flag_onAssert_useStackTrace) == sl_internal::flag_onAssert_useStackTrace)
    {
        slStackTracer::Print();
    }

    if ((flags & sl_internal::flag_onAssert_useDefaultSystemWindow) == sl_internal::flag_onAssert_useDefaultSystemWindow)
    {
        slString str_msg(message);
        slString str_file(file);

        slStringW str1;
        str_msg.to_utf16(str1);

        slStringW str2;
        str_file.to_utf16(str2);

#ifdef SL_PLATFORM_WINDOWS
        _wassert((const wchar_t*)str1.m_data, (const wchar_t*)str2.m_data, line);
#endif
    }
    else
    {
        //printf("%s %s %i\n", message, file, line);
        slLog::PrintError("%s %s %i\n", message, file, line);
#ifdef SL_PLATFORM_WINDOWS
        DebugBreak();
#endif
    }
#endif
}

static void(*g_onAssert)(const char* message, const char* file, uint32_t line, uint32_t flags) = onAssert_default;

namespace sl_internal
{
    SL_API void SL_CDECL onAssert(const char* message, const char* file, uint32_t line, uint32_t flags)
    {
        g_onAssert(message, file, line, flags);
    }
}

extern "C"
{
    SL_API void SL_CDECL slSetOnAssert(void(* f)(const char* message, const char* file, uint32_t line, uint32_t flags))
    {
        g_onAssert = f ? f : onAssert_default;
    }
}
