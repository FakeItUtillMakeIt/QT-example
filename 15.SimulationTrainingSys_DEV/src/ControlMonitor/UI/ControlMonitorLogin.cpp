#include "ControlMonitorLogin.h"
ControlMonitorLogin::ControlMonitorLogin(SingleApplication* app, ControlMonitor* conMoni, QWidget* parent)
	: QWidget(parent)
	, m_pDeviceDAO(nullptr)
{
	ui.setupUi(this);
	m_app = AppCache::instance();
	this->setWindowIcon(QIcon(":/ControlMonitor/images/C3I-64_64.ico"));
	setWindowFlags(Qt::FramelessWindowHint);
	controlMonitor = conMoni;
	m_nowApp = app;
	Init();
	connect(this, &ControlMonitorLogin::sendrocketType, conMoni, &ControlMonitor::recieverocketType);
	connect(ui.LoginBt, SIGNAL(clicked()), this, SLOT(LoginSlot()));
	connect(ui.CloseBt, SIGNAL(clicked()), this, SLOT(CloseSlot()));
}
void ControlMonitorLogin::LoginSlot()
{
	int typeId = ui.TypeComboBox->currentData().toUInt();
	m_app->m_CurrentRocketType = m_app->m_allRocketType[typeId];

	//m_app->rokecttype->setText(ui.TypeComboBox->currentText());

	m_nowApp->w = controlMonitor;
	this->close();
	controlMonitor->Init();
	emit sendrocketType(m_app->m_allRocketType[typeId]->m_id);
	controlMonitor->show();
}
void ControlMonitorLogin::CloseSlot()
{
	this->close();
}

ControlMonitorLogin::~ControlMonitorLogin()
{}
void ControlMonitorLogin::Init()
{
	m_app->m_CurrentRocketType = nullptr;
	//加载基础数据
	m_pDeviceDAO = new DataBase::DeviceDAO(m_app->m_outputPath);
	if (!m_pDeviceDAO->getRocketType())
	{
		QString info = "建立数据库连接失败，请检查数据库配置文件";
		return;
	}
	ui.TypeComboBox->clear();
	for (auto item : m_app->m_allRocketType)
	{
		ui.TypeComboBox->addItem(QString::fromStdString(Utils::GBKToUTF8(item.second->m_name.c_str())), item.first);
	}
	ui.TypeComboBox->setCurrentIndex(0);
}