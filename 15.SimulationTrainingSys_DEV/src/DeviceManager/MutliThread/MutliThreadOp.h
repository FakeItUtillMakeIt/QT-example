
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
#include "../Database/DeviceDBConfigInfo.h"

//class MutliThreadOp : public QObject
//{
//	Q_OBJECT
//
//public:
//	MutliThreadOp(QObject* parent = nullptr) {
//
//	}
//	~MutliThreadOp() {
//
//	}
//};
//
///**
//
//	@class   FileRunnable
//	@brief   �ļ���ȡ�߳� ע���߳��д��������߳̽��������̷߳����ź�ͬʱ����д���ݿ��߳�
//	@details ~
//
//**/
//class FileRunnable :public QObject, public QRunnable
//{
//	Q_OBJECT
//
//public:
//	explicit FileRunnable(QObject* parent = nullptr);
//	~FileRunnable();
//
//	void dealFile(QStringList);
//
//protected:
//	void run() override;
//
//public:
//
//
//signals:
//	void working();
//	void finish();
//
//private:
//	QStringList dealFileList;
//
//};



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



#endif