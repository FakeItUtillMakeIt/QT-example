#pragma once

#ifndef DEVICE_COMMON_VARIES_H
#define DEVICE_COMMON_VARIES_H

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

/**

	@class   DeviceCommonVaries
	@brief   �豸�������еĳ���	 �ռ�
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

	//!<	��������
	unordered_map < string, int> paramType;
	unordered_map<int, string> paramIndex2Type;
	//!<	  ������λ
	vector<string> paramUnit;
	//!<	  ״ֵ̬��Ӧ����
	unordered_map<string, int> statusName;
	//!<      ״ֵ̬����
	unordered_map<int, vector<int>> statusIndex;
	//!<	  �豸����
	unordered_map<string, int> deviceType;
	unordered_map<int, string> deviceIndex2Type;
	//!< 	  ָ������
	unordered_map<string, int> commandType;
	unordered_map<int, string> commandIndex2Type;
	//!< 	  ָ�����
	unordered_map<string, int> commandCode;
	unordered_map<int, string> commandIndex2Code;
	//!< 	  ָ��ǰ׺
	unordered_map<string, int> commandPrefix;
	unordered_map<int, string> commandIndex2Prefix;
	//!< 	  ָ���������
	unordered_map<string, int> commandParamCode;
	unordered_map<int, string> commandIndex2ParamCode;
	//!< 	  ָ���������
	vector<int> commandParamLength;
	//!< 	  ָ���������˳��
	vector<int> commandParamIndex;
	//!< 	  ָ���������
	vector<string> commandParamType;
	//!< 	  ָ�����Ĭ��ֵ
	unordered_map<string, int> commandParamDefaultVal;
	//!< 	  ����ͺ�
	vector<string> rocketType;

	unordered_map<int, string> rocketParamType;


};



#endif