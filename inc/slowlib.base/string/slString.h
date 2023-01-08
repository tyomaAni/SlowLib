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
#ifndef __SL_SLOWLIBBASESTRING_H__
#define __SL_SLOWLIBBASESTRING_H__

// just data and size
// it will use in slString
class slStringA
{
	void reallocate(size_t new_allocated);
public:
	slStringA();
	~slStringA();
	slStringA(const char*);
	slStringA(const char8_t*);
	slStringA(const slStringA&);
	slStringA(slStringA&&);
	void assign(const slStringA&);
	void assign(const char*);
	void assign(const char8_t*);
	void append(const slStringA&);
	void append(const char*);
	void append(const char8_t*);
	void push_back(char8_t);
	void clear();
	const char8_t& operator[](size_t i) const { return m_data[i]; }
	char8_t& operator[](size_t i) { return m_data[i]; }
	size_t size() const { return m_size; }
	char* data() { return (char*)m_data; }
	const char* c_str() { return (const char*)m_data; }
	bool operator==(const slStringA& other) const {
		if (other.size() != m_size)
			return false;
		const size_t sz = other.size();
		const auto* ptr = other.m_data;
		for (size_t i = 0u; i < sz; ++i){
			if (ptr[i] != m_data[i])
				return false;
		}
		return true;
	}
	bool operator!=(const slStringA& other) const {
		if (other.size() != m_size)
			return true;
		const size_t sz = other.size();
		const auto* ptr = other.m_data;
		for (size_t i = 0u; i < sz; ++i){
			if (ptr[i] != m_data[i])
				return true;
		}
		return false;
	}
	slStringA& operator=(const slStringA&);
	slStringA& operator=(slStringA&&);
	size_t m_allocated = 0;
	size_t m_size = 0;
	char8_t* m_data = nullptr;
};
class slStringW
{
	void reallocate(size_t new_allocated);
public:
	slStringW();
	~slStringW();
	slStringW(const slStringW&);
	slStringW(slStringW&&);
	void assign(const slStringW&);
	void append(const slStringW&);
	void push_back(char16_t);
	void clear();
	const char16_t& operator[](size_t i) const { return m_data[i]; }
	char16_t& operator[](size_t i) { return m_data[i]; }
	size_t size() const { return m_size; }
	bool operator==(const slStringW& other) const {
		if (other.size() != m_size)
			return false;
		const size_t sz = other.size();
		const auto* ptr = other.m_data;
		for (size_t i = 0u; i < sz; ++i) {
			if (ptr[i] != m_data[i])
				return false;
		}
		return true;
	}
	bool operator!=(const slStringW& other) const {
		if (other.size() != m_size)
			return true;
		const size_t sz = other.size();
		const auto* ptr = other.m_data;
		for (size_t i = 0u; i < sz; ++i) {
			if (ptr[i] != m_data[i])
				return true;
		}
		return false;
	}
	slStringW& operator=(const slStringW&);
	slStringW& operator=(slStringW&&);
	size_t m_allocated = 0;
	size_t m_size = 0;
	char16_t* m_data = nullptr;
};

// Unicode string.
class slString
{
	size_t m_size = 0;
	size_t m_allocated = 0;
	char32_t* m_data = nullptr;

	void reallocate(size_t new_allocated);
public:
	slString();
	slString(const char*);
	slString(const wchar_t*);
	slString(const char8_t*);
	slString(const char16_t*);
	slString(const char32_t*);
	slString(const slString&);
	slString(slString&&);
	~slString();
	
	const char32_t* c_str()const;
	size_t size() const;
	void reserve(size_t size);
	void clear();

	void assign(const char*);
	void assign(const wchar_t*);
	void assign(const char8_t*);
	void assign(const char16_t*);
	void assign(const char32_t*);
	void assign(const slString&);

	void append(const char*);
	void append(const wchar_t*);
	void append(const char8_t*);
	void append(const char16_t*);
	void append(const char32_t*);
	void append(const slString&);
	void append(int8_t);
	void append(int16_t);
	void append(int32_t);
	void append(int64_t);
	void append(uint8_t);
	void append(uint16_t);
	void append(uint32_t);
	void append(uint64_t);
	void append_hex(uint64_t);
	void append(float);
	void append(double);

	void push_back(char);
	void push_back(wchar_t);
	void push_back(char8_t);
	void push_back(char16_t);
	void push_back(char32_t);

	void pop_back();
	char32_t pop_back_return();
	void pop_back_before(char32_t before_this);
	void pop_front();
	void replace(char32_t a, char32_t b);
	void shrink_to_fit();

	void erase(size_t begin, size_t end);
	void insert(const char32_t* str, size_t where);
	void trim_spaces();

	// Only for ASCII
	void to_lower();
	void to_upper();
	void flip();
	void flip_slash();
	void flip_backslash();
	int32_t to_int();
	int64_t to_intll();
	uint32_t to_uint();
	uint64_t to_uintll();
	float to_float();
	double to_double();

	slString& operator=(const char*);
	slString& operator=(const wchar_t*);
	slString& operator=(const char8_t*);
	slString& operator=(const char16_t*);
	slString& operator=(const char32_t*);
	slString& operator=(const slString&);
	slString& operator=(slString&&);

	const char32_t& operator[](size_t i) const;
	char32_t& operator[](size_t i);
	const char32_t& at(size_t i) const;
	char32_t& at(size_t i);

	void operator+=(const char*);
	void operator+=(const wchar_t*);
	void operator+=(const char8_t*);
	void operator+=(const char16_t*);
	void operator+=(const char32_t*);
	void operator+=(int8_t);
	void operator+=(int16_t);
	void operator+=(int32_t);
	void operator+=(int64_t);
	void operator+=(uint8_t);
	void operator+=(uint16_t);
	void operator+=(uint32_t);
	void operator+=(uint64_t);
	void operator+=(float);
	void operator+=(double);

	slString operator+(const char*);
	slString operator+(const wchar_t*);
	slString operator+(const char8_t*);
	slString operator+(const char16_t*);
	slString operator+(const char32_t*);
	slString operator+(int8_t);
	slString operator+(int16_t);
	slString operator+(int32_t);
	slString operator+(int64_t);
	slString operator+(uint8_t);
	slString operator+(uint16_t);
	slString operator+(uint32_t);
	slString operator+(uint64_t);
	slString operator+(float);
	slString operator+(double);

	bool operator==(const slString& other) const;
	bool operator!=(const slString& other) const;

	void to_utf8(slStringA&) const;
	void to_utf16(slStringW&) const;
};

#endif
