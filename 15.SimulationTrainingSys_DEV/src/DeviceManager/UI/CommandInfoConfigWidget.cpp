#include "CommandInfoConfigWidget.h"

using namespace CommandInfoConfig;

InfoConfigWidget* InfoConfigWidget::instance = nullptr;

InfoConfigWidget::InfoConfigWidget(QWidget* parent)
	: QWidget(parent)
{
	currentDeviceFlag = DeviceCommonVaries::getInstance()->DeviceModule::ADD_MODULE;

	this->setWindowTitle(QString("指令配置"));
	this->setWindowIcon(QIcon(":/icon/icon/bb.png"));

	//this->setBackgroundRole(QPalette::Light);

	//获得widget的palette
	QPalette palette = this->palette();
	palette.setBrush(QPalette::Window, QBrush(Qt::white));
	this->setPalette(palette);
	this->setAttribute(Qt::WA_StyledBackground);
	this->setAutoFillBackground(true);

	this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);

	InitUILayout();

	widgetConfig();

	initConnect();
}

InfoConfigWidget::~InfoConfigWidget()
{

}

/**
	@brief 根据不同模块初始化配置布局
**/
void InfoConfigWidget::InitUILayout() {
	//this->setFixedSize(640, 480);
	wss = WidgetStyleSheet::getInstace();

	//!< command_info
	configRocketType = new QLabel(QString("火箭型号:"));
	userSelectRocketType = new QComboBox;
	configBackCommand = new QLabel(QString("回令:"));
	userSelectBackCMD = new QComboBox;
	configCommandName = new QLabel(QString("指令名称:"));
	userInputCMDName = new QLineEdit;
	userInputCMDName->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	configCommandCode = new QLabel(QString("指令编码:"));;
	userSelectCMDCode = new QComboBox;

	configCommandType = new QLabel(QString("指令类型:"));
	userSelectCMDType = new QComboBox;
	configCommandPrefix = new QLabel(QString("指令前缀:"));
	userSelectCMDPrefix = new QComboBox;

	commandInfoOKBtn = new QPushButton(QString("更新指令"));

	//!< 	command_param_info
	configEmitCmdID = new QLabel(QString("测发指令:"));
	userSelectEmitCmdID = new QComboBox();
	configParamName = new QLabel(QString("参数名称:"));
	userInputParamName = new QLineEdit;
	userInputParamName->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	configParamCode = new QLabel(QString("参数编码:"));
	userSelectParamCode = new QComboBox();
	configIndex = new QLabel(QString("索引:"));
	userSelectIndex = new QComboBox();
	configParamLength = new QLabel(QString("参数字节长度:"));
	userSelectParamLength = new QComboBox();
	configParamType = new QLabel(QString("参数类型:"));
	userSelectParamType = new QComboBox();
	configParamDefaultVal = new QLabel(QString("参数默认值:"));
	userSelectParamDefualtVal = new QComboBox();

	commandParamInfoOKBtn = new QPushButton(QString("新增指令帧内容"));

	configCmdDevID = new QLabel(QString("测发指令:"));
	userSelectCmdDevID = new QComboBox;
	configDevID = new QLabel(QString("设备状态:"));
	userSelectDevStatID = new QComboBox;

	commandDeviceInfoOKBtn = new QPushButton(QString("新增指令设备"));

	updateCommandBtn = new QPushButton(QString("取消"));

	QGridLayout* infoUILayout = new QGridLayout;
	int rowCount = 40;
	int columnCount = 4;
	int row = 0;
	int column = 0;

	infoUILayout->addWidget(new QLabel(QString("指令配置")), row++, 0);

	QFrame* hframe0 = new QFrame;
	hframe0->setFrameShape(QFrame::HLine);
	//hframe0->setStyleSheet("background:gray;min-height:1px");

	infoUILayout->addWidget(hframe0, row++, 0, 1, columnCount);
	//第一行
	infoUILayout->addWidget(configRocketType, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectRocketType, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configBackCommand, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectBackCMD, row++ % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configCommandName, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputCMDName, row % rowCount, column++ % columnCount, 1, 1);

	infoUILayout->addWidget(configCommandCode, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectCMDCode, row++ % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configCommandType, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectCMDType, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configCommandPrefix, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectCMDPrefix, row++ % rowCount, column++ % columnCount, 1, 1);

	infoUILayout->addWidget(commandInfoOKBtn, row++ % rowCount, 0, 1, 1);

	QFrame* frameSpace0 = new QFrame;
	frameSpace0->setFrameShape(QFrame::Box);
	infoUILayout->addWidget(frameSpace0, row++, 0);
	frameSpace0->setLineWidth(0);

	infoUILayout->addWidget(new QLabel(QString("指令帧配置")), row++, 0);
	QFrame* hframe1 = new QFrame;
	hframe1->setFrameShape(QFrame::HLine);
	infoUILayout->addWidget(hframe1, row++, 0, 1, columnCount);

	//第二行
	infoUILayout->addWidget(configEmitCmdID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectEmitCmdID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configParamName, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputParamName, row++ % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configParamCode, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectParamCode, row % rowCount, column++ % columnCount, 1, 1);

	infoUILayout->addWidget(configIndex, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectIndex, row++ % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configParamLength, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectParamLength, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configParamType, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectParamType, row++ % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configParamDefaultVal, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectParamDefualtVal, row++ % rowCount, column++ % columnCount, 1, 1);

	infoUILayout->addWidget(commandParamInfoOKBtn, row++ % rowCount, 0, 1, 1);


	QFrame* frameSpace1 = new QFrame;
	frameSpace0->setFrameShape(QFrame::Box);
	infoUILayout->addWidget(frameSpace1, row++, 0);
	frameSpace0->setLineWidth(0);

	infoUILayout->addWidget(new QLabel(QString("指令设备配置")), row++, 0);
	QFrame* hframe2 = new QFrame;
	hframe2->setFrameShape(QFrame::HLine);
	infoUILayout->addWidget(hframe2, row++, 0, 1, columnCount);

	column = 0;
	infoUILayout->addWidget(configCmdDevID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectCmdDevID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configDevID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectDevStatID, row++ % rowCount, column++ % columnCount, 1, 1);


	infoUILayout->addWidget(commandDeviceInfoOKBtn, row++ % rowCount, 0, 1, 1);


	infoUILayout->addWidget(updateCommandBtn, row++ % rowCount, columnCount - 1, 1, 1);


	QString qss = wss->infoConfigLabelStyleSheet.arg("QLabel") + wss->infoConfigLineEditStyleSheet.arg("QLineEdit")
		+ wss->infoConfigPushButtonStyleSheet.arg("QPushButton1") + wss->infoConfigComboBoxStyleSheet.arg("QComboBox");

	this->setStyleSheet(qss);
	this->setLayout(infoUILayout);
	this->setMinimumHeight(480);

}


