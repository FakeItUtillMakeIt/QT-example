#include "controlmonitor.h" 
#include <qmath.h>
#include <QDateTime>
#include<QFontDatabase>
#include<Qtimer>
#include<QString>
#include"Light_info.h"


#define LIGHT(name) light##name
#define WORD(name) word##name

ControlMonitor::ControlMonitor(QWidget* parent)
	: QMainWindow(parent)
	, m_pCenterOperate(nullptr)
	, m_pUserDAO(nullptr)
	, m_pDeviceDAO(nullptr)
	, m_pCommandDAO(nullptr)
	, tb_show(nullptr)
	, m_myInfoTip(nullptr)
	, m_isMax(false)
{
	ui.setupUi(this);
	m_app = AppCache::instance();
	//ui.lb_title->setText(m_app->m_soft->GetName());
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);//去掉标题栏
	this->setWindowTitle(m_app->m_soft->GetName());
	this->setGeometry(0, 0, 1920, 1080);
	setAttribute(Qt::WA_TranslucentBackground, true);
	//加载字体包至成员变量font
	int id = QFontDatabase::addApplicationFont(":/ControlMonitor/word/word.ttf");
	QStringList s = QFontDatabase::applicationFontFamilies(id);
	font.setFamily(s[0]);

	testnum = 0;
	Init();

	//指示灯测试用代码 
	light_info.append(new Light_info(1, "控制箭上配电", 0));
	light_info.append(new Light_info(2, "测量箭上配电", 0));
	light_info.append(new Light_info(3, "惯组配电", 0));
	light_info.append(new Light_info(4, "瞄准配电", 0));
	light_info.append(new Light_info(5, "火箭起竖", 0));
	light_info.append(new Light_info(6, "舵机配电", 0));
	light_info.append(new Light_info(7, "瞄准", 0));
	light_info.append(new Light_info(8, "瞄准断电", 0));
	light_info.append(new Light_info(9, "上传诸元数据", 0));
	light_info.append(new Light_info(10, "上传瞄准角", 0));
	light_info.append(new Light_info(11, "收星检查", 0));
	light_info.append(new Light_info(12, "天基参数装订", 0));
	light_info.append(new Light_info(13, "电瓷瓶配电", 0));
	light_info.append(new Light_info(14, "人员撤离", 0));
	light_info.append(new Light_info(15, "测量开始记录", 0));
	light_info.append(new Light_info(16, "启动发射程序", 0));
	light_info.append(new Light_info(17, "安全转工作", 0));
	light_info.append(new Light_info(18, "气路阀起爆", 0));
	light_info.append(new Light_info(19, "液路阀起爆", 0));
	light_info.append(new Light_info(20, "电池激活", 0));
	light_info.append(new Light_info(21, "发射", 0));
	QTimer* test = new QTimer(this);
	connect(test, SIGNAL(timeout()), this, SLOT(test()));
	test->start(6000);










	light_load();
}
//测试函数
void ControlMonitor::test()
{
	if (testnum < light_info.size())
	{
		light_info[testnum]->status = true;
		testnum++;
	}

}

