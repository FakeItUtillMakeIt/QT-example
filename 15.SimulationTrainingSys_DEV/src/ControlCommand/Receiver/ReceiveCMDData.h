#include <QObject> 
#include <iostream>
using namespace std;
#include <vector>
#include <QThread>
#include <QUdpSocket>
#include "../AppCache.h"
#include "../../Model/Command.h"

using namespace std;

class ReceiveCMDData : public QObject
{
	Q_OBJECT
public:
	ReceiveCMDData(QObject* parent = Q_NULLPTR);
	~ReceiveCMDData();  
public slots: 
	void receiveData();
	void init();
private:
	bool m_isRun; 
	QUdpSocket* m_pCMDSocket; 
	AppCache* m_app;

signals:
	void receiverCMD(QVariant command);
};