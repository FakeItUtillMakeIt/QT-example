#include "FlowInfoConfig2DB.h"

FlowInfoConfig2DB* FlowInfoConfig2DB::instance = nullptr;


FlowInfoConfig2DB::FlowInfoConfig2DB() {

	//����APPcache�е�������Ϣ�������ݿ��ʼ��
	flowInfoDBOp = new DataBase::UserDAO(AppCache::instance()->m_outputPath);

	if (flowInfoDBOp->connect())
	{
		qDebug() << "�������ݿ�ʧ�ܣ�������ݿ�����<<FlowDBConfigInfo";

	}

}
/**
	@brief ��ȡָ���
**/
void FlowInfoConfig2DB::readCommandDB2FlowEdit() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`command_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	commandInfo.clear();
	flowInfoDBOp->queryMysql(sqlString, commandInfo);

}

/**
	@brief ��ȡ�����Ϣ��
**/
void FlowInfoConfig2DB::readRocketDB2FlowEdit() {

	QString qSqlString = "SELECT * FROM `simulatedtraining`.`rocket_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	rocketInfo.clear();
	flowInfoDBOp->queryMysql(sqlString, rocketInfo);

}

/**
	@brief ��ȡ������
**/
void FlowInfoConfig2DB::readMainFlowDB2FlowEdit() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`main_flow_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	mainFlowInfo.clear();
	flowInfoDBOp->queryMysql(sqlString, mainFlowInfo);

}


/**
	@brief ��ȡ������
**/
void FlowInfoConfig2DB::readSubFlowDB2FlowEdit() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`sub_flow_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	subFlowInfo.clear();
	flowInfoDBOp->queryMysql(sqlString, subFlowInfo);

}

/**
	@brief ���ϲ�ѯ���� ��ѯ������������������mainID��Ӧʱ�����̶�Ӧ��ָ�
	���������ֱ��Ӧ index mainID commandID
**/
void FlowInfoConfig2DB::readUnionSearchDB2FlowEdit(QString RocketID) {
	QString qSqlString = "SELECT a.`index` as 'index',b.id as 'mainID',b.command_id,a.id from sub_flow_info b left join main_flow_info a on main_id=a.id WHERE a.rocket_id=%1;";
	qSqlString = qSqlString.arg(RocketID);
	string sqlString = qSqlString.toStdString();
	unionSearchInfo.clear();
	flowInfoDBOp->unionQueryMysql(sqlString, unionSearchInfo);
}

/**
	@brief �Զ����ѯ���
	@param qSqlString -
**/
bool FlowInfoConfig2DB::customDBQuery(QString qSqlString) {
	customSearchInfo.clear();

	string sqlString = qSqlString.toStdString();
	return flowInfoDBOp->queryMysql(sqlString, customSearchInfo);

}

/**
	@brief �Զ����ѯ���
	@param qSqlString -
**/
bool FlowInfoConfig2DB::customDBQuery1(QString qSqlString) {
	customSearchInfo1.clear();

	string sqlString = qSqlString.toStdString();
	return flowInfoDBOp->queryMysql(sqlString, customSearchInfo1);

}

/**
	@brief �洢��������Ϣ
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
		qDebug() << "��main_flow_info���ݿ�д��ʧ��!!!";
	}

}

/**
	@brief �洢��������Ϣ
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
		qDebug() << "��sub_flow_info���ݿ�д��ʧ��!!!";
	}

}


void FlowInfoConfig2DB::clearMainFlowDB(int rocketID) {
	readMainFlowDB2FlowEdit();

	QString qSqlString = "DELETE FROM `simulatedtraining`.`main_flow_info` WHERE `rocket_id` = %1;";
	qSqlString = qSqlString.arg(rocketID);
	auto ret = flowInfoDBOp->exec_sql(qSqlString.toStdString());
	if (!ret)
	{
		qDebug() << "��sub_flow_info���ݿ�д��ʧ��!!!";
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
			qDebug() << "��sub_flow_info���ݿ�д��ʧ��!!!";
		}
	}


}