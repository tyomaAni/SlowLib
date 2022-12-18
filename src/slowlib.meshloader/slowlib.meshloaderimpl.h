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
#ifndef _SL_MSHLDRIMPL_H_
#define _SL_MSHLDRIMPL_H_

#include "slowlib.base/geometry/slMesh.h"
#include "slowlib.base/geometry/slMeshLoader.h"
#include "slowlib.base/containers/slArray.h"

struct OBJMaterial;
class slMeshLoaderImpl : public slMeshLoader
{
	enum class extension { _bad, obj };
	extension _GetExtension(const char* path);

	void ImportOBJ_MTL(slArray<OBJMaterial*>& materials, const char* obj_fileName, const char* mtl_fileName, slMeshLoaderCallback* cb);
	void LoadOBJ(const char* path, slMeshLoaderCallback*);
	void LoadOBJ(const char* path, slMeshLoaderCallback*, uint8_t* buffer, uint32_t bufferSz);
public:
	slMeshLoaderImpl();
	virtual ~slMeshLoaderImpl();

	virtual uint32_t GetSupportedFilesCount() final;
	virtual slString GetSupportedFileExtension(uint32_t) final;
	virtual slString GetSupportedFileName(uint32_t) final;

	virtual void Load(const char* path, slMeshLoaderCallback*) final;
	virtual void Load(const char* path, slMeshLoaderCallback*, uint8_t* buffer, uint32_t bufferSz) final;
};


#endif
