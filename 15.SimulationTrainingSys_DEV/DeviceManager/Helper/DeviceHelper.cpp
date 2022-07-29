#include "DeviceHelper.h"
 
#include <QDateTime> 
#define FRAMELENGTH 36
DeviceHelper::DeviceHelper(QString baseFile, QObject* parent)
	: QObject(parent), 
	m_isRun(true), 
    m_comFile(nullptr),
    m_baseFile(baseFile)
{   
    init();
}

DeviceHelper::~DeviceHelper()
{
    if (m_comFile)
        fclose(m_comFile);
    m_comFile = nullptr;
}

void DeviceHelper::init()
{   
    QString filepath = m_baseFile + "/gps" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".bin";
    m_comFile = fopen(filepath.toStdString().c_str(), "wb");
}


void DeviceHelper::receiveData()
{
 
} 