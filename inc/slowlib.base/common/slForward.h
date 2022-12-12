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
#ifndef __SL_SLOWLIBBASEFWD_H__
#define __SL_SLOWLIBBASEFWD_H__

struct slInputData;
class SL_API slInput;
class SL_API slString;
class SL_API slStringA;
class SL_API slStringW;
class SL_API slWindow;
class SL_API slCamera;
class SL_API slRay;
class SL_API slAabb;
class SL_API slPolygonMesh;
class SL_API slPolygon;
class SL_API slPolyTriangle;
class slWindowCallback;
class slGS;
class slPolyEdge;
struct slMaterial;
class slTexture;
class slMeshLoaderCallback;
class slMeshLoader;

template<typename T>
class slVec2_t;
template<typename T>
class slVec3_t;
template<typename T>
class slVec4_t;
template<typename T>
class slMatrix4_t;

using slVec2  = slVec2_t<real_t>;
using slVec2f = slVec2_t<float>;
using slVec3  = slVec3_t<real_t>;
using slVec3f = slVec3_t<float>;
using slVec4  = slVec4_t<real_t>;
using slVec4f = slVec4_t<float>;
using slMat4  = slMatrix4_t<real_t>;

template<typename _type>
struct slListNode;

template<typename _type>
class slList;

#endif
