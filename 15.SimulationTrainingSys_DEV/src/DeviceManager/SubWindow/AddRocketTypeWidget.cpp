#include "AddRocketTypeWidget.h"

AddRocketTypeWidget* AddRocketTypeWidget::instance = nullptr;

AddRocketTypeWidget::AddRocketTypeWidget(QWidget* parent)
	: QWidget(parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

	currentModule = DeviceCommonVaries::ADD_MODULE;

	windowName = "��������ͺ�";
	InitUILayout();

	connect(windowCloseBtn, &QPushButton::clicked, this, &AddRocketTypeWidget::clickWindowClose);
	connect(CancelBtn, &QPushButton::clicked, this, &AddRocketTypeWidget::clickRocketTypeCancel);
	connect(OkBtn, &QPushButton::clicked, this, &AddRocketTypeWidget::clickRocketTypeOk);

}

AddRocketTypeWidget::~AddRocketTypeWidget()
{}

/**
	@brief ��ʼ������
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
	//����ͺŲ���
	rocketName = new QLabel(QString("����ͺ�����:"));
	userInputRocketName = new QLineEdit;
	userInputRocketName->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	rocketDescript = new QLabel(QString("����:"));
	userInputDescript = new QLineEdit;
	userInputDescript->setValidator(new QRegExpValidator(QRegExp("\\S+")));
	//��������
	paramName = new QLabel(QString("��������:"));
	userInputParamName = new QLineEdit;
	paramType = new QLabel(QString("��������:"));
	userSelectParamType = new QComboBox;
	paramUnit = new QLabel(QString("������λ:"));
	userInputParamUnit = new QLineEdit;
	//�豸����
	rocketType = new QLabel(QString("����ͺ�:"));
	userSelectRocketType = new QComboBox;
	deviceName = new QLabel(QString("�豸����:"));
	userInputDevName = new QLineEdit;
	deviceType = new QLabel(QString("�豸����:"));
	userSelectDevType = new QComboBox;
	//ָ���
	commandType = new QLabel(QString("ָ������:"));
	userSelectCmdType = new QComboBox;
	commandName = new QLabel(QString("ָ������:"));
	userInputCmdName = new QLineEdit;
	cmdBackCmd = new QLabel(QString("�ⷢ����:"));
	userSelectBackCmd = new QComboBox;

	//
	CancelBtn = new QPushButton(QString("ȡ��"));
	OkBtn = new QPushButton(QString("ȷ��"));

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
	//����ͺŲ���
	UIGrid->addWidget(rocketName, row++, column, 1, 3);
	UIGrid->addWidget(userInputRocketName, row++, column, 1, 3);
	UIGrid->addWidget(rocketDescript, row++, column, 1, 3);
	UIGrid->addWidget(userInputDescript, row++, column, 1, 3);
	//��������
	UIGrid->addWidget(paramName, row++, column, 1, 3);
	UIGrid->addWidget(userInputParamName, row++, column, 1, 3);
	UIGrid->addWidget(paramType, row++, column, 1, 3);
	UIGrid->addWidget(userSelectParamType, row++, column, 1, 3);
	UIGrid->addWidget(paramUnit, row++, column, 1, 3);
	UIGrid->addWidget(userInputParamUnit, row++, column, 1, 3);
	//�豸����
	UIGrid->addWidget(rocketType, row++, column, 1, 3);
	UIGrid->addWidget(userSelectRocketType, row++, column, 1, 3);
	UIGrid->addWidget(deviceName, row++, column, 1, 3);
	UIGrid->addWidget(userInputDevName, row++, column, 1, 3);
	UIGrid->addWidget(deviceType, row++, column, 1, 3);
	UIGrid->addWidget(userSelectDevType, row++, column, 1, 3);
	//ָ���
	UIGrid->addWidget(commandType, row++, column, 1, 3);
	UIGrid->addWidget(userSelectCmdType, row++, column, 1, 3);
	UIGrid->addWidget(commandName, row++, column, 1, 3);
	UIGrid->addWidget(userInputCmdName, row++, column, 1, 3);
	UIGrid->addWidget(cmdBackCmd, row++, column, 1, 3);
	UIGrid->addWidget(userSelectBackCmd, row++, column, 1, 3);

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



	userInputDescript->setStyleSheet("QLineEdit{border:2px groove gray;border-radius:5px;padding:2px 4px;font: 12pt ΢���ź�;}");
	userInputRocketName->setStyleSheet("QLineEdit{border:2px groove gray;border-radius:5px;padding:2px 4px;font: 12pt ΢���ź�;}");
	this->setStyleSheet("*{background-color:rgba(255,255,255,1);padding:2px;}\
			QLineEdit{border:2px groove gray;border-radius:5px;padding:2px 4px;font: 12pt ΢���ź�;}\
			QLabel{font: 12pt ΢���ź�;}\
			QComboBox{border: 1px solid darkgray;font: 10pt Arial;selection - background - color: rgb(0, 170, 255);font: 12pt ΢���ź�; }");
	CancelBtn->setStyleSheet("\
				\
				QPushButton:hover{background-color:transparent;\
					border-image:url(:/DeviceManager/bt_suspension.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;\
					}\
				QPushButton:pressed{\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_disable.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;}\
				QPushButton{\
					height:30px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;\
					}\
		");
	OkBtn->setStyleSheet("\
				\
				QPushButton:hover{background-color:transparent;\
					border-image:url(:/DeviceManager/bt_suspension.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;\
					}\
				QPushButton:pressed{\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_disable.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;}\
				QPushButton{\
					height:30px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;\
					}\
		");
	//������������
	userInputRocketName->setValidator(new QRegExpValidator(QRegExp("\\S+")));
	userInputDescript->setValidator(new QRegExpValidator(QRegExp("\\S+")));
	userInputParamName->setValidator(new QRegExpValidator(QRegExp("\\S+")));
	userInputParamUnit->setValidator(new QRegExpValidator(QRegExp("\\S+")));
	userInputDevName->setValidator(new QRegExpValidator(QRegExp("\\S+")));
	userInputCmdName->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	windowIcon->setStyleSheet("image:url(:/icon/icon/bb.png);left:6px;height:25px;");
	windowCloseBtn->setStyleSheet("QPushButton{height:25px;image:url(:/DeviceManager/images/close_normal.png);image-position:right;background-color:rgba(255,255,255,1);border:0px;}\
			QPushButton:hover{image:url(:/DeviceManager/images/�ر�-����.png);}\
			QPushButton:pressed{image:url(:/DeviceManager/images/�ر�-���.png);}");

	//��ʼ������
	//��������
	for (pair<int, string> ele : DeviceCommonVaries::getInstance()->paramIndex2Type)
	{
		userSelectParamType->addItem(QString::fromLocal8Bit(ele.second.c_str()), ele.first);
	}
	//�豸
	for (pair<int, string> ele : DeviceCommonVaries::getInstance()->deviceIndex2Type)
	{
		userSelectDevType->addItem(QString::fromLocal8Bit(ele.second.c_str()), ele.first);
	}
	//ָ��
	DeviceDBConfigInfo::getInstance()->readRocketDB2UI();
	for (pair<int, vector<string>> ele : DeviceDBConfigInfo::getInstance()->rocketInfo)
	{
		userSelectRocketType->addItem(QString::fromStdString(ele.second[1]), ele.first);
	}
	for (pair<int, string> ele : DeviceCommonVaries::getInstance()->commandIndex2Type)
	{
		userSelectCmdType->addItem(QString::fromLocal8Bit(ele.second.c_str()), ele.first);
	}


	QString qSqlString = QString("SELECT\
		command_info.id,\
		command_info.`name`,\
		command_info.prefix,\
		command_info.createTime\
		FROM\
		command_info\
		WHERE\
		command_info.type = %1").arg(DeviceCommonVaries::getInstance()->commandType[string(QString("�ⷢ����").toLocal8Bit())]);
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
	for (pair<int, vector<string>> ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
	{
		userSelectBackCmd->addItem(QString::fromStdString(ele.second[1]), ele.first);
	}
	userSelectBackCmd->addItem(QString("�޻���"), 0);

}

void AddRocketTypeWidget::setWindowName(QString name1) {
	windowTitle->setText(name1);
}

/**
	@brief ����Ϊ�Ǹ�����
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
	@brief ����༭ʱ��������Ϣ���
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
	@brief ����༭ʱ��������Ϣ���
	@param paramId   -
	@param paramName -
	@param paramType -
	@param paramUnit -
**/
void AddRocketTypeWidget::setParamInfo(int paramId, QString paramName, QString paramType, QString paramUnit) {
	paramID = paramId;
	userInputParamName->setText(paramName);
	userSelectParamType->setCurrentText(paramType);
	userInputParamUnit->setText(paramUnit);
	if (paramId != 0)
	{
		currentModule = DeviceCommonVaries::UPDATE_MODULE;
	}
}

