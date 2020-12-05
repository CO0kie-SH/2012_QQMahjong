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


/*	�ṹ�����̻ص���Ϣ�ṹ��
	����CO0kieؼ
	��Դ��https://docs.microsoft.com/zh-cn/previous-versions/windows/desktop/legacy/ms644984(v=vs.85)
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



/*	�ṹ�����̻ص���Ϣ�ṹ��
	����CO0kieؼ
	��Դ������ģ��-С��
	�ο�1��https://www.cnblogs.com/buyishi/p/10320789.html
	�ο�2��https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/ns-winuser-input
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



/*	�ࣺ�������࣬���ڲ�������������
	���ߣ�CO0kieؼ
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

/*	�ࣺ��Ϸ�࣬���ڲ�����Ϸ����
	���ߣ�CO0kieؼ
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

/*����ȫ�������*/
extern CGame g_CGame;
