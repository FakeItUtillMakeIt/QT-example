#pragma once

#ifndef MYSQL_OP_H
#define MYSQL_OP_H


#include <string>
#include <mysql.h>
#include <vector>
#include <map>
#include <QDebug>


using namespace std;
/**
    @namespace MYSQL_OP
    @brief     mysql���ݿ������
**/
namespace MYSQL_OP {
	class MysqlOP
	{

	private:
		MysqlOP();
		~MysqlOP();

	private:
		MYSQL mysql_connection;
		static MysqlOP* instance;

	public:
		static MysqlOP* getInstance();

	public:
		/*����*/
		bool connectMysql(const char* host,const char* user,const char* password,const char* db_name,unsigned int port);
		/*��*/
		int queryMysql(string sql_str,map<int,vector<string>>& contents);

		bool writeMysql(string sql_str);

		/*�ر����ݿ�*/
		void closeMysqlConnct();

	};

}


#endif