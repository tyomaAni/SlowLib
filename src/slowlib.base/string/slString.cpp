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

#include "slowlib.h"

struct slUnicodeCharNode
{
	uint32_t m_utf8;
	uint32_t m_utf16;
};
static slUnicodeCharNode g_UnicodeChars[0x32000] =
{
#include "UnicodeChars.inl"
};


union UC
{
	uint8_t bytes[4];
	uint16_t shorts[2];
	uint32_t integer;
};

static uint8_t g_stringWordSize = 16;
namespace _internal
{
	const double string_to_float_table[17] =
	{
		0.f,
		0.1,
		0.01,
		0.001,
		0.0001,
		0.00001,
		0.000001,
		0.0000001,
		0.00000001,
		0.000000001,
		0.0000000001,
		0.00000000001,
		0.000000000001,
		0.0000000000001,
		0.00000000000001,
		0.000000000000001,
		0.0000000000000001,
	};
}

template<typename other_type>
size_t sl_string_getlen(const other_type* str)
{
	unsigned int len = 0u;
	if (str[0] != 0)
	{
		const other_type* p = &str[0u];
		while ((size_t)*p++)
			len++;
	}
	return len;
}

void slStringA::reallocate(size_t new_allocated)
{
	char8_t* new_data = (char8_t*)slMemory::malloc(new_allocated * sizeof(char8_t));
	if (m_data)
	{
		memcpy(new_data, m_data, m_size * sizeof(char8_t));
		slMemory::free(m_data);
	}
	else
	{
		memset(new_data, 0, new_allocated);
	}
	m_data = new_data;
	m_allocated = new_allocated;
}
slStringA::slStringA(){}
slStringA::~slStringA(){if (m_data) slMemory::free(m_data);}
void slStringA::push_back(char8_t c)
{
	size_t new_size = m_size + 1u;
	if ((new_size + 1u) > m_allocated)
		reallocate((new_size + 1u) + (1 + (size_t)(m_size * 0.5f)));
	m_data[m_size] = c;
	m_size = new_size;
	m_data[m_size] = 0;
}
void slStringW::reallocate(size_t new_allocated)
{
	char16_t* new_data = (char16_t*)slMemory::malloc(new_allocated * sizeof(char16_t));
	if (m_data)
	{
		memcpy(new_data, m_data, m_size * sizeof(char16_t));
		slMemory::free(m_data);
	}
	else
	{
		memset(new_data, 0, new_allocated);
	}
	m_data = new_data;
	m_allocated = new_allocated;
}
slStringW::slStringW() {}
slStringW::~slStringW() { if (m_data) slMemory::free(m_data); }
void slStringW::push_back(char16_t c)
{
	size_t new_size = m_size + 1u;
	if ((new_size + 1u) > m_allocated)
		reallocate((new_size + 1u) + (1 + (size_t)(m_size * 0.5f)));
	m_data[m_size] = c;
	m_size = new_size;
	m_data[m_size] = 0;
}


void slString::reallocate(size_t new_allocated)
{
	char32_t* new_data = (char32_t*)slMemory::malloc(new_allocated * sizeof(char32_t));
	if (m_data)
	{
		memcpy(new_data, m_data, m_size * sizeof(char32_t));
		slMemory::free(m_data);
	}
	else
	{
		memset(new_data, 0, new_allocated);
	}
	m_data = new_data;
	m_allocated = new_allocated;
}

slString::slString()
{
	m_allocated = g_stringWordSize;
	reallocate(m_allocated);
}

slString::slString(const char* s) : slString()
{
	assign(s);
}

slString::slString(const wchar_t* s) : slString()
{
	assign(s);
}

slString::slString(const char8_t* s) : slString()
{
	assign(s);
}

slString::slString(const char16_t* s) : slString()
{
	assign(s);
}

slString::slString(const char32_t* s) : slString()
{
	assign(s);
}

slString::slString(const slString& s) : slString()
{
	assign(s);
}

slString::slString(slString&& s) : slString()
{
	this->operator=(std::move(s));
}

slString::~slString()
{
	if (m_data)
		slMemory::free(m_data);
}

const char32_t* slString::c_str()
{
	return m_data;
}

size_t slString::size() const
{
	return m_size;
}

void slString::reserve(size_t size)
{
	if (size > m_allocated)
	{
		reallocate(size);
		m_data[m_size] = 0;
	}
}

void slString::clear()
{
	m_data[0] = 0;
	m_size = 0;
}

void slString::assign(const char* s)
{
	clear();
	append(s);
}

