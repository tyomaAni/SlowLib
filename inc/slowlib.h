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
#ifndef __SL_SLOWLIB_H__
#define __SL_SLOWLIB_H__

#include "slowlib.base/common/slowlib.base.defs.h"
#include "slowlib.base/common/slowlib.base.forward.h"
#include "slowlib.base/memory/slowlib.base.memory.h"
#include "slowlib.base/string/slowlib.base.string.h"
#include "slowlib.base/unicode/slowlib.base.unicode.converter.h"
#include "slowlib.base/common/slowlib.base.assert.h"
#include "slowlib.base/system/slowlib.base.dll.h"

// test stacktrace
class hkStackTracer
{
public:

	/// Stores multiple call stacks in a tree to save space.
	/// Call stacks are inserted and return a TraceId which can be
	/// later used to retrieve the original call stack.
	/// For this to work optimally, the full call stack should be used
	/// so that identical prefixes can be merged and the tree does not
	/// become multi-rooted.
	class CallTree {
	public:
		typedef int TraceId;

		CallTree(){}
		CallTree(const CallTree& rhs);
		void operator=(const CallTree& rhs);
		~CallTree();
		void init() {}
		void quit();

		TraceId insertCallStack(hkStackTracer& tracer);
		TraceId insertCallStack(const uint32_t* addrs, int numAddrs);
		void releaseCallStack(TraceId id);
		int getCallStack(TraceId id, uint32_t* addrs, int maxAddrs) const;
		int getCallStackSize(TraceId id) const;
		//void getTraces(hkArray<uint32_t>& addrs, hkArray<int>& parents) const;
		void dumpTrace(TraceId id) const;
		void swap(CallTree& rhs);
		//const hkArrayBase<Node>& getNodes() const { return m_nodes; }
	protected:
		int getFreeNode();
		void removeNodeIfUnused(int id);
	protected:
	};

	hkStackTracer();
	virtual ~hkStackTracer();
	//typedef void (HK_CALL* printFunc)(const char*, void* context);

	void dumpStackTrace(const uint32_t* trace, int numtrace) const;
	int getStackTrace(uint32_t* trace, int maxtrace, int framesToSkip = 0);
	void refreshSymbols();
	void getModuleInfo() const;
	static void* getImplementation();
	static void replaceImplementation(void* newInstance);
	static void setNeedsSymInitialize(bool enabled);
};

#endif
