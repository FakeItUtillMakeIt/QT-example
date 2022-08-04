#include "controlpoint.h"
#include "controlregion.h"
#include <QDebug>
#include <QHBoxLayout>
ConfigNameSpaceStart
ControlRegion::ControlRegion(QWidget *parent):QWidget(parent)
{
    containwid = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(containwid);
    layout->setMargin(5);
    setLayout(layout);
    setFocusPolicy(Qt::ClickFocus);
    load_ctrl_point();
    show_ctrl_point(false);

}

void ControlRegion::update_ctrl_point_pos()
{
    tlPoint->move(containwid->rect().topLeft());
    blPoint->move(containwid->rect().bottomLeft());
    trPoint->move(containwid->rect().topRight());
    brPoint->move(containwid->rect().bottomRight());

    tmPoint->move((containwid->rect().left()+ containwid->rect().right())/2,containwid->rect().top());
    bmPoint->move((containwid->rect().left()+ containwid->rect().right())/2,containwid->rect().bottom());
    lmPoint->move(containwid->rect().left(), (containwid->rect().bottom() + containwid->rect().top())/2);
    rmPoint->move(containwid->rect().right(), (containwid->rect().top() + containwid->rect().bottom())/2);
    emit control_changed();
}

void ControlRegion::show_ctrl_point(bool enabled)
{

    tlPoint->setVisible(enabled);
    blPoint->setVisible(enabled);
    trPoint->setVisible(enabled);
    brPoint->setVisible(enabled);

    tmPoint->setVisible(enabled);
    bmPoint->setVisible(enabled);
    lmPoint->setVisible(enabled);
    rmPoint->setVisible(enabled);
}

void ControlRegion::load_ctrl_point()
{
    tlPoint = new ControlPoint(10,ControlPoint::dTopLeft, this);
    tlPoint->setControlTyle(ControlPoint::cMoveCtrol);
    connect(tlPoint,&ControlPoint::control_moved,[=](QPoint interval){
    //    qDebug() << "interval:" << interval;
        QPoint newpos = pos() + interval;
        if (newpos.x() < 0)
            newpos.setX(0);
        if (newpos.y() < 0)
            newpos.setY(0);
        QRect arearange = ((QWidget*)this->parent())->geometry();
        if (newpos.x() > arearange.width()-(this->width()/2))
            newpos.setX(arearange.width() - (this->width() / 2));
        if (newpos.y() > arearange.height()- (this->height()/2))
            newpos.setY(arearange.height() - (this->height() / 2));

        this->move(newpos);
        update_ctrl_point_pos();
    });
    tlPoint->move(containwid->rect().topLeft());
    tlPoint->show();

    blPoint = new ControlPoint(10,ControlPoint::dBottomLeft,this);
    connect(blPoint,&ControlPoint::control_moved,[=](QPoint interval){
       this->setGeometry(geometry().adjusted(interval.x(),0,0,interval.y()));
       update_ctrl_point_pos();
    });
    blPoint->move(containwid->rect().bottomLeft());
    blPoint->show();

    trPoint = new ControlPoint(10,ControlPoint::dTopRight,this);
    connect(trPoint,&ControlPoint::control_moved,[=](QPoint interval){
       this->setGeometry(geometry().adjusted(0,interval.y(),interval.x(),0));
       update_ctrl_point_pos();
    });
    trPoint->move(containwid->rect().topRight());
    trPoint->show();

    brPoint = new ControlPoint(10,ControlPoint::dBottomRight,this);
    connect(brPoint,&ControlPoint::control_moved,[=](QPoint interval){
       this->setGeometry(geometry().adjusted(0,0,interval.x(),interval.y()));
       update_ctrl_point_pos();
    });
    brPoint->move(containwid->rect().bottomRight());
    brPoint->show();

//中间的控制点

    tmPoint = new ControlPoint(10,ControlPoint::dTopMiddle,this);
    connect(tmPoint,&ControlPoint::control_moved,[=](QPoint interval){
       this->setGeometry( geometry().adjusted(0,interval.y(),0,0));
       update_ctrl_point_pos();
    });
    tmPoint->move((containwid->rect().left()+ containwid->rect().right())/2,containwid->rect().top());
    tmPoint->show();

    bmPoint = new ControlPoint(10,ControlPoint::dBottomMiddle,this);
    connect(bmPoint,&ControlPoint::control_moved,[=](QPoint interval){
       this->setGeometry(geometry().adjusted(0,0,0,interval.y()));
       update_ctrl_point_pos();
    });
    bmPoint->move((containwid->rect().left()+ containwid->rect().right())/2,containwid->rect().bottom());
    bmPoint->show();


    lmPoint = new ControlPoint(10,ControlPoint::dLeftMiddle,this);
    connect(lmPoint,&ControlPoint::control_moved,[=](QPoint interval){
       this->setGeometry(geometry().adjusted(interval.x(),0,0,0));
       update_ctrl_point_pos();
    });
    lmPoint->move(containwid->rect().left(), (containwid->rect().bottom() + containwid->rect().top())/2);
    lmPoint->show();

    rmPoint = new ControlPoint(10,ControlPoint::dRightMiddle,this);
    connect(rmPoint,&ControlPoint::control_moved,[=](QPoint interval){
       this->setGeometry(geometry().adjusted(0,0,interval.x(),0));
       update_ctrl_point_pos();
    });
    rmPoint->move(containwid->rect().right(), (containwid->rect().top() + containwid->rect().bottom())/2);
    rmPoint->show();

}

void ControlRegion::showEvent(QShowEvent *event)
{
//    if(!inited)
//    {
//        qDebug() << "load_ctrl_point";
//      inited = true;
//      load_ctrl_point();
//      show_ctrl_point(false);
//    }
}

void ControlRegion::focusInEvent(QFocusEvent *event)
{
    show_ctrl_point(true);

}
void ControlRegion::focusOutEvent(QFocusEvent *event)
{
  //  qDebug() << "focusOutEvent";
    //controlwidget->befocused = false;
    //controlwidget->update();
    show_ctrl_point(false);
   // emit sig_selected(false);
}

void ControlRegion::resizeEvent(QResizeEvent *event)
{
    update_ctrl_point_pos();
}


void ControlRegion::mousePressEvent(QMouseEvent *event)//窗口移动事件
{
    update_ctrl_point_pos();
    if(event->button()==Qt::LeftButton)
    {
          dragPosition=event->globalPos()-frameGeometry().topLeft();//鼠标位置减去左上角的左边
          event->accept();

          if(containwid->geometry().adjusted(30,30,-30,-30).contains(event->pos()))
          {
            //  qDebug() << "moving region:" << geometry().adjusted(30,30,-30,-30) << ":"<<geometry() << ":" << event->pos();

          }

    }
    raise();
   // QWidget::mousePressEvent(event);

}
void ControlRegion::mouseMoveEvent(QMouseEvent *event)//窗口移动事件
{
    if(event->buttons()&Qt::LeftButton)
    {
        if(dragenabled)
        {
          QPoint  newpoint = event->globalPos() - dragPosition;
          if (newpoint.x() < 0)
          {
              newpoint.setX(2);
          }
          event->accept();
          emit control_changed();
        }
    }

}

void ControlRegion::mouseReleaseEvent(QMouseEvent *event)
{
     dragenabled = false;
     //QWidget::mouseReleaseEvent(event);
}

ConfigNameSpaceEnd
