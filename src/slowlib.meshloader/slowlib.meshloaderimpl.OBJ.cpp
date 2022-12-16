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

#include "slowlib.meshloaderimpl.h"
#include "slowlib.base/geometry/slGeometry.h"
#include "slowlib.base/geometry/slPolyMesh.h"
#include <vector>
#include <map>
#include <string>

enum class OBJFaceType
{
	p,
	pu,
	pun,
	pn
};

struct OBJSimpleArr
{
	OBJSimpleArr() {
		sz = 0;
	}

	int data[0x100];
	unsigned int sz;

	void push_back(int v) { data[sz++] = v; }
	unsigned int size() { return sz; }
	void reset() { sz = 0; }
};

struct OBJFace
{
	OBJFace() {
		ft = OBJFaceType::pun;
	}

	OBJSimpleArr p, u, n;
	OBJFaceType ft;

	void reset()
	{
		ft = OBJFaceType::pun;
		p.reset();
		u.reset();
		n.reset();
	}
};

unsigned char* OBJNextLine(unsigned char* ptr);
unsigned char* OBJSkipSpaces(unsigned char* ptr);
unsigned char* OBJReadVec2(unsigned char* ptr, slVec2f& vec2);
unsigned char* OBJReadFloat(unsigned char* ptr, float& value);
unsigned char* OBJReadVec3(unsigned char* ptr, slVec3f& vec3);
unsigned char* OBJReadFace(unsigned char* ptr, OBJFace& f, char* s);
unsigned char* OBJReadWord(unsigned char* ptr, slStringA& str);
unsigned char* OBJReadLastWord(unsigned char* ptr, slStringA& str);

bool g_ImportOBJ_triangulate = false;
bool g_ImportOBJ_readMTL = true;

struct OBJMaterial
{
	OBJMaterial() {
		m_specularExponent = 10.f;
		m_opacity = 1.f;
		m_transparency = 0.f;
		m_refraction = 1.f;
	}

	slStringA m_name; // newmtl
	slVec3f m_ambient;   // Ka
	slVec3f m_diffuse;   // Kd
	slVec3f m_specular;  // Ks
	float m_specularExponent; // Ns
	float m_opacity;    // d
	float m_transparency; // Tr
	float m_refraction;  // Ni

	slStringA m_map_diffuse; // map_Kd  
	slStringA m_map_ambient; // map_Ka 
	slStringA m_map_specular; // map_Ks   
	slStringA m_map_specularHighlight; // map_Ns    
	slStringA m_map_alpha; // map_d     
	slStringA m_map_bump; // map_bump bump 
	slStringA m_map_displacement; // disp 
	slStringA m_map_reflection; // refl  
};

bool OBJStringEqual(const slStringA& str, const char* c_Str)
{
	auto c_str_len = std::strlen(c_Str);
	if (c_str_len != str.size())
		return false;

	for (size_t i = 0, sz = str.size(); i < sz; ++i)
	{
		if (str[i] != (uint32_t)c_Str[i])
			return false;
	}

	return true;
}

