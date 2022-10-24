#pragma once

#ifndef DEVICE_MANAGER_LOGIN_H
#define DEVICE_MANAGER_LOGIN_H


#include <QWidget>
#include "ui_DeviceManagerLogin.h"
#include "DeviceManager.h"
#include "../AppCache.h"
#include "../Database/DeviceDAO.h"
#include "../Model/SingleApplication.h"
#include <QMovie>

#include <QSqlDatabase>

class DeviceManagerLogin : public QWidget
{
	Q_OBJECT

public:
	DeviceManagerLogin(SingleApplication* app,DeviceManager*, QWidget* parent = nullptr);
	~DeviceManagerLogin();

private:
	Ui::DeviceManagerLoginClass ui;
	DeviceManager* deviceManager;
	void Init();
	bool DatabseInit();
	int ExcuteSqlFile();
	AppCache* m_app;
	SingleApplication* m_nowApp;
	DataBase::DeviceDAO* m_pDeviceDAO;

	QWidget* dataLoading;
	QMovie* mv = new QMovie(":/DeviceManager/icon/loading2.gif");

	QSqlDatabase m_database;
public slots:
	void LoginSlot();
	void CloseSlot();
signals:
	void LoginSignal();
};

#endif