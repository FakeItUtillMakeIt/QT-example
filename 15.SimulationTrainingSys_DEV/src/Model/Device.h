#pragma once

#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QtWidgets>
#include <string>
#include <map>


using namespace std;
class Device :public QObject
{
	Q_OBJECT
public:
	Device();
	~Device();

	int m_id;
	string m_deviceName;
	int isVirtual; //0��ʾ��ʵ�豸��1��ʾ�����豸
	int m_iRocketId; //����ͺ�
	string m_sRocketName; //����ͺ�����

	string m_sCurStatus;//��ǰ״̬����硢�ϵ硢����

};



#endif