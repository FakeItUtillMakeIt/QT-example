#include "controlpoint.h"
#include <QDebug>
#include <QPainter>

ConfigNameSpaceStart
ControlPoint::ControlPoint(int size,Direct direct,QWidget *parent):QWidget(parent),m_size(size),m_direct(direct)
{
    resize(size,size);
}

void ControlPoint::update_contrlwidget(QWidget *controlwidget)
{
    m_ctrlwidget = controlwidget;
}
void ControlPoint::enable_move(bool bval)
{
    m_moveenabled = bval;
}

void ControlPoint::setControlTyle(ControlPoint::CtrolType ctrltype)
{
    m_controltype = ctrltype;
}

void ControlPoint::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing, true);   //打开防锯齿
    QRect irect = rect();
    if(m_controltype == cResizeControl)
    {
        painter.setBrush(QColor(250,250,0));
        painter.drawEllipse(rect().adjusted(1,1,-1,-1));
    }else if(m_controltype == cMoveCtrol)
    {
        painter.setBrush(QColor(0,0,0));
        painter.drawRect(rect().adjusted(1,1,-1,-1));
    }
    painter.setRenderHints(QPainter::Antialiasing, false);   //打开防锯齿

}
void ControlPoint::mousePressEvent(QMouseEvent *event)//窗口移动事件
{
    qDebug() << "ControlPoint mousePressEvent";

    if(event->button()==Qt::LeftButton)
    {
          dragPosition=event->globalPos()-frameGeometry().topLeft();//鼠标位置减去左上角的左边
          lastPosition = frameGeometry().topLeft();

          event->accept();
    }

  //  QWidget::mousePressEvent(event);
}

QRect & ControlPoint::ajust_distance(QRect& rect,QPoint&  interval )
{
    switch (m_direct) {
    case dTopLeft:
        break;
    case dBottomLeft:
        rect = rect.adjusted(interval.x(),0,0,interval.y());
        break;
    case dTopRight:
        rect = rect.adjusted(0,interval.y(),interval.x(),0);

        break;
    case dBottomRight:
        rect = rect.adjusted(0,0,interval.x(),interval.y());

        break;
    case dTopMiddle:
        rect = rect.adjusted(0,interval.y(),0,0);

        break;
    case dBottomMiddle:
        rect = rect.adjusted(0,0,0,interval.y());

        break;
    case dLeftMiddle:
        rect = rect.adjusted(interval.x(),0,0,0);

        break;
    case dRightMiddle:
        rect = rect.adjusted(0,0,interval.x(),0);

        break;
    default:
        break;
    }
    return  rect;
}

void ControlPoint::eventHandler(QPoint& interval)
{

    if(m_controltype == cMoveCtrol)
    {
        QPoint ctrlnewpos = m_ctrlwidget->pos() + interval;
        if (ctrlnewpos.x() < 0)
            ctrlnewpos.setX(0);
        if (ctrlnewpos.y() < 0)
            ctrlnewpos.setY(0);
        QRect arearange = ((QWidget*)m_ctrlwidget->parent())->geometry();
        if (ctrlnewpos.x() > arearange.width()-(this->width()/2))
            ctrlnewpos.setX(arearange.width() - (this->width() / 2));
        if (ctrlnewpos.y() > arearange.height()- (this->height()/2))
            ctrlnewpos.setY(arearange.height() - (this->height() / 2));

        m_ctrlwidget->move(ctrlnewpos);
    }
    if(m_controltype == cResizeControl)
    {
        QRect prirect = m_ctrlwidget->geometry();
        QRect adrect = ajust_distance(prirect,interval );
       // qDebug() << "prirect:" << prirect << " adrect:" << adrect  << " interval" <<interval;
        m_ctrlwidget->setGeometry(adrect);
    }
}

void ControlPoint::mouseMoveEvent(QMouseEvent *event)//窗口移动事件
{
  //  qDebug() << "ControlPoint mouseMoveEvent";

    if((event->buttons()&Qt::LeftButton)&&(m_moveenabled))
    {
    //    qDebug() << "ControlPoint m_moveenabled";

        QPoint newpos = event->globalPos()-dragPosition;
        QPoint interval = newpos - lastPosition;
        lastPosition  = newpos;
        eventHandler(interval);
    }
    QWidget::mouseMoveEvent(event);

}

void ControlPoint::mouseReleaseEvent(QMouseEvent *event)
{

     QWidget::mouseReleaseEvent(event);

}
ConfigNameSpaceEnd
