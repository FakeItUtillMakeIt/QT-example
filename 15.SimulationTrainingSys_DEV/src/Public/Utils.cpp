#include "Utils.h"

#include <boost/regex.hpp>

Utils::Utils()
{}

Utils::~Utils()
{}

string Utils::GetExePath()
{
	return "";
}

void Utils::SplitString(string origin, string spliter, vector<string> &items, bool b)
{
	//boost::split(items, origin, boost::is_any_of(":"), boost::token_compress_on);
	boost::regex reg(spliter);
	boost::sregex_token_iterator it(origin.begin(), origin.end(), reg, -1);
	boost::sregex_token_iterator end;
	while (it != end)
	{
		items.push_back(*it++);
	}
}

int Utils::AsInt(string str)
{
	return atoi(str.c_str());
}

unsigned int Utils::GetHashKey(unsigned short tableNum, unsigned short codeNum)
{
	return tableNum * 65535 + codeNum;
}

int Utils::GetResultTypeID(string strResultType)
{
	if(strResultType == "double")
        return RESULT_TYPE_DOUBLE;
	else if (strResultType == "unsigned long long")
		return RESULT_TYPE_ULONGLONG;
	else if (strResultType == "long long")
		return RESULT_TYPE_LONGLONG;
	else if (strResultType == "float")
        return RESULT_TYPE_FLOAT;
	else if (strResultType == "unsigned int")
		return RESULT_TYPE_UINT;
	else if (strResultType == "int")
        return RESULT_TYPE_INT;
	else if (strResultType == "unsigned short")
		return RESULT_TYPE_USHORT;
	else if (strResultType == "short")
        return RESULT_TYPE_SHORT;
	else if (strResultType == "unsigned char")
		return RESULT_TYPE_UCHAR;
	else if (strResultType == "char")
        return RESULT_TYPE_CHAR;
	else
		return RESULT_TYPE_UINT;
}

string Utils::inttostring(int a)
{
	string str;
	stringstream ss;
	ss << a;
	ss >> str;
	return str;
}
/// <summary>
/// 获取纳秒数
/// </summary>
/// <param name="pData"></param>
/// <returns>相对于当日零时的纳秒数</returns>
double Utils::GetBCDTimeCode(unsigned char* pData)
{
    double d = 0.0;
    d += (*pData & 0x0F)*0.1;
    d += ((*pData & 0xF0) >> 4);
    d += (*(pData + 1) & 0x0F)*10;
    d += ((*(pData + 1) & 0xF0) >> 4)*100;
    d += (*(pData + 2) & 0x0F)*1000;
    d += ((*(pData + 2) & 0xF0) >> 4)*10000;
    d += (*(pData + 3) & 0x0F)*60000;
    d += ((*(pData + 3) & 0xF0) >> 4)*600000;
    d += (*(pData + 4) & 0x0F)*3600000;
    d += ((*(pData + 4) & 0x30) >> 4)*36000000;
    return d;
}

/// <summary>
/// UTF8转GBK
/// </summary>
/// <param name="strUTF8"></param>
/// <returns></returns>
std::string Utils::UTF8ToGBK(const char* strUTF8)

{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);

	wchar_t* wszGBK = new wchar_t[len + 1];

	memset(wszGBK, 0, len * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);

	char* szGBK = new char[len + 1];

	memset(szGBK, 0, len + 1);

	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);

	std::string strTemp(szGBK);

	if (wszGBK) delete[] wszGBK;

	if (szGBK) delete[] szGBK;

	return strTemp;

}
 
/// <summary>
/// GBK转UTF8
/// </summary>
/// <param name="strGBK"></param>
/// <returns></returns>
std::string Utils::GBKToUTF8(const char* strGBK)

{
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);

	wchar_t* wstr = new wchar_t[len + 1];

	memset(wstr, 0, len + 1);

	MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);

	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);

	char* str = new char[len + 1];

	memset(str, 0, len + 1);

	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);

	std::string strTemp = str;

	if (wstr) delete[] wstr;

	if (str) delete[] str;

	return strTemp;

} 