#pragma once

#include <string>
using namespace std;

#include "../AppCache.h"
#include "../../Public/log.h"
#include "../../Model/Fault.h"
#include "mysql.h" 
#include "../../Public/Utils.h" 
#include "../../Model/Device.h"
#include "../../Model/DeviceParam.h"
#include "../../Model/RocketType.h"
//#include "FaultFrameInfo.h"

namespace DataBaseF
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
		bool getRocketType();
		bool GetAllParameterInfoFrames();//获取parameter_info表信息
		bool GetAllDeviceInfoFrames();//获取device_info表信息
		bool GetAllDeviceParamInfoFrames();//获取device_param_info表信息
		bool GetDeviceIDParamIDFrames();//获取设备id和参数id对应项（联表查询）
		int GetNowId(string sql);
		int queryMysql(std::string sql_str, unordered_map<int, vector<string>>& contents);
	private:
		OutputPath* m_path;
		MYSQL my_connection;
		bool is_connected;
		AppCache* m_app;
	};
}
