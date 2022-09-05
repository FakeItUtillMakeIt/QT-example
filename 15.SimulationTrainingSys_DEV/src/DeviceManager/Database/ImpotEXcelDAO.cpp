#pragma execution_character_set("utf-8")
#include "ImpotEXcelDAO.h"
#include <boost\lexical_cast.hpp>

namespace DataBase
{
	ImpotEXcelDAO::ImpotEXcelDAO(OutputPath* path)
		: m_path(path)
		, is_connected(false)
	{
		m_app = AppCache::instance();
	}

	ImpotEXcelDAO::~ImpotEXcelDAO()
	{
		if (is_connected)
		{
			mysql_close(&my_connection);//�Ͽ�����  
			is_connected = false;
		}
	}

	bool ImpotEXcelDAO::connect()
	{
		if (!is_connected)
		{
			if (!m_path)
			{
				LOG(INFO) << "ȱ�����ݿ�����������Ϣ";
				return false;
			}
			mysql_init(&my_connection);
			if (!mysql_real_connect(&my_connection, m_path->m_server.c_str(), m_path->m_user_name.c_str(),
				m_path->m_password.c_str(), m_path->m_db_name.c_str(), m_path->m_port, NULL, 0))
			{
				LOG(INFO) << "�������ݿ�ʧ��";
				mysql_close(&my_connection);//�Ͽ�����  
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
	/// ��ȡ��ǰ���еĻ������
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::GetRocketType()
	{
		if (!connected())
		{
			LOG(INFO) << "�������ݿ�����";
			if (!connect())
				return false;
		}
		MYSQL_RES* result = nullptr;
		MYSQL_ROW sql_row;
		int res;
		string sql;
		sql.append("SELECT * FROM rocket_info");
		mysql_query(&my_connection, "SET NAMES UTF8"); //���ñ����ʽ
		res = mysql_query(&my_connection, sql.c_str());//��ѯ
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
					oneDeviceParam->m_name = Utils::UTF8ToGBK(sql_row[1]);
					oneDeviceParam->m_code = Utils::UTF8ToGBK(sql_row[2]);
					m_allRocketTypeI.insert(pair<int, RocketType*>(oneDeviceParam->m_id, oneDeviceParam));
				}
			}
			else
			{
				LOG(INFO) << "��ȡ����ʧ��";
				return false;
			}
		}
		else
		{
			LOG(INFO) << "��ȡ����ʧ��";
			return false;
		}		
		if (result)
			mysql_free_result(result);//�ͷŽ����Դ  
		return true;
	}
	
