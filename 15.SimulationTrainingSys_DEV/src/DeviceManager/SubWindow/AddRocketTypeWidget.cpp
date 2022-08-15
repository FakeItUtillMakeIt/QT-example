#include "AddRocketTypeWidget.h"

AddRocketTypeWidget* AddRocketTypeWidget::instance = nullptr;

AddRocketTypeWidget::AddRocketTypeWidget(QWidget* parent)
	: QWidget(parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);

	windowName = "��������ͺ�";
	InitUILayout();

	connect(windowCloseBtn, &QPushButton::clicked, this, &AddRocketTypeWidget::clickWindowClose);
	connect(rocketTypeCancelBtn, &QPushButton::clicked, this, &AddRocketTypeWidget::clickRocketTypeCancel);
	connect(rocketTypeOkBtn, &QPushButton::clicked, this, &AddRocketTypeWidget::clickRocketTypeOk);

}

AddRocketTypeWidget::~AddRocketTypeWidget()
{}

/**
	@brief ��ʼ������
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

	rocketName = new QLabel(QString("����ͺ�����:"));
	userInputRocketName = new QLineEdit;
	userInputRocketName->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	rocketDescript = new QLabel(QString("����:"));
	userInputDescript = new QLineEdit;
	userInputDescript->setValidator(new QRegExpValidator(QRegExp("\\S+")));

	rocketTypeCancelBtn = new QPushButton(QString("ȡ��"));
	rocketTypeOkBtn = new QPushButton(QString("ȷ��"));

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



	userInputDescript->setStyleSheet("QLineEdit{border:2px groove gray;border-radius:5px;padding:2px 4px;font: 12pt ΢���ź�;}");
	userInputRocketName->setStyleSheet("QLineEdit{border:2px groove gray;border-radius:5px;padding:2px 4px;font: 12pt ΢���ź�;}");
	this->setStyleSheet("*{background-color:rgba(255,255,255,1);}");
	rocketTypeCancelBtn->setStyleSheet("\
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
					height:30px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;\
					}\
		");
	rocketTypeOkBtn->setStyleSheet("\
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
					height:30px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;\
					}\
		");

	windowIcon->setStyleSheet("image:url(:/icon/icon/bb.png);left:12px;height:30px;");
	windowCloseBtn->setStyleSheet("QPushButton{height:30px;image:url(:/DeviceManager/images/close_normal.png);image-position:right;background-color:rgba(255,255,255,1);border:0px;}\
			QPushButton:hover{image:url(:/DeviceManager/images/�ر�-����.png);}\
			QPushButton:pressed{image:url(:/DeviceManager/images/�ر�-���.png);}");


}

/**
	@brief ��������
**/
void AddRocketTypeWidget::widgetConfig() {

}

/**
	@brief ��ʼ���ź����
**/
void AddRocketTypeWidget::initConnect() {

}

/**
	@brief ȡ����ť�ۺ���
**/
void AddRocketTypeWidget::clickRocketTypeCancel() {
	instance->close();
}

/**
	@brief ȷ����ť�ۺ���
**/
void AddRocketTypeWidget::clickRocketTypeOk() {
	instance->close();
}

void AddRocketTypeWidget::clickWindowClose() {

	instance->close();
}