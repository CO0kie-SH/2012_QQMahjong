#include "pch.h"
#include "CProtect.h"
#include <tlhelp32.h>

int CProtect::EnumModule()
{
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

	if (Snapshot == INVALID_HANDLE_VALUE) return false;
	BOOL ret = FALSE;
	MODULEENTRY32 ModuleInfo = { sizeof(MODULEENTRY32) };
	if (Module32First(Snapshot, &ModuleInfo))
	{
		do{
			/*
			typedef struct tagMODULEENTRY32W
			{
				DWORD   th32ProcessID;      // 所属进程ID
				BYTE  * modBaseAddr;        // 模块的加载基地址
				DWORD   modBaseSize;        // 模块的大小
				HMODULE hModule;            // 模块的句柄(加载基址)
				WCHAR   szModule[MAX_MODULE_NAME32 + 1];	// 模块名
				WCHAR   szExePath[MAX_PATH];				// 所在路径
			} MODULEENTRY32W;
			*/
			wprintf_s(L"%p\t%s\n", ModuleInfo.hModule, ModuleInfo.szExePath);
		} while (Module32Next(Snapshot, &ModuleInfo));
		ret = CloseHandle(Snapshot);
	}
	return ret;
}
