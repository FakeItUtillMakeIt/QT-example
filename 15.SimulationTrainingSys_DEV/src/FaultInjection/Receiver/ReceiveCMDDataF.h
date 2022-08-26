#include <QObject> 
#include <iostream>
using namespace std;
#include <vector>
#include <QThread>
#include <QUdpSocket>
#include "../AppCache.h"
#include "../../Model/Command.h"

using namespace std;

class ReceiveCMDDataF : public QObject
{
	Q_OBJECT
public:
	ReceiveCMDDataF(string strIP, unsigned int iPort,QObject* parent = Q_NULLPTR);
	~ReceiveCMDDataF();  
public slots: 
	void receiveData();
	void init(string strIP,unsigned int iPort);
private:
	bool m_isRun; 
	QUdpSocket* m_pCMDSocket; 
	AppCache* m_app;

signals:
	void receiverCMD(QByteArray command);
};