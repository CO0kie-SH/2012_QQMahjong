#pragma once
#define szPATH L"E:\\qq连连看单机版2\\kyodai.exe"


enum EMProCtrl
{
	proStart,
	dllStart,
	proExit,
	dllExit
};

typedef struct _GameInfo
{
	HWND wMain;
	DWORD TID;
	DWORD PID;
	HANDLE hPro;
}GameInfo;

int GetHwndText(HWND hWnd, CStringW& str);
int GetHwndClass(HWND hWnd, CStringW& str);

class CMyStart
{
public:
	CMyStart() {
		OutputDebugStringW(_path);
	};
	BOOL BtnClick(int index);
	BOOL CreatGame(int Num = 0);
	HWND CheckGame(DWORD TID, DWORD PID, DWORD TimeOut = 1000);
private:
	WCHAR _path[MAX_PATH] = szPATH;
	GameInfo _INFO;
};

