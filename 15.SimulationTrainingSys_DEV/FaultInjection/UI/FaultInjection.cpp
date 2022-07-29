 #include "FaultInjection.h" 
#include <qmath.h>
#include <QDateTime>
#include<qtimer.h>
#include<QFont>
#include<QFontDatabase>
FaultInjection::FaultInjection(QWidget* parent)
	: QMainWindow(parent)
	, m_pFaultDAO(nullptr)
	, m_pFaultParamDAO(nullptr)
	, m_pDeviceDAO(nullptr)
	, tb_show(nullptr)
	, m_myInfoTip(nullptr)
	, m_isMax(false)
{
	ui.setupUi(this);
	m_app = AppCache::instance();
	m_app->m_rockedType = 2;//选择火箭类型,TODO需要修改，后面可以在界面上进行选择。

	ui.rocket_type->setText(m_app->m_soft->GetType());
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);//去掉标题栏
	this->setWindowTitle(m_app->m_soft->GetName());
	setAttribute(Qt::WA_TranslucentBackground, true);

	Init();
}

void FaultInjection::Init()
{
	connect(ui.pb_close, &QPushButton::clicked, this, &FaultInjection::CloseWindow);
	connect(ui.pb_min, &QPushButton::clicked, this, &FaultInjection::ShowMinimized);
	//connect(ui.pb_resize, &QPushButton::clicked, this, &FaultInjection::changeResize);
	//时间相关槽函数绑定
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeupdate()));
	connect(timer, SIGNAL(timeout()), this, SLOT(timecount()));
	timer->start(1000);
	timer->setTimerType(Qt::PreciseTimer);
	baseTime = QTime::currentTime();
	
	//加载字体
	int id = QFontDatabase::addApplicationFont(":/FaultInjection/word/word.ttf");
	QStringList s = QFontDatabase::applicationFontFamilies(id);
	QFont f;
	f.setFamily(s[0]);
	ui.softwarename->setFont(f);
	ui.rtlabel->setFont(f);
	ui.ctlabel->setFont(f);
	ui.ftlabel->setFont(f);
	ui.rocket_type->setFont(f);
	ui.curtime->setFont(f);
	ui.test_time->setFont(f);
	ui.rocket_type->setText(m_app->m_soft->GetType());
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
	m_pFaultDAO = new DataBase::FaultDAO(m_app->m_outputPath);
	if (!m_pFaultDAO->getFault())
	{
		QString info = "建立数据库连接失败，请检查数据库配置文件";
		displayStatuInfo(info, true);
		return;
	}

	//获取rocket_info表信息
	if (!m_pFaultDAO->GetAllRocketInfoFrame())
	{
		QString info = "获取rocket_info数据信息失败！";
		displayStatuInfo(info, true);
		return;
	}

	//获取command_info表信息
	if (!m_pFaultDAO->GetAllCommandInfoFrame())
	{
		QString info = "获取command_info数据信息失败！";
		displayStatuInfo(info, true);
		return;
	}

	//获取command_param_info表信息
	if (!m_pFaultDAO->GetAllCommandParamInfoFrame())
	{
		QString info = "获取command_param_info数据信息失败！";
		displayStatuInfo(info, true);
		return;
	}

	m_pFaultParamDAO = new DataBase::FaultParamDAO(m_app->m_outputPath);
	//获取fault_param_info表信息
	if (!m_pFaultParamDAO->GetAllFaultParamInfoFrames())
	{
		QString info = "获取fault_param_info数据信息失败！";
		displayStatuInfo(info, true);
		return;
	}

	//获取故障参数fault_command_info表信息
	if (!m_pFaultParamDAO->GetAllFaultCommandInfoFrames())
	{
		QString info = "获取fault_command_info数据信息失败！";
		displayStatuInfo(info, true);
		return;
	}


	m_pDeviceDAO = new DataBase::DeviceDAO(m_app->m_outputPath);
	//获取故障参数parameter_info表信息
	if (!m_pDeviceDAO->GetAllParameterInfoFrames())
	{
		QString info = "获取parameter_info数据信息失败！";
		displayStatuInfo(info, true);
		return;
	}

	//获取故障参数device_info表信息
	if (!m_pDeviceDAO->GetAllDeviceInfoFrames())
	{
		QString info = "获取rocket_info数据信息失败！";
		displayStatuInfo(info, true);
		return;
	}


	//获取故障参数device_param_info表信息
	if (!m_pDeviceDAO->GetAllDeviceParamInfoFrames())
	{
		QString info = "获取device_param_info数据信息失败！";
		displayStatuInfo(info, true);
		return;
	}


	QHBoxLayout* hlayout = new QHBoxLayout;
	m_centerOperate = new CenterOperate();
	hlayout->addWidget(m_centerOperate);
	ui.center_wgt->setLayout(hlayout);

	displayStatuInfo("加载故障数据完毕！");
	displayStatuInfo("系统启动完毕！");
}
void FaultInjection::timeupdate()
{

	QDateTime time = QDateTime::currentDateTime();
	ui.curtime->setText(time.toString("yyyy-MM-dd hh:mm:ss"));
}
void FaultInjection::timecount()
{
	
	QTime current = QTime::currentTime();//获取系统当前时间
	int t = this->baseTime.msecsTo(current);  //两者相减的时间之差
	QTime showtime(0, 0, 0, 0);//初始时间
	showtime = showtime.addMSecs(t);//增加tms
	QString str = showtime.toString("hh:mm:ss"); //转换为string类型
	ui.test_time->setText(str);
}
void FaultInjection::CloseWindow()
{
	this->close();
}
void FaultInjection::ShowMinimized()
{
	this->showMinimized();
}


void FaultInjection::displayStatuInfo(QString info, bool is_alarm)
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

void FaultInjection::changeResize()
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
		tb_show->setGeometry(QRect(6, ui.center_wgt->height() - 223, ui.center_wgt->width() - 12, 300));

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
		"background-image:url(:/FaultInjection/images/%1);\n"
		"border:0px;}\n"
		"QPushButton:pressed{\n"
		"background-color:transparent;\n"
		"background-image:url(:/FaultInjection/images/%2);\n"
		"border:0px;}\n"
		"QPushButton {\n"
		"background-color:transparent;\n"
		"background-image: url(:/FaultInjection/images/%3);\n"
		"border:0px;\n"
		"}").arg(hoverMark).arg(pressMark).arg(mark));*/
}
	

void FaultInjection::paintEvent(QPaintEvent* event)
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

void FaultInjection::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = true;
		mLastMousePosition = event->globalPos();
	}
}

void FaultInjection::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons().testFlag(Qt::LeftButton) && mMoving)
	{
		setCursor(Qt::OpenHandCursor);
		this->move(this->pos() + (event->globalPos() - mLastMousePosition));
		mLastMousePosition = event->globalPos();
	}
}

void FaultInjection::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = false;
		setCursor(Qt::ArrowCursor);
	}
}

void FaultInjection::mouseDoubleClickEvent(QMouseEvent* event) {
	//if (Qt::LeftButton == event->button())
	//	changeResize();//此处调用最大化/还原按钮点击槽
	//event->ignore();
}
