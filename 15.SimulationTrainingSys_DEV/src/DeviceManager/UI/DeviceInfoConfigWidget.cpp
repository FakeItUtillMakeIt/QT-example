#include "DeviceInfoConfigWidget.h"

using namespace DeviceInfoConfig;

InfoConfigWidget* InfoConfigWidget::instance = nullptr;

InfoConfigWidget::InfoConfigWidget(QWidget* parent)
	: QWidget(parent)
{

	this->setWindowTitle(QString("添加设备"));
	this->setWindowIcon(QIcon(":/icon/icon/bb.png"));

	//this->setBackgroundRole(QPalette::Light);

	//获得widget的palette
	QPalette palette = this->palette();
	palette.setBrush(QPalette::Window, QBrush(Qt::white));
	this->setPalette(palette);
	this->setAttribute(Qt::WA_StyledBackground);
	this->setAutoFillBackground(true);

	this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
	//this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);

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



	configRocketType = new QLabel(QString("火箭类型:"));
	userSelectRocketType = new QComboBox;
	configDeviceName = new QLabel(QString("设备名称:"));
	userInputDeviceName = new QLineEdit;
	userInputDeviceName->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	configDeviceType = new QLabel(QString("设备类型:"));
	userSelectDeviceType = new QComboBox;

	deviceConfigOKBtn = new QPushButton(QString("新增设备"));
	//!< 	开关量状态值
	configStatusName = new QLabel(QString("设备状态名称:"));;
	userInputStatusName = new QLineEdit;
	userInputStatusName->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	statusConfigOKBtn = new QPushButton(QString("新增状态"));
	//!< 	开关量显示值
	configDeviceID = new QLabel(QString("设备:"));
	userSelectDeviceID = new QComboBox;
	configStatusID = new QLabel(QString("状态:"));
	userSelectStatusID = new QComboBox;
	configDataPath = new QLabel(QString("状态文件:"));
	userSelectPath = new QLabel();
	userSelectPathBtn = new QPushButton(QString("选择状态文件"));

	deviceStatusOKBtn = new QPushButton(QString("新增设备状态信息"));
	//!< 	device_param_info
	configDeviceParamID = new QLabel(QString("设备:"));
	userSelectDeviceParamID = new	QComboBox;
	configParamID = new QLabel(QString("参数:"));
	userSelectParamID = new QComboBox;

	deviceParamInfoOKBtn = new QPushButton(QString("新增设备参数"));

	updateDeviceInfoBtn = new QPushButton(QString("取消"));

	QGridLayout* infoUILayout = new QGridLayout;
	int rowCount = 40;
	int columnCount = 4;
	int row = 0;
	int column = 0;

	infoUILayout->addWidget(new QLabel(QString("设备配置")), row++, 0);

	QFrame* hframe0 = new QFrame;
	hframe0->setFrameShape(QFrame::HLine);
	//hframe0->setStyleSheet("background:gray;min-height:1px");

	infoUILayout->addWidget(hframe0, row++, 0, 1, columnCount);
	//第一行
	column = 0;
	infoUILayout->addWidget(configRocketType, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectRocketType, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configDeviceName, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputDeviceName, row++ % rowCount, column++ % columnCount, 1, 1);

	infoUILayout->addWidget(configDeviceType, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectDeviceType, row++ % rowCount, column++ % columnCount, 1, 1);
	column = 0;

	infoUILayout->addWidget(deviceConfigOKBtn, row++ % rowCount, column++ % columnCount, 1, 1);

	QFrame* frameSpace0 = new QFrame;
	frameSpace0->setFrameShape(QFrame::Box);
	infoUILayout->addWidget(frameSpace0, row++, 0);
	frameSpace0->setLineWidth(0);

	infoUILayout->addWidget(new QLabel(QString("设备状态配置")), row++, 0);
	QFrame* hframe1 = new QFrame;
	hframe1->setFrameShape(QFrame::HLine);
	infoUILayout->addWidget(hframe1, row++, 0, 1, columnCount);

	//第二行
	column = 0;
	infoUILayout->addWidget(configStatusName, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputStatusName, row++ % rowCount, column++ % columnCount, 1, 1);
	column = 0;

	infoUILayout->addWidget(statusConfigOKBtn, row++ % rowCount, column++ % columnCount, 1, 1);

	QFrame* frameSpace1 = new QFrame;
	frameSpace0->setFrameShape(QFrame::Box);
	infoUILayout->addWidget(frameSpace1, row++, 0);
	frameSpace0->setLineWidth(0);

	infoUILayout->addWidget(new QLabel(QString("设备状态信息配置")), row++, 0);
	QFrame* hframe2 = new QFrame;
	hframe2->setFrameShape(QFrame::HLine);
	infoUILayout->addWidget(hframe2, row++, 0, 1, columnCount);
	//第三行
	column = 0;
	infoUILayout->addWidget(configDeviceID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectDeviceID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configStatusID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectStatusID, row++ % rowCount, column++ % columnCount, 1, 1);
	//
	infoUILayout->addWidget(configDataPath, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectPath, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectPathBtn, row++ % rowCount, column++ % columnCount, 1, 1);

	column = 0;
	infoUILayout->addWidget(deviceStatusOKBtn, row++ % rowCount, column++ % columnCount, 1, 1);

	QFrame* frameSpace2 = new QFrame;
	frameSpace0->setFrameShape(QFrame::Box);
	infoUILayout->addWidget(frameSpace2, row++, 0);
	frameSpace0->setLineWidth(0);

	infoUILayout->addWidget(new QLabel(QString("设备参数信息配置")), row++, 0);
	QFrame* hframe3 = new QFrame;
	hframe3->setFrameShape(QFrame::HLine);
	infoUILayout->addWidget(hframe3, row++, 0, 1, columnCount);
	//第四行
	column = 0;
	infoUILayout->addWidget(configDeviceParamID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectDeviceParamID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configParamID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectParamID, row++ % rowCount, column++ % columnCount, 1, 1);

	column = 0;
	infoUILayout->addWidget(deviceParamInfoOKBtn, row++ % rowCount, column++ % columnCount, 1, 1);


	infoUILayout->addWidget(updateDeviceInfoBtn, row++ % rowCount, columnCount - 1, 1, 1);


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
	//设备类型
	userSelectDeviceType->clear();
	for (auto ele = commonVaries->deviceType.begin(); ele != commonVaries->deviceType.end(); ele++)
	{
		userSelectDeviceType->addItem(QString::fromLocal8Bit(ele->first.c_str()), commonVaries->deviceType[ele->first.c_str()]);
	}

	//设备ID  

	deviceManageDbOp->deviceInfo.clear();
	userSelectDeviceID->clear();
	deviceManageDbOp->readDeviceDB2UI();

	for (auto ele = deviceManageDbOp->deviceInfo.begin(); ele != deviceManageDbOp->deviceInfo.end(); ele++)
	{
		//显示名称  后带数据为ID
		userSelectDeviceID->addItem(QString(ele->second[2].c_str()), QString(ele->second[0].c_str()).toInt());
	}
	//状态ID
	userSelectStatusID->clear();
	deviceManageDbOp->readStatusInfoDB2UI();

	for (auto ele = deviceManageDbOp->statusInfo.begin(); ele != deviceManageDbOp->statusInfo.end(); ele++)
	{
		//状态名称和 ID 
		userSelectStatusID->addItem(QString(ele->second[1].c_str()), QString(ele->second[0].c_str()).toInt());
	}

	//设备ID  
	userSelectDeviceParamID->clear();
	deviceManageDbOp->deviceInfo.clear();
	deviceManageDbOp->readDeviceDB2UI();

	for (auto ele = deviceManageDbOp->deviceInfo.begin(); ele != deviceManageDbOp->deviceInfo.end(); ele++)
	{
		userSelectDeviceParamID->addItem(QString(ele->second[2].c_str()), QString(ele->second[0].c_str()).toInt());
	}
	//参数ID
	deviceManageDbOp->paramInfo.clear();
	deviceManageDbOp->readParamDB2UI();
	userSelectParamID->clear();
	for (auto ele = deviceManageDbOp->paramInfo.begin(); ele != deviceManageDbOp->paramInfo.end(); ele++)
	{
		userSelectParamID->addItem(QString(ele->second[1].c_str()), QString(ele->second[0].c_str()).toInt());
	}

}

