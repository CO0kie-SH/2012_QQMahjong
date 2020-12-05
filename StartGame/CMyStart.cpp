#include "pch.h"
#include "CMyStart.h"



#pragma region 类内函数
/*	函数：循环窗口，判断游戏是否成功开启
	作者：特招
	链接：https://blog.csdn.net/dgyanyong/article/details/52994204
	优化：CO0kie丶
*/
HWND CMyStart::CheckGame(DWORD TID, DWORD PID, DWORD TimeOut /*= 1000*/)
{
	ULONGLONG oldTime = GetTickCount64(), newTime;
	WCHAR buff[MAX_PATH];
	HWND hwndRet = NULL, hwndWindow = NULL;
	DWORD dwPid = NULL, dwTheardID = NULL;
	//CStringW str;

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
				//str.Format(L"句柄 %d\tTID %d==%d,PID %d==%d %s\t",
				//	(int)hwndWindow, TID, dwTheardID, PID, dwPid, buff);
				GetWindowTextW(hwndWindow, buff, MAX_PATH);
				//str.AppendFormat(L"%s\n", buff);
				//OutputDebugStringW(str);


				if (wcscmp(buff, L"程序错误") == 0)
					return FALSE;
				if (wcscmp(buff, L"致命错误") == 0)
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

int CMyStart::BtnClick(int index)
{
	//每次进入时，检查窗口是否正常
	BOOL isWind = IsWindow(this->_INFO.wMain);
	if (this->_INFO.wMain > 0 && isWind == 0)
	{
		CloseHandle(this->_INFO.hPro);
		ZeroMemory(&this->_INFO, sizeof(this->_INFO));
	}
	
	if (index == proStart)
	{
		OutputDebugStringA("pro开启\n");
		OutputDebugStringW(this->_path);
		if (isWind)	//重复开启
			BtnClick(proExit);
		CreatGame();
	}
	else if (index == proExit)
	{
		OutputDebugStringA("pro退出\n");
		if (isWind)
		{
			TerminateProcess(this->_INFO.hPro, 0);
			CloseHandle(this->_INFO.hPro);
			ZeroMemory(&this->_INFO, sizeof(this->_INFO));
		}
	}
	else if (index == dllStart)
	{
		OutputDebugStringA("dll注入\n");
		if (!isWind)
			MessageBoxA(0, "请先开启游戏", 0, 0);
		else if(g_hHOOK!=0)
			MessageBoxA(0, "请先卸载注入", 0, 0);
		else
			g_hHOOK = (HHOOK)DLLGAME_HOOK_KEY(this->_INFO.TID);
		if (g_hHOOK && this->_INFO.wMain>0)
		{
			SetActiveWindow(this->_INFO.wMain);
			Sleep(22);
			keybd_event(VK_F12, MapVirtualKeyA(VK_F12, 0), 0, 0);
			keybd_event(VK_F12, MapVirtualKeyA(VK_F12, 0), KEYEVENTF_KEYUP, 0);
		}
	}
	else if (index == dllExit)
	{
		OutputDebugStringA("dll卸载\n");
		if (g_hHOOK)
		{
			isWind = UnhookWindowsHookEx(g_hHOOK);
			OutputDebugStringA(isWind ? "卸载成功" : "卸载失败");
		}
		g_hHOOK = 0;
	}
	else if (index == mouseStart)
	{
		if (g_hHOOK == 0)
			MessageBoxA(0, "请先注入游戏", 0, 0);
		else if (this->_INFO.wMain == 0)
			MessageBoxA(0, "请先开启游戏", 0, 0);
		else
		{
			this->_Game->CLLK.KeyDown(VK_F12, this->_INFO.wMain);
		}
		//isWind = AttachThreadInput(GetCurrentThreadId(), this->_INFO.TID, TRUE);
		//DWORD pos = 666 + 533 * 65536;
		//PostMessageW(this->_INFO.wMain, 513, 1, pos);
		//PostMessageW(this->_INFO.wMain, 514, 0, pos);
		//AttachThreadInput(GetCurrentThreadId(), this->_INFO.TID, FALSE);
	}
	else if (index == mouseMsg)
	{
		if (g_hHOOK == 0)
			MessageBoxA(0, "请先注入游戏", 0, 0);
		else if (this->_INFO.wMain == 0)
			MessageBoxA(0, "请先开启游戏", 0, 0);
		else
		{
			this->_Game->CLLK.KeyDown(VK_F4, this->_INFO.wMain);
		}
		//ZeroMemory(_buff, 260);
		//SIZE_T size;
		//ReadProcessMemory(this->_INFO.hPro, (LPVOID)0x19BB34, _buff, 260, &size);
		//_Game->MousePost(this->_INFO.wMain, 31 * 3, 180 + 30 * 6);
	}
	return 0;
}


/*	函数：启动游戏
	作者：CO0kie丶
*/
BOOL CMyStart::CreatGame(int Num /*= 0*/)
{
	STARTUPINFOW si = { sizeof(STARTUPINFOW) };//启动信息
	PROCESS_INFORMATION pi;			//进程信息
	ZeroMemory(&pi, sizeof(pi));
	if (!CreateProcessW(
		this->_path,		//文件路径
		NULL,		//命令行
		NULL,		//进程安全属性
		NULL,		//线程安全属性
		FALSE,		//是否继承句柄
		FALSE,		//创建方式
		NULL,		//环境
		NULL,		//置运行目录
		&si,		//启动信息
		&pi)		//进程信息
		)
	{
		printf_s("CreateProcess failed (%d).\n", GetLastError());
		return false;
	}
	HWND retHwnd = this->CheckGame(pi.dwThreadId, pi.dwProcessId, 500);
	if (retHwnd > 0)
	{
		//成功开启游戏
		CloseHandle(pi.hThread); //CloseHandle(pi.hProcess);
		this->_INFO = GameInfo{
			retHwnd,		//HWND		wMain;
			pi.dwThreadId,	//DWORD		TID;
			pi.dwProcessId,	//DWORD		PID;
			pi.hProcess		//HANDLE	hPro;
		};
		Num = AttachThreadInput(GetCurrentThreadId(), this->_INFO.TID, TRUE);
		MessageBoxA(this->_thisWin, Num ? "成功" : "失败", "打开游戏", 0);
	}
	else if (Num <= 50)
	{
		//失败则重启游戏
		TerminateProcess(pi.hProcess, 0);
		CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
		CreatGame(Num + 1);
	}
	else if (Num > 50)
	{
		MessageBoxW(0, L"启动游戏失败，请重试", 0, 0);
	}

	/* 旧方法 */
	/*
	WaitForSingleObject(pi.hProcess, 600);			//等待进程
	CloseHandle(pi.hThread);

	DWORD addRead;
	ReadProcessMemory(pi.hProcess, (LPCVOID)0x199DC0, &addRead, 4, 0);

	if (addRead > 0)
	{
		//启动失败
		TerminateProcess(pi.hProcess, 0);
		CloseHandle(pi.hProcess);
		addRead = Num + 1;
		if(Num < 50)
			CreatGame(addRead);
	}
	else
	{
		//启动成功，赋值数据
		_INFO.hPro = pi.hProcess;
	}*/
	return Num;
}

#pragma endregion


