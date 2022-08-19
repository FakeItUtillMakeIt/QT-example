#include "AllInfoConfigWidget.h"

AllInfoConfigWidget* AllInfoConfigWidget::instance = nullptr;

AllInfoConfigWidget::AllInfoConfigWidget(QWidget* parent)
	: QWidget(parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);

	InitUILayout();

	initConnect();

}

AllInfoConfigWidget::~AllInfoConfigWidget()
{}

/**
	@brief
	@retval  -
**/
QLayout* AllInfoConfigWidget::publicTopLayout() {
	QHBoxLayout* topLayout = new QHBoxLayout;
	QVBoxLayout* topLineVlay = new QVBoxLayout;

	topLayout->addWidget(windowIcon);
	topLayout->addWidget(windowTitle);
	topLayout->addSpacing(this->width() - 250);
	topLayout->addWidget(windowCloseBtn, 0, Qt::AlignRight);

	hframe1 = new QFrame;
	hframe1->setFrameShape(QFrame::HLine);
	//	hframe1->setFixedWidth(640);
	hframe1->setBackgroundRole(QPalette::Background);

	topLineVlay->addLayout(topLayout);
	topLineVlay->addWidget(hframe1);


	windowCloseBtn->setStyleSheet("QPushButton{height:25px;image:url(:/DeviceManager/images/close_normal.png);image-position:right;background-color:rgba(255,255,255,1);border:0px;}\
			QPushButton:hover{image:url(:/DeviceManager/images/¹Ø±Õ-Ðü¸¡.png);}\
			QPushButton:pressed{image:url(:/DeviceManager/images/¹Ø±Õ-µã»÷.png);}");
	windowIcon->setStyleSheet("height:25px;width:25px;image:url(:/icon/icon/bb.png);left:12px;");

	topLayout->setContentsMargins(0, 0, 12, 0);
	topLineVlay->setContentsMargins(0, 0, 0, 0);

	return topLineVlay;

}
/**
	@brief
	@retval  -
**/
QLayout* AllInfoConfigWidget::publicBottomLayout() {
	QHBoxLayout* bottomLayout = new QHBoxLayout;
	QVBoxLayout* bottomLineVlay = new QVBoxLayout;

	hframe2 = new QFrame;
	hframe2->setFrameShape(QFrame::HLine);
	//	hframe1->setFixedWidth(640);
	hframe2->setBackgroundRole(QPalette::Background);
	bottomLineVlay->addWidget(hframe2);
	bottomLineVlay->addSpacing(10);

	bottomLayout->addSpacing(this->width() - 200);
	bottomLayout->addWidget(cancelBtn, 1, Qt::AlignRight);
	bottomLayout->addWidget(okBtn, 1, Qt::AlignRight);

	bottomLineVlay->addLayout(bottomLayout);

	cancelBtn->setStyleSheet("\
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
					height:30px;width:60px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt Î¢ÈíÑÅºÚ;\
					border:0px;\
					}\
		");
	okBtn->setStyleSheet("\
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
					height:30px;width:60px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt Î¢ÈíÑÅºÚ;\
					border:0px;\
					}\
		");


	bottomLineVlay->setContentsMargins(0, 0, 0, 0);
	bottomLayout->setContentsMargins(12, 0, 12, 12);

	return bottomLineVlay;

}


