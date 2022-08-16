#pragma once
#ifndef MUTLI_THREAD_H
#define MUTLI_THREAD_H


#include <QObject>
#include <QtWidgets>
#include <QRunnable>

class MutliThread : public QObject, public QRunnable
{
	Q_OBJECT

public:
	explicit MutliThread(QObject* parent = nullptr);
	void recvNum(int num);
	~MutliThread();

signals:
	void working(int num);
	void finish(QString elapsedTime);

protected:
	void run() override;
private:
	int iRecvNum;
};

#endif