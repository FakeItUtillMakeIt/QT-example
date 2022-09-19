#pragma execution_character_set("utf-8")
#include "DeviceDAOF.h"
#include <boost\lexical_cast.hpp>
 
namespace DataBaseF
{
	DeviceDAO::DeviceDAO(OutputPath* path)
		: m_path(path)
		, is_connected(false)
	{
		m_app = AppCache::instance();
	}

	DeviceDAO::~DeviceDAO()
	{
		if (is_connected)
		{
			mysql_close(&my_connection);//断开连接  
			is_connected = false;
		}
	}

	bool DeviceDAO::connect()
	{
		if (!is_connected)
		{
			if (!m_path)
			{
				LOG(INFO) << "缺少数据库连接配置信息";
				return false;
			}
			mysql_init(&my_connection);
			if (!mysql_real_connect(&my_connection, m_path->m_server.c_str(), m_path->m_user_name.c_str(),
				m_path->m_password.c_str(), m_path->m_db_name.c_str(), m_path->m_port, NULL, 0))
			{
				LOG(INFO) << "连接数据库失败";
				mysql_close(&my_connection);//断开连接  
				is_connected = false;
			}
			else
				is_connected = true;
		}
		return is_connected;
	}
	bool DeviceDAO::connected()
	{
		return is_connected;
	}
 

