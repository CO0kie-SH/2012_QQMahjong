#pragma once
#define TEST_API __declspec(dllexport)

//extern "C"
//{
//	int WINAPI MessageBoxTimeoutA(IN HWND hWnd, IN LPCSTR lpText, IN LPCSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);
//	int WINAPI MessageBoxTimeoutW(IN HWND hWnd, IN LPCWSTR lpText, IN LPCWSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);
//}
//
//#ifdef UNICODE
//#define MessageBoxTimeout MessageBoxTimeoutW
//#else
//#define MessageBoxTimeout MessageBoxTimeoutA
//#endif

extern "C" TEST_API int DLLGAME_Init(HMODULE hMFCMod);
extern TEST_API int DLLGAME_HOOK(int TID);
extern TEST_API int DLLGAME_HOOK_KEY(int TID);


/*	结构：键盘回调信息结构体
	整理：CO0kie丶
	来源：https://docs.microsoft.com/zh-cn/previous-versions/windows/desktop/legacy/ms644984(v=vs.85)
*/
typedef struct _KEYINFO
{
	unsigned int num : 16;
	unsigned int scan : 8;
	unsigned int extend : 1;
	unsigned int NULL1 : 4;
	unsigned int ALT : 1;
	unsigned int oldDOWN : 1;
	unsigned int nowDOWN : 1;
}KEYINFO, * LPKEYINFO;



/*	结构：键盘回调信息结构体
	整理：CO0kie丶
	来源：超级模块-小烦
	参考1：https://www.cnblogs.com/buyishi/p/10320789.html
	参考2：https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/ns-winuser-input
*/
typedef struct _KEYInput
{
	unsigned int num : 16;
	unsigned int scan : 8;
	unsigned int extend : 1;
	unsigned int NULL1 : 4;
	unsigned int ALT : 1;
	unsigned int oldDOWN : 1;
	unsigned int nowDOWN : 1;
}KEYInput, * LPKEYInput;



/*	类：连连看类，用于操作连连看进程
	作者：CO0kie丶
*/
class TEST_API CLLK
{
public:
	void InitLLK();

	DWORD ReadProcessINT(LPDWORD list, DWORD len);
	DWORD KeyDown(WORD KEY_VK, HWND hWnd = 0);
private:
	char* mmap = 0;
};

/*	类：游戏类，用于查找游戏数据
	作者：CO0kie丶
*/
class TEST_API CGame
{
public:
	int InitDLL(HINSTANCE hMod, HWND wMain);

	BOOL MousePost(HWND hWnd, DWORD x, DWORD y);
	DWORD GetTid() { return _TID; }
public:
	HMODULE _DLLMOD = 0;
	HMODULE _MFCMOD = 0;
	HWND wMain = 0;
	CLLK CLLK;
private:
	DWORD _TID = 0;
	DWORD _PID = 0;
	DWORD _THREAD = 0;
};

/*定义全局类对象*/
extern CGame g_CGame;
