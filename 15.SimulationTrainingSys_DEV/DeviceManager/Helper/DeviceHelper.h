#include <QObject> 
#include <iostream>
using namespace std;
#include <vector>
#include <QThread> 
using namespace std;

class DeviceHelper : public QObject
{
	Q_OBJECT
public:
	DeviceHelper(QString baseFile, QObject* parent = Q_NULLPTR);
	~DeviceHelper(); 
	FILE* m_comFile;
public slots: 
	void receiveData();
	void init();
private:
	bool m_isRun; 
 
	QString m_baseFile;
};