/**
	@brief ����༭ʱ��������Ϣ���
	@param devId      -
	@param rocketType -
	@param devName    -
	@param devType    -
**/
void AddRocketTypeWidget::setDevInfo(int devId, QString rocketType, QString devName, QString devType) {
	deviceID = devId;
	userSelectRocketType->setCurrentText(rocketType);
	userInputDevName->setText(devName);
	userSelectDevType->setCurrentText(devType);
	if (devId != 0)
	{
		currentModule = DeviceCommonVaries::UPDATE_MODULE;
	}
}

/**
	@brief ����༭ʱ��������Ϣ���
	@param cmdId      -
	@param rocketType -
	@param cmdType    -
	@param cmdName    -
	@param backCmd    -
**/
void AddRocketTypeWidget::setCommandInfo(int cmdId, QString rocketType, QString cmdType, QString cmdName, QString backCmd) {

	commandID = cmdId;
	userSelectRocketType->setCurrentText(rocketType);
	userSelectCmdType->setCurrentText(cmdType);
	userInputCmdName->setText(cmdName);
	userSelectBackCmd->setCurrentText(backCmd);
	if (cmdId != 0)
	{
		currentModule = DeviceCommonVaries::UPDATE_MODULE;
	}
}



/**
	@brief ��ʾ����ͺ�����
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
	@brief ��ʾ��������
	@param flag -
**/
void AddRocketTypeWidget::paramInfoDisplay(bool flag) {
	if (flag)
	{
		paramName->show();
		userInputParamName->show();
		paramType->show();
		userSelectParamType->show();
		paramUnit->show();
		userInputParamUnit->show();
	}
	else
	{
		paramName->hide();
		userInputParamName->hide();
		paramType->hide();
		userSelectParamType->hide();
		paramUnit->hide();
		userInputParamUnit->hide();
	}
}
/**
	@brief ��ʾ�豸����
	@param flag -
**/
void AddRocketTypeWidget::deviceInfoDisplay(bool flag) {
	if (flag)
	{
		rocketType->show();
		userSelectRocketType->show();
		deviceName->show();
		userInputDevName->show();
		deviceType->show();
		userSelectDevType->show();
	}
	else
	{
		rocketType->hide();
		userSelectRocketType->hide();
		deviceName->hide();
		userInputDevName->hide();
		deviceType->hide();
		userSelectDevType->hide();
	}
}
/**
	@brief ��ʾ���ָ������
	@param flag -
**/
void AddRocketTypeWidget::commandInfoDisplay(bool flag) {
	if (flag)
	{
		rocketType->show();
		userSelectRocketType->show();
		commandType->show();
		userSelectCmdType->show();
		commandName->show();
		userInputCmdName->show();
		cmdBackCmd->show();
		userSelectBackCmd->show();
	}
	else
	{
		rocketType->hide();
		userSelectRocketType->hide();
		commandType->hide();
		userSelectCmdType->hide();
		commandName->hide();
		userInputCmdName->hide();
		cmdBackCmd->hide();
		userSelectBackCmd->hide();
	}
}

