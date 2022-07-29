#include "configpairlabel.h"
#include "ctrolfactory.h"
ConfigNameSpaceStart
CtrolFactory::CtrolFactory()
{

}

QWidget *CtrolFactory::CreateControl(ControlType ctroltype,ControlRole ctrlrole,QString text,QWidget* parent,QString groupuuid)
{
    QWidget* newwidget = nullptr;
    ConfigButton* btn = nullptr;
    ConfigLabel* label = nullptr;
    ConfigPairLabel* pairlabel = nullptr;
    switch (ctroltype) {
    case cConfigButton:
        btn  = new ConfigButton("按钮",parent);
        btn->m_ingroup = true;
        btn->setRole(ctrlrole);
        btn->setGroupId(groupuuid);
        newwidget = btn;
        break;
    case cConfigLabel:
        label = new ConfigLabel(parent);
        label->m_ingroup = true;

        label->setGroupId(groupuuid);
        label->setText("单独标签");
        label->setRole(ctrlrole);
        newwidget = label;
        break;
    case cConfigGroup:
        btn  = new ConfigButton(text,parent);
        btn->setRole(ctrlrole);
        btn->setGroupId(groupuuid);
        newwidget = btn;
        break;
    case cConfigPairLabel:
        pairlabel  = new ConfigPairLabel(parent);
        pairlabel->m_ingroup = true;
        pairlabel->setRole(ctrlrole);
        pairlabel->setGroupId(groupuuid);
        newwidget = pairlabel;
        break;
    default:
        newwidget = new QWidget(parent);
        break;
    }
    return newwidget;
}


ConfigNameSpaceEnd
