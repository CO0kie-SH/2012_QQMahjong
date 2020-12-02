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
				DWORD   th32ProcessID;      // ��������ID
				BYTE  * modBaseAddr;        // ģ��ļ��ػ���ַ
				DWORD   modBaseSize;        // ģ��Ĵ�С
				HMODULE hModule;            // ģ��ľ��(���ػ�ַ)
				WCHAR   szModule[MAX_MODULE_NAME32 + 1];	// ģ����
				WCHAR   szExePath[MAX_PATH];				// ����·��
			} MODULEENTRY32W;
			*/
			wprintf_s(L"%p\t%s\n", ModuleInfo.hModule, ModuleInfo.szExePath);
		} while (Module32Next(Snapshot, &ModuleInfo));
		ret = CloseHandle(Snapshot);
	}
	return ret;
}
