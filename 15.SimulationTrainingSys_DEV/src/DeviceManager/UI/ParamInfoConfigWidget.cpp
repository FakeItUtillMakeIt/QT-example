#include "ParamInfoConfigWidget.h"

using namespace ParamInfoConfig;

InfoConfigWidget* InfoConfigWidget::instance = nullptr;

InfoConfigWidget::InfoConfigWidget(QWidget* parent)
	: QWidget(parent)
{
	this->setWindowTitle(QString("��Ӳ���"));
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

	wss = WidgetStyleSheet::getInstace();


	configParamName = new QLabel(QString("��������:"));
	userInputParamName = new QLineEdit;
	userInputParamName->setValidator(new QRegExpValidator(QRegExp("\\S+")));
	configParamType = new QLabel(QString("��������:"));
	userSelectType = new QComboBox;
	configParamUnit = new QLabel(QString("������λ:"));
	userSelcetUnit = new QComboBox;
	//paramCancelBtn = new QPushButton(QString("ȡ��"));
	paramOKBtn = new QPushButton(QString("��������"));
	//!< 	������״ֵ̬
	configSwitchValName = new QLabel(QString("����������:"));;
	userInputSwitchVal = new QLineEdit;
	userInputSwitchVal->setValidator(new QRegExpValidator(QRegExp("\\S+")));
	//switchValCancelBtn = new QPushButton(QString("ȡ��"));;
	switchValOKBtn = new QPushButton(QString("����������"));
	//!< 	��������ʾֵ
	configSwitchShowParamID = new QLabel(QString("����ID:"));
	userSlctSwitchShowParamID = new QComboBox;
	configSwitchValID = new QLabel(QString("״ֵ̬ID:"));
	userSlctSwitchValID = new QComboBox;
	configStatusVal = new QLabel(QString("״̬����:"));
	userInputStatusVal = new QLineEdit;

	userInputStatusVal->setValidator(new QIntValidator(0, 3));
	//switchShowInfoCancelBtn = new QPushButton(QString("ȡ��"));
	switchShowInfoOKBtn = new QPushButton(QString("������������ʾֵ"));

	updateParamInfoBtn = new QPushButton(QString("ȡ��"));

	QGridLayout* infoUILayout = new QGridLayout;
	int rowCount = 40;
	int columnCount = 4;
	int row = 0;
	int column = 0;

	infoUILayout->addWidget(new QLabel(QString("��������")), row++, 0);

	QFrame* hframe0 = new QFrame;
	hframe0->setFrameShape(QFrame::HLine);
	//hframe0->setStyleSheet("background:gray;min-height:1px");

	infoUILayout->addWidget(hframe0, row++, 0, 1, columnCount);
	//��һ��
	column = 0;
	infoUILayout->addWidget(configParamName, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputParamName, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configParamType, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectType, row++ % rowCount, column++ % columnCount, 1, 1);
	//�ڶ���
	column = 0;
	infoUILayout->addWidget(configParamUnit, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelcetUnit, row++ % rowCount, column++ % columnCount, 1, 1);
	column = 0;
	//infoUILayout->addWidget(paramCancelBtn, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(paramOKBtn, row++ % rowCount, column++ % columnCount, 1, 1);

	QFrame* frameSpace0 = new QFrame;
	frameSpace0->setFrameShape(QFrame::Box);
	infoUILayout->addWidget(frameSpace0, row++, 0);
	frameSpace0->setLineWidth(0);

	infoUILayout->addWidget(new QLabel(QString("������״ֵ̬����")), row++, 0);
	QFrame* hframe1 = new QFrame;
	hframe1->setFrameShape(QFrame::HLine);
	infoUILayout->addWidget(hframe1, row++, 0, 1, columnCount);

	//������
	column = 0;
	infoUILayout->addWidget(configSwitchValName, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputSwitchVal, row++ % rowCount, column++ % columnCount, 1, 1);
	column = 0;
	//infoUILayout->addWidget(switchValCancelBtn, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(switchValOKBtn, row++ % rowCount, column++ % columnCount, 1, 1);

	QFrame* frameSpace1 = new QFrame;
	frameSpace0->setFrameShape(QFrame::Box);
	infoUILayout->addWidget(frameSpace1, row++, 0);
	frameSpace0->setLineWidth(0);

	infoUILayout->addWidget(new QLabel(QString("��������ʾֵ����")), row++, 0);
	QFrame* hframe2 = new QFrame;
	hframe2->setFrameShape(QFrame::HLine);
	infoUILayout->addWidget(hframe2, row++, 0, 1, columnCount);
	//������
	column = 0;
	infoUILayout->addWidget(configSwitchShowParamID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSlctSwitchShowParamID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configSwitchValID, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSlctSwitchValID, row++ % rowCount, column++ % columnCount, 1, 1);
	//
	infoUILayout->addWidget(configStatusVal, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputStatusVal, row++ % rowCount, column++ % columnCount, 1, 1);
	column = 0;
	//infoUILayout->addWidget(switchShowInfoCancelBtn, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(switchShowInfoOKBtn, row++ % rowCount, column++ % columnCount, 1, 1);

	QFrame* frameSpace2 = new QFrame;
	frameSpace0->setFrameShape(QFrame::Box);
	infoUILayout->addWidget(frameSpace2, row++, 0);
	frameSpace0->setLineWidth(0);

	infoUILayout->addWidget(updateParamInfoBtn, row++ % rowCount, columnCount - 1, 1, 1);


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
	//��������
	DeviceCommonVaries* commonVaries = DeviceCommonVaries::getInstance();
	userSelectType->clear();
	for (auto ele = commonVaries->paramType.begin(); ele != commonVaries->paramType.end(); ele++)
	{
		userSelectType->addItem(QString::fromLocal8Bit(ele->first.c_str()));
	}
	//������λ
	userSelcetUnit->clear();
	for (auto ele : commonVaries->paramUnit)
	{
		userSelcetUnit->addItem(QString::fromLocal8Bit(ele.c_str()));
	}

	//���ʲ�����parameter_info��״ֵ̬��switch_value
	auto deviceManageDbOp = DeviceDBConfigInfo::getInstance();
	deviceManageDbOp->readParamDB2UI();
	userSlctSwitchShowParamID->clear();
	for (auto ele = deviceManageDbOp->paramInfo.begin(); ele != deviceManageDbOp->paramInfo.end(); ele++)
	{
		userSlctSwitchShowParamID->addItem(QString(ele->second[1].c_str()), QString(ele->second[0].c_str()).toInt());
	}
	deviceManageDbOp->readSwitchValueDB2UI();
	userSlctSwitchValID->clear();
	for (auto ele = deviceManageDbOp->switchValueInfo.begin(); ele != deviceManageDbOp->switchValueInfo.end(); ele++)
	{
		userSlctSwitchValID->addItem(QString(ele->second[1].c_str()), QString(ele->second[0].c_str()).toInt());
	}

}

