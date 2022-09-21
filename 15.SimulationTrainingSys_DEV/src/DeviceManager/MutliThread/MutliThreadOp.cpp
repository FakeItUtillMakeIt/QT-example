
#include "MutliThreadOp.h"



WorkThread::WorkThread(QObject* parent) {

}

WorkThread::~WorkThread() {


}


void WorkThread::doWork() {

	//把设备的状态及相应参数的配置进行初始化
	QString initStat = QString("断电");
	for (Device* eachDev : AppCache::instance()->m_allDevice)
	{
		eachDev->m_sCurStatus = initStat.toStdString();
	}

	DeviceDBConfigInfo::getInstance()->readStatusInfoDB2UI();
	QString statusFilePath;
	auto curPath = QCoreApplication::applicationDirPath();
	statusFilePath = curPath + "/device/";
	//设备参数初始化  四个需要初始化的值
	for (auto ele = AppCache::instance()->m_allDeviceParam.begin(); ele != AppCache::instance()->m_allDeviceParam.end(); ele++)
	{
		//初始化状态不需要读取实时值列表
		/*在这里启动定时器刷数*/

		for (auto eachStat : AppCache::instance()->m_dev2DeviceStatusID[ele->second->m_deviceId])
		{
			QFile file(statusFilePath + QString::fromStdString(AppCache::instance()->m_allDeviceStatus[eachStat]->m_dataPath));
			auto ret = file.open(QIODevice::ReadOnly | QIODevice::Text);
			if (!ret)
			{
				continue;
			}
			int row = 1;
			QTextStream in(&file);
			in.setCodec("UTF-8");
			QStringList paramNameList;

			unordered_map<string, vector<double>> tempData;

			while (!in.atEnd())
			{
				QString lineData = in.readLine();
				if (row == 1)
				{
					paramNameList = lineData.split(QRegExp("[,]"));
					paramNameList.pop_back();
					row--;
					continue;
				}
				auto lineDataList = lineData.split(QRegExp("[,]"));
				lineDataList.pop_back();
				//舍弃数据不足的行
				if (lineDataList.size() < paramNameList.size())
				{
					continue;
				}
				for (int i = 0; i < paramNameList.size(); i++)
				{
					tempData[Utils::UTF8ToGBK(paramNameList[i].toStdString().c_str())].push_back(lineDataList[i].toDouble());
				}
			}
			//状态id->状态名称
			ele->second->m_statusRealData[AppCache::instance()->m_allDeviceStatus[eachStat]->m_statusName] = tempData[ele->second->m_subParameterName];
		}
	}

	emit workFinished();
}

DeleteDBDataThread::DeleteDBDataThread(QObject* parent) {
	//根据APPcache中的配置信息进行数据库初始化
	deviceManageDBOp = new DataBase::DeviceDAO(AppCache::instance()->m_outputPath);

	if (deviceManageDBOp->connect())
	{
		qDebug() << "连接数据库失败，检查数据库设置<<DeviceDBConfigInfo";

	}
}

DeleteDBDataThread::~DeleteDBDataThread() {

}

/**
	@brief	  自定义执行语句
	@param qSqlString -
**/
void DeleteDBDataThread::customRunSql(QString qSqlString) {
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "执行语句失败!!!";
	}
}

/**
	@brief 自定义读取
	@param qSqlString -
**/
void DeleteDBDataThread::customReadTableInfo(QString qSqlString) {
	string sqlString = qSqlString.toStdString();
	customReadInfoMap.clear();
	bool opRet = deviceManageDBOp->queryMysql(sqlString, customReadInfoMap);
	if (!opRet)
	{
		qDebug() << "获取自定义数据失败!!!";

	}
}

void DeleteDBDataThread::setRocketID(int rocketID) {
	curRocketID = rocketID;
}