void ControlMonitor::Init()
{
	//请把主控发射流程页放在ui.page1 中
	m_pCenterOperate = new CenterOperate(ui.page2);
	//这里放入主控发射流程展示页，父窗口为ui.page1
	/************************************************************************/
	/* 这里不能删除2022/07/29  如需修改默认显示，将zhukongclick注释就行                                                                     */
	/************************************************************************/
	QString rocketTypeName = m_app->m_soft->GetType();
	QString qSqlString = "select rocket_info.`name`='%1',rocket_info.id,rocket_info.code,rocket_info.createTime,rocket_info.lastUpdateTime from simulatedtraining.rocket_info;";
	qSqlString = qSqlString.arg(rocketTypeName);
	int rocketId = -1;
	QString rocketCode = "";
	if (FlowInfoConfig2DB::getInstance()->customDBQuery(qSqlString)) {
		for (auto ele = FlowInfoConfig2DB::getInstance()->customSearchInfo.begin(); ele != FlowInfoConfig2DB::getInstance()->customSearchInfo.end(); ele++)
		{
			if (QString::fromStdString(ele->second[0]).toInt() == 1)
			{
				rocketId = QString::fromStdString(ele->second[1]).toInt();
				rocketCode = QString::fromStdString(ele->second[2]);
			}
		}
	}


	flowDisplayWidget = new FlowDisplayWidget(ui.page1, rocketCode, rocketId);

	flowDisplayWidget->setGeometry(0, 0, 1920, 768);

	ui.center_wgt->setObjectName("mainControlWidget");
	ui.center_wgt->setStyleSheet(QString("#mainControlWidget{border-image:url(%1);}").arg(":/ControlMonitor/images/Flow/bg.png"));
	ui.page1->setStyleSheet("background-image:url(:/flowload/images/Flow/白底20%透明@2x.png);");
	//默认显示流程
	zhukongclick();
	/************************************************************************/
	/* 2022/07/29修改结束                                                                     */
	/************************************************************************/

	connect(ui.pb_close, &QPushButton::clicked, this, &ControlMonitor::CloseWindow);
	connect(ui.pb_min, &QPushButton::clicked, this, &ControlMonitor::ShowMinimized);

	connect(m_pCenterOperate, &CenterOperate::curRunCmd, flowDisplayWidget, &FlowDisplayWidget::updateFlowStat);
	//connect(ui.pb_resize, &QPushButton::clicked, this, &ControlMonitor::changeResize);


	//指示灯检查
	lightnumber = 0;
	lightflag = true;
	inspect = new QTimer(this);
	connect(inspect, SIGNAL(timeout()), this, SLOT(light_inspect()));
	inspect->start(1000);
	flush = new QTimer(this);
	connect(flush, SIGNAL(timeout()), this, SLOT(light_flash()));


	//加载字体
	ui.softwarename->setFont(font);
	ui.rtlabel->setFont(font);
	ui.ctlabel->setFont(font);
	ui.ftlabel->setFont(font);
	ui.rokect_type->setFont(font);
	ui.curtime->setFont(font);
	ui.time->setFont(font);
	ui.rokect_type->setText(m_app->m_soft->GetType());

	//加载时间
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeupdate()));
	connect(timer, SIGNAL(timeout()), this, SLOT(timecount()));
	timer->start(1000);
	timer->setTimerType(Qt::PreciseTimer);
	baseTime = QTime::currentTime();


	//按钮界面切换

	connect(ui.zhukong, SIGNAL(clicked()), this, SLOT(zhukongclick()));
	connect(ui.erwei, SIGNAL(clicked()), this, SLOT(erwei_displayclick()));


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
	displayStatuInfo("加载用户数据完毕！");
	displayStatuInfo("系统启动完毕！");
}