void InfoConfigWidget::initConnect() {

	connect(paramOKBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickParamOKBtn);

	connect(switchValOKBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickSwitchValOKBtn);

	connect(switchShowInfoOKBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickSwitchShowInfoOKBtn);

	connect(updateParamInfoBtn, &QPushButton::clicked, this, &InfoConfigWidget::clickUpdateParamBtn);


	connect(userSlctSwitchValID, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, [=]() {
		int top1 = userSlctSwitchValID->currentText().toInt();
		top1 = DeviceCommonVaries::getInstance()->statusIndex[top1].size();
		userInputStatusVal->setValidator(new QIntValidator(0, top1));
		});
}

void InfoConfigWidget::setConfigHeader(QString header) {

}

/**
	@brief ȷ�ϲ�������
**/
void InfoConfigWidget::clickParamOKBtn() {
	QString paramName = userInputParamName->text();
	if (paramName == "")
	{
		QMessageBox::information(this, "info", QString("����������Ϊ�գ�"));
		return;
	}
	int paramTypeIndex = userSelectType->currentIndex() + 1;
	QString paramUnit = userSelcetUnit->currentText();

	DeviceDBConfigInfo::getInstance()->paramConfigOp2DB(paramName, paramTypeIndex, paramUnit);
	DeviceDBConfigInfo::getInstance()->readParamDB2UI();
	widgetConfig();
	emit updateParams();
}

/**
	@brief ȷ��switch_value��Ϣ����
**/
void InfoConfigWidget::clickSwitchValOKBtn() {

	QString switchValName = userInputSwitchVal->text();
	if (switchValName == "")
	{
		QMessageBox::information(this, "info", QString("���������Ʋ���Ϊ�գ�"));
		return;
	}
	DeviceDBConfigInfo::getInstance()->paramSwitchVal2DB(switchValName);
	DeviceDBConfigInfo::getInstance()->readSwitchValueDB2UI();
	widgetConfig();
}

/**
	@brief ȷ��switch_value��Ϣ����
**/
void InfoConfigWidget::clickSwitchShowInfoOKBtn() {

	int bindParamID = userSlctSwitchShowParamID->itemData(userSlctSwitchShowParamID->currentIndex()).toInt();
	int bindSwitchValID = userSlctSwitchValID->itemData(userSlctSwitchValID->currentIndex()).toInt();
	int statusIndex = userInputStatusVal->text().toInt();
	DeviceDBConfigInfo::getInstance()->paramSwitchShowInfo2DB(bindParamID, bindSwitchValID, statusIndex);
	DeviceDBConfigInfo::getInstance()->readSwitchShowInfoDB2UI();
	widgetConfig();
}

/**
	@brief ȷ�����������������ã����¸�������
**/
void InfoConfigWidget::clickUpdateParamBtn() {

	this->close();

}