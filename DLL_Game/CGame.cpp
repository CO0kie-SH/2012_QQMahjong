#include "pch.h"
#include "CGame.h"

CGame g_CGame;
DWORD g_this = 0, g_clean = 0, * g_nums = 0;
char* g_map = 0;
CString gstr;

LRESULT CALLBACK ShellProcMy(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	gstr.Format(L"��ǹ��ӣ�nC=%d,wP=%d,lP=%d", nCode, wParam, lParam);
	if (nCode == 8)
		gstr += L"���ڼ�����";
	
	OutputDebugString(gstr);
	return CallNextHookEx(0, nCode, wParam, lParam);
};


/*	���������̹��ӻص�
	����2��wParam = VK__**
	����3��lParam = struct _KEYINFO
	���ߣ�CO0kieؼ
*/
LRESULT CALLBACK KeyboardProcMy(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{
	KEYINFO key; UINT rh = 0;
	memcpy(&key, &lParam, 4);
	//��ʽ����Ϣ
	gstr.Format(L"���̹��ӣ�nC=%d,wP=%d,lP=%X", nCode, wParam, lParam);
	gstr.AppendFormat(L"��Num %d,Scam %d,��չ%d,ALT %d,old %d,now %d",
		key.num, key.scan,key.extend, key.ALT, key.oldDOWN, key.nowDOWN);

	//�ж�����
	if (nCode == 0 && wParam == VK_F12 && key.nowDOWN && g_map)
	{
		gstr.AppendFormat(L" F12_UP,����%X", g_this);
		g_CGame.MousePost(g_CGame.wMain, 666, 533);
	}
	if (nCode == 0 && wParam == VK_F11 && key.nowDOWN && g_map)
	{
		gstr.AppendFormat(L" F11_UP,ȫɾ%X", g_this);
		while (*g_nums)
		{
			DWORD count = *g_nums / 2, i = 0;
			for (; i < count; i++)
			{
				_asm
				{
					MOV ECX, g_this;
					PUSH 0XF4;
					PUSH 0;
					PUSH 0;
					MOV EAX, 0x41E691;
					CALL EAX;
				}
			}
			if (*g_nums) //��Ҫ���õ�ͼ
			{
				_asm
				{
					MOV ECX, g_this;
					PUSH 0XF1;		//F1����ͼ,F2����
					PUSH 0;
					PUSH 0;
					MOV EAX, 0x41E691;
					CALL EAX;
				}
			}
		}
		
		//POINT x = {0};
		//POINT y = {0};
		//_asm
		//{
		//	mov ECX, g_clean;
		//	lea eax, x;
		//	push eax;
		//	lea eax, y;
		//	push eax;
		//	mov eax, 0x42923F;
		//	call eax;			
		//}
	}
	else if (nCode==0 && key.oldDOWN && key.nowDOWN && g_map)
	{
		_asm
		{
			XOR EAX, EAX
			XOR EAX, EAX
		}
		switch (wParam)
		{
		case 0: break;
		case VK_F1:
			gstr.AppendFormat(L" F1_UP,ָ����%X", g_this);
			_asm
			{
				MOV ECX, g_this;
				PUSH 0XF0;
				PUSH 0;
				PUSH 0;
				MOV EAX, 0x41E691;
				CALL EAX;
			}break;
		case VK_F2:
			gstr.AppendFormat(L" F2_UP,����ͼ%X", g_this);
			_asm
			{
				MOV ECX, g_this;
				PUSH 0XF1;		//F1����ͼ,F2����
				PUSH 0;
				PUSH 0;
				MOV EAX, 0x41E691;
				CALL EAX;
			}break;
		case VK_F3:
			gstr.AppendFormat(L" F3_UP,����%X", g_this);
			_asm
			{
				MOV ECX, g_this;
				PUSH 0XF3;
				PUSH 0;
				PUSH 0;
				MOV EAX, 0x41E691;
				CALL EAX;
			}break;
		case VK_F4:
			gstr.AppendFormat(L" F4_UP,ը��%X", g_this);
			_asm
			{
				MOV ECX, g_this;
				PUSH 0XF4;
				PUSH 0;
				PUSH 0;
				MOV EAX, 0x41E691;
				CALL EAX;
			}break;
		case VK_F5:
			gstr.AppendFormat(L" F5_UP,����%X", g_this);


			break;
			_asm
			{
				MOV ECX, g_this;
				PUSH 0XF5;	//����
				PUSH 0;
				PUSH 0;
				MOV EAX, 0x41E691;
				CALL EAX;
			}break;
		case VK_F6:
			gstr.AppendFormat(L" F6_UP,����%X", g_this);
			_asm
			{
				MOV ECX, g_this;
				PUSH 0XF6;
				PUSH 0;
				PUSH 0;
				MOV EAX, 0x41E691;
				CALL EAX;
			}break;
		case VK_F7:
			gstr.AppendFormat(L" F7_UP,·��%X", g_this);
			_asm
			{
				MOV ECX, g_this;
				PUSH 0XF7;
				PUSH 0;
				PUSH 0;
				MOV EAX, 0x41E691;
				CALL EAX;
			}break;
		default:
			break;
		}
	}
	OutputDebugString(gstr);
	return CallNextHookEx(0, nCode, wParam, lParam);
}

/*	�������̻߳ص������ڴ�������
*/
unsigned __stdcall ThreadProc()
{
	AttachThreadInput(GetCurrentThreadId(), g_CGame.GetTid(), TRUE);
	CMyDlg dlg;
	g_CGame.CLLK.InitLLK();

	dlg.DoModal();
	return 0;
}


int DLLGAME_Init(HMODULE hMFCMod)
{
	g_CGame._MFCMOD = hMFCMod;
	gstr.Format(L"DLLGAME_Init��%p,%p,%p\n", g_CGame._DLLMOD, g_CGame._MFCMOD, &g_CGame);
	OutputDebugStringW(gstr);
	return (int)&g_CGame;
}


int DLLGAME_HOOK(int TID)
{
	HHOOK hHook = SetWindowsHookExA(WH_SHELL, ShellProcMy,
		g_CGame._DLLMOD, TID);
	OutputDebugStringW(hHook ? L"��ǰ�װ�ɹ�\n" : L"��ǰ�װʧ��\n");
	return (int)hHook;
}

int DLLGAME_HOOK_KEY(int TID)
{
	HHOOK hHook = SetWindowsHookExA(WH_KEYBOARD, KeyboardProcMy,
		g_CGame._DLLMOD, TID);
	OutputDebugStringW(hHook ? L"���̰�װ�ɹ�\n" : L"���̰�װʧ��\n");
	return (int)hHook;
}

#pragma region �ڴ���
void CLLK::InitLLK()
{
	//��ʼ���༶ָ��
	DWORD map[4] = { 0x45DCF8 ,0x1E84 ,4 };
	map[3] = this->ReadProcessINT(map, 3);

	//��ʼ��ȫ������
	this->mmap = (char*)map[3];
	g_map = this->mmap;
	g_this = map[3] - 0x14C8;
	g_nums = (LPDWORD)(map[3] + 0x39CC);

	DWORD map2[2] = { 0x45DCF8 ,0x1E84 };
	g_clean = this->ReadProcessINT(map, 2);

	//��ӡ����
	gstr.Format(L"��ͼָ��0x%p thisָ��%08X ��ѯthis %08X", mmap, g_this, g_clean);
	OutputDebugStringW(gstr);
}

DWORD CLLK::ReadProcessINT(LPDWORD list, DWORD len)
{
	DWORD read = 0;
	for (DWORD i = 0, *p; i < len; i++)
	{
		p = (DWORD*)(read + list[i]);
		read = *p;
		gstr.Format(L"��%d��ָ��0x%p %08X=%d", i + 1, p, read, read);
		OutputDebugStringW(gstr);
	}
	return read;
}


DWORD CLLK::KeyDown(WORD KEY_VK, HWND hWnd)
{
	INPUT inputs[2];
	if (hWnd)
		SetFocus(hWnd);

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki = { KEY_VK, 0, 0, 0, 0 };
	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki = { KEY_VK, 0, KEYEVENTF_KEYUP, 0, 0 };
	return SendInput(sizeof inputs / sizeof(INPUT), inputs, sizeof(INPUT));
}
#pragma endregion



#pragma region ���ں���
int CGame::InitDLL(HINSTANCE hMod, HWND wMain)
{
	_DLLMOD = hMod;
	_TID = GetCurrentThreadId();
	_PID = GetCurrentProcessId();
	this->wMain = wMain;
	gstr.Format(L"ע��TID %d,PID %d,����ַ%p,����%d", _TID, _PID, hMod, (int)wMain);
	if (wMain)
	{
		_THREAD = _beginthreadex(0, 0, (_beginthreadex_proc_type)ThreadProc, 0, 0, 0);
		gstr.AppendFormat(L" beTID %d", _THREAD);
	}
	OutputDebugString(gstr);
	return (int)_THREAD;
}


BOOL CGame::MousePost(HWND hWnd, DWORD x, DWORD y)
{
	if (hWnd > 0)
	{
		y = x + y * 65536;
		PostMessageW(hWnd, 513, 1, y);
		PostMessageW(hWnd, 514, 0, y);
	}
	return 0;
}
#pragma endregion


