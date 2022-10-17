#include "DeviceManagerLogin.h"
#include "../../Public/Utils.h"
#include <QMovie>




DeviceManagerLogin::DeviceManagerLogin(SingleApplication* app,DeviceManager* devManager, QWidget* parent)
	: QWidget(parent)
	, m_pDeviceDAO(nullptr)
{
	ui.setupUi(this);
	m_app = AppCache::instance();
	m_nowApp = app;
	deviceManager = devManager;
	this->setWindowIcon(QIcon(":/DeviceManager/images/教学管理64_64.ico"));
	setWindowFlags(Qt::FramelessWindowHint);
	Init();
	connect(ui.LoginBt, SIGNAL(clicked()), this, SLOT(LoginSlot()));
	connect(ui.CloseBt, SIGNAL(clicked()), this, SLOT(CloseSlot()));

	dataLoading = new QWidget(deviceManager);
	dataLoading->setFixedSize(QApplication::desktop()->size());
	dataLoading->setAttribute(Qt::WA_AlwaysStackOnTop);
	dataLoading->setAutoFillBackground(true);
	QGridLayout* dataLoadLayout = new QGridLayout;
	QLabel* loadingLabel = new QLabel("数据加载中...");
	loadingLabel->setAlignment(Qt::AlignCenter);
	QFont font;
	font.setBold(true);
	font.setPixelSize(20);
	loadingLabel->setFont(font);

	dataLoading->hide();

	//动画
	/*loadingLabel->setMovie(mv);
	mv->start();*/
	/*static QMovie mv1(":/DeviceManager/icon/loading2.gif");
	loadingLabel->setMovie(&mv1);
	mv1.start();*/


	dataLoadLayout->addWidget(loadingLabel);
	dataLoading->setLayout(dataLoadLayout);

	connect(deviceManager, &DeviceManager::deviceLoadOver, this, [=]() {
		deviceManager->show();
		dataLoading->deleteLater();

		});

}
void DeviceManagerLogin::LoginSlot()
{
	int typeId = ui.TypeComboBox->currentData().toUInt();
	m_app->m_CurrentRocketType = m_app->m_allRocketType[typeId];
	
	m_nowApp->w = deviceManager;
	dataLoading->show();

	this->close();
	deviceManager->Init();

	//	deviceManager->show();
	//ControlComPage.show();
}
void DeviceManagerLogin::CloseSlot()
{
	this->close();
}

DeviceManagerLogin::~DeviceManagerLogin()
{}

void DeviceManagerLogin::Init()
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