/**
	@brief
	@param wid -
**/
void AddRocketTypeWidget::opRocketInfo(DeviceCommonVaries::DeviceModule wid) {

	if (userInputRocketName->text().isEmpty() | userInputDescript->text().isEmpty())
	{
		QMessageBox::warning(this, QString("����"), QString("����ͺŻ���������Ϊ��"));
		return;
	}
	if (wid == DeviceCommonVaries::ADD_MODULE)
	{
		DeviceDBConfigInfo::getInstance()->rocketConfigOp2DB(userInputRocketName->text(), userInputDescript->text());
	}
	else
	{
		DeviceDBConfigInfo::getInstance()->updateRocketInfo2DB(rocketID, userInputRocketName->text(), userInputDescript->text());
	}
	emit updateRocketInfos();
}

/**
	@brief
	@param wid -
**/
void AddRocketTypeWidget::opParamInfo(DeviceCommonVaries::DeviceModule wid) {

	if (userInputParamName->text().isEmpty() | userInputParamUnit->text().isEmpty())
	{
		QMessageBox::warning(this, QString("����"), QString("�������ƻ�λ����Ϊ��"));
		return;
	}
	if (wid == DeviceCommonVaries::ADD_MODULE)
	{
		DeviceDBConfigInfo::getInstance()->paramConfigOp2DB(userInputParamName->text(), userSelectParamType->currentData().toInt(), userInputParamUnit->text());

	}
	else
	{
		DeviceDBConfigInfo::getInstance()->updateParamInfo2DB(paramID, userInputParamName->text(), userSelectParamType->currentData().toInt(), userInputParamUnit->text());

	}
	emit updateParamInfos();
}

