#ifndef ELEMENTDISPLAYER_H
#define ELEMENTDISPLAYER_H

#include <QLabel>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QWidget>
#include <qcheckbox.h>


class ElementDisPlayer:public QWidget
{
public:
    ElementDisPlayer(QWidget* parent = nullptr);
    void addWidget(QWidget*namelabel, QWidget* widget);
    void removeWidget(QWidget* namelabel, QWidget* widget);
private:
    int curpos = 0;
    QVBoxLayout* vlayout = nullptr;
    int m_minimumHeight = 1000;
    QList<QWidget*> namelabelList;
    QList<QWidget*> styleWidgetlist;
protected:
    void mousePressEvent(QMouseEvent *event);
    int lastpos  = 0;
    int interval = 5;
};

#endif // ELEMENTDISPLAYER_H
