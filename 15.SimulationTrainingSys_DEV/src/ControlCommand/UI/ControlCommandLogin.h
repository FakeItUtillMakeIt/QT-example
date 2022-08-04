#pragma once

#include <QWidget>
#include "ui_ControlCommandLogin.h"
#include	"controlcommand.h"
class ControlCommandLogin : public QWidget
{
	Q_OBJECT

public:
	ControlCommandLogin(ControlCommand* ,QWidget *parent = nullptr);
	~ControlCommandLogin();
	
private:
	Ui::ControlCommandLoginClass ui;
	ControlCommand* controlCommand;
public slots:
	void LoginSlot();
	void CloseSlot();
signals:
	void LoginSignal();
};
