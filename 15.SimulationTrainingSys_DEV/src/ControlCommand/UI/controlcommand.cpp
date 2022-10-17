#include "controlcommand.h" 
#include <qmath.h>
#include <QDateTime>
#include<QFontDatabase>
#include<Qtimer>
#include "../../ConfigInterface/configinterface.h"
#include "../../ConfigInterface/xmlstore.h"


static CenterOperate* gCenterOperate = nullptr;

void processCMD(int cmd, ConfigNameSpace::ConfigButton* btn)
{
	if (!gCenterOperate)
	{
		QMessageBox::warning(nullptr, ("错误"), ("指令处理接口未初始化"));
		return;
	}
	gCenterOperate->sendCMDFromInterface(cmd, btn);
}

ControlCommand::ControlCommand(QWidget* parent)
	: QMainWindow(parent)
	, m_pCenterOperate(nullptr)
	, m_pUserDAO(nullptr)
	, m_pDeviceDAO(nullptr)
	, m_pCommandDAO(nullptr)
	, m_pRocketDataDAO(nullptr)
	, tb_show(nullptr)
	, m_myInfoTip(nullptr)
	, m_isMax(false)
	, m_pReceiveRocketData(nullptr)
{
	ui.setupUi(this);
	m_app = AppCache::instance();
	//ui.lb_title->setText(m_app->m_soft->GetName());
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);//去掉标题栏
	this->setWindowTitle(m_app->m_soft->GetName());
	setAttribute(Qt::WA_TranslucentBackground, true);
	Init();
	//CreatConfigInterface();
}
void ControlCommand::CreatConfigInterface()
{
	gCenterOperate = m_pCenterOperate;
	ConfigNameSpace::ConfigGlobal::cmdhandler = processCMD;
	//m_allDeviceParam = m_app->m_allDeviceParam;
	ConfigNameSpace::ConfigGlobal::m_allDeviceParamPtr = &m_app->m_allDeviceParam;
	ConfigNameSpace::ConfigGlobal::m_allCommadPrt = &m_app->m_allCommad;
	ConfigNameSpace::ConfigGlobal::m_allFaultCommnd = &m_app->m_allFaultCommnd;
	ConfigNameSpace::XmlStore::InitRocketFile(true);
	ConfigNameSpace::XmlStore::InitStylePath();
	QString exepath = QApplication::applicationDirPath();
	QList<ConfigNameSpace::SceneInfo>  sceneinfolist;
	QString  filepath = exepath + "/rocket/" + QString::fromLocal8Bit(m_app->m_CurrentRocketType->m_name.c_str()) + "/scenes.xml";
	ConfigNameSpace::ConfigGlobal::currentRocket = QString::fromLocal8Bit(m_app->m_CurrentRocketType->m_name.c_str());
	ConfigNameSpace::ConfigGlobal::currentRocketID = m_app->m_CurrentRocketType->m_id;
	int result = ConfigNameSpace::XmlStore::ReadSceneFile(filepath.toLocal8Bit().data(), sceneinfolist);
	//样式管理器
	ConfigNameSpace::StyleManager* stylemanager = new   ConfigNameSpace::StyleManager;
	ConfigNameSpace::ConfigGlobal::gstylemanager = stylemanager;
	//   //布局管理器
	ConfigNameSpace::MoveAbleFrame::CreateControl();
	ConfigNameSpace::ConfigInterface* configinterface = new  ConfigNameSpace::ConfigInterface;
	ConfigNameSpace::ConfigGlobal::gconfiginterface = configinterface;
	configinterface->firstinit();
	configinterface->RestoreSceneFromXml(sceneinfolist);
	configinterface->AddSceneEntry();
	QHBoxLayout* hlayout = new QHBoxLayout;
	hlayout->addWidget(configinterface);
	ui.center_wgt->setLayout(hlayout);
	hlayout->setMargin(0);
}


bool ControlCommand::InitFrame()
{
	//设置协议帧
	m_app->m_CurrentRocketDataFrame = nullptr;
	for (auto pair : m_app->m_RocketDataFrame)
	{
		if (pair.second->m_rocketId == m_app->m_CurrentRocketType->m_id)
		{
			m_app->m_CurrentRocketDataFrame = pair.second;
		}
	}
	if (m_app->m_CurrentRocketDataFrame == nullptr) return false;
	m_pReceiveRocketData = new ReceiveRocketData();
	return true;
}