void DeleteDBDataThread::deleteRocketInfo() {
	//删除和型号相关数据
	QString qSqlString;

	//参数信息
	qSqlString = QString("SELECT\
		parameter_rocket_info.parameter_id,\
		parameter_rocket_info.createTime,\
		parameter_rocket_info.lastUpdateTime\
		FROM\
		parameter_rocket_info\
		WHERE\
		parameter_rocket_info.rocket_id = %1").arg(curRocketID);
	customReadTableInfo(qSqlString);
	auto releParamInfo = customReadInfoMap;
	for (auto ele : releParamInfo)
	{
		int curParamID = ele.first;
		QString qSqlString = QString("SELECT\
		device_param_info.id,\
		device_param_info.createTime,\
		device_param_info.lastUpdateTime\
		FROM\
		device_param_info\
		WHERE\
		device_param_info.parameter_id = %1").arg(curParamID);
		customReadTableInfo(qSqlString);
		auto releDataInfo = customReadInfoMap;
		for (auto ele1 : releDataInfo)
		{
			customRunSql(QString("DELETE FROM simulatedtraining.rockect_param_info WHERE device_parameter_id=%1").arg(ele1.first));
		}
		//设备参数
		customRunSql(QString("DELETE FROM simulatedtraining.device_param_info WHERE parameter_id=%1").arg(curParamID));
		//参数型号
		customRunSql(QString("DELETE FROM simulatedtraining.parameter_rocket_info WHERE parameter_id=%1").arg(curParamID));
		//参数信息
		customRunSql(QString("DELETE FROM simulatedtraining.parameter_info WHERE id=%1").arg(curParamID));
	}
	//参数型号信息
	customRunSql(QString("DELETE FROM simulatedtraining.param_table_info WHERE rocket_id=%1").arg(curRocketID));

	//参数型号信息
	//递归删除和设备相关的所有信息
	qSqlString = QString("SELECT\
		device_info.id,\
		device_info.createTime,\
		device_info.lastUpdateTime\
		FROM\
		device_info\
		WHERE\
		device_info.rocket_id = %1").arg(curRocketID);
	customReadTableInfo(qSqlString);
	auto releDeviceInfo = customReadInfoMap;
	for (auto ele : releDeviceInfo)
	{
		int curDeviceID = ele.first;
		//设备关联的设备状态信息
		qSqlString = QString("SELECT\
		device_status_info.id,\
		device_status_info.createTime,\
		device_status_info.lastUpdateTime\
		FROM\
		device_status_info\
		WHERE\
		device_status_info.device_id = %1").arg(curDeviceID);
		customReadTableInfo(qSqlString);
		auto releDataInfo = customReadInfoMap;
		for (auto ele1 : releDataInfo)
		{
			customRunSql(QString("DELETE FROM simulatedtraining.command_devicestatus_info WHERE deviceStatus_id=%1").arg(ele1.first));
		}
		//箭上参数
		qSqlString = QString("SELECT\
		device_param_info.id,\
		device_param_info.createTime,\
		device_param_info.lastUpdateTime\
		FROM\
		device_param_info\
		WHERE\
		device_param_info.device_id = %1").arg(curDeviceID);
		customReadTableInfo(qSqlString);
		releDataInfo = customReadInfoMap;
		for (auto ele2 : releDataInfo)
		{
			customRunSql(QString("DELETE FROM simulatedtraining.rockect_param_info WHERE device_parameter_id=%1").arg(ele2.first));
		}
		//设备参数
		customRunSql(QString("DELETE FROM simulatedtraining.device_param_info WHERE device_id=%1").arg(curDeviceID));
		//设备状态
		customRunSql(QString("DELETE FROM simulatedtraining.device_status_info WHERE device_id=%1").arg(curDeviceID));
	}
	//设备信息
	customRunSql(QString("DELETE FROM simulatedtraining.device_info WHERE rocket_id=%1").arg(curRocketID));

	//递归删除和指令相关的所有信息
	qSqlString = QString("SELECT\
		command_info.id,\
		command_info.createTime,\
		command_info.lastUpdateTime\
		FROM\
		command_info\
		WHERE\
		command_info.rocket_id = %1").arg(curRocketID);
	customReadTableInfo(qSqlString);
	auto releCmdInfo = customReadInfoMap;
	for (auto ele : releCmdInfo)
	{
		int curCmdID = ele.first;
		//子流程信息
		customRunSql(QString("DELETE FROM simulatedtraining.sub_flow_info WHERE command_id=%1").arg(curCmdID));
		//指令参数
		customRunSql(QString("DELETE FROM simulatedtraining.command_param_info WHERE command_id=%1").arg(curCmdID));
		//指令设备状态
		customRunSql(QString("DELETE FROM simulatedtraining.command_devicestatus_info WHERE command_id=%1").arg(curCmdID));
		//指令表信息
		customRunSql(QString("DELETE FROM simulatedtraining.command_commandtable_info WHERE command_id=%1").arg(curCmdID));
		//指令信息
		customRunSql(QString("DELETE FROM simulatedtraining.command_info WHERE id=%1").arg(curCmdID));
	}
	//指令信息
	customRunSql(QString("DELETE FROM simulatedtraining.command_table_info WHERE rocket_id=%1").arg(curRocketID));
	//箭上参数
	qSqlString = QString("SELECT\
		rocket_data_info.id,\
		rocket_data_info.createTime,\
		rocket_data_info.lastUpdateTime\
		FROM\
		rocket_data_info\
		WHERE\
		rocket_data_info.rocket_id = %1").arg(curRocketID);
	customReadTableInfo(qSqlString);
	auto releRocketDataInfo = customReadInfoMap;
	for (auto ele : releRocketDataInfo)
	{
		customRunSql(QString("DELETE FROM simulatedtraining.rockect_param_info WHERE rocket_data_id=%1").arg(ele.first));
	}
	//箭上数据
	customRunSql(QString("DELETE FROM simulatedtraining.rocket_data_info WHERE rocket_id=%1").arg(curRocketID));
	//子流程信息
	qSqlString = QString("SELECT\
		main_flow_info.id,\
		main_flow_info.createTime,\
		main_flow_info.lastUpdateTime\
		FROM\
		main_flow_info\
		WHERE\
		main_flow_info.rocket_id = %1").arg(curRocketID);
	customReadTableInfo(qSqlString);
	auto releMainFlowInfo = customReadInfoMap;
	for (auto ele : releMainFlowInfo)
	{
		customRunSql(QString("DELETE FROM simulatedtraining.sub_flow_info WHERE main_id=%1").arg(ele.first));
	}
	//主流程信息
	customRunSql(QString("DELETE FROM simulatedtraining.main_flow_info WHERE rocket_id=%1").arg(curRocketID));
	//故障指令信息
	customRunSql(QString("DELETE FROM simulatedtraining.fault_command_info WHERE rocket_id=%1").arg(curRocketID));
	// 故障参数信息
	customRunSql(QString("DELETE FROM simulatedtraining.fault_param_info WHERE rocket_id=%1").arg(curRocketID));
	//型号岗位信息
	customRunSql(QString("DELETE FROM simulatedtraining.rocket_task_info WHERE rocket_id=%1").arg(curRocketID));
	//型号信息
	customRunSql(QString("DELETE FROM simulatedtraining.rocket_info WHERE id=%1").arg(curRocketID));

	emit workFinished();
}

