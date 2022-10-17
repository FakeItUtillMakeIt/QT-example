#pragma execution_character_set("utf-8")
#include "RocketDataDAO.h"
#include <boost\lexical_cast.hpp>
#include <stdlib.h>
#include "../../../tools/quazip/include/JlCompress.h"

namespace DataBase
{
#define MYSQL_SET_BIND(bind_, type_, buffer_, is_null_, len_) \
	{ \
		bind_.buffer_type = type_; \
		bind_.buffer = buffer_; \
		bind_.is_null = is_null_; \
		bind_.length = len_; \
	}

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
	
	bool RocketDataDAO::SaveConfigToDb(QString& msg)
	{
		QString exepath = QApplication::applicationDirPath();
		QString rocketzip = exepath + "/rocket.zip";
		QFile  file(rocketzip);
		if (!file.exists())
		{
			msg = "zip file not exit";
			return false;
		}
		QString stylezip = exepath + "/style.zip";
		QFile  file2(stylezip);
		if (!file2.exists())
		{
			msg = "zip file not exit";
			return false;
		}


		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		string sql;
		sql.append("UPDATE config_info set updatetime=SYSDATE(),scene=LOAD_FILE(\"");
		sql.append(rocketzip.toStdString());
		sql.append("\")");
		sql.append(",style=LOAD_FILE(\"");
		sql.append(stylezip.toStdString());
		sql.append("\") where id = 1;");
	//	sql.append("INSERT INTO config_info VALUES(1, SYSDATE(), NULL)");
		bool bret = exec_sql(sql);
		if (!bret)
		{
			msg = "数据保存失败";
			return false;
		}

		return true;
	}

	bool RocketDataDAO::Compress(QString& msg)
	{
		QString exepath = QApplication::applicationDirPath();
		QString rocketzip = exepath + "/rocket.zip";
		QString rocketdir = exepath + "/rocket";
		QFile  file(rocketzip);
		if (file.exists())
		{
			QFile::remove(rocketzip);
		}
		bool bret = JlCompress::compressDir(rocketzip, rocketdir);
		if (!bret)
		{
			msg = "comppress failed";
			return false;
		}
		QString stylezip = exepath + "/style.zip";
		QString styledir = exepath + "/style";
		QFile  filestyle(stylezip);
		if (filestyle.exists())
		{
			QFile::remove(stylezip);
		}
		bret = JlCompress::compressDir(stylezip, styledir);
		if (!bret)
		{
			msg = "comppress failed";
			return false;
		}
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
			m_app->m_TaskManageInfo.clear();
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

					m_app->m_TaskManageInfo.insert(pair<int, TaskManageInfo*>(oneTask->m_id, oneTask));
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
	bool RocketDataDAO::initConfig(QString& msg)
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
   		sql.append("CREATE TABLE  if not EXISTS`config_info` (\
			`id` int NOT NULL,\
			`updatetime` datetime DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,\
			`scene` longblob,\
			`style` longblob,\
			PRIMARY KEY(`id`)\
			) ENGINE = InnoDB DEFAULT CHARSET = utf8; ");
 
		bool bret  = exec_sql(sql);
		if (!bret)
		{
			msg = "创建配置表失败";
			return false;
		}
		return true;
   }

