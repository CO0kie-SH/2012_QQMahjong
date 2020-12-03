#include "pch.h"
#include "CGame.h"

CGame g_CGame;
CString gstr;

LRESULT CALLBACK ShellProcMy(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	gstr.Format(L"钩子：nC=%d,wP=%d,lP=%d", nCode, wParam, lParam);
	if (nCode == 8)
		gstr += L"窗口激活了";
	OutputDebugString(gstr);
	return CallNextHookEx(0, nCode, wParam, lParam);
};


int DLLGAME_Init(HMODULE hMFCMod)
{
	g_CGame._MFCMOD = hMFCMod;
	CString str;
	str.Format(L"DLLGAME_Init：%p,%p,%p\n", g_CGame._DLLMOD, g_CGame._MFCMOD, &g_CGame);
	OutputDebugString(str);
	return (int)&g_CGame;
}


int DLLGAME_HOOK(int TID)
{
	HHOOK hHook = SetWindowsHookExA(WH_SHELL, ShellProcMy,
		g_CGame._DLLMOD, TID);
	OutputDebugStringW(hHook ? L"安装成功\n" : L"安装失败\n");
	return (int)hHook;
}



#pragma region 类内函数
void CGame::InitDLL(HINSTANCE hMod)
{
	_DLLMOD = hMod;
	_PID = GetCurrentProcessId();
	_HWND = CheckGame(0, _PID, 0);
	gstr.Format(L"注入进程%lu，本基址%p，窗口%d", _PID, hMod, (int)_HWND);
	OutputDebugString(gstr);
}


/*	函数：循环窗口，判断游戏是否成功开启
	作者：特招
	链接：https://blog.csdn.net/dgyanyong/article/details/52994204
	优化：CO0kie丶
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
			// 通过窗口句柄取得进程ID
			dwTheardID = ::GetWindowThreadProcessId(hwndWindow, &dwPid);
			if (dwTheardID == TID && dwPid == PID)
			{
				GetClassNameW(hwndWindow, buff, MAX_PATH);
				str.Format(L"句柄 %d\tTID %d==%d,PID %d==%d %s\t",
					(int)hwndWindow, TID, dwTheardID, PID, dwPid, buff);
				GetWindowTextW(hwndWindow, buff, MAX_PATH);
				str.AppendFormat(L"%s\n", buff);
				OutputDebugStringW(str);


				if (wcscmp(buff, L"程序错误") == 0)
					return FALSE;
				if (wcscmp(buff, L"QQ连连看") == 0)
					hwndRet = hwndWindow;
				HWND tmp = ::GetNextWindow(hwndWindow, GW_HWNDNEXT);
				GetWindowTextW(tmp, buff, MAX_PATH);
				if (wcscmp(buff, L"QQ连连看") == 0)
					hwndRet = tmp;
			}
			// 取得下一个窗口句柄
		} while (hwndWindow = ::GetNextWindow(hwndWindow, GW_HWNDNEXT));
	} while (newTime - oldTime < TimeOut);
	
	//获得的不一定是顶层窗口，向上查找一下
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


