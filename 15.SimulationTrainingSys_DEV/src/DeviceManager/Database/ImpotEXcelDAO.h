#pragma once
#ifndef IMPORTEXCELDAO_H
#define IMPORTEXCELDAO_H
//#pragma execution_character_set("utf-8")

#include <string>
#include <unordered_map>
using namespace std;

#include "../AppCache.h"
//#include "../../Public/log.h"
//#include "../../Public/Utils.h" 
//#include "../../Model/Device.h"
//#include "../../Model/DeviceParam.h"
//#include "../../Model/DeviceStatus.h"
//#include "../../Model/RocketType.h"
//#include "../../Model/TaskManageInfo.h"
#include "mysql.h" 

namespace DataBase
{
	class PCTable
	{
	public:
		PCTable() {};
		PCTable(int id, int rid, string name) { m_id = id; m_roketID = rid; m_tableName = name; };
		~PCTable() {};
		int m_id;
		int m_roketID;
		string m_tableName;// ����
	};


	class ParamInfo
	{
	public:
		ParamInfo() {};
		~ParamInfo() {};
		int m_id;
		string m_name;// ����
		int m_type;
		string m_unit;
		int m_rocketid;
		int m_paramtableid;
		int m_deviceid;
	};

	class DeviceInfo
	{
	public:
		DeviceInfo() {};
		~DeviceInfo() {};
		int m_id;
		int m_rocketid;
		string m_name;// ����
		int m_isVirtual;
	};

	//ע�⡷�����У���ʾ��ǰ���������
	class ImpotEXcelDAO
	{

	public:
		ImpotEXcelDAO(OutputPath* path);
		~ImpotEXcelDAO();

		bool connected();
		bool connect();

		void Init(string name);

		bool GetRocketType();//��ȡȫ������ͺ�
		//bool GetWholeParamInfo();//��ȡȫ������

		bool GetAllParamInfo();//��ȡ��ǰ������в���
		bool GetCommandInfo();//��ȡ��ǰ�������ָ��
		bool GetAllParamtable();//��ȡ��ǰ������в�����
		bool GetAllCommandtable();//��ȡ��ǰ�������ָ���
		bool GetAllDeviceInfo();//��ȡ��ǰ��������豸��Ϣ

		int ParamtableIsExist(PCTable pTable);//�жϵ�ǰ�������Ƿ���ڣ���������д��
		//bool NowParamExist(ParamInfo* oneParamInfo,int& ParamId);//�ж��������������Ƿ���ڸò�������������д��
		int NowRocketParamExist(ParamInfo* oneParamInfo, bool& isExit);//�жϵ�ǰ�����Ƿ����,��������д���µĲ���
		bool InsertParamParamtable(ParamInfo* oneParamInfo);//д������Ͳ�����Ӧ��ϵ��

		int CommandtableIsExist(PCTable cTable);//�жϵ�ǰָ����Ƿ���ڣ���������д��
		int NowCommandExist(Command* oneCommand);//�жϵ�ǰ�����Ƿ����,��������д���µ�ָ�� 
		int NowCommandBackExist(Command* oneCommandBack, bool& isExit);//д�뵱ǰ��ָ��Ļ��� (��Ҫ��д����)
		bool InsertCommandCommandtable(Command* oneCommand);//д��ָ���ָ���Ӧ��ϵ��

		int DeviceIsExist(DeviceInfo* oneDevice);//�жϵ�ǰ�豸�Ƿ���ڣ���������д��
		bool InsertParamDevice(ParamInfo* oneParamInfo);//д���豸�Ͳ�����Ӧ��ϵ��


		inline int GetRocketID()
		{
			return m_importRocketId;
		}

	private:
		OutputPath* m_path;
		MYSQL my_connection;
		bool is_connected;
		AppCache* m_app;

	public:

		map<int, RocketType*> m_allRocketTypeI;//ȫ���Ļ���ͺ�
		string m_importRocketName;//��Ҫ����Ļ���ͺ�����
		int m_importRocketId;//��Ҫ����Ļ���ͺ�ID

		//map<int, ParamInfo*> m_wholeParamI;//ȫ������

		map<int, Command*> m_allCommadI;//��ǰ�������ָ��
		map<int, ParamInfo*> m_allParamI;//��ǰ������в���

		map<int, PCTable*> m_allCommandtableI;//��ǰ�������ָ���
		map<int, PCTable*> m_allParamtableI;//��ǰ������в�����

		map<int, DeviceInfo*> m_allDeviceI;//��ǰ������в���
	};
}


#endif