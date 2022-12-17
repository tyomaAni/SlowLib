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
#include "slowlib.base/gs/slImage.h"
#include "slowlib.base/common/slColor.h"

struct r5g6b5
{
	union {
		uint16_t u16;
		uint8_t  u8[2];
	};
};
struct r8g8b8
{
	uint8_t r, g, b;
};
struct r8g8b8a8
{
	uint8_t r, g, b, a;
};

slImage::slImage() {}
slImage::~slImage()
{
	if (m_data)
		slMemory::free(m_data);
}

// fast creating
void slImage::Create(uint32_t x, uint32_t y)
{
	SL_ASSERT_ST(x);
	SL_ASSERT_ST(y);
	SL_ASSERT_ST(!m_data);
	m_info.m_width = x;
	m_info.m_height = y;
	m_info.m_pitch = m_info.m_width * 4;
	m_dataSize = m_info.m_pitch * m_info.m_height;
	m_data = (uint8_t*)slMemory::malloc(m_dataSize);
}

// must be created
void slImage::FlipVertical()
{
	SL_ASSERT_ST(m_data);
	uint8_t* line = nullptr;
	line = (uint8_t*)slMemory::malloc(m_info.m_pitch);
	uint8_t* p_Up = &m_data[0u];
	uint8_t* p_Down = &m_data[m_info.m_pitch * m_info.m_height - m_info.m_pitch];
	for (uint32_t i = 0u; i < m_info.m_height / 2u; ++i)
	{
		memcpy(line, p_Up, m_info.m_pitch);
		memcpy(p_Up, p_Down, m_info.m_pitch);
		memcpy(p_Down, line, m_info.m_pitch);
		p_Up += m_info.m_pitch;
		p_Down -= m_info.m_pitch;
	}
	slMemory::free(line);
}

// must be created
void slImage::FlipPixel()
{
	for (uint32_t i = 0u; i < m_dataSize; )
	{
		uint8_t r = m_data[i];
		uint8_t g = m_data[i + 1u];
		uint8_t b = m_data[i + 2u];
		uint8_t a = m_data[i + 3u];
		m_data[i] = b;
		m_data[i + 1u] = g;
		m_data[i + 2u] = r;
		m_data[i + 3u] = a;
		i += 4;
	}
}

// must be created
void slImage::Fill(const slColor& color)
{
	uint8_t* data = m_data;
	for (uint32_t h = 0; h < m_info.m_height; ++h)
	{
		for (uint32_t w = 0; w < m_info.m_width; ++w)
		{
			*data = color.getAsByteRed();   ++data;
			*data = color.getAsByteGreen(); ++data;
			*data = color.getAsByteBlue();  ++data;
			*data = color.getAsByteAlpha(); ++data;
		}
	}
}

