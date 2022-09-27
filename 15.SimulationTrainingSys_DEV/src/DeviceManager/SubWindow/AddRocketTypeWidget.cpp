#include "AddRocketTypeWidget.h"

AddRocketTypeWidget* AddRocketTypeWidget::instance = nullptr;

AddRocketTypeWidget::AddRocketTypeWidget(QWidget* parent)
	: QWidget(parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

	currentModule = DeviceCommonVaries::ADD_MODULE;

	itemDelegate = new ItemDelegate(this);

	windowName = "新增火箭型号";
	InitUILayout();
	initConnect();

	paramTableType->installEventFilter(this);
	cmdTableType->installEventFilter(this);
}

AddRocketTypeWidget::~AddRocketTypeWidget()
{}

/**
	@brief 初始化布局
**/
void AddRocketTypeWidget::InitUILayout() {

	int row = 0;
	int column = 0;
	int rowC = 100;
	int columnC = 8;

	QGridLayout* UIGrid = new QGridLayout;

	windowIcon = new QLabel;
	windowTitle = new QLabel(windowName);
	windowCloseBtn = new QPushButton;
	//火箭型号部分
	rocketName = new QLabel(QString("火箭型号名称:"));
	userInputRocketName = new QLineEdit;
	userInputRocketName->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	rocketDescript = new QLabel(QString("描述:"));
	userInputDescript = new QLineEdit;
	userInputDescript->setValidator(new QRegExpValidator(QRegExp("\\S+")));
	//参数部分
	paramRocket = new QLabel(QString("所属型号"));
	paramRocketType = new QLineEdit();
	paramRocketType->setText(QString::fromLocal8Bit(AppCache::instance()->m_CurrentRocketType->m_name.c_str()));
	paramRocketType->setEnabled(false);
	paramName = new QLabel(QString("参数名称:"));
	userInputParamName = new QLineEdit;
	paramType = new QLabel(QString("参数类型:"));
	userSelectParamType = new QComboBox;
	paramUnit = new QLabel(QString("参数单位:"));
	userInputParamUnit = new QLineEdit;
	paramTable = new QLabel(QString("所属参数表"));
	paramTableType = new QComboBox;
	paramTableType->setItemDelegate(itemDelegate);
	paramTableType->setEditable(false);
	addParamTable = new QPushButton();
	addParamTable->setStyleSheet("QPushButton{height:30px;border:none;image:url(:/FaultInjection/images/addFault (2).png);}\
								QPushButton::hover{image:url(:/FaultInjection/images/addFault.png);}");

	//设备部分
	rocketType = new QLabel(QString("火箭型号:"));
	userSelectRocketType = new QComboBox;
	deviceName = new QLabel(QString("设备名称:"));
	userInputDevName = new QLineEdit;
	deviceType = new QLabel(QString("设备类型:"));
	userSelectDevType = new QComboBox;
	//指令部分
	commandType = new QLabel(QString("指令类型:"));
	userSelectCmdType = new QComboBox;
	commandName = new QLabel(QString("指令名称:"));
	userInputCmdName = new QLineEdit;
	cmdBackCmd = new QLabel(QString("测发回令:"));
	userSelectBackCmd = new QComboBox;
	cmdBackCmd->hide();
	userSelectBackCmd->hide();
	cmdTable = new QLabel(QString("所属指令表"));
	cmdTableType = new QComboBox;
	cmdTableType->setEditable(false);
	cmdTableType->setItemDelegate(itemDelegate);
	addCmdTable = new QPushButton();
	addCmdTable->setStyleSheet("QPushButton{height:30px;border:none;image:url(:/FaultInjection/images/addFault (2).png);}\
								QPushButton::hover{image:url(:/FaultInjection/images/addFault.png);}");

	//
	CancelBtn = new QPushButton(QString("取消"));
	OkBtn = new QPushButton(QString("确定"));

	QHBoxLayout* topLayout = new QHBoxLayout;


	topLayout->addWidget(windowIcon);
	topLayout->addWidget(windowTitle);
	topLayout->addSpacerItem(new QSpacerItem(440, 20));
	topLayout->addWidget(windowCloseBtn);

	UIGrid->addLayout(topLayout, 0, 0, 1, columnC);

	row = 2;
	column = 0;
	QFrame* hframe1 = new QFrame;
	hframe1->setFrameShape(QFrame::HLine);
	hframe1->setFixedWidth(640);
	hframe1->setBackgroundRole(QPalette::Background);

	QVBoxLayout* lineLayout = new QVBoxLayout;
	lineLayout->addWidget(hframe1);
	lineLayout->addSpacing(30);

	UIGrid->addLayout(lineLayout, row++, 0, 1, columnC);
	UIGrid->setContentsMargins(0, 12, 12, 12);
	UIGrid->setVerticalSpacing(5);

	row += 2;
	column = 2;
	//火箭型号部分
	UIGrid->addWidget(rocketName, row++, column, 1, 3);
	UIGrid->addWidget(userInputRocketName, row++, column, 1, 3);
	UIGrid->addWidget(rocketDescript, row++, column, 1, 3);
	UIGrid->addWidget(userInputDescript, row++, column, 1, 3);
	//参数部分
	UIGrid->addWidget(paramRocket, row++, column, 1, 3);
	UIGrid->addWidget(paramRocketType, row++, column, 1, 3);
	UIGrid->addWidget(paramName, row++, column, 1, 3);
	UIGrid->addWidget(userInputParamName, row++, column, 1, 3);
	UIGrid->addWidget(paramType, row++, column, 1, 3);
	UIGrid->addWidget(userSelectParamType, row++, column, 1, 3);
	UIGrid->addWidget(paramUnit, row++, column, 1, 3);
	UIGrid->addWidget(userInputParamUnit, row++, column, 1, 3);
	UIGrid->addWidget(paramTable, row++, column, 1, 3);
	UIGrid->addWidget(paramTableType, row, column, 1, 3);
	UIGrid->addWidget(addParamTable, row++, column + 3, 1, 1);


	//设备部分
	UIGrid->addWidget(rocketType, row++, column, 1, 3);
	UIGrid->addWidget(userSelectRocketType, row++, column, 1, 3);
	UIGrid->addWidget(deviceName, row++, column, 1, 3);
	UIGrid->addWidget(userInputDevName, row++, column, 1, 3);
	UIGrid->addWidget(deviceType, row++, column, 1, 3);
	UIGrid->addWidget(userSelectDevType, row++, column, 1, 3);
	//指令部分
	UIGrid->addWidget(commandType, row++, column, 1, 3);
	UIGrid->addWidget(userSelectCmdType, row++, column, 1, 3);
	UIGrid->addWidget(commandName, row++, column, 1, 3);
	UIGrid->addWidget(userInputCmdName, row++, column, 1, 3);
	UIGrid->addWidget(cmdBackCmd, row++, column, 1, 3);
	UIGrid->addWidget(userSelectBackCmd, row++, column, 1, 3);
	UIGrid->addWidget(cmdTable, row++, column, 1, 3);
	UIGrid->addWidget(cmdTableType, row, column, 1, 3);
	UIGrid->addWidget(addCmdTable, row++, column + 3, 1, 1);

	UIGrid->setRowStretch(row, 3);

	QFrame* hframe2 = new QFrame;
	hframe2->setFrameShape(QFrame::HLine);
	hframe2->setFixedWidth(640);
	hframe2->setBackgroundRole(QPalette::Background);

	QVBoxLayout* lineLayout2 = new QVBoxLayout;
	lineLayout2->addWidget(hframe2);

	UIGrid->addLayout(lineLayout2, rowC - 2, 0, 1, columnC);

	UIGrid->addWidget(CancelBtn, rowC - 1, columnC - 2, 1, 1);
	UIGrid->addWidget(OkBtn, rowC - 1, columnC - 1, 1, 1);



	this->setLayout(UIGrid);
	this->setFixedSize(640, 640);
	this->setContentsMargins(0, 0, 0, 0);



	userInputDescript->setStyleSheet("QLineEdit{border:2px groove gray;border-radius:5px;padding:2px 4px;font: 12pt 微软雅黑;}");
	userInputRocketName->setStyleSheet("QLineEdit{border:2px groove gray;border-radius:5px;padding:2px 4px;font: 12pt 微软雅黑;}");
	this->setStyleSheet("*{background-color:rgba(255,255,255,1);padding:2px;}\
			QLineEdit{border:2px groove gray;border-radius:5px;padding:2px 4px;font: 12pt 微软雅黑;}\
			QLabel{font: 12pt 微软雅黑;}\
			QComboBox{border: 1px solid darkgray;font: 10pt Arial;selection - background - color: rgb(0, 170, 255);font: 12pt 微软雅黑; }");
	CancelBtn->setStyleSheet("\
				\
				QPushButton:hover{background-color:transparent;\
					border-image:url(:/DeviceManager/bt_suspension.png);\
					color:rgb(255,255,255);\
					font:12pt 微软雅黑;\
					border:0px;\
					}\
				QPushButton:pressed{\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_disable.png);\
					color:rgb(255,255,255);\
					font:12pt 微软雅黑;\
					border:0px;}\
				QPushButton{\
					height:30px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt 微软雅黑;\
					border:0px;\
					}\
		");
	OkBtn->setStyleSheet("\
				\
				QPushButton:hover{background-color:transparent;\
					border-image:url(:/DeviceManager/bt_suspension.png);\
					color:rgb(255,255,255);\
					font:12pt 微软雅黑;\
					border:0px;\
					}\
				QPushButton:pressed{\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_disable.png);\
					color:rgb(255,255,255);\
					font:12pt 微软雅黑;\
					border:0px;}\
				QPushButton{\
					height:30px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt 微软雅黑;\
					border:0px;\
					}\
		");
	//输入内容限制
	userInputRocketName->setValidator(new QRegExpValidator(QRegExp("\\S+")));
	userInputDescript->setValidator(new QRegExpValidator(QRegExp("\\S+")));
	userInputParamName->setValidator(new QRegExpValidator(QRegExp("\\S+")));
	userInputParamUnit->setValidator(new QRegExpValidator(QRegExp("\\S+")));
	userInputDevName->setValidator(new QRegExpValidator(QRegExp("\\S+")));
	userInputCmdName->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	windowIcon->setStyleSheet("image:url(:/icon/icon/bb.png);left:6px;height:25px;");
	windowCloseBtn->setStyleSheet("QPushButton{height:25px;image:url(:/DeviceManager/images/close_normal.png);image-position:right;background-color:rgba(255,255,255,1);border:0px;}\
			QPushButton:hover{image:url(:/DeviceManager/images/关闭-悬浮.png);}\
			QPushButton:pressed{image:url(:/DeviceManager/images/关闭-点击.png);}");

	//初始化数据
	//参数类型
	for (pair<int, string> ele : DeviceCommonVaries::getInstance()->paramIndex2Type)
	{
		userSelectParamType->addItem(QString::fromLocal8Bit(ele.second.c_str()), ele.first);
	}
	//设备
	for (pair<int, string> ele : DeviceCommonVaries::getInstance()->deviceIndex2Type)
	{
		userSelectDevType->addItem(QString::fromLocal8Bit(ele.second.c_str()), ele.first);
	}
	//指令
	DeviceDBConfigInfo::getInstance()->readRocketDB2UI();
	for (pair<int, vector<string>> ele : DeviceDBConfigInfo::getInstance()->rocketInfo)
	{
		userSelectRocketType->addItem(QString::fromStdString(ele.second[1]), ele.first);
	}
	/*for (pair<int, string> ele : DeviceCommonVaries::getInstance()->commandIndex2Type)
	{
		userSelectCmdType->addItem(QString::fromLocal8Bit(ele.second.c_str()), ele.first);
	}*/
	userSelectCmdType->addItem(QString("测发指令"), 1);

	QString qSqlString = QString("SELECT\
		command_info.id,\
		command_info.`name`,\
		command_info.prefix,\
		command_info.createTime\
		FROM\
		command_info\
		WHERE\
		command_info.type = %1 AND\
		command_info.rocket_id = %2")
		.arg(DeviceCommonVaries::getInstance()->commandType[string(QString("测发回令").toLocal8Bit())])
		.arg(AppCache::instance()->m_CurrentRocketType->m_id);
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
	userSelectBackCmd->addItem(QString("无回令"), 0);
	for (pair<int, vector<string>> ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
	{
		userSelectBackCmd->addItem(QString::fromStdString(ele.second[1]), ele.first);
	}

}

void AddRocketTypeWidget::setWindowName(QString name1) {
	windowTitle->setText(name1);
}

/**
	@brief 区别为那个窗口
	@param type -
**/
void AddRocketTypeWidget::setInfoWidget(int type) {
	currentDealType = type;
	switch (type)
	{
	case DeviceCommonVaries::ROCKET_WIDGET:

		rocketInfoDisplay(true);
		paramInfoDisplay(false);
		deviceInfoDisplay(false);
		commandInfoDisplay(false);
		break;
	case DeviceCommonVaries::PARAM_WIDGET:
		tableType = TableType::PARAM;
		rocketInfoDisplay(false);
		paramInfoDisplay(true);
		deviceInfoDisplay(false);
		commandInfoDisplay(false);
		break;
	case DeviceCommonVaries::DEVICE_WIDGET:
		rocketInfoDisplay(false);
		paramInfoDisplay(false);
		deviceInfoDisplay(true);
		commandInfoDisplay(false);
		break;
	case DeviceCommonVaries::COMMAND_WIDGET:
		tableType = TableType::COMMAND;
		rocketInfoDisplay(false);
		paramInfoDisplay(false);
		deviceInfoDisplay(false);
		commandInfoDisplay(true);
		break;
	default:
		break;
	}
}

/**
	@brief 点击编辑时将现有信息填充
	@param rocketId     -
	@param rocketName   -
	@param rocketDescrp -
**/
void AddRocketTypeWidget::setRocketInfo(int rocketId, QString rocketName, QString rocketDescrp) {
	rocketID = rocketId;
	userInputRocketName->setText(rocketName);
	userInputDescript->setText(rocketDescrp);

	if (rocketId != 0)
	{
		currentModule = DeviceCommonVaries::UPDATE_MODULE;
	}
}

/**
	@brief 点击编辑时将现有信息填充
	@param paramId   -
	@param paramName -
	@param paramType -
	@param paramUnit -
**/
void AddRocketTypeWidget::setParamInfo(int paramId, QString paramName, QString paramType, QString paramUnit, QString paramTableName) {
	paramID = paramId;

	paramRocketType->setText(QString::fromLocal8Bit(AppCache::instance()->m_CurrentRocketType->m_name.c_str()));
	userInputParamName->setText(paramName);
	userSelectParamType->setCurrentText(paramType);
	userInputParamUnit->setText(paramUnit);

	DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
					param_table_info.id,\
					param_table_info.`name`,\
					param_table_info.createTime,\
					param_table_info.lastUpdateTime\
					FROM\
					param_table_info WHERE\
					param_table_info.rocket_id = %1\
		").arg(AppCache::instance()->m_CurrentRocketType->m_id));
	paramTableType->clear();
	paramTableType->addItem("", -1);
	for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
	{

		paramTableType->addItem(QString::fromStdString(ele.second[1]), ele.first);
	}
	paramTableType->setCurrentText(paramTableName);
#ifdef IDENTIFY_ROCKET_//预留给需要根据火箭类型区别参数表的情况
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
		parameter_info.id,\
		param_table_info.id,\
		param_table_info.`name`,\
		parameter_info.createTime,\
		parameter_info.lastUpdateTime\
		FROM\
		param_table_info\
		INNER JOIN parameter_rocket_info ON parameter_rocket_info.param_table_id = param_table_info.id\
		INNER JOIN parameter_info ON parameter_info.id = parameter_rocket_info.parameter_id\
		WHERE\
		parameter_rocket_info.rocket_id = %1\
		").arg(AppCache::instance()->m_CurrentRocketType->m_id));
	paramTableType->clear();
	paramTableType->addItem("", -1);
	map<int, string> curRocketParamTypeTable;
	for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
	{
		if (curRocketParamTypeTable.count(atoi(ele.second[1].c_str())) > 0)
		{
			continue;
		}
		else
		{
			curRocketParamTypeTable[atoi(ele.second[1].c_str())] = ele.second[2];
		}

	}
	for (auto elem : curRocketParamTypeTable)
	{
		paramTableType->addItem(QString::fromStdString(elem.second), elem.first);
	}
#endif

	if (paramId != 0)
	{
		currentModule = DeviceCommonVaries::UPDATE_MODULE;
	}
}

/**
	@brief 点击编辑时将现有信息填充
	@param devId      -
	@param rocketType -
	@param devName    -
	@param devType    -
**/
void AddRocketTypeWidget::setDevInfo(int devId, QString rocketType, QString devName, QString devType) {
	deviceID = devId;
	auto aaa = QString::fromLocal8Bit(AppCache::instance()->m_CurrentRocketType->m_name.c_str());

	paramRocketType->setText(QString::fromLocal8Bit(AppCache::instance()->m_CurrentRocketType->m_name.c_str()));
	userSelectRocketType->setCurrentText(aaa);
	userSelectRocketType->setEnabled(false);
	userInputDevName->setText(devName);
	userSelectDevType->setCurrentText(devType);
	if (devId != 0)
	{
		currentModule = DeviceCommonVaries::UPDATE_MODULE;
	}
}

/**
	@brief 点击编辑时将现有信息填充
	@param cmdId      -
	@param rocketType -
	@param cmdType    -
	@param cmdName    -
	@param backCmd    -
**/
void AddRocketTypeWidget::setCommandInfo(int cmdId, QString rocketType, QString cmdType, QString cmdName, QString backCmd, QString cmdTable) {

	commandID = cmdId;

	paramRocketType->setText(QString::fromLocal8Bit(AppCache::instance()->m_CurrentRocketType->m_name.c_str()));
	userSelectRocketType->setCurrentText(QString::fromLocal8Bit(AppCache::instance()->m_CurrentRocketType->m_name.c_str()));
	userSelectCmdType->setCurrentText(cmdType);
	userInputCmdName->setText(cmdName);


	userSelectBackCmd->clear();
	userSelectBackCmd->addItem("无回令", 0);
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
		command_info.id,\
		command_info.`name`,\
		command_info.prefix,\
		command_info.createTime\
		FROM\
		command_info\
		WHERE\
		command_info.type = %1 AND\
		command_info.rocket_id = %2").arg(2).arg(AppCache::instance()->m_CurrentRocketType->m_id));
	for (auto elem : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
	{
		userSelectBackCmd->addItem(QString::fromStdString(elem.second[1]), elem.first);
	}
	userSelectBackCmd->setCurrentText(backCmd);

	DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
					command_table_info.id,\
					command_table_info.`name`,\
					command_table_info.createTime,\
					command_table_info.lastUpdateTime\
					FROM\
					command_table_info WHERE\
					command_table_info.rocket_id = %1")
		.arg(AppCache::instance()->m_CurrentRocketType->m_id));
	cmdTableType->clear();
	cmdTableType->addItem("", -1);
	for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
	{
		cmdTableType->addItem(QString::fromStdString(ele.second[1]), ele.first);
	}
	cmdTableType->setCurrentText(cmdTable);
#ifdef IDENTIFY_ROCKET_////预留给需要根据火箭类型区别参数表的情况
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
		command_info.id,\
		command_table_info.id,\
		command_table_info.`name`,\
		command_table_info.createTime,\
		command_table_info.lastUpdateTime\
		FROM\
		command_table_info\
		INNER JOIN command_commandtable_info ON command_commandtable_info.command_table_id = command_table_info.id\
		INNER JOIN command_info ON command_info.id = command_commandtable_info.command_id\
		WHERE\
		command_info.rocket_id = %1").arg(AppCache::instance()->m_CurrentRocketType->m_id));
	cmdTableType->clear();
	cmdTableType->addItem("", -1);
	map<int, string> curRocketCmdTypeTable;
	for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
	{
		if (curRocketCmdTypeTable.count(atoi(ele.second[1].c_str())) > 0)
		{
			continue;
		}
		else
		{
			curRocketCmdTypeTable[atoi(ele.second[1].c_str())] = ele.second[2];
		}
	}
	for (auto elem : curRocketCmdTypeTable)
	{
		cmdTableType->addItem(QString::fromStdString(elem.second), elem.first);
	}
#endif
	if (cmdId != 0)
	{
		currentModule = DeviceCommonVaries::UPDATE_MODULE;
	}
}



/**
	@brief 显示火箭型号配置
	@param flag -
**/
void AddRocketTypeWidget::rocketInfoDisplay(bool flag) {

	if (flag)
	{
		rocketName->show();
		userInputRocketName->show();
		rocketDescript->show();
		userInputDescript->show();

	}
	else
	{
		rocketName->hide();
		userInputRocketName->hide();
		rocketDescript->hide();
		userInputDescript->hide();
	}
}
/**
	@brief 显示参数配置
	@param flag -
**/
void AddRocketTypeWidget::paramInfoDisplay(bool flag) {

	if (flag)
	{

		paramRocket->show();
		paramRocketType->show();
		paramName->show();
		userInputParamName->show();
		paramType->show();
		userSelectParamType->show();
		paramUnit->show();
		userInputParamUnit->show();
		paramTable->show();
		paramTableType->show();
		addParamTable->show();
	}
	else
	{
		paramRocket->hide();
		paramRocketType->hide();
		paramName->hide();
		userInputParamName->hide();
		paramType->hide();
		userSelectParamType->hide();
		paramUnit->hide();
		userInputParamUnit->hide();
		paramTable->hide();
		paramTableType->hide();
		addParamTable->hide();
	}
}
/**
	@brief 显示设备配置
	@param flag -
**/
void AddRocketTypeWidget::deviceInfoDisplay(bool flag) {

	if (flag)
	{
		paramRocket->show();
		paramRocketType->show();
		rocketType->hide();
		userSelectRocketType->hide();
		deviceName->show();
		userInputDevName->show();
		deviceType->show();
		userSelectDevType->show();
	}
	else
	{
		/*paramRocket->hide();
		paramRocketType->hide();*/
		rocketType->hide();
		userSelectRocketType->hide();
		deviceName->hide();
		userInputDevName->hide();
		deviceType->hide();
		userSelectDevType->hide();
	}
}
/**
	@brief 显示火箭指令配置
	@param flag -
**/
void AddRocketTypeWidget::commandInfoDisplay(bool flag) {

	if (flag)
	{
		/*rocketType->show();
		userSelectRocketType->show();*/
		paramRocket->show();
		paramRocketType->show();
		commandType->show();
		userSelectCmdType->show();
		commandName->show();
		userInputCmdName->show();
		cmdBackCmd->hide();
		userSelectBackCmd->hide();
		cmdTable->show();
		cmdTableType->show();
		addCmdTable->show();
	}
	else
	{
		/*rocketType->hide();
		userSelectRocketType->hide();*/
		/*paramRocket->hide();
		paramRocketType->hide();*/
		commandType->hide();
		userSelectCmdType->hide();
		commandName->hide();
		userInputCmdName->hide();
		cmdBackCmd->hide();
		userSelectBackCmd->hide();
		cmdTable->hide();
		cmdTableType->hide();
		addCmdTable->hide();
	}
}

/**
	@brief
	@param wid -
**/
void AddRocketTypeWidget::opRocketInfo(DeviceCommonVaries::DeviceModule wid) {

	if (userInputRocketName->text().isEmpty() | userInputDescript->text().isEmpty())
	{
		QMessageBox::warning(this, QString("警告"), QString("火箭型号或描述不能为空"));
		return;
	}
	//检查表中是否有同名数据
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
		rocket_info.id,\
		rocket_info.createTime,\
		rocket_info.lastUpdateTime\
		FROM\
		rocket_info\
		WHERE\
		rocket_info.`name` = '%1'").arg(userInputRocketName->text()));


	if (wid == DeviceCommonVaries::ADD_MODULE)
	{
		if (DeviceDBConfigInfo::getInstance()->customReadInfoMap.size() != 0)
		{
			QMessageBox::warning(this, QString("警告"), QString("火箭型号已存在"));
			return;
		}
		DeviceDBConfigInfo::getInstance()->rocketConfigOp2DB(userInputRocketName->text(), userInputDescript->text());
	}
	else
	{
		DeviceDBConfigInfo::getInstance()->updateRocketInfo2DB(rocketID, userInputRocketName->text(), userInputDescript->text());
	}
	instance->close();
	emit updateRocketInfos();
}

/**
	@brief 参数
	@param wid -
**/
void AddRocketTypeWidget::opParamInfo(DeviceCommonVaries::DeviceModule wid) {

	if (userInputParamName->text().isEmpty() | userInputParamUnit->text().isEmpty())
	{
		QMessageBox::warning(this, QString("警告"), QString("参数名称或单位不能为空"));
		return;
	}

	DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
		parameter_info.id,\
		parameter_info.`name`,\
		parameter_info.createTime,\
		parameter_info.lastUpdateTime\
		FROM\
		parameter_info\
		INNER JOIN parameter_rocket_info ON parameter_rocket_info.parameter_id = parameter_info.id\
		WHERE\
		parameter_rocket_info.rocket_id = %1 AND\
		parameter_info.`name` = '%2'")
		.arg(AppCache::instance()->m_CurrentRocketType->m_id).arg(userInputParamName->text()));

	if (wid == DeviceCommonVaries::ADD_MODULE)
	{
		if (DeviceDBConfigInfo::getInstance()->customReadInfoMap.size() != 0)
		{
			QMessageBox::warning(this, QString("警告"), QString("该型号下参数已存在"));
			return;
		}
		/*DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
			parameter_info.id,\
			parameter_info.`name`,\
			parameter_info.createTime,\
			parameter_info.lastUpdateTime\
			FROM\
			parameter_info\
			WHERE\
			parameter_info.`name` = '%1'").arg(userInputParamName->text()));
		if (DeviceDBConfigInfo::getInstance()->customReadInfoMap.size() != 0)
		{
			QMessageBox::warning(this, QString("警告"), QString("数据表中已经存在该同名参数"));
			return;
		}*/
		DeviceDBConfigInfo::getInstance()->paramConfigOp2DB(userInputParamName->text(), userSelectParamType->currentData().toInt(), userInputParamUnit->text());
		//写参数表
		DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
			parameter_info.id,\
			parameter_info.`name`,\
			parameter_info.createTime,\
			parameter_info.lastUpdateTime\
			FROM\
			parameter_info\
			WHERE\
			parameter_info.`name` = '%1'").arg(userInputParamName->text()));
		int paramID1 = -1;
		for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
		{
			paramID1 = ele.first;
		}
		if (paramID1 == -1)
		{
			return;
		}

		QString qSqlString = QString("INSERT INTO `simulatedtraining`.`parameter_rocket_info`(`parameter_id`, `rocket_id`, `param_table_id`) VALUES (%1, %2, %3)").arg(paramID1).arg(AppCache::instance()->m_CurrentRocketType->m_id).arg(paramTableType->currentData().toInt());
		DeviceDBConfigInfo::getInstance()->customRunSql(qSqlString);
	}
	else
	{
		DeviceDBConfigInfo::getInstance()->updateParamInfo2DB(paramID, userInputParamName->text(), userSelectParamType->currentData().toInt(), userInputParamUnit->text());
		//改参数表
		QString qSqlString = QString("UPDATE `simulatedtraining`.`parameter_rocket_info` SET `param_table_id` = %1 WHERE `parameter_rocket_info`.`parameter_id`=%2").arg(paramTableType->currentData().toInt()).arg(paramID);
		DeviceDBConfigInfo::getInstance()->customRunSql(qSqlString);
	}
	instance->close();
	emit updateParamInfos();
}

