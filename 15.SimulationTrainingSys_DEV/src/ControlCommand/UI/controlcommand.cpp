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
		QMessageBox::warning(nullptr, ("����"), ("ָ���ӿ�δ��ʼ��"));
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
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);//ȥ��������
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
	//��ʽ������
	ConfigNameSpace::StyleManager* stylemanager = new   ConfigNameSpace::StyleManager;
	ConfigNameSpace::ConfigGlobal::gstylemanager = stylemanager;
	//   //���ֹ�����
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
	//����Э��֡
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

	//��������
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

	//����ʱ��
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeupdate()));
	connect(timer, SIGNAL(timeout()), this, SLOT(timecount()));
	timer->start(1000);
	timer->setTimerType(Qt::PreciseTimer);
	baseTime = QTime::currentTime();


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
	if (!m_pCommandDAO->getFaultCommand())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("���ع�����Ϣ������ϣ�");

	m_pRocketDataDAO = new DataBase::RocketDataDAO(m_app->m_outputPath);
	if (!m_pRocketDataDAO->getRocketData())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("���ؼ���ң������֡Э��������ϣ�");
	if (!m_pRocketDataDAO->getRocketParam())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("����֡Э�����������ϣ�");

	QString msg, curtime;
	if (!m_pRocketDataDAO->initConfig(msg))//��ʼ�����ݿⱣ�������ļ��ı���������ھʹ���
	{
		QString info = "���������ļ����ݱ�ʧ��";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("�����ļ����ݱ��ʼ����ɣ�");
	if (m_pRocketDataDAO->ReadConfigTime(msg, curtime))//��ȡ���ñ����ʱ�䣬����������У��򷵻���ʱ�䣬���򷵻�ʧ��
	{
		if (m_pRocketDataDAO->ReadConfigFromDb(msg, curtime))
			displayStatuInfo("��ȡ��̬�����ļ��ɹ���");
	}

	//Э��֡����
	for (auto item : m_app->m_RocketDataFrame)
	{
		item.second->sortParams();
	}
	//InitFrame();
	displayStatuInfo("�����û�������ϣ�");
	displayStatuInfo("ϵͳ������ϣ�");
}
void ControlCommand::timeupdate()
{
	QDateTime time = QDateTime::currentDateTime();
	ui.curtime->setText(time.toString("yyyy-MM-dd hh:mm:ss"));
}
void ControlCommand::timecount()
{
	QTime current = QTime::currentTime();//��ȡϵͳ��ǰʱ��
	int t = this->baseTime.msecsTo(current);  //���������ʱ��֮��
	QTime showtime(0, 0, 0, 0);//��ʼʱ��
	showtime = showtime.addMSecs(t);//����tms
	QString str = showtime.toString("hh:mm:ss"); //ת��Ϊstring����
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
	tb_show->moveCursor(QTextCursor::Start);//ʼ�ն�Ϊ�ڵ�һ��
	QTextCursor cursor = tb_show->textCursor();
	cursor.select(QTextCursor::LineUnderCursor);   //ѡ��Ҫ��ɫ������
	QTextCharFormat fmt;
	fmt.setForeground(is_alarm ? QColor(Qt::red) : QColor(Qt::black));
	cursor.mergeCharFormat(fmt);    //�����ı���ʽ
	cursor.clearSelection(); //����ѡ��
	//cursor.movePosition(QTextCursor::EndOfLine);  //cursor��anchor������ĩβ  
}

void ControlCommand::changeResize()
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
		ui.vl_UI->setContentsMargins(0, 0, 0, 0); //ȥ���������߽�
		this->move(this->pos() + (QPoint(1, 1))); //���������Ҫ 
		showMaximized();
		tb_show->setGeometry(QRect(6, ui.center_wgt->height() - 183, ui.center_wgt->width() - 12, 300));

	}
	else
	{
		mark = "�Ŵ�-Ĭ��.png";
		pressMark = "�Ŵ�-���.png";
		hoverMark = "�Ŵ�-����.png";
		//ui.pb_resize->setToolTip("���");
		showNormal();
		ui.vl_UI->setContentsMargins(5, 5, 5, 5);//�����������߽�
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
	//	changeResize();//�˴��������/��ԭ��ť�����
	//event->ignore();
}
