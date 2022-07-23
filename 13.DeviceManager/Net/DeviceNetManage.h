#pragma once
#pragma execution_character_set("utf-8")


#include <QObject>

class DeviceNetManage  : public QObject
{
	Q_OBJECT

public:
	DeviceNetManage(QObject *parent);
	~DeviceNetManage();
};
