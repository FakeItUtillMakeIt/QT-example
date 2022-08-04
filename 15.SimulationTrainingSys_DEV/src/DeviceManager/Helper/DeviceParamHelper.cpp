#include "DeviceParamHelper.h"

#include <QDateTime> 
#define FRAMELENGTH 36
DeviceParamHelper::DeviceParamHelper(DeviceParam* deviceParam, int dStatId, QObject* parent)
	: QObject(parent),
	m_deviceParam(deviceParam), deviceStatID(dStatId)
{
	updateDeviceParam();
}

DeviceParamHelper::~DeviceParamHelper()
{

}

/**
	@brief 更新设备参数
**/
void DeviceParamHelper::updateDeviceParam() {
	//根据设备状态Id获取相应文件
	AppCache* m_app = AppCache::instance();
	auto deviceStatInfo = m_app->m_allDeviceStatus[deviceStatID];
	//打开文件
	QString filePath1 = tr("./device/") + QString::fromStdString(deviceStatInfo->m_dataPath);

	QFile file(filePath1);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}

	QTextStream in(&file);
	while (!in.atEnd())
	{
		QString line = in.readLine();
	}

	m_deviceParam->m_iCurStr;
	m_deviceParam->m_iCurValue;
	m_deviceParam->m_status;
	m_deviceParam->m_statusRealData;
}
