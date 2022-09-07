#include "DeviceDBConfigInfo.h"

DeviceDBConfigInfo* DeviceDBConfigInfo::instance = nullptr;


DeviceDBConfigInfo::DeviceDBConfigInfo() {

	//根据APPcache中的配置信息进行数据库初始化
	deviceManageDBOp = new DataBase::DeviceDAO(AppCache::instance()->m_outputPath);

	if (deviceManageDBOp->connect())
	{
		qDebug() << "连接数据库失败，检查数据库设置<<DeviceDBConfigInfo";

	}

}



/**
	@brief 操作parameter_info表
	@param paramName -
	@param paramType -
	@param paramUnit -
**/
void DeviceDBConfigInfo::paramConfigOp2DB(QString paramName, int paramType, QString paramUnit) {
	QString qSqlString = "INSERT INTO `simulatedtraining`.`parameter_info`(`name`, `type`, `unit`) VALUES ('%1', %2, '%3');";
	qSqlString = qSqlString.arg(paramName).arg(paramType).arg(paramUnit);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "往参数数据库写入失败!!!";
	}
}

/**
	@brief 操作switch_value表
	@param statusName -
**/
void DeviceDBConfigInfo::paramSwitchVal2DB(QString statusName) {
	QString qSqlString = "INSERT INTO `simulatedtraining`.`switch_value`(`name`) VALUES ('%1');";
	qSqlString = qSqlString.arg(statusName);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "往参数数据库写入失败!!!";
	}
}

/**
	@brief 操作switch_show_info
	@param bindParamID      -
	@param bindSwithValueID -
	@param statusValIndex   -
**/
void DeviceDBConfigInfo::paramSwitchShowInfo2DB(int bindParamID, int bindSwithValueID, int statusValIndex) {
	QString qSqlString = "INSERT INTO `simulatedtraining`.`switch_show_info`(`param_id`, `switch_valueid`, `status_value`) VALUES  ('%1', %2, '%3');";
	qSqlString = qSqlString.arg(bindParamID).arg(bindSwithValueID).arg(statusValIndex);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "往参数数据库写入失败!!!";
	}
}

/**
	@brief 操作device_info
	@param deviceName -
	@param virtualDev -  0代表真实设备，1代表虚拟设备，默认为1
**/
void DeviceDBConfigInfo::deviceConfigOp2DB(int rocketType, QString deviceName, int virtualDev) {
	QString qSqlString = "INSERT INTO `simulatedtraining`.`device_info`(`rocket_id`, `name`, `isVirtual`) VALUES  (%1, '%2', %3);";
	qSqlString = qSqlString.arg(rocketType).arg(deviceName).arg(virtualDev);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "往参数数据库写入失败!!!";
	}
}

/**
	@brief 操作status_info
	@param statusName -
**/
void DeviceDBConfigInfo::deviceStatusInfo2DB(QString statusName) {
	QString qSqlString = "INSERT INTO `simulatedtraining`.`status_info`(`name`) VALUES  ('%1');";
	qSqlString = qSqlString.arg(statusName);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "往参数数据库写入失败!!!";
	}
}

/**
	@brief 操作device_status_info
	@param deviceID -
	@param statusID -
	@param dataPath -
**/
void DeviceDBConfigInfo::deviceStatusInfo2DB(int deviceID, int statusID, QString dataPath) {
	QString qSqlString = "INSERT INTO `simulatedtraining`.`device_status_info`(`device_id`, `status_id`, `dataPath`)  VALUES (%1, %2, '%3');";
	qSqlString = qSqlString.arg(deviceID).arg(statusID).arg(dataPath);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "往参数数据库写入失败!!!";
	}
}

/**
	@brief 操作device_param_info
	@param deviceID    -
	@param parameterID -
**/
void DeviceDBConfigInfo::deviceParamInfo2DB(int deviceID, int parameterID) {
	QString qSqlString = "INSERT INTO `simulatedtraining`.`device_param_info`(`device_id`, `parameter_id`) VALUES (%1, %2);";
	qSqlString = qSqlString.arg(deviceID).arg(parameterID);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "往参数数据库写入失败!!!";
	}
}


