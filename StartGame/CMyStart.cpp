#include "pch.h"
#include "CMyStart.h"

LRESULT CALLBACK
MYLowLevelKeyboardProc(
	_In_ int    nCode,	//挂钩过程用来确定如何处理消息的代码。
	_In_ WPARAM wParam,	//键盘消息的标识符。
	//此参数可以是以下消息之一：WM_KEYDOWN，WM_KEYUP，WM_SYSKEYDOWN或WM_SYSKEYUP。
	_In_ LPARAM lParam) {	//指向KBDLLHOOKSTRUCT结构的指针。
	WCHAR* buff = new WCHAR[200];
	wsprintfW(buff, L"消息=%d，wP=%d，lP=%d\n", nCode, wParam, lParam);
	OutputDebugStringW(buff);
	delete[] buff;
	return CallNextHookEx(0, nCode, wParam, lParam);
}


#pragma region 类内函数
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
		if (isWind)
		{
			BtnClick(proExit);
		}
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
			g_hHOOK = (HHOOK)DLLGAME_HOOK(this->_INFO.TID);
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
	else if (index == mouseStart && this->_INFO.wMain > 0)
	{
		//isWind = AttachThreadInput(GetCurrentThreadId(), this->_INFO.TID, TRUE);
		DWORD pos = 666 + 533 * 65536;
		PostMessageW(this->_INFO.wMain, 513, 1, pos);
		PostMessageW(this->_INFO.wMain, 514, 0, pos);
		//AttachThreadInput(GetCurrentThreadId(), this->_INFO.TID, FALSE);
	}
	else if (index == mouseMsg && this->_INFO.wMain > 0)
	{
		ZeroMemory(_buff, 260);
		SIZE_T size;
		ReadProcessMemory(this->_INFO.hPro, (LPVOID)0x19BB34, _buff, 260, &size);
		_Game->MousePost(this->_INFO.wMain, 31 * 3, 180 + 30 * 6);
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
	HWND retHwnd = _Game->CheckGame(pi.dwThreadId, pi.dwProcessId, 500);
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
		AttachThreadInput(GetCurrentThreadId(), this->_INFO.TID, TRUE);
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


/*	函数：开始键盘钩子(作废)
	作者：CO0kie丶
*/
BOOL CMyStart::BeginHook()
{
	/*	//函数原型：WINUSERAPI	HHOOK	WINAPI
SetWindowsHookExW(
	_In_ int idHook,			//钩子类型
	_In_ HOOKPROC lpfn,			//回调函数
	_In_opt_ HINSTANCE hmod,	//调用模块句柄
	_In_ DWORD dwThreadId);		//需要钩的线程，为0则全部
*/


	/*	//函数原型：LRESULT CALLBACK
LowLevelKeyboardProc(
  _In_ int    nCode,	//挂钩过程用来确定如何处理消息的代码。
  _In_ WPARAM wParam,	//键盘消息的标识符。
  //此参数可以是以下消息之一：WM_KEYDOWN，WM_KEYUP，WM_SYSKEYDOWN或WM_SYSKEYUP。
  _In_ LPARAM lParam);	//指向KBDLLHOOKSTRUCT结构的指针。
 */
	
	//HHOOK hHook = SetWindowsHookExW(WH_KEYBOARD_LL, (HOOKPROC)MYLowLevelKeyboardProc,
	//	this->_DLLMOD, 5212);
	//	//GetModuleHandleW(NULL), 18100);
	//OutputDebugStringW(hHook ? L"成功\n" : L"失败\n");
	//MessageBox(0, hHook ? L"成功" : L"失败", 0, 0);
	//if (!hHook)return false;
	//g_hHOOK = hHook;
	return 0;
}
#pragma endregion


