#pragma once
#ifndef IMPORTEXCELDAO_H
#define IMPORTEXCELDAO_H


#include <string>
#include <unordered_map>
using namespace std;

#include "../AppCache.h"
//#include "../../Public/log.h"
#include "../../Public/Utils.h" 
//#include "../../Model/Device.h"
//#include "../../Model/DeviceParam.h"
//#include "../../Model/DeviceStatus.h"
//#include "../../Model/RocketType.h"
//#include "../../Model/TaskManageInfo.h"
#include "mysql.h" 

namespace DataBase
{
	class PCTable
	{
	public:
		PCTable() {};
		~PCTable() {};
		int m_id;
		string m_tableName;// 名称
	};


	class ParamInfo
	{
	public:
		ParamInfo() {};
		~ParamInfo() {};
		int m_id;
		string m_name;// 名称
		int m_type;
		string m_unit;
		int m_rocketid;
		int m_paramtableid;
	};

    //注意》》所有：表示当前火箭的所有
	class ImpotEXcelDAO
	{
	public:
		ImpotEXcelDAO(OutputPath* path);
		~ImpotEXcelDAO();

		bool connected();
		bool connect();

		bool GetRocketType();//获取全部火箭型号

		bool GetAllParamInfo();//获取所有参数
		bool GetCommandInfo();//获取所有指令
		bool GetAllParamtable();//获取所有参数表
		bool GetAllCommandtable();//获取所有指令表

		bool ParamtableIsExist(string pTableName);//判断当前参数表是否存在，不存在则写入
		bool InsertNewParam(ParamInfo* oneParamInfo);//写入新的参数
		bool InsertParamParamtable(ParamInfo* oneParamInfo);//写入参数和参数对应关系表

		bool CommandtableIsExist(string cTableName);//判断当前指令表是否存在，不存在则写入

		bool InsertNewCommand(Command* oneCommand);//写入新的指令 
		bool InsertNewCommandBack(Command* oneCommandBack);//写入当前新指令的回令 (需要先写回令)

		bool InsertCommandCommandtable(Command* oneCommand);//写入指令和指令对应关系表

		inline void SetImportRocketID(string name)
		{ 
			for (auto item : m_allRocketTypeI)
			{
				if (item.second->m_name == name)
				{
					m_importRocketName = name;
					m_importRocketId = item.second->m_id;
					return;
				}
			}
		}

	private:
		OutputPath* m_path;
		MYSQL my_connection;
		bool is_connected;
		AppCache* m_app;

		map<int, RocketType*> m_allRocketTypeI;//全部的火箭型号
		string m_importRocketName;//需要导入的火箭型号名称
		int m_importRocketId;//需要导入的火箭型号ID

		map<int, Command*> m_allCommadI;//所有指令
		map<int, ParamInfo*> m_allParamI;//所有参数

		map<int, PCTable*> m_allCommandtableI;//所有指令表
		map<int, PCTable*> m_allParamtableI;//所有参数表


	};
}


#endif