/**
	@brief 操作command_info
	@param cmdName   -
	@param backCmdID -
	@param cmdCode   -
	@param cmdType   -
	@param cmdPrefix -
**/
void DeviceDBConfigInfo::commandConfigOp2DB(QString cmdName, int rocketID, int backCmdID, int cmdCode, int cmdType, int cmdPrefix) {
	QString qSqlString = "INSERT INTO `simulatedtraining`.`command_info`(`rocket_id`, `back_id`, `name`, `code`, `type`, `prefix`)  VALUES (%1, %2, '%3', %4, %5, %6);";
	qSqlString = qSqlString.arg(rocketID).arg(backCmdID).arg(cmdName).arg(cmdCode).arg(cmdType).arg(cmdPrefix);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "往参数数据库写入失败!!!";
	}
}

/**
	@brief 操作command_param_info
	@param cmdID      -
	@param paramName  -
	@param paramCode  -
	@param index      -
	@param length     -
	@param resultType -
	@param defaultVal -
**/
void DeviceDBConfigInfo::commandParamInfo2DB(int cmdID, QString paramName, int paramCode, int index, int length, QString resultType, float defaultVal) {
	QString qSqlString = "INSERT INTO `simulatedtraining`.`command_param_info`(`command_id`, `name`, `code`, `index`, `length`, `resultType`, `defaultValue`)  VALUES (%1, '%2', %3, %4, %5, '%6', %7);";
	qSqlString = qSqlString.arg(cmdID).arg(paramName).arg(paramCode).arg(index).arg(length).arg(resultType).arg(defaultVal);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "往参数数据库写入失败!!!";
	}
}

/**
	@brief 操作commandDeviceStat_info表
	@param cmdID        -
	@param deviceStatID -
**/
void DeviceDBConfigInfo::commandDeviceStatInfo2DB(int cmdID, int deviceStatID) {
	QString qSqlString = "INSERT INTO `simulatedtraining`.`command_devicestatus_info`(`command_id`, `deviceStatus_id`)  VALUES (%1, %2);";
	qSqlString = qSqlString.arg(cmdID).arg(deviceStatID);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "往commandDeviceStat_info表写入失败!!!";
	}
}


/**
	@brief 操作rocket_info
	@param rocketName     -
	@param rocketTypeCode -
**/
void DeviceDBConfigInfo::rocketConfigOp2DB(QString rocketName, QString rocketDescript) {
	QString qSqlString = "INSERT INTO `simulatedtraining`.`rocket_info`(`name`, `code`) VALUES ('%1', '%2');";
	qSqlString = qSqlString.arg(rocketName).arg(rocketDescript);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "往参数数据库写入失败!!!";
	}
}
/**
	@brief 操作rocket_data_info
	@param rocketID  -
	@param cmdName   -
	@param cmdCode   -
	@param cmdPrefix -
**/
void DeviceDBConfigInfo::rocketDataInfo2DB(int rocketID, QString cmdName, int cmdCode, int cmdPrefix) {
	QString qSqlString = "INSERT INTO `simulatedtraining`.`rocket_data_info`(`rocket_id`, `name`, `code`, `prefix`) VALUES (%1, '%2', %3, %4);";
	qSqlString = qSqlString.arg(rocketID).arg(cmdName).arg(cmdCode).arg(cmdPrefix);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "往参数数据库写入失败!!!";
	}
}

/**
	@brief 操作rocket_param_info
	@param rocketDataID -
	@param parameterID  -
	@param index        -
	@param length       -
	@param resultType   -
**/
void DeviceDBConfigInfo::rocketParamInfo2DB(int rocketDataID, int parameterID, int index, int length, int resultType) {
	QString qSqlString = "INSERT INTO `simulatedtraining`.`rockect_param_info`(`rocket_data_id`, `device_parameter_id`, `index`, `length`, `resultType`) VALUES (%1, %2, %3, %4, '%5');";
	qSqlString = qSqlString.arg(rocketDataID).arg(parameterID).arg(index).arg(length).arg(resultType);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "往参数数据库写入失败!!!";
	}
}

