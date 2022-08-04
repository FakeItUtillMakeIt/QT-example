#pragma once 

#ifndef COMMAND_H
#define COMMAND_H


#include "CommandParam.h"
#include <qvariant.h>
#include <string>
#include <vector>

#include "../Model/Device.h"
using namespace std;
class Command
{
public:
	Command();
	~Command();
	int m_id;
	string m_sName;// 名称
	int m_iType; //指令类型，1表示测发指令、2表示测发回令、3表示故障指令
	int m_iCode;//指令编码,一个十六进制数
	int m_iRocketId; //火箭型号
	string m_sRocketName; //火箭型号名称
	int m_iBackId;//测发回令ID
	vector<CommandParam*> m_vCommandParam;//参数列表


};
Q_DECLARE_METATYPE(Command*);

#endif





