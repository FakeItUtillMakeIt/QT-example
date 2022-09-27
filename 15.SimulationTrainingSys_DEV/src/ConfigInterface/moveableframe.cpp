#include "moveableframe.h"
#include <QDebug>
#include <QPainter>

ConfigNameSpaceStart
 ControlPoint*  MoveAbleFrame::tlPoint;
 ControlPoint*  MoveAbleFrame::blPoint;
 ControlPoint*  MoveAbleFrame::trPoint;
 ControlPoint*  MoveAbleFrame::brPoint;

 ControlPoint*  MoveAbleFrame::tmPoint;
 ControlPoint*  MoveAbleFrame::bmPoint;
 ControlPoint*  MoveAbleFrame::lmPoint;
 ControlPoint*  MoveAbleFrame::rmPoint;

 QWidget* MoveAbleFrame::g_controledWidget = nullptr;
 QWidget* MoveAbleFrame::g_pointParent = nullptr;

 QList<ControlPoint*> MoveAbleFrame::ctrllist;
 int MoveAbleFrame::ctrlsize = 10;

 ControlType   MoveAbleFrame::m_ctrtype;
 bool    MoveAbleFrame::controlenabled = false;
 bool   MoveAbleFrame::beingroup = false;
 QString MoveAbleFrame::m_controledUuid;

MoveAbleFrame::MoveAbleFrame(QObject* parent):QObject(parent)
{

}

void MoveAbleFrame::focusInEvent(QFocusEvent *event)
{
    qDebug() << "focusInEvent";
    focused = true;
    //update_ctrl_point_pos();
    //QWidget::focusInEvent(event);
}


void MoveAbleFrame::focusOutEvent(QFocusEvent *event)
{
//    focused = false;
//    qDebug() << "focusOutEvent";
//    for(auto  it: ctrllist)
//    {
//        it->enable_move(false);
//        it->hide();
//    }
    focused = false;
   // QWidget::focusOutEvent(event);
}
void MoveAbleFrame::hide_ctrl_point()
{
    qDebug() << "hide_ctrl_point";
    for(auto  it: ctrllist)
    {
        it->enable_move(false);
        it->hide();
    }
    controlenabled = false;
    g_controledWidget = nullptr;
}


QPoint MoveAbleFrame::transPoint(QPoint  point, ControlPoint::Direct direct)
{                
    QPoint  offset(-5,-5);
   return  g_controledWidget->mapToParent(point+ offset);
}
QPoint MoveAbleFrame::transPoint(int  x, int y,ControlPoint::Direct direct)
{
    return  g_controledWidget->mapToParent(QPoint(x,y)- QPoint(5,5) );
}

void MoveAbleFrame::moveEvent(QMoveEvent *event)
{
  //  update_ctrl_point_pos();
}

void MoveAbleFrame::resizeEvent(QResizeEvent *event)
{
    //update_ctrl_point_pos_2();
}

void MoveAbleFrame::paintEvent(QPaintEvent *event)
{
  //  QPainter  painter(this);
//    painter.save();
//    if(focused)
//    {
//        QPen pen;
//        pen.setColor(QColor(250,0,0));
//        pen.setStyle(Qt::DotLine);
//        pen.setWidth(2);
//        painter.setPen(pen);
//        painter.drawRect(rect());
//    }
//    painter.restore();
}
void MoveAbleFrame::update_ctrl_point_pos(QWidget* controledWidget,QWidget* parenWidget)
{
    g_controledWidget = controledWidget;
    g_pointParent = parenWidget;
    for(auto  it: ctrllist)
    {
        it->setParent(parenWidget);
        it->raise();
    }
    tlPoint->move( transPoint(controledWidget->rect().topLeft()));
    blPoint->move(transPoint(controledWidget->rect().bottomLeft()));
    trPoint->move(transPoint(controledWidget->rect().topRight()));
    brPoint->move(transPoint(controledWidget->rect().bottomRight()));

    tmPoint->move( transPoint((controledWidget->rect().left()+ controledWidget->rect().right())/2,controledWidget->rect().top()));
    bmPoint->move( transPoint((controledWidget->rect().left()+ controledWidget->rect().right())/2,controledWidget->rect().bottom()));
    lmPoint->move( transPoint(controledWidget->rect().left(), (controledWidget->rect().bottom() + controledWidget->rect().top())/2));
    rmPoint->move( transPoint(controledWidget->rect().right(), (controledWidget->rect().top() + controledWidget->rect().bottom())/2));

    for(auto  it: ctrllist)
    {
        it->update_contrlwidget(controledWidget);
        it->enable_move(true);
        it->show();
    }
    controlenabled = true;
}

void MoveAbleFrame::setControlStyle(ControlType  ctrtype, bool ingroup, QString uuid)
{
    m_ctrtype = ctrtype;
    beingroup = ingroup;
    m_controledUuid = uuid;
}

void MoveAbleFrame::GetControlWidget(ControlType& ctrtype, QString& uuid, bool& ingroup)
{
    ctrtype = m_ctrtype;
    uuid =   m_controledUuid;
    ingroup = beingroup;
}

void MoveAbleFrame::update_ctrl_point_pos_2()
{
    if(g_controledWidget == nullptr) return;
    tlPoint->move( transPoint(g_controledWidget->rect().topLeft()));
    blPoint->move(transPoint(g_controledWidget->rect().bottomLeft()));
    trPoint->move(transPoint(g_controledWidget->rect().topRight()));
    brPoint->move(transPoint(g_controledWidget->rect().bottomRight()));

    tmPoint->move( transPoint((g_controledWidget->rect().left()+ g_controledWidget->rect().right())/2,g_controledWidget->rect().top()));
    bmPoint->move( transPoint((g_controledWidget->rect().left()+ g_controledWidget->rect().right())/2,g_controledWidget->rect().bottom()));
    lmPoint->move( transPoint(g_controledWidget->rect().left(), (g_controledWidget->rect().bottom() + g_controledWidget->rect().top())/2));
    rmPoint->move( transPoint(g_controledWidget->rect().right(), (g_controledWidget->rect().top() + g_controledWidget->rect().bottom())/2));
}
void MoveAbleFrame::CreateControl()
{
    ctrlsize = 10;
    tlPoint = new ControlPoint(15,ControlPoint::dTopLeft);
    tlPoint->setControlTyle(ControlPoint::cMoveCtrol);
    blPoint = new ControlPoint(ctrlsize,ControlPoint::dBottomLeft);
    trPoint = new ControlPoint(ctrlsize,ControlPoint::dTopRight);
    brPoint = new ControlPoint(ctrlsize,ControlPoint::dBottomRight);

    tmPoint = new ControlPoint(ctrlsize,ControlPoint::dTopMiddle);
    bmPoint = new ControlPoint(ctrlsize,ControlPoint::dBottomMiddle);
    lmPoint = new ControlPoint(ctrlsize,ControlPoint::dLeftMiddle);
    rmPoint = new ControlPoint(ctrlsize,ControlPoint::dRightMiddle);

    ctrllist.append(tlPoint);
    ctrllist.append(blPoint);
    ctrllist.append(trPoint);
    ctrllist.append(brPoint);
    ctrllist.append(tmPoint);
    ctrllist.append(bmPoint);
    ctrllist.append(lmPoint);
    ctrllist.append(rmPoint);
    for(auto  it: ctrllist)
        it->hide();
}
ConfigNameSpaceEnd
