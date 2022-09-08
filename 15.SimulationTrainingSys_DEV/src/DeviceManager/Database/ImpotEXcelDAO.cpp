//#pragma execution_character_set("utf-8")
#include "ImpotEXcelDAO.h"
#include <boost\lexical_cast.hpp>

namespace DataBase
{
	ImpotEXcelDAO::ImpotEXcelDAO(OutputPath* path)
		: m_path(path)
		, is_connected(false)
		, m_importRocketId(-1)
	{
		m_app = AppCache::instance();
	}

	ImpotEXcelDAO::~ImpotEXcelDAO()
	{
		if (is_connected)
		{
			mysql_close(&my_connection);//断开连接  
			is_connected = false;
		}
	}


	void ImpotEXcelDAO::Init(string name)
	{
		GetRocketType();//获取全部火箭型号

		for (auto item : m_allRocketTypeI)
		{
			if (item.second->m_name.compare(name) == 0)
			{
				m_importRocketName = name;
				m_importRocketId = item.second->m_id;

				GetAllParamInfo();//获取所有参数
				GetCommandInfo();//获取所有指令
				GetAllParamtable();//获取所有参数表
				GetAllCommandtable();//获取所有指令表
				return;
			}
		}
	}

	bool ImpotEXcelDAO::connect()
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
	bool ImpotEXcelDAO::connected()
	{
		return is_connected;
	}

	/// <summary>
	/// 获取当前所有的火箭类型
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::GetRocketType()
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
				m_allRocketTypeI.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					RocketType* oneDeviceParam = new RocketType();
					oneDeviceParam->m_id = atoi(sql_row[0]);
					oneDeviceParam->m_name = sql_row[1];
					oneDeviceParam->m_code = sql_row[2];
					m_allRocketTypeI.insert(pair<int, RocketType*>(oneDeviceParam->m_id, oneDeviceParam));
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
	/// 获取全部的参数
	/// </summary>
	/// <returns></returns>
	//bool ImpotEXcelDAO::GetWholeParamInfo()
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
	//	sql.append("SELECT * FROM parameter_info");

	//	mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
	//	res = mysql_query(&my_connection, sql.c_str());//查询
	//	if (!res)
	//	{
	//		result = mysql_store_result(&my_connection);
	//		if (result)
	//		{
	//			m_wholeParamI.clear();
	//			while (sql_row = mysql_fetch_row(result))
	//			{
	//				ParamInfo* oneParam = new ParamInfo();
	//				oneParam->m_id = atoi(sql_row[0]);
	//				oneParam->m_name = sql_row[1];
	//				oneParam->m_type = atoi(sql_row[2]);
	//				oneParam->m_unit = sql_row[3];

