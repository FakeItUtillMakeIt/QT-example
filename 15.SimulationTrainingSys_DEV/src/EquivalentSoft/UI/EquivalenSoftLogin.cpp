#include "EquivalenSoftLogin.h"

EquivalenSoftLogin::EquivalenSoftLogin(EquivalentSoft *eq,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	equivalentSoft = eq;
	setWindowFlags(Qt::FramelessWindowHint);

	connect(ui.LoginBt, SIGNAL(clicked()), this, SLOT(LoginSlot()));
	connect(ui.CloseBt, SIGNAL(clicked()), this, SLOT(CloseSlot()));
}
void EquivalenSoftLogin::LoginSlot()
{
	this->close();
	equivalentSoft->show();
	//ControlComPage.show();
}
void EquivalenSoftLogin::CloseSlot()
{
	this->close();
}

EquivalenSoftLogin::~EquivalenSoftLogin()
{}