//指示灯加载
void ControlMonitor::light_load()
{
	int width = (int)1840 / light_info.size();
	int margin = (int)((width - 36) / 2);
	int setvalue = 0;
	int word_setvalue = 0;
	font.setPixelSize(16);
	for (int i = 0; i < light_info.size(); i++)
	{
		setvalue = margin + i * width;
		QLabel* LIGHT(i) = new QLabel(ui.dengtiao);

		QString str = "light";
		QString count_str = QString::number(i);

		LIGHT(i)->setObjectName(str.append(count_str));
		LIGHT(i)->setGeometry(QRect(setvalue, 0, 36, 36));
		LIGHT(i)->setPixmap(QPixmap(QString::fromUtf8(":/ControlMonitor/lightbg")));
		LIGHT(i)->setScaledContents(true);
		LIGHT(i)->show();
		if (light_info[i]->status == 1)
		{
			LIGHT(i)->setPixmap(QPixmap(QString::fromUtf8(":/ControlMonitor/light")));
		}

		if (i % 2 == 0)
		{
			word_setvalue = setvalue + 18 - 55 + 40;
			QLabel* WORD(i) = new QLabel(ui.down_wordbar);
			QString str0 = "word";
			QString count_str0 = QString::number(i);
			WORD(i)->setObjectName(str0.append(count_str0));
			WORD(i)->setText(light_info[i]->name);
			WORD(i)->setGeometry(QRect(word_setvalue, 0, 110, 36));
			WORD(i)->setAlignment(Qt::AlignCenter);
			WORD(i)->setWordWrap(true);
			WORD(i)->setFont(font);
			WORD(i)->setStyleSheet("color:rgb(202,220,255)");
			WORD(i)->show();
		}
		else {
			word_setvalue = setvalue + 18 - 55 + 40;
			QLabel* WORD(i) = new QLabel(ui.up_wordbar);
			QString str1 = "word";;
			QString count_str1 = QString::number(i);
			WORD(i)->setObjectName(str1.append(count_str1));
			WORD(i)->setText(light_info[i]->name);
			WORD(i)->setGeometry(QRect(word_setvalue, 0, 110, 36));
			WORD(i)->setAlignment(Qt::AlignCenter);
			WORD(i)->setWordWrap(true);
			WORD(i)->setFont(font);
			WORD(i)->setStyleSheet("color:rgb(202,220,255)");
			WORD(i)->show();
		}


	}

}
//指示灯状态检查
void ControlMonitor::light_inspect()
{

	if (lightnumber < light_info.size())
	{
		if (light_info[lightnumber]->status == true)
		{
			flush->stop();
			QString s = "light";
			QString t = "word";
			QString wstr = t.append(QString::number(lightnumber));
			QString str = s.append(QString::number(lightnumber));
			QLabel* curlabel = ui.dengtiao->findChild<QLabel*>(str);
			QLabel* curword = ui.lightbar->findChild<QLabel*>(wstr);
			curlabel->setPixmap(QPixmap(QString::fromUtf8(":/ControlMonitor/light")));
			curword->setStyleSheet("color:white");
			if (lightnumber < light_info.size())
			{
				lightnumber++;
			}
			else {
				inspect->stop();
			}

		}
		else {
			flush->start(500);
		}

	}



}
//指示灯闪烁
void ControlMonitor::light_flash() {
	QString s = "light";
	QString str = s.append(QString::number(lightnumber));
	QLabel* curlabel = ui.dengtiao->findChild<QLabel*>(str);
	if (lightflag == true)
	{
		lightflag = false;
		curlabel->setPixmap(QPixmap(QString::fromUtf8(":/ControlMonitor/light")));
	}
	else {
		lightflag = true;
		curlabel->setPixmap(QPixmap(QString::fromUtf8(":/ControlMonitor/lightbg")));
	}





}
//底部tab页切换
void ControlMonitor::zhukongclick()
{
	ui.tabwgt->setCurrentIndex(0);//对象名为page1
	ui.zhukong->setStyleSheet("QPushButton{color:rgb(0,0,0);border:none;background-color:rgb(242,247,252);}");
	ui.erwei->setStyleSheet("QPushButton{color:rgb(180,180,180);border:none;}QPushButton:hover{color:black;border:none;}");

	//调用流程编排
	if (flowDisplayWidget == nullptr)
	{
		ui.page2->hide();
		ui.page1->setStyleSheet("background-color:rgb(245,245,245);");
		//ui.page1->setStyleSheet("background-image:url(:/flowload/images/Flow/白底20%透明@2x.png);");
		//进来就自动先加载流程
		flowDisplayWidget = new FlowDisplayWidget(ui.page1, "Ling", 1);
		flowDisplayWidget->setMinimumWidth(ui.page1->width());
		flowDisplayWidget->setMinimumHeight(ui.page1->height());

		//flowDisplayWidget->setRocketType("Ling", 1);
	}

	flowDisplayWidget->show();
	ui.page1->show();
}
void ControlMonitor::erwei_displayclick()
{
	ui.tabwgt->setCurrentIndex(1);//对象名为page2
	ui.erwei->setStyleSheet("QPushButton{color:rgb(0,0,0);border:none;background-color:rgb(242,247,252);}");
	ui.zhukong->setStyleSheet("QPushButton{color:rgb(180,180,180);border:none;}QPushButton:hover{color:black;border:none;}");
}
void ControlMonitor::timeupdate()
{

	QDateTime time = QDateTime::currentDateTime();
	ui.curtime->setText(time.toString("yyyy-MM-dd hh:mm:ss"));
}
void ControlMonitor::timecount()
{

	QTime current = QTime::currentTime();//获取系统当前时间
	int t = this->baseTime.msecsTo(current);  //两者相减的时间之差
	QTime showtime(0, 0, 0, 0);//初始时间
	showtime = showtime.addMSecs(t);//增加tms
	QString str = showtime.toString("hh:mm:ss"); //转换为string类型
	ui.time->setText(str);
}
void ControlMonitor::CloseWindow()
{
	this->close();
}
void ControlMonitor::ShowMinimized()
{
	this->showMinimized();
}


