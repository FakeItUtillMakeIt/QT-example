#pragma once

#ifndef DEVICE_STATUS_H
#define DEVICE_STATUS_H


#include <string>
#include <map>
using namespace std;
class DeviceStatus
{
public:
	DeviceStatus();
	~DeviceStatus();

	int m_id;
	int m_deviceId; //�豸ID
	string m_statusName; //״̬����
	string m_dataPath; //ʵʱֵ�ļ�·�� 
};

#endif