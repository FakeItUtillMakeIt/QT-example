#include "DeviceCommonVaries.h"

DeviceCommonVaries* DeviceCommonVaries::instance = nullptr;

DeviceCommonVaries::DeviceCommonVaries() {
	commandType["测发指令"] = 1;
	commandType["测发回令"] = 2;
	commandType["故障指令"] = 3;

	commandIndex2Type[1] = "测发指令";
	commandIndex2Type[2] = "测发回令";
	commandIndex2Type[3] = "故障指令";

	paramType["连续量"] = 1;
	paramType["开关量"] = 2;

	paramIndex2Type[1] = "连续量";
	paramIndex2Type[2] = "开关量";

	paramUnit = { "伏特" , "安倍" };
	statusName["请求改平"] = 1;
	statusName["断开APCS"] = 1;
	statusName["AI交联状态"] = 1;
	statusName["通导上电"] = 1;
	statusName["目标速度门限"] = 2;
	statusName["扫描宽度"] = 2;

	statusIndex[1] = { 0,1 };
	statusIndex[2] = { 0,1 ,2,3 };
	statusIndex[3] = { 0,1 ,2,3,4,5,6,7 };
	statusIndex[4] = { 0,1 ,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

	deviceType["真实设备"] = 0;
	deviceType["虚拟设备"] = 1;

	deviceIndex2Type[0] = "真实设备";
	deviceIndex2Type[1] = "虚拟设备";


	for (int i = 0; i < 1000; i++) {
		char buffer[4];
		itoa(i, buffer, 10);
		commandCode[buffer] = i;
		commandIndex2Code[i] = buffer;
		commandParamCode[buffer] = i;
		commandIndex2ParamCode[i] = buffer;
		commandParamIndex.push_back(i + 1);
	}


	commandParamLength = { 1,2,4,8,16 };


	commandPrefix["0X55AA"] = 0;
	commandIndex2Prefix[0] = "0X55AA";

	commandParamType = { "unsigned char","unsigned short","unsigned int",
		"char","short","int","float"
	};

	commandParamDefaultVal["0XAA"] = atoi("0XAA");
	commandParamDefaultVal["-10"] = -10;

	rocketType = { "灵龙","长征" };

	for (int i = 0; i < commandParamType.size(); i++)
	{
		rocketParamType[i] = commandParamType[i];
	}

}

DeviceCommonVaries::~DeviceCommonVaries() {

}