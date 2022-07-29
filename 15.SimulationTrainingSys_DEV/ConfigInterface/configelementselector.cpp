#include "configelementselector.h"

#include <QDrag>
#include <QMimeData>
ConfigNameSpaceStart
ConfigElementSelector::ConfigElementSelector(QWidget *parent):QPushButton(parent)
{
    setMaximumWidth(100);
}
void ConfigElementSelector::handleDragEvent()
{
    QDrag *dg=new QDrag(this);
    //将需要拖动的数据放入QMimedata对象中，该对象用于保存需要传递的数据，数据的内
    //容完全由程序员自行设定。通常为界面上所选择内容。
    QMimeData *md=new QMimeData;

    md->setData("index","1");    			//这是md中存储的内容(即拖放时传递的数据)。
    md->setData("name","text");    			//这是md中存储的内容(即拖放时传递的数据)。
    md->setData("operation",QByteArray::number(m_ctrlrole));    			//这是md中存储的内容(即拖放时传递的数据)。
    md->setData("controltype",QByteArray::number(m_ctrltype));    			//这是md中存储的内容(即拖放时传递的数据)。
    md->setData("groupid","");    			//这是md中存储的内容(即拖放时传递的数据)。


    dg->setMimeData(md);   			//步骤1：设置拖动的数据。该函数会获得md的所有权。
    dg->setPixmap(this->grab());
    dg->setHotSpot(QPoint(this->width()/2,this->height()/2));
    dg->exec(Qt::CopyAction | Qt::MoveAction);

}
void ConfigElementSelector::setSelectorInfo(ControlType ctrtype,QString name)
{
    m_ctrltype = ctrtype;
    m_ctrlrole = cCreateEntry;
    setText(name);
}

void ConfigElementSelector::mousePressEvent(QMouseEvent *e)
{
    handleDragEvent();
    QWidget::mousePressEvent(e);
}
ConfigNameSpaceEnd
