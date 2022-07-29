#include "elementdisplayer.h"

#include <QComboBox>
#include <QPushButton>
#include <QDebug>
ElementDisPlayer::ElementDisPlayer(QWidget *parent):QWidget(parent)
{
    setMinimumHeight(m_minimumHeight);
}

void ElementDisPlayer::addWidget(QLabel* namelabel,QWidget *widget)
{
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

void ElementDisPlayer::mousePressEvent(QMouseEvent *event)
{
   qDebug() << "event:" << event->pos();
   QWidget::mousePressEvent(event);
}