/**
	@brief 控件配置
**/
void InfoConfigWidget::widgetConfig() {
	//火箭类型
	DeviceCommonVaries* commonVaries = DeviceCommonVaries::getInstance();
	auto deviceManageDbOp = DeviceDBConfigInfo::getInstance();

	userSelectRocketType->clear();
	deviceManageDbOp->rocketInfo.clear();
	deviceManageDbOp->readRocketDB2UI();
	for (auto ele = deviceManageDbOp->rocketInfo.begin(); ele != deviceManageDbOp->rocketInfo.end(); ele++)
	{
		userSelectRocketType->addItem(QString(ele->second[1].c_str()), QString(ele->second[0].c_str()).toInt());
	}
	//回令
	userSelectBackCMD->clear();
	deviceManageDbOp->commandInfo.clear();
	deviceManageDbOp->readCommandDB2UI();

	for (auto ele = deviceManageDbOp->commandInfo.begin(); ele != deviceManageDbOp->commandInfo.end(); ele++)
	{

		//筛选测发回令
		if (QString(ele->second[5].c_str()).toInt() == 2)
		{
			userSelectBackCMD->addItem(QString(ele->second[3].c_str()), QString(ele->second[0].c_str()).toInt());
		}

	}
	userSelectBackCMD->addItem(QString("无回令"), 0);

	//指令编码
	userSelectCMDCode->clear();
	for (auto ele = commonVaries->commandCode.begin(); ele != commonVaries->commandCode.end(); ele++)
	{
		userSelectCMDCode->addItem(QString(ele->first.c_str()), ele->second);
	}
	//指令类型
	userSelectCMDType->clear();
	for (auto ele = commonVaries->commandType.begin(); ele != commonVaries->commandType.end(); ele++)
	{
		userSelectCMDType->addItem(QString::fromLocal8Bit(ele->first.c_str()), ele->second);
	}

	//指令前缀
	userSelectCMDPrefix->clear();
	for (auto ele = commonVaries->commandPrefix.begin(); ele != commonVaries->commandPrefix.end(); ele++)
	{
		auto aa = QString(ele->first.c_str()).toUInt(nullptr, 16);
		userSelectCMDPrefix->addItem(QString(ele->first.c_str()), aa);
	}

	//测发指令ID
	QString qSqlString = QString("SELECT\
		command_info.id,\
		command_info.`name`,\
		command_info.prefix,\
		command_info.createTime\
		FROM\
		command_info\
		WHERE\
		command_info.rocket_id = %1 AND\
		command_info.type = %2; \
		");

	qSqlString = qSqlString.arg(AppCache::instance()->m_CurrentRocketType->m_id).arg(1);
	deviceManageDbOp->customReadTableInfo(qSqlString);
	userSelectEmitCmdID->clear();

	//这里需要筛选类型为测发指令的
	for (auto ele = deviceManageDbOp->customReadInfoMap.begin(); ele != deviceManageDbOp->customReadInfoMap.end(); ele++)
	{

		userSelectEmitCmdID->addItem(QString(ele->second[1].c_str()), QString(ele->second[0].c_str()).toInt());
	}

	//参数编码
	userSelectParamCode->clear();
	for (auto ele = commonVaries->commandParamCode.begin(); ele != commonVaries->commandParamCode.end(); ele++)
	{
		userSelectParamCode->addItem(QString(ele->first.c_str()), ele->second);
	}
	//索引
	userSelectIndex->clear();
	for (auto ele : commonVaries->commandParamIndex)
	{
		userSelectIndex->addItem(QString::number(ele));
	}
	//长度
	userSelectParamLength->clear();
	for (auto ele : commonVaries->commandParamLength)
	{
		userSelectParamLength->addItem(QString::number(ele));
	}

	//参数类型
	userSelectParamType->clear();
	for (auto ele : commonVaries->commandParamType)
	{
		userSelectParamType->addItem(QString::fromLocal8Bit(ele.c_str()));
	}
	//参数默认值
	userSelectParamDefualtVal->clear();
	for (auto ele = commonVaries->commandParamDefaultVal.begin(); ele != commonVaries->commandParamDefaultVal.end(); ele++)
	{
		userSelectParamDefualtVal->addItem(QString(ele->first.c_str()), ele->second);
	}

	//测发指令ID
	//这里需要筛选类型为测发指令的
	qSqlString = QString("SELECT\
		command_info.id,\
		command_info.`name`,\
		command_info.prefix,\
		command_info.createTime\
		FROM\
		command_info\
		WHERE\
		command_info.rocket_id = %1 AND\
		command_info.type = %2; \
		");

	qSqlString = qSqlString.arg(AppCache::instance()->m_CurrentRocketType->m_id).arg(1);
	deviceManageDbOp->customReadTableInfo(qSqlString);
	userSelectCmdDevID->clear();
	for (auto ele = deviceManageDbOp->customReadInfoMap.begin(); ele != deviceManageDbOp->customReadInfoMap.end(); ele++)
	{

		userSelectCmdDevID->addItem(QString(ele->second[1].c_str()), QString(ele->second[0].c_str()).toInt());

	}
	//设备状态ID
	qSqlString = QString("SELECT\
		device_status_info.id,\
		device_info.`name`,\
		status_info.`name`,\
		status_info.createTime,\
		status_info.lastUpdateTime\
		FROM\
		device_info\
		INNER JOIN device_status_info ON device_info.id = device_status_info.device_id\
		INNER JOIN status_info ON device_status_info.status_id = status_info.id\
		WHERE\
		device_info.rocket_id = %1;");

	qSqlString = qSqlString.arg(AppCache::instance()->m_CurrentRocketType->m_id);
	deviceManageDbOp->customReadTableInfo(qSqlString);
	userSelectDevStatID->clear();


	for (auto ele = deviceManageDbOp->customReadInfoMap.begin(); ele != deviceManageDbOp->customReadInfoMap.end(); ele++)
	{
		QString tmpp = QString::fromStdString(ele->second[1].c_str()) + QString::fromStdString(ele->second[2].c_str());
		userSelectDevStatID->addItem(tmpp, atoi(ele->second[0].c_str()));
	}

}

