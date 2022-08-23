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
	int m_id;//这个是唯一标识符，用于绑定组态控件
	int m_deviceId;
	int m_rockcketid;
	string m_deviceName;
	int m_subParameterId;//外键引用parameter_info.id
	string m_subParameterName;
	string m_unit;//单位 

	double m_time;//遥测时间,相对于当日零时的纳秒数
	//下面的需要根据设备状态进行赋值
	double m_iCurValue;//连续量实时值
	string m_iCurStr;//开关量文本值

	Status m_curStatus;//当前状态
	string m_status;//参数状态

	Status m_preStatus;//上一个状态
	int m_Validity;//参数有效性 0：无效；1：有效


	map<string, vector<double>> m_statusRealData;//每个状态对应的实时值列表
	//unordered_map<int, unordered_map<string, vector<double>>> m_statusVal;//状态id对应的实时值列表
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