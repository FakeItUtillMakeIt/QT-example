#pragma execution_character_set("utf-8")
#pragma once

#include <QMutex>
#include <QMessageBox>
#include <QtWidgets/QApplication>
#include "Software.h" // Software.h
#include <string>
#include <map> 
#include <vector>
#include <QCryptographicHash>
#include "../Model/Fault.h"
#include "glog/logging.h"
#include "../Public/tinyxml2.h" 
#include "Database/FaultFrameInfo.h"
#include "../Model/Command.h"

using namespace std;
using namespace DataBase;
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
	PeerInfo* m_cmdSender; //�����������
	vector<Fault*> m_allFault;
	int* argc;
	char*** argv;
	int m_status;//ϵͳ״̬��0��ʾ���У�1��ʾ���ݻط��У�2��ʾ���ݴ����У�3��ʾʵʱ������
	bool CheckStatus(bool isOpen);
	void SetStatus(int status);
	QString GetStatus();

	map<int, Command*> m_allFaultCommad;//�ָⷢ��(�������ݽ��պͷ��ͽ���)

	int m_rockedType;//����ͺ�

	map<int, FaultRocketInfo*> m_RocketInfoframes;//rocked_info����id�Ͷ�Ӧ��
	map<int, FaultCommandInfo*> m_CommandInfoframes;//command_info����id�Ͷ�Ӧ��
	map<int, FaultCommandParamInfo*> m_CommandParamInfoframes;//command_param_info����id�Ͷ�Ӧ��
	
	map<int, FaultFaultParamInfo*> m_FaultParamInfoFrames;//fault_param_info����id�Ͷ�Ӧ��
	map<int, FaultFaultCommandInfo*> m_FaultCommandInfoFrames;//fault_command_info����id�Ͷ�Ӧ��

	map<int, FaultParameterInfo*> m_FaultParameterInfoFrames;//parameter_info����id�Ͷ�Ӧ��
	map<int, FaultDeviceInfo*> m_FaultDeviceInfoFrames;//device_info����id�Ͷ�Ӧ��
	map<int, FaultDeviceParamInfo*> m_FaultDeviceParamInfoFrames;//device_param_info����id�Ͷ�Ӧ��
	map<int, vector<int>> m_DeviceIDParamID;//�豸id�Ͳ���id��Ӧ��


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
	bool GetCmdSender(); 
};
