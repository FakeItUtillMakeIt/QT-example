#pragma execution_character_set("utf-8")
#include "FaultParamDAO.h"
#include <boost\lexical_cast.hpp>
 
namespace DataBase
{
	FaultParamDAO::FaultParamDAO(OutputPath* path)
		: m_path(path)
		, is_connected(false)
	{
		m_app = AppCache::instance();
	}

	FaultParamDAO::~FaultParamDAO()
	{
		if (is_connected)
		{
			mysql_close(&my_connection);//断开连接  
			is_connected = false;
		}
	}

	bool FaultParamDAO::connect()
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
	bool FaultParamDAO::connected()
	{
		return is_connected;
	}



	/// <summary>
	/// 获取所有的fault_param_info信息
	/// </summary>
	/// <returns></returns>
	bool FaultParamDAO::GetAllFaultParamInfoFrames()
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
		sql.append("select * from fault_param_info;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_FaultParamInfoFrames.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					FaultFaultParamInfo* oneFrame = new FaultFaultParamInfo();
					int id = atoi(sql_row[0]);
					oneFrame->m_id = id;
					oneFrame->m_rocketID = (sql_row[1] == nullptr) ? -1 : atoi(sql_row[1]);
					oneFrame->m_name = sql_row[2];
					oneFrame->m_faultType = (sql_row[3] == nullptr) ? -1 : atoi(sql_row[3]);
					oneFrame->m_FaultCommandID = (sql_row[4] == nullptr) ? -1 : atoi(sql_row[4]);
					oneFrame->m_deviceParamInfoID = (sql_row[5] == nullptr) ? -1 : atoi(sql_row[5]);
					oneFrame->m_createTime = sql_row[6];
					oneFrame->m_lastUpdateTime = sql_row[7];

					if (oneFrame->m_rocketID == m_app->m_rockedType)
					{
						m_app->m_FaultParamInfoFrames.insert(pair<int, FaultFaultParamInfo*>(id, oneFrame));
					}
				}
			}
			else
				LOG(INFO) << "获取fault_param_info数据失败";
		}
		else
			LOG(INFO) << "获取fault_param_info数据失败";
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
	}


	/// <summary>
	///插入数据到fault_param_info数据表中
	/// </summary>
	/// <param name="fameInfo"></param>
	/// <returns></returns>
	bool FaultParamDAO::InsertFaultParamInfo(FaultFaultParamInfo* fameInfo)
	{
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		stringstream ss;
		ss << "insert into fault_param_info(name,rocketID,fault_type,command_id,device_param_info_id) values ('";
		ss << boost::lexical_cast<string>(fameInfo->m_name) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_rocketID) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_faultType) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_FaultCommandID) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_deviceParamInfoID) + "')";

		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		string sql = ss.str();
		int res = mysql_query(&my_connection, sql.c_str());
		if (!res)
		{
			int res1;
			mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
			res1 = mysql_query(&my_connection, "select @@IDENTITY");
			if (!res1)
			{
				MYSQL_RES* result = nullptr;
				MYSQL_ROW sql_row;
				result = mysql_store_result(&my_connection);
				if (result)
				{
					sql_row = mysql_fetch_row(result);
					int id = atoi(sql_row[0]);
					fameInfo->m_id = id;

					m_app->m_FaultParamInfoFrames.insert(pair<int, FaultFaultParamInfo*>(id, fameInfo));
					return true;
				}
			}
		}
		else
		{
			LOG(INFO) << "FaultParamDAO[fault_param_info]:" << "插入数据库失败!" << sql;
			return false;
		}
		return false;
	}

	/// <summary>
	/// 根据id 删除fault_param_info数据表中的对应信息
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	bool FaultParamDAO::DelFaultParamInfoFrame(int id)
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
		sql.append("delete from fault_param_info where id = '");
		sql.append(boost::lexical_cast<string>(id) + "'; ");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			m_app->m_FaultParamInfoFrames.erase(id);//更新记录map
			return true;
		}
		else
			LOG(INFO) << "FaultParamDAO[fault_param_info]:" << "删除数据失败!" << sql;
		if (result)
			mysql_free_result(result);//释放结果资源  
		return false;
	}

	/// <summary>
	/// 根据名字删除
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	bool FaultParamDAO::DelFaultParamInfoFrame(string name)
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
		sql.append("delete from fault_param_info where name = '");
		sql.append(boost::lexical_cast<string>(name) + "'; ");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			//更新记录map
			map<int, FaultFaultParamInfo*> tempFrames ;
			for (map<int, FaultFaultParamInfo*>::iterator iter = m_app->m_FaultParamInfoFrames.begin(); iter != m_app->m_FaultParamInfoFrames.end(); iter++)
			{
				if (iter->second->m_name != name)
				{
					//m_app->m_FaultParamInfoFrames.erase(iter->first);//这样删多个有问题
					tempFrames.insert(pair<int, FaultFaultParamInfo*>(iter->first, iter->second));
				}
			}

			m_app->m_FaultParamInfoFrames = tempFrames;
			return true;
		}
		else
			LOG(INFO) << "FaultParamDAO[fault_param_info]:" << "删除数据失败!" << sql;
		if (result)
			mysql_free_result(result);//释放结果资源  
		return false;
	}


	/// <summary>
	/// 获取所有的fault_command_info信息
	/// </summary>
	/// <returns></returns>
	bool FaultParamDAO::GetAllFaultCommandInfoFrames()
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
		sql.append("select * from fault_command_info;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_FaultCommandInfoFrames.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					FaultFaultCommandInfo* oneFrame = new FaultFaultCommandInfo();
					int id = atoi(sql_row[0]);
					oneFrame->m_id = id;
					oneFrame->m_rocketID = (sql_row[1] == nullptr) ? -1 : atoi(sql_row[1]);
					oneFrame->m_name = sql_row[2];
					oneFrame->m_faultType = (sql_row[3] == nullptr) ? -1 : atoi(sql_row[3]);
					oneFrame->m_FaultCommandID = (sql_row[4] == nullptr) ? -1 : atoi(sql_row[4]);
					oneFrame->m_responseCommandID = (sql_row[5] == nullptr) ? -1 : atoi(sql_row[5]);
					oneFrame->m_createTime = sql_row[6];
					oneFrame->m_lastUpdateTime = sql_row[6];

					if (oneFrame->m_rocketID == m_app->m_rockedType)
					{
						m_app->m_FaultCommandInfoFrames.insert(pair<int, FaultFaultCommandInfo*>(id, oneFrame));
					}
					
				}
			}
			else
				LOG(INFO) << "获取fault_command_info数据失败";
		}
		else
			LOG(INFO) << "获取fault_command_info数据失败";
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
	}


	/// <summary>
	///插入数据到fault_command_info数据表中
	/// </summary>
	/// <param name="fameInfo"></param>
	/// <returns></returns>
	bool FaultParamDAO::InsertFaultCommandInfoFrame(FaultFaultCommandInfo* fameInfo)
	{
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		stringstream ss;
		ss << "insert into fault_command_info(name,rocketID,fault_type,fault_command_id,response_command_id) values ('";
		ss << boost::lexical_cast<string>(fameInfo->m_name) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_rocketID) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_faultType) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_FaultCommandID) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_responseCommandID) + "')";

		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		string sql = ss.str();
		int res = mysql_query(&my_connection, sql.c_str());
		if (!res)
		{
			int res1;
			mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
			res1 = mysql_query(&my_connection, "select @@IDENTITY");
			if (!res1)
			{
				MYSQL_RES* result = nullptr;
				MYSQL_ROW sql_row;
				result = mysql_store_result(&my_connection);
				if (result)
				{
					sql_row = mysql_fetch_row(result);
					int id = atoi(sql_row[0]);
					fameInfo->m_id = id;

					m_app->m_FaultCommandInfoFrames.insert(pair<int, FaultFaultCommandInfo*>(id, fameInfo));
					return true;
				}
			}
		}
		else
		{
			LOG(INFO) << "FaultParamDAO[fault_command_info]:" << "插入数据库失败!" << sql;
			return false;
		}
		return false;
	}

	/// <summary>
	/// 根据id 删除fault_command_info数据表中的对应信息
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	bool FaultParamDAO::DelFaultCommandInfoFrame(int id)
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
		sql.append("delete from fault_command_info where id = '");
		sql.append(boost::lexical_cast<string>(id) + "'; ");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			m_app->m_FaultCommandInfoFrames.erase(id);//更新记录map
			return true;
		}
		else
			LOG(INFO) << "FaultParamDAO[fault_command_info]:" << "删除数据失败!" << sql;
		if (result)
			mysql_free_result(result);//释放结果资源  
		return false;
	}


	/// <summary>
	/// 根据名字删除 fault_command_info
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	bool FaultParamDAO::DelFaultCommandInfoFrame(string name)
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
		sql.append("delete from fault_command_info where name = '");
		sql.append(boost::lexical_cast<string>(name) + "'; ");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			//更新记录map
			map<int, FaultFaultCommandInfo*> tempFrames;
			for (map<int, FaultFaultCommandInfo*>::iterator iter = m_app->m_FaultCommandInfoFrames.begin(); iter != m_app->m_FaultCommandInfoFrames.end(); iter++)
			{
				if (iter->second->m_name != name)
				{
					tempFrames.insert(pair<int, FaultFaultCommandInfo*>(iter->first, iter->second));
				}
			}

			m_app->m_FaultCommandInfoFrames = tempFrames;
			return true;
		}
		else
			LOG(INFO) << "FaultParamDAO[fault_command_info]:" << "删除数据失败!" << sql;
		if (result)
			mysql_free_result(result);//释放结果资源  
		return false;
	}

	/// <summary>
	/// 获取下一个id值
	/// </summary>
	/// <param name="tableName"></param>
	/// <returns></returns>
	int FaultParamDAO::GetNowId(string tableName)
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

	bool FaultParamDAO::exec_sql(string  sql)
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