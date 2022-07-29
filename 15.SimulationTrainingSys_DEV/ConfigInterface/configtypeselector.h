#ifndef CONFIGTYPESELECTOR_H
#define CONFIGTYPESELECTOR_H

#include <QDialog>
#include <QListWidget>
#include <QWidget>
#include "configdefine.h"

ConfigNameSpaceStart
class StyleGroup;
class StylePairLabel;
class StyleButton;
class StyleCurve;
class ConfigTypeSelector:public QDialog
{
    Q_OBJECT
public:
    ConfigTypeSelector(QWidget* parent = nullptr);
    QListWidget* listwidget = nullptr;
    void UpdateTyleInfo(QList<StyleGroup *> &stylegroup);
    int getResult();
    void UpdateTyleInfo(QList<StylePairLabel *> &stylepairlabel);
    void UpdateTyleInfo(QList<StyleButton *> &stylebuttons);
    void UpdateTyleInfo(QList<StyleCurve *> &stylebuttons);
private:
    int  m_selectindex;
};
ConfigNameSpaceEnd
#endif // CONFIGTYPESELECTOR_H
