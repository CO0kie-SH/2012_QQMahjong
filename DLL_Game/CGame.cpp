#include "pch.h"
#include "CGame.h"

CGame* g_CGame = nullptr;

int Init()
{
	OutputDebugString(L"我是初始化");
	return 0;
}



#pragma region 类内函数
int CGame::Init()
{
	return 0;
}
#pragma endregion

