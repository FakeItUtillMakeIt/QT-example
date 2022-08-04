
#ifndef DEVICE_HELPER_H
#define DEVICE_HELPER_H

#include "../AppCache.h"
#include "../Model/Device.h"
#include <QObject> 
#include <iostream>
using namespace std;
#include <vector>
#include <QThread> 


using namespace std;

/**

	@class   DeviceHelper
	@brief		设备代理  进行设备状态切换
	@details ~

**/
class DeviceHelper : public QObject
{
	Q_OBJECT
public:
	DeviceHelper(Device* m_Device, int m_StatusId, QObject* parent = Q_NULLPTR);
	~DeviceHelper();

public slots:


private:
	Device* opDevice;
	int statId;

	void updateDeviceStatus();

};

#endif