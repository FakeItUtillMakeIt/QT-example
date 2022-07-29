#ifndef CTROLFACTORY_H
#define CTROLFACTORY_H

#include "configbutton.h"
#include "configlabel.h"

ConfigNameSpaceStart

class CtrolFactory
{
public:
    CtrolFactory();
    QWidget* CreateControl(ControlType ctroltype, ControlRole ctrlrole, QString text, QWidget *parent, QString groupuuid ="");
};

ConfigNameSpaceEnd
#endif // CTROLFACTORY_H
