#include "AppCache.h"
#include <QFileInfo>
#include <QDir> 
#include <QDesktopWidget>
#include <boost/algorithm/string.hpp>
#include <boost/xpressive/xpressive_dynamic.hpp>
#include <QDateTime>
AppCache* AppCache::m_instance = nullptr;
QMutex AppCache::m;
QMutex AppCache::mData;

AppCache::AppCache()
{ 
	m_soft = nullptr; 
	m_CurrentRocketType = nullptr;
	m_CurrentRocketDataFrame = nullptr;
}

AppCache::~AppCache()
{

}
/// <summary>
///  ����
/// </summary>
/// <returns></returns>
AppCache* AppCache::instance()
{
	if (m_instance == nullptr)
	{
		QMutexLocker locker(&m);
		if (m_instance == nullptr)
			m_instance = new AppCache();
	}
	return m_instance;
}

/// <summary>
/// ����·��
/// </summary>
/// <param name="exePath"></param>
/// <returns></returns>
bool AppCache::SetExePath(QString exePath) {
	QDir currPath = exePath;
	m_exePath = currPath.absolutePath();
	//�жϲ�����־�ļ���
	m_logdir = exePath + "/log";
	QDir dir(m_logdir);
	if (!dir.exists()) {
		//ֻ����һ����Ŀ¼�������뱣֤�ϼ�Ŀ¼����
		if (!dir.mkdir(m_logdir)) {
			return false;
		}
	}
	return true;
}

/// <summary>
/// ������־
/// </summary>
/// <param name="days"></param>
/// <param name="isShow"></param>
/// <returns></returns>
bool AppCache::clearLogs(int days, bool isShow)
{
	QDir dir(exePath() + "/log");
	if (!dir.exists())
	{
		QMessageBox::warning(qApp->activeWindow(), QObject::tr("����"), "�Ҳ�����־���Ŀ¼��");
		return false;
	}
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	QFileInfoList fileList = dir.entryInfoList();

	int count = fileList.count();
	if (count <= 0) {
		if (isShow) QMessageBox::warning(qApp->activeWindow(), QObject::tr("����"), "û�д��������־�ļ���");
		return false;
	}

	for each (auto item in fileList)
	{
		QString absFilePath = item.absoluteFilePath();
		QFileInfo fileInfo(absFilePath);
		if (fileInfo.lastModified().addDays(days).toTime_t() < QDateTime::currentDateTime().toTime_t())
		{
			QFile::remove(absFilePath);
		}
	}
	LOG(INFO) << "�����������" << days << "�����־��";
	if (isShow) QMessageBox::warning(qApp->activeWindow(), QObject::tr("����"), QString("���������%1�����־��").arg(days));
	return true;
}
/// <summary>
/// ���ϵͳ״̬
/// </summary>
/// <param name="isOpen">�Ƿ����ʾ��</param>
/// <returns>�Ƿ����</returns>
bool AppCache::CheckStatus(bool isOpen)
{
	switch (m_status)
	{
	case 1:
	{
		if (isOpen) QMessageBox::warning(qApp->activeWindow(), QObject::tr("����"), "���ݻط��У�");
		return false;
	}
	case 2:
	{
		if (isOpen) QMessageBox::warning(qApp->activeWindow(), QObject::tr("����"), "���ݴ����У�");
		return false;
	}
	case 3:
	{
		if (isOpen) QMessageBox::warning(qApp->activeWindow(), QObject::tr("����"), "���������У�");
		return false;
	}
	}
	return true;
}

/// <summary>
/// ����״̬
/// </summary>
/// <param name="status"></param>
void AppCache::SetStatus(int status)
{
	m_status = status;
}
/// <summary>
/// ����״̬��0��ʾ���У�1��ʾ���ݻط��У�2��ʾ���ݴ����У�3��ʾʵʱ�����У�
/// </summary>
/// <returns></returns>
QString AppCache::GetStatus()
{
	switch (m_status)
	{
	case 0:
		return "����";
	case 1:
		return "���ݻط���...";
	case 2:
		return "�º�����...";
	case 3:
		return "����������...";
	defalut:
		return "����";
	}
	return "�쳣";
}

/// <summary>
/// �������ļ�
/// </summary>
/// <returns></returns>
bool AppCache::OpenConfig() {
	//�������ļ�
	m_xmlPath = m_exePath + "/config/config.xml";
	int error = doc.LoadFile(m_xmlPath.toStdString().c_str());
	if (error) {
		LOG(ERROR) << "�������ļ���" << m_xmlPath.toStdString() << "��ʱ����,��ȷ�������ļ�·���Ƿ���ȷ";
		return false;
	}
	LOG(INFO) << "�������ļ���" << m_xmlPath.toStdString() << "��";
	return true;
}

