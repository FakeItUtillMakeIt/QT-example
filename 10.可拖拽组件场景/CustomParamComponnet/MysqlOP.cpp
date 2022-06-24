#include "MysqlOP.h"

using namespace MYSQL_OP;

MysqlOP* MysqlOP::instance = nullptr;

MysqlOP::MysqlOP() {

}

MysqlOP::~MysqlOP() {

}

MysqlOP* MysqlOP::getInstance() {
	if (instance==nullptr)
	{
		instance = new MysqlOP();
	}
	return instance;

}

/**
    @brief  �������ݿ⣬�κ�ʱ��Ӧ��ֻ��һ������
    @param  host     ����
    @param  user     �û� 
    @param  password ����
    @param  db_name  ���ݿ���
    @param  port     �˿� 
    @retval          �Ƿ����ӳɹ� 
**/
bool MysqlOP::connectMysql(const char* host, const char* user, const char* password, const char* db_name, unsigned int port) {
	mysql_init(&mysql_connection);
	//��ȡ���ݿ���
	auto ret=mysql_real_connect(&mysql_connection,host,user,password,db_name,port, NULL, NULL);
	if (!ret)
	{
		qDebug() << QString::fromLocal8Bit("����ʧ�ܣ�") << mysql_error(&mysql_connection) ;

		return false;
	}
		return true;

}

/**
    @brief  ��ȡ����
    @param  sql_str - 
    @retval         - 
**/
int MysqlOP::queryMysql(std::string sql_str, map<int, vector<string>>& contents) {

	mysql_query(&mysql_connection, "set names utf8");

	int ret = mysql_query(&mysql_connection, sql_str.c_str());
	if (ret)
	{
		qDebug() << "�޷���ȡ���ݣ�" << mysql_error(&mysql_connection) ;
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
			contents[(int)(row[0])].push_back(string(row[i]));
		}
		counts++;
	}
	
	return counts;
}

bool MysqlOP::writeMysql(string sql_str) {

	mysql_query(&mysql_connection, "set names utf8");

	int ret = mysql_query(&mysql_connection, sql_str.c_str());
	if (ret)
	{
		qDebug() << "�޷�д�����ݣ�" << mysql_error(&mysql_connection);
		return false;
	}
	return true;
}

/**
    @brief �ر����ݿ�
**/
void MysqlOP::closeMysqlConnct() {

	mysql_close(&mysql_connection);
}