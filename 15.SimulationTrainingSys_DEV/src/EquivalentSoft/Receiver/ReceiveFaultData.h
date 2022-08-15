#include <QObject> 
#include <iostream>
using namespace std;
#include <vector>
#include <QThread>
#include <QUdpSocket>
#include "../AppCache.h"
#include "../../Model/Command.h"

using namespace std;

class ReceiveFaultData : public QObject
{
	Q_OBJECT
public:
	ReceiveFaultData(QObject* parent = Q_NULLPTR);
	~ReceiveFaultData();  
public slots: 
	void receiveData();
	void init();
private:
	bool m_isRun; 
	QUdpSocket* m_pCMDSocket; 
	AppCache* m_app;

signals:
	void receiverFault(QVariant command);
};