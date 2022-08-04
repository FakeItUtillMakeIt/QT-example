#pragma once

#ifndef DEVICE_COMMON_VARIES_H
#define DEVICE_COMMON_VARIES_H

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

/**

	@class   DeviceCommonVaries
	@brief   设备管理器中的常量	 空间
	@details ~

**/
class DeviceCommonVaries
{
private:
	DeviceCommonVaries();
	~DeviceCommonVaries();

public:
	static DeviceCommonVaries* instance;
	static DeviceCommonVaries* getInstance() {
		if (instance == nullptr)
		{
			instance = new	DeviceCommonVaries;

		}

		return instance;
	}


public:

	//!<	参数类型
	unordered_map < string, int> paramType;
	unordered_map<int, string> paramIndex2Type;
	//!<	  参数单位
	vector<string> paramUnit;
	//!<	  状态值对应解释
	unordered_map<string, int> statusName;
	//!<      状态值索引
	unordered_map<int, vector<int>> statusIndex;
	//!<	  设备类型
	unordered_map<string, int> deviceType;
	unordered_map<int, string> deviceIndex2Type;
	//!< 	  指令类型
	unordered_map<string, int> commandType;
	unordered_map<int, string> commandIndex2Type;
	//!< 	  指令编码
	unordered_map<string, int> commandCode;
	unordered_map<int, string> commandIndex2Code;
	//!< 	  指令前缀
	unordered_map<string, int> commandPrefix;
	unordered_map<int, string> commandIndex2Prefix;
	//!< 	  指令参数编码
	unordered_map<string, int> commandParamCode;
	unordered_map<int, string> commandIndex2ParamCode;
	//!< 	  指令参数长度
	vector<int> commandParamLength;
	//!< 	  指令参数索引顺序
	vector<int> commandParamIndex;
	//!< 	  指令参数类型
	vector<string> commandParamType;
	//!< 	  指令参数默认值
	unordered_map<string, int> commandParamDefaultVal;
	//!< 	  火箭型号
	vector<string> rocketType;

	unordered_map<int, string> rocketParamType;


};



#endif