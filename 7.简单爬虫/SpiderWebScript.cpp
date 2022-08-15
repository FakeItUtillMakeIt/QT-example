#include "SpiderWebScript.h"



//解析URL，得到主机名，资源名
bool SpiderWebScript::parseUrl(const string& url, string& host, string& resource) {
	if (strlen(url.c_str()) > 2000)
		return false;
	const char* headers[2] = { "https://", "http://" };
	for (int i = 0; i < 2; i++)
	{
		const char* pos = strstr(url.c_str(), headers[i]);//返回首次出现字串的位置
		if (pos == NULL) {
			pos = url.c_str();
		}
		else {
			pos += strlen(headers[i]);
			if (strstr(pos, "/") == 0)
				return false;
			char pHost[100];
			char pResource[2000];
			sscanf(pos, "%[^/]%s", pHost, pResource);
			host = pHost;
			resource = pResource;

			return true;

		}


	}

}

//使用get请求得到响应
bool SpiderWebScript::getHttpResponse(const string& url, char*& response, int& bytesRead) {
	string host, resource;
	if (!parseUrl(url, host, resource)) {
		cout << "can not parser the url" << endl;
		return false;
	}

	//建立socket

	struct hostent* hp = gethostbyname(host.c_str());

	if (hp == NULL) {
		cout << "can not find host address" << endl;
		return false;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == -1 || sock == -2) {
		cout << "can not create sock" << endl;
		return false;
	}
	//建立服务器地址
	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(80);
	memcpy(&sa.sin_addr, hp->h_addr, 4);
	//建立连接
	if (0 != connect(sock, (SOCKADDR*)&sa, sizeof(sa))) {
		cout << "can not connect:" << url << endl;
		closesocket(sock);
		return false;
	}
	//准备发送数据
	string request = "GET " + resource + " HTTP/1.1\r\nHost:" + host + "\r\nConnection:Close\r\n\r\n";
	//发送数据
	if (SOCKET_ERROR == send(sock, request.c_str(), request.size(), 0)) {
		cout << "send error" << endl;
		closesocket(sock);
		return false;
	}
	//接收数据
	int m_nContentLength = DEFAULT_PAGE_BUFF_SIZE;
	char* pagebuf = (char*)malloc(sizeof(char) * m_nContentLength);
	memset(pagebuf, 0, m_nContentLength);

	bytesRead = 0;
	int ret = 1;
	cout << "Read:";
	while (ret > 0) {
		ret = recv(sock, pagebuf + bytesRead, m_nContentLength - bytesRead, 0);
		if (ret > 0) {
			bytesRead += ret;
		}
		if (m_nContentLength - bytesRead < 100) {
			cout << "\nRealloc memorry" << endl;
			m_nContentLength *= 2;
			pagebuf = (char*)realloc(pagebuf, m_nContentLength);//重新分配内存
		}
		cout << ret << " ";
	}
	cout << endl;
	pagebuf[bytesRead] = '\0';
	response = pagebuf;
	_response = static_cast<string>(response);
	closesocket(sock);
	return true;


}

//提取所有URL及图片URL
void SpiderWebScript::HTMLParse(string& htmlresponse, vector<string>& imgurls, const string& host) {
	//找所有连接，加入queue
	const char* p = htmlresponse.c_str();
	const char* tag = "href=\"";
	const char* pos = strstr(p, tag);
	ofstream ofile("url.txt", ios::app);
	while (pos) {
		pos += strlen(tag);
		const char* nextQ = strstr(pos, "\"");
		if (nextQ) {
			char* url = new char[nextQ - pos + 1];
			sscanf(pos, "%[^\"]", url);
			string surl = url;
			if (visitedurl.find(surl) == visitedurl.end()) {
				visitedurl.insert(surl);
				ofile << surl << endl;
				hrefurl.push(surl);
			}
			pos = strstr(pos, tag);
			delete[] url;
		}
	}
	ofile << endl << endl;
	ofile.close();

	tag = "<img";
	const char* att1 = "src=\"";
	const char* att2 = "lazt-src=\"";
	const char* pos0 = strstr(p, tag);
	while (pos0)
	{
		pos0 += strlen(tag);
		const char* pos2 = strstr(pos0, att2);
		if (!pos2 || pos2 > strstr(pos0, ">")) {
			pos = strstr(pos0, att1);
			if (!pos) {
				pos0 = strstr(att1, tag);
				continue;
			}
			else {
				pos = pos + strlen(att1);

			}
		}
		else {
			pos = pos2 + strlen(att2);
		}

		const char* nextQ = strstr(pos, "\"");
		if (nextQ) {
			char* url = new char[nextQ - pos + 1];
			sscanf(pos, "%[^\"]", url);
			cout << url << endl;
			string imgurl = url;
			if (visitedimg.find(imgurl) == visitedimg.end()) {
				visitedimg.insert(imgurl);
				imgurls.push_back(imgurl);
			}
			pos0 = strstr(pos0, tag);
			delete[] url;
		}

	}
	cout << "end of parse this html" << endl;
}

