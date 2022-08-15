#include "DeviceInfoConfigWidget.h"

using namespace DeviceInfoConfig;

InfoConfigWidget* InfoConfigWidget::instance = nullptr;

InfoConfigWidget::InfoConfigWidget(QWidget* parent)
	: QWidget(parent)
{

	this->setWindowTitle(QString("����豸"));
	this->setWindowIcon(QIcon(":/icon/icon/bb.png"));

	//this->setBackgroundRole(QPalette::Light);

	//���widget��palette
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
	@brief ���ݲ�ͬģ���ʼ�����ò���
**/
void InfoConfigWidget::InitUILayout() {
	//this->setFixedSize(640, 480);
	wss = WidgetStyleSheet::getInstace();



	configRocketType = new QLabel(QString("�������:"));
	userSelectRocketType = new QComboBox;
	configDeviceName = new QLabel(QString("�豸����:"));
	userInputDeviceName = new QLineEdit;
	userInputDeviceName->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	configDeviceType = new QLabel(QString("�豸����:"));
	userSelectDeviceType = new QComboBox;

	deviceConfigOKBtn = new QPushButton(QString("�����豸"));
	//!< 	������״ֵ̬
	configStatusName = new QLabel(QString("�豸״̬����:"));;
	userInputStatusName = new QLineEdit;
	userInputStatusName->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	statusConfigOKBtn = new QPushButton(QString("����״̬"));
	//!< 	��������ʾֵ
	configDeviceID = new QLabel(QString("�豸:"));
	userSelectDeviceID = new QComboBox;
	configStatusID = new QLabel(QString("״̬:"));
	userSelectStatusID = new QComboBox;
	configDataPath = new QLabel(QString("״̬�ļ�:"));
	userSelectPath = new QLabel();
	userSelectPathBtn = new QPushButton(QString("ѡ��״̬�ļ�"));

	deviceStatusOKBtn = new QPushButton(QString("�����豸״̬��Ϣ"));
	//!< 	device_param_info
	configDeviceParamID = new QLabel(QString("�豸:"));
	userSelectDeviceParamID = new	QComboBox;
	configParamID = new QLabel(QString("����:"));
	userSelectParamID = new QComboBox;

	deviceParamInfoOKBtn = new QPushButton(QString("�����豸����"));

	updateDeviceInfoBtn = new QPushButton(QString("ȡ��"));

	QGridLayout* infoUILayout = new QGridLayout;
	int rowCount = 40;
	int columnCount = 4;
	int row = 0;
	int column = 0;

	infoUILayout->addWidget(new QLabel(QString("�豸����")), row++, 0);

	QFrame* hframe0 = new QFrame;
	hframe0->setFrameShape(QFrame::HLine);
	//hframe0->setStyleSheet("background:gray;min-height:1px");

	infoUILayout->addWidget(hframe0, row++, 0, 1, columnCount);
	//��һ��
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

	infoUILayout->addWidget(new QLabel(QString("�豸״̬����")), row++, 0);
	QFrame* hframe1 = new QFrame;
	hframe1->setFrameShape(QFrame::HLine);
	infoUILayout->addWidget(hframe1, row++, 0, 1, columnCount);

	//�ڶ���
	column = 0;
	infoUILayout->addWidget(configStatusName, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputStatusName, row++ % rowCount, column++ % columnCount, 1, 1);
	column = 0;

	infoUILayout->addWidget(statusConfigOKBtn, row++ % rowCount, column++ % columnCount, 1, 1);

	QFrame* frameSpace1 = new QFrame;
	frameSpace0->setFrameShape(QFrame::Box);
	infoUILayout->addWidget(frameSpace1, row++, 0);
	frameSpace0->setLineWidth(0);

	infoUILayout->addWidget(new QLabel(QString("�豸״̬��Ϣ����")), row++, 0);
	QFrame* hframe2 = new QFrame;
	hframe2->setFrameShape(QFrame::HLine);
	infoUILayout->addWidget(hframe2, row++, 0, 1, columnCount);
	//������
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

	infoUILayout->addWidget(new QLabel(QString("�豸������Ϣ����")), row++, 0);
	QFrame* hframe3 = new QFrame;
	hframe3->setFrameShape(QFrame::HLine);
	infoUILayout->addWidget(hframe3, row++, 0, 1, columnCount);
	//������
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
	@brief �ؼ�����
**/
void InfoConfigWidget::widgetConfig() {
	//�������
	DeviceCommonVaries* commonVaries = DeviceCommonVaries::getInstance();
	auto deviceManageDbOp = DeviceDBConfigInfo::getInstance();
	userSelectRocketType->clear();
	deviceManageDbOp->rocketInfo.clear();
	deviceManageDbOp->readRocketDB2UI();

	for (auto ele = deviceManageDbOp->rocketInfo.begin(); ele != deviceManageDbOp->rocketInfo.end(); ele++)
	{
		userSelectRocketType->addItem(QString(ele->second[1].c_str()), QString(ele->second[0].c_str()).toInt());
	}
	//�豸����
	userSelectDeviceType->clear();
	for (auto ele = commonVaries->deviceType.begin(); ele != commonVaries->deviceType.end(); ele++)
	{
		userSelectDeviceType->addItem(QString::fromLocal8Bit(ele->first.c_str()), commonVaries->deviceType[ele->first.c_str()]);
	}

	//�豸ID  

	deviceManageDbOp->deviceInfo.clear();
	userSelectDeviceID->clear();
	deviceManageDbOp->readDeviceDB2UI();

	for (auto ele = deviceManageDbOp->deviceInfo.begin(); ele != deviceManageDbOp->deviceInfo.end(); ele++)
	{
		//��ʾ����  �������ΪID
		userSelectDeviceID->addItem(QString(ele->second[2].c_str()), QString(ele->second[0].c_str()).toInt());
	}
	//״̬ID
	userSelectStatusID->clear();
	deviceManageDbOp->readStatusInfoDB2UI();

	for (auto ele = deviceManageDbOp->statusInfo.begin(); ele != deviceManageDbOp->statusInfo.end(); ele++)
	{
		//״̬���ƺ� ID 
		userSelectStatusID->addItem(QString(ele->second[1].c_str()), QString(ele->second[0].c_str()).toInt());
	}

	//�豸ID  
	userSelectDeviceParamID->clear();
	deviceManageDbOp->deviceInfo.clear();
	deviceManageDbOp->readDeviceDB2UI();

	for (auto ele = deviceManageDbOp->deviceInfo.begin(); ele != deviceManageDbOp->deviceInfo.end(); ele++)
	{
		userSelectDeviceParamID->addItem(QString(ele->second[2].c_str()), QString(ele->second[0].c_str()).toInt());
	}
	//����ID
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
	@brief ѡ���ļ�
**/
void InfoConfigWidget::selectFilePathOKBtn() {
	//���˸�ʽtxt
	QString filepath1 = QFileDialog::getOpenFileName(nullptr, QString(), QString(), QString(tr("*.txt")));

	filepath1 = filepath1.split("/device/")[1];
	userSelectPath->setWordWrap(true);
	userSelectPath->setText(filepath1);
}

/**
	@brief ȷ���豸����
**/
void InfoConfigWidget::clickDeviceOKBtn() {
	//����device_info
	int rocketType = userSelectRocketType->currentData().toInt();
	QString deviceName = userInputDeviceName->text();

	if (deviceName == "")
	{
		QMessageBox::information(this, "info", QString("�豸������Ϊ�գ�"));
		return;
	}

	int deviceType = userSelectDeviceType->currentData().toInt();
	DeviceDBConfigInfo::getInstance()->deviceConfigOp2DB(rocketType, deviceName, deviceType);

	widgetConfig();
	emit updateDeviceInfo();
}

/**
	@brief ȷ��״̬����
**/
void InfoConfigWidget::clickStatusOKBtn() {
	QString statusName = userInputStatusName->text();
	if (statusName == "")
	{
		QMessageBox::information(this, "info", QString("״̬������Ϊ�գ�"));
		return;
	}
	DeviceDBConfigInfo::getInstance()->deviceStatusInfo2DB(statusName);
	widgetConfig();
}

/**
	@brief ȷ���豸״̬��Ϣ����
**/
void InfoConfigWidget::clickDeviceStatusInfoOKBtn() {
	int deviceID1 = userSelectDeviceID->currentData().toInt();
	int deviceStatusID1 = userSelectStatusID->currentData().toInt();
	QString statusPath = userSelectPath->text();
	if (statusPath == "")
	{
		QMessageBox::information(this, "info", QString("����豸״̬��Ϣ��"));
		return;
	}
	DeviceDBConfigInfo::getInstance()->deviceStatusInfo2DB(deviceID1, deviceStatusID1, statusPath);
	widgetConfig();
}

/**
	@brief ȷ���豸��������
**/
void InfoConfigWidget::clickDeviceParamOKBtn() {

	int deviceID2 = userSelectDeviceParamID->currentData().toInt();
	int paramID2 = userSelectParamID->currentData().toInt();
	//ͬһ�豸������ϢΨһ

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
			QMessageBox::warning(this, QString("����"), QString("ͬһ�豸�����ظ���������ѡ��"));
			return;
		}
	}

	DeviceDBConfigInfo::getInstance()->deviceParamInfo2DB(deviceID2, paramID2);
	widgetConfig();
}


void InfoConfigWidget::clickUpateDeviceBtn() {

	this->close();
}