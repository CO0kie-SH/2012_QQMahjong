#pragma once
#define szPATH L"E:\\qq连连看单机版2\\kyodai.exe"

#include "..\DLL_Game\CGame.h"
#pragma comment(lib,"..\\Debug\\DLL_Game.lib")

//定义连连看进程结构
typedef struct _GameInfo
{
	HWND wMain;
	DWORD TID;
	DWORD PID;
	HANDLE hPro;
}GameInfo;

//定义枚举类型
enum EMProCtrl
{
	proStart,
	dllStart,
	proExit,
	dllExit,
	mouseStart,
	mouseMsg
};


//定义键盘钩子回调函数
LRESULT CALLBACK
MYLowLevelKeyboardProc(
	_In_ int    nCode,	//挂钩过程用来确定如何处理消息的代码。
	_In_ WPARAM wParam,	//键盘消息的标识符。
	//此参数可以是以下消息之一：WM_KEYDOWN，WM_KEYUP，WM_SYSKEYDOWN或WM_SYSKEYUP。
	_In_ LPARAM lParam);	//指向KBDLLHOOKSTRUCT结构的指针。


//定义全局钩子句柄，用于卸载钩子
static HHOOK g_hHOOK = 0;



class CMyStart
{
public:
	void InitCMyStart(HMODULE hMod)
	{
		_Game = (CGame*)DLLGAME_Init(hMod);
		_DLLMOD = _Game->_DLLMOD;
	}

	BOOL BtnClick(int index);
	BOOL CreatGame(int Num = 0);
	BOOL BeginHook();
private:
	WCHAR _path[MAX_PATH] = szPATH;
	GameInfo _INFO;

	HMODULE _DLLMOD;
	CGame* _Game;
};

