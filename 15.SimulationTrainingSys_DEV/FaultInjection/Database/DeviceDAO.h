﻿#pragma once

#include <string>
using namespace std;

#include "../AppCache.h"
#include "../../Public/log.h"
#include "../../Model/Fault.h"
#include "mysql.h" 

//#include "FaultFrameInfo.h"

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

		bool GetAllParameterInfoFrames();//获取parameter_info表信息
		bool GetAllDeviceInfoFrames();//获取device_info表信息
		bool GetAllDeviceParamInfoFrames();//获取device_param_info表信息

		int GetNowId(string sql);

	private:
		OutputPath* m_path;
		MYSQL my_connection;
		bool is_connected;
		AppCache* m_app;
	};
}