void slMeshLoaderImpl::ImportOBJ_MTL(
	slArray<OBJMaterial*>& materials,
	const char* obj_fileName,
	const char* mtl_fileName,
	slMeshLoaderCallback* cb
)
{
	//auto relPath = g_sdk->FileGetRelativePath(obj_fileName);
	slString relPath = obj_fileName;

	for (size_t i = 0, sz = relPath.size(); i < sz; ++i)
	{
		auto c = relPath.pop_back_return();
		if (c == U'/' || c == U'\\')
		{
			relPath += c;
			break;
		}
	}

	slString mtlPath = relPath;
	mtlPath += mtl_fileName;
	
	if (slFramework::FileExist(mtlPath))
	{
		slStringA utf8;
		mtlPath.to_utf8(utf8);

		FILE* file = 0;
		fopen_s(&file, utf8.data(), "rb");
		if (file)
		{
			auto file_size = (size_t)slFramework::FileSize(utf8.data());

			std::vector<unsigned char> file_byte_array((unsigned int)file_size + 2);
			unsigned char* ptr = file_byte_array.data();

			fread(ptr, 1, file_size, file);
			fclose(file);

			ptr[(unsigned int)file_size] = ' ';
			ptr[(unsigned int)file_size + 1] = 0;

			OBJMaterial* curMaterial = 0;

			while (*ptr)
			{
				switch (*ptr)
				{
				case '#':
				case 'd':
				case 'T':
				case 'i':
				case 'K':
					ptr = OBJNextLine(ptr);
					break;
				case 'n':
				{
					slStringA mtlWord;
					ptr = OBJReadWord(ptr, mtlWord);

					if (OBJStringEqual(mtlWord, "newmtl"))
					{
						curMaterial = new OBJMaterial;
						materials.push_back(curMaterial);

						ptr = OBJReadWord(ptr, curMaterial->m_name);
					}
				}break;
				case 'N':
				{
					slStringA word;
					ptr = OBJReadWord(ptr, word);
					if (OBJStringEqual(word, "Ns"))
					{
						ptr = OBJSkipSpaces(ptr);
						ptr = OBJReadFloat(ptr, curMaterial->m_specularExponent);
					}
					else if (OBJStringEqual(word, "Ni"))
					{
						ptr = OBJSkipSpaces(ptr);
						ptr = OBJReadFloat(ptr, curMaterial->m_refraction);
					}
					else
						ptr = OBJNextLine(ptr);

				}break;
				case 'm':
				{
					slStringA word;
					ptr = OBJReadWord(ptr, word);
					if (OBJStringEqual(word, "map_Kd"))
					{
						ptr = OBJReadLastWord(ptr, word);

						slString mapPath;
						mapPath.append(word.data());

						if (!slFramework::FileExist(mapPath))
						{
							mapPath = relPath;
							mapPath.append(word.data());

							//if (g_sdk->FileExist(mapPath.data()))
							//	printf("OK\n");
						}

						mapPath.to_utf8(curMaterial->m_map_diffuse);

						/*ptr = OBJSkipSpaces(ptr);
						if (*ptr == L'-')
						{
							ptr = OBJReadWord(ptr, word);
							if (OBJStringEqual(word, "-bm"))
							{

							}
						}*/
					}
					else
						ptr = OBJNextLine(ptr);
				}break;
				default:
					++ptr;
					break;
				}
			}
		}
	}
}

OBJMaterial* OBJGetMaterial(
	slArray<OBJMaterial*>& materials,
	const slStringA& name
)
{
	for (uint32_t i = 0; i < materials.m_size; ++i)
	{
		if (materials.m_data[i]->m_name == name)
			return materials.m_data[i];
	}
	return 0;
}

