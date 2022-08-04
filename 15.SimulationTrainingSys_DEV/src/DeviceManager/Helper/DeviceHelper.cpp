#include "DeviceHelper.h"

#include <QDateTime> 
#define FRAMELENGTH 36
DeviceHelper::DeviceHelper(Device* m_Device, int m_StatusId, QObject* parent)
	: QObject(parent), opDevice(m_Device), statId(m_StatusId)
{
	updateDeviceStatus();
}

DeviceHelper::~DeviceHelper()
{

}

/**
	@brief 更新设备状态
**/
void DeviceHelper::updateDeviceStatus() {
	AppCache* m_app = AppCache::instance();

	//根据状态ID进行设备状态切换
	opDevice->m_sCurStatus = m_app->m_allDeviceStatus[statId]->m_statusName;

}
