#pragma once

#include <string>
using namespace std;

/**
* @brief ����boost����ļ���Ŀ¼������ \n
*/
class DirOperation
{
public:
	DirOperation();
	~DirOperation();

	/**
	* @brief ����Ŀ¼
	* @param[in] dir �ļ���·��
	* @return �����ɹ����
	*        -<em>false</em> ʧ��
	*        -<em>true</em> �ɹ�
	*/
	static bool CreateDir(string dir);
	/**
	* @brief ��ȡ��ִ�г����ִ��·��
	* @return ִ��·��
	*/
	static string GetExePath();
	/**
	* @brief ���������ļ�·��Ϊһ���ļ�·��
	* @param[in] path1 �ļ�·��1
	* @param[in] path2 �ļ�·��2
	* @return ���Ӻ���ļ�·��
	*/
	static string ConcatePath(string path1, string path2);
private:

};

