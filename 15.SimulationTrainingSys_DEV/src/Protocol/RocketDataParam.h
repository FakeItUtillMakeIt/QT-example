#pragma once

#ifndef ROCKET_DATA_PARAM_H
#define ROCKET_DATA_PARAM_H
 
#include <string>
#include <map>
#include <vector>
#include "../Model/DeviceParam.h"
using namespace std;

#define PARAM_LENGTH 13 

#define PARAM_ADDR_ID 0 
#define PARAM_ADDR_VALUE     4
#define PARAM_ADDR_STATUS    12

class RocketDataFrame;
class RocketDataParam
{
public:
	RocketDataParam();
	~RocketDataParam();
	int Id() const { return m_id; } 
	double CurValue() const { return m_deviceParam->m_iCurValue; }
	unsigned char CurStatus() const { return m_deviceParam->m_curStatus.m_id; }
	int Size();

	int m_id;//这个是唯一标识符，用于绑定组态控件
	int m_rocketDataId;//指令id 
	int m_index;//参数索引顺序
	int m_length;//参数字节长度
	string m_resultType;//参数类型 
	DeviceParam* m_deviceParam;

private:

	friend class RocketDataFrame;

	void DeSerialize(unsigned char* const buff);
	void Serialize(unsigned char* const buff);
};

#endif