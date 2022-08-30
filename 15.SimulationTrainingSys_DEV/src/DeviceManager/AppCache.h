
#pragma once
#pragma execution_character_set("utf-8")

#ifndef APP_CACHE_H
#define APP_CACHE_H



#include <QMutex>
#include <QMessageBox>
#include <QtWidgets/QApplication>
#include "Software.h" // Software.h
#include <string>
#include <map> 
#include <vector>
#include <QCryptographicHash>
using namespace std;
#include "../Model/Device.h"
#include "../Model/DeviceParam.h"
#include "../Model/DeviceStatus.h"
#include "../Model/Status.h"
#include "../Model/Command.h"
#include "../Model/RocketType.h"
#include "../Protocol/RocketDataFrame.h"
#include "glog/logging.h"
#include "../Public/tinyxml2.h" 



#include <QLabel>
#include "../Model/Fault.h"
#include "Database/FaultFrameInfo.h"
using namespace DataBaseF;

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
	OutputPath* m_outputPath;

	PeerInfo* m_cmdReceiver; //测控指令接收端口
	PeerInfo* m_responseSender; //测控回令发送端口
	PeerInfo* m_yaoCeSender; //箭上数据发送端口


	vector<Device*> m_allDevice;//软件的设备信息
	map<int, Status*> m_allStatus;
	map<int, Device*> m_allDeviceCopy;
	map<int, Command*> m_allCommad;//测发指令

	map<int, DeviceParam*> m_allDeviceParam;//设备参数
	map<int, DeviceStatus*> m_allDeviceStatus;//设备状态
	map<int, vector<int>> m_dev2DeviceParamID;//设备ID和设备参数的对应关系
	map<int, vector<int>> m_dev2DeviceStatusID;//设备ID和设备状态的对应关系
	map<int, RocketType*> m_allRocketType;//火箭型号
	RocketType* m_CurrentRocketType;//当前火箭型号 
	map<int, RocketDataFrame*> m_RocketDataFrame;
	RocketDataFrame* m_CurrentRocketDataFrame;//当前火箭数据协议帧

	int* argc;
	char*** argv;
	int m_status;//系统状态：0表示空闲，1表示数据回放中，2表示数据处理中，3表示实时试验中
	bool CheckStatus(bool isOpen);
	void SetStatus(int status);
	QString GetStatus();

	void UpdateSysDeviceInfo();

	/// <summary>
	/// 故障注入添加的
	/// </summary>
	PeerInfo* m_cmdSender; //命令发送
	PeerInfo* m_paramSender;//参数发送
	PeerInfo* m_paramResponseReceiver;//参数故障回令接收
	PeerInfo* m_cmdResponseReceiver;//指令故障回令接收
	vector<Fault*> m_allFault;

	map<int, Command*> m_allFaultCommad;//测发指令(用于数据接收和发送解析)

	int m_rockedType;//火箭型号

	map<int, FaultRocketInfo*> m_RocketInfoframes;//rocked_info表格的id和对应项
	map<int, FaultCommandInfo*> m_CommandInfoframes;//command_info表格的code和对应项
	map<int, FaultCommandParamInfo*> m_CommandParamInfoframes;//command_param_info表格的id和对应项

	map<int, FaultFaultParamInfo*> m_FaultParamInfoFrames;//fault_param_info表格的id和对应项
	map<int, FaultFaultCommandInfo*> m_FaultCommandInfoFrames;//fault_command_info表格的id和对应项

	map<int, FaultParameterInfo*> m_FaultParameterInfoFrames;//parameter_info表格的id和对应项 (只有对应火箭型号的数据)
	map<int, FaultDeviceInfo*> m_FaultDeviceInfoFrames;//device_info表格的id和对应项
	map<int, FaultDeviceParamInfo*> m_FaultDeviceParamInfoFrames;//device_param_info表格的id和对应项
	map<int, vector<int>> m_DeviceIDParamID;//设备id和参数id对应项
	//map<int, DeviceParam*> m_allDeviceParam;//设备参数
	//map<int, RocketType*> m_allRocketType;//火箭型号
	//RocketType* m_CurrentRocketType;//当前火箭型号
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


	//bool OpenConfig();
	//bool GetLogConfig();
	//bool GetBaseInfo();
	//bool GetOutputPath();
	bool GetCmdSender();
};


#endif