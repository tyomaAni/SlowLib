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

#pragma pack(push,2)
struct BitmapHeader {
	uint16_t		bfType;
	uint32_t		bfSize;
	uint16_t		bfReserved1;
	uint16_t		bfReserved2;
	uint32_t		bfOffBits;
};
#pragma pack(pop)

struct ciexyzTriple {
	int32_t		ciexyzRed[3];
	int32_t		ciexyzGreen[3];
	int32_t		ciexyzBlue[3];
};
struct BitmapInfoHeader_v5 {
	uint32_t			bV5Size;		//	размер header в файле
	int32_t		bV5Width;		//	ширина
	int32_t		bV5Height;		//	высота
	uint16_t	bV5Planes;		//	хз что это, всегда 1
	uint16_t	bV5BitCount;	//	биты
	uint32_t	bV5Compression;	//	1 - RLE 8bit, 2 - RLE 4bit, 3 или что-то, видимо, специальные обозначения у разработчиков 2D редакторов.
	uint32_t	bV5SizeImage;	//	размер массива пикселей/индексов
	int32_t		bV5XPelsPerMeter;// размер в чём-то, видимо для печати или вывода ещё кудато
	int32_t		bV5YPelsPerMeter;//	для обычного использования в ПК не играет никакой роли
	uint32_t	bV5ClrUsed;		//	обычно тут ноль
	uint32_t	bV5ClrImportant;//	и тут ноль
	uint32_t	bV5RedMask;		//	для определения позиции цветов
	uint32_t	bV5GreenMask;	//	в форматах типа x1r5g5b5
	uint32_t	bV5BlueMask;
	uint32_t	bV5AlphaMask;
	uint32_t	bV5CSType;		//	далее информация для более специализированного
	ciexyzTriple bV5Endpoints;	//	использования.
	uint32_t	bV5GammaRed;	//	для передачи простой картинки достаточно того
	uint32_t	bV5GammaGreen;	//	что указано выше. А эта часть нужна для, например,
	uint32_t	bV5GammaBlue;	//	тех кто делает видео плеер, видео редактор
	uint32_t	bV5Intent;		//	что-то типа этого. Как бы универсальное решение
	uint32_t	bV5ProfileData;	//	от Microsoft
	uint32_t	bV5ProfileSize;
	uint32_t	bV5Reserved;
};

slImage* slImageLoaderImpl::LoadBMP(const char* path)
{
	SL_ASSERT_ST(path);

	slImage* img = 0;
	uint32_t file_size = 0;
	uint8_t* ptr = slFramework::SummonFileBuffer(path, &file_size, false);
	if (ptr)
	{
		img = LoadBMP(path, ptr, (uint32_t)file_size);
		slDestroy(ptr);
	}
	return img;
}

