#pragma once

#include <QWidget>
#include "ui_FaultInjectionLogin.h"
#include "FaultInjection.h"
#include "../AppCache.h"
#include "../Database/DeviceDAOF.h"
class FaultInjectionLogin : public QWidget
{
	Q_OBJECT

public:
	FaultInjectionLogin(FaultInjection *,QWidget *parent = nullptr);
	~FaultInjectionLogin();
public slots:
	void LoginSlot();
	void CloseSlot();
signals:
	void LoginSignal();
private:
	Ui::FaultInjectionLoginClass ui;
	FaultInjection* faultInjection;
	void Init();
	AppCache* m_app;
	DataBaseF::DeviceDAO* m_pDeviceDAO;
};
