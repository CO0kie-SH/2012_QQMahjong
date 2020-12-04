// 2012_QQMahjong.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <vector>
using std::vector;


#include "..\Dll_Protect\CProtect.h"
#pragma comment(lib,"..\\Debug\\Dll_Protect.lib")


static void showBy(byte& buf)
{
	char* str = 0;
	switch (buf)
	{
	case 0x00:	printf("　");	break;
	case 0x02:	printf("章");	break;
	case 0x03:	printf("星");	break;
	case 0x04:	printf("桃");	break;
	case 0x05:	printf("鲸");	break;
	case 0x06:	printf("瓜");	break;
	case 0x07:	printf("GG");	break;
	case 0x08:	printf("MM");	break;
	case 0x09:	printf("猫");	break;
	case 0x0A:	printf("鸡");	break;
	case 0x0D:	printf("黄");	break;
	case 0x0E:	printf("红");	break;
	case 0x0F:	printf("黑");	break;
	case 0x10:	printf("搜");	break;
	case 0x11:	printf("紫");	break;
	case 0x12:	printf("白");	break;
	case 0x13:	printf("拳");	break;
	case 0x14:	printf("布");	break;
	case 0x15:	printf("耶");	break;
	case 0x16:	printf("墙");	break;
	case 0x18:	printf("折");	break;
	case 0x19:	printf("钟");	break;
	case 0xF2:	printf("禁");	break;
	case 0xF3:	printf("闹");	break;
	case 0xF4:	printf("炸");	break;
	case 0xF6:	printf("门");	break;
	case 0xF7:	printf("障");	break;
	default:	printf("%02X", buf);	break;
	}
}

byte show(char* map, int x, int y, bool isPr = false)
{
	if (x == 0 || x > 19 || y == 0 || y > 11)
		return 0xFF;
	byte buf = map[--y * 19 + --x];
	if (isPr) showBy(buf);
	return buf;
}

void showMap(char* map)
{
	for (DWORD i = 1; i <= 19; i++)
	{
		printf("%2d", i);
	}
	printf("\n");
	for (DWORD y = 1, add = 0; y <= 11; y++)
	{
		for (DWORD x = 1; x <= 19; x++)
		{
			byte buf = map[add++];
			showBy(buf);
		}
		printf("\n");
	}
}


typedef struct _MyStruct
{
	byte X;
	byte Y;
	byte N;
	byte B;
}MyStruct;

BOOL clear1(char* map, int x1, int y1, int x2, int y2)
{
	if (x1 == x2)
	{
		printf("X相同，上下找Y：\n(%2d,%2d)A=", x1, y1);
		int maxy = y1 > y2 ? y1 : y2,
			y = y1 < y2 ? y1 : y2;
		byte buf = show(map, x1, y1, true);
		printf("B=(%2d,%2d)", x2, y2);
		while (++y <= maxy)
		{
			printf("[%2d,%2d]", x1, y);
			byte tmp = show(map, x1, y, true);
			if (tmp == 0)
				continue;
			else if (tmp == buf)
				return TRUE;
			else{
				puts("←上下被阻塞");
				return FALSE;
			}
		}
		return TRUE;
	}
	else if (y1 == y2)
	{
		printf("Y相同，左右找X\n(%2d,%2d)A=", x1, y1);		//X不变，左右找Y
		byte buf = show(map, x1, y1);
		if (buf == 0)
			buf = show(map, x2, y2);
		showBy(buf);
		printf("B=(%2d,%2d)", x1, y2);
		while (x1 != x2)
		{
			x1 = x1 > x2 ? x1 - 1 : x1 + 1;
			printf("[%2d,%2d]", x1, y1);
			byte tmp = show((char*)map, x1, y1, true);
			if (tmp == 0)
				continue;
			else if (tmp == buf)
				return TRUE;
			else{
				puts("←左右被阻塞");
				return FALSE;
			}
		}
		return TRUE;
	}
	return 0;
}
BOOL clear2(byte* map, MyStruct& oldMy, MyStruct& newMy)
{
	BOOL bret = 0, dir = 0;
	printf("\nB=(%2d,%2d)", newMy.X, newMy.Y);
	showBy(newMy.B);
	printf("\t比较：");

	//如果0折，则返回成功
	if (clear1((char*)map, oldMy.X, oldMy.Y, newMy.X, newMy.Y))
		return TRUE;
	else
		printf("\t单连失败：");
	//计算象限
	if (newMy.X > oldMy.X)
		dir = newMy.Y > oldMy.Y ? 4 : 1;
	else
		dir = newMy.Y > oldMy.Y ? 3 : 2;
	printf("第%d象限\t", dir);

	if (dir == 3)
	{
		if (clear1((char*)map, oldMy.X, oldMy.Y, oldMy.X, newMy.Y) //↓查找，x不变
			//clear1((char*)map, newMy.X, newMy.Y, oldMy.X, newMy.Y));//←查找，y不变
			) {
			printf("上下可连接，找左右\t");
			if (clear1((char*)map, newMy.X, newMy.Y, oldMy.X, newMy.Y))//←查找，y不变)
			{
				printf("可连接\n");
				return 0;
			}
		}
		else if (clear1((char*)map, newMy.X, oldMy.Y, oldMy.X, oldMy.Y) //←查找，y不变
			) {
			//if (clear1((char*)map, newMy.X, newMy.Y, oldMy.X, newMy.Y))//←查找，y不变)
				printf("左右可连接，找上下\t");
		}
	}
	return 0;


	//计算
	if (dir == 5)
	{
		MyStruct tmpMy = {
			oldMy.X,	//↓查找，x不变
			newMy.Y,	//
			0,oldMy.B
		};
		if (clear2(map, newMy, tmpMy))
		{
			//printf("【↓向连接】");
			if (clear2(map, oldMy, tmpMy))
			{
				//printf("【可连接】");
				bret = TRUE;
			}
		}
		MyStruct tmpMy2 = {
			newMy.X,	//
			oldMy.Y,	//←查找，y不变
			0,oldMy.B
		};
		if (clear2(map, newMy, tmpMy2))
		{
			//printf("【←向连接】\n");
			//printf("A=(%2d,%2d)", oldMy.X, oldMy.Y);
			if (clear2(map, oldMy, tmpMy2))
			{
				//printf("【可连接】");
				bret = TRUE;
			}
		}
	}
	printf("\n\n");
	return bret;
}