/**
	@brief 操作设备
	@param wid -
**/
void AddRocketTypeWidget::opDeviceInfo(DeviceCommonVaries::DeviceModule wid) {

	if (userInputDevName->text().isEmpty())
	{
		QMessageBox::warning(this, QString("警告"), QString("设备名称不能为空"));
		return;
	}
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
		device_info.id,\
		device_info.createTime,\
		device_info.lastUpdateTime\
		FROM\
		device_info\
		WHERE\
		device_info.rocket_id = %1\
		AND device_info.`name` = '%2'").arg(AppCache::instance()->m_CurrentRocketType->m_id).arg(userInputDevName->text()));


	if (wid == DeviceCommonVaries::ADD_MODULE)
	{
		if (DeviceDBConfigInfo::getInstance()->customReadInfoMap.size() != 0)
		{
			QMessageBox::warning(this, QString("警告"), QString("设备已存在"));
			return;
		}
		DeviceDBConfigInfo::getInstance()->deviceConfigOp2DB(AppCache::instance()->m_CurrentRocketType->m_id, userInputDevName->text(), userSelectDevType->currentData().toInt());

	}
	else
	{
		DeviceDBConfigInfo::getInstance()->updateDeviceInfo2DB(deviceID, AppCache::instance()->m_CurrentRocketType->m_id, userInputDevName->text(), userSelectDevType->currentData().toInt());

	}
	instance->close();
	emit updateDeviceInfos();
}