/// <summary>
/// ��ȡ�����ļ�
/// </summary>
/// <returns></returns>
bool AppCache::ReadConfig()
{
	//�������ļ�
	if (!OpenConfig()) {
		return false;
	}

	if (!GetBaseInfo()) {
		LOG(INFO) << "��ȡ������Ϣ�������ʱ����,��ȷ�������ļ������Ƿ���ȷ";
		return false;
	}
	if (!GetNetworkPeer()) {
		LOG(INFO) << "��ȡ�����������������ʱ����,��ȷ��CmdSender�����ļ������Ƿ���ȷ";
		return false;
	}
	if (!GetOutputPath()) {
		LOG(INFO) << "��ȡ���ݿ�����storage�ڵ��������ʱ����,��ȷ�������ļ������Ƿ���ȷ";
		return false;
	}
	if (!GetLogConfig()) {
		LOG(INFO) << "��ȡ���ݿ�����log�ڵ��������ʱ����,��ȷ�������ļ������Ƿ���ȷ";
		return false;
	}
	LOG(INFO) << "�ɹ���ȡ�����ļ�";
	return true;
}

/// <summary>
/// ��ȡ��־����
/// </summary>
/// <returns></returns>
bool AppCache::GetLogConfig()
{
	tinyxml2::XMLElement* publishElement = doc.RootElement()->FirstChildElement("software")
		->FirstChildElement("log");
	m_logInfo = new LogInfo();
	m_logInfo->level = publishElement->IntAttribute("level");
	m_logInfo->days = publishElement->IntAttribute("days");
	m_logInfo->auto_clear = publishElement->IntAttribute("auto_clear");

	//�����־
	if (m_logInfo->auto_clear == 1)
	{
		clearLogs(m_logInfo->days);
	}
	return true;
}

/// <summary>
/// �����ڵ�cmdReceiver
/// </summary>
/// <returns></returns>
bool AppCache::GetNetworkPeer()
{
	//���ָ����ն˿�
	tinyxml2::XMLElement* publishElement = doc.RootElement()->FirstChildElement("software")
		->FirstChildElement("cmdReceiver");
	m_cmdReceiver = new PeerInfo();
	m_cmdReceiver->m_strNetworkType = publishElement->Attribute("network_type");
	m_cmdReceiver->m_strIP = publishElement->Attribute("ServerIP");
	m_cmdReceiver->m_iPort = publishElement->IntAttribute("port");

	//��ػ���Ͷ˿�
	publishElement = doc.RootElement()->FirstChildElement("software")
		->FirstChildElement("responseSender");
	m_responseSender = new PeerInfo();
	m_responseSender->m_strNetworkType = publishElement->Attribute("network_type");
	m_responseSender->m_strIP = publishElement->Attribute("ServerIP");
	m_responseSender->m_iPort = publishElement->IntAttribute("port");

	//�������ݷ��Ͷ˿�
	publishElement = doc.RootElement()->FirstChildElement("software")
		->FirstChildElement("yaoCeSender");
	m_yaoCeSender = new PeerInfo();
	m_yaoCeSender->m_strNetworkType = publishElement->Attribute("network_type");
	m_yaoCeSender->m_strIP = publishElement->Attribute("ServerIP");
	m_yaoCeSender->m_iPort = publishElement->IntAttribute("port");
	return true;
}

/// <summary>
/// �����ڵ�software
/// </summary>
/// <returns></returns>
bool AppCache::GetBaseInfo()
{
	tinyxml2::XMLElement* softwareElement = doc.RootElement()->FirstChildElement("software");
	m_soft = new Software();
	m_soft->setCode(QString::fromStdString(softwareElement->Attribute("code")));
	m_soft->setName(QString::fromStdString(softwareElement->Attribute("name")));
	m_soft->setVertionNode(QString::fromStdString(softwareElement->Attribute("vertion_code")));
	m_soft->setLogoIcon(QString::fromStdString(softwareElement->Attribute("icon")));
	m_soft->setLogLevel(softwareElement->FirstChildElement("log")->IntAttribute("level"));
	m_soft->setBaseSizeInfo(softwareElement->IntAttribute("width"), softwareElement->IntAttribute("height"));

	QDesktopWidget* _desk = QApplication::desktop();
	QRect deskRect = _desk->availableGeometry();
	m_soft->setFullScreenSizeInfo(deskRect.width(), deskRect.height());
	return true;
}

/// <summary>
/// �����ڵ�OutputPath
/// </summary>
/// <returns></returns>
bool AppCache::GetOutputPath()
{
	tinyxml2::XMLElement* outputElement = doc.RootElement()->FirstChildElement("software")
		->FirstChildElement("storage");
	m_outputPath = new OutputPath();
	m_outputPath->m_strType = outputElement->Attribute("type");
	m_outputPath->m_server = outputElement->Attribute("server");
	m_outputPath->m_db_name = outputElement->Attribute("db_name");
	m_outputPath->m_port = outputElement->IntAttribute("port");
	m_outputPath->m_user_name = outputElement->Attribute("user_name");
	m_outputPath->m_password = outputElement->Attribute("password");

	return true;
}

/**
	@brief �����豸��Ϣ�����豸��Ϣ����ʱ��Ҫ���øú����ĵط�ʹ���Ը���ϵͳ�������豸��Ϣ
**/
void AppCache::UpdateSysDeviceInfo() {

	//�������ݿ��豸�������豸�������豸

}