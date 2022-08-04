#ifndef CONFIGELEMENTSELECTOR_H
#define CONFIGELEMENTSELECTOR_H

#include <QPushButton>
#include "configdefine.h"
ConfigNameSpaceStart
class ConfigElementSelector:public QPushButton
{
public:
    ConfigElementSelector(QWidget* parent = nullptr);
    ControlType m_ctrltype;
    ControlRole m_ctrlrole;
    void mousePressEvent(QMouseEvent *e);
    void setSelectorInfo(ControlType ctrtype, QString name);
private:
    void handleDragEvent();
};
ConfigNameSpaceEnd
#endif // CONFIGELEMENTSELECTOR_H
