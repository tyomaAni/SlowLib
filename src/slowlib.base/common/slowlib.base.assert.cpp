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

namespace sl_internal
{
    SL_API void SL_CDECL onAssert(const char* message, const char* file, uint32_t line, uint32_t flags)
    {
        if ((flags & flag_onAssert_useDefaultSystemWindow) == flag_onAssert_useDefaultSystemWindow)
        {
            size_t message_len = strlen(message);
            size_t file_len = strlen(file);
            if (message_len && file_len)
            {
                /*wchar_t* wmsg = (wchar_t*)malloc((message_len + 1) * sizeof(wchar_t));
                if (wmsg)
                {
                    wchar_t* wfile = (wchar_t*)malloc((file_len + 1) * sizeof(wchar_t));
                    if (wfile)
                    {
                        slUnicodeConverter uc;
                        uc.CharToWchar(message, wmsg);
                        uc.CharToWchar(file, wfile);

                        _wassert(wmsg, wfile, line);
                        free(wfile);
                    }
                    free(wmsg);
                }*/
            }
        }
        else
        {
            printf("%s %s %i\n", message, file, line);
            DebugBreak();
        }
    }
}
