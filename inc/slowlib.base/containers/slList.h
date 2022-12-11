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
#ifndef __SL_SLOWLIBBASELIST_H__
#define __SL_SLOWLIBBASELIST_H__

template<typename _type>
struct slListNode
{
	slListNode() :m_left(0), m_right(0) {}
	~slListNode() {}
	_type m_data;
	slListNode* m_left;
	slListNode* m_right;
};

// circular double linked list
template<typename _type>
class slList
{
	slList(const slList& other) {};
	slList(slList&& other) {};
public:
	slList() :m_head(0) {}
	~slList() {
		clear();
	}

	slListNode<_type>* find(const _type& data)
	{
		if (!m_head)
			return 0;

		auto node = m_head;
		auto last = m_head->m_left;
		while (true)
		{
			if (node->m_data == data)
				return node;

			if (node == last)
				break;
			node = node->m_right;
		}
		return 0;
	}

	void clear()
	{
		if (!m_head)
			return;
		auto last = m_head->m_left;
		while (true)
		{
			auto next = m_head->m_right;
			m_head->~slListNode();
			slMemory::free(m_head);

			if (m_head == last)
				break;
			m_head = next;
		}
		m_head = nullptr;
	}


	slListNode<_type>* insert_after(const _type& after_this, const _type& data) {
		slListNode<_type>* node = (slListNode<_type>*)slMemory::malloc(sizeof(slListNode<_type>));
		new(node)slListNode<_type>();

		node->m_data = data;

		if (!m_head)
		{
			m_head = node;
			m_head->m_right = m_head;
			m_head->m_left = m_head;
		}
		else
		{
			auto c = m_head;
			auto l = c->m_left;
			while (true)
			{
				if (c->m_data == after_this)
				{
					auto r = c->m_right;

					node->m_left = c;
					node->m_right = r;

					c->m_right = node;
					r->m_left = node;

					return node;
				}

				if (c == l)
					break;
				c = c->m_right;
			}

			auto last = m_head->m_left;
			last->m_right = node;
			node->m_left = last;
			node->m_right = m_head;
			m_head->m_left = node;
		}
		return node;
	}

	slListNode<_type>* insert_before(const _type& after_this, const _type& data) 
	{
		slListNode<_type>* node = (slListNode<_type>*)slMemory::malloc(sizeof(slListNode<_type>));
		new(node)slListNode<_type>();

		node->m_data = data;

		if (!m_head)
		{
			m_head = node;
			m_head->m_right = m_head;
			m_head->m_left = m_head;
		}
		else
		{
			auto c = m_head;
			auto l = c->m_left;
			while (true)
			{
				if (c->m_data == after_this)
				{
					auto l = c->m_left;

					node->m_left = l;
					node->m_right = c;

					c->m_left = node;
					l->m_right = node;

					return node;
				}

				if (c == l)
					break;
				c = c->m_right;
			}

			auto last = m_head->m_left;
			last->m_right = node;
			node->m_left = last;
			node->m_right = m_head;
			m_head->m_left = node;
		}
		return node;
	}

	slListNode<_type>* push_back(const _type& data)
	{
		slListNode<_type>* node = (slListNode<_type>*)slMemory::malloc(sizeof(slListNode<_type>));
		new(node)slListNode<_type>();

		node->m_data = data;

		if (!m_head)
		{
			m_head = node;
			m_head->m_right = m_head;
			m_head->m_left = m_head;
		}
		else
		{
			auto last = m_head->m_left;
			last->m_right = node;
			node->m_left = last;
			node->m_right = m_head;
			m_head->m_left = node;
		}
		return node;
	}

	slListNode<_type>* push_front(const _type& data)
	{
		slListNode<_type>* node = (slListNode<_type>*)slMemory::malloc(sizeof(slListNode<_type>));
		new(node)slListNode<_type>();

		node->m_data = data;

		if (!m_head)
		{
			m_head = node;
			m_head->m_right = m_head;
			m_head->m_left = m_head;
		}
		else
		{
			node->m_right = m_head;
			node->m_left = m_head->m_left;
			m_head->m_left->m_right = node;
			m_head->m_left = node;
			m_head = node;
		}
		return node;
	}

