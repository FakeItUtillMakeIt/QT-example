#include "stylelist.h"

#include <QStackedWidget>

ConfigNameSpaceStart
StyleList::StyleList(QWidget * parent):QScrollArea(parent)
{
    setStyleSheet("background-color:rgba(200,200,200,50)");
    scrollWidget = new ElementDisPlayer;
    scrollWidget->setMinimumSize(1115,1000);
    setWidget(scrollWidget);
}

void StyleList::addWidget(QLabel* namelabel,QWidget *widget)
{
    if(widget&&scrollWidget)
    {
        scrollWidget->addWidget(namelabel,widget);
    }
}
ConfigNameSpaceEnd
