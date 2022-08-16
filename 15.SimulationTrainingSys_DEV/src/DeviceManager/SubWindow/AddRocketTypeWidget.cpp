#include "AddRocketTypeWidget.h"

AddRocketTypeWidget* AddRocketTypeWidget::instance = nullptr;

AddRocketTypeWidget::AddRocketTypeWidget(QWidget* parent)
	: QWidget(parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);

	windowName = "ÐÂÔö»ð¼ýÐÍºÅ";
	InitUILayout();

	connect(windowCloseBtn, &QPushButton::clicked, this, &AddRocketTypeWidget::clickWindowClose);
	connect(rocketTypeCancelBtn, &QPushButton::clicked, this, &AddRocketTypeWidget::clickRocketTypeCancel);
	connect(rocketTypeOkBtn, &QPushButton::clicked, this, &AddRocketTypeWidget::clickRocketTypeOk);

}

AddRocketTypeWidget::~AddRocketTypeWidget()
{}

/**
	@brief ³õÊ¼»¯²¼¾Ö
**/
void AddRocketTypeWidget::InitUILayout() {

	int row = 0;
	int column = 0;
	int rowC = 8;
	int columnC = 8;

	QGridLayout* UIGrid = new QGridLayout;

	windowIcon = new QLabel;
	windowTitle = new QLabel(windowName);
	windowCloseBtn = new QPushButton;

	rocketName = new QLabel(QString("»ð¼ýÐÍºÅÃû³Æ:"));
	userInputRocketName = new QLineEdit;
	userInputRocketName->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	rocketDescript = new QLabel(QString("ÃèÊö:"));
	userInputDescript = new QLineEdit;
	userInputDescript->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	rocketTypeCancelBtn = new QPushButton(QString("È¡Ïû"));
	rocketTypeOkBtn = new QPushButton(QString("È·¶¨"));

	UIGrid->addWidget(windowIcon, row, column++);
	UIGrid->addWidget(windowTitle, row, column, 1, 2);
	UIGrid->addWidget(windowCloseBtn, row++, columnC - 1);

	column = 0;
	QFrame* hframe1 = new QFrame;
	hframe1->setFrameShape(QFrame::HLine);
	hframe1->setFixedWidth(640);


	UIGrid->addWidget(hframe1, row++, column++, 1, columnC);
	UIGrid->setContentsMargins(0, 12, 12, 12);

	column = 2;
	UIGrid->addWidget(rocketName, row++, column, 1, 3);
	UIGrid->addWidget(userInputRocketName, row++, column, 1, 3);
	UIGrid->addWidget(rocketDescript, row++, column, 1, 3);
	UIGrid->addWidget(userInputDescript, row++, column, 1, 3);

	UIGrid->setRowStretch(row, 3);

	UIGrid->addWidget(rocketTypeCancelBtn, rowC - 1, columnC - 2, 1, 1);
	UIGrid->addWidget(rocketTypeOkBtn, rowC - 1, columnC - 1, 1, 1);



	this->setLayout(UIGrid);
	this->setFixedSize(640, 640);
	this->setContentsMargins(0, 0, 0, 0);



	userInputDescript->setStyleSheet("QLineEdit{border:2px groove gray;border-radius:5px;padding:2px 4px;font: 12pt Î¢ÈíÑÅºÚ;}");
	userInputRocketName->setStyleSheet("QLineEdit{border:2px groove gray;border-radius:5px;padding:2px 4px;font: 12pt Î¢ÈíÑÅºÚ;}");
	this->setStyleSheet("*{background-color:rgba(255,255,255,1);}");
	rocketTypeCancelBtn->setStyleSheet("\
				\
				QPushButton:hover{background-color:transparent;\
					border-image:url(:/DeviceManager/bt_suspension.png);\
					color:rgb(255,255,255);\
					font:12pt Î¢ÈíÑÅºÚ;\
					border:0px;\
					}\
				QPushButton:pressed{\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_disable.png);\
					color:rgb(255,255,255);\
					font:12pt Î¢ÈíÑÅºÚ;\
					border:0px;}\
				QPushButton{\
					height:30px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt Î¢ÈíÑÅºÚ;\
					border:0px;\
					}\
		");
	rocketTypeOkBtn->setStyleSheet("\
				\
				QPushButton:hover{background-color:transparent;\
					border-image:url(:/DeviceManager/bt_suspension.png);\
					color:rgb(255,255,255);\
					font:12pt Î¢ÈíÑÅºÚ;\
					border:0px;\
					}\
				QPushButton:pressed{\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_disable.png);\
					color:rgb(255,255,255);\
					font:12pt Î¢ÈíÑÅºÚ;\
					border:0px;}\
				QPushButton{\
					height:30px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt Î¢ÈíÑÅºÚ;\
					border:0px;\
					}\
		");

	windowIcon->setStyleSheet("image:url(:/icon/icon/bb.png);left:12px;height:30px;");
	windowCloseBtn->setStyleSheet("QPushButton{height:30px;image:url(:/DeviceManager/images/close_normal.png);image-position:right;background-color:rgba(255,255,255,1);border:0px;}\
			QPushButton:hover{image:url(:/DeviceManager/images/¹Ø±Õ-Ðü¸¡.png);}\
			QPushButton:pressed{image:url(:/DeviceManager/images/¹Ø±Õ-µã»÷.png);}");


}

/**
	@brief ÏÔÊ¾»ð¼ýÐÍºÅÅäÖÃ
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
	@brief ÏÔÊ¾²ÎÊýÅäÖÃ
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
	@brief ÏÔÊ¾Éè±¸ÅäÖÃ
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
	@brief ÏÔÊ¾»ð¼ýÖ¸ÁîÅäÖÃ
	@param flag -
**/
void AddRocketTypeWidget::commandInfoDisplay(bool flag) {
	if (flag)
	{
		rocketType->show();
		userSelectRocketType->show();
		commandType->show();
		userSelectCmdType->show();
		
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
	@brief ½çÃæÅäÖÃ
**/
void AddRocketTypeWidget::widgetConfig() {

}

/**
	@brief ³õÊ¼»¯ÐÅºÅÓë²Û
**/
void AddRocketTypeWidget::initConnect() {

}

/**
	@brief È¡Ïû°´Å¥²Ûº¯Êý
**/
void AddRocketTypeWidget::clickRocketTypeCancel() {
	instance->close();
}

/**
	@brief È·¶¨°´Å¥²Ûº¯Êý
**/
void AddRocketTypeWidget::clickRocketTypeOk() {
	instance->close();
}

/**
	@brief µã»÷¹Ø±Õ°´Å¥²Ûº¯Êý
**/
void AddRocketTypeWidget::clickWindowClose() {

	instance->close();
}



/**
	@brief Êó±ê°´ÏÂ²Ù×÷
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
	@brief Êó±êÒÆ¶¯²Ù×÷
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
	@brief Êó±êÊÍ·Å²Ù×÷
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