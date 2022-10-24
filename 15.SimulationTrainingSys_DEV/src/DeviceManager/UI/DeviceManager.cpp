#include "DeviceManager.h" 
#include <qmath.h>
#include <QDateTime>
#include <QFont>
DeviceManager::DeviceManager(QWidget* parent)
	: QMainWindow(parent)
	, m_pCenterOperate(nullptr)
	, m_CenterOperateF(nullptr)
	, m_taskConfiguration(nullptr)
	, m_hlayoutF(nullptr)
	, m_hlayouttask(nullptr)
	, m_pDeviceDAO(nullptr)
	, m_pCommandDAO(nullptr)
	, m_pRocketDataDAO(nullptr)
	, tb_show(nullptr)
	, m_myInfoTip(nullptr)
	, m_isMax(false)
	, m_hlayoutdisplay(nullptr)
{
	ui.setupUi(this);
	m_app = AppCache::instance();
	//ui.lb_title->setText(m_app->m_soft->GetName());//ui 进行了修改，标题不需要再读入（杨）
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);//去掉标题栏
	this->setWindowTitle(m_app->m_soft->GetName());
	this->setWindowIcon(QIcon(":/DeviceManager/images/教学管理64_64.ico"));
	setAttribute(Qt::WA_TranslucentBackground, true);
	ui.tabWidget->setCurrentIndex(0);
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
	ui.rtlabel->setFont(f);
	ui.ctlabel->setFont(f);
	ui.ctlabel->setStyleSheet("color:#98b8f9;font-size:28px;");
	ui.rtlabel->setStyleSheet("color:#98b8f9;font-size:28px;");
	for each (auto var in m_app->m_allRocketType)
	{
		ui.comboBox->addItem(QString::fromLocal8Bit(var.second->m_name.c_str()), var.second->m_id);
	}
	ui.comboBox->setCurrentText(QString::fromLocal8Bit(m_app->m_CurrentRocketType->m_name.c_str()));
	ui.comboBox->setFont(f);
	ui.curtime->setFont(f);

	connect(ui.comboBox, QOverload<const QString&>::of(&QComboBox::currentTextChanged), this, [=]() {

		m_app->m_CurrentRocketType->m_id = ui.comboBox->currentData().toInt();
		m_app->m_CurrentRocketType->m_name = ui.comboBox->currentText().toLocal8Bit();
		//m_app->m_CurrentRocketType = m_app->m_allRocketType[ui.comboBox->currentData().toUInt()];
		emit rocketTypeChanged();

		DataFaultLoad();//故障重新加载
		TaskManagement();//任务岗位配置
		preRocket = ui.comboBox->currentText();
		});

	//初始化调试信息显示区
	m_myInfoTip = new MyInfoTip(ui.wgt_status_left);
	//ui.wgt_status_left->setStyleSheet("background-color:red");
	connect(m_myInfoTip, &MyInfoTip::hoverInfo, this, [this]() {
		tb_show->raise();
		tb_show->show();

		});

	connect(ui.tabWidget, &QTabWidget::currentChanged, this, [=](int index)
		{
			if (index == 2)
			{
				//DataFaultLoad();//故障重新加载
			}
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

	m_centeralWidget = new CenterOperate(ui.tab_DeviceManagement);//设备管理


	connect(this->findChild<RocketTypeManageModule*>("rocketUI"), &RocketTypeManageModule::rocketInfoChanged, this, [=]() {

		ui.comboBox->disconnect(ui.comboBox, QOverload<const QString&>::of(&QComboBox::currentTextChanged), nullptr, nullptr);
		curSelectedText = ui.comboBox->currentText();
		ui.comboBox->clear();

		DeviceDBConfigInfo::getInstance()->readRocketDB2UI();
		for (auto elem : DeviceDBConfigInfo::getInstance()->rocketInfo)
		{
			ui.comboBox->addItem(QString::fromStdString(elem.second[1]), elem.first);
		}

		//ui.comboBox->setCurrentText(curSelectedText);

		connect(ui.comboBox, QOverload<const QString&>::of(&QComboBox::currentTextChanged), this, [=]() {

			m_app->m_CurrentRocketType->m_id = ui.comboBox->currentData().toInt();
			m_app->m_CurrentRocketType->m_name = ui.comboBox->currentText().toLocal8Bit();
			//m_app->m_CurrentRocketType = m_app->m_allRocketType[ui.comboBox->currentData().toUInt()];
			emit rocketTypeChanged();

			DataFaultLoad();//故障重新加载
			TaskManagement();//任务岗位配置
			preRocket = ui.comboBox->currentText();
			});
		if (curSelectedText != ui.comboBox->currentText())
		{
			emit ui.comboBox->currentTextChanged(ui.comboBox->currentText());
		}

		});

	//故障加载
	DataFaultLoad();
	//任务岗位配置
	TaskManagement();
	//二维展示
	//new twoDdisplay(ui.tab_taskManege);
	if (m_hlayoutdisplay != nullptr)
	{
		/*delete m_twoDdisplay;
		m_twoDdisplay = nullptr;*/

		delete m_hlayoutdisplay;
		m_hlayoutdisplay = nullptr;
	}
	m_twoDdisplay = new twoDdisplay(this);
	m_hlayoutdisplay = new QHBoxLayout;
	m_hlayoutdisplay->setContentsMargins(0, 0, 0, 0);
	m_hlayoutdisplay->addWidget(m_twoDdisplay);
	ui.tab_Display->setLayout(m_hlayoutdisplay);

	QElapsedTimer timer2;
	displayStatuInfo("加载基础数据完毕！");
	displayStatuInfo("系统启动完毕！");
	InitDevice();
	//qDebug() << "初始化设备用时:" << timer2.elapsed();

	emit deviceLoadOver();

}


/// <summary>
/// 获取故障注入所需要的数据
/// </summary>
void DeviceManager::DataFaultLoad()
{
	m_app->m_rockedType = m_app->m_CurrentRocketType->m_id;//选择火箭类型

	//加载基础数据
	m_pFaultDAOF = new DataBaseF::FaultDAO(m_app->m_outputPath);
	if (!m_pFaultDAOF->getFault())
	{
		QString info = "建立数据库连接失败，请检查数据库配置文件";
		displayStatuInfo(info, true);
		//return;
	}

	//获取rocket_info表信息
	if (!m_pFaultDAOF->GetAllRocketInfoFrame())
	{
		QString info = "获取rocket_info数据信息失败！";
		displayStatuInfo(info, true);
		//return;
	}

	//获取command_info表信息
	if (!m_pFaultDAOF->GetAllCommandInfoFrame())
	{
		QString info = "获取command_info数据信息失败！";
		displayStatuInfo(info, true);
		//return;
	}

	//获取command_param_info表信息
	if (!m_pFaultDAOF->GetAllCommandParamInfoFrame())
	{
		QString info = "获取command_param_info数据信息失败！";
		displayStatuInfo(info, true);
		//return;
	}

	m_pFaultParamDAOF = new DataBaseF::FaultParamDAO(m_app->m_outputPath);
	//获取fault_param_info表信息
	if (!m_pFaultParamDAOF->GetAllFaultParamInfoFrames())
	{
		QString info = "获取fault_param_info数据信息失败！";
		displayStatuInfo(info, true);
		//return;
	}

	//获取故障参数fault_command_info表信息
	if (!m_pFaultParamDAOF->GetAllFaultCommandInfoFrames())
	{
		QString info = "获取fault_command_info数据信息失败！";
		displayStatuInfo(info, true);
		//return;
	}


	m_pDeviceDAOF = new DataBaseF::DeviceDAO(m_app->m_outputPath);
	//获取故障参数parameter_info表信息
	if (!m_pDeviceDAOF->GetAllParameterInfoFrames())
	{
		QString info = "获取parameter_info数据信息失败！";
		displayStatuInfo(info, true);
		//return;
	}

	//获取故障参数device_info表信息
	if (!m_pDeviceDAOF->GetAllDeviceInfoFrames())
	{
		QString info = "获取rocket_info数据信息失败！";
		displayStatuInfo(info, true);
		//return;
	}


	//获取故障参数device_param_info表信息
	if (!m_pDeviceDAOF->GetAllDeviceParamInfoFrames())
	{
		QString info = "获取device_param_info数据信息失败！";
		displayStatuInfo(info, true);
		//return;
	}

	if (!m_pDeviceDAOF->GetDeviceIDParamIDFrames())
	{
		QString info = "获取数据信息失败！";
		displayStatuInfo(info, true);
		//return;
	}

	if (m_hlayoutF != nullptr)
	{
		delete m_CenterOperateF;
		m_CenterOperateF = nullptr;

		delete m_hlayoutF;
		m_hlayoutF = nullptr;
	}
	m_CenterOperateF = new CenterOperateF();
	m_hlayoutF = new QHBoxLayout;
	m_hlayoutF->setContentsMargins(0, 0, 0, 0);
	m_hlayoutF->addWidget(m_CenterOperateF);
	ui.tab_faultInjection->setLayout(m_hlayoutF);

	displayStatuInfo("加载故障数据完毕！");
}

/// <summary>
/// 加载任务岗位配置m_pRocketDataDAO
/// </summary>
void DeviceManager::TaskManagement()
{
	//m_app->m_rockedType = m_app->m_CurrentRocketType->m_id;//选择火箭类型

	if (!m_pRocketDataDAO->GetTaskInfo())
	{
		QString info = "获取岗位信息失败";
		displayStatuInfo(info, true);
		return;
	}

	if (m_hlayouttask != nullptr)
	{
		delete m_taskConfiguration;
		m_taskConfiguration = nullptr;

		delete m_hlayouttask;
		m_hlayouttask = nullptr;
	}
	m_taskConfiguration = new TaskConfiguration(this);
	m_hlayouttask = new QHBoxLayout;
	m_hlayouttask->setContentsMargins(0, 0, 0, 0);
	m_hlayouttask->addWidget(m_taskConfiguration);
	ui.tab_taskManege->setLayout(m_hlayouttask);
}

void DeviceManager::timeUpdate() {

	QDateTime time = QDateTime::currentDateTime();
	ui.curtime->setText(time.toString("yyyy-MM-dd hh:mm:ss"));

}

void DeviceManager::CloseWindow()
{
	if (m_app->IsDeleting == true)
	{
		QMessageBox::information(nullptr, "信息", "数据删除中，请稍后！");
		return;
	}

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
		QMessageBox::information(this, "信息", "数据加载完毕！", "确定");
		});

	////设备参数初始化  四个需要初始化的值
	for (auto ele = m_app->m_allDeviceParam.begin(); ele != m_app->m_allDeviceParam.end(); ele++)
	{
		ele->second->timer->start(50);

	}

}