/**
	@brief 读参数数据库
	@param IDV        -
	@param paramNameV -
	@param paramTypeV -
**/
void DeviceDBConfigInfo::readParamDB2UI() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`parameter_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	paramInfo.clear();
	deviceManageDBOp->queryMysql(sqlString, paramInfo);

	qDebug() << qSqlString;
}

/**
	@brief 读设备数据库
	@param IDV           -
	@param rocketTypeIDV -
	@param deviceNameV   -
	@param deviceTypeV   -
**/
void DeviceDBConfigInfo::readDeviceDB2UI() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`device_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	deviceInfo.clear();
	deviceManageDBOp->queryMysql(sqlString, deviceInfo);

	qDebug() << qSqlString;
}

/**
	@brief 读指令数据库
	@param IDV          -
	@param commandNameV -
	@param backCmdIDV   -
	@param cmdCodeV     -
	@param cmdTypeV     -
	@param cmdPrefixV   -
**/
void DeviceDBConfigInfo::readCommandDB2UI() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`command_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	commandInfo.clear();
	deviceManageDBOp->queryMysql(sqlString, commandInfo);

	qDebug() << qSqlString;
}

/**
	@brief 读火箭数据库
	@param IDV         -
	@param rocketNameV -
	@param rocketTypeV -
**/
void DeviceDBConfigInfo::readRocketDB2UI() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`rocket_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	rocketInfo.clear();
	deviceManageDBOp->queryMysql(sqlString, rocketInfo);

	qDebug() << qSqlString;
}


//其他相关表读操作
/**
	@brief switchValueInfo
**/
void DeviceDBConfigInfo::readSwitchValueDB2UI() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`switch_value` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	switchValueInfo.clear();
	deviceManageDBOp->queryMysql(sqlString, switchValueInfo);
}

/**
	@brief switchShowInfo
**/
void DeviceDBConfigInfo::readSwitchShowInfoDB2UI() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`switch_show_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	switchShowInfo.clear();
	deviceManageDBOp->queryMysql(sqlString, switchShowInfo);
}

/**
	@brief statusInfo
**/
void DeviceDBConfigInfo::readStatusInfoDB2UI() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`status_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	statusInfo.clear();
	deviceManageDBOp->queryMysql(sqlString, statusInfo);
}

/**
	@brief deviceStatusInfo
**/
void DeviceDBConfigInfo::readDeviceStatusDB2UI() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`device_status_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	deviceStatusInfo.clear();
	deviceManageDBOp->queryMysql(sqlString, deviceStatusInfo);
}

/**
	@brief deviceParamInfo
**/
void DeviceDBConfigInfo::readDeviceParamDB2UI() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`device_param_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	deviceParamInfo.clear();
	deviceManageDBOp->queryMysql(sqlString, deviceParamInfo);
}

/**
	@brief commandParamInfo
**/
void DeviceDBConfigInfo::readCommandParamDB2UI() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`command_param_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	commandParamInfo.clear();
	deviceManageDBOp->queryMysql(sqlString, commandParamInfo);
}

/**
	@brief rocketDataInfo
**/
void DeviceDBConfigInfo::readRocketDataDB2UI() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`rocket_data_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	rocketDataInfo.clear();
	deviceManageDBOp->queryMysql(sqlString, rocketDataInfo);
}

/**
	@brief rocketParamInfo
**/
void DeviceDBConfigInfo::readRocketParamDB2UI() {
	QString qSqlString = "SELECT * FROM `simulatedtraining`.`rockect_param_info` LIMIT 0, 1000; ";
	string sqlString = qSqlString.toStdString();
	rocketParamInfo.clear();
	deviceManageDBOp->queryMysql(sqlString, rocketParamInfo);
}

/**
	@brief commandDeviceStatInfo
**/
void DeviceDBConfigInfo::readCMDDeviceStatDB2UI() {
	QString qSqlString = "SELECT\
		command_devicestatus_info.command_id,\
		device_status_info.device_id,\
		device_status_info.status_id,\
		device_status_info.id,\
		device_status_info.createTime,\
		device_status_info.lastUpdateTime\
		FROM\
		command_info\
		INNER JOIN command_devicestatus_info ON command_info.id = command_devicestatus_info.command_id\
		INNER JOIN device_status_info ON command_devicestatus_info.deviceStatus_id = device_status_info.id; ";
	string sqlString = qSqlString.toStdString();
	commandDeviceStatInfo.clear();
	deviceManageDBOp->queryMysql(sqlString, commandDeviceStatInfo);
}

