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
#ifndef __SL_SLOWLIB_H__
#define __SL_SLOWLIB_H__

template<class T>
const T& slMax(const T& a, const T& b)
{
    return (a < b) ? b : a;
}
template<class T>
const T& slMin(const T& a, const T& b)
{
    return (b < a) ? b : a;
}

#include "slowlib.base/common/slDefs.h"
#include "slowlib.base/common/slForward.h"
#include "slowlib.base/common/slLog.h"
#include "slowlib.base/common/slUID.h"
#include "slowlib.base/memory/slMemory.h"
#include "slowlib.base/string/slString.h"
#include "slowlib.base/common/slAssert.h"
#include "slowlib.base/system/slDll.h"
#include "slowlib.base/system/slStacktracer.h"
#include "slowlib.base/framework/slFramework.h"
#include "slowlib.base/system/slWindow.h"
#include "slowlib.base/math/slMath.h"




#endif
