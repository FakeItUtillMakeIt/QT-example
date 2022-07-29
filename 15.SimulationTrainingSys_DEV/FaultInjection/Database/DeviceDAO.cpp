#pragma execution_character_set("utf-8")
#include "DeviceDAO.h"
#include <boost\lexical_cast.hpp>
 
namespace DataBase
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
		sql.append("select * from parameter_info;");
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
					oneFrame->m_name = sql_row[1];
					oneFrame->m_type = (sql_row[2] == nullptr) ? -1 : atoi(sql_row[2]);
					oneFrame->m_unit = (sql_row[3] == nullptr) ? -1 : atoi(sql_row[3]);
					oneFrame->m_createTime = sql_row[4];
					oneFrame->m_lastUpdateTime = sql_row[5];
				 
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
					oneFrame->m_deviceID = (sql_row[1] == nullptr) ? -1 : atoi(sql_row[2]);
					oneFrame->m_parameterID = (sql_row[2] == nullptr) ? -1 : atoi(sql_row[3]);
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
		int rownumb;
		MYSQL_RES* result = nullptr;
		string sql;
		sql.append("select * from ");
		sql.append(tableName);
		sql.append(";");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			rownumb = result->row_count;
		}
		else
			rownumb = -1;
		return rownumb + 1;
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
}
