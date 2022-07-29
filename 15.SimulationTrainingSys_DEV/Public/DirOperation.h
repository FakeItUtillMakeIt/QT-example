#pragma once

#include <string>
using namespace std;

/**
* @brief 基于boost库的文件、目录操作类 \n
*/
class DirOperation
{
public:
	DirOperation();
	~DirOperation();

	/**
	* @brief 创建目录
	* @param[in] dir 文件夹路径
	* @return 创建成功与否
	*        -<em>false</em> 失败
	*        -<em>true</em> 成功
	*/
	static bool CreateDir(string dir);
	/**
	* @brief 获取可执行程序的执行路径
	* @return 执行路径
	*/
	static string GetExePath();
	/**
	* @brief 连接两个文件路径为一个文件路径
	* @param[in] path1 文件路径1
	* @param[in] path2 文件路径2
	* @return 连接后的文件路径
	*/
	static string ConcatePath(string path1, string path2);
private:

};

