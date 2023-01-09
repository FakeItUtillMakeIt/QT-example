#include "BMapEmbed.h"

BMapEmbed::BMapEmbed(QWidget* parent) :QWidget(parent)
, m_jsCommWithQt(nullptr)
, bmapWidget(nullptr)
, bmapOpWindow(nullptr)
{
	initWindow();
	initConnect();
}

BMapEmbed::~BMapEmbed()
{
	commThread.quit();
	commThread.wait();
}

void BMapEmbed::initWindow() {
	//
	m_jsCommWithQt = new JsCommWithQt();

	m_jsCommWithQt->moveToThread(&commThread);
	commThread.start();

	//加载webchannel.js
	qDebug() << "BMap" << this->geometry();

	view = new QWebEngineView();

	view->setMinimumWidth(this->width() - 200);
	view->setMinimumHeight(this->height());
	view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	channel = new QWebChannel();
	channel->registerObject("jsCommWithQt", (QObject*)m_jsCommWithQt);
	QString exe_path = QDir::currentPath();
	QString bmap_path = exe_path.split("Harpia/")[0] + QString::fromLocal8Bit("src/bmap_offline/bmap.html");
	QUrl url = QUrl::fromLocalFile(bmap_path);
	//QUrl url = QUrl::fromLocalFile(BUILD_DIR "demo01.html");

	view->load(url);
	view->page()->setWebChannel(channel);

	//界面初始化
	QGridLayout* bmapLayout = new QGridLayout();

	if (bmapWidget == nullptr) {
		bmapWidget = new QWidget;
	}
	if (bmapOpWindow == nullptr) {
		bmapOpWindow = new QWidget;
	}


	QLabel* cityLabel = new QLabel(QString::fromLocal8Bit("城市搜索:"));
	cityLine = new QLineEdit;

	citySearchBtn = new QPushButton(QString::fromLocal8Bit("搜索"));
	markerBtn = new QPushButton(QString::fromLocal8Bit("标记"));
	drawLineBtn = new QPushButton(QString::fromLocal8Bit("路线绘制"));
	linePlayBtn = new QPushButton(QString::fromLocal8Bit("路线回放"));

	exitEditBtn = new QPushButton(QString::fromLocal8Bit("清除所有标记"));

	QGridLayout* searchLayout = new QGridLayout;
	QGridLayout* opLayout = new QGridLayout;
	searchLayout->addWidget(cityLabel, 0, 0);
	searchLayout->addWidget(cityLine, 0, 1);
	searchLayout->addWidget(citySearchBtn, 1, 1);

	opLayout->addWidget(markerBtn, 0, 0);
	opLayout->addWidget(drawLineBtn, 1, 0);
	opLayout->addWidget(linePlayBtn, 2, 0);
	opLayout->addWidget(exitEditBtn, 3, 0);


	QGridLayout* bmapOpLayout = new QGridLayout;
	bmapOpLayout->addLayout(searchLayout, 0, 0);
	bmapOpLayout->addLayout(opLayout, 1, 0);

	bmapOpWindow->setLayout(bmapOpLayout);

	bmapLayout->addWidget(view, 0, 0);
	bmapLayout->addWidget(bmapOpWindow, 0, 1);


	bmapOpWindow->setMaximumWidth(200);

	bmapLayout->setSpacing(0);
	bmapLayout->setContentsMargins(0, 0, 0, 0);

	this->setLayout(bmapLayout);

	//view->show();
	view->showMaximized();
}

void BMapEmbed::initConnect() {
	connect(citySearchBtn, &QPushButton::clicked, this, &BMapEmbed::sendCity);

	connect(markerBtn, &QPushButton::clicked, this, &BMapEmbed::sendData1);
	connect(drawLineBtn, &QPushButton::clicked, this, &BMapEmbed::sendData2);
	connect(linePlayBtn, &QPushButton::clicked, this, &BMapEmbed::sendData3);

	connect(exitEditBtn, &QPushButton::clicked, this, &BMapEmbed::exitMapEdit);
}

void BMapEmbed::sendCity() {
	m_jsCommWithQt->commMsgToJS(view, QString("citySearch('%1')").arg(cityLine->text()));
}

void BMapEmbed::sendData1() {
	m_jsCommWithQt->commMsgToJS(view, QString("execQtCommand(%1)").arg(COMM_CMD::ADD_MARKER));
}
void BMapEmbed::sendData2() {
	m_jsCommWithQt->commMsgToJS(view, QString("execQtCommand(%1)").arg(COMM_CMD::ADD_LINE));
}
void BMapEmbed::sendData3() {
	m_jsCommWithQt->commMsgToJS(view, QString("execQtCommand(%1)").arg(COMM_CMD::PLAY_LINE));
}

void BMapEmbed::exitMapEdit() {
	m_jsCommWithQt->commMsgToJS(view, QString("execQtCommand(%1)").arg(COMM_CMD::EXIT_OP));
}

void BMapEmbed::testComm(QString text) {
	QMessageBox::information(nullptr, "info", text);
}