/**
	@brief
	@param wid -
**/
void AddRocketTypeWidget::opCommandInfo(DeviceCommonVaries::DeviceModule wid) {


	if (userInputCmdName->text().isEmpty())
	{
		QMessageBox::warning(this, QString("警告"), QString("指令名称不能为空"));
		return;
	}
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
		command_info.id,\
		command_info.createTime,\
		command_info.lastUpdateTime\
		FROM\
		command_info\
		WHERE\
		command_info.rocket_id = %1 AND\
		command_info.`name` = '%2'").arg(AppCache::instance()->m_CurrentRocketType->m_id).arg(userInputCmdName->text()));

	int cmdcode = 1;
	int backCmdCode = 1;

	QVector<int> savedCode;
	if (wid == DeviceCommonVaries::ADD_MODULE)
	{
		//查火箭型号下指令是否已存在
		if (DeviceDBConfigInfo::getInstance()->customReadInfoMap.size() != 0) {
			QMessageBox::warning(this, QString("警告"), QString("该火箭型号下指令已存在"));
			return;
		}
		//同一火箭型号下指令code唯一
		DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
			command_info.id,\
			command_info.`code`,\
			command_info.createTime,\
			command_info.lastUpdateTime\
			FROM\
			command_info\
			WHERE\
			command_info.rocket_id = %1").arg(AppCache::instance()->m_CurrentRocketType->m_id));

		for (pair<int, vector<string>> eachele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
		{
			savedCode.push_back(atoi(eachele.second[1].c_str()));
		}
		int findCount = 0;
		for (int i = 0; i < 0xffff; i++)
		{
			if (savedCode.count(i) == 0 && findCount == 0)
			{
				cmdcode = i;
				findCount++;
				continue;
			}
			if (savedCode.count(i) == 0 && findCount == 1)
			{
				backCmdCode = i;
				break;
			}
		}
		QString backCmdName = userInputCmdName->text() + QString("回令");
		DeviceDBConfigInfo::getInstance()->commandConfigOp2DB(backCmdName, AppCache::instance()->m_CurrentRocketType->m_id, 0, backCmdCode, 2, 0x55aa);
		//获取当前新增指令回令的ID
		DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
			command_info.id,\
			command_info.createTime,\
			command_info.lastUpdateTime\
			FROM\
			command_info\
			WHERE\
			command_info.rocket_id = %1 AND\
			command_info.`name` = '%2'").arg(AppCache::instance()->m_CurrentRocketType->m_id).arg(backCmdName));
		int backCmdID;
		for (auto elem : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
		{
			backCmdID = elem.first;
		}
		DeviceDBConfigInfo::getInstance()->commandConfigOp2DB(userInputCmdName->text(), AppCache::instance()->m_CurrentRocketType->m_id, backCmdID, cmdcode, userSelectCmdType->currentData().toInt(), 0x55aa);

		//获取当前新增指令的ID
		DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
			command_info.id,\
			command_info.createTime,\
			command_info.lastUpdateTime\
			FROM\
			command_info\
			WHERE\
			command_info.rocket_id = %1 AND\
			command_info.`name` = '%2'").arg(AppCache::instance()->m_CurrentRocketType->m_id).arg(userInputCmdName->text()));
		int cmdIID;
		for (auto elem : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
		{
			cmdIID = elem.first;
		}
		DeviceDBConfigInfo::getInstance()->customRunSql(QString("INSERT INTO `simulatedtraining`.`command_commandtable_info`( `command_id`, `command_table_id`) VALUES ( %1, %2)")
			.arg(cmdIID).arg(cmdTableType->currentData().toInt()));

	}
	else
	{
		DeviceDBConfigInfo::getInstance()->readCommandDB2UI();
		cmdcode = atoi(DeviceDBConfigInfo::getInstance()->commandInfo[commandID][4].c_str());
		DeviceDBConfigInfo::getInstance()->updateCommandInfo2DB(commandID, userInputCmdName->text(), AppCache::instance()->m_CurrentRocketType->m_id, userSelectBackCmd->currentData().toInt(), cmdcode, userSelectCmdType->currentData().toInt(), 0x55aa);
		//先查询表中是否存在
		//若存在则修改，不存在则添加
		DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
			command_commandtable_info.id,\
			command_commandtable_info.command_id,\
			command_commandtable_info.command_table_id,\
			command_commandtable_info.createTime,\
			command_commandtable_info.lastUpdateTime\
			FROM\
			command_commandtable_info\
			INNER JOIN command_info ON command_commandtable_info.command_id = command_info.id\
			WHERE\
			command_info.rocket_id = %1 AND\
			command_commandtable_info.command_id = %2")
			.arg(AppCache::instance()->m_CurrentRocketType->m_id)
			.arg(commandID));
		int queryRet = DeviceDBConfigInfo::getInstance()->customReadInfoMap.size();
		if (queryRet != 0)
		{
			DeviceDBConfigInfo::getInstance()->customRunSql(QString("UPDATE `simulatedtraining`.`command_commandtable_info` SET  `command_table_id` = %1 WHERE command_commandtable_info.command_id=%2")
				.arg(cmdTableType->currentData().toInt()).arg(commandID));
		}
		else
		{
			DeviceDBConfigInfo::getInstance()->customRunSql(QString("INSERT INTO `simulatedtraining`.`command_commandtable_info`( `command_id`, `command_table_id`) VALUES ( %1, %2)")
				.arg(commandID).arg(cmdTableType->currentData().toInt()));

		}
	}
	instance->close();
	emit updateCommandInfos();
}

/**
	@brief 界面配置
**/
void AddRocketTypeWidget::widgetConfig() {

}

/**
	@brief 初始化信号与槽
**/
void AddRocketTypeWidget::initConnect() {
	connect(itemDelegate, &ItemDelegate::deleteItem, this, [=](const QModelIndex& index) {
		//区分是那个类型表
		if (tableType == TableType::PARAM)
		{
			//删除数据表
			QString deleteString = QString("DELETE FROM `simulatedtraining`.`param_table_info` WHERE `name` = '%1'").arg(paramTableType->itemText(index.row()));
			DeviceDBConfigInfo::getInstance()->customRunSql(deleteString);
			paramTableType->removeItem(index.row());
		}
		else
		{
			QString deleteString = QString("DELETE FROM `simulatedtraining`.`command_table_info` WHERE `name` = '%1'").arg(cmdTableType->itemText(index.row()));
			DeviceDBConfigInfo::getInstance()->customRunSql(deleteString);
			cmdTableType->removeItem(index.row());
		}
		});

	connect(windowCloseBtn, &QPushButton::clicked, this, &AddRocketTypeWidget::clickWindowClose);
	connect(CancelBtn, &QPushButton::clicked, this, &AddRocketTypeWidget::clickRocketTypeCancel);
	connect(OkBtn, &QPushButton::clicked, this, &AddRocketTypeWidget::clickRocketTypeOk);

	connect(addParamTable, &QPushButton::clicked, this, [=]() {
		paramTableType->setEditable(true);
		});
	connect(addCmdTable, &QPushButton::clicked, this, [=]() {
		cmdTableType->setEditable(true);
		});
}

/**
	@brief 取消按钮槽函数
**/
void AddRocketTypeWidget::clickRocketTypeCancel() {

	currentModule = DeviceCommonVaries::ADD_MODULE;
	instance->close();
}

/**
	@brief 确定按钮槽函数  区分不同窗口
**/
void AddRocketTypeWidget::clickRocketTypeOk() {


	//写入数据表
	switch (currentDealType)
	{
	case DeviceCommonVaries::ROCKET_WIDGET:
		opRocketInfo(currentModule);
		break;
	case DeviceCommonVaries::PARAM_WIDGET:
		opParamInfo(currentModule);
		break;
	case DeviceCommonVaries::DEVICE_WIDGET:
		opDeviceInfo(currentModule);
		break;
	case DeviceCommonVaries::COMMAND_WIDGET://code自动生成
		opCommandInfo(currentModule);
		break;
	default:
		break;
	}

	currentModule = DeviceCommonVaries::ADD_MODULE;
	//instance->close();
}

/**
	@brief 点击关闭按钮槽函数
**/
void AddRocketTypeWidget::clickWindowClose() {

	currentModule = DeviceCommonVaries::ADD_MODULE;
	instance->close();
}



/**
	@brief 鼠标按下操作
	@param event -
**/
void AddRocketTypeWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = true;
		mLastMousePosition = event->globalPos();
	}
}

