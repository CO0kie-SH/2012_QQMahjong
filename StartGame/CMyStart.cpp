#include "pch.h"
#include "CMyStart.h"



#pragma region ���ں���
/*	������ѭ�����ڣ��ж���Ϸ�Ƿ�ɹ�����
	���ߣ�����
	���ӣ�https://blog.csdn.net/dgyanyong/article/details/52994204
	�Ż���CO0kieؼ
*/
HWND CMyStart::CheckGame(DWORD TID, DWORD PID, DWORD TimeOut /*= 1000*/)
{
	ULONGLONG oldTime = GetTickCount64(), newTime;
	WCHAR buff[MAX_PATH];
	HWND hwndRet = NULL, hwndWindow = NULL;
	DWORD dwPid = NULL, dwTheardID = NULL;
	//CStringW str;

	if (TimeOut)
		Sleep(55);

	do {
		if (TimeOut) Sleep(15);
		newTime = GetTickCount64();
		if (0 == (hwndWindow = ::GetTopWindow(0)))
			return FALSE;

		do
		{
			// ͨ�����ھ��ȡ�ý���ID
			dwTheardID = ::GetWindowThreadProcessId(hwndWindow, &dwPid);
			if (dwTheardID == TID && dwPid == PID)
			{
				GetClassNameW(hwndWindow, buff, MAX_PATH);
				//str.Format(L"��� %d\tTID %d==%d,PID %d==%d %s\t",
				//	(int)hwndWindow, TID, dwTheardID, PID, dwPid, buff);
				GetWindowTextW(hwndWindow, buff, MAX_PATH);
				//str.AppendFormat(L"%s\n", buff);
				//OutputDebugStringW(str);


				if (wcscmp(buff, L"�������") == 0)
					return FALSE;
				if (wcscmp(buff, L"��������") == 0)
					return FALSE;
				if (wcscmp(buff, L"QQ������") == 0)
					hwndRet = hwndWindow;
				HWND tmp = ::GetNextWindow(hwndWindow, GW_HWNDNEXT);
				GetWindowTextW(tmp, buff, MAX_PATH);
				if (wcscmp(buff, L"QQ������") == 0)
					hwndRet = tmp;
			}
			// ȡ����һ�����ھ��
		} while (hwndWindow = ::GetNextWindow(hwndWindow, GW_HWNDNEXT));
	} while (newTime - oldTime < TimeOut);

	//��õĲ�һ���Ƕ��㴰�ڣ����ϲ���һ��
	return hwndRet;
}

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
		OutputDebugStringW(this->_path);
		if (isWind)	//�ظ�����
			BtnClick(proExit);
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
			g_hHOOK = (HHOOK)DLLGAME_HOOK_KEY(this->_INFO.TID);
		if (g_hHOOK && this->_INFO.wMain>0)
		{
			SetActiveWindow(this->_INFO.wMain);
			Sleep(22);
			keybd_event(VK_F12, MapVirtualKeyA(VK_F12, 0), 0, 0);
			keybd_event(VK_F12, MapVirtualKeyA(VK_F12, 0), KEYEVENTF_KEYUP, 0);
		}
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
	else if (index == mouseStart)
	{
		if (g_hHOOK == 0)
			MessageBoxA(0, "����ע����Ϸ", 0, 0);
		else if (this->_INFO.wMain == 0)
			MessageBoxA(0, "���ȿ�����Ϸ", 0, 0);
		else
		{
			this->_Game->CLLK.KeyDown(VK_F12, this->_INFO.wMain);
		}
		//isWind = AttachThreadInput(GetCurrentThreadId(), this->_INFO.TID, TRUE);
		//DWORD pos = 666 + 533 * 65536;
		//PostMessageW(this->_INFO.wMain, 513, 1, pos);
		//PostMessageW(this->_INFO.wMain, 514, 0, pos);
		//AttachThreadInput(GetCurrentThreadId(), this->_INFO.TID, FALSE);
	}
	else if (index == mouseMsg)
	{
		if (g_hHOOK == 0)
			MessageBoxA(0, "����ע����Ϸ", 0, 0);
		else if (this->_INFO.wMain == 0)
			MessageBoxA(0, "���ȿ�����Ϸ", 0, 0);
		else
		{
			this->_Game->CLLK.KeyDown(VK_F4, this->_INFO.wMain);
		}
		//ZeroMemory(_buff, 260);
		//SIZE_T size;
		//ReadProcessMemory(this->_INFO.hPro, (LPVOID)0x19BB34, _buff, 260, &size);
		//_Game->MousePost(this->_INFO.wMain, 31 * 3, 180 + 30 * 6);
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
	HWND retHwnd = this->CheckGame(pi.dwThreadId, pi.dwProcessId, 500);
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
		Num = AttachThreadInput(GetCurrentThreadId(), this->_INFO.TID, TRUE);
		MessageBoxA(this->_thisWin, Num ? "�ɹ�" : "ʧ��", "����Ϸ", 0);
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

#pragma endregion


