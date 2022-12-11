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
#ifndef __SL_SLOWLIBBASEARRAY_H__
#define __SL_SLOWLIBBASEARRAY_H__

// `clear` must do m_size = 0;
template<typename type>
class slArray
{
	uint32_t m_allocated;
	void reallocate(uint32_t new_capacity)
	{
		new_capacity += (uint32_t)ceilf((float)m_allocated * 0.5f);
		auto tmp_size = new_capacity * sizeof(type);
		pointer new_data = static_cast<type*>(slMemory::malloc(tmp_size));
		memset(new_data, 0, tmp_size);

		if (m_data)
		{
			for (uint32_t i = 0u; i < m_size; ++i)
			{
				new(&new_data[i]) type(m_data[i]);
				(&m_data[i])->~type();
			}
			slMemory::free(m_data);
		}
		m_data = new_data;
		m_allocated = new_capacity;
	}
public:
	typedef type* pointer;
	typedef type& reference;
	typedef const type& const_reference;

	slArray() 
		:
		m_allocated(0), 
		m_size(0), 
		m_data(0) 
	{}

	// OK?
	/*slArray(const slArray& other)
		:
		slArray()
	{
		m_allocated = other.m_allocated;
		m_size = other.m_size;

		if (m_allocated)
			reallocate(m_allocated);

		if (m_size)
		{
			for (uint32_t i = 0u; i < m_size; ++i)
			{
				new(&m_data[i]) type(other.m_data[i]);
				m_data[i] = other.m_data[i];
			}
		}
	}*/

	~slArray() 
	{
		free_memory(); 
	}

	const_reference operator[](uint32_t id) const { return m_data[id]; }
	reference       operator[](uint32_t id) { return m_data[id]; }

	uint32_t capacity() 
	{
		return m_allocated;
	}

	uint32_t size() 
	{
		return m_size;
	}

	void shrink_to_fit()
	{
		auto new_capacity = m_size;
		auto tmp_size = new_capacity * sizeof(type);
		pointer new_data = static_cast<type*>(slMemory::malloc(tmp_size));
		memset(new_data, 0, tmp_size);

		if (m_data)
		{
			for (uint32_t i = 0u; i < m_size; ++i)
			{
				new(&new_data[i]) type(m_data[i]);
				(&m_data[i])->~type();
			}
			slMemory::free(m_data);
		}
		m_data = new_data;
		m_allocated = new_capacity;
	}

	void reserve(uint32_t new_capacity)
	{
		if (new_capacity > m_allocated)
			reallocate(new_capacity);
	}

	void insert(uint32_t where, const_reference object)
	{
		if (where >= m_size)
		{
			push_back(object);
		}
		else
		{
			uint32_t new_size = m_size + 1u;
			if (new_size > m_allocated)
				reallocate(new_size);

			for (uint32_t i = m_size; i > where;)
			{
				m_data[i] = m_data[i - 1];
				--i;
			}
			new(&m_data[where]) type(object);

			m_size = new_size;
		}
	}

	void push_back(const_reference object)
	{
		uint32_t new_size = m_size + 1u;
		if (new_size > m_allocated)
			reallocate(new_size);
		new(&m_data[m_size]) type(object);
		m_size = new_size;
	}

	void push_front(const_reference object)
	{
		insert(0, object);
	}

	void free_memory()
	{
		if (m_data)
		{
			for (uint32_t i = 0u; i < m_allocated; ++i)
			{
				(&m_data[i])->~type();
			}
			slMemory::free(m_data);

			m_allocated = m_size = 0u;
			m_data = nullptr;
		}
	}

	void clear()
	{
		m_size = 0;
	}

	type* get(uint32_t index)
	{
		if (index >= m_allocated)
			return nullptr;

		return &m_data[index];
	}

	/*
	struct _pred
	{
	bool operator() (const Map::renderNode& a, const Map::renderNode& b) const
	{
	return a.m_position.y > b.m_position.y;
	}
	};
	m_renderSprites.sort_insertion(_pred());
	*/
	template<class _Pr>
	void sort_insertion(_Pr _pred)
	{
		uint32_t i, j;
		type t;
		for (i = 1; i < m_size; ++i)
		{
			for (j = i; j > 0 && _pred(m_data[j - 1], m_data[j]); --j)
			{
				t = m_data[j];
				m_data[j] = m_data[j - 1];
				m_data[j - 1] = t;
			}
		}
	}

	void erase(uint32_t index)
	{
		for (uint32_t i = index; i < m_size; ++i)
		{
			if (i + 1 < m_size)
				m_data[i] = m_data[i + 1];
			else
				m_data[i] = 0;
		}
		if (m_size)
			--m_size;
	}

	void erase_first(const_reference ref)
	{
		for (uint32_t i = 0u; i < m_size; ++i)
		{
			if (m_data[i] == ref)
			{
				erase(i);
				return;
			}
		}
	}

	pointer m_data;
	uint32_t     m_size;
};

#endif