	bool RocketDataDAO::ReadConfigTime(QString& msg,QString& curtime)
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
		sql.append("select updatetime,scene from config_info where id =1;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result) //如果查询到数据，取出数据
			{
				sql_row = mysql_fetch_row(result);
				if (sql_row)
				{
					curtime = sql_row[0];
					string content;
					if (sql_row[1])
					{
						mysql_free_result(result);//释放结果资源  
						return true;
					}
					else
					{
						msg = "数据库无最新数据";
						mysql_free_result(result);//释放结果资源  
						return false;
					}
				}
				//没查询到数据，插入初始数据
				sql.clear();
				sql.append("INSERT INTO config_info VALUES(1, SYSDATE(), NULL,NULL)");
				bool bret = exec_sql(sql);
				if (!bret)
				{
					msg = "插入初始数据失败";
					return false;
				}
			}
			else
				LOG(INFO) << "获取数据失败";
		}
		else {
			LOG(INFO) << "获取数据失败";
		
		}
		if (result)
			mysql_free_result(result);//释放结果资源  
		return false;
	}
	bool RocketDataDAO::removeFolderContent(const QString& folderDir)
	{
		QDir dir(folderDir);
		QFileInfoList fileList;
		QFileInfo curFile;
		if (!dir.exists()) { return false; }//文件不存，则返回false
		fileList = dir.entryInfoList(QDir::Dirs | QDir::Files
			| QDir::Readable | QDir::Writable
			| QDir::Hidden | QDir::NoDotAndDotDot
			, QDir::Name);
		while (fileList.size() > 0)
		{
			int infoNum = fileList.size();
			for (int i = infoNum - 1; i >= 0; i--)
			{
				curFile = fileList[i];
				if (curFile.isFile())//如果是文件，删除文件
				{
					QFile fileTemp(curFile.filePath());
					fileTemp.remove();
					fileList.removeAt(i);
				}
				if (curFile.isDir())//如果是文件夹
				{
					QDir dirTemp(curFile.filePath());
					QFileInfoList fileList1 = dirTemp.entryInfoList(QDir::Dirs | QDir::Files
						| QDir::Readable | QDir::Writable
						| QDir::Hidden | QDir::NoDotAndDotDot
						, QDir::Name);
					if (fileList1.size() == 0)//下层没有文件或文件夹
					{
						dirTemp.rmdir(".");
						fileList.removeAt(i);
					}
					else//下层有文件夹或文件
					{
						for (int j = 0; j < fileList1.size(); j++)
						{
							if (!(fileList.contains(fileList1[j])))
								fileList.append(fileList1[j]);
						}
					}
				}
			}
		}
		dir.removeRecursively();
		return true;
	}
	bool RocketDataDAO::ReadConfigFromDb(QString& msg, QString curtime)
	{
		bool  needupdate = false;
		//{
			QString filepath = QApplication::applicationDirPath();
			filepath += ("/config/time");
			QFile file(filepath);
			bool bret = false;
			bret = file.open(QIODevice::ReadWrite);
			if (!bret)
			{
				return false;
			}
			QByteArray filecontent = file.readAll();
			if (filecontent.isNull())
				needupdate = true;
			else
			{
				QDateTime dbtime = QDateTime::fromString(curtime, "yyyy-MM-dd hh:mm:ss");
				QDateTime curtime = QDateTime::fromString(filecontent, "yyyy-MM-dd hh:mm:ss");
				uint stime = dbtime.toTime_t();
				uint ctime = curtime.toTime_t();
				needupdate = (stime > ctime);
			}
			if (!needupdate)
				return false;
		//}

		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		{
			QString exepath = QApplication::applicationDirPath();
			filepath = exepath + "/newrocket.zip";
			QFile::remove(filepath);
			//MYSQL_RES* result = nullptr;
			//MYSQL_ROW sql_row;
			//int res;
			//string sql;
			//sql.append("select scene from config_info where id =1 into dumpfile\"");
			//sql.append(filepath.toStdString());
			//sql.append("\"");
			//bret = exec_sql(sql);
			//if (!bret)
			//{
			//	msg = "加载数据库组态配置失败";
			//	return false;
			//}
			const char* sql = "select scene from config_info where ID=1";
			if (GetConfigFile(filepath, sql))
			{
				QString compressdir = exepath + "/rocket";
				QString backdir = exepath + "/backrocket";
				bret = removeFolderContent(backdir);
				rename(compressdir.toStdString().data(), backdir.toStdString().data());
				JlCompress::extractDir(filepath, compressdir);
			}
			
		}
		{
			QString exepath = QApplication::applicationDirPath();
			filepath = exepath + "/newstyle.zip";
			QFile::remove(filepath);
			//MYSQL_RES* result = nullptr;
			//MYSQL_ROW sql_row;
			//int res;
			//string sql;
			//sql.append("select style from config_info where id =1 into dumpfile\"");
			//sql.append(filepath.toStdString());
			//sql.append("\"");
			//bret = exec_sql(sql);
			//if (!bret)
			//{
			//	msg = "加载数据库组态配置失败";
			//	return false;
			//}
			const char* sql = "select style from config_info where ID=1";
			if (GetConfigFile(filepath, sql))
			{
				QString compressdir = exepath + "/style";
				QString backdir = exepath + "/backstyle";
				bret = removeFolderContent(backdir);
				rename(compressdir.toStdString().data(), backdir.toStdString().data());
				JlCompress::extractDir(filepath, compressdir);
			}
		}
		file.close();
		bret = file.open(QIODevice::WriteOnly);
		if (!bret)
		{
			return false;
		}
		file.write(curtime.toLatin1());
		file.close();
		return true;
	}
	bool RocketDataDAO::getMainSchedule(int rocketid,QVector<int>&  schedue)
	{
		bool bret = true;
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		MYSQL_RES* result = nullptr;
		MYSQL_ROW sql_row;
		int res;
	//	QString  sql = QString("select s.command_id from main_flow_info as m LEFT JOIN sub_flow_info as s on m.id=s.main_id where m.rocket_id=%1 ORDER BY s.main_id ").arg(rocketid);
		QString  sql = QString("select s.command_id from main_flow_info as m LEFT JOIN sub_flow_info as s on m.id=s.main_id where m.rocket_id=%1").arg(rocketid);
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.toLocal8Bit().data());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				while (sql_row = mysql_fetch_row(result))
				{
					int id = atoi(sql_row[0]);
					schedue.push_back(id);
				}
			}
			else
			{
				LOG(INFO) << "获取数据失败";
				bret = false;
			}
		}
		else
		{
			LOG(INFO) << "获取数据失败";
			bret = false;
		}
		if (result)
			mysql_free_result(result);//释放结果资源  
		return bret;
	}
	bool RocketDataDAO::GetConfigFile(QString filename, const char* sql)
	{
		MYSQL_STMT* stmt = mysql_stmt_init(&my_connection);
		assert(NULL != stmt);
		int sql_len = strlen(sql);
		int ret = mysql_stmt_prepare(stmt, sql, sql_len);
		assert(0 == ret);
		qDebug("param count:%d", (int)mysql_stmt_param_count(stmt));
		MYSQL_BIND result = { 0 };
		unsigned long total_length = 0;
		result.buffer_type = MYSQL_TYPE_LONG_BLOB;
		result.length = &total_length;		
		ret = mysql_stmt_bind_result(stmt, &result);
		assert(0 == ret);
		ret = mysql_stmt_execute(stmt);
		assert(0 == ret);
		ret = mysql_stmt_store_result(stmt);
		assert(0 == ret);
		QFile file(filename);
		if(file.open(QIODevice::WriteOnly|QIODevice::Append) == false)
		{
			return false;
		}	
		//while (mysql_stmt_fetch(stmt)!=0)
		for (;;)
		{
			ret = mysql_stmt_fetch(stmt);
			if (ret != 0 && ret != MYSQL_DATA_TRUNCATED) 
				return ret;
			int start = 0;
			char buf[1024] = { 0 };
			printf("total_length=%lu\n", total_length);
			result.buffer = buf;
			while (start < (int)total_length)
			{
			//	result.buffer = (buf + start);
				result.buffer_length = 1000;  //每次读这么长
				ret = mysql_stmt_fetch_column(stmt, &result, 0, start);
				if (ret != 0)
				{
					qDebug("code=%d, msg=%s", (int)mysql_errno(&my_connection), mysql_error(&my_connection));
					return false;
				}
				start += result.buffer_length;
				int writelenth = 0;
				if (start > (int)total_length)
				{
					writelenth = result.buffer_length - (start - total_length);
				}
				else
				{
					writelenth = result.buffer_length;
				}
				file.write((const char*)result.buffer, writelenth);
			}			
			file.close();
		}
		mysql_stmt_close(stmt);	
		return true;
	}

	bool RocketDataDAO::SaveConfigFile(QString filename, const char* sql)
	{
		MYSQL_STMT* stmt = mysql_stmt_init(&my_connection);
		assert(NULL != stmt);
		//const char* sql = "update config_info set scene=? where id=1";
		int sql_len = strlen(sql);
		int ret = mysql_stmt_prepare(stmt, sql, sql_len);
		assert(0 == ret);
		qDebug("param count:%d", (int)mysql_stmt_param_count(stmt));
		char null_flag = 0;
		MYSQL_BIND param = { 0 };
		MYSQL_SET_BIND(param, MYSQL_TYPE_LONG_BLOB, NULL, &null_flag, NULL);
		ret = mysql_stmt_bind_param(stmt, &param);
		assert(0 == ret);
		QFile file(filename);
		if (file.open(QIODevice::ReadOnly) == false)
		{
			return false;
		}
		char buf[1001];
		int iret = file.read(buf,1000);
		while (iret>0)	
		{
			char ret1 = mysql_stmt_send_long_data(stmt, 0, buf, iret);
			if (ret1 != 0)
			{
				qDebug("code=%d, msg=%s", (int)mysql_errno(&my_connection), mysql_error(&my_connection));
				return false;
			}
			iret = file.read(buf, 1000);
		}
		ret = mysql_stmt_execute(stmt);
		assert(0 == ret);
		mysql_stmt_close(stmt);
		return true;
	}
}


