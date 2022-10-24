#pragma once

#ifndef DEVICE_DAO1_H
#define DEVICE_DAO1_H


#include <string>
#include <unordered_map>
using namespace std;

#include "../AppCache.h"
#include "../../Public/log.h"
#include "../../Public/Utils.h" 
#include "../../Model/Device.h"
#include "../../Model/DeviceParam.h"
#include "../../Model/DeviceStatus.h"
#include "../../Model/RocketType.h"
#include "../../Model/TaskManageInfo.h"
#include "mysql.h" 

namespace DataBase
{
	class DeviceDAO
	{
	public:
		DeviceDAO(OutputPath* path);
		~DeviceDAO();
		bool exec_sql(string sql);
		bool Isexist(string name);
		bool connected();
		bool connect();
		bool test();
		bool getRocketType();
		bool getDevice(); 
		bool getDeviceParam();
		bool getDeviceStatus();
		bool getStatus();
		int queryMysql(std::string sql_str, unordered_map<int, vector<string>>& contents);
	private:
		OutputPath* m_path;
		MYSQL my_connection;
		bool is_connected;
		AppCache* m_app;
	};
}


#endif