void InfoConfigWidget::initConnect() {
	connect(userSelectPathBtn, &QPushButton::clicked, this, &InfoConfigWidget::selectFilePathOKBtn);

	connect(deviceConfigOKBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickDeviceOKBtn);

	connect(statusConfigOKBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickStatusOKBtn);

	connect(deviceStatusOKBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickDeviceStatusInfoOKBtn);

	connect(deviceParamInfoOKBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickDeviceParamOKBtn);

	connect(updateDeviceInfoBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickUpateDeviceBtn);
}

void InfoConfigWidget::setConfigHeader(QString header) {

}

/**
	@brief 选择文件
**/
void InfoConfigWidget::selectFilePathOKBtn() {
	//过滤格式txt
	QString filepath1 = QFileDialog::getOpenFileName(nullptr, QString(), QString(), QString(tr("*.txt")));

	filepath1 = filepath1.split("/device/")[1];
	userSelectPath->setWordWrap(true);
	userSelectPath->setText(filepath1);
}

/**
	@brief 确认设备配置
**/
void InfoConfigWidget::clickDeviceOKBtn() {
	//更改device_info
	int rocketType = userSelectRocketType->currentData().toInt();
	QString deviceName = userInputDeviceName->text();

	if (deviceName == "")
	{
		QMessageBox::information(this, "info", QString("设备名不能为空！"));
		return;
	}

	int deviceType = userSelectDeviceType->currentData().toInt();
	DeviceDBConfigInfo::getInstance()->deviceConfigOp2DB(rocketType, deviceName, deviceType);

	widgetConfig();
	emit updateDeviceInfo();
}