void DeleteDBDataThread::deleteDeviceInfo() {
	//删除相关数据
//设备关联的设备状态信息
	QString qSqlString = QString("SELECT\
		device_status_info.id,\
		device_status_info.createTime,\
		device_status_info.lastUpdateTime\
		FROM\
		device_status_info\
		WHERE\
		device_status_info.device_id = %1").arg(curDeviceID);
	customReadTableInfo(qSqlString);
	auto releDataInfo = customReadInfoMap;
	for (auto ele : releDataInfo)
	{
		customRunSql(QString("DELETE FROM simulatedtraining.command_devicestatus_info WHERE deviceStatus_id=%1").arg(ele.first));
	}
	//箭上参数
	qSqlString = QString("SELECT\
		device_param_info.id,\
		device_param_info.createTime,\
		device_param_info.lastUpdateTime\
		FROM\
		device_param_info\
		WHERE\
		device_param_info.device_id = %1").arg(curDeviceID);
	customReadTableInfo(qSqlString);
	releDataInfo = customReadInfoMap;
	for (auto ele : releDataInfo)
	{
		customRunSql(QString("DELETE FROM simulatedtraining.rockect_param_info WHERE device_parameter_id=%1").arg(ele.first));
	}
	//设备参数
	customRunSql(QString("DELETE FROM simulatedtraining.device_param_info WHERE device_id=%1").arg(curDeviceID));
	//设备状态
	customRunSql(QString("DELETE FROM simulatedtraining.device_status_info WHERE device_id=%1").arg(curDeviceID));
	//设备信息
	customRunSql(QString("DELETE FROM simulatedtraining.device_info WHERE id=%1").arg(curDeviceID));

}

