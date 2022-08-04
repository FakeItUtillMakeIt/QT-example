#include "FaultInjectionLogin.h"

FaultInjectionLogin::FaultInjectionLogin(FaultInjection * fj,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	faultInjection = fj;
	setWindowFlags(Qt::FramelessWindowHint);

	connect(ui.LoginBt, SIGNAL(clicked()), this, SLOT(LoginSlot()));
	connect(ui.CloseBt, SIGNAL(clicked()), this, SLOT(CloseSlot()));
}
void FaultInjectionLogin::LoginSlot()
{
	this->close();
	faultInjection->show();
	//ControlComPage.show();
}
void FaultInjectionLogin::CloseSlot()
{
	this->close();
}
FaultInjectionLogin::~FaultInjectionLogin()
{}