/**
	@brief
	@param wid -
**/
void AddRocketTypeWidget::opDeviceInfo(DeviceCommonVaries::DeviceModule wid) {

	if (userInputDevName->text().isEmpty())
	{
		QMessageBox::warning(this, QString("����"), QString("�豸���Ʋ���Ϊ��"));
		return;
	}
	if (wid == DeviceCommonVaries::ADD_MODULE)
	{
		DeviceDBConfigInfo::getInstance()->deviceConfigOp2DB(userSelectRocketType->currentData().toInt(), userInputDevName->text(), userSelectDevType->currentData().toInt());

	}
	else
	{
		DeviceDBConfigInfo::getInstance()->updateDeviceInfo2DB(deviceID, userSelectRocketType->currentData().toInt(), userInputDevName->text(), userSelectDevType->currentData().toInt());

	}
	emit updateDeviceInfos();
}

/**
	@brief
	@param wid -
**/
void AddRocketTypeWidget::opCommandInfo(DeviceCommonVaries::DeviceModule wid) {
	if (userInputCmdName->text().isEmpty())
	{
		QMessageBox::warning(this, QString("����"), QString("ָ�����Ʋ���Ϊ��"));
		return;
	}
	int cmdcode = 1;

	QVector<int> savedCode;
	if (wid == DeviceCommonVaries::ADD_MODULE)
	{

		DeviceDBConfigInfo::getInstance()->readCommandDB2UI();

		for (pair<int, vector<string>> eachele : DeviceDBConfigInfo::getInstance()->commandInfo)
		{
			savedCode.push_back(atoi(eachele.second[4].c_str()));
		}
		for (int i = 0; i < 0xffff; i++)
		{
			if (!savedCode.contains(i))
			{
				cmdcode = i;
				break;
			}
		}
		DeviceDBConfigInfo::getInstance()->commandConfigOp2DB(userInputCmdName->text(), userSelectRocketType->currentData().toInt(), userSelectBackCmd->currentData().toInt(), cmdcode, userSelectCmdType->currentData().toInt(), 0x55aa);

	}
	else
	{
		DeviceDBConfigInfo::getInstance()->readCommandDB2UI();
		cmdcode = atoi(DeviceDBConfigInfo::getInstance()->commandInfo[commandID][4].c_str());
		DeviceDBConfigInfo::getInstance()->updateCommandInfo2DB(commandID, userInputCmdName->text(), userSelectRocketType->currentData().toInt(), userSelectBackCmd->currentData().toInt(), cmdcode, userSelectCmdType->currentData().toInt(), 0x55aa);

	}

	emit updateCommandInfos();
}

/**
	@brief ��������
**/
void AddRocketTypeWidget::widgetConfig() {

}

/**
	@brief ��ʼ���ź����
**/
void AddRocketTypeWidget::initConnect() {

}

/**
	@brief ȡ����ť�ۺ���
**/
void AddRocketTypeWidget::clickRocketTypeCancel() {

	currentModule = DeviceCommonVaries::ADD_MODULE;
	instance->close();
}

/**
	@brief ȷ����ť�ۺ���  ���ֲ�ͬ����
**/
void AddRocketTypeWidget::clickRocketTypeOk() {


	//д�����ݱ�
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
	case DeviceCommonVaries::COMMAND_WIDGET://code�Զ�����
		opCommandInfo(currentModule);
		break;
	default:
		break;
	}

	currentModule = DeviceCommonVaries::ADD_MODULE;
	instance->close();
}

/**
	@brief ����رհ�ť�ۺ���
**/
void AddRocketTypeWidget::clickWindowClose() {

	currentModule = DeviceCommonVaries::ADD_MODULE;
	instance->close();
}



/**
	@brief ��갴�²���
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
	@brief ����ƶ�����
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
	@brief ����ͷŲ���
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