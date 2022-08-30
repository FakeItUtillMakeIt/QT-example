
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
* @brief Ӧ�ó����ȫ�ֻ����� \n
* �����ࡣ���������ļ��е�����������Ϣ
*/
class AppCache
{
public:
	~AppCache();

	/**
	* @brief ����AppCache��Ψһʵ��ָ��
	* @return ����AppCache��Ψһʵ��ָ��
	*/
	static AppCache* instance();

	bool SetExePath(QString exePath);
	inline QString exePath() { return m_exePath; }
	bool clearLogs(int days, bool isShow = false);
#pragma region �����ļ�����
	/**
	* @brief ��־��¼����
	*/
	int m_log_level;

	/**
	* @brief ������־���ݵ�Ŀ¼
	*/
	QString m_logdir;


	/**
	* @brief ��ȡ�����ļ�
	* @return ��ȡ�ɹ����
	*        -<em>false</em> ��ȡʧ��
	*        -<em>true</em> ��ȡ�ɹ�
	*/
	bool AppCache::ReadConfig();
	QString m_xmlPath;
	static QMutex mData;

	Software* m_soft;
	LogInfo* m_logInfo;
	OutputPath* m_outputPath;

	PeerInfo* m_cmdReceiver; //���ָ����ն˿�
	PeerInfo* m_responseSender; //��ػ���Ͷ˿�
	PeerInfo* m_yaoCeSender; //�������ݷ��Ͷ˿�


	vector<Device*> m_allDevice;//������豸��Ϣ
	map<int, Status*> m_allStatus;
	map<int, Device*> m_allDeviceCopy;
	map<int, Command*> m_allCommad;//�ָⷢ��

	map<int, DeviceParam*> m_allDeviceParam;//�豸����
	map<int, DeviceStatus*> m_allDeviceStatus;//�豸״̬
	map<int, vector<int>> m_dev2DeviceParamID;//�豸ID���豸�����Ķ�Ӧ��ϵ
	map<int, vector<int>> m_dev2DeviceStatusID;//�豸ID���豸״̬�Ķ�Ӧ��ϵ
	map<int, RocketType*> m_allRocketType;//����ͺ�
	RocketType* m_CurrentRocketType;//��ǰ����ͺ� 
	map<int, RocketDataFrame*> m_RocketDataFrame;
	RocketDataFrame* m_CurrentRocketDataFrame;//��ǰ�������Э��֡

	int* argc;
	char*** argv;
	int m_status;//ϵͳ״̬��0��ʾ���У�1��ʾ���ݻط��У�2��ʾ���ݴ����У�3��ʾʵʱ������
	bool CheckStatus(bool isOpen);
	void SetStatus(int status);
	QString GetStatus();

	void UpdateSysDeviceInfo();

	/// <summary>
	/// ����ע����ӵ�
	/// </summary>
	PeerInfo* m_cmdSender; //�����
	PeerInfo* m_paramSender;//��������
	PeerInfo* m_paramResponseReceiver;//�������ϻ������
	PeerInfo* m_cmdResponseReceiver;//ָ����ϻ������
	vector<Fault*> m_allFault;

	map<int, Command*> m_allFaultCommad;//�ָⷢ��(�������ݽ��պͷ��ͽ���)

	int m_rockedType;//����ͺ�

	map<int, FaultRocketInfo*> m_RocketInfoframes;//rocked_info����id�Ͷ�Ӧ��
	map<int, FaultCommandInfo*> m_CommandInfoframes;//command_info����code�Ͷ�Ӧ��
	map<int, FaultCommandParamInfo*> m_CommandParamInfoframes;//command_param_info����id�Ͷ�Ӧ��

	map<int, FaultFaultParamInfo*> m_FaultParamInfoFrames;//fault_param_info����id�Ͷ�Ӧ��
	map<int, FaultFaultCommandInfo*> m_FaultCommandInfoFrames;//fault_command_info����id�Ͷ�Ӧ��

	map<int, FaultParameterInfo*> m_FaultParameterInfoFrames;//parameter_info����id�Ͷ�Ӧ�� (ֻ�ж�Ӧ����ͺŵ�����)
	map<int, FaultDeviceInfo*> m_FaultDeviceInfoFrames;//device_info����id�Ͷ�Ӧ��
	map<int, FaultDeviceParamInfo*> m_FaultDeviceParamInfoFrames;//device_param_info����id�Ͷ�Ӧ��
	map<int, vector<int>> m_DeviceIDParamID;//�豸id�Ͳ���id��Ӧ��
	//map<int, DeviceParam*> m_allDeviceParam;//�豸����
	//map<int, RocketType*> m_allRocketType;//����ͺ�
	//RocketType* m_CurrentRocketType;//��ǰ����ͺ�
#pragma endregion

private:
	AppCache();
	static QMutex m;
	/**
	* @brief AppCache��Ψһʵ��
	*/
	static AppCache* m_instance;

	/**
	* @brief ��ִ�г�������·��
	*/
	QString m_exePath;

	/**
	* @brief �������ļ�config.xml��XMLDocument����
	*/
	tinyxml2::XMLDocument doc;

	/**
	* @brief ���������ļ�config.xml
	* @return �򿪳ɹ����
	*        -<em>false</em> ��ʧ��
	*        -<em>true</em> �򿪳ɹ�
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