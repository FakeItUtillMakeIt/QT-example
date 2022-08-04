#pragma once

#ifndef DEVICE_MANAGER_LOGIN_H
#define DEVICE_MANAGER_LOGIN_H


#include <QWidget>
#include "ui_DeviceManagerLogin.h"
#include "DeviceManager.h"
#include "../AppCache.h"
#include "../Database/DeviceDAO.h"
class DeviceManagerLogin : public QWidget
{
	Q_OBJECT

public:
	DeviceManagerLogin(DeviceManager*, QWidget* parent = nullptr);
	~DeviceManagerLogin();

private:
	Ui::DeviceManagerLoginClass ui;
	DeviceManager* deviceManager;
	void Init();
	AppCache* m_app;
	DataBase::DeviceDAO* m_pDeviceDAO;
public slots:
	void LoginSlot();
	void CloseSlot();
signals:
	void LoginSignal();
};

#endif