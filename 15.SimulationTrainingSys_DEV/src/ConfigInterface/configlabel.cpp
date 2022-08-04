#include "configlabel.h"
#include <QUuid>
#include <QDrag>
#include <QMimeData>
#include <QDebug>
#include "moveableframe.h"
#include "configglobal.h"
ConfigNameSpaceStart
ConfigLabel::ConfigLabel(QWidget *parent, Qt::WindowFlags f):QLabel(parent,f)
{
    setStyleSheet("background-color:rgb(250,0,0)");
    m_uuid = get_uuid();

}

void ConfigLabel::setState(ControlOperateState state)
{
    m_state = state;
}

void ConfigLabel::setRole(ControlRole role)
{
    m_ctrlrole = role;
}
void ConfigLabel::setGroupId(QString groupid)
{
    m_groupid = groupid;
}

void ConfigLabel::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
            startPos = ev->pos();
 //   ConfigGlobal::gpropeetyset->setObject(cConfigLabel,this, m_valueSetMap);
    if(!m_ingroup)
         MoveAbleFrame::update_ctrl_point_pos(this,(QWidget*)parent());
  //  QLabel::mousePressEvent(ev);
}
void ConfigLabel::updateGeometryData()
{
//    QRect irect = geometry();
//    m_width = irect.width();
//    m_height = irect.height();
//    m_xPos  = irect.x();
//    m_yPos = irect.y();

//    *(int*)m_valueSetMap["位置属性"].valuelist[ConfigPairLabel::eXPos]->value  = m_xPos;
//    *(int*)m_valueSetMap["位置属性"].valuelist[ConfigPairLabel::eYPos]->value  = m_yPos;
//    *(int*)m_valueSetMap["位置属性"].valuelist[ConfigPairLabel::eWidth]->value  = m_width;
//    *(int*)m_valueSetMap["位置属性"].valuelist[ConfigPairLabel::eHeight]->value  = m_height;
}
void ConfigLabel::mouseMoveEvent(QMouseEvent *ev)
{
    if(m_ingroup)
    {
        if (ev->buttons() & Qt::LeftButton) {
           int distance = (ev->pos() - startPos).manhattanLength();
           if (distance >= QApplication::startDragDistance())
               handleDragEvent();
        }
    }
    QLabel::mouseMoveEvent(ev);

}

void ConfigLabel::moveEvent(QMoveEvent *event)
{
    if(!m_ingroup)
      MoveAbleFrame::update_ctrl_point_pos_2();
    QLabel::moveEvent(event);
}

void ConfigLabel::resizeEvent(QResizeEvent *event)
{
    if(!m_ingroup)
       MoveAbleFrame::update_ctrl_point_pos_2();
    QLabel::resizeEvent(event);
}
void ConfigLabel::handleDragEvent()
{
    QDrag *dg=new QDrag(this);
    //将需要拖动的数据放入QMimedata对象中，该对象用于保存需要传递的数据，数据的内
    //容完全由程序员自行设定。通常为界面上所选择内容。
    QMimeData *md=new QMimeData;
    md->setData("index",property("index").toByteArray());    			//这是md中存储的内容(即拖放时传递的数据)。
    md->setData("name","text");    			//这是md中存储的内容(即拖放时传递的数据)。
    md->setData("operation",QByteArray::number(m_ctrlrole));    			//这是md中存储的内容(即拖放时传递的数据)。
    md->setData("controltype",QByteArray::number(m_ctrltype));    			//这是md中存储的内容(即拖放时传递的数据)。
    md->setData("groupid",m_groupid.toLatin1());    			//这是md中存储的内容(即拖放时传递的数据)。

    dg->setMimeData(md);   			//步骤1：设置拖动的数据。该函数会获得md的所有权。
    dg->setPixmap(this->grab());
    dg->setHotSpot(QPoint(this->width()/2,this->height()/2));
    dg->exec(Qt::CopyAction | Qt::MoveAction);

}

ConfigNameSpaceEnd
