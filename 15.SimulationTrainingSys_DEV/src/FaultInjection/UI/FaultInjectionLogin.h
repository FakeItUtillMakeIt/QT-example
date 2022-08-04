#pragma once

#include <QWidget>
#include "ui_FaultInjectionLogin.h"
#include "FaultInjection.h"
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
};
