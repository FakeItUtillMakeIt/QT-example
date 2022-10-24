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
	this->setWindowIcon(QIcon(":/DeviceManager/images/��ѧ����64_64.ico"));
	setWindowFlags(Qt::FramelessWindowHint);
	Init();
	connect(ui.LoginBt, SIGNAL(clicked()), this, SLOT(LoginSlot()));
	connect(ui.CloseBt, SIGNAL(clicked()), this, SLOT(CloseSlot()));

	dataLoading = new QWidget(deviceManager);
	dataLoading->setFixedSize(QApplication::desktop()->size());
	dataLoading->setAttribute(Qt::WA_AlwaysStackOnTop);
	dataLoading->setAutoFillBackground(true);
	QGridLayout* dataLoadLayout = new QGridLayout;
	QLabel* loadingLabel = new QLabel("���ݼ�����......");
	loadingLabel->setAlignment(Qt::AlignCenter);
	QFont font;
	font.setBold(true);
	font.setPixelSize(20);
	loadingLabel->setFont(font);

	dataLoading->hide();

	//����
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
	//���ݿ��ʼ�� �����ݿ�ô�м��ش���
	//if (DatabseInit() == false)
	//{
	//	QMessageBox::critical(this,"����","���ݿ��ʼ��ʧ�ܣ�","ȷ��");
	//	return;
	//}

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

bool DeviceManagerLogin::DatabseInit()
{
	QString creteSql = "";

	m_database = QSqlDatabase::addDatabase("QMYSQL", "Database");
	m_database.setHostName("localhost");     // ���ݿ������IP127.0.01
	m_database.setUserName("root");          // ���ݿ��û���
	m_database.setPassword("123456");        // ����
	//m_database.open();                     // ʹ�õ�ǰ���õ�����ֵ�������������õ���Щֵ���������ݿ�����
	if (m_database.open() == false)
	{
		QSqlError sql_error = m_database.lastError();
		qDebug() << sql_error;
		QMessageBox::information(this, "��ʾ", "���Ӳ������ݿ�ʧ�ܣ�", "ȷ��");
		return false;
	}

	QString cmd = "show databases;";
	QSqlQuery query(cmd, m_database);
	while (query.next()) {
		if (query.value(0).toString() == "simulatedtraining")
		{
			//QMessageBox::information(this, "��ʾ", "gai���ݿ��Ѵ��ڣ�", QMessageBox::Ok);
			return true;
		}
		qDebug() << "database name:" << query.value(0).toString();
	}

	// �������ݿ⡣
	creteSql = QString("create database if not exists %1").arg("simulatedtraining");
	m_database.exec(creteSql);

	// �����ϸոմ����õ����ݿ�
	m_database.setDatabaseName("simulatedtraining");
	// �������µ���һ��.open()
	if (m_database.open() == false)
	{
		QMessageBox::information(this, "��ʾ", "�����������ݿ�ʧ�ܣ�", QMessageBox::Ok);
		return false;
	}

	//ExcuteSqlFile();
}

int DeviceManagerLogin::ExcuteSqlFile()
{
	int iRet = 0;
	//ִ�д����ݿ⣬��Լ��Ҫ������
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
			QMessageBox::information(this, "��ʾ", "sql�ļ����г���", QMessageBox::Ok);
			iRet = -1;
			break;
		}
	}

	QMessageBox::information(this, "��ʾ", "sql�ļ�������ϣ�", QMessageBox::Ok);
	return iRet;
}