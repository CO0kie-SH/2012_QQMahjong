#pragma once
#define szPATH L"E:\\qq������������2\\kyodai.exe"

#include "..\DLL_Game\CGame.h"
#pragma comment(lib,"..\\Debug\\DLL_Game.lib")

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


//������̹��ӻص�����
LRESULT CALLBACK
MYLowLevelKeyboardProc(
	_In_ int    nCode,	//�ҹ���������ȷ����δ�����Ϣ�Ĵ��롣
	_In_ WPARAM wParam,	//������Ϣ�ı�ʶ����
	//�˲���������������Ϣ֮һ��WM_KEYDOWN��WM_KEYUP��WM_SYSKEYDOWN��WM_SYSKEYUP��
	_In_ LPARAM lParam);	//ָ��KBDLLHOOKSTRUCT�ṹ��ָ�롣


//����ȫ�ֹ��Ӿ��������ж�ع���
static HHOOK g_hHOOK = 0;



class CMyStart
{
public:
	void InitCMyStart(HMODULE hMod,HWND hWnd)
	{
		_Game = (CGame*)DLLGAME_Init(hMod);
		_DLLMOD = _Game->_DLLMOD;
		this->_thisWin = hWnd;
		hWnd = FindWindow(L"#32770", L"QQ������");
		if (hWnd)
		{
			_INFO.wMain = hWnd;
			_INFO.TID = GetWindowThreadProcessId(hWnd, &_INFO.PID);
			_INFO.hPro = OpenProcess(PROCESS_VM_READ, 0, _INFO.PID);
			CStringW str;
			str.Format(L"TID %d,PID %d,HWND %d,hPro %d",
				_INFO.TID, _INFO.PID, (int)_INFO.wMain, (int)_INFO.hPro);
			OutputDebugStringW(str);
		}
	}

	BOOL BtnClick(int index);
	BOOL CreatGame(int Num = 0);
	BOOL BeginHook();
private:
	HWND _thisWin;
	WCHAR _path[MAX_PATH] = szPATH;
	GameInfo _INFO;

	HMODULE _DLLMOD;
	CGame* _Game;
	BYTE _buff[260];
};

