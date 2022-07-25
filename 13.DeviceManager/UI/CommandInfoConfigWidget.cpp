#include "CommandInfoConfigWidget.h"

using namespace CommandInfoConfig;

InfoConfigWidget* InfoConfigWidget::instance = nullptr;

InfoConfigWidget::InfoConfigWidget(QWidget* parent)
	: QWidget(parent)
{

	this->setWindowTitle(QString("���ָ��"));
	this->setWindowIcon(QIcon(":/icon/icon/squareBl.png"));

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
	wss = WidgetStyleSheet::getInstace();

	//!< command_info
	configRocketType = new QLabel(QString("����ͺ�:"));
	userSelectRocketType = new QComboBox;
	configBackCommand = new QLabel(QString("����ID:"));
	userSelectBackCMD = new QComboBox;
	configCommandName = new QLabel(QString("ָ������:"));
	userInputCMDName = new QLineEdit;
	userInputCMDName->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	configCommandCode = new QLabel(QString("ָ�����:"));;
	userSelectCMDCode = new QComboBox;

	configCommandType = new QLabel(QString("ָ������:"));
	userSelectCMDType = new QComboBox;
	configCommandPrefix = new QLabel(QString("ָ��ǰ׺:"));
	userSelectCMDPrefix = new QComboBox;

	commandInfoOKBtn = new QPushButton(QString("����ָ��"));

	//!< 	command_param_info
	configEmitCmdID = new QLabel(QString("�ָⷢ��ID:"));
	userSelectEmitCmdID = new QComboBox();
	configParamName = new QLabel(QString("��������:"));
	userInputParamName = new QLineEdit;
	userInputParamName->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	configParamCode = new QLabel(QString("��������:"));
	userSelectParamCode = new QComboBox();
	configIndex = new QLabel(QString("����:"));
	userSelectIndex = new QComboBox();
	configParamLength = new QLabel(QString("�����ֽڳ���:"));
	userSelectParamLength = new QComboBox();
	configParamType = new QLabel(QString("��������:"));
	userSelectParamType = new QComboBox();
	configParamDefaultVal = new QLabel(QString("����Ĭ��ֵ:"));
	userSelectParamDefualtVal = new QComboBox();

	commandParamInfoOKBtn = new QPushButton(QString("����ָ�����"));

	updateCommandBtn = new QPushButton(QString("ȡ��"));

	QGridLayout* infoUILayout = new QGridLayout;
	int rowCount = 40;
	int columnCount = 4;
	int row = 0;
	int column = 0;

	infoUILayout->addWidget(new QLabel(QString("ָ������")), row++, 0);

	QFrame* hframe0 = new QFrame;
	hframe0->setFrameShape(QFrame::HLine);
	//hframe0->setStyleSheet("background:gray;min-height:1px");

	infoUILayout->addWidget(hframe0, row++, 0, 1, columnCount);
	//��һ��
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

	infoUILayout->addWidget(new QLabel(QString("ָ���������")), row++, 0);
	QFrame* hframe1 = new QFrame;
	hframe1->setFrameShape(QFrame::HLine);
	infoUILayout->addWidget(hframe1, row++, 0, 1, columnCount);

	//�ڶ���
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


	infoUILayout->addWidget(updateCommandBtn, row++ % rowCount, columnCount - 1, 1, 1);


	QString qss = wss->infoConfigLabelStyleSheet.arg("QLabel") + wss->infoConfigLineEditStyleSheet.arg("QLineEdit")
		+ wss->infoConfigPushButtonStyleSheet.arg("QPushButton") + wss->infoConfigComboBoxStyleSheet.arg("QComboBox");

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
	//����
	userSelectBackCMD->clear();
	deviceManageDbOp->commandInfo.clear();
	deviceManageDbOp->readCommandDB2UI();

	for (auto ele = deviceManageDbOp->commandInfo.begin(); ele != deviceManageDbOp->commandInfo.end(); ele++)
	{

		//ɸѡ�ⷢ����
		if (QString(ele->second[5].c_str()).toInt() == 2)
		{
			userSelectBackCMD->addItem(QString(ele->second[3].c_str()), QString(ele->second[0].c_str()).toInt());
		}

	}

	//ָ�����
	userSelectCMDCode->clear();
	for (auto ele = commonVaries->commandCode.begin(); ele != commonVaries->commandCode.end(); ele++)
	{
		userSelectCMDCode->addItem(QString(ele->first.c_str()), ele->second);
	}
	//ָ������
	userSelectCMDType->clear();
	for (auto ele = commonVaries->commandType.begin(); ele != commonVaries->commandType.end(); ele++)
	{
		userSelectCMDType->addItem(QString::fromLocal8Bit(ele->first.c_str()), ele->second);
	}

	//ָ��ǰ׺
	userSelectCMDPrefix->clear();
	for (auto ele = commonVaries->commandPrefix.begin(); ele != commonVaries->commandPrefix.end(); ele++)
	{
		userSelectCMDPrefix->addItem(QString(ele->first.c_str()), ele->second);
	}

	//�ָⷢ��ID
	userSelectEmitCmdID->clear();
	deviceManageDbOp->commandInfo.clear();
	deviceManageDbOp->readCommandDB2UI();

	//������Ҫɸѡ����Ϊ�ָⷢ���
	for (auto ele = deviceManageDbOp->commandInfo.begin(); ele != deviceManageDbOp->commandInfo.end(); ele++)
	{
		auto a = QString(ele->second[5].c_str()).toInt();
		auto b = commonVaries->commandType["�ָⷢ��"];
		if (QString(ele->second[5].c_str()).toInt() == 1)
		{
			userSelectEmitCmdID->addItem(QString(ele->second[3].c_str()), QString(ele->second[0].c_str()).toInt());
		}
	}
	//��������
	userSelectParamCode->clear();
	for (auto ele = commonVaries->commandParamCode.begin(); ele != commonVaries->commandParamCode.end(); ele++)
	{
		userSelectParamCode->addItem(QString(ele->first.c_str()), ele->second);
	}
	//����
	userSelectIndex->clear();
	for (auto ele : commonVaries->commandParamIndex)
	{
		userSelectIndex->addItem(QString::number(ele));
	}
	//����
	userSelectParamLength->clear();
	for (auto ele : commonVaries->commandParamLength)
	{
		userSelectParamLength->addItem(QString::number(ele));
	}

	//��������
	userSelectParamType->clear();
	for (auto ele : commonVaries->commandParamType)
	{
		userSelectParamType->addItem(QString::fromLocal8Bit(ele.c_str()));
	}
	//����Ĭ��ֵ
	userSelectParamDefualtVal->clear();
	for (auto ele = commonVaries->commandParamDefaultVal.begin(); ele != commonVaries->commandParamDefaultVal.end(); ele++)
	{
		userSelectParamDefualtVal->addItem(QString(ele->first.c_str()), ele->second);
	}
}

