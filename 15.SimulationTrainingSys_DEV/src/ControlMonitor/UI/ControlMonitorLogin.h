#pragma once

#include <QWidget>
#include "ui_ControlMonitorLogin.h"
#include "controlMonitor.h"
class ControlMonitorLogin : public QWidget
{
	Q_OBJECT

public:
	ControlMonitorLogin(ControlMonitor *, QWidget *parent = nullptr);
	~ControlMonitorLogin();

private:
	Ui::ControlMonitorLoginClass ui;
	ControlMonitor* controlMonitor;
	void Init();
	AppCache* m_app;
	DataBase::DeviceDAO* m_pDeviceDAO;
public slots:
	void LoginSlot();
	void CloseSlot();
signals:
	void LoginSignal();
};
