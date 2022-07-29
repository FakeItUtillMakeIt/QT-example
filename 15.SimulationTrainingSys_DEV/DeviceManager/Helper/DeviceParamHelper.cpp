#include "DeviceParamHelper.h"
 
#include <QDateTime> 
#define FRAMELENGTH 36
DeviceParamHelper::DeviceParamHelper(DeviceParam* deviceParam, QObject* parent)
	: QObject(parent), 
	m_isRun(true), 
    m_comFile(nullptr),
    m_deviceParam(deviceParam)
{   
    init();
}

DeviceParamHelper::~DeviceParamHelper()
{
    if (m_comFile)
        fclose(m_comFile);
    m_comFile = nullptr;
}

void DeviceParamHelper::init()
{   
    QString filepath = m_baseFile + "/gps" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".bin";
    m_comFile = fopen(filepath.toStdString().c_str(), "wb");
}


void DeviceParamHelper::receiveData()
{ 
 
} 