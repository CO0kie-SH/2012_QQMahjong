#include "pch.h"
#include "CMyStart.h"




int GetHwndText(HWND hWnd, CStringW& str)
{
	int len = GetWindowTextLengthW(hWnd) + 1;
	WCHAR* buff = new WCHAR[len];
	len = GetWindowTextW(hWnd, buff, len);
	str = buff;
	delete[] buff;
	str += L"\n";
	return len;
}

int GetHwndClass(HWND hWnd, CStringW& str)
{
	int ret = 0;
	WCHAR buff[MAX_PATH];
	ret = GetClassNameW(hWnd, buff, MAX_PATH);
	str += buff;
	str += L"\n";
	return ret;
}
#pragma region 类内函数
int CMyStart::BtnClick(int index)
{
	if (index == proStart)
	{
		OutputDebugStringA("proStart\n");
		CreatGame();
	}
	else if (index == proExit)
	{
		OutputDebugStringA("proExit\n");
	}
	else if (index == dllStart)
	{
		OutputDebugStringA("dllStart\n");
	}
	else if (index == dllExit)
	{
		OutputDebugStringA("dllExit\n");
	}
	return 0;
}

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
	HWND retHwnd = CheckGame(pi.dwThreadId, pi.dwProcessId, 500);
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
	}
	else if (Num < 50)
	{
		//失败则重启游戏
		TerminateProcess(pi.hProcess, 0);
		CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
		CreatGame(Num + 1);
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


/*	函数：循环窗口，判断游戏是否成功开启
	作者：特招
	链接：https://blog.csdn.net/dgyanyong/article/details/52994204
	优化：CO0kie丶
*/
HWND CMyStart::CheckGame(DWORD TID, DWORD PID, DWORD TimeOut /*= 1000*/)
{
	ULONGLONG oldTime = GetTickCount64(), newTime;
	Sleep(88);
	WCHAR buff[MAX_PATH];
	HWND hwndRet = NULL, hwndWindow = NULL;
	DWORD dwPid;
	CStringW str;
	do {
		Sleep(15);
		newTime = GetTickCount64();
		if (0 == (hwndWindow = ::GetTopWindow(0)))
			break;
		do
		{
			// 通过窗口句柄取得进程ID
			DWORD dwTheardID = ::GetWindowThreadProcessId(hwndWindow, &dwPid);
			if (dwTheardID == TID)
			{
				GetClassNameW(hwndWindow, buff, MAX_PATH);
				if (wcscmp(buff, L"#32770"))
					break;
				str.Format(L"TID %lu==%lu\tPID %lu==%lu\t%lu\t%s\t",
					TID, dwTheardID, PID, dwPid, (DWORD)hwndWindow, buff);
				GetWindowTextW(hwndWindow, buff, MAX_PATH);

				//判断错误窗口
				if (wcscmp(buff, L"程序错误") == 0)
					return FALSE;

				//打印信息
				str.AppendFormat(L"%s\n", buff);
				OutputDebugStringW(str);
				hwndRet = hwndWindow;
			}
			// 取得下一个窗口句柄
		} while (hwndWindow = ::GetNextWindow(hwndWindow, GW_HWNDNEXT));
	} while (newTime - oldTime < TimeOut);

	if (hwndRet > 0)
		return hwndRet;
	else if (hwndWindow > 0)
	{
		while (HWND tmp = ::GetParent(hwndWindow))
		{
			hwndWindow = tmp;
		}
		return hwndWindow;
	}
	return 0;
}
#pragma endregion


