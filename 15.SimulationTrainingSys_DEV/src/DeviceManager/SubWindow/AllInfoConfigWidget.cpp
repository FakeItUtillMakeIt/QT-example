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
			QPushButton:hover{image:url(:/DeviceManager/images/�ر�-����.png);}\
			QPushButton:pressed{image:url(:/DeviceManager/images/�ر�-���.png);}");
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
					height:30px;width:60px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;\
					}\
		");
	okBtn->setStyleSheet("\
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
					height:30px;width:60px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;\
					}\
		");


	bottomLineVlay->setContentsMargins(0, 0, 0, 0);
	bottomLayout->setContentsMargins(12, 0, 12, 12);

	return bottomLineVlay;

}


/**
	@brief ��ʼ������
**/
QLayout* AllInfoConfigWidget::initRocketConfigLayout() {
	windowTitle->setText("����ͺ�����");

	UIGrid = new QGridLayout;

	QGridLayout* leftGrid = new QGridLayout;
	QGridLayout* rightGrid = new QGridLayout;

	QHBoxLayout* midUILayout = new QHBoxLayout;

	int rowC = 8;
	int columnC = 4;
	int row = 0;
	int column = 0;
	//���
	leftGrid->addWidget(rocketTypeParamTitle, row, column++, Qt::AlignLeft);
	leftGrid->addWidget(searchLineEdit, row++, columnC - 1, Qt::AlignRight);
	column = 0;
	leftGrid->addWidget(deviceParamTree, row, column, rowC - 2, columnC);
	leftGrid->addWidget(selectAllBox, rowC - 1, column++);
	leftGrid->addWidget(cancelAllBox, rowC - 1, column);

	//�Ҳ�
	row = 0; column = 0;
	rightGrid->addWidget(selectParamTitle, row, column, Qt::AlignLeft);
	rightGrid->addWidget(searchSelect, row, columnC - 1, Qt::AlignRight);
	row++;
	rightGrid->addWidget(hadSelectedParamsL, row, column, rowC - 1, columnC);


	midUILayout->addLayout(leftGrid);
	midUILayout->addLayout(rightGrid);
	midUILayout->setContentsMargins(12, 12, 12, 0);

	searchLineEdit->setPlaceholderText(QString("����"));
	searchSelect->setPlaceholderText(QString("����"));


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
	windowTitle->setText("�豸����");

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
	windowTitle->setText("ָ������");

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
	@brief ��ʼ�����沼��
**/
void AllInfoConfigWidget::InitUILayout() {
	this->setFixedSize(QSize(1200, 940));

	this->setStyleSheet("*{background-color:rgba(255,255,255,1);padding:2px;}\
			QLineEdit{background-color:rgba(238, 240, 243, 1);border:2px groove gray;border-radius:10px;padding:2px 4px;font: 12pt ΢���ź�;}\
			QLabel{font: 12pt ΢���ź�;}\
			QComboBox{border: 1px solid darkgray;font: 10pt Arial;selection - background - color: rgb(0, 170, 255);font: 12pt ΢���ź�; }");


	//�����ϲ�
	windowIcon = new QLabel;
	windowTitle = new QLabel("test");
	windowCloseBtn = new QPushButton;

	//�����²�
	cancelBtn = new QPushButton(QString("ȡ��"));
	okBtn = new QPushButton(QString("ȷ��"));
	/*!
	 *  Inits the u i layout.����ͺ�
	 */
	 //���
	rocketTypeParamTitle = new QLabel(QString("�ͺŲ���"));
	searchLineEdit = new QLineEdit;
	searchLineEdit->setStatusTip(QString("����"));
	deviceParamTree = new QTreeWidget;
	selectAllBox = new QCheckBox;
	selectAllBox->setText(QString("ȫѡ"));
	cancelAllBox = new QCheckBox;
	cancelAllBox->setText(QString("��ѡ"));
	//�Ҳ�
	selectParamTitle = new QLabel(QString("��ѡ�����"));
	searchSelect = new QLineEdit;
	searchSelect->setStatusTip(QString("����"));
	hadSelectedParamsL = new QListWidget;

	/*!
	 *  Inits the u i layout.�豸����
	 */
	 //��
	searchDeviceCfg = new QLineEdit;
	searchDeviceCfg->setPlaceholderText(QString("����"));
	addDeviceCfg = new QPushButton;
	deviceCfgList = new QListWidget;
	//��1
	devStatusLayout = new QGridLayout;
	//��2
	searchDevParam = new QLineEdit;
	searchDevParam->setPlaceholderText(QString("����"));
	devParamLayout = new QGridLayout;

	/*!
	 *  Inits the u i layout.ָ������
	 */
	 //���
	searchCmdCfg = new QLineEdit;
	searchCmdCfg->setPlaceholderText(QString("����"));
	addCmdCfg = new QPushButton;
	cmdCfgList = new QListWidget;
	//��1
	deviceLabel = new QLabel(QString("�豸"));
	deviceCombox = new QComboBox;
	deviceStatLayout = new QGridLayout;
	//��2
	searchCmdFrame = new QLineEdit;
	searchCmdFrame->setPlaceholderText(QString("����"));
	cmdFrameLabel = new QLabel(QString("ָ��֡�����б�"));
	addCmdFrame = new QPushButton(QString("����"));
	cmdFrameTable = new QTableWidget;


	//initRocketConfigLayout();
	initDeviceConfigLayout();
}

/**
	@brief	  ���ݴ���Ĵ������͸���UI
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
	@brief ��ʼ���ź����
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
	@brief ��갴�²���
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
	@brief ����ƶ�����
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
	@brief ����ͷŲ���
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