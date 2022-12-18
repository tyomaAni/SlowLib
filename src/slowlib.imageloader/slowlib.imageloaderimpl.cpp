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

slImageLoaderImpl::slImageLoaderImpl(){}
slImageLoaderImpl::~slImageLoaderImpl(){}

uint32_t slImageLoaderImpl::GetSupportedFilesCount()
{
	return 1;
}

slString slImageLoaderImpl::GetSupportedFileExtension(uint32_t i)
{
	switch (i)
	{
	case 0:
		return "bmp";
	}
	return "-";
}

slString slImageLoaderImpl::GetSupportedFileName(uint32_t i)
{
	switch (i)
	{
	case 0:
		return "Windows Bitmap";
	}
	return "-";
}

slImageLoaderImpl::extension slImageLoaderImpl::_GetExtension(const char* path)
{
	size_t len = strlen(path);
	if (len > 4)
	{
		// find last '.'
		size_t last_dot = 0;
		for (size_t i = 0; i < len; ++i)
		{
			if (path[i] == '.')
				last_dot = i;
		}
		if (last_dot)
		{
			if (strcmp(".bmp", &path[last_dot]) == 0)
				return slImageLoaderImpl::extension::bmp;
		}
	}
	return extension::_bad;
}

slImage* slImageLoaderImpl::Load(const char* path)
{
	auto e = _GetExtension(path);
	switch (e)
	{
	case slImageLoaderImpl::extension::_bad:
		break;
	case slImageLoaderImpl::extension::bmp:
		return LoadBMP(path);
	}
	return nullptr;
}

slImage* slImageLoaderImpl::Load(const char* path, uint8_t* buffer, uint32_t bufferSz)
{
	auto e = _GetExtension(path);
	switch (e)
	{
	case slImageLoaderImpl::extension::_bad:
		break;
	case slImageLoaderImpl::extension::bmp:
		return LoadBMP(path, buffer, bufferSz);
	}
	return nullptr;
}