/// <summary>
/// 检查数据库是否存在，不存在就创建相关信息
/// </summary>
bool DeviceManager::CheckDatabase()
{
	return true;

	//判断数据库中是否存 SimulatedTraining 数据库
	if (m_pDeviceDAO->Isexist("SimulatedTraining"))
	{
		return true;
	}

	//执行此数据库，大约需要两分钟
	QString path = QCoreApplication::applicationDirPath().append("/linglongdata.sql");
	QFile qfile(path);
	if (!qfile.exists()) {
		return -1;
	}
	if (!qfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return -1;
	}
	QTextStream in(&qfile);
	in.setCodec("UTF-8");
	QString qstr_file_data = in.readAll();
	//QSqlQuery qsql(m_database);
	QStringList qstrlist_sql = qstr_file_data.split(";");
	for (int i = 0; i < qstrlist_sql.size() - 1; i++) {
		QString qstr_sql_part = qstrlist_sql.at(i).toUtf8();
		bool sql_result = m_pDeviceDAO->exec_sql(qstr_sql_part.toStdString());
		if (!sql_result) {
			//QSqlError sql_error = qsql.lastError();
			//std::cout << sql_error.text().toStdString() << std::endl;
			//std::cout << sql_error.number() << std::endl;
			//qDebug() << sql_error;
			//iRet = -1;
			return false;
		}
	}

	return true;

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
	//if (Qt::LeftButton == event->button())
	//	changeResize();//此处调用最大化/还原按钮点击槽
	//event->ignore();
}

