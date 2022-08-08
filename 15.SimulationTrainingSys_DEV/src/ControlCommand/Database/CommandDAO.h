#pragma once

#ifndef COMMAND_DAO_H
#define COMMAND_DAO_H


#include <string>
#include <unordered_map>
using namespace std;

#include "../AppCache.h"
#include "../../Public/log.h"
#include "../../Public/Utils.h" 
#include "../../Model/Command.h"
#include "mysql.h" 

namespace DataBase
{
	class CommandDAO
	{
	public:
		CommandDAO(OutputPath* path);
		~CommandDAO();
		bool exec_sql(string sql);
		bool connected();
		bool connect();
		bool test();
		bool getCommand(); 
		bool getCommandParam(); 
		bool getFaultCommand();
	private:
		OutputPath* m_path;
		MYSQL my_connection;
		bool is_connected;
		AppCache* m_app;
	};
}


#endif