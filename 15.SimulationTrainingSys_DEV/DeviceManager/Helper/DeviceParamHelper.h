#include <QObject> 
#include <iostream>
using namespace std;
#include <vector>
#include <QThread> 
#include "../../Model/DeviceParam.h"
using namespace std;

class DeviceParamHelper : public QObject
{
	Q_OBJECT
public:
	DeviceParamHelper(DeviceParam* deviceParam, QObject* parent = Q_NULLPTR);
	~DeviceParamHelper(); 
	FILE* m_comFile;
public slots: 
	void receiveData();
	void init();
private:
	bool m_isRun;  
	QString m_baseFile; 
	DeviceParam* m_deviceParam;
};