void slMeshLoaderImpl::LoadOBJ(const char* path, slMeshLoaderCallback* cb)
{
	SL_ASSERT_ST(path);
	SL_ASSERT_ST(cb);

	slArray<OBJMaterial*> obj_materials;

	FILE* file = 0;
	fopen_s(&file, path, "rb");
	if (!file)
		return;

	auto file_size = (size_t)slFramework::FileSize(path);

	std::vector<unsigned char> file_byte_array((unsigned int)file_size + 2);

	unsigned char* ptr = file_byte_array.data();
	fread(ptr, 1, file_size, file);
	fclose(file);

	ptr[(unsigned int)file_size] = ' ';
	ptr[(unsigned int)file_size + 1] = 0;

	bool groupBegin = false;
	bool isModel = false;
	bool grpFound = false;

	slVec2f tcoords;
	slVec3f pos;
	slVec3f norm;

	std::vector<slVec3f> position;
	std::vector<slVec2f> uv;
	std::vector<slVec3f> normal;

	position.reserve(0xffff);
	uv.reserve(0xffff);
	normal.reserve(0xffff);

	position.reserve(0xffff);
	uv.reserve(0xffff);
	normal.reserve(0xffff);

	//std::string name_word;
	slStringA tmp_word;
	slString curr_word;
	slString prev_word;

	OBJFace f;
	char s[0xff];

	int last_counter[3] = { 0,0,0 };

	slPolygonCreator polygonCreator;
	slPolygonMesh* polygonMesh = 0;

	OBJMaterial* currMaterial = 0;
	while (*ptr)
	{
		switch (*ptr)
		{
		case 'm'://mtllib
		{
			slStringA word;
			ptr = OBJReadWord(ptr, word);

			if (OBJStringEqual(word, "mtllib"))
			{
				ptr = OBJReadWord(ptr, word);
				//wprintf(L"MTL: %s\n", mtlWord.data());
				ImportOBJ_MTL(obj_materials, path, word.data(), cb);
			}
		}break;
		case 'u'://usemtl
		{
			slStringA word;
			ptr = OBJReadWord(ptr, word);

			if (OBJStringEqual(word, "usemtl"))
			{
				ptr = OBJReadWord(ptr, word);
				currMaterial = OBJGetMaterial(obj_materials, word);
			}
		}break;
		case 's':
		case 'l':
		case 'c'://curv
		case 'p'://parm
		case 'd'://deg
		case '#':
		case 'e'://end
			ptr = OBJNextLine(ptr);
			break;
		case 'v':
		{
			++ptr;
			if (groupBegin)
				groupBegin = false;
			switch (*ptr)
			{
			case 't':
				ptr = OBJReadVec2(++ptr, tcoords);
				uv.push_back(tcoords);
				++last_counter[1];
				break;
			case 'n':
				ptr = OBJReadVec3(++ptr, norm);
				normal.push_back(norm);
				++last_counter[2];
				break;
			default:
				ptr = OBJReadVec3(ptr, pos);
				position.push_back(pos);
				++last_counter[0];
				//newModel->m_aabb.add(pos);
				break;
			}
		}break;
		case 'f':
		{
			isModel = true;
			f.reset();
			ptr = OBJReadFace(++ptr, f, s);

			polygonCreator.Clear();

			bool genNormals = true;

			for (size_t sz2 = f.p.size(), i2 = 0; i2 < sz2; ++i2)
			{
				auto index = i2;
				auto pos_index = f.p.data[index];


				if (pos_index < 0)
				{
					// если индекс отрицательный то он указывает на позицию относительно последнего элемента
					// -1 = последний элемент
					// if index is negative then he points on position relative last element
					// -1 = last element
					pos_index = last_counter[0] + pos_index + 1;
				}

				auto v = position[pos_index];

				slVec3f pcPos, pcNorm;
				slVec2f pcUV;

				pcPos = v;

				if (f.ft == OBJFaceType::pu || f.ft == OBJFaceType::pun)
				{
					auto uv_index = f.u.data[index];

					if (uv_index < 0)
					{
						uv_index = last_counter[1] + uv_index + 1;
					}

					auto u = uv[uv_index];
					pcUV.x = u.x;
					pcUV.y = 1.f - u.y;
				}

				if (f.ft == OBJFaceType::pn || f.ft == OBJFaceType::pun)
				{
					auto nor_index = f.n.data[index];

					if (nor_index < 0)
					{
						nor_index = last_counter[2] + nor_index + 1;
					}

					auto n = normal[nor_index];
					//	geometry_creator->AddNormal(n.x, n.y, n.z);
					pcNorm = n;
					genNormals = false;
				}
				polygonCreator.SetPosition(pcPos);
				polygonCreator.SetNormal(pcNorm);
				polygonCreator.SetUV(pcUV);
				polygonCreator.AddVertex();
			}

			if (!polygonMesh)
				polygonMesh = slFramework::SummonPolygonMesh();
			polygonMesh->AddPolygon(&polygonCreator, genNormals);

		}break;
		case 'o':
		case 'g':
		{
			if (!groupBegin)
				groupBegin = true;
			else
			{
				ptr = OBJNextLine(ptr);
				break;
			}

			/*std::string tmp_word;
			ptr = OBJReadWord(++ptr, tmp_word);
			if (tmp_word.size())
			{
				if (!name_word.size())
					name_word = tmp_word;
			}*/
			ptr = OBJReadWord(++ptr, tmp_word);
			if (tmp_word.size())
			{
				prev_word = curr_word;
				curr_word.assign(tmp_word.data());
			}

			if (grpFound)
			{
				if (polygonMesh->m_first_polygon)
				{
					slMaterial* m = 0;
					if (currMaterial)
					{
						/*m = g_sdk->CreateMaterial(currMaterial->m_name.data());
						if (currMaterial->m_map_diffuse.size())
							m->m_maps[m->mapSlot_Diffuse].m_texturePath = currMaterial->m_map_diffuse;*/
					}
					//g_sdk->CreateSceneObjectFromHelper(&importerHelper, prev_word.data(), m);
					cb->OnMesh(polygonMesh->CreateMesh(), &prev_word, 0);

					slDestroy(polygonMesh);
					polygonMesh = 0;
				}
			}
			grpFound = true;
		}break;
		default:
			++ptr;
			break;
		}
	}

	if (polygonMesh)
	{
		if (polygonMesh->m_first_polygon)
		{
			slMaterial* m = 0;
			if (currMaterial)
			{
				/*m = g_sdk->CreateMaterial(currMaterial->m_name.data());
				if (currMaterial->m_map_diffuse.size())
					m->m_maps[m->mapSlot_Diffuse].m_texturePath = currMaterial->m_map_diffuse;*/
			}

			cb->OnMesh(polygonMesh->CreateMesh(), &prev_word, 0);

			slDestroy(polygonMesh);
			polygonMesh = 0;
		}
	}

	for (uint32_t i = 0; i < obj_materials.m_size; ++i)
	{
		delete obj_materials.m_data[i];
	}
}
unsigned char* OBJNextLine(unsigned char* ptr)
{
	while (*ptr)
	{
		if (*ptr == '\n')
		{
			ptr++;
			return ptr;
		}
		ptr++;
	}
	return ptr;
}

