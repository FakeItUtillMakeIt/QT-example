#pragma once
#include "../AppCache.h"
#include "../../Public/log.h"
#include "mysql.h" 



namespace DataBase
{
	class mainFlowDao
	{
	public:
		mainFlowDao(OutputPath* path);
		~mainFlowDao();
		bool exec_sql(string sql);
		bool connected();
		bool connect();
		bool test();
		bool getMainflow();

	private:
		OutputPath* m_path;
		MYSQL my_connection;
		bool is_connected;
		AppCache* m_app;
	};
}
