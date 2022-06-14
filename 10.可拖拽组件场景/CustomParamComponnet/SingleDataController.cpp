#include "SingleDataController.h"

SingleDataController* SingleDataController::instance = nullptr;

SingleDataController::SingleDataController() {

}

map<int, vector<string>> SingleDataController::getDataBaseParamInfo() {
	/*获取数据库参数信息*/
	MYSQL_OP::MysqlOP* sqlOP = new MYSQL_OP::MysqlOP;
	bool ret = sqlOP->connectMysql("127.0.0.1", "root", "123456", "simulatedtraining", 3306);

	map<int, vector<string>> contents;
	if (ret) {
		sqlOP->queryMysql("SELECT * FROM `simulatedtraining`.`param_info` LIMIT 0, 1000; ", contents);
	}
	sqlOP->closeMysqlConnct();

	return contents;
}