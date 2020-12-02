#pragma once
#include <iostream>


#ifdef API_TEST_EXPORTS
#define TEST_API __declspec(dllexport)
#else
#define TEST_API __declspec(dllimport)
#endif



class TEST_API CProtect
{
public:
	int EnumModule();
};