/**
	@brief ³õÊ¼»¯½çÃæ
**/
QLayout* AllInfoConfigWidget::initRocketConfigLayout() {
	windowTitle->setText("»ð¼ýÐÍºÅÅäÖÃ");

	UIGrid = new QGridLayout;

	QGridLayout* leftGrid = new QGridLayout;
	QGridLayout* rightGrid = new QGridLayout;

	QHBoxLayout* midUILayout = new QHBoxLayout;

	int rowC = 8;
	int columnC = 4;
	int row = 0;
	int column = 0;
	//×ó²à
	leftGrid->addWidget(rocketTypeParamTitle, row, column++, Qt::AlignLeft);
	leftGrid->addWidget(searchLineEdit, row++, columnC - 1, Qt::AlignRight);
	column = 0;
	leftGrid->addWidget(deviceParamTree, row, column, rowC - 2, columnC);
	leftGrid->addWidget(selectAllBox, rowC - 1, column++);
	leftGrid->addWidget(cancelAllBox, rowC - 1, column);

	//ÓÒ²à
	row = 0; column = 0;
	rightGrid->addWidget(selectParamTitle, row, column, Qt::AlignLeft);
	rightGrid->addWidget(searchSelect, row, columnC - 1, Qt::AlignRight);
	row++;
	rightGrid->addWidget(hadSelectedParamsL, row, column, rowC - 1, columnC);


	midUILayout->addLayout(leftGrid);
	midUILayout->addLayout(rightGrid);
	midUILayout->setContentsMargins(12, 12, 12, 0);

	searchLineEdit->setPlaceholderText(QString("ËÑË÷"));
	searchSelect->setPlaceholderText(QString("ËÑË÷"));


	UIGrid->addLayout(publicTopLayout(), 0, 0, 1, 16);
	UIGrid->addLayout(midUILayout, 1, 0, 1, 16);
	UIGrid->addLayout(publicBottomLayout(), 2, 0, 1, 16);

	UIGrid->setContentsMargins(0, 12, 0, 0);


	this->setLayout(UIGrid);
	return UIGrid;
}

/**
	@brief
	@retval  -
**/
QLayout* AllInfoConfigWidget::initDeviceConfigLayout() {
	windowTitle->setText("Éè±¸ÅäÖÃ");

	UIGrid = new QGridLayout;

	QGridLayout* midUILayout = new QGridLayout;

	UIGrid->addLayout(publicTopLayout(), 0, 0, 1, 16);
	UIGrid->addLayout(midUILayout, 1, 0, 1, 16);
	UIGrid->addLayout(publicBottomLayout(), 2, 0, 1, 16);

	UIGrid->setContentsMargins(0, 12, 0, 0);


	this->setLayout(UIGrid);
	return UIGrid;
}

/**
	@brief
	@retval  -
**/
QLayout* AllInfoConfigWidget::initCommandConfigLayout() {
	windowTitle->setText("Ö¸ÁîÅäÖÃ");

	UIGrid = new QGridLayout;
	QGridLayout* midUILayout = new QGridLayout;

	UIGrid->addLayout(publicTopLayout(), 0, 0, 1, 16);
	UIGrid->addLayout(midUILayout, 1, 0, 1, 16);
	UIGrid->addLayout(publicBottomLayout(), 2, 0, 1, 16);

	UIGrid->setContentsMargins(0, 12, 0, 0);


	this->setLayout(UIGrid);
	return UIGrid;
}


