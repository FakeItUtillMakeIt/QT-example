#pragma execution_character_set("utf-8")
#include "FaultDAO.h"
#include <boost\lexical_cast.hpp>
 
#include <vector>
using namespace std;

#define MAXCODE 65535

namespace DataBaseF
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
	/// 获取所有故障指令信息
	/// </summary>
	/// <returns></returns>
	bool FaultDAO::GetFaultCommand()
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
		sql.append("select b.name as '火箭型号', a.* from command_info a left join rocket_info b on a.rocket_id = b.id,a.rocket_id = 2 ;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_app->m_allFaultCommad.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					/*Command* oneCommnd = new Command();
					oneCommnd->m_sRocketName = Utils::UTF8ToGBK(sql_row[0]);
					oneCommnd->m_id = atoi(sql_row[1]);
					oneCommnd->m_iRocketId = atoi(sql_row[2]);
					oneCommnd->m_iBackId = atoi(sql_row[3]);
					oneCommnd->m_sName = Utils::UTF8ToGBK(sql_row[4]);
					oneCommnd->m_iCode = atoi(sql_row[5]);
					oneCommnd->m_iType = atoi(sql_row[6]);
					m_app->m_allFaultCommad.insert(pair<int, Command*>(oneCommnd->m_id, oneCommnd));*/
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
				 
					//对应到火箭型号
					if (oneFrame->m_rocketID != m_app->m_rockedType)
					{
						continue;
					}
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
	/// 根据名称删除
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	bool FaultDAO::DelCommandInfoParam(string name)
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
		sql.append("delete from command_info where name = '");
		sql.append(boost::lexical_cast<string>(name) + "'; ");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			//更新记录map
			for (map<int, FaultCommandInfo*>::iterator iter = m_app->m_CommandInfoframes.begin(); iter != m_app->m_CommandInfoframes.end(); iter++)
			{
				if (iter->second->m_name == name)
				{
					m_app->m_CommandInfoframes.erase(iter->first);
					return true;
				}
			}
		}
		else
			LOG(INFO) << "FaultParamDAO[command_info]:" << "删除数据失败!" << sql;
		if (result)
			mysql_free_result(result);//释放结果资源  
		return false;
	}


	/// <summary>
	/// 获取当前 command_info中所有的code
	/// </summary>
	/// <returns></returns>
	bool FaultDAO::GetCommandCode(vector<int>* allCode)
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
		sql.append("SELECT command_info.code FROM command_info;");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				allCode->clear();
				while (sql_row = mysql_fetch_row(result))
				{
					int code = atoi(sql_row[0]);
					allCode->push_back(code);
				}
			}
			else
				LOG(INFO) << "获取Command_Info的code数据失败";
		}
		else
			LOG(INFO) << "获取Command_Info的code数据失败";
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
		return false;
	}

	/// <summary>
	/// 获取新的唯一的code
	/// </summary>
	/// <returns></returns>
	int FaultDAO::GetNewCode()
	{
		int newcode = 0;
		vector<int> code;
		GetCommandCode(&code);

		while (true)
		{
			newcode = rand() % MAXCODE;
			if (find(code.begin(), code.end(), newcode) == code.end())
			{
				break;
			}
		}

		return newcode;
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


	/// <summary>
	/// 返回表中最大id+1
	/// </summary>
	/// <param name="tableName"></param>
	/// <returns></returns>
	int FaultDAO::GetNowId(string tableName)
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
		string sql ;
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


	int FaultDAO::GetNowId2(string tableName)
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
		sql.append("SHOW TABLE STATUS [FROM simulatedtraining] [LIKE ");
		sql.append(tableName);
		sql.append("];");
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
}
