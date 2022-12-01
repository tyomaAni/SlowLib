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

#include <stdio.h>

#ifdef SL_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <dbghelp.h>
typedef BOOL(__stdcall* tSymInitialize)(HANDLE hProcess, PSTR UserSearchPath, IN BOOL fInvadeProcess);
typedef BOOL(__stdcall* tSymRefreshModuleList)(IN HANDLE hProcess);
typedef DWORD(__stdcall* tSymGetOptions)(VOID);
typedef DWORD(__stdcall* tSymSetOptions)(IN DWORD SymOptions);
typedef BOOL(__stdcall* tSymFromAddr)(IN HANDLE hProcess, IN DWORD64 dwAddr, OUT PDWORD64 pdwDisplacement, OUT PSYMBOL_INFO Symbol);
typedef BOOL(__stdcall* tStackWalk64)(DWORD MachineType, HANDLE hProcess, HANDLE hThread, LPSTACKFRAME64 StackFrame, PVOID ContextRecord, PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine, PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine, PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine, PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress);
typedef PVOID(__stdcall* tSymFunctionTableAccess64)(HANDLE hProcess, DWORD64 AddrBase);
typedef DWORD64(__stdcall* tSymGetModuleBase64)(IN HANDLE hProcess, IN DWORD64 dwAddr);
typedef BOOL(__stdcall* tSymGetLineFromAddr64)(IN HANDLE hProcess, IN DWORD64 dwAddr, OUT PDWORD64 pdwDisplacement, OUT PIMAGEHLP_LINE64 Line);
typedef BOOL(__stdcall* tSymCleanup)(IN HANDLE hProcess);
typedef void(__stdcall* tRtlCaptureContext)(OUT PCONTEXT context);
typedef USHORT(__stdcall* tRtlCaptureStackBackTrace)(IN ULONG FramesToSkip, IN ULONG FramesToCapture, OUT PVOID* BackTrace, OUT PULONG BackTraceHash);
#endif

// Need to rewrite it because I just changed names.
// Original code taken from Havok

class slStackTracerImpl
{
public:
	slStackTracerImpl();
	~slStackTracerImpl();

	void ensureInitialized();

	void dumpStackTrace(const size_t* trace, int numtrace);
	int  getStackTrace(size_t* trace, int maxtrace, int framesToSkip = 0);

	CRITICAL_SECTION m_lock;
	bool m_lockInitialized = false;
	bool m_initialized = false;
	HINSTANCE hDbgHelpDll;
	HINSTANCE hKernel32Dll;
	tSymInitialize pSymInitialize;
	tSymRefreshModuleList pSymRefreshModuleList;
	tSymGetOptions pSymGetOptions;
	tSymSetOptions pSymSetOptions;
	tSymFromAddr pSymFromAddr;
	tStackWalk64 pStackWalk64;
	tSymFunctionTableAccess64 pSymFunctionTableAccess64;
	tSymGetModuleBase64 pSymGetModuleBase64;
	tSymGetLineFromAddr64 pSymGetLineFromAddr64;
	tRtlCaptureContext pRtlCaptureContext;
	tRtlCaptureStackBackTrace pRtlCaptureStackBackTrace;
};

#define NATIVE_WARN_IF(A, T) if (A) { OutputDebugStringA("StackTracer: " T "\r\n"); } else
#define LOAD_FUNCTION(A) if(1) { p##A = (t##A) GetProcAddress(hDbgHelpDll, #A); } else
#define LOAD_FUNCTION_WARN(A) if(1) { p##A = (t##A) GetProcAddress(hDbgHelpDll, #A); NATIVE_WARN_IF( p##A == NULL, "Could not load symbol " #A " from dbghelp.dll, version too old, but will continue without it."); } else
extern const char* slStackTracerDbghelpPath;
const char* slStackTracerDbghelpPath = "dbghelp.dll";


slStackTracerImpl::slStackTracerImpl()
{
	InitializeCriticalSection(&m_lock);
	m_lockInitialized = true;
}

slStackTracerImpl::~slStackTracerImpl()
{
	m_lockInitialized = false;
	DeleteCriticalSection(&m_lock);
}

