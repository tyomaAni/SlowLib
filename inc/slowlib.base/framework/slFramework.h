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
#ifndef __SL_SLOWLIBBASEFWK_H__
#define __SL_SLOWLIBBASEFWK_H__

// Simple typeswithout methods
struct slPoint {
	int32_t x = 0;
	int32_t y = 0;
};
struct slRect {
	int32_t left = 0;
	int32_t top = 0;
	int32_t right = 0;
	int32_t bottom = 0;
};
struct slRectf {
	float left = 0.f;
	float top = 0.f;
	float right = 0.f;
	float bottom = 0.f;
};

#include "slowlib.base/input/slInput.h"

// Some callbacks. Some information from framework will be here.
class slFrameworkCallback
{
public:
	slFrameworkCallback() {}
	virtual ~slFrameworkCallback() {}

	virtual void OnMessage();
};

// Main API for framework.
// It will be static methods.
class SL_API slFramework
{
public:
	//allocate implementation
	static void Start(slFrameworkCallback*);
	
	//deallocate implementation
	static void Stop();
	
	//calculate deltatime, update input, update GUI, update window
	static void Update();

	//create window. CreateWindow is winapi function so I will use word `summon`
	static slWindow* SummonWindow(slWindowCallback*, int sx, int sy);

	static float* GetDeltaTime();

	static bool PointInRect(slRect* r, slPoint* p);
	static void PointSet(slPoint* p, int32_t x, int32_t y);
	static void RectSet(slRect* rct, int32_t l, int32_t t, int32_t r, int32_t b);
	
	// Need to test. can I use it like this? RectfSet(&r, {0.f,1.f,2.f,3.f});
	static void RectfSet(slRectf* rct, float*);
};

#endif
