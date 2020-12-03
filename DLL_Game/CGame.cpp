#include "pch.h"
#include "CGame.h"

CGame g_CGame;
CString gstr;

LRESULT CALLBACK ShellProcMy(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	gstr.Format(L"���ӣ�nC=%d,wP=%d,lP=%d", nCode, wParam, lParam);
	if (nCode == 8)
		gstr += L"���ڼ�����";
	OutputDebugString(gstr);
	return CallNextHookEx(0, nCode, wParam, lParam);
};


int DLLGAME_Init(HMODULE hMFCMod)
{
	g_CGame._MFCMOD = hMFCMod;
	CString str;
	str.Format(L"DLLGAME_Init��%p,%p,%p\n", g_CGame._DLLMOD, g_CGame._MFCMOD, &g_CGame);
	OutputDebugString(str);
	return (int)&g_CGame;
}


int DLLGAME_HOOK(int TID)
{
	HHOOK hHook = SetWindowsHookExA(WH_SHELL, ShellProcMy,
		g_CGame._DLLMOD, TID);
	OutputDebugStringW(hHook ? L"��װ�ɹ�\n" : L"��װʧ��\n");
	return (int)hHook;
}



#pragma region ���ں���
void CGame::InitDLL(HINSTANCE hMod)
{
	_DLLMOD = hMod;
	_PID = GetCurrentProcessId();
	_HWND = CheckGame(0, _PID, 0);
	gstr.Format(L"ע�����%lu������ַ%p������%d", _PID, hMod, (int)_HWND);
	OutputDebugString(gstr);
}


/*	������ѭ�����ڣ��ж���Ϸ�Ƿ�ɹ�����
	���ߣ�����
	���ӣ�https://blog.csdn.net/dgyanyong/article/details/52994204
	�Ż���CO0kieؼ
*/
HWND CGame::CheckGame(DWORD TID, DWORD PID, DWORD TimeOut /*= 1000*/)
{
	ULONGLONG oldTime = GetTickCount64(), newTime;
	WCHAR buff[MAX_PATH];
	HWND hwndRet = NULL, hwndWindow = NULL;
	DWORD dwPid = NULL, dwTheardID = NULL;
	CStringW str;

	if (TimeOut)
		Sleep(55);

	do {
		if (TimeOut) Sleep(15);
		newTime = GetTickCount64();
		if (0 == (hwndWindow = ::GetTopWindow(0)))
			return FALSE;

		do
		{
			// ͨ�����ھ��ȡ�ý���ID
			dwTheardID = ::GetWindowThreadProcessId(hwndWindow, &dwPid);
			if (dwTheardID == TID && dwPid == PID)
			{
				GetClassNameW(hwndWindow, buff, MAX_PATH);
				str.Format(L"��� %d\tTID %d==%d,PID %d==%d %s\t",
					(int)hwndWindow, TID, dwTheardID, PID, dwPid, buff);
				GetWindowTextW(hwndWindow, buff, MAX_PATH);
				str.AppendFormat(L"%s\n", buff);
				OutputDebugStringW(str);


				if (wcscmp(buff, L"�������") == 0)
					return FALSE;
				if (wcscmp(buff, L"QQ������") == 0)
					hwndRet = hwndWindow;
				HWND tmp = ::GetNextWindow(hwndWindow, GW_HWNDNEXT);
				GetWindowTextW(tmp, buff, MAX_PATH);
				if (wcscmp(buff, L"QQ������") == 0)
					hwndRet = tmp;
			}
			// ȡ����һ�����ھ��
		} while (hwndWindow = ::GetNextWindow(hwndWindow, GW_HWNDNEXT));
	} while (newTime - oldTime < TimeOut);
	
	//��õĲ�һ���Ƕ��㴰�ڣ����ϲ���һ��
	return hwndRet;
}


BOOL CGame::MousePost(HWND hWnd, DWORD x, DWORD y)
{
	if (hWnd > 0)
	{
		y = x + y * 65536;
		PostMessageW(hWnd, 513, 1, y);
		PostMessageW(hWnd, 514, 0, y);
	}
	return 0;
}
#pragma endregion


