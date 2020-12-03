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

