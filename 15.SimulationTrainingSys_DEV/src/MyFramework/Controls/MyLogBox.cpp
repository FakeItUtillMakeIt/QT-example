#include "MyLogBox.h"

MyLogBox::MyLogBox(QWidget* parent) : QTextBrowser(parent)
{
    this->setStyleSheet("background-color:rgb(220, 220, 220)");
    //this->setGeometry(QRect(2, 517, 910, 220));
}
MyLogBox::~MyLogBox()
{

}
void MyLogBox::leaveEvent(QEvent *e)
{
    emit leaveInfo(this);
}
 
 