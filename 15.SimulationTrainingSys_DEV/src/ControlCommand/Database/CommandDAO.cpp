#pragma execution_character_set("utf-8")
#include "CommandDAO.h"
#include <boost\lexical_cast.hpp>
#include <stdlib.h>

namespace DataBase
{
	CommandDAO::CommandDAO(OutputPath* path)
		: m_path(path)
		, is_connected(false)
	{
		m_app = AppCache::instance();
	}

	CommandDAO::~CommandDAO()
	{
		if (is_connected)
		{
			mysql_close(&my_connection);//断开连接  
			is_connected = false;
		}
	}

	bool CommandDAO::connect()
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
	bool CommandDAO::connected()
	{
		return is_connected;
	}

	bool CommandDAO::getCommand()
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
		sql.append("select b.name as '火箭型号', a.* from command_info a left join rocket_info b on a.rocket_id = b.id ;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_allCommad.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					Command* oneCommnd = new Command();
					oneCommnd->m_sRocketName = Utils::UTF8ToGBK(sql_row[0]);
					oneCommnd->m_id = atoi(sql_row[1]);
					oneCommnd->m_iRocketId = atoi(sql_row[2]);
					oneCommnd->m_iBackId = atoi(sql_row[3]);
					oneCommnd->m_sName = Utils::UTF8ToGBK(sql_row[4]);
					oneCommnd->m_iCode = atoi(sql_row[5]);
					oneCommnd->m_iType = atoi(sql_row[6]);
					m_app->m_allCommad.insert(pair<int, Command*>(oneCommnd->m_id, oneCommnd));
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
	/// 查询指令参数
	/// </summary>
	/// <returns></returns>
	bool CommandDAO::getCommandParam()
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
				while (sql_row = mysql_fetch_row(result))
				{
					CommandParam* oneCommnd = new CommandParam(); 
					oneCommnd->m_id = atoi(sql_row[0]); 
					oneCommnd->m_iCommand_id = atoi(sql_row[1]);
					oneCommnd->m_sName = Utils::UTF8ToGBK(sql_row[2]); 
					oneCommnd->m_iCode = atoi(sql_row[3]); 
					oneCommnd->m_iIndex = atoi(sql_row[4]); 
					oneCommnd->m_iLength = atoi(sql_row[5]);
					oneCommnd->m_sResultType = atoi(sql_row[6]); 
					oneCommnd->m_dDefaultValue = atof(sql_row[7]); 
					oneCommnd->m_val.val_f = 0;
					if (m_app->m_allCommad.find(oneCommnd->m_iCommand_id) != m_app->m_allCommad.end())
					{
						Command* command = m_app->m_allCommad[oneCommnd->m_iCommand_id];
						command->m_vCommandParam.push_back(oneCommnd);
					} 
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
	/// 指令故障
	/// </summary>
	/// <returns></returns>
	bool CommandDAO::getFaultCommand()
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
				m_app->m_allFaults.clear();
				while (sql_row = mysql_fetch_row(result))
				{  
					int fault_command_id = atoi(sql_row[4]);
					int response_command_id = atoi(sql_row[5]); 
					if (m_app->m_allCommad.find(fault_command_id) == m_app->m_allCommad.end() ||
						m_app->m_allCommad.find(response_command_id) == m_app->m_allCommad.end())
					{
						continue;
					}  
					Command* command = m_app->m_allCommad[response_command_id];
					m_app->m_allFaults[fault_command_id].push_back(command); 
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



	///// <summary>
	///// 获取所有帧
	///// </summary>
	///// <returns></returns>
	//bool CommandDAO::getAllFrame()
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
	//bool CommandDAO::isSameName(string name)
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
	//bool CommandDAO::add(FrameInfo* fameInfo)
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
	//		LOG(INFO) << "CommandDAO[frame_info]:"<< "插入数据库失败!" << sql;
	//		return false;
	//	}
	//	return false;
	//}

	bool CommandDAO::test()
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

	bool CommandDAO::exec_sql(string  sql)
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