	void pop_front()
	{
		if (!m_head)
			return;
		auto next = m_head->m_right;
		auto last = m_head->m_left;
		m_head->~slListNode();
		slMemory::free(m_head);

		if (next == m_head)
		{
			m_head = nullptr;
			return;
		}
		m_head = next;
		next->m_left = last;
		last->m_right = next;
	}

	void pop_back()
	{
		if (!m_head)
			return;

		auto lastNode = m_head->m_left;
		lastNode->m_left->m_right = m_head;
		m_head->m_left = lastNode->m_left;

		lastNode->~slListNode();
		slMemory::free(lastNode);

		if (lastNode == m_head)
		{
			m_head = nullptr;
		}
	}

	// maybe not correct
	void erase(slListNode<_type>* node) {
		auto l = node->m_left;
		auto r = node->m_right;
		l->m_right = r;
		r->m_left = l;
		node->~slListNode();
		slMemory::free(node);
		if (node == m_head)
			m_head = 0;
	}

	bool erase_first(const _type& object) {
		if (!m_head)
			return false;

		auto node = m_head;
		auto last = m_head->m_left;
		while (true)
		{
			auto next = node->m_right;
			if (node->m_data == object)
			{
				if (node == m_head)
					pop_front();
				else
				{
					node->m_left->m_right = node->m_right;
					node->m_right->m_left = node->m_left;
					node->~slListNode();
					slMemory::free(node);

					// ???
					if (node == m_head)
						m_head = 0;
				}
				return true;
			}
			if (node == last)
				break;
			node = next;
		}
		return false;
	}

	void replace(const _type& oldObject, const _type& newObject) {
		if (!m_head)
			return;

		auto node = m_head;
		auto last = m_head->m_left;
		while (true)
		{
			auto next = node->m_right;
			if (node->m_data == oldObject)
			{
				node->m_data = newObject;
				return;
			}
			if (node == last)
				break;
			node = next;
		}
	}

	// ???
	// hard to understand but it works
	void reverse()
	{
		if (!m_head)
			return;
		slListNode<_type>* tail = m_head->m_left;
		slListNode<_type>* curr = m_head;
		while (true)
		{
			auto l = curr->m_left;
			auto r = curr->m_right;

			curr->m_left = r;
			curr->m_right = l;

			if (curr == tail)
				break;
			curr = r;
		}
		m_head = tail;
	}

	void erase_by_node(slListNode<_type>* object)
	{
		if (!m_head)
			return;

		object->m_left->m_right = object->m_right;
		object->m_right->m_left = object->m_left;

		if (object == m_head)
			m_head = m_head->m_right;

		if (object == m_head)
			m_head = 0;

		object->~slListNode();
		slMemory::free(object);
	}

	class Iterator
	{
		//	friend class ConstIterator;

		slListNode<_type>* m_node;
		slListNode<_type>* m_nodeEnd;
		bool m_isEnd;
	public:
		Iterator() :m_node(0), m_isEnd(true) {}
		Iterator(slListNode<_type>* head) :m_node(head), m_isEnd(false)
		{
			if (!head)
			{
				m_isEnd = true;
			}
			else
			{
				m_nodeEnd = head->m_left;
			}
		}
		~Iterator() {}

		Iterator& operator ++() {
			if (m_node == m_nodeEnd) {
				m_isEnd = true;
			}
			m_node = m_node->m_right;
			return *this;
		}

		bool operator ==(const Iterator& other) const {
			if (m_isEnd != other.m_isEnd) return false;
			return true;
		}
		bool operator !=(const Iterator& other) const {
			if (m_isEnd != other.m_isEnd) return true;
			return false;
		}
		const _type& operator*() {
			return m_node->m_data;
		}
		_type* operator->() {
			return m_node->m_data;
		}
	};

	Iterator begin() {
		return Iterator(m_head);
	}
	Iterator end() {
		return Iterator();
	}

	slListNode<_type>* m_head;
};

#endif
