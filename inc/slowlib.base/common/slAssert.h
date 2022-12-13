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
#ifndef __SL_SLOWLIBBASEASSERT_H__
#define __SL_SLOWLIBBASEASSERT_H__

// Custom assert()

namespace sl_internal
{
    enum flag_onAssert
    {
        flag_onAssert_useDefaultSystemWindow = 0x1,
        flag_onAssert_useStackTrace = 0x2,
    };
    void SL_CDECL onAssert(const char* message, const char* file, uint32_t line, uint32_t flags = 0);
}

#ifdef SL_DEBUG
#define SL_ASSERT(expression) if((expression) == 0){sl_internal::onAssert(#expression, SL_FILE, SL_LINE, 0);}
#define SL_ASSERT_ST(expression) if((expression) == 0){sl_internal::onAssert(#expression, SL_FILE, SL_LINE, sl_internal::flag_onAssert_useDefaultSystemWindow | sl_internal::flag_onAssert_useStackTrace);}
#else
#define SL_ASSERT(expression) ((void)0)
#define SL_ASSERT_ST(expression) ((void)0)
#endif

extern "C"
{
    void SL_CDECL slSetOnAssert(void(*)(const char* message, const char* file, uint32_t line, uint32_t flags));
}

#endif
