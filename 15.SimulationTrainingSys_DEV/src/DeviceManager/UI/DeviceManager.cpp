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
	//ui.lb_title->setText(m_app->m_soft->GetName());//ui �������޸ģ����ⲻ��Ҫ�ٶ��루�
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);//ȥ��������
	this->setWindowTitle(m_app->m_soft->GetName());
	setAttribute(Qt::WA_TranslucentBackground, true);
	//Init();


	/**
	 * ����
	 */
	connect(ui.ceshiCommand1, &QPushButton::clicked, this, [=]() {
		Command* command = new Command();
		command->m_iType = 1;
		command->m_iCode = 2;
		command->m_iBackId = 3;
		command->m_id = 2;

		QVariant temp;
		temp.setValue(command);
		emit m_centeralWidget->receiverCMD(temp);
		});

	connect(ui.ceshiCommand2, &QPushButton::clicked, this, [=]() {
		Command* command = new Command();
		command->m_iType = 1;
		command->m_iCode = 1;
		command->m_iBackId = 3;
		command->m_id = 1;

		QVariant temp;
		temp.setValue(command);
		emit m_centeralWidget->receiverCMD(temp);
		});
	/**
	 * ����over
	 */
}

void DeviceManager::Init()
{
	//��CenterOperate����ΪDeviceManager��������
	//m_centeralWidget = new CenterOperate(ui.center_wgt);
	connect(ui.pb_close, &QPushButton::clicked, this, &DeviceManager::CloseWindow);
	connect(ui.pb_min, &QPushButton::clicked, this, &DeviceManager::ShowMinimized);

	int id = QFontDatabase::addApplicationFont(":/DeviceManager/word/word.ttf");
	QStringList s = QFontDatabase::applicationFontFamilies(id);
	QFont f;
	f.setFamily(s[0]);
	f.setPixelSize(12);
	ui.title->setFont(f);

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

#pragma region ���ػ�������

	m_pDeviceDAO = new DataBase::DeviceDAO(m_app->m_outputPath);
	if (!m_pDeviceDAO->getStatus())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("����״̬������ϣ�");

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


	if (!m_pDeviceDAO->getDeviceStatus())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("�����豸״̬������ϣ�");

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
	//Э��֡����
	for (auto item : m_app->m_RocketDataFrame)
	{
		item.second->sortParams();
	}

	displayStatuInfo("���ػ���������ϣ�");
	displayStatuInfo("ϵͳ������ϣ�");
	InitDevice();

	m_centeralWidget = new CenterOperate(ui.center_wgt);
}


void DeviceManager::CloseWindow()
{
	this->close();
	ParamInfoConfig::InfoConfigWidget::closeInstance();
	DeviceInfoConfig::InfoConfigWidget::closeInstance();
	CommandInfoConfig::InfoConfigWidget::closeInstance();
	RocketInfoConfig::InfoConfigWidget::closeInstance();
	WidgetStyleSheet::closeInstance();
}
void DeviceManager::ShowMinimized()
{
	this->showMinimized();
}

/// <summary>
/// ��ʼ���豸  ״̬�Ͳ���������Ϣ
/// </summary>
void DeviceManager::InitDevice()
{
	//���豸��״̬����Ӧ���������ý��г�ʼ��
	QString initStat = QString("�ϵ�");
	for (Device* eachDev : m_app->m_allDevice)
	{
		eachDev->m_sCurStatus = initStat.toStdString();
	}



	DeviceDBConfigInfo::getInstance()->readStatusInfoDB2UI();
	QString statusFilePath;
	auto curPath = QCoreApplication::applicationDirPath();
	statusFilePath = curPath + "/device/";
	//�豸������ʼ��  �ĸ���Ҫ��ʼ����ֵ
	for (auto ele = m_app->m_allDeviceParam.begin(); ele != m_app->m_allDeviceParam.end(); ele++)
	{
		//��ʼ��״̬����Ҫ��ȡʵʱֵ�б�
		/*������������ʱ��ˢ��*/
		for (auto eachStat : m_app->m_dev2DeviceStatusID[ele->second->m_deviceId])
		{
			QFile file(statusFilePath + QString::fromStdString(m_app->m_allDeviceStatus[eachStat]->m_dataPath));
			auto ret = file.open(QIODevice::ReadOnly | QIODevice::Text);
			if (!ret)
			{
				continue;
			}
			int row = 1;
			QTextStream in(&file);
			in.setCodec("UTF-8");
			QStringList paramNameList;

			unordered_map<string, vector<double>> tempData;

			while (!in.atEnd())
			{
				QString lineData = in.readLine();
				if (row == 1)
				{
					paramNameList = lineData.split(QRegExp("[,]"));
					paramNameList.pop_back();
					row--;
				}
				auto lineDataList = lineData.split(QRegExp("[,]"));
				lineDataList.pop_back();
				//�������ݲ������
				if (lineDataList.size() < paramNameList.size())
				{
					continue;
				}
				for (int i = 0; i < paramNameList.size(); i++)
				{

					tempData[Utils::UTF8ToGBK(paramNameList[i].toStdString().c_str())].push_back(lineDataList[i].toDouble());
					//�����쳣
					//try
					//{
					//	if (lineDataList.size() < paramNameList.size())
					//	{
					//		//throw - 1;
					//		throw QException();
					//	}
					//	else
					//	{
					//		tempData[Utils::UTF8ToGBK(paramNameList[i].toStdString().c_str())].push_back(lineDataList[i].toDouble());
					//	}

					//}
					//catch (QException ex) {
					//	qDebug() << ex.what();
					//}
					//catch (...) {

					//}


				}
			}
			//״̬id->״̬����
			//string tmpKey = DeviceDBConfigInfo::getInstance()->statusInfo[eachStat][1];
			//ele->second->m_statusVal[eachStat] = tempData;
			ele->second->m_statusRealData[m_app->m_allDeviceStatus[eachStat]->m_statusName] = tempData[ele->second->m_subParameterName];
		}

		ele->second->timer->start(50);

	}

}

void DeviceManager::displayStatuInfo(QString info, bool is_alarm)
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

void DeviceManager::changeResize()
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
		changeResize();//�˴��������/��ԭ��ť�����
	event->ignore();
}