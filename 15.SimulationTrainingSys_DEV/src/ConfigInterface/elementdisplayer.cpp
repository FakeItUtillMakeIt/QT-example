#include "elementdisplayer.h"

#include <QComboBox>
#include <QPushButton>
#include <QDebug>

ElementDisPlayer::ElementDisPlayer(QWidget *parent):QWidget(parent)
{
    setMinimumHeight(m_minimumHeight);
}

void ElementDisPlayer::addWidget(QWidget* namelabel,QWidget *widget)
{
    namelabelList.append(namelabel);
    styleWidgetlist.append(widget);
    QVector<QWidget*> styleWidgetlist;
    namelabel->setParent(this);
    namelabel->setFixedSize(100,50);
    widget->setParent(this);
    namelabel->move(10,lastpos);
    widget->move(160,lastpos);
    lastpos = lastpos + widget->height() +5;
    qDebug() << "ElementDisPlayer::addWidget widget:" << widget->geometry();
    namelabel->show();
    widget->show();
    if(lastpos > this->height())
    {
        m_minimumHeight += 1000;
        setMinimumHeight(m_minimumHeight);
    }
}
void ElementDisPlayer::removeWidget(QWidget* namelabel, QWidget* widget)
{
    namelabel->setParent(nullptr);
    widget->setParent(nullptr);
    namelabel->setVisible(false);
    widget->setVisible(false);
    namelabelList.removeOne(namelabel);
    styleWidgetlist.removeOne(widget);

    lastpos = 0;
    for (int i = 0; i < styleWidgetlist.size(); i++)
    {
        namelabelList[i]->move(10, lastpos);
        styleWidgetlist[i]->move(160, lastpos);
        lastpos = lastpos + styleWidgetlist[i]->height() + 5;
    }
}
void ElementDisPlayer::mousePressEvent(QMouseEvent *event)
{
   qDebug() << "event:" << event->pos();
   QWidget::mousePressEvent(event);
}
