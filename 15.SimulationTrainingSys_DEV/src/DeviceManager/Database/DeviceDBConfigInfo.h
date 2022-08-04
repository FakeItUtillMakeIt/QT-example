#pragma once

#ifndef DEVICE_DB_CONFIG_INFO_H
#define DEVICE_DB_CONFIG_INFO_H

#include <string>
#include <map>
using namespace std;

#include <QObject>
#include <QtWidgets>
// ------------------------------------- !< �������ݿ����ͷ------------------------------------
#include "../Database/DeviceDAO.h"


/**

	@class   DeviceDBConfigInfo
	@brief   �豸�������������ù���������ݿ��Ķ�д����
	@details ~

**/
class DeviceDBConfigInfo
{

private:
	DeviceDBConfigInfo();
	~DeviceDBConfigInfo() {

	}

public:
	static DeviceDBConfigInfo* instance;
	static DeviceDBConfigInfo* getInstance() {
		if (!instance)
		{
			instance = new DeviceDBConfigInfo;
		}
		return instance;
	}

	static void closeInstance() {
		if (instance)
		{
			instance->~DeviceDBConfigInfo();
		}
	}

	//!< 	  �ĸ���Ϣ���Ϊ������
	unordered_map<int, vector<string>> paramInfo;
	unordered_map<int, vector<string>> deviceInfo;
	unordered_map<int, vector<string>> commandInfo;
	unordered_map<int, vector<string>> rocketInfo;
	//!< 	  ������
	unordered_map<int, vector<string>> switchValueInfo;
	unordered_map<int, vector<string>> switchShowInfo;
	unordered_map<int, vector<string>> statusInfo;
	unordered_map<int, vector<string>> deviceStatusInfo;
	unordered_map<int, vector<string>> deviceParamInfo;
	unordered_map<int, vector<string>> commandParamInfo;
	unordered_map<int, vector<string>> rocketDataInfo;
	unordered_map<int, vector<string>> rocketParamInfo;

	//!<	ָ���豸״̬������
	unordered_map<int, vector<string>> commandDeviceStatInfo;
	unordered_map<int, vector<string>> device2Param;
public:
	//�������豸��ָ�����ͺ�  ���ݿ�д����
	void paramConfigOp2DB(QString paramName, int paramType, QString paramUnit);
	void paramSwitchVal2DB(QString statusName);
	void paramSwitchShowInfo2DB(int bindParamID, int bindSwithValueID, int statusValIndex);

	void deviceConfigOp2DB(int rocketType, QString deviceName, int virtualDev);
	void deviceStatusInfo2DB(QString statusName);
	void deviceStatusInfo2DB(int deviceID, int statusID, QString dataPath);
	void deviceParamInfo2DB(int deviceID, int parameterID);

	void commandConfigOp2DB(QString cmdName, int rocketID, int backCmdID, int cmdCode, int cmdType, int cmdPrefix);
	void commandParamInfo2DB(int cmdID, QString paramName, int paramCode, int index, int length, QString resultType, float defaultVal);

	void rocketConfigOp2DB(QString rocketName, int rocketTypeCode);
	void rocketDataInfo2DB(int rocketID, QString cmdName, int cmdCode, int cmdPrefix);
	void rocketParamInfo2DB(int rocketDataID, int parameterID, int index, int length, int resultType);

	//�������豸��ָ�����ͺ�  ���ݿ������
	void readParamDB2UI();
	void readDeviceDB2UI();
	void readCommandDB2UI();
	void readRocketDB2UI();

	//������ر������
	void readSwitchValueDB2UI();
	void readSwitchShowInfoDB2UI();
	void readStatusInfoDB2UI();
	void readDeviceStatusDB2UI();
	void readDeviceParamDB2UI();
	void readCommandParamDB2UI();
	void readRocketDataDB2UI();
	void readRocketParamDB2UI();

	//��ָ���豸״̬��
	void readCMDDeviceStatDB2UI();
	void readDevice2ParamInfo();
	//�������ݱ���Ϣ
	void updateParamInfo2DB(int paramID, QString paramName, int paramType, QString paramUnit);
	void updateDeviceInfo2DB(int paramID, int rocketType, QString deviceName, int virtualDev);
	void updateCommandInfo2DB(int paramID, QString cmdName, int rocketID, int backCmdID, int cmdCode, int cmdType, int cmdPrefix);
	void updateRocketInfo2DB(int paramID, QString rocketName, int rocketTypeCode);
private:
	DataBase::DeviceDAO* deviceManageDBOp;


};

#endif