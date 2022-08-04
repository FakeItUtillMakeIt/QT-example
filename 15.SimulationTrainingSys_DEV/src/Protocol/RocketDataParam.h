#pragma once

#ifndef ROCKET_DATA_PARAM_H
#define ROCKET_DATA_PARAM_H
 
#include <string>
#include <map>
#include <vector>
#include "../Model/DeviceParam.h"
using namespace std;

#define PARAM_LENGTH 9 

#define PARAM_ADDR_ID 0 
#define PARAM_ADDR_VALUE     4
#define PARAM_ADDR_STATUS    8

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
private:
	int m_id;//�����Ψһ��ʶ�������ڰ���̬�ؼ�
	int m_rocket_data_id;//ָ��id 
	int m_index;//��������˳��
	int m_length;//�����ֽڳ���
	string m_resultType;//�������� 
	DeviceParam* m_deviceParam; 

	friend class RocketDataFrame;

	void DeSerialize(unsigned char* const buff);
	void Serialize(unsigned char* const buff);
};

#endif