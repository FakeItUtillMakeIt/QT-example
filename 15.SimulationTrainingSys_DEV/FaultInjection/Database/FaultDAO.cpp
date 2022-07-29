#pragma execution_character_set("utf-8")
#include "FaultDAO.h"
#include <boost\lexical_cast.hpp>
 
namespace DataBase
{
	FaultDAO::FaultDAO(OutputPath* path)
		: m_path(path)
		, is_connected(false)
	{
		m_app = AppCache::instance();
	}

	FaultDAO::~FaultDAO()
	{
		if (is_connected)
		{
			mysql_close(&my_connection);//断开连接  
			is_connected = false;
		}
	}

	bool FaultDAO::connect()
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
	bool FaultDAO::connected()
	{
		return is_connected;
	}
 
	bool FaultDAO::getFault()
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
		sql.append("select * from user_info;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_allFault.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					Fault* oneFault = new Fault();
					int id = atoi(sql_row[0]);
					oneFault->m_id = id;
					oneFault->m_name = sql_row[1];
					oneFault->m_type = sql_row[2];
					m_app->m_allFault.push_back(oneFault);
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
	/// 获取rocket_info信息
	/// </summary>
	/// <returns></returns>
	bool FaultDAO::GetAllRocketInfoFrame()
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
		sql.append("select * from rocket_info;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_RocketInfoframes.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					FaultRocketInfo* oneFrame = new FaultRocketInfo();
					int id = atoi(sql_row[0]);
					oneFrame->m_id = id;
					oneFrame->m_name = sql_row[1];
					oneFrame->m_code = (sql_row[2] == nullptr) ? -1 : atoi(sql_row[2]);
					oneFrame->m_createTime = sql_row[3];
					oneFrame->m_lastUpdateTime = sql_row[4];

					m_app->m_RocketInfoframes.insert(pair<int, FaultRocketInfo*>(id, oneFrame));
				}
			}
			else
				LOG(INFO) << "获取Command_Info数据失败";
		}
		else
			LOG(INFO) << "获取Command_Info数据失败";
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
	}



	/// <summary>
	/// 获取Command_Info信息
	/// </summary>
	/// <returns></returns>
	bool FaultDAO::GetAllCommandInfoFrame()
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
		sql.append("select * from command_info;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_CommandInfoframes.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					FaultCommandInfo* oneFrame = new FaultCommandInfo();
					int id = atoi(sql_row[0]);
					oneFrame->m_id = id;
					oneFrame->m_rocketID = (sql_row[1] == nullptr)? -1 : atoi(sql_row[1]);
					oneFrame->m_backID = (sql_row[2] == nullptr)? -1 : atoi(sql_row[2]);
					oneFrame->m_name = sql_row[3];
					oneFrame->m_code = (sql_row[4] == nullptr)? -1 : atoi(sql_row[4]);
					oneFrame->m_type = (sql_row[5] == nullptr) ? -1 : atoi(sql_row[5]);
					oneFrame->m_prefix = (sql_row[6] == nullptr) ? -1 : atoi(sql_row[6]);
					oneFrame->m_createTime = sql_row[7];
					oneFrame->m_lastUpdateTime = sql_row[8];
				 
					m_app->m_CommandInfoframes.insert(pair<int, FaultCommandInfo*>(id, oneFrame));
				}
			}
			else
				LOG(INFO) << "获取Command_Info数据失败";
		}
		else
			LOG(INFO) << "获取Command_Info数据失败";
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
	}


	/// <summary>
	///插入数据到 command_info数据表中
	/// </summary>
	/// <param name="fameInfo"></param>
	/// <returns></returns>
	bool FaultDAO::InsertCommandInfo(FaultCommandInfo* fameInfo)
	{
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		stringstream ss;
		ss<<"insert into command_info(rocket_id,back_id,name,code,type,prefix) values ('";
		ss << boost::lexical_cast<string>(fameInfo->m_rocketID) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_backID) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_name) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_code) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_type) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_prefix) + "')";

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
					m_app->m_CommandInfoframes.insert(pair<int, FaultCommandInfo*>(id, fameInfo));//更新记录map
					return true;
				} 
			} 
		}
		else
		{
			LOG(INFO) << "FaultDAO[frame_info]:"<< "插入数据库失败!" << sql;
			return false;
		}
		return false;
	}

	/// <summary>
	/// 根据id 删除command_info数据表中的对应信息
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	bool FaultDAO::DelCommandInfoParam(int id)
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
		sql.append("delete from command_info where id = '");
		sql.append(boost::lexical_cast<string>(id) + "'; ");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			m_app->m_CommandInfoframes.erase(id);//更新记录map
			return true;
		}
		else
			LOG(INFO) << "FaultDAO[command_info]:" << "删除数据失败!" << sql;
		if (result)
			mysql_free_result(result);//释放结果资源  
		return false;
	}

	/// <summary>
	/// 获取所有的command_param_info信息
	/// </summary>
	/// <returns></returns>
	bool FaultDAO::GetAllCommandParamInfoFrame()
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
		sql.append("select * from command_param_info;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_CommandParamInfoframes.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					FaultCommandParamInfo* oneFrame = new FaultCommandParamInfo();
					int id = atoi(sql_row[0]);
					oneFrame->m_id = id;
					oneFrame->m_commandID = (sql_row[1] == nullptr) ? -1 : atoi(sql_row[1]);
					oneFrame->m_name = sql_row[2];
					oneFrame->m_code = (sql_row[3] == nullptr) ? -1 : atoi(sql_row[3]);
					oneFrame->m_index = (sql_row[4] == nullptr) ? -1 : atoi(sql_row[4]);
					oneFrame->m_length = (sql_row[5] == nullptr) ? -1 : atoi(sql_row[5]);
					oneFrame->m_resultType = (sql_row[6] == nullptr) ? -1 : atoi(sql_row[6]);
					oneFrame->m_defaultValue = (sql_row[7] == nullptr) ? -1 : atoi(sql_row[7]);
					oneFrame->m_createTime = sql_row[8];
					oneFrame->m_lastUpdateTime = sql_row[9];

					m_app->m_CommandParamInfoframes.insert(pair<int, FaultCommandParamInfo*>(id, oneFrame));
				}
			}
			else
				LOG(INFO) << "获取command_param_info数据失败";
		}
		else
			LOG(INFO) << "获取command_param_info数据失败";
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
	}


	/// <summary>
	/// 向command_param_info表中插入信息
	/// </summary>
	/// <param name="fameInfo"></param>
	/// <returns></returns>
	bool FaultDAO::InsertCommandParamInfo(FaultCommandParamInfo* fameInfo)
	{
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		stringstream ss;
		ss << "insert into command_info(command_id,name,code,index,length,resultType,defaultValue) values ('";
		ss << boost::lexical_cast<string>(fameInfo->m_commandID) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_name) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_code) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_index) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_length) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_resultType) + "','";
		ss << boost::lexical_cast<string>(fameInfo->m_defaultValue) + "')";

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
					m_app->m_CommandParamInfoframes.insert(pair<int, FaultCommandParamInfo*>(id, fameInfo));//更新记录map
					return true;
				}
			}
		}
		else
		{
			LOG(INFO) << "FaultDAO[frame_param_info]:" << "插入数据库失败!" << sql;
			return false;
		}
		return false;
	}

	/// <summary>
	/// 根据id 删除command_param_info数据表中的对应信息
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	bool FaultDAO::DelCommandParamInfoParam(int id)
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
		sql.append("delete from command_param_info where id = '");
		sql.append(boost::lexical_cast<string>(id) + "'; ");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			m_app->m_CommandParamInfoframes.erase(id);//更新记录map
			return true;
		}
		else
			LOG(INFO) << "FaultDAO[command_param_info]:" << "删除数据失败!" << sql;
		if (result)
			mysql_free_result(result);//释放结果资源  
		return false;
	}







	///// <summary>
	///// 获取所有帧
	///// </summary>
	///// <returns></returns>
	//bool FaultDAO::getAllFrame()
	//{
	//	if (!connected())
	//	{
	//		LOG(INFO) << "创建数据库连接";
	//		if (!connect())
	//			return false;
	//	}
	//	MYSQL_RES* result = nullptr;
	//	MYSQL_ROW sql_row;
	//	int res;
	//	string sql;
	//	sql.append("select * from frame_info;");
	//	mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
	//	res = mysql_query(&my_connection, sql.c_str());//查询
	//	if (!res)
	//	{
	//		result = mysql_store_result(&my_connection);
	//		if (result)
	//		{
	//			m_app->m_vDataFlow->m_frames.clear();
	//			while (sql_row = mysql_fetch_row(result))
	//			{
	//				FrameInfo* oneFrame = new FrameInfo(); 
	//				int id = atoi(sql_row[0]);
	//				oneFrame->m_id = id;
	//				oneFrame->m_iName = sql_row[1];
	//				oneFrame->m_iLengthMinor = (sql_row[2] == nullptr)? -1 : atoi(sql_row[2]);
	//				oneFrame->m_iLengthSub = (sql_row[3] == nullptr) ? -1 : atoi(sql_row[3]);
	//				oneFrame->m_iLengthTime = (sql_row[4] == nullptr)? -1 : atoi(sql_row[4]);
	//				 
	//				if (sql_row[5] != nullptr)
	//				{
	//					oneFrame->m_iSyncMinor = std::stoul(sql_row[5], nullptr, 16);
	//					oneFrame->m_iSyncMinorStr = sql_row[5];
	//				}
	//				else
	//				{
	//					oneFrame->m_iSyncMinor = -1;
	//					oneFrame->m_iSyncMinorStr = "-1";
	//				}
	//				  
	//				oneFrame->m_iSyncMinorLen = (sql_row[6] == nullptr)? -1 : atoi(sql_row[6]);
	//				oneFrame->m_iSyncMinorStart = (sql_row[7] == nullptr)? -1 : atoi(sql_row[7]);

	//				if (sql_row[8] != nullptr)
	//				{
	//					oneFrame->m_iSyncSub = std::stoul(sql_row[8], nullptr, 16);
	//					oneFrame->m_iSyncSubStr = sql_row[8];
	//				}
	//				else
	//				{
	//					oneFrame->m_iSyncSub = -1;
	//					oneFrame->m_iSyncSubStr = "-1";
	//				}

	//				oneFrame->m_iSyncSubLen = (sql_row[9] == nullptr)? -1 : atoi(sql_row[9]);
	//				oneFrame->m_iSyncSubStart = (sql_row[10] == nullptr)? -1 : atoi(sql_row[10]);
	//				oneFrame->m_iFrameSerialLen = (sql_row[11] == nullptr)? -1 : atoi(sql_row[11]);
	//				oneFrame->m_iFrameSerialStart = (sql_row[12] == nullptr)? -1 : atoi(sql_row[12]);
	//				oneFrame->m_iTimeCodeStart = (sql_row[13] == nullptr)? -1 : atoi(sql_row[13]);
	//				oneFrame->m_iSyncMinorType = (sql_row[14] == nullptr) ? -1 : atoi(sql_row[14]);
	//				oneFrame->m_iMode = (sql_row[15] == nullptr) ? -1 : atoi(sql_row[15]);
	//				oneFrame->m_iStartPosType = (sql_row[16] == nullptr) ? -1 : atoi(sql_row[16]);
	//				oneFrame->m_iColumnCountType = (sql_row[17] == nullptr) ? -1 : atoi(sql_row[17]);
	//				m_app->m_vDataFlow->m_frames.insert(pair<int, FrameInfo*>(id, oneFrame));

	//			}

	//		}
	//		else
	//			LOG(INFO) << "获取数据失败";
	//	}
	//	else
	//		LOG(INFO) << "获取数据失败";
	//	if (result)
	//		mysql_free_result(result);//释放结果资源  
	//	return true;
	//}
	//
	///// <summary>
	///// 查重
	///// </summary>
	///// <param name="name"></param>
	///// <returns></returns>
	//bool FaultDAO::isSameName(string name)
	//{
	//	if (!connected())
	//	{
	//		LOG(INFO) << "创建数据库连接";
	//		if (!connect())
	//			return false;
	//	}
	//	MYSQL_RES* result = nullptr;
	//	MYSQL_ROW sql_row;
	//	int res;
	//	string sql;
	//	sql.append("select count(*) from frame_info where name = '");
	//	sql.append(name + "'; ");
	//	mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
	//	res = mysql_query(&my_connection, sql.c_str());//查询
	//	if (!res)
	//	{
	//		result = mysql_store_result(&my_connection);
	//		if (result)
	//		{
	//			sql_row = mysql_fetch_row(result);
	//			if (atoi(sql_row[0]) > 0) return true;  
	//		}
	//		else
	//			LOG(INFO) << "获取数据失败";
	//	}
	//	else
	//		LOG(INFO) << "获取数据失败";
	//	if (result)
	//		mysql_free_result(result);//释放结果资源  
	//	return false;
	//}
	
	///// <summary>
	///// 添加
	///// </summary>
	///// <param name="fameInfo"></param>
	///// <returns></returns>
	//bool FaultDAO::add(FrameInfo* fameInfo)
	//{
	//	if (!connected())
	//	{
	//		LOG(INFO) << "创建数据库连接";
	//		if (!connect())
	//			return false;
	//	}
	//	stringstream ss;
	//	 
	//	ss<<"insert into frame_info(name,minor_frame_len,sub_frame_len,time_word_len,minor_frame_sync_code,";
	//	ss << "minor_frame_sync_len,minor_frame_sync_start,sub_frame_sync_code,sub_frame_sync_len,sub_frame_sync_start,";
	//	ss << "frame_serial_len,frame_serial_start,time_code_start,sub_frame_sync_type) values ('";
	//	ss << boost::lexical_cast<string>(fameInfo->m_iName) + "','";
	//	ss << boost::lexical_cast<string>(fameInfo->m_iLengthMinor) + "','";
	//	ss << boost::lexical_cast<string>(fameInfo->m_iLengthSub) + "','";
	//	ss << boost::lexical_cast<string>(fameInfo->m_iLengthTime) + "','";
	//	ss << fameInfo->m_iSyncMinorStr + "','";
	//	ss << boost::lexical_cast<string>(fameInfo->m_iSyncMinorLen) + "','";
	//	ss << boost::lexical_cast<string>(fameInfo->m_iSyncMinorStart) + "','";
	//	ss << fameInfo->m_iSyncSubStr + "','";
	//	ss << boost::lexical_cast<string>(fameInfo->m_iSyncSubLen) + "','";
	//	ss << boost::lexical_cast<string>(fameInfo->m_iSyncSubStart) + "','";
	//	ss << boost::lexical_cast<string>(fameInfo->m_iFrameSerialLen) + "','";
	//	ss << boost::lexical_cast<string>(fameInfo->m_iFrameSerialStart) + "','";
	//	ss << boost::lexical_cast<string>(fameInfo->m_iTimeCodeStart) + "','";
	//	ss << boost::lexical_cast<string>(fameInfo->m_iSyncMinorType) + "')";

	//	mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
	//	string sql = ss.str();
	//	int res = mysql_query(&my_connection, sql.c_str());
	//	if (!res)
	//	{ 
	//		int res1;
	//		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
	//		res1 = mysql_query(&my_connection, "select @@IDENTITY");
	//		if (!res1)
	//		{
	//			MYSQL_RES* result = nullptr;
	//			MYSQL_ROW sql_row;
	//			result = mysql_store_result(&my_connection);
	//			if (result)
	//			{
	//				sql_row = mysql_fetch_row(result);
	//				int id = atoi(sql_row[0]);
	//				fameInfo->m_id = id;
	//				m_app->m_vDataFlow->m_frames.insert(pair<int, FrameInfo*>(id, fameInfo));
	//				return true;
	//			} 
	//		} 
	//	}
	//	else
	//	{
	//		LOG(INFO) << "FaultDAO[frame_info]:"<< "插入数据库失败!" << sql;
	//		return false;
	//	}
	//	return false;
	//}

	bool FaultDAO::test()
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
		sql.append("select * from user_info;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				while (sql_row = mysql_fetch_row(result))
				{
					int id = atoi(sql_row[0]);
					string name = sql_row[1];
					string pwd = sql_row[2];
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
	   
	bool FaultDAO::exec_sql(string  sql)
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


	int FaultDAO::GetNowId(string tableName)
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
		string sql ;
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
}