void InfoConfigWidget::initConnect() {

	connect(commandInfoOKBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickCommandOKBtn);

	connect(commandParamInfoOKBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickCommandParamOKBtn);

	connect(commandDeviceInfoOKBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickCommandDeviceStatOKBtn);

	connect(updateCommandBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickUpateCommandBtn);


}


void InfoConfigWidget::setConfigHeader(QString header) {

}

/**
	@brief 指令配置确认
**/
void InfoConfigWidget::clickCommandOKBtn() {
	QString cmdName = userInputCMDName->text();
	if (cmdName == "")
	{
		QMessageBox::information(this, "info", QString("指令名不能为空！"));
		return;
	}
	auto tmpInstance = DeviceDBConfigInfo::getInstance();

	int rocketTypeID = userSelectRocketType->currentData().toInt();
	int cmdBackID = userSelectBackCMD->currentData().toInt();
	int cmdCode = userSelectCMDCode->currentData().toInt();//编码唯一


	int cmdType = userSelectCMDType->currentData().toInt();
	int cmdPredix = userSelectCMDPrefix->currentData().toInt();

	if (currentDeviceFlag == DeviceCommonVaries::getInstance()->DeviceModule::ADD_MODULE)
	{
		//访问指令表  查询是否存在相同编码
		DeviceDBConfigInfo::getInstance()->readCommandDB2UI();
		for (auto var : DeviceDBConfigInfo::getInstance()->commandInfo)
		{
			if (atoi(var.second[4].c_str()) == cmdCode)
			{
				QMessageBox::warning(this, QString("警告"), QString("已存在该指令编码，请重新选择编码!"));
				return;
			}
		}
		tmpInstance->commandConfigOp2DB(cmdName, rocketTypeID, cmdBackID, cmdCode, cmdType, cmdPredix);
	}
	if (currentDeviceFlag == DeviceCommonVaries::getInstance()->DeviceModule::UPDATE_MODULE)
	{
		tmpInstance->updateCommandInfo2DB(editId, cmdName, rocketTypeID, cmdBackID, cmdCode, cmdType, cmdPredix);
		this->close();
	}

	widgetConfig();
	emit updateCommandInfos();
	currentDeviceFlag = DeviceCommonVaries::getInstance()->DeviceModule::ADD_MODULE;
}