	/// <summary>
	/// ��ȡ��ǰ���еĲ���
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::GetAllParamInfo()
	{
		if (!connected())
		{
			LOG(INFO) << "�������ݿ�����";
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
			"parameter_rocket_info.param_table_id"
			"FROM "
			"parameter_info "
			"INNER JOIN "
			"parameter_rocket_info ON parameter_info.id = parameter_rocket_info.parameter_id");

		mysql_query(&my_connection, "SET NAMES UTF8"); //���ñ����ʽ
		res = mysql_query(&my_connection, sql.c_str());//��ѯ
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
					oneParam->m_name = Utils::UTF8ToGBK(sql_row[1]);
					oneParam->m_type = atoi(sql_row[2]);
					oneParam->m_unit = Utils::UTF8ToGBK(sql_row[3]);
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
				LOG(INFO) << "��ȡ��������ʧ��";
				return false;
			}
		}
		else
		{
			LOG(INFO) << "��ȡ��������ʧ��";
			return false;
		}
		if (result)
			mysql_free_result(result);//�ͷŽ����Դ  
		return true;
	}



	/// <summary>
	/// ��ȡ��ǰ���е�ָ��
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::GetCommandInfo()
	{
		if (!connected())
		{
			LOG(INFO) << "�������ݿ�����";
			if (!connect())
				return false;
		}
		MYSQL_RES* result = nullptr;
		MYSQL_ROW sql_row;
		int res;
		string sql;
		sql.append("SELECT * FROM command_info");
		mysql_query(&my_connection, "SET NAMES UTF8"); //���ñ����ʽ
		res = mysql_query(&my_connection, sql.c_str());//��ѯ
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
					oneCommand->m_sName = Utils::UTF8ToGBK(sql_row[2]);
					oneCommand->m_iCode = atoi(sql_row[3]);
					oneCommand->m_iType = atoi(sql_row[4]);

					if (oneCommand->m_iRocketId == m_importRocketId)
					{
						m_allCommadI.insert(pair<int, Command*>(oneCommand->m_id, oneCommand));
					}
					
				}
			}
			else
			{
				LOG(INFO) << "��ȡָ������ʧ��";
				return false;
			}
		}
		else
		{
			LOG(INFO) << "��ȡָ������ʧ��";
			return false;
		}
		if (result)
			mysql_free_result(result);//�ͷŽ����Դ  
		return true;
	}

	/// <summary>
	/// ����ָ���
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::GetAllCommandtable()
	{
		if (!connected())
		{
			LOG(INFO) << "�������ݿ�����";
			if (!connect())
				return false;
		}
		MYSQL_RES* result = nullptr;
		MYSQL_ROW sql_row;
		int res;
		string sql;
		sql.append("SELECT * FROM command_table_info");
		mysql_query(&my_connection, "SET NAMES UTF8"); //���ñ����ʽ
		res = mysql_query(&my_connection, sql.c_str());//��ѯ
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
					onetable->m_tableName = Utils::UTF8ToGBK(sql_row[1]);

					m_allCommandtableI.insert(pair<int, PCTable*>(onetable->m_id, onetable));
				}
			}
			else
			{
				LOG(INFO) << "��ȡָ���ʧ��";
				return false;
			}	
		}
		else
		{
			LOG(INFO) << "��ȡָ���ʧ��";
			return false;
		}
			
		if (result)
			mysql_free_result(result);//�ͷŽ����Դ  
		return true;
	}

	/// <summary>
	/// ���в�����
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::GetAllParamtable()
	{
		if (!connected())
		{
			LOG(INFO) << "�������ݿ�����";
			if (!connect())
				return false;
		}
		MYSQL_RES* result = nullptr;
		MYSQL_ROW sql_row;
		int res;
		string sql;
		sql.append("SELECT * FROM param_table_info");
		mysql_query(&my_connection, "SET NAMES UTF8"); //���ñ����ʽ
		res = mysql_query(&my_connection, sql.c_str());//��ѯ
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
					onetable->m_tableName = Utils::UTF8ToGBK(sql_row[1]);

					m_allParamtableI.insert(pair<int, PCTable*>(onetable->m_id, onetable));
				}
			}
			else
			{
				LOG(INFO) << "��ȡ������ʧ��";
				return false;
			}
				
		}
		else
		{
			LOG(INFO) << "��ȡ������ʧ��";
			return false;
		}
		if (result)
			mysql_free_result(result);//�ͷŽ����Դ  
		return true;
	}

	/// <summary>
	/// �жϵ�ǰ�������Ƿ���ڣ���������д��
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::ParamtableIsExist(string pTableName)
	{
		bool isexist = false;
		for(auto item : m_allParamtableI)
		{
			if (item.second->m_tableName == pTableName)
			{
				isexist = true;
				break;
			}
		}

		//����
		if (isexist)
		{
			return true;
		}

		//�����������ݿ������
		if (!connected())
		{
			LOG(INFO) << "�������ݿ�����";
			if (!connect())
				return false;
		}
		stringstream ss;
		ss << "insert into param_table_info(name) values ('";
		ss << boost::lexical_cast<string>(pTableName) + "')";

		mysql_query(&my_connection, "SET NAMES UTF8"); //���ñ����ʽ
		string sql = ss.str();
		int res = mysql_query(&my_connection, sql.c_str());
		if (!res)
		{
			int res1;
			mysql_query(&my_connection, "SET NAMES UTF8"); //���ñ����ʽ
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
					oneTable->m_tableName = pTableName;

					m_allParamtableI.insert(pair<int, PCTable*>(oneTable->m_id, oneTable));
					return true;
				}
			}
		}
		else
		{
			LOG(INFO) << "���������ʧ��!" << sql;
			return false;
		}
		return true;
	}

	/// <summary>
	/// д���µĲ���
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::InsertNewParam(ParamInfo* oneParamInfo)
	{
		if (!connected())
		{
			LOG(INFO) << "�������ݿ�����";
			if (!connect())
				return false;
		}
		stringstream ss;
		ss << "insert into parameter_info(name,type,unit) values ('";
		ss << boost::lexical_cast<string>(oneParamInfo->m_name) + "','";
		ss << boost::lexical_cast<string>(oneParamInfo->m_type) + "','";
		ss << boost::lexical_cast<string>(oneParamInfo->m_unit) + "')";

		mysql_query(&my_connection, "SET NAMES UTF8"); //���ñ����ʽ
		string sql = ss.str();
		int res = mysql_query(&my_connection, sql.c_str());
		if (!res)
		{
			//ÿ�θ������ں�������
			if (GetAllParamtable())
				return true;
			else
				return false;
		}
		else
		{
			LOG(INFO) << "�������ʧ��!" << sql;
			return false;
		}
		return true;
	}

	/// <summary>
	/// д������Ͳ�����Ӧ��ϵ��
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::InsertParamParamtable(ParamInfo* oneParamInfo)
	{
		if (!connected())
		{
			LOG(INFO) << "�������ݿ�����";
			if (!connect())
				return false;
		}
		stringstream ss;
		ss << "insert into parameter_rocket_info(parameter_id,rocket_id,param_table_id) values ('";
		ss << boost::lexical_cast<string>(oneParamInfo->m_id) + "','";
		ss << boost::lexical_cast<string>(oneParamInfo->m_rocketid) + "','";
		ss << boost::lexical_cast<string>(oneParamInfo->m_paramtableid) + "')";

		mysql_query(&my_connection, "SET NAMES UTF8"); //���ñ����ʽ
		string sql = ss.str();
		int res = mysql_query(&my_connection, sql.c_str());
		if (!res)
		{
			return true;
		}
		else
		{
			LOG(INFO) << "�������ʧ��!" << sql;
			return false;
		}
		return true;
	}

	/// <summary>
	/// �жϵ�ǰָ����Ƿ���ڣ���������д��
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::CommandtableIsExist(string cTableName)
	{
		bool isexist = false;
		for (auto item : m_allCommandtableI)
		{
			if (item.second->m_tableName == cTableName)
			{
				isexist = true;
				break;
			}
		}

		//����
		if (isexist)
		{
			return true;
		}

		//�����������ݿ������
		if (!connected())
		{
			LOG(INFO) << "�������ݿ�����";
			if (!connect())
				return false;
		}
		stringstream ss;
		ss << "insert into command_table_info(name) values ('";
		ss << boost::lexical_cast<string>(cTableName) + "')";

		mysql_query(&my_connection, "SET NAMES UTF8"); //���ñ����ʽ
		string sql = ss.str();
		int res = mysql_query(&my_connection, sql.c_str());
		if (!res)
		{
			int res1;
			mysql_query(&my_connection, "SET NAMES UTF8"); //���ñ����ʽ
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
					oneTable->m_tableName = cTableName;

					m_allCommandtableI.insert(pair<int, PCTable*>(oneTable->m_id, oneTable));
					return true;
				}
			}
		}
		else
		{
			LOG(INFO) << "����ָ���ʧ��!" << sql;
			return false;
		}
		return true;
	}


	/// <summary>
	/// д���µ�ָ�� 
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::InsertNewCommand(Command* oneCommand)
	{
		if (!connected())
		{
			LOG(INFO) << "�������ݿ�����";
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

		mysql_query(&my_connection, "SET NAMES UTF8"); //���ñ����ʽ
		string sql = ss.str();
		int res = mysql_query(&my_connection, sql.c_str());
		if (!res)
		{
			//ÿ�θ������ں�������
			if (GetCommandInfo())
				return true;
			else
				return false;
		}
		else
		{
			LOG(INFO) << "����ָ��ʧ��!" << sql;
			return false;
		}
		return true;
	}

	/// <summary>
	/// д�뵱ǰ��ָ��Ļ��� 
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::InsertNewCommandBack(Command* oneCommandBack)
	{
		if (!connected())
		{
			LOG(INFO) << "�������ݿ�����";
			if (!connect())
				return false;
		}
		stringstream ss;
		ss << "insert into command_info(rocket_id,back_id,name,code,type) values ('";
		ss << boost::lexical_cast<string>(oneCommandBack->m_iRocketId) + "','";
		ss << boost::lexical_cast<string>(oneCommandBack->m_iBackId) + "','";
		ss << boost::lexical_cast<string>(oneCommandBack->m_sName) + "','";
		ss << boost::lexical_cast<string>(oneCommandBack->m_iCode) + "','";
		ss << boost::lexical_cast<string>(oneCommandBack->m_iType) + "')";

		mysql_query(&my_connection, "SET NAMES UTF8"); //���ñ����ʽ
		string sql = ss.str();
		int res = mysql_query(&my_connection, sql.c_str());
		if (!res)
		{
			int res1;
			mysql_query(&my_connection, "SET NAMES UTF8"); //���ñ����ʽ
			res1 = mysql_query(&my_connection, "select @@IDENTITY");
			if (!res1)
			{
				MYSQL_RES* result = nullptr;
				MYSQL_ROW sql_row;
				result = mysql_store_result(&my_connection);
				if (result)
				{
					sql_row = mysql_fetch_row(result);
					oneCommandBack->m_id = atoi(sql_row[0]);//�õ������id ����дָ���ʱ��ʹ��   TODOע���ͷ�result
				}
			}

			//ÿ�θ������ں�������
			if (GetCommandInfo())
				return true;
			else
				return false;
		}
		else
		{
			LOG(INFO) << "����ָ�����ʧ��!" << sql;
			return false;
		}
		return true;
	}


	/// <summary>
	/// д��ָ���ָ���Ӧ��ϵ��
	/// </summary>
	/// <returns></returns>
	bool ImpotEXcelDAO::InsertCommandCommandtable(Command* oneCommand)
	{
		if (!connected())
		{
			LOG(INFO) << "�������ݿ�����";
			if (!connect())
				return false;
		}
		stringstream ss;
		ss << "insert into command_ commandtable_info(command_id,command_table_id) values ('";
		ss << boost::lexical_cast<string>(oneCommand->m_id) + "','";
		ss << boost::lexical_cast<string>(oneCommand->m_commandTableId) + "')";

		mysql_query(&my_connection, "SET NAMES UTF8"); //���ñ����ʽ
		string sql = ss.str();
		int res = mysql_query(&my_connection, sql.c_str());
		if (!res)
		{
			return true;
		}
		else
		{
			LOG(INFO) << "����ָ���ָ���Ӧ��ϵ��ʧ��!" << sql;
			return false;
		}
		return true;
	}

}