	/// <summary>
	/// 获取parameter_info信息
	/// </summary>
	/// <returns></returns>
	bool DeviceDAO::GetAllParameterInfoFrames()
	{
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		MYSQL_RES* result = nullptr;
		MYSQL_ROW sql_row;
		int res;
		string sql;
		sql.append("SELECT "
			"parameter_info.id,parameter_info.type,parameter_info.name,parameter_info.unit,"
			"`parameter_rocket_info`.rocket_id,parameter_info.createTime,parameter_info.lastUpdateTime "
			"FROM "
			"parameter_info "
			"INNER JOIN "
			"`parameter_rocket_info` ON parameter_info.id = `parameter_rocket_info`.`parameter_id`;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_FaultParameterInfoFrames.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					FaultParameterInfo* oneFrame = new FaultParameterInfo();
					int id = atoi(sql_row[0]);
					oneFrame->m_id = id;
					oneFrame->m_type = atoi(sql_row[1]);
					oneFrame->m_name = sql_row[2];
					oneFrame->m_unit = (sql_row[3] == nullptr) ? -1 : atoi(sql_row[3]);
					oneFrame->m_rocketId = (sql_row[4] == nullptr) ? -1 : atoi(sql_row[4]);
					oneFrame->m_createTime = sql_row[5];
					oneFrame->m_lastUpdateTime = sql_row[6];
				 
					if (oneFrame->m_rocketId != m_app->m_rockedType)
					{
						continue;
					}
					m_app->m_FaultParameterInfoFrames.insert(pair<int, FaultParameterInfo*>(id, oneFrame));
				}
			}
			else
				LOG(INFO) << "获取parameter_info数据失败";
		}
		else
			LOG(INFO) << "获取parameter_info数据失败";
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
	}
	bool DeviceDAO::getRocketType()
	{
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		MYSQL_RES* result = nullptr;
		MYSQL_ROW sql_row;
		int res;
		string sql;
		sql.append("SELECT * FROM rocket_info");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_allRocketType.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					RocketType* oneDeviceParam = new RocketType();
					oneDeviceParam->m_id = atoi(sql_row[0]);
					oneDeviceParam->m_name = Utils::UTF8ToGBK(sql_row[1]);
					oneDeviceParam->m_code = Utils::UTF8ToGBK(sql_row[2]);
					m_app->m_allRocketType.insert(pair<int, RocketType*>(oneDeviceParam->m_id, oneDeviceParam));
				}
			}
			else
				LOG(INFO) << "获取数据失败";
		}
		else
			LOG(INFO) << "获取数据失败";
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
	}
	/// <summary>
	/// 获取device_info信息
	/// </summary>
	/// <returns></returns>
	bool DeviceDAO::GetAllDeviceInfoFrames()
	{
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		MYSQL_RES* result = nullptr;
		MYSQL_ROW sql_row;
		int res;
		string sql;
		sql.append("select * from device_info;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_FaultDeviceInfoFrames.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					FaultDeviceInfo* oneFrame = new FaultDeviceInfo();
					int id = atoi(sql_row[0]);
					oneFrame->m_id = id; 
					oneFrame->m_rocketID = (sql_row[1] == nullptr) ? -1 : atoi(sql_row[1]);
					oneFrame->m_name = sql_row[2];
					oneFrame->m_isVirtual = (sql_row[3] == nullptr) ? -1 : atoi(sql_row[3]);
					oneFrame->m_createTime = sql_row[4];
					oneFrame->m_lastUpdateTime = sql_row[5];

					//对应到火箭型号
					if (oneFrame->m_rocketID != m_app->m_rockedType)
					{
						continue;
					}
					m_app->m_FaultDeviceInfoFrames.insert(pair<int, FaultDeviceInfo*>(id, oneFrame));
				}
			}
			else
				LOG(INFO) << "获取device_info数据失败";
		}
		else
			LOG(INFO) << "获取device_info数据失败";
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
	}

	/// <summary>
	/// 获取device_param_info信息
	/// </summary>
	/// <returns></returns>
	bool DeviceDAO::GetAllDeviceParamInfoFrames()
	{
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		MYSQL_RES* result = nullptr;
		MYSQL_ROW sql_row;
		int res;
		string sql;
		sql.append("select * from device_param_info;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_FaultDeviceParamInfoFrames.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					FaultDeviceParamInfo* oneFrame = new FaultDeviceParamInfo();
					int id = atoi(sql_row[0]);
					oneFrame->m_id = id;
					oneFrame->m_deviceID = (sql_row[1] == nullptr) ? -1 : atoi(sql_row[1]);
					oneFrame->m_parameterID = (sql_row[2] == nullptr) ? -1 : atoi(sql_row[2]);
					oneFrame->m_createTime = sql_row[3];
					oneFrame->m_lastUpdateTime = sql_row[4];

					m_app->m_FaultDeviceParamInfoFrames.insert(pair<int, FaultDeviceParamInfo*>(id, oneFrame));
				}
			}
			else
				LOG(INFO) << "获取device_param_info数据失败";
		}
		else
			LOG(INFO) << "获取device_param_info数据失败";
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
	}


	/// <summary>
	/// 设备id和参数id对应项
	/// </summary>
	/// <returns></returns>
	bool DeviceDAO::GetDeviceIDParamIDFrames()
	{
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		MYSQL_RES* result = nullptr;
		MYSQL_ROW sql_row;
		int res;
		string sql;
		sql.append("SELECT device_info.id,parameter_info.id FROM device_param_info INNER JOIN parameter_info ON device_param_info.parameter_id = parameter_info.id INNER JOIN device_info ON device_info.id = device_param_info.device_id;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_DeviceIDParamID.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					int deviceId = atoi(sql_row[0]);
					int ParamId = atoi(sql_row[1]);
					m_app->m_DeviceIDParamID[deviceId].push_back(ParamId);
					//m_app->m_DeviceIDParamID.insert(pair<int, vector<int>>(deviceId, ParamId));
				}
			}
			else
				LOG(INFO) << "联表获取数据失败";
		}
		else
			LOG(INFO) << "联表获取数据失败";
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
	}


	/// <summary>
	/// 获取下一个id值
	/// </summary>
	/// <param name="tableName"></param>
	/// <returns></returns>
	int DeviceDAO::GetNowId(string tableName)
	{
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		int res;
		int nextId = 0;
		MYSQL_RES* result = nullptr;
		string sql;
		sql.append("select max(id) from ");
		sql.append(tableName);
		sql.append(";");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			MYSQL_RES* result = nullptr;
			MYSQL_ROW sql_row;
			result = mysql_store_result(&my_connection);
			if (result)
			{
				sql_row = mysql_fetch_row(result);
				nextId = atoi(sql_row[0]);
				return nextId + 1;
			}
			return -1;
		}
		else
			return -1;
	}
	
	   
	bool DeviceDAO::exec_sql(string  sql)
	{
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		int res;
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());
		if (!res)
			return true;
		else
		{
			//LOG(ERROR) << "SQL执行失败，错误："<< mysql_errno(&my_connection) << "，错误信息：" << mysql_error(&my_connection);
			return false;
		}
	} 
	int DeviceDAO::queryMysql(std::string sql_str, unordered_map<int, vector<string>>& contents) {

		mysql_query(&my_connection, "set names utf8");

		int ret = mysql_query(&my_connection, sql_str.c_str());
		if (ret)
		{
			LOG(INFO) << "无法读取数据：" << mysql_error(&my_connection);
		}
		MYSQL_RES* res;
		MYSQL_ROW row;
		unsigned int num_feilds;
		unsigned int num_rows;

		res = mysql_store_result(&my_connection);
		if (res) {
			num_feilds = mysql_num_fields(res);
		}
		else
		{
			if (mysql_field_count(&my_connection) == 0) {
				num_rows = mysql_affected_rows(&my_connection);
			}
			else
			{
				LOG(INFO) << "error:" << mysql_error(&my_connection);
				return 0;
			}
			return 0;
		}

		int counts = 0;
		while ((row = mysql_fetch_row(res)))
		{
			unsigned long* lengths;
			lengths = mysql_fetch_lengths(res);
			//不需要读创建时间和修改时间
			for (int i = 0; i < num_feilds - 2; i++)
			{
				LOG(INFO) << (int)lengths[i] << (row[i] ? row[i] : "NULL ");
				contents[(int)(row[0])].push_back(string(row[i]));
			}
			counts++;
		}

		return counts;
	}
}
