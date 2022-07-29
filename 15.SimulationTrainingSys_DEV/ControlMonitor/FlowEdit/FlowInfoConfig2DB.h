#pragma once

#ifndef FLOW_INFO_CONFIG2DB_H
#define FLOW_INFO_CONFIG2DB_H


#include <string>
#include <vector>
#include <unordered_map>

// ------------------------------------- !< 引入数据库操作头------------------------------------
#include "../Database/UserDAO.h"
#include <QDebug>

using namespace std;

/**

	@class   FlowInfoConfig2DB
	@brief   流程信息数据库读写操作
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


	//!< 	  两个信息表 主流程和子流程
	unordered_map<int, vector<string>> mainFlowInfo;
	unordered_map<int, vector<string>> subFlowInfo;
	//!< 	  关联命令和火箭信息
	unordered_map<int, vector<string>> commandInfo;
	unordered_map<int, vector<string>> rocketInfo;
	//!< 	  联合查询结果
	unordered_map<int, vector<int> > unionSearchInfo;

public:
	void readCommandDB2FlowEdit();
	void readRocketDB2FlowEdit();
	void readMainFlowDB2FlowEdit();
	void readSubFlowDB2FlowEdit();


	//@brief 联合查询函数
	void readUnionSearchDB2FlowEdit(QString RocketID);

	void mainFlowConfigOp2DB(int rocketTypeID, QString mainFlowName, QString mainFlowIndex, QString backCmdInfo, QString otherInfo);
	void subFlowConfigOp2DB(int mainFlowID, int emitCmdID, QString subFlowName, QString otherInfo);

	//删除数据库
	void clearMainFlowDB();
	void clearSubFlowDB();

private:
	DataBase::UserDAO* flowInfoDBOp;
};


#endif