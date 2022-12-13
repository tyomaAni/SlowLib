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
#ifndef __SL_SLOWLIBBASEDEFS_H__
#define __SL_SLOWLIBBASEDEFS_H__

// Some defines

// I think this is unnecessary, maybe this is old style thing, but I saw it in many
// professional projects.
// Just...`inline` will not guarantee that function will be inline
#ifdef _MSC_VER
#define SL_FORCEINLINE __forceinline
#else
#define SL_FORCEINLINE inline
#endif

#include <stddef.h> // NULL, size_t
#include <stdlib.h>
#include <assert.h> 
#include <stdint.h> // int32_t uint32_t and other
#include <ctype.h>  // isspace
#include <utility>  // std::move

using real_t = double;

// Detect platform
#if defined(WIN32) | defined(_WIN64) | defined(_WIN32)
#define SL_PLATFORM_WINDOWS
#else
#error Please, write code for other platform
#endif

// For C inside C++
#ifdef SL_PLATFORM_WINDOWS
#define SL_CDECL _cdecl
#else
#error Please, write code for other platform
#endif

// Our own _DEBUG
#ifdef _DEBUG
#define SL_DEBUG
#endif

// `pack` 4 bytes into 1 uint32_t
// I don't know about speed.
// uint32_i magic = SL_MAKEFOURCC('‰', 'P', 'N', 'G');
//    //magic must be `1196314761`
#define SL_MAKEFOURCC( ch0, ch1, ch2, ch3 )\
	((uint32_t)(uint8_t)(ch0)|((uint32_t)(uint8_t)(ch1)<<8)|\
	((uint32_t)(uint8_t)(ch2)<<16)|((uint32_t)(uint8_t)(ch3)<<24))

// Need to know if this is 64 bit configuration
#ifdef SL_PLATFORM_WINDOWS
#if defined _WIN64 || defined __x86_64__
#define SL_BIT_64
#endif
#else
#error Please, write code for other platform
#endif

// Show file name, line and function
// printf("%s %i %s\n", SL_FILE, SL_LINE, SL_FUNCTION);
#ifdef SL_PLATFORM_WINDOWS
#define SL_FILE __FILE__
#define SL_LINE __LINE__
#define SL_FUNCTION __FUNCTION__
#else
#error Please, write code for other platform
#endif

// For easy linking
#ifdef _MSC_VER

#if _MSC_VER >= 1920 && _MSC_VER < 1930
#define SL_LINK_LIBRARY_CMP "_v142"
#elif _MSC_VER >= 1930 && _MSC_VER < 1933
#error Please, do something!
#define SL_LINK_LIBRARY_CMP "_v142"
#endif

#ifdef SL_BIT_64
#define SL_LINK_LIBRARY_ARCH "_x64"
#else
#define SL_LINK_LIBRARY_ARCH "_x86"
#endif

#ifdef SL_DEBUG
#define SL_LINK_LIBRARY_CONF "_Debug"
#else
#define SL_LINK_LIBRARY_CONF "_Release"
#endif

// Use like this: SL_LINK_LIBRARY("libname")
#define SL_LINK_LIBRARY(n) \
	__pragma(comment(lib, n SL_LINK_LIBRARY_CMP SL_LINK_LIBRARY_ARCH SL_LINK_LIBRARY_CONF ".lib"))

#else
#error Please, do something!
#endif

// For example if you have 0xAABBCCDD
// You can get 0xAABB and 0xCCDD using this macros
#define SL_LO32(l) ((uint16_t)(((uint32_t)(l)) & 0xffff))
#define SL_HI32(l) ((uint16_t)((((uint32_t)(l)) >> 16) & 0xffff))

#endif
