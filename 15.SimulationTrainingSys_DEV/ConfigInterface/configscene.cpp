

#include "configscene.h"
#include <QMimeData>
#include <QDebug>
#include "configcurve.h"

ConfigNameSpaceStart
ConfigScene::ConfigScene(QWidget *parent):QWidget(parent)
{
    m_sceneid = get_uuid();
    setAcceptDrops(true);
}
void ConfigScene::AutoCreateOpBtn(QString scenename)
{
    selbutton = new QPushButton(scenename);
    delbutton = new QPushButton();
}
void ConfigScene::RestoreSceneInfo(SceneInfo &sceneinfo)
{
    m_path = sceneinfo.scenepath;
    m_sceneName = sceneinfo.scenename;
    m_sceneid = sceneinfo.sceneid;
    for(auto& buttoninfo: sceneinfo.buttonInfolist)
    {
        AddButtonElement(buttoninfo);
    }
    for(auto& pairlabelinfo: sceneinfo.PairLabelInfolist)
    {
        AddPairLabelElement(pairlabelinfo);
    }
    for(auto& groupinfo: sceneinfo.groupinfolist)
    {
        AddGroupElement(groupinfo);
    }
    for(auto& curceinfo: sceneinfo.curveInfolist)
    {
        AddCurveElement(curceinfo);
    }
    AutoCreateOpBtn(m_sceneName);
}
void  ConfigScene::AddButtonElement(QMap<QString,QString>& buttoninfo)
{
    ConfigButton* btn = new ConfigButton("按钮",this);
    btn->InitFromXmlInfo(buttoninfo);
    btn->show();
    sceneButtonList.push_back(btn);
}
void  ConfigScene::AddPairLabelElement(QMap<QString,QString>& PairLabelinfo)
{
    ConfigPairLabel* label = new ConfigPairLabel(this);
    label->InitFromXmlInfo(PairLabelinfo);
    label->show();
    label->updateGeometryData();
    scenePairLabelList.push_back(label);
}
void  ConfigScene::AddGroupElement(GroupElementInfo& groupElementInfo)
{
    GroupElement*  groupElement = new GroupElement(this);
    groupElement->InitFromXmlInfo(groupElementInfo);
    groupElement->show();
    groupElement->updateGeometryData();
    sceneGroupList.push_back(groupElement);
}
void  ConfigScene::AddCurveElement(QMap<QString,QString>& curveinfo)
{
    ConfigCurve* curve = new ConfigCurve(this);
    curve->InitFromXmlInfo(curveinfo);
    curve->show();
 //   curve->updateGeometryData();
    sceneCurveList.push_back(curve);
}

QString ConfigScene::GetName()
{
    return  m_sceneName;
}

void ConfigScene::SetPath(QString path)
{
    m_path = path;
}

QString ConfigScene::GetPath()
{
    return m_path;
}

void ConfigScene::SetID(QString id)
{
    m_sceneid = id;
}

QString ConfigScene::GetID()
{
    return m_sceneid;
}
void ConfigScene::SetName(QString scenename)
{
    m_sceneName = scenename;
}
void ConfigScene::dragEnterEvent(QDragEnterEvent *event)
{
    QString groupid =  event->mimeData()->data("groupid");
    ControlRole  ctrlrole = (ControlRole)event->mimeData()->data("operation").toInt();

   // if(groupid == m_uuid || ctrlrole == cCreateEntry)
    event->accept();
}

void ConfigScene::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "ConfigScene::mousePressEvent";
    MoveAbleFrame::hide_ctrl_point();
    QWidget::mousePressEvent(event);
}

void ConfigScene::dragMoveEvent(QDragMoveEvent *event)
{
     event->accept();
}

void ConfigScene::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
    update();
}

void ConfigScene::dropEvent(QDropEvent *event)
{
    int index =    event->mimeData()->data("index").toInt();
    QByteArray name =    event->mimeData()->data("name");
    ControlRole  ctrlrole = (ControlRole)event->mimeData()->data("operation").toInt();
    ControlType  ctrltype = (ControlType)event->mimeData()->data("controltype").toInt();
    if(ctrltype == cConfigGroup)
    {
        GroupElement*  groupElement = new GroupElement(this);
        groupElement->InitFromDefaultStyle();
        groupElement->resize(500,200);
        groupElement->move(event->pos());
        groupElement->show();
        groupElement->updateGeometryData();
        sceneGroupList.push_back(groupElement);

    }
    else if(ctrltype == cConfigButton)
    {
       ConfigButton* btn = new ConfigButton("按钮",this);
       btn->InitFromDefaultStyle();
       btn->resize(200,50);
       btn->move(event->pos());
       btn->show();
       sceneButtonList.push_back(btn);


    }
    else if(ctrltype == cConfigLabel)
    {
        ConfigLabel* label = new ConfigLabel(this);
        label->resize(300,20);
        label->setText("单独标签");
        label->move(event->pos());
        label->show();
    }
    else if(ctrltype == cConfigPairLabel)
    {
        ConfigPairLabel* label = new ConfigPairLabel(this);
        label->InitFromDefaultStyle();

        label->resize(300,50);
        label->move(event->pos());
        label->show();
        label->updateGeometryData();
        scenePairLabelList.push_back(label);

    }
    else if(ctrltype == cConfigCurve)
    {
        ConfigCurve* curve = new ConfigCurve(this);
        curve->move(event->pos());
        curve->resize(500,200);
        curve->show();
        curve->updateGeometryData();
        sceneCurveList.push_back(curve);

    }
    update();

}
ConfigNameSpaceEnd
