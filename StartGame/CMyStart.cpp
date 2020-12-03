#include "pch.h"
#include "CMyStart.h"




int GetHwndText(HWND hWnd, CStringW& str)
{
	int len = GetWindowTextLengthW(hWnd) + 1;
	WCHAR* buff = new WCHAR[len];
	len = GetWindowTextW(hWnd, buff, len);
	str = buff;
	delete[] buff;
	str += L"\n";
	return len;
}

int GetHwndClass(HWND hWnd, CStringW& str)
{
	int ret = 0;
	WCHAR buff[MAX_PATH];
	ret = GetClassNameW(hWnd, buff, MAX_PATH);
	str += buff;
	str += L"\n";
	return ret;
}
#pragma region ���ں���
int CMyStart::BtnClick(int index)
{
	if (index == proStart)
	{
		OutputDebugStringA("proStart\n");
		CreatGame();
	}
	else if (index == proExit)
	{
		OutputDebugStringA("proExit\n");
	}
	else if (index == dllStart)
	{
		OutputDebugStringA("dllStart\n");
	}
	else if (index == dllExit)
	{
		OutputDebugStringA("dllExit\n");
	}
	return 0;
}

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
	HWND retHwnd = CheckGame(pi.dwThreadId, pi.dwProcessId, 500);
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
	}
	else if (Num < 50)
	{
		//ʧ����������Ϸ
		TerminateProcess(pi.hProcess, 0);
		CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
		CreatGame(Num + 1);
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


/*	������ѭ�����ڣ��ж���Ϸ�Ƿ�ɹ�����
	���ߣ�����
	���ӣ�https://blog.csdn.net/dgyanyong/article/details/52994204
	�Ż���CO0kieؼ
*/
HWND CMyStart::CheckGame(DWORD TID, DWORD PID, DWORD TimeOut /*= 1000*/)
{
	ULONGLONG oldTime = GetTickCount64(), newTime;
	Sleep(88);
	WCHAR buff[MAX_PATH];
	HWND hwndRet = NULL, hwndWindow = NULL;
	DWORD dwPid;
	CStringW str;
	do {
		Sleep(15);
		newTime = GetTickCount64();
		if (0 == (hwndWindow = ::GetTopWindow(0)))
			break;
		do
		{
			// ͨ�����ھ��ȡ�ý���ID
			DWORD dwTheardID = ::GetWindowThreadProcessId(hwndWindow, &dwPid);
			if (dwTheardID == TID)
			{
				GetClassNameW(hwndWindow, buff, MAX_PATH);
				if (wcscmp(buff, L"#32770"))
					break;
				str.Format(L"TID %lu==%lu\tPID %lu==%lu\t%lu\t%s\t",
					TID, dwTheardID, PID, dwPid, (DWORD)hwndWindow, buff);
				GetWindowTextW(hwndWindow, buff, MAX_PATH);

				//�жϴ��󴰿�
				if (wcscmp(buff, L"�������") == 0)
					return FALSE;

				//��ӡ��Ϣ
				str.AppendFormat(L"%s\n", buff);
				OutputDebugStringW(str);
				hwndRet = hwndWindow;
			}
			// ȡ����һ�����ھ��
		} while (hwndWindow = ::GetNextWindow(hwndWindow, GW_HWNDNEXT));
	} while (newTime - oldTime < TimeOut);

	if (hwndRet > 0)
		return hwndRet;
	else if (hwndWindow > 0)
	{
		while (HWND tmp = ::GetParent(hwndWindow))
		{
			hwndWindow = tmp;
		}
		return hwndWindow;
	}
	return 0;
}
#pragma endregion


