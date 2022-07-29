#include "booldialog.h"
ConfigNameSpaceStart
BoolDialog::BoolDialog(QWidget *parent, Qt::WindowFlags f):QDialog(parent,f)
{
    okbtn.setText("显示");
    cancelbtn.setText("不显示");
    hlayout.addWidget(&okbtn);
    hlayout.addWidget(&cancelbtn);
    hlayout.setSpacing(20);
    setLayout(&hlayout);
    connect(&okbtn,&QPushButton::clicked,[=](){
        this->accept();
    });
    connect(&cancelbtn,&QPushButton::clicked,[=](){
        this->reject();
    });
}
ConfigNameSpaceEnd