unsigned char* OBJReadVec2(unsigned char* ptr, slVec2f& vec2)
{
	ptr = OBJSkipSpaces(ptr);
	float x, y;
	if (*ptr == '\n')
	{
		ptr++;
	}
	else
	{
		ptr = OBJReadFloat(ptr, x);
		ptr = OBJSkipSpaces(ptr);
		ptr = OBJReadFloat(ptr, y);
		ptr = OBJNextLine(ptr);
		vec2.x = x;
		vec2.y = y;
	}
	return ptr;
}

unsigned char* OBJSkipSpaces(unsigned char* ptr)
{
	while (*ptr)
	{
		if (*ptr != '\t' && *ptr != ' ')
			break;
		ptr++;
	}
	return ptr;
}

unsigned char* OBJReadFloat(unsigned char* ptr, float& value)
{
	char str[32u];
	memset(str, 0, 32);
	char* p = &str[0u];
	while (*ptr) {
		if (!isdigit(*ptr) && (*ptr != '-') && (*ptr != '+')
			&& (*ptr != 'e') && (*ptr != 'E') && (*ptr != '.')) break;
		*p = *ptr;
		++p;
		++ptr;
	}
	value = (float)atof(str);
	return ptr;
}

unsigned char* OBJReadVec3(unsigned char* ptr, slVec3f& vec3) {
	ptr = OBJSkipSpaces(ptr);
	float x, y, z;
	if (*ptr == '\n') {
		ptr++;
	}
	else {
		ptr = OBJReadFloat(ptr, x);
		ptr = OBJSkipSpaces(ptr);
		ptr = OBJReadFloat(ptr, y);
		ptr = OBJSkipSpaces(ptr);
		ptr = OBJReadFloat(ptr, z);
		ptr = OBJNextLine(ptr);
		vec3.x = x;
		vec3.y = y;
		vec3.z = z;
	}
	return ptr;
}

unsigned char* OBJSkipSpace(unsigned char* ptr) {
	while (*ptr) {
		if (*ptr != ' ' && *ptr != '\t') break;
		ptr++;
	}
	return ptr;
}

unsigned char* OBJGetInt(unsigned char* p, int& i)
{
	char str[8u];
	memset(str, 0, 8);
	char* pi = &str[0u];

	while (*p)
	{
		/*if( *p == '-' )
		{
		++p;
		continue;
		}*/

		if (!isdigit(*p) && *p != '-') break;


		*pi = *p;
		++pi;
		++p;
	}
	i = atoi(str);
	return p;
}

unsigned char* OBJReadFace(unsigned char* ptr, OBJFace& f, char* s) {
	ptr = OBJSkipSpaces(ptr);
	if (*ptr == '\n')
	{
		ptr++;
	}
	else
	{
		while (true)
		{
			int p = 1;
			int u = 1;
			int n = 1;

			ptr = OBJGetInt(ptr, p);

			if (*ptr == '/')
			{
				ptr++;
				if (*ptr == '/')
				{
					ptr++;
					f.ft = OBJFaceType::pn;
					ptr = OBJGetInt(ptr, n);
				}
				else
				{
					ptr = OBJGetInt(ptr, u);
					if (*ptr == '/')
					{
						ptr++;
						f.ft = OBJFaceType::pun;
						ptr = OBJGetInt(ptr, n);
					}
					else
					{
						f.ft = OBJFaceType::pu;
					}
				}
			}
			else
			{
				f.ft = OBJFaceType::p;
			}
			f.n.push_back(n - 1);
			f.u.push_back(u - 1);
			f.p.push_back(p - 1);
			ptr = OBJSkipSpace(ptr);

			if (*ptr == '\r')
				break;
			else if (*ptr == '\n')
				break;
		}
	}
	return ptr;
}

unsigned char* OBJReadWord(unsigned char* ptr, slStringA& str)
{
	ptr = OBJSkipSpaces(ptr);
	str.clear();
	while (*ptr)
	{
		if (isspace(*ptr))
			break;
		str.push_back(*ptr);
		ptr++;
	}
	return ptr;
}

unsigned char* OBJReadLastWord(unsigned char* ptr, slStringA& str) {
	while (true)
	{
		ptr = OBJSkipSpaces(ptr);
		ptr = OBJReadWord(ptr, str);

		ptr = OBJSkipSpaces(ptr);

		if (*ptr == '\r' || *ptr == '\n')
		{
			break;
		}

		if (*ptr == 0)
			break;
	}
	return ptr;
}
