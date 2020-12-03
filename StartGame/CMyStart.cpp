#include "pch.h"
#include "CMyStart.h"

LRESULT CALLBACK
MYLowLevelKeyboardProc(
	_In_ int    nCode,	//�ҹ���������ȷ����δ�����Ϣ�Ĵ��롣
	_In_ WPARAM wParam,	//������Ϣ�ı�ʶ����
	//�˲���������������Ϣ֮һ��WM_KEYDOWN��WM_KEYUP��WM_SYSKEYDOWN��WM_SYSKEYUP��
	_In_ LPARAM lParam) {	//ָ��KBDLLHOOKSTRUCT�ṹ��ָ�롣
	WCHAR* buff = new WCHAR[200];
	wsprintfW(buff, L"��Ϣ=%d��wP=%d��lP=%d\n", nCode, wParam, lParam);
	OutputDebugStringW(buff);
	delete[] buff;
	return CallNextHookEx(0, nCode, wParam, lParam);
}


#pragma region ���ں���
int CMyStart::BtnClick(int index)
{
	//ÿ�ν���ʱ����鴰���Ƿ�����
	BOOL isWind = IsWindow(this->_INFO.wMain);
	if (this->_INFO.wMain > 0 && isWind == 0)
	{
		CloseHandle(this->_INFO.hPro);
		ZeroMemory(&this->_INFO, sizeof(this->_INFO));
	}
	
	if (index == proStart)
	{
		OutputDebugStringA("pro����\n");
		if (isWind)
		{
			BtnClick(proExit);
		}
		CreatGame();
	}
	else if (index == proExit)
	{
		OutputDebugStringA("pro�˳�\n");
		if (isWind)
		{
			TerminateProcess(this->_INFO.hPro, 0);
			CloseHandle(this->_INFO.hPro);
			ZeroMemory(&this->_INFO, sizeof(this->_INFO));
		}
	}
	else if (index == dllStart)
	{
		OutputDebugStringA("dllע��\n");
		if (!isWind)
			MessageBoxA(0, "���ȿ�����Ϸ", 0, 0);
		else if(g_hHOOK!=0)
			MessageBoxA(0, "����ж��ע��", 0, 0);
		else
			g_hHOOK = (HHOOK)DLLGAME_HOOK(this->_INFO.TID);
	}
	else if (index == dllExit)
	{
		OutputDebugStringA("dllж��\n");
		if (g_hHOOK)
		{
			isWind = UnhookWindowsHookEx(g_hHOOK);
			OutputDebugStringA(isWind ? "ж�سɹ�" : "ж��ʧ��");
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


/*	������������Ϸ
	���ߣ�CO0kieؼ
*/
BOOL CMyStart::CreatGame(int Num /*= 0*/)
{
	STARTUPINFOW si = { sizeof(STARTUPINFOW) };//������Ϣ
	PROCESS_INFORMATION pi;			//������Ϣ
	ZeroMemory(&pi, sizeof(pi));
	if (!CreateProcessW(
		this->_path,		//�ļ�·��
		NULL,		//������
		NULL,		//���̰�ȫ����
		NULL,		//�̰߳�ȫ����
		FALSE,		//�Ƿ�̳о��
		FALSE,		//������ʽ
		NULL,		//����
		NULL,		//������Ŀ¼
		&si,		//������Ϣ
		&pi)		//������Ϣ
		)
	{
		printf_s("CreateProcess failed (%d).\n", GetLastError());
		return false;
	}
	HWND retHwnd = _Game->CheckGame(pi.dwThreadId, pi.dwProcessId, 500);
	if (retHwnd > 0)
	{
		//�ɹ�������Ϸ
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
		//ʧ����������Ϸ
		TerminateProcess(pi.hProcess, 0);
		CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
		CreatGame(Num + 1);
	}
	else if (Num > 50)
	{
		MessageBoxW(0, L"������Ϸʧ�ܣ�������", 0, 0);
	}

	/* �ɷ��� */
	/*
	WaitForSingleObject(pi.hProcess, 600);			//�ȴ�����
	CloseHandle(pi.hThread);

	DWORD addRead;
	ReadProcessMemory(pi.hProcess, (LPCVOID)0x199DC0, &addRead, 4, 0);

	if (addRead > 0)
	{
		//����ʧ��
		TerminateProcess(pi.hProcess, 0);
		CloseHandle(pi.hProcess);
		addRead = Num + 1;
		if(Num < 50)
			CreatGame(addRead);
	}
	else
	{
		//�����ɹ�����ֵ����
		_INFO.hPro = pi.hProcess;
	}*/
	return Num;
}


/*	��������ʼ���̹���(����)
	���ߣ�CO0kieؼ
*/
BOOL CMyStart::BeginHook()
{
	/*	//����ԭ�ͣ�WINUSERAPI	HHOOK	WINAPI
SetWindowsHookExW(
	_In_ int idHook,			//��������
	_In_ HOOKPROC lpfn,			//�ص�����
	_In_opt_ HINSTANCE hmod,	//����ģ����
	_In_ DWORD dwThreadId);		//��Ҫ�����̣߳�Ϊ0��ȫ��
*/


	/*	//����ԭ�ͣ�LRESULT CALLBACK
LowLevelKeyboardProc(
  _In_ int    nCode,	//�ҹ���������ȷ����δ�����Ϣ�Ĵ��롣
  _In_ WPARAM wParam,	//������Ϣ�ı�ʶ����
  //�˲���������������Ϣ֮һ��WM_KEYDOWN��WM_KEYUP��WM_SYSKEYDOWN��WM_SYSKEYUP��
  _In_ LPARAM lParam);	//ָ��KBDLLHOOKSTRUCT�ṹ��ָ�롣
 */
	
	//HHOOK hHook = SetWindowsHookExW(WH_KEYBOARD_LL, (HOOKPROC)MYLowLevelKeyboardProc,
	//	this->_DLLMOD, 5212);
	//	//GetModuleHandleW(NULL), 18100);
	//OutputDebugStringW(hHook ? L"�ɹ�\n" : L"ʧ��\n");
	//MessageBox(0, hHook ? L"�ɹ�" : L"ʧ��", 0, 0);
	//if (!hHook)return false;
	//g_hHOOK = hHook;
	return 0;
}
#pragma endregion


