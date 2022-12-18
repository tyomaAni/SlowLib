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

#include "slowlib.imageloaderimpl.h"
#include "slowlib.base/common/slFileBuffer.h"

#include <png.h>
#include <zlib.h>

static void __cdecl user_error_fn(png_structp png_ptr, png_const_charp msg) {
	fprintf(stderr, "%s\n", msg);
	longjmp(png_jmpbuf(png_ptr), 1);
}
static void __cdecl user_warning_fn(png_structp /*png_ptr*/, png_const_charp msg) {
	fprintf(stderr, "%s\n", msg);
}
class PNG
{
public:

	PNG() :
		png_ptr(nullptr),
		info_ptr(nullptr)
	{}

	~PNG() {
		if (info_ptr) {
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		}
		else if (png_ptr)
			png_destroy_read_struct(&png_ptr, NULL, NULL);
	}

	png_structp png_ptr;
	png_infop info_ptr;
};

void PNGAPI user_read_fn(png_structp png_ptr, png_bytep data, png_size_t length)
{
	slFileBuffer* f = (slFileBuffer*)png_get_io_ptr(png_ptr);
	f->Read((char*)data, (uint32_t)length);
}

slImage* slImageLoaderImpl::LoadPNG(const char* path)
{
	SL_ASSERT_ST(path);

	slImage* img = 0;
	uint32_t file_size = 0;
	uint8_t* ptr = slFramework::SummonFileBuffer(path, &file_size, false);
	if (ptr)
	{
		img = LoadPNG(path, ptr, (uint32_t)file_size);
		slDestroy(ptr);
	}
	return img;
}

slImage* slImageLoaderImpl::LoadPNG(const char* path, uint8_t* buffer, uint32_t bufferSz)
{
	SL_ASSERT_ST(path);
	SL_ASSERT_ST(buffer);
	SL_ASSERT_ST(bufferSz);

	slFileBuffer file(buffer, bufferSz);

	uint8_t buf[4];
	file.Read((char*)buf, 4);
	if (png_sig_cmp(buf, (png_size_t)0, 4)) {
		slLog::PrintWarning("PNG: unsupported format\n");
		return 0;
	}

	PNG png;
	png.png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, user_error_fn, user_warning_fn);
	if (!png.png_ptr) {
		slLog::PrintWarning("PNG: unsupported format\n");
		return 0;
	}

	png.info_ptr = png_create_info_struct(png.png_ptr);
	if (!png.info_ptr) {
		slLog::PrintWarning("PNG: unsupported format\n");
		return 0;
	}

	if (setjmp(png_jmpbuf(png.png_ptr))) {
		slLog::PrintWarning("PNG: unsupported format\n");
		return 0;
	}

	png_set_read_fn(png.png_ptr, (void*)&file, user_read_fn);
	png_set_sig_bytes(png.png_ptr, 4);
	png_read_info(png.png_ptr, png.info_ptr);

	uint32_t w, h;
	int bits;
	int color_type;
	png_get_IHDR(png.png_ptr, png.info_ptr, &w, &h, &bits, &color_type, NULL, NULL, NULL);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png.png_ptr);

	if (bits < 8) {
		if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_expand_gray_1_2_4_to_8(png.png_ptr);
		else
			png_set_packing(png.png_ptr);
	}

	if (png_get_valid(png.png_ptr, png.info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png.png_ptr);

	if (bits == 16)
		png_set_strip_16(png.png_ptr);

	if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png.png_ptr);

	int intent;
	const double screen_gamma = 2.2;

	if (png_get_sRGB(png.png_ptr, png.info_ptr, &intent))
		png_set_gamma(png.png_ptr, screen_gamma, 0.45455);
	else {
		double image_gamma;
		if (png_get_gAMA(png.png_ptr, png.info_ptr, &image_gamma))
			png_set_gamma(png.png_ptr, screen_gamma, image_gamma);
		else
			png_set_gamma(png.png_ptr, screen_gamma, 0.45455);
	}

	png_read_update_info(png.png_ptr, png.info_ptr);
	png_get_IHDR(png.png_ptr, png.info_ptr, &w, &h, &bits, &color_type,
		NULL, NULL, NULL);

	slImage* image = slCreate<slImage>();
	image->m_info.m_width = w;
	image->m_info.m_height = h;
	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
		image->m_info.m_format = slImageFormat::r8g8b8a8;
		image->m_info.m_pitch = image->m_info.m_width * 4;
	}
	else {
		image->m_info.m_format = slImageFormat::r8g8b8;
		image->m_info.m_pitch = image->m_info.m_width * 3;
	}

	image->m_dataSize = image->m_info.m_pitch * image->m_info.m_height;
	image->m_data = (uint8_t*)slMemory::malloc(image->m_dataSize);

	//std::unique_ptr<png_bytep[]> row_pointers(new png_bytep[image->m_height]);
	png_bytep* row_pointers = new png_bytep[image->m_info.m_height];
	for (uint32_t row = 0, p = 0; row < image->m_info.m_height; ++row) {
		row_pointers[row] = &image->m_data[p];
		p += image->m_info.m_pitch;
	}

	png_read_image(png.png_ptr, row_pointers);
	png_read_end(png.png_ptr, NULL);
	
	if (slFramework::GetImageLoaderConvertToRGBA8())
		image->ConvertTo(slImageFormat::r8g8b8a8);

	return image;
}
