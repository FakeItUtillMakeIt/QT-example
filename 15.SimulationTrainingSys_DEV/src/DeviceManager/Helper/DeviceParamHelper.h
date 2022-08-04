#ifndef DEVICE_PARAM_HELPER_H
#define DEVICE_PARAM_HELPER_H


#include <QObject> 
#include <QtWidgets>
#include <iostream>
using namespace std;
#include <vector>
#include <QThread> 
#include "../../Model/DeviceParam.h"
#include "../AppCache.h"
using namespace std;

class DeviceParamHelper : public QObject
{
	Q_OBJECT
public:
	DeviceParamHelper(DeviceParam* deviceParam, int dStatId, QObject* parent = Q_NULLPTR);
	~DeviceParamHelper();
public slots:

private:
	int deviceStatID;
	QString m_baseFile;
	DeviceParam* m_deviceParam;

	void updateDeviceParam();
};

#endif