void ControlMonitor::displayStatuInfo(QString info, bool is_alarm)
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

void ControlMonitor::changeResize()
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
		//ui.vl_UI->setContentsMargins(0, 0, 0, 0); //去掉软件界面边界
		this->move(this->pos() + (QPoint(1, 1))); //窗口最大化需要 
		showMaximized();
		tb_show->setGeometry(QRect(6, ui.center_wgt->height() - 73, ui.center_wgt->width() - 12, 300));

	}
	else
	{
		mark = "放大-默认.png";
		pressMark = "放大-点击.png";
		hoverMark = "放大-悬浮.png";
		//ui.pb_resize->setToolTip("最大化");
		showNormal();
		//ui.vl_UI->setContentsMargins(5, 5, 5, 5);//设置软件界面边界
		tb_show->setGeometry(QRect(8, ui.center_wgt->height() - 127, ui.center_wgt->width() - 17, 200));

	}

	//ui.lb_title->setGeometry(QRect((ui.wgt_title_middle->width() - 250) / 2, 5, 250, 16));

	//ui.pb_resize->setStyleSheet(QString("QPushButton:hover{\n"
	//	"background-color:transparent;\n"
	//	"background-image:url(:/ControlMonitor/images/%1);\n"
	//	"border:0px;}\n"
	//	"QPushButton:pressed{\n"
	//	"background-color:transparent;\n"
	//	"background-image:url(:/ControlMonitor/images/%2);\n"
	//	"border:0px;}\n"
	//	"QPushButton {\n"
	//	"background-color:transparent;\n"
	//	"background-image: url(:/ControlMonitor/images/%3);\n"
	//	"border:0px;\n"
	//	"}").arg(hoverMark).arg(pressMark).arg(mark));
}

void ControlMonitor::paintEvent(QPaintEvent* event)
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

void ControlMonitor::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = true;
		mLastMousePosition = event->globalPos();
	}
}

void ControlMonitor::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons().testFlag(Qt::LeftButton) && mMoving)
	{
		setCursor(Qt::OpenHandCursor);
		this->move(this->pos() + (event->globalPos() - mLastMousePosition));
		mLastMousePosition = event->globalPos();
	}
}

void ControlMonitor::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = false;
		setCursor(Qt::ArrowCursor);
	}
}

void ControlMonitor::mouseDoubleClickEvent(QMouseEvent* event) {
	//if (Qt::LeftButton == event->button())
	//	changeResize();//此处调用最大化/还原按钮点击槽
	//event->ignore();
}