void DeleteDBDataThread::deleteCmdInfo() {
	//删除相关数据
	//子流程信息
	customRunSql(QString("DELETE FROM simulatedtraining.sub_flow_info WHERE command_id=%1").arg(curCmdID));
	//指令参数
	customRunSql(QString("DELETE FROM simulatedtraining.command_param_info WHERE command_id=%1").arg(curCmdID));
	//指令设备状态
	customRunSql(QString("DELETE FROM simulatedtraining.command_devicestatus_info WHERE command_id=%1").arg(curCmdID));
	//指令表信息
	customRunSql(QString("DELETE FROM simulatedtraining.command_commandtable_info WHERE command_id=%1").arg(curCmdID));
	//指令信息
	customRunSql(QString("DELETE FROM simulatedtraining.command_info WHERE id=%1").arg(curCmdID));
}

void DeleteDBDataThread::deleteParamInfo() {
	//删除相关数据
	//型号参数
	QString qSqlString = QString("SELECT\
		device_param_info.id,\
		device_param_info.createTime,\
		device_param_info.lastUpdateTime\
		FROM\
		device_param_info\
		WHERE\
		device_param_info.parameter_id = %1").arg(curParamID);
	customReadTableInfo(qSqlString);
	auto releDataInfo = DeviceDBConfigInfo::getInstance()->customReadInfoMap;
	for (auto ele : releDataInfo)
	{
		customRunSql(QString("DELETE FROM simulatedtraining.rockect_param_info WHERE device_parameter_id=%1").arg(ele.first));
	}
	//设备参数
	customRunSql(QString("DELETE FROM simulatedtraining.device_param_info WHERE parameter_id=%1").arg(curParamID));
	//参数型号
	customRunSql(QString("DELETE FROM simulatedtraining.parameter_rocket_info WHERE parameter_id=%1").arg(curParamID));
	//参数信息
	customRunSql(QString("DELETE FROM simulatedtraining.parameter_info WHERE id=%1").arg(curParamID));
}



void DeleteDBDataThread1::setRocketID(int rocketID) {
	curRocketID = rocketID;
}