void slImage::ConvertTo(slImageFormat newFormat)
{
	if (m_info.m_format == newFormat)
		return;

	switch (m_info.m_format)
	{
	case slImageFormat::r8g8b8:{
		switch (newFormat)
		{
		case slImageFormat::r8g8b8a8: {
			uint32_t new_pitch = m_info.m_width * 4;
			uint32_t new_dataSize = m_info.m_height * new_pitch;
			uint8_t* new_data = (uint8_t*)slMemory::malloc(new_dataSize);

			r8g8b8* rgb_data = (r8g8b8*)m_data;
			r8g8b8a8* rgba_data = (r8g8b8a8*)new_data;
			uint32_t num_of_pixels = m_info.m_width * m_info.m_height;
			for (uint32_t i = 0; i < num_of_pixels; ++i)
			{
				rgba_data->r = rgb_data->r;
				rgba_data->g = rgb_data->g;
				rgba_data->b = rgb_data->b;
				rgba_data->a = 255;

				rgba_data++;
				rgb_data++;
			}

			slMemory::free(m_data);

			m_data = new_data;
			m_dataSize = new_dataSize;
			m_info.m_pitch = new_pitch;
			m_info.m_format = slImageFormat::r8g8b8a8;
			m_info.m_bits = 32;
		}break;
		default:
			slLog::PrintWarning("slImage::ConvertTo: dst format not implemented\n");
			break;
		}
	}break;
	case slImageFormat::r5g6b5: {
		switch (newFormat)
		{
		case slImageFormat::r8g8b8a8: {
			uint32_t new_pitch = m_info.m_width * 4;
			uint32_t new_dataSize = m_info.m_height * new_pitch;
			uint8_t* new_data = (uint8_t*)slMemory::malloc(new_dataSize);

			r5g6b5* data16 = (r5g6b5*)m_data;
			r8g8b8a8* rgba_data = (r8g8b8a8*)new_data;
			uint32_t num_of_pixels = m_info.m_width * m_info.m_height;
			for (uint32_t i = 0; i < num_of_pixels; ++i)
			{
				rgba_data->r = ((data16->u16 & 0xF800)>>11)*8;
				rgba_data->g = ((data16->u16 & 0x7E0)>>6)* 8;
				rgba_data->b = ((data16->u16 & 0x1F))* 8;
				rgba_data->a = 255;

				rgba_data++;
				data16++;
			}
			slMemory::free(m_data);
			m_data = new_data;
			m_dataSize = new_dataSize;
			m_info.m_pitch = new_pitch;
			m_info.m_format = slImageFormat::r8g8b8a8;
			m_info.m_bits = 32;
		}break;
		default:
			slLog::PrintWarning("slImage::ConvertTo: dst format not implemented\n");
			break;
		}
	}break;
	case slImageFormat::a1r5g5b5: {
		switch (newFormat)
		{
		case slImageFormat::r8g8b8a8: {
			uint32_t new_pitch = m_info.m_width * 4;
			uint32_t new_dataSize = m_info.m_height * new_pitch;
			uint8_t* new_data = (uint8_t*)slMemory::malloc(new_dataSize);

			r5g6b5* data16 = (r5g6b5*)m_data;
			r8g8b8a8* rgba_data = (r8g8b8a8*)new_data;
			uint32_t num_of_pixels = m_info.m_width * m_info.m_height;
			for (uint32_t i = 0; i < num_of_pixels; ++i)
			{
				rgba_data->r = ((data16->u16 & 0xFC00) >> 10) * 8;
				rgba_data->g = ((data16->u16 & 0x3E0) >> 5) * 8;
				rgba_data->b = ((data16->u16 & 0x1F)) * 8;
				rgba_data->a = (data16->u16 & 0x8000) ? 255 : 0;

				rgba_data++;
				data16++;
			}
			slMemory::free(m_data);
			m_data = new_data;
			m_dataSize = new_dataSize;
			m_info.m_pitch = new_pitch;
			m_info.m_format = slImageFormat::r8g8b8a8;
			m_info.m_bits = 32;
		}break;
		default:
			slLog::PrintWarning("slImage::ConvertTo: dst format not implemented\n");
			break;
		}
	}break;
	case slImageFormat::x1r5g5b5: {
		switch (newFormat)
		{
		case slImageFormat::r8g8b8a8: {
			uint32_t new_pitch = m_info.m_width * 4;
			uint32_t new_dataSize = m_info.m_height * new_pitch;
			uint8_t* new_data = (uint8_t*)slMemory::malloc(new_dataSize);

			r5g6b5* data16 = (r5g6b5*)m_data;
			r8g8b8a8* rgba_data = (r8g8b8a8*)new_data;
			uint32_t num_of_pixels = m_info.m_width * m_info.m_height;
			for (uint32_t i = 0; i < num_of_pixels; ++i)
			{
				rgba_data->r = ((data16->u16 & 0xFC00) >> 10) * 8;
				rgba_data->g = ((data16->u16 & 0x3E0) >> 5) * 8;
				rgba_data->b = ((data16->u16 & 0x1F)) * 8;
				rgba_data->a = 255;

				rgba_data++;
				data16++;
			}
			slMemory::free(m_data);
			m_data = new_data;
			m_dataSize = new_dataSize;
			m_info.m_pitch = new_pitch;
			m_info.m_format = slImageFormat::r8g8b8a8;
			m_info.m_bits = 32;
		}break;
		default:
			slLog::PrintWarning("slImage::ConvertTo: dst format not implemented\n");
			break;
		}
	}break;
	default:
		slLog::PrintWarning("slImage::ConvertTo: src format not implemented\n");
		break;
	}

	m_info.m_format = newFormat;
}