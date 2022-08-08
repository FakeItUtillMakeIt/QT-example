#include "FaultInjectionLogin.h"

FaultInjectionLogin::FaultInjectionLogin(FaultInjection * fj,QWidget *parent)
	: QWidget(parent)
	, m_pDeviceDAO(nullptr)
{
	ui.setupUi(this);
	m_app = AppCache::instance();
	faultInjection = fj;
	setWindowFlags(Qt::FramelessWindowHint);
	Init();
	connect(ui.LoginBt, SIGNAL(clicked()), this, SLOT(LoginSlot()));
	connect(ui.CloseBt, SIGNAL(clicked()), this, SLOT(CloseSlot()));
}
void FaultInjectionLogin::LoginSlot()
{
	int typeId = ui.TypeComboBox->currentData().toUInt();
	m_app->m_CurrentRocketType = m_app->m_allRocketType[typeId];
	this->close();
	faultInjection->show();
	//ControlComPage.show();
}
void FaultInjectionLogin::CloseSlot()
{
	this->close();
}
FaultInjectionLogin::~FaultInjectionLogin()
{}
void FaultInjectionLogin::Init()
{
	m_app->m_CurrentRocketType = nullptr;
	//���ػ�������
	m_pDeviceDAO = new DataBase::DeviceDAO(m_app->m_outputPath);
	if (!m_pDeviceDAO->getRocketType())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		return;
	}
	ui.TypeComboBox->clear();
	for (auto item : m_app->m_allRocketType)
	{
		ui.TypeComboBox->addItem(QString::fromStdString(Utils::GBKToUTF8(item.second->m_name.c_str())), item.first);
	}
	ui.TypeComboBox->setCurrentIndex(0);
}