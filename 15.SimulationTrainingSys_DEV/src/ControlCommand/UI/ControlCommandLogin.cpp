#include "ControlCommandLogin.h"
#include "ControlCommand.h"
#include <QpushButton>
ControlCommandLogin::ControlCommandLogin(ControlCommand *conCom,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	controlCommand = conCom;
	connect(ui.LoginBt, SIGNAL(clicked()), this,SLOT(LoginSlot()));
	connect(ui.CloseBt, SIGNAL(clicked()), this, SLOT(CloseSlot()));
}
void ControlCommandLogin::LoginSlot()
{
	this->close();
	controlCommand->show();
	//ControlComPage.show();
}
void ControlCommandLogin::CloseSlot() 
{
	this->close();
}
ControlCommandLogin::~ControlCommandLogin()
{}
