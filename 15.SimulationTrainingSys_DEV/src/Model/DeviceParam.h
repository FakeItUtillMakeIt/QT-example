#pragma once

#ifndef DEVICE_PARAM_H
#define DEVICE_PARAM_H


#include <QObject>
#include <QtWidgets>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include "Status.h"



using namespace std;
class DeviceParam :public  QObject
{
	Q_OBJECT
public:
	DeviceParam();
	~DeviceParam();
	int m_id;//�����Ψһ��ʶ�������ڰ���̬�ؼ�
	int m_deviceId;
	int m_rockcketid;
	string m_deviceName;
	int m_subParameterId;//�������parameter_info.id
	string m_subParameterName;
	string m_unit;//��λ 

	double m_time;//ң��ʱ��,����ڵ�����ʱ��������
	//�������Ҫ�����豸״̬���и�ֵ
	double m_iCurValue;//������ʵʱֵ
	string m_iCurStr;//�������ı�ֵ

	Status m_curStatus;//��ǰ״̬
	string m_status;//����״̬

	Status m_preStatus;//��һ��״̬
	int m_Validity;//������Ч�� 0����Ч��1����Ч


	map<string, vector<double>> m_statusRealData;//ÿ��״̬��Ӧ��ʵʱֵ�б�
	//unordered_map<int, unordered_map<string, vector<double>>> m_statusVal;//״̬id��Ӧ��ʵʱֵ�б�
	QTimer* timer;

	void setStatus(int id) { m_curStatus.m_id = id; }

private:
	int curIndex;
	vector<double> paramData;

public:
	void updateParamRealVal();

private slots:

	void timerUpdateParam();

};

#endif