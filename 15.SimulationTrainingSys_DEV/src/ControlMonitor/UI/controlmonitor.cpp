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
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);//ȥ��������
	this->setWindowTitle(m_app->m_soft->GetName());
	this->setGeometry(0, 0, 1920, 1080);
	setAttribute(Qt::WA_TranslucentBackground, true);
	//�������������Ա����font
	int id = QFontDatabase::addApplicationFont(":/ControlMonitor/word/word.ttf");
	QStringList s = QFontDatabase::applicationFontFamilies(id);
	font.setFamily(s[0]);

	testnum = 0;
	Init();

	//ָʾ�Ʋ����ô��� 
	light_info.append(new Light_info(1, "���Ƽ������", 0));
	light_info.append(new Light_info(2, "�����������", 0));
	light_info.append(new Light_info(3, "�������", 0));
	light_info.append(new Light_info(4, "��׼���", 0));
	light_info.append(new Light_info(5, "�������", 0));
	light_info.append(new Light_info(6, "������", 0));
	light_info.append(new Light_info(7, "��׼", 0));
	light_info.append(new Light_info(8, "��׼�ϵ�", 0));
	light_info.append(new Light_info(9, "�ϴ���Ԫ����", 0));
	light_info.append(new Light_info(10, "�ϴ���׼��", 0));
	light_info.append(new Light_info(11, "���Ǽ��", 0));
	light_info.append(new Light_info(12, "�������װ��", 0));
	light_info.append(new Light_info(13, "���ƿ���", 0));
	light_info.append(new Light_info(14, "��Ա����", 0));
	light_info.append(new Light_info(15, "������ʼ��¼", 0));
	light_info.append(new Light_info(16, "�����������", 0));
	light_info.append(new Light_info(17, "��ȫת����", 0));
	light_info.append(new Light_info(18, "��·����", 0));
	light_info.append(new Light_info(19, "Һ·����", 0));
	light_info.append(new Light_info(20, "��ؼ���", 0));
	light_info.append(new Light_info(21, "����", 0));
	QTimer* test = new QTimer(this);
	connect(test, SIGNAL(timeout()), this, SLOT(test()));
	test->start(6000);










	light_load();
}
//���Ժ���
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
	//������ط�������ҳ����ui.page1 ��
	m_pCenterOperate = new CenterOperate(ui.page2);
	//����������ط�������չʾҳ��������Ϊui.page1
	/************************************************************************/
	/* ���ﲻ��ɾ��2022/07/29  �����޸�Ĭ����ʾ����zhukongclickע�;���                                                                     */
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
	ui.page1->setStyleSheet("background-image:url(:/flowload/images/Flow/�׵�20%͸��@2x.png);");
	//Ĭ����ʾ����
	zhukongclick();
	/************************************************************************/
	/* 2022/07/29�޸Ľ���                                                                     */
	/************************************************************************/

	connect(ui.pb_close, &QPushButton::clicked, this, &ControlMonitor::CloseWindow);
	connect(ui.pb_min, &QPushButton::clicked, this, &ControlMonitor::ShowMinimized);

	connect(m_pCenterOperate, &CenterOperate::curRunCmd, flowDisplayWidget, &FlowDisplayWidget::updateFlowStat);
	//connect(ui.pb_resize, &QPushButton::clicked, this, &ControlMonitor::changeResize);


	//ָʾ�Ƽ��
	lightnumber = 0;
	lightflag = true;
	inspect = new QTimer(this);
	connect(inspect, SIGNAL(timeout()), this, SLOT(light_inspect()));
	inspect->start(1000);
	flush = new QTimer(this);
	connect(flush, SIGNAL(timeout()), this, SLOT(light_flash()));


	//��������
	ui.softwarename->setFont(font);
	ui.rtlabel->setFont(font);
	ui.ctlabel->setFont(font);
	ui.ftlabel->setFont(font);
	ui.rokect_type->setFont(font);
	ui.curtime->setFont(font);
	ui.time->setFont(font);
	ui.rokect_type->setText(m_app->m_soft->GetType());

	//����ʱ��
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeupdate()));
	connect(timer, SIGNAL(timeout()), this, SLOT(timecount()));
	timer->start(1000);
	timer->setTimerType(Qt::PreciseTimer);
	baseTime = QTime::currentTime();


	//��ť�����л�

	connect(ui.zhukong, SIGNAL(clicked()), this, SLOT(zhukongclick()));
	connect(ui.erwei, SIGNAL(clicked()), this, SLOT(erwei_displayclick()));


	//��ʼ��������Ϣ��ʾ��
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

	//���ػ�������
	m_pUserDAO = new DataBase::UserDAO(m_app->m_outputPath);
	if (!m_pUserDAO->getUser())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	//�豸����
	m_pDeviceDAO = new DataBase::DeviceDAO(m_app->m_outputPath);
	if (!m_pDeviceDAO->getDevice())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("�����豸����������ϣ�");

	if (!m_pDeviceDAO->getDeviceParam())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("�����豸����������ϣ�");

	m_pCommandDAO = new DataBase::CommandDAO(m_app->m_outputPath);
	if (!m_pCommandDAO->getCommand())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("����ָ��������ϣ�");
	if (!m_pCommandDAO->getCommandParam())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("����ָ�����������ϣ�");
	displayStatuInfo("�����û�������ϣ�");
	displayStatuInfo("ϵͳ������ϣ�");
}

