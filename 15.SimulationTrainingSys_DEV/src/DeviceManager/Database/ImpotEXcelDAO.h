#pragma once
#ifndef IMPORTEXCELDAO_H
#define IMPORTEXCELDAO_H


#include <string>
#include <unordered_map>
using namespace std;

#include "../AppCache.h"
//#include "../../Public/log.h"
#include "../../Public/Utils.h" 
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
		~PCTable() {};
		int m_id;
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
	};

    //ע�⡷�����У���ʾ��ǰ���������
	class ImpotEXcelDAO
	{
	public:
		ImpotEXcelDAO(OutputPath* path);
		~ImpotEXcelDAO();

		bool connected();
		bool connect();

		bool GetRocketType();//��ȡȫ������ͺ�

		bool GetAllParamInfo();//��ȡ���в���
		bool GetCommandInfo();//��ȡ����ָ��
		bool GetAllParamtable();//��ȡ���в�����
		bool GetAllCommandtable();//��ȡ����ָ���

		bool ParamtableIsExist(string pTableName);//�жϵ�ǰ�������Ƿ���ڣ���������д��
		bool InsertNewParam(ParamInfo* oneParamInfo);//д���µĲ���
		bool InsertParamParamtable(ParamInfo* oneParamInfo);//д������Ͳ�����Ӧ��ϵ��

		bool CommandtableIsExist(string cTableName);//�жϵ�ǰָ����Ƿ���ڣ���������д��

		bool InsertNewCommand(Command* oneCommand);//д���µ�ָ�� 
		bool InsertNewCommandBack(Command* oneCommandBack);//д�뵱ǰ��ָ��Ļ��� (��Ҫ��д����)

		bool InsertCommandCommandtable(Command* oneCommand);//д��ָ���ָ���Ӧ��ϵ��

		inline void SetImportRocketID(string name)
		{ 
			for (auto item : m_allRocketTypeI)
			{
				if (item.second->m_name == name)
				{
					m_importRocketName = name;
					m_importRocketId = item.second->m_id;
					return;
				}
			}
		}

	private:
		OutputPath* m_path;
		MYSQL my_connection;
		bool is_connected;
		AppCache* m_app;

		map<int, RocketType*> m_allRocketTypeI;//ȫ���Ļ���ͺ�
		string m_importRocketName;//��Ҫ����Ļ���ͺ�����
		int m_importRocketId;//��Ҫ����Ļ���ͺ�ID

		map<int, Command*> m_allCommadI;//����ָ��
		map<int, ParamInfo*> m_allParamI;//���в���

		map<int, PCTable*> m_allCommandtableI;//����ָ���
		map<int, PCTable*> m_allParamtableI;//���в�����


	};
}


#endif