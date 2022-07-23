#pragma once

#ifndef DEVICE_DAO_H
#define DEVICE_DAO_H


#include <string>
#include <unordered_map>
using namespace std;

#include "../AppCache.h"
#include "../../Public/log.h"
#include "../../Model/Device.h"
#include "mysql.h" 

namespace DataBase
{
	class DeviceDAO
	{
	public:
		DeviceDAO(OutputPath* path);
		~DeviceDAO();
		bool exec_sql(string sql);
		bool connected();
		bool connect();
		bool test();
		bool getDevice();
		int queryMysql(std::string sql_str, unordered_map<int, vector<string>>& contents);
	private:
		OutputPath* m_path;
		MYSQL my_connection;
		bool is_connected;
		AppCache* m_app;
	};
}


#endif