void ControlCommand::Init()
{
	QWidget* tmpwidget = new QWidget();
	m_pCenterOperate = new CenterOperate(tmpwidget);
	tmpwidget->hide();
	connect(ui.pb_close, &QPushButton::clicked, this, &ControlCommand::CloseWindow);
	connect(ui.pb_min, &QPushButton::clicked, this, &ControlCommand::ShowMinimized);
	//connect(ui.pb_resize, &QPushButton::clicked, this, &ControlCommand::changeResize);

	//加载字体
	int id = QFontDatabase::addApplicationFont(":/ControlCommand/word/word.ttf");
	QStringList s = QFontDatabase::applicationFontFamilies(id);
	QFont f;
	f.setFamily(s[0]);
	ui.softwarename->setFont(f);
	ui.rtlabel->setFont(f);
	ui.ctlabel->setFont(f);
	ui.ftlabel->setFont(f);
	ui.rokect_type->setFont(f);
	ui.curtime->setFont(f);
	ui.time->setFont(f);
	m_app->rokecttype = ui.rokect_type;

	//ui.rokect_type->setText(m_app->m_soft->GetType());

	//加载时间
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeupdate()));
	connect(timer, SIGNAL(timeout()), this, SLOT(timecount()));
	timer->start(1000);
	timer->setTimerType(Qt::PreciseTimer);
	baseTime = QTime::currentTime();


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

	//加载基础数据
	m_pUserDAO = new DataBase::UserDAO(m_app->m_outputPath);
	if (!m_pUserDAO->getUser())
	{
		QString info = "建立数据库连接失败，请检查数据库配置文件";
		displayStatuInfo(info, true);
		return;
	}
	//设备数据
	m_pDeviceDAO = new DataBase::DeviceDAO(m_app->m_outputPath);
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
	if (!m_pCommandDAO->getFaultCommand())
	{
		QString info = "建立数据库连接失败，请检查数据库配置文件";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("加载故障信息数据完毕！");

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

	QString msg, curtime;
	if (!m_pRocketDataDAO->initConfig(msg))//初始化数据库保存配置文件的表，如果不存在就创建
	{
		QString info = "创建配置文件数据表失败";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("配置文件数据表初始化完成！");
	if (m_pRocketDataDAO->ReadConfigTime(msg, curtime))//读取配置表更新时间，如果配置已有，则返回其时间，否则返回失败
	{
		if (m_pRocketDataDAO->ReadConfigFromDb(msg, curtime))
			displayStatuInfo("读取组态配置文件成功！");
	}

	//协议帧排序
	for (auto item : m_app->m_RocketDataFrame)
	{
		item.second->sortParams();
	}
	//InitFrame();
	displayStatuInfo("加载用户数据完毕！");
	displayStatuInfo("系统启动完毕！");
}
void ControlCommand::timeupdate()
{
	QDateTime time = QDateTime::currentDateTime();
	ui.curtime->setText(time.toString("yyyy-MM-dd hh:mm:ss"));
}
void ControlCommand::timecount()
{
	QTime current = QTime::currentTime();//获取系统当前时间
	int t = this->baseTime.msecsTo(current);  //两者相减的时间之差
	QTime showtime(0, 0, 0, 0);//初始时间
	showtime = showtime.addMSecs(t);//增加tms
	QString str = showtime.toString("hh:mm:ss"); //转换为string类型
	ui.time->setText(str);
}
void ControlCommand::CloseWindow()
{
	this->close();
}
void ControlCommand::ShowMinimized()
{
	this->showMinimized();
}


void ControlCommand::displayStatuInfo(QString info, bool is_alarm)
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

void ControlCommand::changeResize()
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
		"background-image:url(:/ControlCommand/images/%1);\n"
		"border:0px;}\n"
		"QPushButton:pressed{\n"
		"background-color:transparent;\n"
		"background-image:url(:/ControlCommand/images/%2);\n"
		"border:0px;}\n"
		"QPushButton {\n"
		"background-color:transparent;\n"
		"background-image: url(:/ControlCommand/images/%3);\n"
		"border:0px;\n"
		"}").arg(hoverMark).arg(pressMark).arg(mark));*/
}

void ControlCommand::paintEvent(QPaintEvent* event)
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

void ControlCommand::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && event->modifiers() == Qt::NoModifier)
	{
		mMoving = true;
		mLastMousePosition = event->globalPos();
	}
}

void ControlCommand::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons().testFlag(Qt::LeftButton) && mMoving)
	{
		setCursor(Qt::OpenHandCursor);
		QPoint offset = event->globalPos() - mLastMousePosition;

		if (abs(offset.x()) > 50 || abs(offset.y()) > 50)
		{

			return;
		}

		this->move(this->pos() + (offset));
		mLastMousePosition = event->globalPos();
	}
}

void ControlCommand::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = false;
		setCursor(Qt::ArrowCursor);
	}
}

void ControlCommand::mouseDoubleClickEvent(QMouseEvent* event) {
	//if (Qt::LeftButton == event->button())
	//	changeResize();//此处调用最大化/还原按钮点击槽
	//event->ignore();
}