/**
	@brief 确认状态配置
**/
void InfoConfigWidget::clickStatusOKBtn() {
	QString statusName = userInputStatusName->text();
	if (statusName == "")
	{
		QMessageBox::information(this, "info", QString("状态名不能为空！"));
		return;
	}
	DeviceDBConfigInfo::getInstance()->deviceStatusInfo2DB(statusName);
	widgetConfig();
}

/**
	@brief 确认设备状态信息配置
**/
void InfoConfigWidget::clickDeviceStatusInfoOKBtn() {
	int deviceID1 = userSelectDeviceID->currentData().toInt();
	int deviceStatusID1 = userSelectStatusID->currentData().toInt();
	QString statusPath = userSelectPath->text();
	if (statusPath == "")
	{
		QMessageBox::information(this, "info", QString("需绑定设备状态信息！"));
		return;
	}
	DeviceDBConfigInfo::getInstance()->deviceStatusInfo2DB(deviceID1, deviceStatusID1, statusPath);
	widgetConfig();
}

/**
	@brief 确认设备参数配置
**/
void InfoConfigWidget::clickDeviceParamOKBtn() {

	int deviceID2 = userSelectDeviceParamID->currentData().toInt();
	int paramID2 = userSelectParamID->currentData().toInt();
	//同一设备参数信息唯一

	QString qSqlString = QString("SELECT\
		device_param_info.id,\
		device_param_info.device_id,\
		device_param_info.parameter_id,\
		device_param_info.createTime,\
		device_param_info.lastUpdateTime\
		FROM\
		device_param_info\
		WHERE\
		device_param_info.device_id = %1;");
	qSqlString = qSqlString.arg(deviceID2);
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
	for each (auto var in DeviceDBConfigInfo::getInstance()->customReadInfoMap)
	{
		if (atoi(var.second[2].c_str()) == paramID2)
		{
			QMessageBox::warning(this, QString("警告"), QString("同一设备参数重复，请重新选择！"));
			return;
		}
	}

	DeviceDBConfigInfo::getInstance()->deviceParamInfo2DB(deviceID2, paramID2);
	widgetConfig();
}


void InfoConfigWidget::clickUpateDeviceBtn() {

	this->close();
}