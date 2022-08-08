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
///  单例
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
/// 设置路径
/// </summary>
/// <param name="exePath"></param>
/// <returns></returns>
bool AppCache::SetExePath(QString exePath) {
	QDir currPath = exePath;
	m_exePath = currPath.absolutePath();
	//判断并打开日志文件夹
	m_logdir = exePath + "/log";
	QDir dir(m_logdir);
	if (!dir.exists()) {
		//只创建一级子目录，即必须保证上级目录存在
		if (!dir.mkdir(m_logdir)) {
			return false;
		}
	}
	return true;
}

/// <summary>
/// 清理日志
/// </summary>
/// <param name="days"></param>
/// <param name="isShow"></param>
/// <returns></returns>
bool AppCache::clearLogs(int days, bool isShow)
{
	QDir dir(exePath() + "/log");
	if (!dir.exists())
	{
		QMessageBox::warning(qApp->activeWindow(), QObject::tr("警告"), "找不到日志存放目录！");
		return false;
	}
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	QFileInfoList fileList = dir.entryInfoList();

	int count = fileList.count();
	if (count <= 0) {
		if (isShow) QMessageBox::warning(qApp->activeWindow(), QObject::tr("警告"), "没有待清除的日志文件！");
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
	LOG(INFO) << "已清除超过‘" << days << "天的日志！";
	if (isShow) QMessageBox::warning(qApp->activeWindow(), QObject::tr("警告"), QString("已清除超过%1天的日志！").arg(days));
	return true;
}
/// <summary>
/// 监测系统状态
/// </summary>
/// <param name="isOpen">是否打开提示窗</param>
/// <returns>是否空闲</returns>
bool AppCache::CheckStatus(bool isOpen)
{
	switch (m_status)
	{
	case 1:
	{
		if (isOpen) QMessageBox::warning(qApp->activeWindow(), QObject::tr("警告"), "数据回放中！");
		return false;
	}
	case 2:
	{
		if (isOpen) QMessageBox::warning(qApp->activeWindow(), QObject::tr("警告"), "数据处理中！");
		return false;
	}
	case 3:
	{
		if (isOpen) QMessageBox::warning(qApp->activeWindow(), QObject::tr("警告"), "正在试验中！");
		return false;
	}
	}
	return true;
}

/// <summary>
/// 设置状态
/// </summary>
/// <param name="status"></param>
void AppCache::SetStatus(int status)
{
	m_status = status;
}
/// <summary>
/// 返回状态（0表示空闲，1表示数据回放中，2表示数据处理中，3表示实时试验中）
/// </summary>
/// <returns></returns>
QString AppCache::GetStatus()
{
	switch (m_status)
	{
	case 0:
		return "空闲";
	case 1:
		return "数据回放中...";
	case 2:
		return "事后处理中...";
	case 3:
		return "正在试验中...";
	defalut:
		return "空闲";
	}
	return "异常";
}

/// <summary>
/// 打开配置文件
/// </summary>
/// <returns></returns>
bool AppCache::OpenConfig() {
	//打开配置文件
	m_xmlPath = m_exePath + "/config/config.xml";
	int error = doc.LoadFile(m_xmlPath.toStdString().c_str());
	if (error) {
		LOG(ERROR) << "打开配置文件‘" << m_xmlPath.toStdString() << "’时出错,请确认配置文件路径是否正确";
		return false;
	}
	LOG(INFO) << "打开配置文件‘" << m_xmlPath.toStdString() << "’";
	return true;
}

/// <summary>
/// 读取配置文件
/// </summary>
/// <returns></returns>
bool AppCache::ReadConfig()
{
	//打开配置文件
	if (!OpenConfig()) {
		return false;
	}

	if (!GetBaseInfo()) {
		LOG(INFO) << "读取基础信息相关内容时出错,请确认配置文件内容是否正确";
		return false;
	}
	if (!GetNetworkPeer()) {
		LOG(INFO) << "读取网络配置项相关内容时出错,请确认CmdSender配置文件内容是否正确";
		return false;
	}
	if (!GetOutputPath()) {
		LOG(INFO) << "读取数据库配置storage节点相关内容时出错,请确认配置文件内容是否正确";
		return false;
	}
	if (!GetLogConfig()) {
		LOG(INFO) << "读取数据库配置log节点相关内容时出错,请确认配置文件内容是否正确";
		return false;
	}
	LOG(INFO) << "成功读取配置文件";
	return true;
}

/// <summary>
/// 读取日志配置
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

	//清除日志
	if (m_logInfo->auto_clear == 1)
	{
		clearLogs(m_logInfo->days);
	}
	return true;
}

/// <summary>
/// 解析节点cmdReceiver
/// </summary>
/// <returns></returns>
bool AppCache::GetNetworkPeer()
{
	//测控指令接收端口
	tinyxml2::XMLElement* publishElement = doc.RootElement()->FirstChildElement("software")
		->FirstChildElement("cmdReceiver");
	m_cmdReceiver = new PeerInfo();
	m_cmdReceiver->m_strNetworkType = publishElement->Attribute("network_type");
	m_cmdReceiver->m_strIP = publishElement->Attribute("ServerIP");
	m_cmdReceiver->m_iPort = publishElement->IntAttribute("port");

	//测控回令发送端口
	publishElement = doc.RootElement()->FirstChildElement("software")
		->FirstChildElement("responseSender");
	m_responseSender = new PeerInfo();
	m_responseSender->m_strNetworkType = publishElement->Attribute("network_type");
	m_responseSender->m_strIP = publishElement->Attribute("ServerIP");
	m_responseSender->m_iPort = publishElement->IntAttribute("port");

	//箭上数据发送端口
	publishElement = doc.RootElement()->FirstChildElement("software")
		->FirstChildElement("yaoCeSender");
	m_yaoCeSender = new PeerInfo();
	m_yaoCeSender->m_strNetworkType = publishElement->Attribute("network_type");
	m_yaoCeSender->m_strIP = publishElement->Attribute("ServerIP");
	m_yaoCeSender->m_iPort = publishElement->IntAttribute("port");
	return true;
}

/// <summary>
/// 解析节点software
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
/// 解析节点OutputPath
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
	@brief 更新设备信息，在设备信息更新时需要调用该函数的地方使用以更新系统的最新设备信息
**/
void AppCache::UpdateSysDeviceInfo() {

	//访问数据库设备表，根据设备表生成设备

}