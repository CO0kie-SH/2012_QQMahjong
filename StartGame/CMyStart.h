#pragma once
#define szPATH L"E:\\qq������������2\\kyodai.exe"

#include "..\DLL_Game\CGame.h"
#pragma comment(lib,"..\\Release\\DLL_Game.lib")

//�������������̽ṹ
typedef struct _GameInfo
{
	HWND wMain;
	DWORD TID;
	DWORD PID;
	HANDLE hPro;
}GameInfo;

//����ö������
enum EMProCtrl
{
	proStart,
	dllStart,
	proExit,
	dllExit,
	mouseStart,
	mouseMsg
};


//����ȫ�ֹ��Ӿ��������ж�ع���
static HHOOK g_hHOOK = 0;


class CMyStart
{
public:
	void InitCMyStart(CEdit* cedt, HMODULE hMod, HWND hWnd)
	{
		cedt->GetWindowTextW(this->_path, MAX_PATH);
		_Game = (CGame*)DLLGAME_Init(hMod);
		_DLLMOD = _Game->_DLLMOD;
		this->_thisWin = hWnd;
		hWnd = 0;
		hWnd = FindWindowA("#32770", "QQ������");
		if (hWnd)
		{
			_INFO.wMain = hWnd;
			_INFO.TID = GetWindowThreadProcessId(hWnd, &_INFO.PID);
			_INFO.hPro = OpenProcess(PROCESS_VM_READ, 0, _INFO.PID);
			CStringW str;
			str.Format(L"TID %d,PID %d,HWND %d,hPro %d,Զ������%d",
				_INFO.TID, _INFO.PID, (int)_INFO.wMain, (int)_INFO.hPro,
				AttachThreadInput(GetCurrentThreadId(), this->_INFO.TID, TRUE));
			OutputDebugStringW(str);
		}
	}
	void SetPath(CEdit* cedt)
	{
		cedt->GetWindowTextW(this->_path, MAX_PATH);
	}


	HWND CheckGame(DWORD TID, DWORD PID, DWORD TimeOut = 1000);
	BOOL BtnClick(int index);
	BOOL CreatGame(int Num = 0);
private:
	HWND _thisWin;
	WCHAR _path[MAX_PATH];
	GameInfo _INFO;

	HMODULE _DLLMOD;
	CGame* _Game;
	BYTE _buff[260];
};

