#pragma once


#ifdef API_TEST_EXPORTS
#define TEST_API __declspec(dllexport)
#else
#define TEST_API __declspec(dllimport)
#endif

int Init();

class CGame
{
public:
	int Init();
private:

};
extern CGame* g_CGame;