/**
	@brief 指令参数配置确认
**/
void InfoConfigWidget::clickCommandParamOKBtn() {
	int cmdID = userSelectEmitCmdID->currentData().toInt();
	QString paramName1 = userInputParamName->text();
	if (paramName1 == "")
	{
		QMessageBox::information(this, "info", QString("参数名不能为空！"));
		return;
	}
	int paramCode1 = userSelectParamCode->currentData().toInt();
	int index1 = userSelectIndex->currentData().toInt();

	QString qSqlString = QString("SELECT\
		command_param_info.id,\
		command_param_info.command_id,\
		command_param_info.`code`,\
		command_param_info.`index`,\
		command_param_info.createTime,\
		command_param_info.lastUpdateTime\
		FROM\
		command_param_info\
		WHERE\
		command_param_info.command_id = %1");

	qSqlString = qSqlString.arg(cmdID);

	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);


	for (auto var : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
	{
		if (atoi(var.second[2].c_str()) == paramCode1)
		{
			QMessageBox::warning(this, QString("警告"), QString("当前指令已存在参数编码，请重新选择编码!"));
			return;
		}
		if (atoi(var.second[3].c_str()) == index1)
		{
			QMessageBox::warning(this, QString("警告"), QString("当前指令已存在索引，请重新选择索引!"));
			return;
		}
	}


	int paramLength1 = userSelectParamLength->currentData().toInt();
	QString paramType1 = userSelectParamType->currentText();
	float paramDefaultVal = userSelectParamDefualtVal->currentData().toFloat();
	DeviceDBConfigInfo::getInstance()->commandParamInfo2DB(cmdID, paramName1, paramCode1, index1, paramLength1, paramType1, paramDefaultVal);
	widgetConfig();
}

/**
	@brief 指令设备
**/
void InfoConfigWidget::clickCommandDeviceStatOKBtn() {

	int cmdID = userSelectCmdDevID->currentData().toInt();
	int devStatusID = userSelectDevStatID->currentData().toInt();

	DeviceDBConfigInfo::getInstance()->commandDeviceStatInfo2DB(cmdID, devStatusID);

	widgetConfig();
}

void InfoConfigWidget::clickUpateCommandBtn() {

	this->close();

}