//把URL转为文件名
string SpiderWebScript::toFilename(const string& url) {
	string filename;
	filename.resize(url.size());
	int k = 0;
	for (int i = 0; i < static_cast<int>(url.size()); i++) {
		char ch = url[i];
		if (ch != '\\' && ch != '/' && ch != ':' && ch != '*' && ch != '?' && ch != '"' && ch != '<' && ch != '>' && ch != '|') {
			filename[k++] = ch;
		}

	}
	return filename.substr(0, k) + ".txt";
}


void SpiderWebScript::downloadImg(vector<string>& imgurls, const string& url) {
	//生成文件夹
	string foldname = toFilename(url);
	foldname = "./img/" + foldname;
	WCHAR w_foldname[200];
	memset(w_foldname, 0, sizeof(w_foldname));

	MultiByteToWideChar(CP_ACP, 0, foldname.c_str(), strlen(foldname.c_str()) + 1, w_foldname, sizeof(w_foldname) / sizeof(w_foldname[0]));

	if (!CreateDirectory(w_foldname, NULL)) {
		cout << "can not create dictory:" << foldname << endl;
	}
	char* imgge;
	int byteread;
	for (int i = 0; i < imgurls.size(); i++) {
		//判断是否为图片
		string str = imgurls[i];
		int pos = str.find_last_of(".");
		if (pos == string::npos) {
			continue;
		}
		else {
			string ext = str.substr(pos + 1, str.size() - pos - 1);
			if (ext != "bmp" && ext != "jpg" && ext != "jpeg" && ext != "png" && ext != "gif" && ext != "mp4") {
				continue;
			}
		}
		//下载内容
		if (getHttpResponse(imgurls[i], imgge, byteread)) {
			if (strlen(imgge) == 0) {
				continue;
			}
			const char* p = imgge;
			const char* pos = strstr(p, "\r\n\r\n") + strlen("\r\n\r\n");
			int index = imgurls[i].find_last_of("/");
			if (index != string::npos) {
				string imgname = imgurls[i].substr(index, imgurls[i].size());
				ofstream ofile(foldname + imgname, ios::binary);
				if (!ofile.is_open()) {
					continue;
				}
				cout << g_imgcnt++ << foldname + imgname << endl;
				ofile.write(pos, byteread - (pos - p));
				ofile.close();
			}
			free(imgge);
		}
	}
}


void SpiderWebScript::BFS(const string& url) {
	char* response;
	int bytes;
	//获取网页响应
	if (!getHttpResponse(url, response, bytes)) {
		cout << "the url is wrong!ignore" << endl;
		return;
	}
	string httpresponse = response;
	free(response);
	string filename = toFilename(url);
	ofstream ofile("./html/" + filename);
	if (ofile.is_open()) {
		//保存网页内容
		ofile << httpresponse << endl;
		ofile.close();
	}
	vector<string> imgurls;
	//解析网页的所有图片链接，放入imgurls
	HTMLParse(httpresponse, imgurls, url);
	//下载所有图片资源
	downloadImg(imgurls, url);
}

bool SpiderWebScript::excuteScript(const string& url) {

	//初始化socket
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		return false;
	}
	//创建文件夹
	WCHAR w_img_dir[200];
	WCHAR w_html_dir[200];
	const char* img_dir = "./img";
	const char* html_dir = "./html";
	memset(w_img_dir, 0, sizeof(w_img_dir));
	memset(w_html_dir, 0, sizeof(w_html_dir));
	MultiByteToWideChar(CP_ACP, 0, img_dir, strlen(img_dir) + 1, w_img_dir, sizeof(w_img_dir) / sizeof(w_img_dir[0]));
	MultiByteToWideChar(CP_ACP, 0, html_dir, strlen(html_dir) + 1, w_html_dir, sizeof(w_html_dir) / sizeof(w_html_dir[0]));
	CreateDirectory(w_img_dir, 0);
	CreateDirectory(w_html_dir, 0);
	/*CreateDirectory("./img", 0);
	CreateDirectory("./html", 0);*/
	//遍历的起始地址
	string urlstart = url;

	BFS(urlstart);
	visitedurl.insert(urlstart);
	while (hrefurl.size() != 0)
	{
		/* code */
		string url = hrefurl.front();
		cout << url << endl;
		BFS(url);
		hrefurl.pop();
	}
	WSACleanup();
	return true;

}
