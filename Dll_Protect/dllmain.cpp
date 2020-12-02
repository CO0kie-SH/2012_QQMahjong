﻿// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "CProtect.h"

BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	DWORD tid = GetCurrentThreadId(), pid = GetCurrentProcessId();
	printf_s("模式%d\t来自线程%d\t进程\t%d\t%X\t%p\n", ul_reason_for_call, tid, pid, pid, hModule);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
	}break;
	case DLL_THREAD_ATTACH: {
		
	}break;
	case DLL_THREAD_DETACH: {
	}break;
	case DLL_PROCESS_DETACH: {
	}break;
	}
	return TRUE;
}

