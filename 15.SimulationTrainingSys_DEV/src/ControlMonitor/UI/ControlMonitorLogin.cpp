#include "ControlMonitorLogin.h"
ControlMonitorLogin::ControlMonitorLogin(ControlMonitor * conMoni,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	controlMonitor = conMoni;
	connect(ui.LoginBt, SIGNAL(clicked()), this, SLOT(LoginSlot()));
	connect(ui.CloseBt, SIGNAL(clicked()), this, SLOT(CloseSlot()));
}
void ControlMonitorLogin::LoginSlot()
{
	this->close();
	controlMonitor->show();
	//ControlComPage.show();
}
void ControlMonitorLogin::CloseSlot()
{
	this->close();
}

ControlMonitorLogin::~ControlMonitorLogin()
{}
