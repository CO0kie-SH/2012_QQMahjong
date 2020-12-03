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
	case 0xF3:	printf("炸");	break;
	case 0xF4:	printf("炸");	break;
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

int clear1(char* map)
{
	for (byte y = 1, buf = 0; y <= 11; y++)
	{
		for (byte x = 1; x <= 19; x++)
		{
			if ((buf = show(map, x, y)) == 0x00)
				continue;
			//printf("开始检查(%d,%d)\t", x, y);
			//showBy(buf);


			//向右搜索
			byte xmm = x, xpp = x, ymm = y, ypp = y, tmp = 0;
			while (++xpp <= 19)
			{
				tmp = show(map, xpp, y);
				if (tmp == buf)
				{
					printf("找到横向，(%d,%d)%X---(%d,%d)%X\n", x, y, buf, xpp, y, buf);
					map[(y - 1) * 19 + (x - 1)] = 0x00;
					map[(y - 1) * 19 + (xpp - 1)] = 0x00;
				}
				else if (tmp > 0)
					break;
			}
			while (++ypp<=11)
			{
				tmp = show(map, x, ypp);
				if (tmp == buf)
				{
					printf("找到纵向，(%d,%d)%X---(%d,%d)%X\n", x, y, buf, x, ypp, buf);
					map[(y - 1) * 19 + (x - 1)] = 0x00;
					map[(ypp - 1) * 19 + (x - 1)] = 0x00;
				}
				else if (tmp > 0)
					break;
			}
			//搜索结束
		}
	}
	showMap(map);
	printf("\n");
	return 0;
}


typedef struct _MyStruct
{
	byte X;
	byte Y;
	byte N;
	byte B;
}MyStruct;

BOOL clear2(byte* map, MyStruct& oldMy, MyStruct& newMy)
{
	BOOL bret = 0, dir = 0;
	printf("\nB=(%2d,%2d)", newMy.X, newMy.Y);
	showBy(newMy.B);
	printf("\t比较：");
	if (oldMy.X == newMy.X)
	{
		puts("X相同，上下找Y");		//X不变，上下找Y
		int maxy = oldMy.Y > newMy.Y ? oldMy.Y : newMy.Y,
			y = oldMy.Y < newMy.Y ? oldMy.Y : newMy.Y;
		while (++y <= maxy)
		{
			printf("[%2d,%2d]", oldMy.X, y);
			byte tmp = show((char*)map, oldMy.X, y, true);
			if (tmp == 0)
				continue;
			else if (tmp == oldMy.B)
				return TRUE;
			else if (tmp != oldMy.B) {
				printf("←被阻塞");
				return FALSE;
			}
		}
		return TRUE;
	}
	else if (oldMy.Y == newMy.Y)
	{
		puts("Y相同，左右找X");		//X不变，左右找Y
		int maxx = oldMy.X > newMy.X ? oldMy.X : newMy.X,
			x = oldMy.X < newMy.X ? oldMy.X : newMy.X;
		while (++x <= maxx)
		{
			printf("[%2d,%2d]", x, oldMy.Y);
			byte tmp = show((char*)map, x, oldMy.Y, true);
			if (tmp == 0)
				continue;
			else if (tmp == oldMy.B)
				return TRUE;
			else if (tmp != oldMy.B) {
				printf("←被阻塞");
				return FALSE;
			}
		}
		return TRUE;
	}

	//计算象限
	if (newMy.X > oldMy.X)
		dir = newMy.Y > oldMy.Y ? 4 : 1;
	else
		dir = newMy.Y > oldMy.Y ? 3 : 2;
	printf("第%d象限", dir);

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
		return 0;
		MousePostW(0, 0);
		auto begin = pos.begin();
		while (begin != pos.end())
		{
			printf("(%2d,%2d)", (*begin).X, (*begin).Y);
			show(map, (*begin).X, (*begin).Y, true);
			MousePostW((*begin).X, (*begin).Y);
			++begin;
			printf("(%2d,%2d)\n", (*begin).X, (*begin).Y);
			show(map, (*begin).X, (*begin).Y, true);
			Sleep(1000);
			MousePostW((*begin).X, (*begin).Y);
			++begin;
			getchar();
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

	HANDLE hPro = OpenProcess(PROCESS_VM_READ, 0, 11616);
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
			clear1(buff + 8);
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
