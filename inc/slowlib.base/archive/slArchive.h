﻿/*
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
#ifndef _SL_LIB_ARCHIVE_H_
#define _SL_LIB_ARCHIVE_H_

#include <vector>

// This is not algorithm.
// This is library. If some library will have many algorithms,
// here will be more options like:
//     megalib_deflate,
//     megalib_lz77,
//     megalib_lzma,
// We have:
//     -   fastlz (LZ77)
enum class slCompressorType : uint32_t
{
    // m_level:
    //  1 for speed
    //  2 compression ratio 
    fastlz
};

// Minimum iformation for data compression.
struct slCompressionInfo
{
    slCompressorType m_compressorType = slCompressorType::fastlz;

    // Read the comments below in Compress\Decomress
    uint32_t m_sizeUncompressed = 0;
    uint8_t* m_dataUncompressed = 0;

    uint32_t m_sizeCompressed = 0;
    uint8_t* m_dataCompressed = 0;

    // See CompressorType. If the value is wrong, the 
    // default value will be used.
    uint32_t m_level = 2;
};

struct slArchiveZipFile
{
    slStringA m_fileName;
    void* m_implementation = 0;
};

class slArchiveSystem
{
public:
    // Add zip file for unzipping files from it.
    static slArchiveZipFile* ZipAdd(const char* zipFile);

    // Unzip file. If a is NULL first found file will be processed.
    // Set a if you have many zip files with same file inside.
    // Function will allocate memory using slMemory::malloc, and will write data size in size.
    // Pointer will return if everything is OK, use slMemory::free for deallocation.
    static uint8_t* ZipUnzip(const char* fileInZip, uint32_t* size, slArchiveZipFile* a = NULL);

    // Set all data in `info`. If compression is OK, true will return
    // and m_dataCompressed will have address.
    // You need to call slMemory::free(m_dataCompressed) for deallocation.
    static bool Compress(slCompressionInfo* info);

    // Like in `Compress` method.
    // Allocate memory m_dataUncompressed by yourself.
    // You must know m_sizeUncompressed.
    static bool Decompress(slCompressionInfo* info);

    // Decompress and store data in std::vector.
    // if info.m_dataCompressed is NULL (or !info.m_sizeCompressed)
    // then function will try to copy data from info.m_dataUncompressed
    // Function will allocate memory, put all data to std::vector and then
    // will free memory.
    // info.m_dataUncompressed and info.m_sizeUncompressed will be ignored
    static bool Decompress(slCompressionInfo* info, std::vector<uint8_t>& toVector);
};

#endif 