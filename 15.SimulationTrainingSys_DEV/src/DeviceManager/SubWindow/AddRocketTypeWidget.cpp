#include "AddRocketTypeWidget.h"

AddRocketTypeWidget* AddRocketTypeWidget::instance = nullptr;

AddRocketTypeWidget::AddRocketTypeWidget(QWidget* parent)
	: QWidget(parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);


	windowName = "新增火箭型号";
	InitUILayout();

	connect(windowCloseBtn, &QPushButton::clicked, this, &AddRocketTypeWidget::clickWindowClose);
	connect(CancelBtn, &QPushButton::clicked, this, &AddRocketTypeWidget::clickRocketTypeCancel);
	connect(OkBtn, &QPushButton::clicked, this, &AddRocketTypeWidget::clickRocketTypeOk);

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
	paramName = new QLabel(QString("参数名称:"));
	userInputParamName = new QLineEdit;
	paramType = new QLabel(QString("参数类型:"));
	userSelectParamType = new QComboBox;
	paramUnit = new QLabel(QString("参数单位:"));
	userInputParamUnit = new QLineEdit;
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
	UIGrid->addWidget(paramName, row++, column, 1, 3);
	UIGrid->addWidget(userInputParamName, row++, column, 1, 3);
	UIGrid->addWidget(paramType, row++, column, 1, 3);
	UIGrid->addWidget(userSelectParamType, row++, column, 1, 3);
	UIGrid->addWidget(paramUnit, row++, column, 1, 3);
	UIGrid->addWidget(userInputParamUnit, row++, column, 1, 3);
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

	UIGrid->setRowStretch(row, 3);

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

	windowIcon->setStyleSheet("image:url(:/icon/icon/bb.png);left:6px;height:25px;");
	windowCloseBtn->setStyleSheet("QPushButton{height:25px;image:url(:/DeviceManager/images/close_normal.png);image-position:right;background-color:rgba(255,255,255,1);border:0px;}\
			QPushButton:hover{image:url(:/DeviceManager/images/关闭-悬浮.png);}\
			QPushButton:pressed{image:url(:/DeviceManager/images/关闭-点击.png);}");


}

void AddRocketTypeWidget::setWindowName(QString name1) {
	windowTitle->setText(name1);
}

/**
	@brief 区别为那个窗口
	@param type -
**/
void AddRocketTypeWidget::setInfoWidget(int type) {
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
	@brief 显示设备配置
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
	@brief 显示火箭指令配置
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
	@brief 界面配置
**/
void AddRocketTypeWidget::widgetConfig() {

}

/**
	@brief 初始化信号与槽
**/
void AddRocketTypeWidget::initConnect() {

}

/**
	@brief 取消按钮槽函数
**/
void AddRocketTypeWidget::clickRocketTypeCancel() {
	instance->close();
}

/**
	@brief 确定按钮槽函数
**/
void AddRocketTypeWidget::clickRocketTypeOk() {
	instance->close();
}

/**
	@brief 点击关闭按钮槽函数
**/
void AddRocketTypeWidget::clickWindowClose() {

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