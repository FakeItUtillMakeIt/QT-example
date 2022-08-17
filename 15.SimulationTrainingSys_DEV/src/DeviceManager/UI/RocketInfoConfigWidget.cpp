#include "RocketInfoConfigWidget.h"

using namespace RocketInfoConfig;

InfoConfigWidget* InfoConfigWidget::instance = nullptr;

InfoConfigWidget::InfoConfigWidget(QWidget* parent)
	: QWidget(parent)
{
	currentDeviceFlag = DeviceCommonVaries::getInstance()->DeviceModule::ADD_MODULE;

	this->setWindowTitle(QString("����ͺ�����"));

	this->setWindowIcon(QIcon(":/icon/icon/bb.png"));

	//this->setBackgroundRole(QPalette::Light);

	//���widget��palette
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
	@brief ���ݲ�ͬģ���ʼ�����ò���
**/
void InfoConfigWidget::InitUILayout() {
	//this->setFixedSize(640, 480);
	auto ap = AppCache::instance();
	wss = WidgetStyleSheet::getInstace();

	//!< rocket_info
	configRocketName = new QLabel(QString("�������:"));
	userInputRocketName = new QLineEdit;
	userInputRocketName->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	configRocketType = new QLabel(QString("�������:"));
	userInputRocketDescript = new QLineEdit;
	userInputRocketDescript->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	rocketInfoOKBtn = new QPushButton(QString("���»���ͺ�"));



	//!< rocke_data_info
	configRocketID = new QLabel(QString("����ͺ�:"));;
	userSelectRocketID = new QComboBox;
	configCmdName = new QLabel(QString("ָ������:"));
	userInputCmdName = new QLineEdit;
	userInputCmdName->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	configCmdCode = new QLabel(QString("ָ�����:"));
	userInputCmdCode = new QLineEdit;
	userInputCmdCode->setValidator(new QRegExpValidator(QRegExp("\\d+")));

	configCmdPrefix = new QLabel(QString("ָ��ǰ׺:"));
	userSelectCmdPrefix = new QComboBox;

	rocketDataInfoOKBtn = new QPushButton(QString("��������Э��"));
	//!< rocket_prame_info
	configRocketDataBackCmdID = new QLabel(QString("�������ݷ���ָ��:"));;
	userSlctRocketDataBackCmdID = new QComboBox;
	configParamID = new QLabel(QString("��������:"));
	userSelectParamID = new QComboBox;
	configIndex = new QLabel(QString("����:"));
	userSlctIndex = new QLineEdit;
	userSlctIndex->setValidator(new QIntValidator(0, 100));

	configParamLength = new QLabel(QString("�����ֽڳ���:"));
	userInputParamLength = new QLineEdit;
	userInputParamLength->setValidator(new QIntValidator(1, 16));

	configParamType = new QLabel(QString("��������"));
	userSelectParamType = new QComboBox;

	rocketParamInfOKBtn = new	QPushButton(QString("���"));

	updateRocketInfoBtn = new QPushButton(QString("ȡ��"));


	QGridLayout* infoUILayout = new QGridLayout;
	int rowCount = 40;
	int columnCount = 4;
	int row = 0;
	int column = 0;

	infoUILayout->addWidget(new QLabel(QString("����ͺ�")), row++, 0);

	QFrame* hframe0 = new QFrame;
	hframe0->setFrameShape(QFrame::HLine);
	//hframe0->setStyleSheet("background:gray;min-height:1px");

	infoUILayout->addWidget(hframe0, row++, 0, 1, columnCount);
	//��һ��
	infoUILayout->addWidget(configRocketName, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputRocketName, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configRocketType, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputRocketDescript, row++ % rowCount, column++ % columnCount, 1, 1);
	column = 0;
	infoUILayout->addWidget(rocketInfoOKBtn, row++ % rowCount, column++ % columnCount, 1, 1);

	QFrame* frameSpace0 = new QFrame;
	frameSpace0->setFrameShape(QFrame::Box);
	infoUILayout->addWidget(frameSpace0, row++, 0);
	frameSpace0->setLineWidth(0);

	infoUILayout->addWidget(new QLabel(QString("�������Э��")), row++, 0);
	QFrame* hframe1 = new QFrame;
	hframe1->setFrameShape(QFrame::HLine);
	infoUILayout->addWidget(hframe1, row++, 0, 1, columnCount);
	//�ڶ���
	column = 0;
	infoUILayout->addWidget(configRocketID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectRocketID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configCmdName, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputCmdName, row++ % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configCmdCode, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputCmdCode, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configCmdPrefix, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectCmdPrefix, row++ % rowCount, column++ % columnCount, 1, 1);
	column = 0;
	infoUILayout->addWidget(rocketDataInfoOKBtn, row++ % rowCount, column++ % columnCount, 1, 1);

	QFrame* frameSpace1 = new QFrame;
	frameSpace1->setFrameShape(QFrame::Box);
	infoUILayout->addWidget(frameSpace1, row++, 0);
	frameSpace1->setLineWidth(0);

	infoUILayout->addWidget(new QLabel(QString("�������ͨ��Э������")), row++, 0);
	QFrame* hframe2 = new QFrame;
	hframe2->setFrameShape(QFrame::HLine);
	infoUILayout->addWidget(hframe2, row++, 0, 1, columnCount);

	//������
	column = 0;
	infoUILayout->addWidget(configRocketDataBackCmdID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSlctRocketDataBackCmdID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configParamID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectParamID, row++ % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configIndex, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSlctIndex, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configParamLength, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputParamLength, row++ % rowCount, column++ % columnCount, 1, 1);
	column = 0;
	infoUILayout->addWidget(configParamType, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectParamType, row++ % rowCount, column++ % columnCount, 1, 1);

	column = 0;
	infoUILayout->addWidget(rocketParamInfOKBtn, row++ % rowCount, column++ % columnCount, 1, 1);

	QFrame* frameSpace2 = new QFrame;
	frameSpace2->setFrameShape(QFrame::Box);
	infoUILayout->addWidget(frameSpace2, row++, 0);
	frameSpace2->setLineWidth(0);



	infoUILayout->addWidget(updateRocketInfoBtn, row++ % rowCount, columnCount - 1, 1, 1);


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
	//����ͺ�
	DeviceCommonVaries* commonVaries = DeviceCommonVaries::getInstance();
	userInputRocketDescript->clear();
	auto deviceManageDbOp = DeviceDBConfigInfo::getInstance();
	deviceManageDbOp->rocketInfo.clear();
	deviceManageDbOp->readRocketDB2UI();

	//����ͺ�
	userSelectRocketID->clear();
	for (auto ele = deviceManageDbOp->rocketInfo.begin(); ele != deviceManageDbOp->rocketInfo.end(); ele++)
	{
		userSelectRocketID->addItem(QString(ele->second[1].c_str()), QString(ele->second[0].c_str()).toInt());
	}

	//ָ��ǰ׺

	userSelectCmdPrefix->clear();
	for (auto ele = commonVaries->commandPrefix.begin(); ele != commonVaries->commandPrefix.end(); ele++)
	{
		userSelectCmdPrefix->addItem(QString(ele->first.c_str()), ele->second);
	}
	//�������ݷ���ָ��
	userSlctRocketDataBackCmdID->clear();
	deviceManageDbOp->rocketDataInfo.clear();
	deviceManageDbOp->readRocketDataDB2UI();

	for (auto ele = deviceManageDbOp->rocketDataInfo.begin(); ele != deviceManageDbOp->rocketDataInfo.end(); ele++)
	{
		userSlctRocketDataBackCmdID->addItem(QString(ele->second[2].c_str()), QString(ele->second[0].c_str()).toInt());
	}
	if (!userLastSelectCmd.isEmpty())
	{
		userSlctRocketDataBackCmdID->setCurrentText(userLastSelectCmd);
	}


	//���ϲ���
	connect(userSlctRocketDataBackCmdID, QOverload<const QString&>::of(&QComboBox::activated), this, [=]() {
		QString qSqlString = QString("SELECT\
			device_param_info.id,\
			parameter_info.`name`,\
			device_info.`name`,\
			device_param_info.createTime,\
			device_param_info.lastUpdateTime\
			FROM\
			device_param_info\
			INNER JOIN parameter_info ON device_param_info.parameter_id = parameter_info.id\
			INNER JOIN device_info ON device_param_info.device_id = device_info.id\
			WHERE\
			device_info.rocket_id = %1");

		DeviceDBConfigInfo::getInstance()->readRocketDataDB2UI();
		int selectCmdRocketId = atoi(DeviceDBConfigInfo::getInstance()->rocketDataInfo[userSlctRocketDataBackCmdID->currentData().toInt()][1].c_str());

		qSqlString = qSqlString.arg(selectCmdRocketId);

		deviceManageDbOp->customReadTableInfo(qSqlString);
		userSelectParamID->clear();
		for (auto ele = deviceManageDbOp->customReadInfoMap.begin(); ele != deviceManageDbOp->customReadInfoMap.end(); ele++)
		{
			//�����豸����
			userSelectParamID->addItem(QString(ele->second[1].c_str()), QString(ele->second[0].c_str()).toInt());
		}

		});

	//���ϲ���

	/*deviceManageDbOp->readParamDB2UI();
	userSelectParamID->clear();
	for (auto ele = deviceManageDbOp->paramInfo.begin(); ele != deviceManageDbOp->paramInfo.end(); ele++)
	{
		userSelectParamID->addItem(QString(ele->second[1].c_str()), QString(ele->second[0].c_str()).toInt());
	}*/

	//��������
	deviceManageDbOp->paramInfo.clear();
	deviceManageDbOp->readParamDB2UI();
	userSelectParamType->clear();
	for (auto ele : commonVaries->rocketParamType)
	{
		userSelectParamType->addItem(QString(ele.second.c_str()), ele.first);
	}
}



void InfoConfigWidget::initConnect() {

	connect(rocketInfoOKBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickRocketOKBtn);

	connect(rocketDataInfoOKBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickRocketDataOKBtn);

	connect(rocketParamInfOKBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickRocketParamOKBtn);

	connect(updateRocketInfoBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickUpateRocketBtn);



}

void InfoConfigWidget::setConfigHeader(QString header) {

}

/**
	@brief ���ȷ������
**/
void InfoConfigWidget::clickRocketOKBtn() {
	QString rocketName = userInputRocketName->text();
	if (rocketName == "")
	{
		QMessageBox::information(this, "info", QString("������Ʋ���Ϊ�գ�"));
		return;
	}
	QString rocketDescript = userInputRocketDescript->text();
	if (rocketDescript == "")
	{
		QMessageBox::information(this, "info", QString("�����������Ϊ�գ�"));
		return;
	}
	if (currentDeviceFlag == DeviceCommonVaries::getInstance()->DeviceModule::ADD_MODULE)
	{
		DeviceDBConfigInfo::getInstance()->rocketConfigOp2DB(rocketName, rocketDescript);
	}
	if (currentDeviceFlag == DeviceCommonVaries::getInstance()->DeviceModule::UPDATE_MODULE)
	{
		DeviceDBConfigInfo::getInstance()->updateRocketInfo2DB(editId, rocketName, rocketDescript);
		this->close();
	}


	widgetConfig();
	emit updateRocketInfo();
	currentDeviceFlag = DeviceCommonVaries::getInstance()->DeviceModule::ADD_MODULE;
}

/**
	@brief �������ȷ������
**/
void InfoConfigWidget::clickRocketDataOKBtn() {
	int rocketTypeID = userSelectRocketID->currentData().toInt();
	QString cmdName1 = userInputCmdName->text();
	if (cmdName1 == "")
	{
		QMessageBox::information(this, "info", QString("ָ�����Ʋ���Ϊ�գ�"));
		return;
	}
	int cmdCode = userInputCmdCode->text().toInt();
	if (userInputCmdCode->text() == "")
	{
		QMessageBox::information(this, "info", QString("ָ����벻��Ϊ�գ�"));
		return;
	}

	DeviceDBConfigInfo::getInstance()->readRocketDB2UI();
	for (auto var : DeviceDBConfigInfo::getInstance()->rocketDataInfo)
	{
		if (atoi(var.second[3].c_str()) == cmdCode)
		{
			QMessageBox::warning(this, QString("����"), QString("�Ѵ��ڸñ��룬�������������!"));
			return;
		}
	}

	int cmdPrefix = userSelectCmdPrefix->currentData().toInt();
	DeviceDBConfigInfo::getInstance()->rocketDataInfo2DB(rocketTypeID, cmdName1, cmdCode, cmdPrefix);
	widgetConfig();
	this->close();
}

/**
	@brief ���������Ϣȷ������
**/
void InfoConfigWidget::clickRocketParamOKBtn() {
	int rocketDataBackCmdID = userSlctRocketDataBackCmdID->currentData().toInt();
	userLastSelectCmd = userSlctRocketDataBackCmdID->currentText();
	int paramId1 = userSelectParamID->currentData().toInt();
	int index1 = userSlctIndex->text().toInt();
	if (userSlctIndex->text() == "")
	{
		QMessageBox::information(this, "info", QString("��������ֵ�����0��"));
		return;
	}

	QString qSqlString = QString("SELECT\
		rockect_param_info.id,\
		rocket_data_info.`code`,\
		rockect_param_info.device_parameter_id,\
		rockect_param_info.`index`,\
		rockect_param_info.createTime,\
		rockect_param_info.lastUpdateTime\
		FROM\
		rocket_data_info\
		INNER JOIN rockect_param_info ON rocket_data_info.id = rockect_param_info.rocket_data_id\
		WHERE\
		rocket_data_info.`code` = %1;\
		");
	//ͬһ����������ָ��Ĳ�������Ψһ
	DeviceDBConfigInfo::getInstance()->readRocketDataDB2UI();
	qSqlString = qSqlString.arg(atoi(DeviceDBConfigInfo::getInstance()->rocketDataInfo[rocketDataBackCmdID][3].c_str()));
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
	for (auto var = DeviceDBConfigInfo::getInstance()->customReadInfoMap.begin(); var != DeviceDBConfigInfo::getInstance()->customReadInfoMap.end(); var++)
	{
		if (paramId1 == atoi(var->second[2].c_str()))
		{
			QMessageBox::warning(this, QString("����"), QString("��ǰָ��󶨲����Ѵ��ڣ����������������"));
			return;
		}
		if (index1 == atoi(var->second[3].c_str()))
		{
			QMessageBox::warning(this, QString("����"), QString("��ǰָ��󶨲�������������������ͻ�������������"));
			return;
		}
	}

	int paramLength1 = userInputParamLength->text().toInt();
	if (userInputParamLength->text() == "")
	{
		QMessageBox::information(this, "info", QString("�����ֽڳ��������0��"));
		return;
	}

	int paramType1 = userSelectParamType->currentData().toInt();
	DeviceDBConfigInfo::getInstance()->rocketParamInfo2DB(rocketDataBackCmdID, paramId1, index1, paramLength1, paramType1);

	widgetConfig();
}


void InfoConfigWidget::clickUpateRocketBtn() {
	this->close();

}