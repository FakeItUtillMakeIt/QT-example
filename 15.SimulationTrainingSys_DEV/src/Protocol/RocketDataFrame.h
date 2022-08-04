#pragma once
#include <QObject> 
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "RocketDataParam.h"
#include "FrameHead.h"
using namespace std;

/**
* @brief 内部网络通信数据帧格式定义 \n
* 表示形式为1个信息头+n个信息字
*/
class RocketDataFrame: public QObject
{
	Q_OBJECT

public:
	RocketDataFrame(QObject* parent = 0);
	RocketDataFrame(RocketDataFrame& frame, QObject* parent = 0);
	RocketDataFrame& operator= (RocketDataFrame& frame);
	~RocketDataFrame();

	FrameHead& head(); 
	vector<RocketDataParam>& params();

	/**
	* @brief 将信息头、信息字中的内容序列化到pData缓冲区
	* @param pData 缓冲区
	* @param iLength 缓冲区最大长度
	* @return 序列化成功与否
	*        -<em>false</em> 失败
	*        -<em>true</em> 成功
	*/
    bool Serialize(unsigned char* pData,int iLength);
	/**
	* @brief 将pData缓冲区中的数据反序列化到信息头、信息字
	* @param pData 缓冲区
	* @param iLength 缓冲区中有效数据长度
	* @return 序列化成功与否
	*        -<em>false</em> 失败
	*        -<em>true</em> 成功
	*/
    bool DeSerialize(unsigned char* pData,int iLength);
	/**
	* @brief 数据帧的大小 
	* @return 序列化成功与否
	*/
	unsigned short Size(); 
  
private: 
	///信息头
	FrameHead m_Head;
	/// 参数 
	vector<RocketDataParam> m_vParams;


	string m_error_info;

	unsigned char CheckSum(unsigned char* pData, int iLength);
};
