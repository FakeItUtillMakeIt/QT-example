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
	, mainflow_finish(false)
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



	//指示灯测试用代码 
	/*light_info.append(new Light_info(1, "控制箭上配电", 0));
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
	test->start(6000);*/











}


void ControlMonitor::Init()
{
	//请把主控发射流程页放在ui.page1 中
	//m_pCenterOperate = new CenterOperate(ui.page1);
	QWidget* one = new QWidget();
	m_pCenterOperate = new CenterOperate(one);
	ui.rokect_type->setText(QString::fromLocal8Bit(m_app->m_CurrentRocketType->m_name.c_str()));

	flowDisplayWidget = new FlowDisplayWidget(ui.page1, QString::fromLocal8Bit(m_app->m_CurrentRocketType->m_name.c_str()), m_app->m_CurrentRocketType->m_id);
	flowDisplayWidget->setRocketType(QString::fromLocal8Bit(m_app->m_CurrentRocketType->m_name.c_str()), m_app->m_CurrentRocketType->m_id);
	flowDisplayWidget->setGeometry(0, 0, 1920, 824);

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

	connect(flowDisplayWidget, &FlowDisplayWidget::sendMainFlowInfo, this, &ControlMonitor::recvMainFlow);
	connect(flowDisplayWidget, &FlowDisplayWidget::sendMainflowchange, this, &ControlMonitor::acceptchage);
	//connect(ui.pb_resize, &QPushButton::clicked, this, &ControlMonitor::changeResize);

	//指示灯初始化
	lightnumber = 0;
	lightflag = true;
	//inspect = new QTimer(this);
//	connect(inspect, SIGNAL(timeout()), this, SLOT(light_inspect()));
	//inspect->start(1000);
	flush = new QTimer(this);
	connect(flush, SIGNAL(timeout()), this, SLOT(light_flash()));


	//lighttest();

	//加载字体
	ui.softwarename->setFont(font);
	ui.rtlabel->setFont(font);
	ui.ctlabel->setFont(font);
	ui.ftlabel->setFont(font);
	ui.rokect_type->setFont(font);
	ui.curtime->setFont(font);
	ui.time->setFont(font);
	m_app->rokecttype = ui.rokect_type;
	//ui.rokect_type->setText(m_app->m_soft->GetType());

	//加载时间
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeupdate()));
	connect(timer, SIGNAL(timeout()), this, SLOT(timecount()));
	timer->start(1000);
	timer->setTimerType(Qt::PreciseTimer);
	baseTime = QTime::currentTime();


	//按钮界面切换

	//connect(ui.zhukong, SIGNAL(clicked()), this, SLOT(zhukongclick()));
	//connect(ui.erwei, SIGNAL(clicked()), this, SLOT(erwei_displayclick()));


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
	QString msg;
	if (!m_pCommandDAO->StartNotify(msg))
	{
		displayStatuInfo(msg, true);
		return;
	}
	displayStatuInfo("加载指令参数数据完毕！");
	displayStatuInfo("加载用户数据完毕！");
	displayStatuInfo("系统启动完毕！");
	displayStatuInfo("更新启动状态完毕！");

	
}

//指示灯加载
void ControlMonitor::light_load()
{
	if (m_app->mainflowlist.size() == 0)
	{
		qDebug() << "没有主流程！！！";
		return;
	}
	else
	{
		int width = (int)1840 / m_app->mainflowlist.size();
		int margin = (int)((width - 36) / 2);
		int setvalue = 0;
		int word_setvalue = 0;
		font.setPixelSize(16);
		for (int i = 0; i < m_app->mainflowlist.size(); i++)
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


			if (i % 2 == 0)
			{
				word_setvalue = setvalue + 18 - 100 + 40;
				QLabel* WORD(i) = new QLabel(ui.down_wordbar);
				QString str0 = "word";
				QString count_str0 = QString::number(i);
				WORD(i)->setObjectName(str0.append(count_str0));
				WORD(i)->setText(m_app->mainflowlist[i]);
				WORD(i)->setGeometry(QRect(word_setvalue, 0, 200, 36));
				WORD(i)->setAlignment(Qt::AlignCenter);
				WORD(i)->setWordWrap(true);
				WORD(i)->setFont(font);
				WORD(i)->setStyleSheet("color:rgb(202,220,255);font-size:20px;");
				WORD(i)->show();
			}
			else {
				word_setvalue = setvalue + 18 - 100 + 40;
				QLabel* WORD(i) = new QLabel(ui.up_wordbar);
				QString str1 = "word";;
				QString count_str1 = QString::number(i);
				WORD(i)->setObjectName(str1.append(count_str1));
				WORD(i)->setText(m_app->mainflowlist[i]);
				WORD(i)->setGeometry(QRect(word_setvalue, 0, 200, 36));
				WORD(i)->setAlignment(Qt::AlignCenter);
				WORD(i)->setWordWrap(true);
				WORD(i)->setFont(font);
				WORD(i)->setStyleSheet("color:rgb(202,220,255);font-size:20px;");
				WORD(i)->show();
			}


		}
	}


}
//指示灯状态检查
//void ControlMonitor::light_inspect()
//{
//
//	if (lightnumber < light_info.size())
//	{
//		if (light_info[lightnumber]->status == true)
//		{
//			flush->stop();
//			QString s = "light";
//			QString t = "word";
//			QString wstr = t.append(QString::number(lightnumber));
//			QString str = s.append(QString::number(lightnumber));
//			QLabel* curlabel = ui.dengtiao->findChild<QLabel*>(str);
//			QLabel* curword = ui.lightbar->findChild<QLabel*>(wstr);
//			curlabel->setPixmap(QPixmap(QString::fromUtf8(":/ControlMonitor/light")));
//			curword->setStyleSheet("color:white");
//			if (lightnumber < light_info.size())
//			{
//				lightnumber++;
//			}
//			else {
//				inspect->stop();
//			}
//
//		}
//		else {
//			flush->start(500);
//		}
//
//	}
//
//
//
//}
// 指示灯收到指令装载
void ControlMonitor::flash_load()
{
	if (m_app->mainflowlist.size() == 0)
	{
		qDebug() << "没有主流程！！！";
		return;
	}
	else {
		QLabel* label;
		QLabel* wordlabel;


		for (int i = 0; i < lightnumber; i++)
		{
			QString s = "light";
			QString w = "word";
			QString word = w.append(QString::number(i));
			QString str = s.append(QString::number(i));
			label = ui.dengtiao->findChild<QLabel*>(str);
			wordlabel = ui.lightbar->findChild<QLabel*>(word);
			label->setPixmap(QPixmap(QString::fromUtf8(":/ControlMonitor/light")));
			wordlabel->setStyleSheet("color:white;font-size:20px;");
		}

		if (mainflow_finish)
		{

			QString s = "light";
			QString w = "word";
			QString word = w.append(QString::number(lightnumber));
			QString str = s.append(QString::number(lightnumber));
			label = ui.dengtiao->findChild<QLabel*>(str);
			wordlabel = ui.lightbar->findChild<QLabel*>(word);
			label->setPixmap(QPixmap(QString::fromUtf8(":/ControlMonitor/light")));
			wordlabel->setStyleSheet("color:white;font-size:20px;");
		}
		else
		{

			flush->start(700);
		}
	}



}

