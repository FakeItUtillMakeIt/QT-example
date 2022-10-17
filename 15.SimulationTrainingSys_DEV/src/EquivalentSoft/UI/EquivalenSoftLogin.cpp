#include "EquivalenSoftLogin.h"
#include "../../Public/Utils.h"
EquivalenSoftLogin::EquivalenSoftLogin(EquivalentSoft *eq,QWidget *parent)
	: QWidget(parent)
	, m_pDeviceDAO(nullptr)
{
	ui.setupUi(this);
	m_app = AppCache::instance();
	equivalentSoft = eq;
	this->setWindowIcon(QIcon(":/EquivalentSoft/images/任务模拟64_64.ico"));
	setWindowFlags(Qt::FramelessWindowHint);
	Init();
	connect(ui.LoginBt, SIGNAL(clicked()), this, SLOT(LoginSlot()));
	connect(ui.CloseBt, SIGNAL(clicked()), this, SLOT(CloseSlot()));

	connect(ui.TypeComboBox, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &EquivalenSoftLogin::TypeComboBoxChange);

}
void EquivalenSoftLogin::LoginSlot()
{
	int typeId = ui.TypeComboBox->currentData().toUInt();
	m_app->m_CurrentRocketType = m_app->m_allRocketType[typeId];
	m_app->rokecttype->setText(ui.TypeComboBox->currentText());
	this->close();
	equivalentSoft->GetRocketData();
	if (!equivalentSoft->InitFrame())
	{
		QMessageBox::warning(qApp->activeWindow(), QObject::tr("警告"), "没有绑定箭上数据协议！");
		this->close();
	}
	m_app->m_softName = ui.TypeComboBox_2->currentText();
	m_app->m_softID = ui.TypeComboBox_2->currentData().toInt();
	equivalentSoft->setWindowTitle(m_app->m_softName);
	equivalentSoft->CreatConfigInterface();	
	equivalentSoft->show();
	//ControlComPage.show();
}
void EquivalenSoftLogin::CloseSlot()
{
	this->close();
}

EquivalenSoftLogin::~EquivalenSoftLogin()
{}
void EquivalenSoftLogin::Init()
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

	//岗位展示
	TypeComboBoxChange(ui.TypeComboBox->currentText());
}


/// <summary>
/// 岗位展示
/// </summary>
/// <param name="rName"></param>
void EquivalenSoftLogin::TypeComboBoxChange(QString rName)
{
	ui.TypeComboBox_2->clear();
	for (auto itemR : m_app->m_allRocketType)
	{
		if (ui.TypeComboBox->currentText() != QString::fromLocal8Bit(itemR.second->m_name.c_str()))
		{
			continue;
		}

		for (auto itemT : m_app->m_TaskManageInfo)
		{
			if (itemR.second->m_id == itemT.second->m_rocketId)
			{
				ui.TypeComboBox_2->addItem(QString::fromStdString(itemT.second->m_roketSoftName), itemT.first);
			}
		}
	}
	ui.TypeComboBox_2->setCurrentIndex(0);
}
