﻿#pragma execution_character_set("utf-8")
#include "RocketDataDAO.h"
#include <boost\lexical_cast.hpp>
#include <stdlib.h>

namespace DataBase
{
	RocketDataDAO::RocketDataDAO(OutputPath* path)
		: m_path(path)
		, is_connected(false)
	{
		m_app = AppCache::instance();
	}

	RocketDataDAO::~RocketDataDAO()
	{
		if (is_connected)
		{
			mysql_close(&my_connection);//断开连接  
			is_connected = false;
		}
	}

	bool RocketDataDAO::connect()
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
	bool RocketDataDAO::connected()
	{
		return is_connected;
	}

	bool RocketDataDAO::getRocketData()
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
		sql.append("select b.name as '火箭型号', a.* from rocket_data_info a left join rocket_info b on a.rocket_id = b.id ;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_RocketDataFrame.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					RocketDataFrame* oneFrame = new RocketDataFrame();
					oneFrame->m_rocketName = Utils::UTF8ToGBK(sql_row[0]);
					oneFrame->m_id = atoi(sql_row[1]);
					oneFrame->m_rocketId = atoi(sql_row[2]); 
					oneFrame->m_frameName = Utils::UTF8ToGBK(sql_row[3]);
					oneFrame->m_code = atoi(sql_row[4]); 
					m_app->m_RocketDataFrame.insert(pair<int, RocketDataFrame*>(oneFrame->m_id, oneFrame));
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
	bool RocketDataDAO::getRocketParam()
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
		sql.append("select * from rockect_param_info;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{ 
				while (sql_row = mysql_fetch_row(result))
				{
					RocketDataParam oneParam; 
					oneParam.m_id = atoi(sql_row[0]);
					oneParam.m_rocketDataId = atoi(sql_row[1]);
					int device_parameter_id = atoi(sql_row[2]);
					oneParam.m_index = atoi(sql_row[3]);
					oneParam.m_length = atoi(sql_row[4]);  
					if (m_app->m_allDeviceParam.find(device_parameter_id) != m_app->m_allDeviceParam.end())
					{
						DeviceParam* param = m_app->m_allDeviceParam[device_parameter_id];
						oneParam.m_deviceParam = param;
					}
					if (m_app->m_RocketDataFrame.find(oneParam.m_rocketDataId) != m_app->m_RocketDataFrame.end())
					{
						RocketDataFrame* frame = m_app->m_RocketDataFrame[oneParam.m_rocketDataId];
						frame->params().push_back(oneParam);
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
	/// //获取岗位信息
	/// </summary>
	/// <returns></returns>
	bool RocketDataDAO::GetTaskInfo()
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
		sql.append("select * from rocket_task_info;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			m_app->m_TaskManageInfoFrames.clear();
			result = mysql_store_result(&my_connection);
			if (result)
			{
				while (sql_row = mysql_fetch_row(result))
				{
					TaskManageInfo* oneTask = new TaskManageInfo();
					oneTask->m_id = atoi(sql_row[0]);
					oneTask->m_rocketId = atoi(sql_row[1]);
					oneTask->m_taskName = sql_row[2];
					oneTask->m_roketSoftName = sql_row[3];
					oneTask->m_createTime = sql_row[4];
					oneTask->m_lastUpdateTime = sql_row[5];

					if (m_app->m_CurrentRocketType->m_id == oneTask->m_rocketId)
					{
						m_app->m_TaskManageInfoFrames.insert(pair<int, TaskManageInfo*>(oneTask->m_id, oneTask));
					}
				}
			}
			else
			{
				LOG(INFO) << "获取数据失败";
				return false;
			}	
		}
		else
		{
			LOG(INFO) << "获取数据失败";
			return false;
		}
			
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
	}

	/// <summary>
	/// 添加岗位信息
	/// </summary>
	/// <returns></returns>
	bool RocketDataDAO::InsertTaskInfo(TaskManageInfo* taskInfo)
	{
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		stringstream ss;
		ss << "insert into rocket_task_info(rocket_id,task_name,roket_softname) values ('";
		ss << boost::lexical_cast<string>(taskInfo->m_rocketId) + "','";
		ss << boost::lexical_cast<string>(taskInfo->m_taskName) + "','";
		ss << boost::lexical_cast<string>(taskInfo->m_roketSoftName) + "')";

		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		string sql = ss.str();
		int res = mysql_query(&my_connection, sql.c_str());
		if (!res)
		{
			GetTaskInfo();//查询一次
		}
		else
		{
			LOG(INFO) << "rocket_task_info:" << "插入数据库失败!" << sql;
			return false;
		}
		return true;
	}

	/// <summary>
	/// 删除岗位信息
	/// </summary>
	/// <param name="taskInfo"></param>
	/// <returns></returns>
	bool RocketDataDAO::DeleteTaskInfo(TaskManageInfo* taskInfo)
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
		sql.append("delete from rocket_task_info where id = '");
		sql.append(boost::lexical_cast<string>(taskInfo->m_id) + "'; ");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());
		if (!res)
		{
			m_app->m_TaskManageInfoFrames.erase(taskInfo->m_id);//更新记录map
			return true;
		}
		else
		{
			LOG(INFO) << "rocket_task_info" << "删除数据失败!" << sql;
			return false;
		}
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
	
	}

	///// <summary>
	///// 获取所有帧
	///// </summary>
	///// <returns></returns>
	//bool RocketDataDAO::getAllFrame()
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
	//bool RocketDataDAO::isSameName(string name)
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
	//bool RocketDataDAO::add(FrameInfo* fameInfo)
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
	//		LOG(INFO) << "RocketDataDAO[frame_info]:"<< "插入数据库失败!" << sql;
	//		return false;
	//	}
	//	return false;
	//}

	bool RocketDataDAO::test()
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

	bool RocketDataDAO::exec_sql(string  sql)
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