//ָʾ�Ƽ���
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
//ָʾ��״̬���
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
//ָʾ����˸
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
//�ײ�tabҳ�л�
void ControlMonitor::zhukongclick()
{
	ui.tabwgt->setCurrentIndex(0);//������Ϊpage1
	ui.zhukong->setStyleSheet("QPushButton{color:rgb(0,0,0);border:none;background-color:rgb(242,247,252);}");
	ui.erwei->setStyleSheet("QPushButton{color:rgb(180,180,180);border:none;}QPushButton:hover{color:black;border:none;}");

	//�������̱���
	if (flowDisplayWidget == nullptr)
	{
		ui.page2->hide();
		ui.page1->setStyleSheet("background-color:rgb(245,245,245);");
		//ui.page1->setStyleSheet("background-image:url(:/flowload/images/Flow/�׵�20%͸��@2x.png);");
		//�������Զ��ȼ�������
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
	ui.tabwgt->setCurrentIndex(1);//������Ϊpage2
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

	QTime current = QTime::currentTime();//��ȡϵͳ��ǰʱ��
	int t = this->baseTime.msecsTo(current);  //���������ʱ��֮��
	QTime showtime(0, 0, 0, 0);//��ʼʱ��
	showtime = showtime.addMSecs(t);//����tms
	QString str = showtime.toString("hh:mm:ss"); //ת��Ϊstring����
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
	tb_show->moveCursor(QTextCursor::Start);//ʼ�ն�Ϊ�ڵ�һ��
	QTextCursor cursor = tb_show->textCursor();
	cursor.select(QTextCursor::LineUnderCursor);   //ѡ��Ҫ��ɫ������
	QTextCharFormat fmt;
	fmt.setForeground(is_alarm ? QColor(Qt::red) : QColor(Qt::black));
	cursor.mergeCharFormat(fmt);    //�����ı���ʽ
	cursor.clearSelection(); //����ѡ��
	//cursor.movePosition(QTextCursor::EndOfLine);  //cursor��anchor������ĩβ  
}

void ControlMonitor::changeResize()
{
	m_isMax = !m_isMax;
	QString hoverMark;
	QString pressMark;
	QString mark;

	if (m_isMax)
	{
		mark = "��С-Ĭ��.png";
		pressMark = "��С-���.png";
		hoverMark = "��С-����.png";
		//ui.pb_resize->setToolTip("���»�ԭ");
		//ui.vl_UI->setContentsMargins(0, 0, 0, 0); //ȥ���������߽�
		this->move(this->pos() + (QPoint(1, 1))); //���������Ҫ 
		showMaximized();
		tb_show->setGeometry(QRect(6, ui.center_wgt->height() - 73, ui.center_wgt->width() - 12, 300));

	}
	else
	{
		mark = "�Ŵ�-Ĭ��.png";
		pressMark = "�Ŵ�-���.png";
		hoverMark = "�Ŵ�-����.png";
		//ui.pb_resize->setToolTip("���");
		showNormal();
		//ui.vl_UI->setContentsMargins(5, 5, 5, 5);//�����������߽�
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
	//	changeResize();//�˴��������/��ԭ��ť�����
	//event->ignore();
}