// 32bitRGBA
// 32bitRGB
// 24bit
// 16bit565RGB
// 16bit5551RGBA
// 16bit5551RGB
slImage* slImageLoaderImpl::LoadBMP(const char* path, uint8_t* buffer, uint32_t bufferSz)
{
	SL_ASSERT_ST(path);
	SL_ASSERT_ST(buffer);
	SL_ASSERT_ST(bufferSz);

	slZeroDecl(BitmapHeader, header);
	slZeroDecl(BitmapInfoHeader_v5, info);

	slFileBuffer file(buffer, bufferSz);

	if (file.Read(&header, sizeof(BitmapHeader)) != sizeof(BitmapHeader))
	{
		slLog::PrintWarning("BMP: Not a correct BMP file\n");
		return 0;
	}

	if (header.bfType != 19778)
	{
		slLog::PrintWarning("BMP: Not a correct BMP file\n");
		return 0;
	}

	file.Read(&info, sizeof(BitmapInfoHeader_v5));
	if (info.bV5Size < 40U)
	{
		slLog::PrintWarning("BMP: Bad header size\n");
		return 0;
	}

	if (info.bV5BitCount != 1u &&
		info.bV5BitCount != 4u &&
		info.bV5BitCount != 8u &&
		info.bV5BitCount != 16u &&
		info.bV5BitCount != 24u &&
		info.bV5BitCount != 32u)
	{
		slLog::PrintWarning("BMP: Bad bit count\n");
		return 0;
	}

	slImageInfo imageInfo;

	slImage* image = NULL;
	imageInfo.m_width = static_cast<uint32_t>(info.bV5Width);
	imageInfo.m_height = static_cast<uint32_t>(info.bV5Height);
	imageInfo.m_bits = info.bV5BitCount;

	bool flipPixel = false;

	if (imageInfo.m_bits == 24u)
	{
		imageInfo.m_format = slImageFormat::r8g8b8;
		imageInfo.m_pitch = imageInfo.m_width * 3;

		image = slCreate<slImage>();
		image->m_dataSize = imageInfo.m_pitch * imageInfo.m_height;
		image->m_data = (uint8_t*)slMemory::malloc(image->m_dataSize);

		file.Seek(header.bfOffBits, SEEK_SET);
		file.Read(image->m_data, image->m_dataSize);
		flipPixel = true;
	}
	else if (imageInfo.m_bits == 32u)
	{
		uint32_t offset = header.bfOffBits;
		imageInfo.m_pitch = imageInfo.m_width * 4;

		if (info.bV5Compression == 3 || info.bV5Compression == 0) // BI_BITFIELDS BI_RGB
		{
			imageInfo.m_pitch = imageInfo.m_width * 4;
			file.Seek(offset, SEEK_SET);
			imageInfo.m_format = slImageFormat::r8g8b8a8;

			image = slCreate<slImage>();
			image->m_dataSize = imageInfo.m_pitch * imageInfo.m_height;
			image->m_data = (uint8_t*)slMemory::malloc(image->m_dataSize);

			file.Read(image->m_data, image->m_dataSize);
			image->FlipPixel();
		}
		else
		{
			slLog::PrintWarning("BMP: unsupported format\n");
			return 0;
		}
	}
	else if (imageInfo.m_bits == 16)
	{
		if (info.bV5Size != 40 && info.bV5Size != 56 && info.bV5Size != 108)
		{
			slLog::PrintWarning("BMP: unsupported format\n");
			return 0;
		}

		imageInfo.m_pitch = imageInfo.m_width * 2;

		if (info.bV5RedMask == 3840 &&
			info.bV5GreenMask == 240 &&
			info.bV5BlueMask == 15)
		{
			if (info.bV5AlphaMask)
			{
				imageInfo.m_format = slImageFormat::a4r4g4b4;
			}
			else
			{
				imageInfo.m_format = slImageFormat::x4r4g4b4;
			}
		}
		else if (info.bV5RedMask == 63488 &&
			info.bV5GreenMask == 2016 &&
			info.bV5BlueMask == 31)
		{
			imageInfo.m_format = slImageFormat::r5g6b5;
		}
		else if (info.bV5RedMask == 31744 &&
			info.bV5GreenMask == 992 &&
			info.bV5BlueMask == 31)
		{
			if (info.bV5AlphaMask)
				imageInfo.m_format = slImageFormat::a1r5g5b5;
		}
		else
		{
			imageInfo.m_format = slImageFormat::x1r5g5b5;
		}
		file.Seek(70, SEEK_SET);
		image = slCreate<slImage>();
		image->m_dataSize = imageInfo.m_pitch * imageInfo.m_height;
		image->m_data = (uint8_t*)slMemory::malloc(image->m_dataSize);

		file.Read(image->m_data, image->m_dataSize);
	}
	else if (imageInfo.m_bits == 8)
	{
		file.Seek(54u, SEEK_SET);

		uint32_t colorNum = 256;
		if (info.bV5ClrUsed)
			colorNum = info.bV5ClrUsed;

		struct rgba {
			uint8_t r, g, b, a;
		};
		rgba colors[256];
		file.Read(colors, colorNum * sizeof(rgba));
		long pos = file.Tell();

		file.Seek(header.bfOffBits, SEEK_SET);

		//uint32_t rleDataSz = header.bfSize - header.bfOffBits;

		imageInfo.m_pitch = imageInfo.m_width * 4;

		image = slCreate<slImage>();
		image->m_dataSize = imageInfo.m_pitch * imageInfo.m_height;
		image->m_data = (uint8_t*)slMemory::malloc(image->m_dataSize);
		imageInfo.m_bits = 32;
		imageInfo.m_format = slImageFormat::r8g8b8a8;

		rgba* _rgba = (rgba*)image->m_data;
		uint32_t dSz = imageInfo.m_width * imageInfo.m_height;
		char8_t* data = new char8_t[dSz];
		file.Read(data, dSz);

		if (info.bV5Compression)
		{

		}
		else
		{
			for (uint32_t i = 0; i < dSz; ++i)
			{
				char8_t b = data[i];

				_rgba->a = colors[b].a;
				_rgba->r = colors[b].b;
				_rgba->g = colors[b].g;
				_rgba->b = colors[b].r;
				_rgba++;
			}
		}
		delete[]data;
	}
	else if (imageInfo.m_bits == 4)
	{
	}
	else if (imageInfo.m_bits == 1)
	{
	}

	if (image)
	{
		image->m_info = imageInfo;
		image->ConvertTo(slImageFormat::r8g8b8a8);
		image->FlipVertical();
		if (flipPixel)
			image->FlipPixel();
	}

	return image;
}
