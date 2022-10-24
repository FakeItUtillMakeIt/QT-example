#include "DeviceManagerLogin.h"
#include "../../Public/Utils.h"
#include <QMovie>

#include <QMessageBox>

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>


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
	QLabel* loadingLabel = new QLabel("数据加载中......");
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
	//数据库初始化 报数据库么有加载错误
	//if (DatabseInit() == false)
	//{
	//	QMessageBox::critical(this,"错误","数据库初始化失败！","确定");
	//	return;
	//}

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

bool DeviceManagerLogin::DatabseInit()
{
	QString creteSql = "";

	m_database = QSqlDatabase::addDatabase("QMYSQL", "Database");
	m_database.setHostName("localhost");     // 数据库服务器IP127.0.01
	m_database.setUserName("root");          // 数据库用户名
	m_database.setPassword("123456");        // 密码
	//m_database.open();                     // 使用当前设置的连接值（就是上面设置的那些值），打开数据库连接
	if (m_database.open() == false)
	{
		QSqlError sql_error = m_database.lastError();
		qDebug() << sql_error;
		QMessageBox::information(this, "提示", "连接测试数据库失败！", "确定");
		return false;
	}

	QString cmd = "show databases;";
	QSqlQuery query(cmd, m_database);
	while (query.next()) {
		if (query.value(0).toString() == "simulatedtraining")
		{
			//QMessageBox::information(this, "提示", "gai数据库已存在！", QMessageBox::Ok);
			return true;
		}
		qDebug() << "database name:" << query.value(0).toString();
	}

	// 创建数据库。
	creteSql = QString("create database if not exists %1").arg("simulatedtraining");
	m_database.exec(creteSql);

	// 连接上刚刚创建好的数据库
	m_database.setDatabaseName("simulatedtraining");
	// 必须重新调用一遍.open()
	if (m_database.open() == false)
	{
		QMessageBox::information(this, "提示", "创建并打开数据库失败！", QMessageBox::Ok);
		return false;
	}

	//ExcuteSqlFile();
}

int DeviceManagerLogin::ExcuteSqlFile()
{
	int iRet = 0;
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
	QSqlQuery qsql(m_database);
	QStringList qstrlist_sql = qstr_file_data.split(";");
	for (int i = 0; i < qstrlist_sql.size() - 1; i++) {
		QString qstr_sql_part = qstrlist_sql.at(i).toUtf8();
		bool sql_result = qsql.exec(qstr_sql_part);
		if (!sql_result) {
			QSqlError sql_error = qsql.lastError();
			//std::cout << sql_error.text().toStdString() << std::endl;
			//std::cout << sql_error.number() << std::endl;
			qDebug() << sql_error;
			QMessageBox::information(this, "提示", "sql文件运行出错！", QMessageBox::Ok);
			iRet = -1;
			break;
		}
	}

	QMessageBox::information(this, "提示", "sql文件运行完毕！", QMessageBox::Ok);
	return iRet;
}