//指示灯清空
void ControlMonitor::lightreset()
{
	QList<QLabel*> lblist = ui.down_wordbar->findChildren<QLabel*>();
	QList<QLabel*> lblist1 = ui.up_wordbar->findChildren<QLabel*>();
	QList<QLabel*> lblist2 = ui.dengtiao->findChildren<QLabel*>();

	qDeleteAll(lblist);
	qDeleteAll(lblist1);

	for (int i = 0; i < lblist2.size(); i++)
	{
		if (QString("dtbg") == lblist2[i]->objectName())
		{
			continue;
		}
		delete lblist2[i];
	}
	flush->stop();

}
//指示灯闪烁
void ControlMonitor::light_flash() {
	QString s = "light";
	QString str = s.append(QString::number(lightnumber));
	curlabel = ui.dengtiao->findChild<QLabel*>(str);

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
//测试函数
void ControlMonitor::lighttest()
{
	QTimer* test = new QTimer(this);


	connect(test, &QTimer::timeout, this, [this]() {
		if (lightnumber < 4)
		{
			lightnumber++;
			if (mainflow_finish)
			{
				mainflow_finish = false;
			}
			else {
				mainflow_finish = true;
			}
		}

		flash_load();
		});

	test->start(3000);


}
//底部tab页切换
void ControlMonitor::zhukongclick()
{
	ui.tabwgt->setCurrentIndex(0);//对象名为page1
	//ui.zhukong->setStyleSheet("QPushButton{color:rgb(0,0,0);border:none;background-color:rgb(242,247,252);}");
	//ui.erwei->setStyleSheet("QPushButton{color:rgb(180,180,180);border:none;}QPushButton:hover{color:black;border:none;}");

	//调用流程编排
	if (flowDisplayWidget == nullptr)
	{

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
//twoDimensionClick
void ControlMonitor::erwei_displayclick()
{
	ui.tabwgt->setCurrentIndex(1);//对象名为page2
	//ui.erwei->setStyleSheet("QPushButton{color:rgb(0,0,0);border:none;background-color:rgb(242,247,252);}");
	//ui.zhukong->setStyleSheet("QPushButton{color:rgb(180,180,180);border:none;}QPushButton:hover{color:black;border:none;}");
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
	QString msg;
	if (!m_pCommandDAO->StopNotify(msg))
	{
		displayStatuInfo(msg, true);
		return;
	}
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
		this->move(this->pos() + (QPoint(1, 1))); //窗口最大化需要 
		showMaximized();
		tb_show->setGeometry(QRect(6, ui.center_wgt->height() - 73, ui.center_wgt->width() - 12, 300));
	}
	else
	{
		mark = "放大-默认.png";
		pressMark = "放大-点击.png";
		hoverMark = "放大-悬浮.png";
		showNormal();
		tb_show->setGeometry(QRect(8, ui.center_wgt->height() - 127, ui.center_wgt->width() - 17, 200));
	}
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


void ControlMonitor::recvMainFlow(int mainFlowIndex, bool curFlowFlag) {

	lightnumber = mainFlowIndex - 1;
	mainflow_finish = curFlowFlag;
	flash_load();

}

void ControlMonitor::recieverocketType(int id)
{
	m_pmainFlowDao = new DataBase::mainFlowDao(m_app->m_outputPath);
	m_pmainFlowDao->getMainflow(id);
	light_load();
	flash_load();
	qDebug() << "this is recieved id";
}

void ControlMonitor::acceptchage()
{
	lightnumber = 1;
	m_app->mainflowlist.clear();
	m_pmainFlowDao = new DataBase::mainFlowDao(m_app->m_outputPath);
	m_pmainFlowDao->getMainflow(m_app->m_allRocketType[m_app->m_CurrentRocketType->m_id]->m_id);
	lightreset();
	light_load();
	flash_load();

	qDebug() << "main flow has been changed!";
}