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
#ifndef __SL_SLOWLIBBASEMEM_H__
#define __SL_SLOWLIBBASEMEM_H__

// Use this functions instead malloc\free new\delete
class slMemory
{
public:
	static void* malloc(size_t size);
	static void* calloc(size_t size);
	static void  free(void*);
	static void* realloc(void*, size_t size);
};

//template<typename _type>
//class slObjectCreator
//{
//public:
//
//	template<typename... Args>
//	static _type* create(Args&&... args)
//	{
//		_type* ptr = (_type*)slMemory::malloc(sizeof(_type));
//		if (ptr)
//			new(ptr) _type(std::forward<Args>(args)...);
//		return ptr;
//	}
//
//	static void destroy(_type* ptr)
//	{
//		assert(ptr);
//		ptr->~_type();
//		slMemory::free(ptr);
//	}
//};

// Create object
template<typename _type, typename... Args>
_type* slCreate(Args&&... args)
{
	_type* ptr = (_type*)slMemory::malloc(sizeof(_type));
	if (ptr)
		new(ptr) _type(std::forward<Args>(args)...);
	return ptr;

	//return slObjectCreator<_type>::create(std::forward<Args>(args)...);
}

template<typename _type>
void slDestroy(_type* ptr)
{
	assert(ptr);
	ptr->~_type();
	slMemory::free(ptr);
	//slObjectCreator<_type>::destroy(ptr);
}


#endif
