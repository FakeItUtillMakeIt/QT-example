#pragma once

#ifndef ROCKETDATA_DAO_H
#define ROCKETDATA_DAO_H


#include <string>
#include <unordered_map>
using namespace std;

#include "../AppCache.h"
#include "../../Public/log.h"
#include "../../Public/Utils.h" 
#include "../../Protocol/RocketDataFrame.h"
#include "../../Protocol/RocketDataParam.h"
#include "mysql.h" 

namespace DataBase
{
	class RocketDataDAO
	{
	public:
		RocketDataDAO(OutputPath* path);
		~RocketDataDAO();
		bool exec_sql(string sql);
		bool connected();
		bool connect();
		bool test();
		bool getRocketData(); 
		bool getRocketParam();
	private:
		OutputPath* m_path;
		MYSQL my_connection;
		bool is_connected;
		AppCache* m_app;
	};
}


#endif