
#ifndef MUTLI_THREAD_OP_H
#define MUTLI_THREAD_OP_H

#pragma once

#include <QtWidgets>
#include <QObject>
#include <QRunnable>
#include <QThread>
#include <Queue>
#include <QMutex>
#include <QThreadPool>

#include "../AppCache.h"
#include "../Public/Utils.h"
#include "../Database/DeviceDAO.h"
#include "../Database/DeviceDBConfigInfo.h"


class WorkThread :public QObject
{
	Q_OBJECT
public:
	WorkThread(QObject* parent = nullptr);
	~WorkThread();

public slots:
	void doWork();

signals:
	void workFinished();
private:

};

class DeleteDBDataThread :public QObject
{
	Q_OBJECT
public:
	DeleteDBDataThread(QObject* parent = nullptr);
	~DeleteDBDataThread();

	void setRocketID(int rocketID);
	void setDeviceID(int deviceID);
	void setCmdID(int CmdID);
	void setParamID(int ParamID);
public slots:
	void deleteRocketInfo();
	void deleteDeviceInfo();
	void deleteCmdInfo();
	void deleteParamInfo();
signals:
	void workFinished();
protected:
private:
	int curRocketID;
	int curCmdID;
	int curDeviceID;
	int curParamID;
	unordered_map<int, vector<string>> customReadInfoMap;

	DataBase::DeviceDAO* deviceManageDBOp;

	void customRunSql(QString qSqlString);
	void customReadTableInfo(QString qSqlString);

};


class DeleteDBDataThread1 :public QThread
{
	Q_OBJECT
public:
	void setRocketID(int rocketID);
	void setDeviceID(int deviceID);
	void setCmdID(int CmdID);
	void setParamID(int ParamID);
protected:
	void run();
private:
	int curRocketID;
};

#endif