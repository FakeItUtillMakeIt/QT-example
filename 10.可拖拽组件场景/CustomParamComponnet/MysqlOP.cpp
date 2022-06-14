#include "MysqlOP.h"

using namespace MYSQL_OP;

bool MysqlOP::connectMysql(const char* host, const char* user, const char* password, const char* db_name, unsigned int port) {
	mysql_init(&mysql_connection);
	//获取数据库句柄
	auto ret=mysql_real_connect(&mysql_connection,host,user,password,db_name,port, NULL, NULL);
	if (!ret)
	{
		qDebug() << QString::fromLocal8Bit("连接失败：") << mysql_error(&mysql_connection) ;

		return false;
	}
		return true;

}

/**
    @brief  获取数据
    @param  sql_str - 
    @retval         - 
**/
int MysqlOP::queryMysql(std::string sql_str, map<int, vector<string>>& contents) {


	mysql_query(&mysql_connection, "set names utf8");

	int ret = mysql_query(&mysql_connection, sql_str.c_str());
	if (ret)
	{
		qDebug() << "无法读取数据：" << mysql_error(&mysql_connection) ;
	}
	MYSQL_RES* res;
	MYSQL_ROW row;
	unsigned int num_feilds;
	unsigned int num_rows;

	res = mysql_store_result(&mysql_connection);
	if (res) {
		num_feilds = mysql_num_fields(res);
	}
	else
	{
		if (mysql_field_count(&mysql_connection) == 0) {
			num_rows = mysql_affected_rows(&mysql_connection);
		}
		else
		{
			qDebug() << "error:" << mysql_error(&mysql_connection);
			return 0;
		}
		return 0;
	}

	int counts = 0;
	while ((row=mysql_fetch_row(res)))
	{
		unsigned long* lengths;
		lengths = mysql_fetch_lengths(res);
		for (int i=1;i<num_feilds;i++)
		{
			qDebug() << (int)lengths[i] <<( row[i] ? row[i] : "NULL " );
			contents[int(row[0])].push_back(string(row[i]));
		}
		counts++;
	}
	
	return counts;
}


void MysqlOP::closeMysqlConnct() {

	mysql_close(&mysql_connection);
}