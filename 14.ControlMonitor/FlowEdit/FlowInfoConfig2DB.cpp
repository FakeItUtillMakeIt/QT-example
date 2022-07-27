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
void FlowInfoConfig2DB::subFlowConfigOp2DB(int mainFlowID, int emitCmdID, QString subFlowName, QString otherInfo) {
	QString qSqlString = "INSERT INTO `simulatedtraining`.`sub_flow_info`(`main_id`, `command_id`, `name`, `remark`)  VALUES (%1, %2, '%3', '%4');";
	qSqlString = qSqlString.arg(mainFlowID).arg(emitCmdID).arg(subFlowName).arg(otherInfo);
	string sqlString = qSqlString.toStdString();
	bool opRet = flowInfoDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "往sub_flow_info数据库写入失败!!!";
	}

}