void MousePostW(int x, int y)
{
	HWND hWnd = (HWND)1248770;
	y = x * 31 + (170 + 35 * y) * 65536;
	PostMessageW(hWnd, 513, 1, y);
	PostMessageW(hWnd, 514, 0, y);
}

int clear2(char* map)
{
	vector<MyStruct> open;
	vector<COORD> pos;
	for (byte y = 0, buf; y < 11; y++)
	{
		for (byte x = 0; x < 19; x++)
		{
			if (buf = map[y * 19 + x])
				open.push_back(
					{
						(byte)(x + 1),
						(byte)(y + 1),
						(byte)(y * 19 + x),buf
					});
		}
	}
	byte map2[19 * 11];
	memcpy(map2, map, 19 * 11);

	for (size_t i = 0, max = open.size(); i < max; i++)
	{
		MyStruct& oldMy = open[i];
		printf("A=(%2d,%2d)", oldMy.X, oldMy.Y);
		showBy(oldMy.B);
		for (size_t j = 0; j < max; j++)
		{
			if (i == j) continue;
			MyStruct& newMy = open[j];
			if (newMy.B && oldMy.B == newMy.B &&
				clear2(map2, oldMy, newMy))
			{
				printf("存在连接");
				pos.push_back({ oldMy.X,oldMy.Y });
				pos.push_back({ newMy.X,newMy.Y });
				oldMy.B = 0;
				newMy.B = 0;
				map2[oldMy.N] = 0;
				map2[newMy.N] = 0;
			}
		}
		printf("\n");
		oldMy.B = 0x00;
	}
	
	if (pos.size()) {
		MousePostW(0, 0);
		auto begin = pos.begin();
		while (begin != pos.end())
		{
			printf("(%2d,%2d)", (*begin).X, (*begin).Y);
			show(map, (*begin).X, (*begin).Y, true);
			getchar();
			MousePostW((*begin).X, (*begin).Y);
			++begin;
			printf("(%2d,%2d)\n", (*begin).X, (*begin).Y);
			show(map, (*begin).X, (*begin).Y, true);
			Sleep(1000);
			MousePostW((*begin).X, (*begin).Y);
			++begin;
		}
	}
	return 0;
}


int main()
{
	char buff[260];
	int ch;

	setlocale(LC_ALL, "chs");
	std::cout << "Hello World!\n";
	CProtect cPro;
	DWORD PID;
	HWND hWnd = FindWindow(L"#32770", L"QQ连连看");
	GetWindowThreadProcessId(hWnd, &PID);
	HANDLE hPro = OpenProcess(PROCESS_VM_READ, 0, PID);
	ReadProcessMemory(hPro, (LPVOID)0x19BB34, buff, 260, 0);

	showMap(buff + 8);

	while ('e' != (ch = getchar()))
	{
		if (ch == 'm') cPro.EnumModule();
		else if (ch == 's') {
			show(buff + 8, 19, 1, true);
			show(buff + 8, 18, 2, true);
			show(buff + 8, 19, 3, true);
			show(buff + 8, 19, 11, true);
			printf("\n");
		}
		else if (ch == '1')
		{
			DWORD t = 666 + 533 * 65536;
			PostMessageA(hWnd, 512, 2, t);
			PostMessageA(hWnd, 513, 1, t);
			PostMessageA(hWnd, 514, 0, t);
		}
		else if (ch == '2')
		{
			ReadProcessMemory(hPro, (LPVOID)0x19BB34, buff, 260, 0);
			showMap(buff + 8);
			clear2(buff + 8);
		}
	}
	CloseHandle(hPro);
}
