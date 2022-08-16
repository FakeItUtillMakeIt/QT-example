#include "DeviceCommonVaries.h"

DeviceCommonVaries* DeviceCommonVaries::instance = nullptr;

DeviceCommonVaries::DeviceCommonVaries() {
	commandType["�ָⷢ��"] = 1;
	commandType["�ⷢ����"] = 2;
	commandType["����ָ��"] = 3;

	commandIndex2Type[1] = "�ָⷢ��";
	commandIndex2Type[2] = "�ⷢ����";
	commandIndex2Type[3] = "����ָ��";

	paramType["������"] = 1;
	paramType["������"] = 2;

	paramIndex2Type[1] = "������";
	paramIndex2Type[2] = "������";

	paramUnit = { "����" , "����" };
	statusName["�����ƽ"] = 1;
	statusName["�Ͽ�APCS"] = 1;
	statusName["AI����״̬"] = 1;
	statusName["ͨ���ϵ�"] = 1;
	statusName["Ŀ���ٶ�����"] = 2;
	statusName["ɨ����"] = 2;

	statusIndex[1] = { 0,1 };
	statusIndex[2] = { 0,1 ,2,3 };
	statusIndex[3] = { 0,1 ,2,3,4,5,6,7 };
	statusIndex[4] = { 0,1 ,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

	deviceType["��ʵ�豸"] = 0;
	deviceType["�����豸"] = 1;

	deviceIndex2Type[0] = "��ʵ�豸";
	deviceIndex2Type[1] = "�����豸";


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

	rocketType = { "����","����" };

	for (int i = 0; i < commandParamType.size(); i++)
	{
		rocketParamType[i] = commandParamType[i];
	}

}

DeviceCommonVaries::~DeviceCommonVaries() {

}