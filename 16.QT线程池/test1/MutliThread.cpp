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

	qDebug() << "�߳�:" << iRecvNum << "||" << QThread::currentThread();
	QElapsedTimer time;
	time.start();
	for (int i = 0; i < iRecvNum; i++)
	{
		QThread::msleep(1);//��������
		emit working(i);
	}

	qDebug() << "�̺߳�ʱ:" << time.elapsed() << "ms";
	emit finish(QString::number(time.elapsed()));

}