void DeleteDBDataThread1::run() {
	//删除和型号相关数据
	QString qSqlString;

	//参数信息
	qSqlString = QString("SELECT\
		parameter_rocket_info.parameter_id,\
		parameter_rocket_info.createTime,\
		parameter_rocket_info.lastUpdateTime\
		FROM\
		parameter_rocket_info\
		WHERE\
		parameter_rocket_info.rocket_id = %1").arg(curRocketID);
	auto deleteOp = DeviceDBConfigInfo::getInstance();
	deleteOp->customReadTableInfo(qSqlString);
	auto releParamInfo = deleteOp->customReadInfoMap;
	for (auto ele : releParamInfo)
	{
		int curParamID = ele.first;
		QString qSqlString = QString("SELECT\
		device_param_info.id,\
		device_param_info.createTime,\
		device_param_info.lastUpdateTime\
		FROM\
		device_param_info\
		WHERE\
		device_param_info.parameter_id = %1").arg(curParamID);
		deleteOp->customReadTableInfo(qSqlString);
		auto releDataInfo = deleteOp->customReadInfoMap;
		for (auto ele1 : releDataInfo)
		{
			deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.rockect_param_info WHERE device_parameter_id=%1").arg(ele1.first));
		}
		//设备参数
		deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.device_param_info WHERE parameter_id=%1").arg(curParamID));
		//参数型号
		deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.parameter_rocket_info WHERE parameter_id=%1").arg(curParamID));
		//参数信息
		deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.parameter_info WHERE id=%1").arg(curParamID));
	}
	//参数型号信息
	deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.param_table_info WHERE rocket_id=%1").arg(curRocketID));

	//参数型号信息
	//递归删除和设备相关的所有信息
	qSqlString = QString("SELECT\
		device_info.id,\
		device_info.createTime,\
		device_info.lastUpdateTime\
		FROM\
		device_info\
		WHERE\
		device_info.rocket_id = %1").arg(curRocketID);
	deleteOp->customReadTableInfo(qSqlString);
	auto releDeviceInfo = deleteOp->customReadInfoMap;
	for (auto ele : releDeviceInfo)
	{
		int curDeviceID = ele.first;
		//设备关联的设备状态信息
		qSqlString = QString("SELECT\
		device_status_info.id,\
		device_status_info.createTime,\
		device_status_info.lastUpdateTime\
		FROM\
		device_status_info\
		WHERE\
		device_status_info.device_id = %1").arg(curDeviceID);
		deleteOp->customReadTableInfo(qSqlString);
		auto releDataInfo = deleteOp->customReadInfoMap;
		for (auto ele1 : releDataInfo)
		{
			deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.command_devicestatus_info WHERE deviceStatus_id=%1").arg(ele1.first));
		}
		//箭上参数
		qSqlString = QString("SELECT\
		device_param_info.id,\
		device_param_info.createTime,\
		device_param_info.lastUpdateTime\
		FROM\
		device_param_info\
		WHERE\
		device_param_info.device_id = %1").arg(curDeviceID);
		deleteOp->customReadTableInfo(qSqlString);
		releDataInfo = deleteOp->customReadInfoMap;
		for (auto ele2 : releDataInfo)
		{
			deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.rockect_param_info WHERE device_parameter_id=%1").arg(ele2.first));
		}
		//设备参数
		deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.device_param_info WHERE device_id=%1").arg(curDeviceID));
		//设备状态
		deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.device_status_info WHERE device_id=%1").arg(curDeviceID));
	}
	//设备信息
	deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.device_info WHERE rocket_id=%1").arg(curRocketID));

	//递归删除和指令相关的所有信息
	qSqlString = QString("SELECT\
		command_info.id,\
		command_info.createTime,\
		command_info.lastUpdateTime\
		FROM\
		command_info\
		WHERE\
		command_info.rocket_id = %1").arg(curRocketID);
	deleteOp->customReadTableInfo(qSqlString);
	auto releCmdInfo = deleteOp->customReadInfoMap;
	for (auto ele : releCmdInfo)
	{
		int curCmdID = ele.first;
		//子流程信息
		deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.sub_flow_info WHERE command_id=%1").arg(curCmdID));
		//指令参数
		deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.command_param_info WHERE command_id=%1").arg(curCmdID));
		//指令设备状态
		deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.command_devicestatus_info WHERE command_id=%1").arg(curCmdID));
		//指令表信息
		deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.command_commandtable_info WHERE command_id=%1").arg(curCmdID));
		//指令信息
		deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.command_info WHERE id=%1").arg(curCmdID));
	}
	//指令信息
	deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.command_table_info WHERE rocket_id=%1").arg(curRocketID));
	//箭上参数
	qSqlString = QString("SELECT\
		rocket_data_info.id,\
		rocket_data_info.createTime,\
		rocket_data_info.lastUpdateTime\
		FROM\
		rocket_data_info\
		WHERE\
		rocket_data_info.rocket_id = %1").arg(curRocketID);
	deleteOp->customReadTableInfo(qSqlString);
	auto releRocketDataInfo = deleteOp->customReadInfoMap;
	for (auto ele : releRocketDataInfo)
	{
		deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.rockect_param_info WHERE rocket_data_id=%1").arg(ele.first));
	}
	//箭上数据
	deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.rocket_data_info WHERE rocket_id=%1").arg(curRocketID));
	//子流程信息
	qSqlString = QString("SELECT\
		main_flow_info.id,\
		main_flow_info.createTime,\
		main_flow_info.lastUpdateTime\
		FROM\
		main_flow_info\
		WHERE\
		main_flow_info.rocket_id = %1").arg(curRocketID);
	deleteOp->customReadTableInfo(qSqlString);
	auto releMainFlowInfo = deleteOp->customReadInfoMap;
	for (auto ele : releMainFlowInfo)
	{
		deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.sub_flow_info WHERE main_id=%1").arg(ele.first));
	}
	//主流程信息
	deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.main_flow_info WHERE rocket_id=%1").arg(curRocketID));
	//故障指令信息
	deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.fault_command_info WHERE rocket_id=%1").arg(curRocketID));
	// 故障参数信息
	deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.fault_param_info WHERE rocket_id=%1").arg(curRocketID));
	//型号岗位信息
	deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.rocket_task_info WHERE rocket_id=%1").arg(curRocketID));
	//型号信息
	deleteOp->customRunSql(QString("DELETE FROM simulatedtraining.rocket_info WHERE id=%1").arg(curRocketID));

	//emit workFinished();
}