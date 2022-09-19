#pragma once
#ifndef IMPORTEXCELDAO_H
#define IMPORTEXCELDAO_H
//#pragma execution_character_set("utf-8")

#include <string>
#include <unordered_map>
using namespace std;

#include "../AppCache.h"
//#include "../../Public/log.h"
//#include "../../Public/Utils.h" 
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
		PCTable(int id, int rid, string name) { m_id = id; m_roketID = rid; m_tableName = name; };
		~PCTable() {};
		int m_id;
		int m_roketID;
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
		int m_deviceid;
	};

	class DeviceInfo
	{
	public:
		DeviceInfo() {};
		~DeviceInfo() {};
		int m_id;
		int m_rocketid;
		string m_name;// 名称
		int m_isVirtual;
	};

	//注意》》所有：表示当前火箭的所有
	class ImpotEXcelDAO
	{

	public:
		ImpotEXcelDAO(OutputPath* path);
		~ImpotEXcelDAO();

		bool connected();
		bool connect();

		void Init(string name);

		bool GetRocketType();//获取全部火箭型号
		//bool GetWholeParamInfo();//获取全部参数

		bool GetAllParamInfo();//获取当前火箭所有参数
		bool GetCommandInfo();//获取当前火箭所有指令
		bool GetAllParamtable();//获取当前火箭所有参数表
		bool GetAllCommandtable();//获取当前火箭所有指令表
		bool GetAllDeviceInfo();//获取当前火箭所有设备信息

		int ParamtableIsExist(PCTable pTable);//判断当前参数表是否存在，不存在则写入
		//bool NowParamExist(ParamInfo* oneParamInfo,int& ParamId);//判断整个参数表中是否存在该参数，不存在则写入
		int NowRocketParamExist(ParamInfo* oneParamInfo, bool& isExit);//判断当前参数是否存在,不存在则写入新的参数
		bool InsertParamParamtable(ParamInfo* oneParamInfo);//写入参数和参数对应关系表

		int CommandtableIsExist(PCTable cTable);//判断当前指令表是否存在，不存在则写入
		int NowCommandExist(Command* oneCommand);//判断当前命令是否存在,不存在则写入新的指令 
		int NowCommandBackExist(Command* oneCommandBack, bool& isExit);//写入当前新指令的回令 (需要先写回令)
		bool InsertCommandCommandtable(Command* oneCommand);//写入指令和指令对应关系表

		int DeviceIsExist(DeviceInfo* oneDevice);//判断当前设备是否存在，不存在则写入
		bool InsertParamDevice(ParamInfo* oneParamInfo);//写入设备和参数对应关系表


		inline int GetRocketID()
		{
			return m_importRocketId;
		}

	private:
		OutputPath* m_path;
		MYSQL my_connection;
		bool is_connected;
		AppCache* m_app;

	public:

		map<int, RocketType*> m_allRocketTypeI;//全部的火箭型号
		string m_importRocketName;//需要导入的火箭型号名称
		int m_importRocketId;//需要导入的火箭型号ID

		//map<int, ParamInfo*> m_wholeParamI;//全部参数

		map<int, Command*> m_allCommadI;//当前火箭所有指令
		map<int, ParamInfo*> m_allParamI;//当前火箭所有参数

		map<int, PCTable*> m_allCommandtableI;//当前火箭所有指令表
		map<int, PCTable*> m_allParamtableI;//当前火箭所有参数表

		map<int, DeviceInfo*> m_allDeviceI;//当前火箭所有参数
	};
}


#endif