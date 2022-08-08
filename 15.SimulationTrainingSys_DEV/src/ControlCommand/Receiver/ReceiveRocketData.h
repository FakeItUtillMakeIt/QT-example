
#ifndef RECEIVE_CMD_DATA_H
#define RECEIVE_CMD_DATA_H


#include <QObject> 
#include <iostream>
using namespace std;
#include <vector>
#include <QThread>
#include <QUdpSocket>
#include "../AppCache.h"
#include "../../Model/Command.h"

using namespace std;

class ReceiveRocketData : public QObject
{
	Q_OBJECT
public:
	ReceiveRocketData(QObject* parent = Q_NULLPTR);
	~ReceiveRocketData();
public slots:
	void receiveData();
	void init();
private:
	bool m_isRun;
	QUdpSocket* m_pSocket;
	AppCache* m_app;

signals:
	void receiverCMD(QVariant command);
};


#endif