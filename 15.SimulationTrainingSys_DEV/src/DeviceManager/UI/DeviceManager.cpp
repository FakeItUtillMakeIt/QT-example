#include "DeviceManager.h" 
#include <qmath.h>
#include <QDateTime>
#include <QFont>
DeviceManager::DeviceManager(QWidget* parent)
	: QMainWindow(parent)
	, m_pCenterOperate(nullptr)
	, m_pDeviceDAO(nullptr)
	, m_pCommandDAO(nullptr)
	, m_pRocketDataDAO(nullptr)
	, tb_show(nullptr)
	, m_myInfoTip(nullptr)
	, m_isMax(false)
{
	ui.setupUi(this);
	m_app = AppCache::instance();
	//ui.lb_title->setText(m_app->m_soft->GetName());//ui 进行了修改，标题不需要再读入（杨）
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);//去掉标题栏
	this->setWindowTitle(m_app->m_soft->GetName());
	setAttribute(Qt::WA_TranslucentBackground, true);
	//Init();


	//加载时间
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeUpdate()));
	timer->start(1000);
}

void DeviceManager::Init()
{


	//将CenterOperate设置为DeviceManager的主界面
	//m_centeralWidget = new CenterOperate(ui.center_wgt);
	connect(ui.pb_close, &QPushButton::clicked, this, &DeviceManager::CloseWindow);
	connect(ui.pb_min, &QPushButton::clicked, this, &DeviceManager::ShowMinimized);

	int id = QFontDatabase::addApplicationFont(":/DeviceManager/word/word.ttf");
	QStringList s = QFontDatabase::applicationFontFamilies(id);
	QFont f;
	f.setFamily(s[0]);
	f.setPixelSize(20);
	ui.title->setFont(f);

	for each (auto var in m_app->m_allRocketType)
	{
		ui.comboBox->addItem(QString::fromLocal8Bit(var.second->m_name.c_str()), var.second->m_id);
	}
	ui.comboBox->setCurrentText(QString::fromLocal8Bit(m_app->m_CurrentRocketType->m_name.c_str()));
	ui.comboBox->setFont(f);
	ui.curtime->setFont(f);

	connect(ui.comboBox, &QComboBox::currentTextChanged, this, [=]() {

		//m_app->m_CurrentRocketType->m_id = ui.comboBox->currentData().toUInt();
		//m_app->m_CurrentRocketType->m_name = ui.comboBox->currentText().toLocal8Bit();

		m_app->m_CurrentRocketType = m_app->m_allRocketType[ui.comboBox->currentData().toUInt()];
		emit rocketTypeChanged();
		});

	//初始化调试信息显示区
	m_myInfoTip = new MyInfoTip(ui.wgt_status_left);
	//ui.wgt_status_left->setStyleSheet("background-color:red");
	connect(m_myInfoTip, &MyInfoTip::hoverInfo, this, [this]() {
		tb_show->raise();
		tb_show->show();

		});
	if (tb_show != nullptr)
	{
		delete tb_show;
	}
	tb_show = new MyLogBox(ui.centralWidget);
	connect(tb_show, &MyLogBox::leaveInfo, this, [this]() {
		tb_show->hide();
		});
	tb_show->hide();
	changeResize();

#pragma region 加载基础数据

	QElapsedTimer timer1;

	m_pDeviceDAO = new DataBase::DeviceDAO(m_app->m_outputPath);
	if (!m_pDeviceDAO->getStatus())
	{
		QString info = "建立数据库连接失败，请检查数据库配置文件";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("加载状态数据完毕！");

	if (!m_pDeviceDAO->getDevice())
	{
		QString info = "建立数据库连接失败，请检查数据库配置文件";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("加载设备基础数据完毕！");

	if (!m_pDeviceDAO->getDeviceParam())
	{
		QString info = "建立数据库连接失败，请检查数据库配置文件";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("加载设备参数数据完毕！");


	if (!m_pDeviceDAO->getDeviceStatus())
	{
		QString info = "建立数据库连接失败，请检查数据库配置文件";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("加载设备状态数据完毕！");

	m_pCommandDAO = new DataBase::CommandDAO(m_app->m_outputPath);
	if (!m_pCommandDAO->getCommand())
	{
		QString info = "建立数据库连接失败，请检查数据库配置文件";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("加载指令数据完毕！");
	if (!m_pCommandDAO->getCommandParam())
	{
		QString info = "建立数据库连接失败，请检查数据库配置文件";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("加载指令参数数据完毕！");

	m_pRocketDataDAO = new DataBase::RocketDataDAO(m_app->m_outputPath);
	if (!m_pRocketDataDAO->getRocketData())
	{
		QString info = "建立数据库连接失败，请检查数据库配置文件";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("加载箭上遥测数据帧协议数据完毕！");
	if (!m_pRocketDataDAO->getRocketParam())
	{
		QString info = "建立数据库连接失败，请检查数据库配置文件";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("加载帧协议参数数据完毕！");
	//协议帧排序
	for (auto item : m_app->m_RocketDataFrame)
	{
		item.second->sortParams();
	}

	qDebug() << "加载基础数据用时:" << timer1.elapsed();

	m_centeralWidget = new CenterOperate(ui.center_wgt);

	QElapsedTimer timer2;
	displayStatuInfo("加载基础数据完毕！");
	displayStatuInfo("系统启动完毕！");
	InitDevice();
	qDebug() << "初始化设备用时:" << timer2.elapsed();

	emit deviceLoadOver();
}


void DeviceManager::timeUpdate() {

	QDateTime time = QDateTime::currentDateTime();
	ui.curtime->setText(time.toString("yyyy-MM-dd hh:mm:ss"));

}

void DeviceManager::CloseWindow()
{
	this->close();
	ParamInfoConfig::InfoConfigWidget::closeInstance();
	DeviceInfoConfig::InfoConfigWidget::closeInstance();
	CommandInfoConfig::InfoConfigWidget::closeInstance();
	RocketInfoConfig::InfoConfigWidget::closeInstance();
	WidgetStyleSheet::closeInstance();

	if (m_centeralWidget != nullptr)
	{
		m_centeralWidget->deleteLater();
	}
}
void DeviceManager::ShowMinimized()
{
	this->showMinimized();
}

/// <summary>
/// 初始化设备  状态和参数配置信息 初始化设备应该仅初始化当前选择的火箭的设备
/// </summary>
void DeviceManager::InitDevice()
{

	QThread* initThread = new QThread;
	WorkThread* initWorker = new WorkThread;
	initWorker->moveToThread(initThread);
	connect(initThread, &QThread::started, initWorker, &WorkThread::doWork);
	initThread->start();
	connect(initWorker, &WorkThread::workFinished, this, [=]() {
		QMessageBox::information(nullptr, "信息", "数据加载完毕！");
		});
	////把设备的状态及相应参数的配置进行初始化
	//QString initStat = QString("断电");
	//for (Device* eachDev : m_app->m_allDevice)
	//{
	//	eachDev->m_sCurStatus = initStat.toStdString();
	//}



	//DeviceDBConfigInfo::getInstance()->readStatusInfoDB2UI();
	//QString statusFilePath;
	//auto curPath = QCoreApplication::applicationDirPath();
	//statusFilePath = curPath + "/device/";
	////设备参数初始化  四个需要初始化的值
	for (auto ele = m_app->m_allDeviceParam.begin(); ele != m_app->m_allDeviceParam.end(); ele++)
	{
		ele->second->timer->start(50);

	}

}

void DeviceManager::displayStatuInfo(QString info, bool is_alarm)
{
	m_myInfoTip->setLabelText(is_alarm, info);
	LOG(INFO) << info.toStdString();
	QString temp_str = QString("%1:%2\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(info);

	tb_show->insertPlainText(temp_str);
	tb_show->moveCursor(QTextCursor::Start);//始终定为在第一行
	QTextCursor cursor = tb_show->textCursor();
	cursor.select(QTextCursor::LineUnderCursor);   //选中要着色的内容
	QTextCharFormat fmt;
	fmt.setForeground(is_alarm ? QColor(Qt::red) : QColor(Qt::black));
	cursor.mergeCharFormat(fmt);    //设置文本格式
	cursor.clearSelection(); //撤销选中
	//cursor.movePosition(QTextCursor::EndOfLine);  //cursor和anchor均移至末尾  
}

void DeviceManager::changeResize()
{
	m_isMax = !m_isMax;
	QString hoverMark;
	QString pressMark;
	QString mark;

	if (m_isMax)
	{
		mark = "缩小-默认.png";
		pressMark = "缩小-点击.png";
		hoverMark = "缩小-悬浮.png";
		//ui.pb_resize->setToolTip("向下还原");
		ui.vl_UI->setContentsMargins(0, 0, 0, 0); //去掉软件界面边界
		this->move(this->pos() + (QPoint(1, 1))); //窗口最大化需要 
		showMaximized();
		tb_show->setGeometry(QRect(6, ui.center_wgt->height() - 183, ui.center_wgt->width() - 12, 300));

	}
	else
	{
		mark = "放大-默认.png";
		pressMark = "放大-点击.png";
		hoverMark = "放大-悬浮.png";
		//ui.pb_resize->setToolTip("最大化");
		showNormal();
		ui.vl_UI->setContentsMargins(5, 5, 5, 5);//设置软件界面边界
		tb_show->setGeometry(QRect(8, ui.center_wgt->height() - 127, ui.center_wgt->width() - 17, 200));

	}

	//ui.lb_title->setGeometry(QRect((ui.wgt_title_middle->width() - 250) / 2, 5, 250, 16));

	/*ui.pb_resize->setStyleSheet(QString("QPushButton:hover{\n"
		"background-color:transparent;\n"
		"background-image:url(:/DeviceManager/images/%1);\n"
		"border:0px;}\n"
		"QPushButton:pressed{\n"
		"background-color:transparent;\n"
		"background-image:url(:/DeviceManager/images/%2);\n"
		"border:0px;}\n"
		"QPushButton {\n"
		"background-color:transparent;\n"
		"background-image: url(:/DeviceManager/images/%3);\n"
		"border:0px;\n"
		"}").arg(hoverMark).arg(pressMark).arg(mark));*/
}

void DeviceManager::paintEvent(QPaintEvent* event)
{
	int border_width = 5;
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(border_width, border_width, this->width() - border_width * 2, this->height() - border_width * 2);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillPath(path, QBrush(Qt::white));
	QColor color(0, 0, 0, 250);
	for (int i = 0; i < border_width; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);
		path.addRect(border_width - i, border_width - i, this->width() - (border_width - i) * 2, this->height() - (border_width - i) * 2);
		color.setAlpha(125 - qSqrt(i) * 50);
		painter.setPen(color);
		painter.drawPath(path);
	}
}

void DeviceManager::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = true;
		mLastMousePosition = event->globalPos();
	}
}

void DeviceManager::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons().testFlag(Qt::LeftButton) && mMoving)
	{
		setCursor(Qt::OpenHandCursor);
		this->move(this->pos() + (event->globalPos() - mLastMousePosition));
		mLastMousePosition = event->globalPos();
	}
}

void DeviceManager::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = false;
		setCursor(Qt::ArrowCursor);
	}
}

void DeviceManager::mouseDoubleClickEvent(QMouseEvent* event) {
	if (Qt::LeftButton == event->button())
		changeResize();//此处调用最大化/还原按钮点击槽
	event->ignore();
}