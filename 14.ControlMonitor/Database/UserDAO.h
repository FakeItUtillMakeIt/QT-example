#pragma once

#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

#include "../AppCache.h"
#include "../../Public/log.h"
#include "../../Model/User.h"
#include "mysql.h" 

namespace DataBase
{
	class UserDAO
	{
	public:
		UserDAO(OutputPath* path);
		~UserDAO();
		bool exec_sql(string sql);
		bool connected();
		bool connect();
		bool test();
		bool getUser();
		int queryMysql(std::string sql_str, unordered_map<int, vector<string>>& contents);
	private:
		OutputPath* m_path;
		MYSQL my_connection;
		bool is_connected;
		AppCache* m_app;
	};
}
