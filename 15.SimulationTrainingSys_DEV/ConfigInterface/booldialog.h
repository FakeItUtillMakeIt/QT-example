#ifndef BOOLDIALOG_H
#define BOOLDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include "configdefine.h"

ConfigNameSpaceStart
class BoolDialog:public QDialog
{
    Q_OBJECT
public:
    BoolDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    QPushButton  okbtn;
    QPushButton cancelbtn;
    QHBoxLayout hlayout;
};
ConfigNameSpaceEnd
#endif // BOOLDIALOG_H