void InfoConfigWidget::initConnect() {

	connect(commandInfoOKBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickCommandOKBtn);

	connect(commandParamInfoOKBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickCommandParamOKBtn);

	connect(updateCommandBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickUpateCommandBtn);


}


void InfoConfigWidget::setConfigHeader(QString header) {

}

/**
	@brief ָ������ȷ��
**/
void InfoConfigWidget::clickCommandOKBtn() {
	QString cmdName = userInputCMDName->text();
	auto tmpInstance = DeviceDBConfigInfo::getInstance();

	int rocketTypeID = userSelectRocketType->currentData().toInt();
	int cmdBackID = userSelectBackCMD->currentData().toInt();
	int cmdCode = userSelectCMDCode->currentData().toInt();
	int cmdType = userSelectCMDType->currentData().toInt();
	int cmdPredix = userSelectCMDPrefix->currentData().toInt();
	tmpInstance->commandConfigOp2DB(cmdName, rocketTypeID, cmdBackID, cmdCode, cmdType, cmdPredix);
	widgetConfig();
	emit updateCommandInfos();

}

/**
	@brief ָ���������ȷ��
**/
void InfoConfigWidget::clickCommandParamOKBtn() {
	int cmdID = userSelectEmitCmdID->currentData().toInt();
	QString paramName1 = userInputParamName->text();
	int paramCode1 = userSelectParamCode->currentData().toInt();
	int index1 = userSelectIndex->currentData().toInt();
	int paramLength1 = userSelectParamLength->currentData().toInt();
	QString paramType1 = userSelectParamType->currentText();
	float paramDefaultVal = userSelectParamDefualtVal->currentData().toFloat();
	DeviceDBConfigInfo::getInstance()->commandParamInfo2DB(cmdID, paramName1, paramCode1, index1, paramLength1, paramType1, paramDefaultVal);
	widgetConfig();
}

void InfoConfigWidget::clickUpateCommandBtn() {

	this->close();

}