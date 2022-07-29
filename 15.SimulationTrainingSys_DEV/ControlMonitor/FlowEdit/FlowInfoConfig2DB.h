#pragma once

#ifndef FLOW_INFO_CONFIG2DB_H
#define FLOW_INFO_CONFIG2DB_H


#include <string>
#include <vector>
#include <unordered_map>

// ------------------------------------- !< �������ݿ����ͷ------------------------------------
#include "../Database/UserDAO.h"
#include <QDebug>

using namespace std;

/**

	@class   FlowInfoConfig2DB
	@brief   ������Ϣ���ݿ��д����
	@details ~

**/
class FlowInfoConfig2DB
{
private:
	FlowInfoConfig2DB();
	~FlowInfoConfig2DB() {

	}

public:
	static FlowInfoConfig2DB* instance;
	static FlowInfoConfig2DB* getInstance() {
		if (instance == nullptr)
		{
			instance = new FlowInfoConfig2DB;
		}
		return instance;
	}

	static void closeInstance() {
		if (instance)
		{
			instance->~FlowInfoConfig2DB();
		}
	}


	//!< 	  ������Ϣ�� �����̺�������
	unordered_map<int, vector<string>> mainFlowInfo;
	unordered_map<int, vector<string>> subFlowInfo;
	//!< 	  ��������ͻ����Ϣ
	unordered_map<int, vector<string>> commandInfo;
	unordered_map<int, vector<string>> rocketInfo;
	//!< 	  ���ϲ�ѯ���
	unordered_map<int, vector<int> > unionSearchInfo;

public:
	void readCommandDB2FlowEdit();
	void readRocketDB2FlowEdit();
	void readMainFlowDB2FlowEdit();
	void readSubFlowDB2FlowEdit();


	//@brief ���ϲ�ѯ����
	void readUnionSearchDB2FlowEdit(QString RocketID);

	void mainFlowConfigOp2DB(int rocketTypeID, QString mainFlowName, QString mainFlowIndex, QString backCmdInfo, QString otherInfo);
	void subFlowConfigOp2DB(int mainFlowID, int emitCmdID, QString subFlowName, QString otherInfo);

	//ɾ�����ݿ�
	void clearMainFlowDB();
	void clearSubFlowDB();

private:
	DataBase::UserDAO* flowInfoDBOp;
};


#endif