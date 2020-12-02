// 2012_QQMahjong.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "..\Dll_Protect\CProtect.h"
#pragma comment(lib,"..\\Debug\\Dll_Protect.lib")




int main()
{
    setlocale(LC_ALL, "chs");
    std::cout << "Hello World!\n";
    CProtect cPro;
    int ch;
    while ('e' != (ch = getchar()))
    {
        if ('m' == ch) cPro.EnumModule();
    }
}