void slString::assign(const wchar_t* s)
{
	clear();
	append(s);
}

void slString::assign(const char8_t* s)
{
	clear();
	append(s);
}

void slString::assign(const char16_t* s)
{
	clear();
	append(s);
}

void slString::assign(const char32_t* s)
{
	clear();
	append(s);
}

void slString::assign(const slString& s)
{
	clear();
	append(s);
}

void slString::append(const char* str)
{
	size_t len = sl_string_getlen(str);

	unsigned char c1 = 0;
	unsigned char c2 = 0;
	unsigned char c3 = 0;
	unsigned char c4 = 0;
	for (size_t i = 0; i < len; ++i)
	{
		c1 = str[i];

		if (c1 <= 0x7F)
		{
			push_back((char32_t)c1);
		}
		else
		{
			if ((c1 & 0xE0) == 0xC0) //2 bytes
			{
				++i;
				if (i < len)
				{
					c2 = str[i];
					if ((c2 & 0xC0) == 0x80)
					{
						char16_t wch = (c1 & 0x1F) << 6;
						wch |= (c2 & 0x3F);
						push_back((char32_t)wch);
					}
				}
			}
			else if ((c1 & 0xF0) == 0xE0) //3
			{
				++i;
				if (i < len)
				{
					c2 = str[i];
					if ((c2 & 0xC0) == 0x80)
					{
						++i;
						if (i < len)
						{
							c3 = str[i];
							if ((c3 & 0xC0) == 0x80)
							{
								char16_t wch = (c1 & 0xF) << 12;
								wch |= (c2 & 0x3F) << 6;
								wch |= (c3 & 0x3F);
								push_back((char32_t)wch);
							}
						}
					}
				}
			}
			else if ((c1 & 0xF8) == 0xF0) //4
			{
				++i;
				if (i < len)
				{
					c2 = str[i];
					if ((c2 & 0xC0) == 0x80)
					{
						++i;
						if (i < len)
						{
							c3 = str[i];
							if ((c3 & 0xC0) == 0x80)
							{
								++i;
								if (i < len)
								{
									c4 = str[i];
									if ((c4 & 0xC0) == 0x80)
									{
										uint32_t u = (c1 & 0x7) << 18;
										u |= (c2 & 0x3F) << 12;
										u |= (c3 & 0x3F) << 6;
										u |= (c4 & 0x3F);

										push_back((char32_t)u);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void slString::append(const wchar_t* str)
{
	size_t len = sl_string_getlen(str);
	for (size_t i = 0u; i < len; ++i)
	{
		char16_t ch16 = str[i];
		if (ch16 < 0x80)
		{
			push_back((char32_t)ch16);
		}
		else if (ch16 < 0x800)
		{
			push_back((char32_t)ch16); //????
		}
		else if ((ch16 & 0xFC00) == 0xD800)
		{
			++i;
			if (i < len)
			{
				char16_t ch16_2 = str[i];
				if ((ch16_2 & 0xFC00) == 0xDC00)
				{
					uint32_t u = (ch16 - 0xD800) * 0x400;
					u += (ch16_2 - 0xDC00) + 0x10000;

					push_back((char32_t)u);
				}
			}
		}
		else
		{
			push_back((char32_t)ch16);
		}
	}
}

void slString::append(const char8_t* s)
{
	append((const char*)s);
}

void slString::append(const char16_t* s)
{
	append((const wchar_t*)s);
}

void slString::append(const char32_t* s)
{
	size_t len = sl_string_getlen(s);

	for (size_t i = 0; i < len; ++i)
	{
		push_back(s[i]);
	}
}

void slString::append(const slString& s)
{
	for (size_t i = 0; i < s.m_size; ++i)
	{
		push_back(s.m_data[i]);
	}
}

void slString::append(int8_t i)
{
	char buf[32u];
	int nob = sprintf_s(buf, 32, "%i", i);
	if (nob)
	{
		for (int i = 0; i < nob; ++i)
		{
			push_back((char32_t)buf[i]);
		}
	}
}

void slString::append(int16_t i)
{
	char buf[32u];
	int nob = sprintf_s(buf, 32, "%i", i);
	if (nob)
	{
		for (int i = 0; i < nob; ++i)
		{
			push_back((char32_t)buf[i]);
		}
	}
}

void slString::append(int32_t i)
{
	char buf[32u];
	int nob = sprintf_s(buf, 32, "%i", i);
	if (nob)
	{
		for (int i = 0; i < nob; ++i)
		{
			push_back((char32_t)buf[i]);
		}
	}
}

void slString::append(int64_t i)
{
	char buf[32u];
	int nob = sprintf_s(buf, 32, "%lli", i);
	if (nob)
	{
		for (int i = 0; i < nob; ++i)
		{
			push_back((char32_t)buf[i]);
		}
	}
}

void slString::append(uint8_t i)
{
	char buf[32u];
	int nob = sprintf_s(buf, 32, "%u", i);
	if (nob)
	{
		for (int i = 0; i < nob; ++i)
		{
			push_back((char32_t)buf[i]);
		}
	}
}

void slString::append(uint16_t i)
{
	char buf[32u];
	int nob = sprintf_s(buf, 32, "%u", i);
	if (nob)
	{
		for (int i = 0; i < nob; ++i)
		{
			push_back((char32_t)buf[i]);
		}
	}
}
void slString::append(uint32_t i)
{
	char buf[32u];
	int nob = sprintf_s(buf, 32, "%u", i);
	if (nob)
	{
		for (int i = 0; i < nob; ++i)
		{
			push_back((char32_t)buf[i]);
		}
	}
}
void slString::append(uint64_t i)
{
	char buf[32u];
	int nob = sprintf_s(buf, 32, "%llu", i);
	if (nob)
	{
		for (int i = 0; i < nob; ++i)
		{
			push_back((char32_t)buf[i]);
		}
	}
}
void slString::append(float f)
{
	char buf[32u];
	int nob = sprintf_s(buf, 32, "%.7f", f);
	if (nob)
	{
		for (int i = 0; i < nob; ++i)
		{
			push_back((char32_t)buf[i]);
		}
	}
}

void slString::append(double f)
{
	char buf[32u];
	int nob = sprintf_s(buf, 32, "%.12f", f);
	if (nob)
	{
		for (int i = 0; i < nob; ++i)
		{
			push_back((char32_t)buf[i]);
		}
	}
}

void slString::push_back(char c)
{
	push_back((char32_t)c);
}

void slString::push_back(wchar_t c)
{
	push_back((char32_t)c);
}

void slString::push_back(char8_t c)
{
	push_back((char32_t)c);
}

void slString::push_back(char16_t c)
{
	push_back((char32_t)c);
}

void slString::push_back(char32_t c)
{
	size_t new_size = m_size + 1u;
	if ((new_size + 1u) > m_allocated)
		reallocate((new_size + 1u) + (1 + (size_t)(m_size * 0.5f)));
	m_data[m_size] = c;
	m_size = new_size;
	m_data[m_size] = 0;
}

void slString::pop_back()
{
	if (m_size)
	{
		--m_size;
		m_data[m_size] = 0;
	}
}

char32_t slString::pop_back_return()
{
	char32_t r = '?';
	if (m_size)
	{
		--m_size;
		r = m_data[m_size];
		m_data[m_size] = 0;
	}
	return r;
}

void slString::pop_back_before(char32_t before_this)
{
	if (size())
		pop_back();

	if (size())
	{
		for (size_t i = size() - 1u; i >= 0u; --i)
		{
			if (m_data[i] == before_this)
				break;
			else
				pop_back();
			if (!i)
				break;
		}
	}
}

void slString::pop_front()
{
	erase(0u, 0u);
}

void slString::replace(char32_t a, char32_t b)
{
	for (size_t i = 0; i < m_size; ++i)
	{
		if (m_data[i] == a)
			m_data[i] = b;
	}
}

void slString::shrink_to_fit()
{
	if (m_size)
	{
		if (m_allocated > (m_size + (1 + (uint32_t)(m_size * 0.5f))))
		{
			reallocate(m_size + 1u);
			m_data[m_size] = 0;
		}
	}
}

void slString::erase(size_t begin, size_t end)
{
	size_t numCharsToDelete = end - begin + 1u; // delete first char: 0 - 0 + 1
	size_t next = end + 1u;
	for (size_t i = begin; i < m_size; ++i)
	{
		if (next < m_size)
		{
			m_data[i] = m_data[next];
			++next;
		}
	}
	m_size -= numCharsToDelete;
	m_data[m_size] = 0;
}

void slString::insert(const char32_t* str, size_t where)
{
	size_t str_len = sl_string_getlen(str);
	size_t new_size = str_len + m_size;
	if ((new_size + 1u) > m_allocated)
		reallocate((new_size + 1u) + (1 + (uint32_t)(m_size * 0.5f)));

	size_t i = m_size;
	while (i >= where)
	{
		m_data[i + str_len] = m_data[i];
		if (i == 0)
			break;
		--i;
	}
	for (size_t i = 0; i < str_len; ++i)
	{
		m_data[i + where] = str[i];
	}
	m_size = new_size;
	m_data[m_size] = 0;
}

// Only for ASCII
void slString::trim_spaces()
{
	while (true)
	{
		if (isspace(m_data[0]))
			pop_front();
		else
			break;
	}

	while (true)
	{
		if (isspace(m_data[m_size - 1]))
			pop_back();
		else
			break;
	}
}

void slString::to_lower()
{
	if (!m_size)
		return;
	for (size_t i = 0u; i < m_size; ++i)
	{
		auto c = m_data[i];
		if (c <= 'Z' && c >= 'A')
			m_data[i] += 32;
	}
}

void slString::to_upper()
{
	if (!m_size)
		return;
	for (size_t i = 0u; i < m_size; ++i)
	{
		auto c = m_data[i];
		if (c <= 'z' && c >= 'a')
			m_data[i] -= 32;
	}
}

void slString::flip()
{
	if (!m_size)
		return;
	slString flippedStr;
	for (size_t i = m_size - 1u; i >= 0u; --i)
	{
		flippedStr += m_data[i];
		if (!i)
			break;
	}
	this->operator=(flippedStr);
}

void slString::flip_slash()
{
	if (!m_size)
		return;
	for (size_t i = 0u; i < m_size; ++i)
	{
		if (m_data[i] == '\\')
			m_data[i] = '/';
	}
}

void slString::flip_backslash()
{
	if (!m_size)
		return;
	for (size_t i = 0u; i < m_size; ++i)
	{
		if (m_data[i] == '/')
			m_data[i] = '\\';
	}
}

int32_t slString::to_int()
{
	size_t len = m_size;
	int32_t result = 0;
	int mul_val = 1;
	bool is_neg = m_data[0] == U'-';
	for (size_t i = 0, last = len - 1; i < len; ++i)
	{
		int char_value = (int)m_data[last] - 0x30;
		if (char_value >= 0 && char_value <= 9)
		{
			result += char_value * mul_val;
			mul_val *= 10;
		}
		--last;
	}
	if (is_neg)
		result = -result;
	return result;
}

int64_t slString::to_intll()
{
	size_t len = m_size;
	int64_t result = 0;
	int mul_val = 1;
	bool is_neg = m_data[0] == U'-';
	for (size_t i = 0, last = len - 1; i < len; ++i)
	{
		int char_value = (int)m_data[last] - 0x30;
		if (char_value >= 0 && char_value <= 9)
		{
			result += char_value * mul_val;
			mul_val *= 10;
		}
		--last;
	}
	if (is_neg)
		result = -result;
	return result;
}

uint32_t slString::to_uint()
{
	size_t len = m_size;
	uint32_t result = 0;
	int mul_val = 1;
	bool is_neg = m_data[0] == U'-';
	for (size_t i = 0, last = len - 1; i < len; ++i)
	{
		int char_value = (int)m_data[last] - 0x30;
		if (char_value >= 0 && char_value <= 9)
		{
			result += char_value * mul_val;
			mul_val *= 10;
		}
		--last;
	}
	return result;
}

uint64_t slString::to_uintll()
{
	size_t len = m_size;
	uint64_t result = 0;
	int mul_val = 1;
	bool is_neg = m_data[0] == U'-';
	for (size_t i = 0, last = len - 1; i < len; ++i)
	{
		int char_value = (int)m_data[last] - 0x30;
		if (char_value >= 0 && char_value <= 9)
		{
			result += char_value * mul_val;
			mul_val *= 10;
		}
		--last;
	}
	return result;
}

float slString::to_float()
{
	float result = 0.f;
	auto ptr = m_data;
	bool is_negative = *ptr == U'-';

	if (is_negative)
		++ptr;

	int i = 0;
	while (*ptr >= U'0' && *ptr <= U'9')
	{
		i *= 10;
		i += *ptr - U'0';
		++ptr;
	}
	result = (float)i;

	i = 0;

	if (*ptr == U'.')
		++ptr;

	int part_2_count = 0;
	while (*ptr >= U'0' && *ptr <= U'9')
	{
		i *= 10;
		i += *ptr - U'0';
		++ptr;
		++part_2_count;
	}
	result += (float)i * (float)_internal::string_to_float_table[part_2_count];

	return is_negative ? -result : result;
}

double slString::to_double()
{
	double result = 0.f;
	auto ptr = m_data;
	bool is_negative = *ptr == U'-';

	if (is_negative)
		++ptr;

	int i = 0;
	while (*ptr >= U'0' && *ptr <= U'9')
	{
		i *= 10;
		i += *ptr - U'0';
		++ptr;
	}
	result = (double)i;

	i = 0;

	if (*ptr == U'.')
		++ptr;

	int part_2_count = 0;
	while (*ptr >= U'0' && *ptr <= U'9')
	{
		i *= 10;
		i += *ptr - U'0';
		++ptr;
		++part_2_count;
	}
	result += (double)i * _internal::string_to_float_table[part_2_count];

	return is_negative ? -result : result;
}

slString& slString::operator=(const char* s)
{
	clear();
	append(s);
	return *this;
}

slString& slString::operator=(const wchar_t* s)
{
	clear();
	append(s);
	return *this;
}

slString& slString::operator=(const char8_t* s)
{
	clear();
	append(s);
	return *this;
}

slString& slString::operator=(const char16_t* s)
{
	clear();
	append(s);
	return *this;
}

slString& slString::operator=(const char32_t* s)
{
	clear();
	append(s);
	return *this;
}

slString& slString::operator=(const slString& s)
{
	clear();
	append(s);
	return *this;
}

slString& slString::operator=(slString&& s)
{
	if(m_data)
		slMemory::free(m_data);
	m_data = s.m_data;
	m_allocated = s.m_allocated;
	m_size = s.m_size;

	s.m_data = 0;
	s.m_allocated = 0;
	s.m_size = 0;
	return *this;
}

const char32_t& slString::operator[](size_t i) const
{
	return m_data[i];
}

char32_t& slString::operator[](size_t i)
{
	return m_data[i];
}

const char32_t& slString::at(size_t i) const
{
	return m_data[i];
}

char32_t& slString::at(size_t i)
{
	return m_data[i];
}

void slString::operator+=(const char* s)
{
	append(s);
}

void slString::operator+=(const wchar_t* s)
{
	append(s);
}

void slString::operator+=(const char8_t* s)
{
	append(s);
}

void slString::operator+=(const char16_t* s)
{
	append(s);
}

void slString::operator+=(const char32_t* s)
{
	append(s);
}

void slString::operator+=(int8_t i)
{
	append(i);
}

void slString::operator+=(int16_t i)
{
	append(i);
}

void slString::operator+=(int32_t i)
{
	append(i);
}

void slString::operator+=(int64_t i)
{
	append(i);
}

void slString::operator+=(uint8_t i)
{
	append(i);
}

void slString::operator+=(uint16_t i)
{
	append(i);
}

void slString::operator+=(uint32_t i)
{
	append(i);
}

void slString::operator+=(uint64_t i)
{
	append(i);
}

void slString::operator+=(float f)
{
	append(f);
}

void slString::operator+=(double f)
{
	append(f);
}

bool slString::operator==(const slString& other) const
{
	if (other.size() != m_size)
		return false;

	const size_t sz = other.size();
	const auto* ptr = other.m_data;
	for (size_t i = 0u; i < sz; ++i)
	{
		if (ptr[i] != m_data[i])
			return false;
	}

	return true;
}

bool slString::operator!=(const slString& other) const
{
	if (other.size() != m_size)
		return true;

	const size_t sz = other.size();
	const auto* ptr = other.m_data;
	for (size_t i = 0u; i < sz; ++i)
	{
		if (ptr[i] != m_data[i])
			return true;
	}

	return false;
}

void slString::to_utf8(slStringA& stra)
{
	UC uc;
	auto ut = &g_UnicodeChars[0];
	for (size_t i = 0; i < m_size; ++i)
	{
		char32_t c = m_data[i];
		if (c >= 0x32000)
			c = '?';

		uc.integer = ut[c].m_utf8;

		if (uc.bytes[3]) stra.push_back(uc.bytes[3]);
		if (uc.bytes[2]) stra.push_back(uc.bytes[2]);
		if (uc.bytes[1]) stra.push_back(uc.bytes[1]);
		if (uc.bytes[0]) stra.push_back(uc.bytes[0]);
	}
}

void slString::to_utf16(slStringW& strw)
{
	UC uc;
	auto ut = &g_UnicodeChars[0];
	for (size_t i = 0; i < m_size; ++i)
	{
		char32_t c = m_data[i];
		if (c >= 0x32000)
			c = '?';

		uc.integer = ut[c].m_utf16;

		if (uc.shorts[1]) strw.push_back(uc.shorts[1]);
		if (uc.shorts[0]) strw.push_back(uc.shorts[0]);
	}
}