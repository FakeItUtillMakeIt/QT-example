#include "FlowInfoConfig2DB.h"

FlowInfoConfig2DB* FlowInfoConfig2DB::instance = nullptr;


FlowInfoConfig2DB::FlowInfoConfig2DB() {

	//根据APPcache中的配置信息进行数据库初始化
	flowInfoDBOp = new DataBase::UserDAO(AppCache::instance()->m_outputPath);

	if (flowInfoDBOp->connect())
	{
		qDebug() << "连接数据库失败，检查数据库设置<<FlowDBConfigInfo";

	}

}
/**
	@brief 读取指令表
**/
void FlowInfoConfig2DB::readCommandDB2FlowEdit() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`command_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	commandInfo.clear();
	flowInfoDBOp->queryMysql(sqlString, commandInfo);

}

/**
	@brief 读取火箭信息表
**/
void FlowInfoConfig2DB::readRocketDB2FlowEdit() {

	QString qSqlString = "SELECT * FROM `simulatedtraining`.`rocket_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	rocketInfo.clear();
	flowInfoDBOp->queryMysql(sqlString, rocketInfo);

}

/**
	@brief 读取主流程
**/
void FlowInfoConfig2DB::readMainFlowDB2FlowEdit() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`main_flow_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	mainFlowInfo.clear();
	flowInfoDBOp->queryMysql(sqlString, mainFlowInfo);

}


/**
	@brief 读取子流程
**/
void FlowInfoConfig2DB::readSubFlowDB2FlowEdit() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`sub_flow_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	subFlowInfo.clear();
	flowInfoDBOp->queryMysql(sqlString, subFlowInfo);

}

/**
	@brief 联合查询函数 查询主流程索引和子流程mainID对应时主流程对应的指令集
	三个参数分别对应 index mainID commandID
**/
void FlowInfoConfig2DB::readUnionSearchDB2FlowEdit(QString RocketID) {
	QString qSqlString = "SELECT a.`index` as 'index',b.id as 'mainID',b.command_id,a.id from sub_flow_info b left join main_flow_info a on main_id=a.id WHERE a.rocket_id=%1;";
	qSqlString = qSqlString.arg(RocketID);
	string sqlString = qSqlString.toStdString();
	unionSearchInfo.clear();
	flowInfoDBOp->unionQueryMysql(sqlString, unionSearchInfo);
}

/**
	@brief 自定义查询语句
	@param qSqlString -
**/
bool FlowInfoConfig2DB::customDBQuery(QString qSqlString) {
	customSearchInfo.clear();

	string sqlString = qSqlString.toStdString();
	return flowInfoDBOp->queryMysql(sqlString, customSearchInfo);

}

/**
	@brief 自定义查询语句
	@param qSqlString -
**/
bool FlowInfoConfig2DB::customDBQuery1(QString qSqlString) {
	customSearchInfo1.clear();

	string sqlString = qSqlString.toStdString();
	return flowInfoDBOp->queryMysql(sqlString, customSearchInfo1);

}

/**
	@brief 存储主流程信息
	@param rocketTypeID  -
	@param mainFlowName  -
	@param mainFlowIndex -
	@param backCmdInfo   -
	@param otherInfo     -
**/
void FlowInfoConfig2DB::mainFlowConfigOp2DB(int rocketTypeID, QString mainFlowName, QString mainFlowIndex, QString backCmdInfo, QString otherInfo) {
	QString qSqlString = "INSERT INTO `simulatedtraining`.`main_flow_info`(`rocket_id`, `name`, `index`, `backInfo`, `remark`) VALUES (%1, '%2', '%3', '%4', '%5');";
	qSqlString = qSqlString.arg(rocketTypeID).arg(mainFlowName).arg(mainFlowIndex).arg(backCmdInfo).arg(otherInfo);
	string sqlString = qSqlString.toStdString();
	bool opRet = flowInfoDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "往main_flow_info数据库写入失败!!!";
	}

}

/**
	@brief 存储子流程信息
	@param mainFlowID  -
	@param emitCmdID   -
	@param subFlowName -
	@param otherInfo   -
**/
void FlowInfoConfig2DB::subFlowConfigOp2DB(int mainFlowID, int emitCmdID, QString subFlowName, QString subBackInfo, QString otherInfo) {
	QString qSqlString = "INSERT INTO `simulatedtraining`.`sub_flow_info`(`main_id`, `command_id`, `name`, `backInfo`, `remark`)  VALUES (%1, %2, '%3', '%4','%5');";
	qSqlString = qSqlString.arg(mainFlowID).arg(emitCmdID).arg(subFlowName).arg(subBackInfo).arg(otherInfo);
	string sqlString = qSqlString.toStdString();
	bool opRet = flowInfoDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "往sub_flow_info数据库写入失败!!!";
	}

}


void FlowInfoConfig2DB::clearMainFlowDB(int rocketID) {
	readMainFlowDB2FlowEdit();

	QString qSqlString = "DELETE FROM `simulatedtraining`.`main_flow_info` WHERE `rocket_id` = %1;";
	qSqlString = qSqlString.arg(rocketID);
	auto ret = flowInfoDBOp->exec_sql(qSqlString.toStdString());
	if (!ret)
	{
		qDebug() << "往sub_flow_info数据库写入失败!!!";
	}



}

void FlowInfoConfig2DB::clearSubFlowDB(vector<int> v) {
	//readSubFlowDB2FlowEdit();

	for (auto ele : v)
	{
		QString qSqlString = "DELETE FROM `simulatedtraining`.`sub_flow_info` WHERE `id` = %1;";
		qSqlString = qSqlString.arg(ele);
		auto ret = flowInfoDBOp->exec_sql(qSqlString.toStdString());
		if (!ret)
		{
			qDebug() << "往sub_flow_info数据库写入失败!!!";
		}
	}


}