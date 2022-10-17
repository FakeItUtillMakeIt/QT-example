#pragma once

#include <QWidget>
#include "ui_ControlMonitorLogin.h"
#include "controlMonitor.h"
#include "../../Model/SingleApplication.h"
class ControlMonitorLogin : public QWidget
{
	Q_OBJECT

public:
	ControlMonitorLogin(SingleApplication* app, ControlMonitor *, QWidget *parent = nullptr);
	~ControlMonitorLogin();

private:
	Ui::ControlMonitorLoginClass ui;
	ControlMonitor* controlMonitor;
	void Init();
	AppCache* m_app;
	SingleApplication* m_nowApp;
	DataBase::DeviceDAO* m_pDeviceDAO;
public slots:
	void LoginSlot();
	void CloseSlot();
signals:
	void LoginSignal();
	void sendrocketType(int id);
};
