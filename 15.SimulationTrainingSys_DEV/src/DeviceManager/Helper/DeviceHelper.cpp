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
	@brief �����豸״̬
**/
void DeviceHelper::updateDeviceStatus() {
	AppCache* m_app = AppCache::instance();

	//����״̬ID�����豸״̬�л�
	opDevice->m_sCurStatus = m_app->m_allDeviceStatus[statId]->m_statusName;

}