/**
	@brief ³õÊ¼»¯½çÃæ²¼¾Ö
**/
void AllInfoConfigWidget::InitUILayout() {
	this->setFixedSize(QSize(1200, 940));

	this->setStyleSheet("*{background-color:rgba(255,255,255,1);padding:2px;}\
			QLineEdit{background-color:rgba(238, 240, 243, 1);border:2px groove gray;border-radius:10px;padding:2px 4px;font: 12pt Î¢ÈíÑÅºÚ;}\
			QLabel{font: 12pt Î¢ÈíÑÅºÚ;}\
			QComboBox{border: 1px solid darkgray;font: 10pt Arial;selection - background - color: rgb(0, 170, 255);font: 12pt Î¢ÈíÑÅºÚ; }");


	//¹«¹²ÉÏ²à
	windowIcon = new QLabel;
	windowTitle = new QLabel("test");
	windowCloseBtn = new QPushButton;

	//¹«¹²ÏÂ²à
	cancelBtn = new QPushButton(QString("È¡Ïû"));
	okBtn = new QPushButton(QString("È·¶¨"));
	/*!
	 *  Inits the u i layout.»ð¼ýÐÍºÅ
	 */
	 //×ó²à
	rocketTypeParamTitle = new QLabel(QString("ÐÍºÅ²ÎÊý"));
	searchLineEdit = new QLineEdit;
	searchLineEdit->setStatusTip(QString("ËÑË÷"));
	deviceParamTree = new QTreeWidget;
	selectAllBox = new QCheckBox;
	selectAllBox->setText(QString("È«Ñ¡"));
	cancelAllBox = new QCheckBox;
	cancelAllBox->setText(QString("·´Ñ¡"));
	//ÓÒ²à
	selectParamTitle = new QLabel(QString("ÒÑÑ¡Ôñ²ÎÊý"));
	searchSelect = new QLineEdit;
	searchSelect->setStatusTip(QString("ËÑË÷"));
	hadSelectedParamsL = new QListWidget;

	/*!
	 *  Inits the u i layout.Éè±¸ÅäÖÃ
	 */
	 //×ó
	searchDeviceCfg = new QLineEdit;
	searchDeviceCfg->setPlaceholderText(QString("ËÑË÷"));
	addDeviceCfg = new QPushButton;
	deviceCfgList = new QListWidget;
	//ÓÒ1
	devStatusLayout = new QGridLayout;
	//ÓÒ2
	searchDevParam = new QLineEdit;
	searchDevParam->setPlaceholderText(QString("ËÑË÷"));
	devParamLayout = new QGridLayout;

	/*!
	 *  Inits the u i layout.Ö¸ÁîÅäÖÃ
	 */
	 //×ó²à
	searchCmdCfg = new QLineEdit;
	searchCmdCfg->setPlaceholderText(QString("ËÑË÷"));
	addCmdCfg = new QPushButton;
	cmdCfgList = new QListWidget;
	//ÓÒ1
	deviceLabel = new QLabel(QString("Éè±¸"));
	deviceCombox = new QComboBox;
	deviceStatLayout = new QGridLayout;
	//ÓÒ2
	searchCmdFrame = new QLineEdit;
	searchCmdFrame->setPlaceholderText(QString("ËÑË÷"));
	cmdFrameLabel = new QLabel(QString("Ö¸ÁîÖ¡ÄÚÈÝÁÐ±í"));
	addCmdFrame = new QPushButton(QString("ÐÂÔö"));
	cmdFrameTable = new QTableWidget;


	//initRocketConfigLayout();
	initDeviceConfigLayout();
}

/**
	@brief	  ¸ù¾Ý´«ÈëµÄ´°¿ÚÀàÐÍ¸ü»»UI
	@param switch_on -
**/
void AllInfoConfigWidget::setCurrentUI(DeviceCommonVaries::InfoWidgetFlag switch_on) {

	switch (switch_on)
	{
	case DeviceCommonVaries::PARAM_WIDGET:
		initRocketConfigLayout();
		break;
	case DeviceCommonVaries::DEVICE_WIDGET:
		initDeviceConfigLayout();
		break;
	case DeviceCommonVaries::COMMAND_WIDGET:
		initCommandConfigLayout();
		break;
	default:
		break;
	}

}

/**
	@brief
**/
void AllInfoConfigWidget::widgetConfig() {

}

/**
	@brief ³õÊ¼»¯ÐÅºÅÓë²Û
**/
void AllInfoConfigWidget::initConnect() {

	connect(windowCloseBtn, &QPushButton::clicked, this, [=]() {
		instance->close();
		});

	connect(cancelBtn, &QPushButton::clicked, this, &AllInfoConfigWidget::clickedCancelBtn);
	connect(okBtn, &QPushButton::clicked, this, &AllInfoConfigWidget::clickedOkBtn);

	connect(searchLineEdit, &QLineEdit::editingFinished, this, [=]() {

		});

	connect(searchSelect, &QLineEdit::editingFinished, this, [=]() {

		});

}

/**
	@brief
**/
void AllInfoConfigWidget::clickedCancelBtn() {

	this->close();
}

/**
	@brief
**/
void AllInfoConfigWidget::clickedOkBtn() {

	this->close();
}

/**
	@brief Êó±ê°´ÏÂ²Ù×÷
	@param event -
**/
void AllInfoConfigWidget::mousePressEvent(QMouseEvent* event)
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
void AllInfoConfigWidget::mouseMoveEvent(QMouseEvent* event)
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
void AllInfoConfigWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = false;
		setCursor(Qt::ArrowCursor);
	}
}