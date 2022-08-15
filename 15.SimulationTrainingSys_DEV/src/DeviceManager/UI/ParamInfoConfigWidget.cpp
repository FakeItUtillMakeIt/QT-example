#include "ParamInfoConfigWidget.h"

using namespace ParamInfoConfig;

InfoConfigWidget* InfoConfigWidget::instance = nullptr;

InfoConfigWidget::InfoConfigWidget(QWidget* parent)
	: QWidget(parent)
{
	this->setWindowTitle(QString("添加参数"));
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


	configParamName = new QLabel(QString("参数名称:"));
	userInputParamName = new QLineEdit;
	userInputParamName->setValidator(new QRegExpValidator(QRegExp("\\S+")));
	configParamType = new QLabel(QString("参数类型:"));
	userSelectType = new QComboBox;
	configParamUnit = new QLabel(QString("参数单位:"));
	userSelcetUnit = new QComboBox;
	//paramCancelBtn = new QPushButton(QString("取消"));
	paramOKBtn = new QPushButton(QString("新增参数"));
	//!< 	开关量状态值
	configSwitchValName = new QLabel(QString("开关量名称:"));;
	userInputSwitchVal = new QLineEdit;
	userInputSwitchVal->setValidator(new QRegExpValidator(QRegExp("\\S+")));
	//switchValCancelBtn = new QPushButton(QString("取消"));;
	switchValOKBtn = new QPushButton(QString("新增开关量"));
	//!< 	开关量显示值
	configSwitchShowParamID = new QLabel(QString("参数ID:"));
	userSlctSwitchShowParamID = new QComboBox;
	configSwitchValID = new QLabel(QString("状态值ID:"));
	userSlctSwitchValID = new QComboBox;
	configStatusVal = new QLabel(QString("状态索引:"));
	userInputStatusVal = new QLineEdit;

	userInputStatusVal->setValidator(new QIntValidator(0, 3));
	//switchShowInfoCancelBtn = new QPushButton(QString("取消"));
	switchShowInfoOKBtn = new QPushButton(QString("新增开关量显示值"));

	updateParamInfoBtn = new QPushButton(QString("取消"));

	QGridLayout* infoUILayout = new QGridLayout;
	int rowCount = 40;
	int columnCount = 4;
	int row = 0;
	int column = 0;

	infoUILayout->addWidget(new QLabel(QString("参数配置")), row++, 0);

	QFrame* hframe0 = new QFrame;
	hframe0->setFrameShape(QFrame::HLine);
	//hframe0->setStyleSheet("background:gray;min-height:1px");

	infoUILayout->addWidget(hframe0, row++, 0, 1, columnCount);
	//第一行
	column = 0;
	infoUILayout->addWidget(configParamName, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userInputParamName, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(configParamType, row % rowCount, column++ % columnCount, 1, 1);
	infoUILayout->addWidget(userSelectType, row++ % rowCount, column++ % columnCount, 1, 1);
	//第二行
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

	infoUILayout->addWidget(new QLabel(QString("开关量状态值配置")), row++, 0);
	QFrame* hframe1 = new QFrame;
	hframe1->setFrameShape(QFrame::HLine);
	infoUILayout->addWidget(hframe1, row++, 0, 1, columnCount);

	//第三行
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

	infoUILayout->addWidget(new QLabel(QString("开关量显示值配置")), row++, 0);
	QFrame* hframe2 = new QFrame;
	hframe2->setFrameShape(QFrame::HLine);
	infoUILayout->addWidget(hframe2, row++, 0, 1, columnCount);
	//第四行
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
	@brief 控件配置
**/
void InfoConfigWidget::widgetConfig() {
	//参数类型
	DeviceCommonVaries* commonVaries = DeviceCommonVaries::getInstance();
	userSelectType->clear();
	for (auto ele = commonVaries->paramType.begin(); ele != commonVaries->paramType.end(); ele++)
	{
		userSelectType->addItem(QString::fromLocal8Bit(ele->first.c_str()));
	}
	//参数单位
	userSelcetUnit->clear();
	for (auto ele : commonVaries->paramUnit)
	{
		userSelcetUnit->addItem(QString::fromLocal8Bit(ele.c_str()));
	}

	//访问参数表parameter_info和状态值表switch_value
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
	@brief 确认参数配置
**/
void InfoConfigWidget::clickParamOKBtn() {
	QString paramName = userInputParamName->text();
	if (paramName == "")
	{
		QMessageBox::information(this, "info", QString("参数名不能为空！"));
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
	@brief 确认switch_value信息配置
**/
void InfoConfigWidget::clickSwitchValOKBtn() {

	QString switchValName = userInputSwitchVal->text();
	if (switchValName == "")
	{
		QMessageBox::information(this, "info", QString("开关量名称不能为空！"));
		return;
	}
	DeviceDBConfigInfo::getInstance()->paramSwitchVal2DB(switchValName);
	DeviceDBConfigInfo::getInstance()->readSwitchValueDB2UI();
	widgetConfig();
}

/**
	@brief 确认switch_value信息配置
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
	@brief 确认整个参数部分配置，更新父窗口行
**/
void InfoConfigWidget::clickUpdateParamBtn() {

	this->close();

}