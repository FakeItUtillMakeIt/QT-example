#include "MutliThread.h"

MutliThread::MutliThread(QObject* parent)
	: QObject(parent)
{
	setAutoDelete(false);
}

MutliThread::~MutliThread()
{}

void MutliThread::recvNum(int num) {
	iRecvNum = num;
}


void MutliThread::run() {

	qDebug() << "线程:" << iRecvNum << "||" << QThread::currentThread();
	QElapsedTimer time;
	time.start();
	for (int i = 0; i < iRecvNum; i++)
	{
		QThread::msleep(1);//处理任务
		emit working(i);
	}

	qDebug() << "线程耗时:" << time.elapsed() << "ms";
	emit finish(QString::number(time.elapsed()));

}