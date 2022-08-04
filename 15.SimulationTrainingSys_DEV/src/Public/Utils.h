#pragma once

#include <string>
#include <vector>
#include <sstream>
using namespace std;

#include "MacroDefs.h" 

class Utils
{
public:
	Utils();
	~Utils();

	static string GetExePath();
	static void SplitString(string origin, string spliter, vector<string> &items, bool b);
	static int AsInt(string str);
	static unsigned int GetHashKey(unsigned short tableNum, unsigned short codeNum);
    static double GetBCDTimeCode(unsigned char* pData);
    static int GetResultTypeID(string strResultType);
	static string inttostring(int a);
	static string UTF8ToGBK(const char* strUTF8);
	static string GBKToUTF8(const char* strGBK);
private:

};
