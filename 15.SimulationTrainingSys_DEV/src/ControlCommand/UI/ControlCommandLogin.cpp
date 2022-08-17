#include "ControlCommandLogin.h"
#include "ControlCommand.h"
#include <QpushButton>
ControlCommandLogin::ControlCommandLogin(ControlCommand *conCom,QWidget *parent)
	: QWidget(parent)
	, m_pDeviceDAO(nullptr)
{
	ui.setupUi(this);
	m_app = AppCache::instance();
	setWindowFlags(Qt::FramelessWindowHint);
	controlCommand = conCom;
	Init();
	connect(ui.LoginBt, SIGNAL(clicked()), this,SLOT(LoginSlot()));
	connect(ui.CloseBt, SIGNAL(clicked()), this, SLOT(CloseSlot()));
}
void ControlCommandLogin::LoginSlot()
{
	int typeId = ui.TypeComboBox->currentData().toUInt();
	m_app->m_CurrentRocketType = m_app->m_allRocketType[typeId];
	m_app->rokecttype->setText(ui.TypeComboBox->currentText());
	this->close();
	if (!controlCommand->InitFrame())
	{
		QMessageBox::warning(qApp->activeWindow(), QObject::tr("警告"), "该型号没有绑定箭上数据协议！");
		this->close();
	}
	controlCommand->CreatConfigInterface();
	controlCommand->show(); 
}
void ControlCommandLogin::CloseSlot() 
{
	this->close();
}
ControlCommandLogin::~ControlCommandLogin()
{}
void ControlCommandLogin::Init()
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