/**
	@brief 设备到参数的对应关系
**/
void DeviceDBConfigInfo::readDevice2ParamInfo() {
	QString qSqlString = "SELECT\
		device_param_info.device_id,\
		device_param_info.parameter_id,\
		device_param_info.createTime,\
		device_param_info.lastUpdateTime\
		FROM\
		device_param_info; ";

	string sqlString = qSqlString.toStdString();
	device2Param.clear();
	deviceManageDBOp->queryMysql(sqlString, device2Param);
}

/**
	@brief 更新参数表信息
	@param paramID   -
	@param paramName -
	@param paramType -
	@param paramUnit -
**/
void DeviceDBConfigInfo::updateParamInfo2DB(int paramID, QString paramName, int paramType, QString paramUnit) {

	QString qSqlString = "UPDATE `simulatedtraining`.`parameter_info` SET `name` = '%2', `type` = %3, `unit` = '%4' WHERE `id` = %1;";
	qSqlString = qSqlString.arg(paramID).arg(paramName).arg(paramType).arg(paramUnit);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "更新写入参数表失败!!!";
	}
}

/**
	@brief 更新设备表
	@param paramID    -
	@param rocketType -
	@param deviceName -
	@param virtualDev -
**/
void DeviceDBConfigInfo::updateDeviceInfo2DB(int paramID, int rocketType, QString deviceName, int virtualDev) {
	QString qSqlString = "UPDATE `simulatedtraining`.`device_info` SET `rocket_id` = %2, `name` = '%3', `isVirtual` = %4 WHERE `id` = %1;";
	qSqlString = qSqlString.arg(paramID).arg(rocketType).arg(deviceName).arg(virtualDev);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "更新写入设备表失败!!!";
	}

}

/**
	@brief 更新指令表
	@param paramID   -
	@param cmdName   -
	@param rocketID  -
	@param backCmdID -
	@param cmdCode   -
	@param cmdType   -
	@param cmdPrefix -
**/
void DeviceDBConfigInfo::updateCommandInfo2DB(int paramID, QString cmdName, int rocketID, int backCmdID, int cmdCode, int cmdType, int cmdPrefix) {

	QString qSqlString = "UPDATE `simulatedtraining`.`command_info` SET `rocket_id` = %3, `back_id` = %4, `name` = '%2', `code` = %5, `type` = %6, `prefix` = %7 WHERE `id` = %1;";
	qSqlString = qSqlString.arg(paramID).arg(cmdName).arg(rocketID).arg(backCmdID).arg(cmdCode).arg(cmdType).arg(cmdPrefix);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "更新写入指令表失败!!!";
	}
}

/**
	@brief 更新火箭表
	@param paramID        -
	@param rocketName     -
	@param rocketTypeCode -
**/
void DeviceDBConfigInfo::updateRocketInfo2DB(int paramID, QString rocketName, QString rocketTypeCode) {
	QString qSqlString = "UPDATE `simulatedtraining`.`rocket_info` SET `name` = '%2', `code` = '%3' WHERE `id` = %1;";
	qSqlString = qSqlString.arg(paramID).arg(rocketName).arg(rocketTypeCode);
	string sqlString = qSqlString.toStdString();
	bool opRet = deviceManageDBOp->exec_sql(sqlString);
	if (!opRet)
	{
		qDebug() << "更新写入火箭表失败!!!";
	}

}

/**
	@brief	  自定义执行语句
	@param qSqlString -
**/
void DeviceDBConfigInfo::customRunSql(QString qSqlString) {
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
void DeviceDBConfigInfo::customReadTableInfo(QString qSqlString) {
	string sqlString = qSqlString.toStdString();
	customReadInfoMap.clear();
	bool opRet = deviceManageDBOp->queryMysql(sqlString, customReadInfoMap);
	if (!opRet)
	{
		qDebug() << "获取自定义数据失败!!!";
	}
}