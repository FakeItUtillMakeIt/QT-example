#pragma once

#include <WinSock2.h>
#include <QtWidgets>
#include<iostream>
#include<string>
#include<queue>
#include<fstream>
#include<vector>

#include<time.h>
#include<unordered_set>


#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")  

using namespace std;

#define DEFAULT_PAGE_BUFF_SIZE 1048576

using namespace std;
class SpiderWebScript
{
	//Q_OBJECT

private:

	queue<string> hrefurl;
	unordered_set<string> visitedurl;
	unordered_set<string> visitedimg;
	int depth = 0;
	int g_imgcnt = 1;

private:
	bool parseUrl(const string& url, string& host, string& resource);
	bool getHttpResponse(const string& url, char*& response, int& bytesRead);
	void HTMLParse(string& htmlresponse, vector<string>& imgurls, const string& host);
	string toFilename(const string& url);
	void downloadImg(vector<string>& imgurls, const string& url);
	void BFS(const string& url);

public:
	string _response;
	bool excuteScript(const string& url);
};

