#include "MyInfoTip.h"

MyInfoTip::MyInfoTip(QWidget* parent) : QLabel(parent)
{
	this->setGeometry(QRect(38, 4, 790, 24));
	this->setCursor(Qt::PointingHandCursor);
}
MyInfoTip::~MyInfoTip()
{

}
void MyInfoTip::enterEvent(QEvent *e)
{
    emit hoverInfo(this);
}
//void MyInfoTip::leaveEvent(QEvent *e)
//{ 
//	QWidget::leaveEvent(e);
//} 
void MyInfoTip::setLabelText(bool is_alarm, QString info)
{
	this->setText(info);
	QString name_str1 = is_alarm ? "rgb(255, 0, 0)" : "rgb(55, 55, 55)";
	this->setStyleSheet(QString("QLabel {\n"
		"	color: %1;\n"
		"}").arg(name_str1));
}