
#pragma once
#pragma execution_character_set("utf-8")


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
#include "glog/logging.h"
#include "../Public/tinyxml2.h" 
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
	vector<Device*> m_allDevice;
	int* argc;
	char*** argv;
	int m_status;//ϵͳ״̬��0��ʾ���У�1��ʾ���ݻط��У�2��ʾ���ݴ����У�3��ʾʵʱ������
	bool CheckStatus(bool isOpen);
	void SetStatus(int status);
	QString GetStatus();
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
};
