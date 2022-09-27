#ifndef STYLELIST_H
#define STYLELIST_H
#include "configdefine.h"
#include "elementdisplayer.h"
#include <QLabel>
#include <QScrollArea>
#include <QWidget>

ConfigNameSpaceStart

class StyleList:public QScrollArea
{
    Q_OBJECT
public:
    StyleList(QWidget* widget = nullptr);
    ElementDisPlayer * scrollWidget = nullptr;
  //  void addWidget(QWidget* widget);
    void addWidget(QWidget*namelabel, QWidget *widget);
    void removeWidget(QWidget* namelabel, QWidget* widget);

};

ConfigNameSpaceEnd
#endif // STYLELIST_H