/**
	@brief 鼠标移动操作
	@param event -
**/
void AddRocketTypeWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons().testFlag(Qt::LeftButton) && mMoving)
	{
		setCursor(Qt::OpenHandCursor);
		this->move(this->pos() + (event->globalPos() - mLastMousePosition));
		mLastMousePosition = event->globalPos();
	}
}

/**
	@brief 鼠标释放操作
	@param event -
**/
void AddRocketTypeWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = false;
		setCursor(Qt::ArrowCursor);
	}
}



/**
	@brief 参数表的增加
	@param  watched -
	@param  event   -
	@retval         -
**/
bool AddRocketTypeWidget::eventFilter(QObject* watched, QEvent* event) {
	if (event->type() == QEvent::KeyPress)
	{

		if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_Return)
		{
			if (watched == paramTableType)
			{
				qDebug() << paramTableType->currentText();
				//先检查是否已有或空
				if (paramTableType->currentText().isEmpty())
				{
					QMessageBox::warning(this, QString("警告"), QString("参数表名不能为空"));
					return true;
				}
				DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
					param_table_info.id,\
					param_table_info.`name`,\
					param_table_info.createTime,\
					param_table_info.lastUpdateTime\
					FROM\
					param_table_info WHERE\
					param_table_info.rocket_id = %1")
					.arg(AppCache::instance()->m_CurrentRocketType->m_id));
				for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
				{
					if (QString::fromStdString(ele.second[1]) == paramTableType->currentText())
					{
						QMessageBox::warning(this, QString("警告"), QString("已存在同名参数表"));
						return true;
					}
				}
				DeviceDBConfigInfo::getInstance()->customRunSql(QString("INSERT INTO `simulatedtraining`.`param_table_info`(`rocket_id`, `name`) VALUES (%1, '%2')")
					.arg(AppCache::instance()->m_CurrentRocketType->m_id).arg(paramTableType->currentText()));
				DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
					param_table_info.id,\
					param_table_info.`name`,\
					param_table_info.createTime,\
					param_table_info.lastUpdateTime\
					FROM\
					param_table_info WHERE\
					param_table_info.rocket_id = %1")
					.arg(AppCache::instance()->m_CurrentRocketType->m_id));
				paramTableType->clear();
				for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
				{

					paramTableType->addItem(QString::fromStdString(ele.second[1]), ele.first);
				}
				paramTableType->setEditable(false);
				return true;
			}
			if (watched == cmdTableType)
			{
				qDebug() << cmdTableType->currentText();
				//先检查是否已有或空
				if (cmdTableType->currentText().isEmpty())
				{
					QMessageBox::warning(this, QString("警告"), QString("指令表名不能为空"));
					return true;
				}
				DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
					command_table_info.id,\
					command_table_info.`name`,\
					command_table_info.createTime,\
					command_table_info.lastUpdateTime\
					FROM\
					command_table_info WHERE\
					command_table_info.rocket_id = %1")
					.arg(AppCache::instance()->m_CurrentRocketType->m_id));
				for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
				{
					if (QString::fromStdString(ele.second[1]) == cmdTableType->currentText())
					{
						QMessageBox::warning(this, QString("警告"), QString("已存在同名指令表"));
						return true;
					}
				}
				DeviceDBConfigInfo::getInstance()->customRunSql(QString("INSERT INTO `simulatedtraining`.`command_table_info`(`rocket_id`, `name`) VALUES (%1, '%2')")
					.arg(AppCache::instance()->m_CurrentRocketType->m_id).arg(cmdTableType->currentText()));

				DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
					command_table_info.id,\
					command_table_info.`name`,\
					command_table_info.createTime,\
					command_table_info.lastUpdateTime\
					FROM\
					command_table_info WHERE\
					command_table_info.rocket_id = %1")
					.arg(AppCache::instance()->m_CurrentRocketType->m_id));
				cmdTableType->clear();
				for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
				{

					cmdTableType->addItem(QString::fromStdString(ele.second[1]), ele.first);
				}
				cmdTableType->setEditable(false);
				return true;
			}
		}
	}
	return QWidget::eventFilter(watched, event);
}