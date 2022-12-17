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
#ifndef __SL_SLOWLIBBASEIMG_H__
#define __SL_SLOWLIBBASEIMG_H__

enum class slImageFormat : uint32_t
{
	r8g8b8,
	r8g8b8a8,
	x1r5g5b5,
	a4r4g4b4,
	x4r4g4b4,
	r5g6b5,
	a1r5g5b5
};

struct slImageInfo
{
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	uint32_t m_bits = 32;
	uint32_t m_pitch = 0;
	slImageFormat m_format = slImageFormat::r8g8b8a8;
};

class slImage
{
public:
	slImage();
	~slImage();

	uint8_t* m_data = 0;
	uint32_t m_dataSize = 0;
	slImageInfo m_info;

	// fast creating
	void Create(uint32_t x, uint32_t y);
	
	// must be created
	// only for r8g8b8a8
	void FlipVertical();
	// must be created
	// only for r8g8b8a8
	void FlipPixel();
	// must be created
	// only for r8g8b8a8
	void Fill(const slColor& color);

	void ConvertTo(slImageFormat);
};

#endif
