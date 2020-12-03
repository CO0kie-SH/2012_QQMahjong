#pragma once


#define TEST_API __declspec(dllexport)


extern "C" TEST_API int DLLGAME_Init(HMODULE hMFCMod);
extern TEST_API int DLLGAME_HOOK(int TID);


class _declspec(dllexport) CGame
{
public:
	void InitDLL(HINSTANCE hMod);
	HWND CheckGame(DWORD TID, DWORD PID, DWORD TimeOut = 1000);

	BOOL MousePost(HWND hWnd, DWORD x, DWORD y);
public:
	HMODULE _DLLMOD = 0;
	HMODULE _MFCMOD = 0;
private:
	DWORD _PID = 0;
	HWND _HWND;
	//CString mstr;
};

extern CGame g_CGame;