void slStackTracerImpl::ensureInitialized()
{
	if (!m_initialized)
	{
		EnterCriticalSection(&m_lock);
		if (!m_initialized)
		{
			hDbgHelpDll = LoadLibraryA(slStackTracerDbghelpPath);
			NATIVE_WARN_IF(hDbgHelpDll == NULL, "slStackTracer: Unable to load dbghelp.dll");

			if (hDbgHelpDll)
			{
				hKernel32Dll = LoadLibraryA("kernel32.dll");
				NATIVE_WARN_IF(hKernel32Dll == NULL, "slStackTracer: Unable to load kernel32.dll");

				LOAD_FUNCTION(SymInitialize);
				LOAD_FUNCTION_WARN(SymRefreshModuleList);
				LOAD_FUNCTION(SymGetOptions);
				LOAD_FUNCTION(SymSetOptions);
				LOAD_FUNCTION(SymFromAddr);
				LOAD_FUNCTION(StackWalk64);
				LOAD_FUNCTION(SymFunctionTableAccess64);
				LOAD_FUNCTION(SymGetModuleBase64);
				LOAD_FUNCTION(SymGetLineFromAddr64);

				pRtlCaptureContext = (tRtlCaptureContext)GetProcAddress(hKernel32Dll, "RtlCaptureContext");
				pRtlCaptureStackBackTrace = (tRtlCaptureStackBackTrace)GetProcAddress(hKernel32Dll, "RtlCaptureStackBackTrace");

				DWORD symOptions = pSymGetOptions();
				symOptions |= SYMOPT_LOAD_LINES | SYMOPT_DEBUG | SYMOPT_DEFERRED_LOADS;
				pSymSetOptions(symOptions);

#if (_WIN32_WINNT >= 0x0600)
				UINT prevErrMode = GetErrorMode();
				SetErrorMode(SEM_FAILCRITICALERRORS);
#endif
				BOOL initsymbols = pSymInitialize(GetCurrentProcess(), NULL, TRUE);
				NATIVE_WARN_IF(initsymbols == FALSE, "slStackTracer: No debug symbols found for current Process, stack trace may fail.");

#if (_WIN32_WINNT >= 0x0600)
				SetErrorMode(prevErrMode);
#endif

				m_initialized = true;
			}
		}
		LeaveCriticalSection(&m_lock);
	}
}

void slStackTracerImpl::dumpStackTrace(const size_t* trace, int maxtrace)
{
	ensureInitialized();

	SYMBOL_INFO_PACKAGE sym;
	sym.si.SizeOfStruct = sizeof(sym.si);
	sym.si.MaxNameLen = sizeof(sym.name);

	HANDLE curproc = GetCurrentProcess();

	for (int i = 0; i < maxtrace; ++i)
	{
		uint64_t symaddress = trace[i];

		DWORD64 displacement = 0;
		BOOL ok_symbol = pSymFromAddr(
			curproc,
			DWORD64(symaddress),
			&displacement,
			&sym.si);


		if (!ok_symbol)
			::strcpy_s(sym.si.Name, 2000, "(unknown)");
		IMAGEHLP_LINE64 line;
		::memset(&line, 0, sizeof(line));
		line.SizeOfStruct = sizeof(line);

		pSymGetLineFromAddr64(
			curproc,
			DWORD64(symaddress),
			&displacement,
			&line);

		char trace[2048];
		_snprintf_s(trace, 2048, "%s(%i):'%s'\n", line.FileName, line.LineNumber, sym.si.Name);
		//printf(trace);
		slLog::Print("%s\n", trace);
	}
}

int slStackTracerImpl::getStackTrace(size_t* trace, int maxtrace, int framesToSkip)
{
	ensureInitialized();
	if (pRtlCaptureStackBackTrace)
		return pRtlCaptureStackBackTrace(framesToSkip, maxtrace, (PVOID*)trace, NULL);
	return 0;
}


static slStackTracerImpl g_stackTracer;

void slStackTracer::Print()
{
	size_t t[100];
	g_stackTracer.dumpStackTrace(t, g_stackTracer.getStackTrace(t, 100, 0));
}
