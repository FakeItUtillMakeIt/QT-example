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

	bool DeviceDAO::getDevice()
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
		sql.append("select b.name as '火箭型号', a.* from device_info a left join rocket_info b on a.rocket_id = b.id ;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_allDevice.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					Device* oneDevice = new Device();
					oneDevice->m_sRocketName = Utils::UTF8ToGBK(sql_row[0]);
					oneDevice->m_id = atoi(sql_row[1]);
					oneDevice->m_iRocketId = atoi(sql_row[2]);
					oneDevice->m_deviceName = Utils::UTF8ToGBK(sql_row[3]);
					oneDevice->isVirtual = atoi(sql_row[4]);
					m_app->m_allDevice.push_back(oneDevice);
					m_app->m_allDeviceCopy.insert(pair<int, Device*>(oneDevice->m_id, oneDevice));
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

	bool DeviceDAO::getDeviceParam()
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
		sql.append("SELECT a.id, b.id as device_id, b.name as device_name,c.id as subparameter_id, c.name as subparameter_name, c.unit FROM device_param_info a left join device_info b on a.device_id = b.id left join parameter_info c on a.parameter_id = c.id");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_allDeviceParam.clear();
				m_app->m_dev2DeviceParamID.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					DeviceParam* oneDeviceParam = new DeviceParam();
					oneDeviceParam->m_id = atoi(sql_row[0]);
					oneDeviceParam->m_deviceId = atoi(sql_row[1]);
					oneDeviceParam->m_deviceName = Utils::UTF8ToGBK(sql_row[2]);
					oneDeviceParam->m_subParameterId = atoi(sql_row[3]);
					oneDeviceParam->m_subParameterName = Utils::UTF8ToGBK(sql_row[4]);
					oneDeviceParam->m_unit = Utils::UTF8ToGBK(sql_row[5]);
					m_app->m_allDeviceParam.insert(pair<int, DeviceParam*>(oneDeviceParam->m_id, oneDeviceParam));
					m_app->m_dev2DeviceParamID[oneDeviceParam->m_deviceId].push_back(oneDeviceParam->m_id);
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
	/// 获取状态列表
	/// </summary>
	/// <returns></returns>
	bool DeviceDAO::getStatus()
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
		sql.append("SELECT * FROM status_info;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_allStatus.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					Status* one = new Status();
					one->m_id = atoi(sql_row[0]);
					one->m_name = Utils::UTF8ToGBK(sql_row[1]);
					m_app->m_allStatus.insert(pair<int, Status*>(one->m_id, one));
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
	/// 查询设备状态
	/// </summary>
	/// <returns></returns>
	bool DeviceDAO::getDeviceStatus()
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
		sql.append("SELECT a.id, a.device_id,c.name,a.dataPath  FROM device_status_info a left join device_info b on a.device_id = b.id left join status_info c on a.status_id = c.id");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_allDeviceStatus.clear();
				m_app->m_dev2DeviceStatusID.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					DeviceStatus* oneDeviceParam = new DeviceStatus();
					oneDeviceParam->m_id = atoi(sql_row[0]);
					oneDeviceParam->m_deviceId = atoi(sql_row[1]);
					oneDeviceParam->m_statusName = Utils::UTF8ToGBK(sql_row[2]);
					oneDeviceParam->m_dataPath = sql_row[3];
					m_app->m_allDeviceStatus.insert(pair<int, DeviceStatus*>(oneDeviceParam->m_id, oneDeviceParam));
					m_app->m_dev2DeviceStatusID[oneDeviceParam->m_deviceId].push_back(oneDeviceParam->m_id);
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



	///// <summary>
	///// 获取所有帧
	///// </summary>
	///// <returns></returns>
	//bool DeviceDAO::getAllFrame()
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
	//bool DeviceDAO::isSameName(string name)
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
	//bool DeviceDAO::add(FrameInfo* fameInfo)
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
	//		LOG(INFO) << "DeviceDAO[frame_info]:"<< "插入数据库失败!" << sql;
	//		return false;
	//	}
	//	return false;
	//}

	bool DeviceDAO::test()
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

	/**
	@brief  获取数据
	@param  sql_str -
	@retval         -
**/
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
				contents[atoi(row[0])].push_back(string(row[i]));
			}
			counts++;
		}

		return counts;
	}
}


