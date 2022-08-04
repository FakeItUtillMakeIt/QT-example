#pragma once
#pragma execution_character_set("utf-8")

#ifndef DEVICE_NET_MANAGER_H
#define DEVICE_NET_MANAGER_H



#include <QObject>

class DeviceNetManage : public QObject
{
	Q_OBJECT

public:
	DeviceNetManage(QObject* parent);
	~DeviceNetManage();
};


#endif