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
	m_app->m_rockedType = 2;//ѡ��������,TODO��Ҫ�޸ģ���������ڽ����Ͻ���ѡ��

	ui.rocket_type->setText(m_app->m_soft->GetType());
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);//ȥ��������
	this->setWindowTitle(m_app->m_soft->GetName());
	setAttribute(Qt::WA_TranslucentBackground, true);

	Init();
}

void FaultInjection::Init()
{
	connect(ui.pb_close, &QPushButton::clicked, this, &FaultInjection::CloseWindow);
	connect(ui.pb_min, &QPushButton::clicked, this, &FaultInjection::ShowMinimized);
	//connect(ui.pb_resize, &QPushButton::clicked, this, &FaultInjection::changeResize);
	//ʱ����زۺ�����
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeupdate()));
	connect(timer, SIGNAL(timeout()), this, SLOT(timecount()));
	timer->start(1000);
	timer->setTimerType(Qt::PreciseTimer);
	baseTime = QTime::currentTime();
	
	//��������
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
	m_pFaultDAO = new DataBase::FaultDAO(m_app->m_outputPath);
	if (!m_pFaultDAO->getFault())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}

	//��ȡrocket_info����Ϣ
	if (!m_pFaultDAO->GetAllRocketInfoFrame())
	{
		QString info = "��ȡrocket_info������Ϣʧ�ܣ�";
		displayStatuInfo(info, true);
		return;
	}

	//��ȡcommand_info����Ϣ
	if (!m_pFaultDAO->GetAllCommandInfoFrame())
	{
		QString info = "��ȡcommand_info������Ϣʧ�ܣ�";
		displayStatuInfo(info, true);
		return;
	}

	//��ȡcommand_param_info����Ϣ
	if (!m_pFaultDAO->GetAllCommandParamInfoFrame())
	{
		QString info = "��ȡcommand_param_info������Ϣʧ�ܣ�";
		displayStatuInfo(info, true);
		return;
	}

	m_pFaultParamDAO = new DataBase::FaultParamDAO(m_app->m_outputPath);
	//��ȡfault_param_info����Ϣ
	if (!m_pFaultParamDAO->GetAllFaultParamInfoFrames())
	{
		QString info = "��ȡfault_param_info������Ϣʧ�ܣ�";
		displayStatuInfo(info, true);
		return;
	}

	//��ȡ���ϲ���fault_command_info����Ϣ
	if (!m_pFaultParamDAO->GetAllFaultCommandInfoFrames())
	{
		QString info = "��ȡfault_command_info������Ϣʧ�ܣ�";
		displayStatuInfo(info, true);
		return;
	}


	m_pDeviceDAO = new DataBase::DeviceDAO(m_app->m_outputPath);
	//��ȡ���ϲ���parameter_info����Ϣ
	if (!m_pDeviceDAO->GetAllParameterInfoFrames())
	{
		QString info = "��ȡparameter_info������Ϣʧ�ܣ�";
		displayStatuInfo(info, true);
		return;
	}

	//��ȡ���ϲ���device_info����Ϣ
	if (!m_pDeviceDAO->GetAllDeviceInfoFrames())
	{
		QString info = "��ȡrocket_info������Ϣʧ�ܣ�";
		displayStatuInfo(info, true);
		return;
	}


	//��ȡ���ϲ���device_param_info����Ϣ
	if (!m_pDeviceDAO->GetAllDeviceParamInfoFrames())
	{
		QString info = "��ȡdevice_param_info������Ϣʧ�ܣ�";
		displayStatuInfo(info, true);
		return;
	}


	QHBoxLayout* hlayout = new QHBoxLayout;
	m_centerOperate = new CenterOperate();
	hlayout->addWidget(m_centerOperate);
	ui.center_wgt->setLayout(hlayout);

	displayStatuInfo("���ع���������ϣ�");
	displayStatuInfo("ϵͳ������ϣ�");
}
void FaultInjection::timeupdate()
{

	QDateTime time = QDateTime::currentDateTime();
	ui.curtime->setText(time.toString("yyyy-MM-dd hh:mm:ss"));
}
void FaultInjection::timecount()
{
	
	QTime current = QTime::currentTime();//��ȡϵͳ��ǰʱ��
	int t = this->baseTime.msecsTo(current);  //���������ʱ��֮��
	QTime showtime(0, 0, 0, 0);//��ʼʱ��
	showtime = showtime.addMSecs(t);//����tms
	QString str = showtime.toString("hh:mm:ss"); //ת��Ϊstring����
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
	tb_show->moveCursor(QTextCursor::Start);//ʼ�ն�Ϊ�ڵ�һ��
	QTextCursor cursor = tb_show->textCursor();
	cursor.select(QTextCursor::LineUnderCursor);   //ѡ��Ҫ��ɫ������
	QTextCharFormat fmt;
	fmt.setForeground(is_alarm ? QColor(Qt::red) : QColor(Qt::black));
	cursor.mergeCharFormat(fmt);    //�����ı���ʽ
	cursor.clearSelection(); //����ѡ��
	//cursor.movePosition(QTextCursor::EndOfLine);  //cursor��anchor������ĩβ  
}

void FaultInjection::changeResize()
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
		tb_show->setGeometry(QRect(6, ui.center_wgt->height() - 223, ui.center_wgt->width() - 12, 300));

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
	//	changeResize();//�˴��������/��ԭ��ť�����
	//event->ignore();
}
