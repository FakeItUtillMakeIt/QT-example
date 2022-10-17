#pragma execution_character_set("utf-8")
#include"mainFlowDao.h"

namespace DataBase
{
	mainFlowDao::mainFlowDao(OutputPath* path)
		: m_path(path)
		, is_connected(false)
	{
		m_app = AppCache::instance();
	}

	mainFlowDao::~mainFlowDao()
	{
		if (is_connected)
		{
			mysql_close(&my_connection);//�Ͽ�����  
			is_connected = false;
		}
	}

	bool mainFlowDao::connect()
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
	bool mainFlowDao::connected()
	{
		return is_connected;
	}

	bool mainFlowDao::getMainflow(int id)
	{

		if (!connected())
		{
			LOG(INFO) << "�������ݿ�����";
			if (!connect())
				return false;
		}
		MYSQL_RES* result = nullptr;
		MYSQL_ROW sql_row;
		string sql;
		string sql2 = to_string(id);
		string sql3 = " ORDER BY `index`;";
		sql.append("SELECT `name`,`index` FROM main_flow_info where rocket_id=").append(sql2);
		int res = 0;

		mysql_query(&my_connection, "SET NAMES UTF8"); //���ñ����ʽ
		res = mysql_query(&my_connection, sql.c_str());//��ѯ

		if (!res)
		{

			result = mysql_store_result(&my_connection);
			if (result)
			{

				while (sql_row = mysql_fetch_row(result))
				{

					QString str = QString(sql_row[0]);
					m_app->mainflowlist.append(str);

				}
			}
			else
				LOG(INFO) << "��ȡ����ʧ��";
		}
		else
			LOG(INFO) << "��ȡ����ʧ��";
		if (result)
			mysql_free_result(result);//�ͷŽ����Դ  
		return true;
	}

}