	//				if (oneParam->m_rocketid == m_importRocketId)
	//				{
	//					m_wholeParamI.insert(pair<int, ParamInfo*>(oneParam->m_id, oneParam));
	//				}
	//			}
	//		}
	//		else
	//		{
	//			LOG(INFO) << "获取参数数据失败";
	//			return false;
	//		}
	//	}
	//	else
	//	{
	//		LOG(INFO) << "获取参数数据失败";
	//		return false;
	//	}
	//	if (result)
	//		mysql_free_result(result);//释放结果资源  
	//	return true;
	//}

	
	/// <summary>
	/// 获取当前所有的参数
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::GetAllParamInfo()
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
			"parameter_info.id,"
			"parameter_info.name,"
			"parameter_info.type,"
			"parameter_info.unit,"
			"parameter_rocket_info.rocket_id,"
			"parameter_rocket_info.param_table_id "
			"FROM "
			"parameter_info "
			"INNER JOIN parameter_rocket_info ON parameter_info.id = parameter_rocket_info.parameter_id");

		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_allParamI.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					ParamInfo* oneParam = new ParamInfo();
					oneParam->m_id = atoi(sql_row[0]);
					oneParam->m_name = sql_row[1];
					oneParam->m_type = atoi(sql_row[2]);
					oneParam->m_unit = sql_row[3];
					oneParam->m_rocketid = atoi(sql_row[4]);
					oneParam->m_paramtableid = atoi(sql_row[5]);

					if (oneParam->m_rocketid == m_importRocketId)
					{
						m_allParamI.insert(pair<int, ParamInfo*>(oneParam->m_id, oneParam));
					}
				}
			}
			else
			{
				LOG(INFO) << "获取参数数据失败";
				return false;
			}
		}
		else
		{
			LOG(INFO) << "获取参数数据失败";
			return false;
		}
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
	}



	/// <summary>
	/// 获取当前所有的指令
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::GetCommandInfo()
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
		sql.append("SELECT * FROM command_info");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_allCommadI.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					Command* oneCommand = new Command();
					oneCommand->m_id = atoi(sql_row[0]);
					oneCommand->m_iRocketId = atoi(sql_row[1]);
					oneCommand->m_iBackId = atoi(sql_row[2]);
					oneCommand->m_sName = sql_row[3];
					oneCommand->m_iCode = atoi(sql_row[4]);
					oneCommand->m_iType = atoi(sql_row[5]);

					if (oneCommand->m_iRocketId == m_importRocketId)
					{
						m_allCommadI.insert(pair<int, Command*>(oneCommand->m_id, oneCommand));
					}
					
				}
			}
			else
			{
				LOG(INFO) << "获取指令数据失败";
				return false;
			}
		}
		else
		{
			LOG(INFO) << "获取指令数据失败";
			return false;
		}
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
	}

	/// <summary>
	/// 所有指令表
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::GetAllCommandtable()
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
		sql.append("SELECT * FROM command_table_info");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_allCommandtableI.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					PCTable* onetable = new PCTable();
					onetable->m_id = atoi(sql_row[0]);
					onetable->m_tableName = sql_row[2];

					m_allCommandtableI.insert(pair<int, PCTable*>(onetable->m_id, onetable));
				}
			}
			else
			{
				LOG(INFO) << "获取指令表失败";
				return false;
			}	
		}
		else
		{
			LOG(INFO) << "获取指令表失败";
			return false;
		}
			
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
	}

	/// <summary>
	/// 所有参数表
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::GetAllParamtable()
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
		sql.append("SELECT * FROM param_table_info");
		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		res = mysql_query(&my_connection, sql.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&my_connection);
			if (result)
			{
				m_allParamtableI.clear();
				while (sql_row = mysql_fetch_row(result))
				{
					PCTable* onetable = new PCTable();
					onetable->m_id = atoi(sql_row[0]);
					onetable->m_tableName = sql_row[2];

					m_allParamtableI.insert(pair<int, PCTable*>(onetable->m_id, onetable));
				}
			}
			else
			{
				LOG(INFO) << "获取参数表失败";
				return false;
			}
				
		}
		else
		{
			LOG(INFO) << "获取参数表失败";
			return false;
		}
		if (result)
			mysql_free_result(result);//释放结果资源  
		return true;
	}

	/// <summary>
	/// 判断当前参数表是否存在，不存在则写入,返回当前参数参数表的id
	/// </summary>
	/// return -1：错误；0：已存在
	/// <returns></returns>
	int ImpotEXcelDAO::ParamtableIsExist(PCTable pTable)
	{
		int tableNameId = -1;

		//判断是否存在
		for(auto item : m_allParamtableI)
		{
			if (item.second->m_tableName.compare(pTable.m_tableName) == 0)
			{
				tableNameId = item.first;
				return tableNameId;
			}
		}

		//不存在则数据库中添加
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return tableNameId;
		}
		stringstream ss;
		ss << "insert into param_table_info(rocket_id,name) values ('";
		ss << boost::lexical_cast<string>(pTable.m_roketID) + "','";
		ss << boost::lexical_cast<string>(pTable.m_tableName) + "')";

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

					PCTable* oneTable = new PCTable();
					oneTable->m_id = atoi(sql_row[0]);
					oneTable->m_tableName = pTable.m_tableName;

					m_allParamtableI.insert(pair<int, PCTable*>(oneTable->m_id, oneTable));
					tableNameId = oneTable->m_id;
					return tableNameId;
				}
			}
		}
		else
		{
			LOG(INFO) << "插入参数表失败!" << sql;
			return tableNameId;
		}
		return tableNameId;
	}


	/// <summary>
	/// 判断整个参数表中是否存在该参数，不存在则写入
	/// 返回参数id
	/// </summary>
	/// <returns></returns>
	//bool ImpotEXcelDAO::NowParamExist(ParamInfo* oneParamInfo, int& ParamId)
	//{
	//	for (auto item : m_wholeParamI)
	//	{
	//		if (item.second->m_name.compare(oneParamInfo->m_name) == 0)
	//		{
	//			ParamId = item.first;
	//			return true;
	//		}
	//	}

	//	if (!connected())
	//	{
	//		LOG(INFO) << "创建数据库连接";
	//		if (!connect())
	//			return false;
	//	}
	//	stringstream ss;
	//	ss << "insert into parameter_info(name,type,unit) values ('";
	//	ss << boost::lexical_cast<string>(oneParamInfo->m_name) + "','";
	//	ss << boost::lexical_cast<string>(oneParamInfo->m_type) + "','";
	//	ss << boost::lexical_cast<string>(oneParamInfo->m_unit) + "')";

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

	//				PCTable* oneTable = new PCTable();
	//				oneTable->m_id = atoi(sql_row[0]);
	//				//m_allParamtableI.insert(pair<int, PCTable*>(oneTable->m_id, oneTable));
	//				ParamId = oneTable->m_id;
	//				return true;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		LOG(INFO) << "插入参数失败!" << sql;
	//		return false;
	//	}
	//	return true;
	//}

	/// <summary>
	/// 判断当前火箭是否绑定该参数
	/// </summary>
	/// <returns></returns>
	int ImpotEXcelDAO::NowRocketParamExist(ParamInfo* oneParamInfo,bool& isExit)
	{
		int paramId = -1;
		for (auto item : m_allParamI)
		{
			if (item.second->m_name.compare(oneParamInfo->m_name) == 0)
			{
				paramId = item.first;
				isExit = true;
				return paramId;
			}
		}
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return paramId;
		}
		stringstream ss;
		ss << "insert into parameter_info(name,type,unit) values ('";
		ss << boost::lexical_cast<string>(oneParamInfo->m_name) + "','";
		ss << boost::lexical_cast<string>(oneParamInfo->m_type) + "','";
		ss << boost::lexical_cast<string>(oneParamInfo->m_unit) + "')";

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

					PCTable* oneTable = new PCTable();
					oneTable->m_id = atoi(sql_row[0]);
					//m_allParamtableI.insert(pair<int, PCTable*>(oneTable->m_id, oneTable));
					paramId = oneTable->m_id;
					return paramId;
				}
			}
		}
		else
		{
			LOG(INFO) << "插入参数失败!" << sql;
			return paramId;
		}
		return paramId;

	}

	/// <summary>
	/// 写入参数和参数对应关系表
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::InsertParamParamtable(ParamInfo* oneParamInfo)
	{
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		stringstream ss;
		ss << "insert into parameter_rocket_info(parameter_id,rocket_id,param_table_id) values ('";
		ss << boost::lexical_cast<string>(oneParamInfo->m_id) + "','";
		ss << boost::lexical_cast<string>(oneParamInfo->m_rocketid) + "','";
		ss << boost::lexical_cast<string>(oneParamInfo->m_paramtableid) + "')";

		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		string sql = ss.str();
		int res = mysql_query(&my_connection, sql.c_str());
		if (!res)
		{
			return true;
		}
		else
		{
			LOG(INFO) << "插入参数失败!" << sql;
			return false;
		}
		return true;
	}

	/// <summary>
	/// 判断当前指令表是否存在，不存在则写入
	/// </summary>
	/// <returns></returns>
	int ImpotEXcelDAO::CommandtableIsExist(PCTable cTable)
	{
		int cmdTableId = -1;
		//判断是否存在
		for (auto item : m_allCommandtableI)
		{
			if (item.second->m_tableName.compare(cTable.m_tableName) == 0)
			{
				cmdTableId = item.first;
				return cmdTableId;
			}
		}

		//不存在则数据库中添加
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return cmdTableId;
		}
		stringstream ss;
		ss << "insert into command_table_info(rocket_id,name) values ('";
		ss << boost::lexical_cast<string>(cTable.m_roketID) + "','";
		ss << boost::lexical_cast<string>(cTable.m_tableName) + "')";

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

					PCTable* oneTable = new PCTable();
					oneTable->m_id = atoi(sql_row[0]);
					oneTable->m_tableName = cTable.m_tableName;

					m_allCommandtableI.insert(pair<int, PCTable*>(oneTable->m_id, oneTable));
					cmdTableId = oneTable->m_id;
					return cmdTableId;
				}
			}
		}
		else
		{
			LOG(INFO) << "插入指令表失败!" << sql;
			return cmdTableId;
		}
		return cmdTableId;
	}


	/// <summary>
	/// 写入新的指令 
	/// </summary>
	/// <returns></returns>
	int ImpotEXcelDAO::NowCommandExist(Command* oneCommand)
	{
		int cmdId = -1;
		//判断是否存在
		for (auto item : m_allCommadI)
		{
			if (item.second->m_sName.compare(oneCommand->m_sName) == 0)
			{
				cmdId = item.first;
				return cmdId;
			}
		}

		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		stringstream ss;
		ss << "insert into command_info(rocket_id,back_id,name,code,type) values ('";
		ss << boost::lexical_cast<string>(oneCommand->m_iRocketId) + "','";
		ss << boost::lexical_cast<string>(oneCommand->m_iBackId) + "','";
		ss << boost::lexical_cast<string>(oneCommand->m_sName) + "','";
		ss << boost::lexical_cast<string>(oneCommand->m_iCode) + "','";
		ss << boost::lexical_cast<string>(oneCommand->m_iType) + "')";

		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		string sql = ss.str();
		int res = mysql_query(&my_connection, sql.c_str());
		if (!res)
		{
			//每次更新用于后续查重
			if (GetCommandInfo())
			{
				for (auto item : m_allCommadI)
				{
					if (item.second->m_sName == oneCommand->m_sName)
					{
						cmdId = item.first;
						return cmdId;
					}
				}
				return cmdId;
			}
			else
				return cmdId;
		}
		else
		{
			LOG(INFO) << "插入指令失败!" << sql;
			return cmdId;
		}
		return cmdId;
	}

	/// <summary>
	/// 写入当前新指令的回令 
	/// </summary>
	/// <returns></returns>
	int ImpotEXcelDAO::NowCommandBackExist(Command* oneCommandBack, bool& isExit)
	{
		int cmdBackId = -1;
		//判断是否存在
		for (auto item : m_allCommadI)
		{
			if (item.second->m_sName.compare(oneCommandBack->m_sName) == 0)
			{
				cmdBackId = item.first;
				isExit = true;
				return cmdBackId;//指令存在返回 0
			}
		}

		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return cmdBackId;
		}
		stringstream ss;
		ss << "insert into command_info(rocket_id,back_id,name,code,type) values ('";
		ss << boost::lexical_cast<string>(oneCommandBack->m_iRocketId) + "','";
		ss << boost::lexical_cast<string>(oneCommandBack->m_iBackId) + "','";
		ss << boost::lexical_cast<string>(oneCommandBack->m_sName) + "','";
		ss << boost::lexical_cast<string>(oneCommandBack->m_iCode) + "','";
		ss << boost::lexical_cast<string>(oneCommandBack->m_iType) + "')";

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
					oneCommandBack->m_id = atoi(sql_row[0]);//得到回令的id 用于写指令的时候使用   TODO注意释放result
					cmdBackId = oneCommandBack->m_id;
					return cmdBackId;
				}
			}

			////每次更新用于后续查重
			//if (GetCommandInfo())
			//{
			//	for (auto item : m_allCommadI)
			//	{
			//		if (item.second->m_sName.compare(oneCommandBack->m_sName) == 0)
			//		{
			//			cmdBackId = item.first;
			//			return cmdBackId;
			//		}
			//	}
			//	return cmdBackId;
			//}
			//else
			//	return cmdBackId;
		}
		else
		{
			LOG(INFO) << "插入指令回令失败!" << sql;
			return cmdBackId;
		}
		return cmdBackId;
	}


	/// <summary>
	/// 写入指令和指令对应关系表
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::InsertCommandCommandtable(Command* oneCommand)
	{
		if (!connected())
		{
			LOG(INFO) << "创建数据库连接";
			if (!connect())
				return false;
		}
		stringstream ss;
		ss << "insert into command_commandtable_info(command_id,command_table_id) values ('";
		ss << boost::lexical_cast<string>(oneCommand->m_id) + "','";
		ss << boost::lexical_cast<string>(oneCommand->m_commandTableId) + "')";


		mysql_query(&my_connection, "SET NAMES UTF8"); //设置编码格式
		string sql = ss.str();
		int res = mysql_query(&my_connection, sql.c_str());
		if (!res)
		{
			return true;
		}
		else
		{
			LOG(INFO) << "插入指令和指令对应关系表失败!" << sql;
			return false;
		}
		return true;
	}

}


