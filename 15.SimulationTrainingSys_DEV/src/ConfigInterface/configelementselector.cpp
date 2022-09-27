#include "configelementselector.h"

#include <QDrag>
#include <QMimeData>
#include "configbutton.h"
#include     "configcurve.h"
#include  "groupelement.h"
#include "configpairlabel.h"
#include "configalarm.h"
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
    dg->setPixmap(getElementMap(m_ctrltype));
    dg->setHotSpot(QPoint(this->width()/2,this->height()/2));
    dg->exec(Qt::CopyAction | Qt::MoveAction);

}
QPixmap ConfigElementSelector::getElementMap(ControlType ctrtyle)
{
    /*
        cConfigLabel,
    cConfigButton,
    cConfigCurve,
    cConfigGroup,
    cConfigPairLabel,
    cConfigAlarm
    */   
    if (ctrtyle == cConfigButton)
    {
        ConfigButton btn ("按钮", nullptr);
        btn.setStyleSheet("background-color:transparent;");
        btn.InitFromDefaultStyle();
        btn.resize(200, 50);       
        return btn.grab();
    }
    else if (ctrtyle == cConfigCurve)
    {
        ConfigCurve curve;
        curve.resize(500, 200);
        return curve.grab();
    }
    else if (ctrtyle == cConfigGroup)
    {
        GroupElement groupElement;
        groupElement.setStyleSheet("background-color:transparent;");
        groupElement.InitFromDefaultStyle();
        groupElement.resize(500, 200);
        return groupElement.grab();
    }
    else if (ctrtyle == cConfigPairLabel)
    {
        ConfigPairLabel  label;
        label.setStyleSheet("background-color:transparent;");
        label.InitFromDefaultStyle();
        label.resize(300, 50);
        return label.grab();
    }
    else if (ctrtyle == cConfigAlarm)
    {
        ConfigAlarm alarm;
        alarm.setStyleSheet("background-color:transparent;");
        alarm.InitFromDefaultStyle();
        alarm.resize(300, 100);
        return alarm.grab();
    }
    return QPixmap();
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
