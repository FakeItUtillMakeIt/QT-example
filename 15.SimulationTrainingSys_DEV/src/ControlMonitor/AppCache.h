#pragma execution_character_set("utf-8")
#pragma once

#include <QMutex>
#include <QMessageBox>
#include <QtWidgets/QApplication>
#include "Software.h" // Software.h
#include <QLabel>
#include<QString>
#include<QList>
#include <string>
#include <map> 
#include <vector>
#include <QCryptographicHash>
using namespace std;
#include "../Model/User.h"
#include "../Model/Device.h"
#include "../Model/DeviceParam.h"
#include "../Model/Command.h"
#include "glog/logging.h"
#include "../Public/tinyxml2.h" 
#include "../Model/RocketType.h"
/**
* @brief 应用程序的全局缓冲区 \n
* 单例类。保存配置文件中的所有数据信息
*/
class AppCache
{
public:
	~AppCache();
	
	/**
	* @brief 返回AppCache的唯一实例指针
	* @return 返回AppCache的唯一实例指针
	*/
	static AppCache* instance();
 
	bool SetExePath(QString exePath);
	inline QString exePath() { return m_exePath; }
	bool clearLogs(int days, bool isShow = false);
	#pragma region 配置文件缓存
	/**
	* @brief 日志记录级别
	*/
	int m_log_level;

	/**
	* @brief 保存日志数据的目录
	*/
	QString m_logdir;
	 

	/**
	* @brief 读取配置文件
	* @return 读取成功与否
	*        -<em>false</em> 读取失败
	*        -<em>true</em> 读取成功
	*/
	bool AppCache::ReadConfig();
	QString m_xmlPath;
	static QMutex mData;
	Software* m_soft;
	LogInfo* m_logInfo; 
	QLabel* rokecttype;
	OutputPath* m_outputPath;
	QList<QString> mainflowlist;
	PeerInfo* m_cmdSender; //处理命令接收
	PeerInfo* m_responseReceiver; //测控回令接收端口
	PeerInfo* m_yaoCeReceiver; //箭上数据接收端口
	vector<User*> m_allUser;
	vector<Device*> m_allDevice;
	map<int, Command*> m_allCommad;//测发指令
	map<int, DeviceParam*> m_allDeviceParam;//设备参数
	map<int, RocketType*> m_allRocketType;//火箭型号
	RocketType* m_CurrentRocketType;//当前火箭型号
	int* argc;
	char*** argv;
	int m_status;//系统状态：0表示空闲，1表示数据回放中，2表示数据处理中，3表示实时试验中
	bool CheckStatus(bool isOpen);
	void SetStatus(int status);
	QString GetStatus();
#pragma endregion


private:
	AppCache();
	static QMutex m;
	/**
	* @brief AppCache的唯一实例
	*/
	static AppCache* m_instance;
  
	/**
	* @brief 可执行程序所在路径
	*/
	QString m_exePath;

	/**
	* @brief 主配置文件config.xml的XMLDocument对象
	*/
	tinyxml2::XMLDocument doc;
  
	/**
	* @brief 打开主配置文件config.xml
	* @return 打开成功与否
	*        -<em>false</em> 打开失败
	*        -<em>true</em> 打开成功
	*/
	bool OpenConfig(); 
	bool GetLogConfig();
	bool GetBaseInfo(); 
	bool GetOutputPath();
	bool GetNetworkPeer();
};
