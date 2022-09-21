

#include "configscene.h"
#include <QMimeData>
#include <QDebug>
#include "configcurve.h"
#include "configalarm.h"
#include "booldialog.h"
#include "configtabbutton.h"
ConfigNameSpaceStart
ConfigScene::ConfigScene(QWidget *parent):QWidget(parent)
{
    m_sceneid = get_uuid();
    setAcceptDrops(true);
}
void ConfigScene::AutoCreateOpBtn(QString scenename)
{
    selbutton = new ConfigTabButton(scenename);
    selbutton->configscene = this;
    AddContextMenuToBtn(selbutton);
    delbutton = new ConfigTabButton();
    delbutton->configscene = this;
}
void ConfigScene::ButtonSwap(ConfigScene* swapScene)
{
    ConfigTabButton* iselbutton = selbutton;
    ConfigTabButton* idelbutton = delbutton;
    QString  swaptext = swapScene->selbutton->text();

    selbutton = swapScene->selbutton;
    selbutton->setText(iselbutton->text());
    selbutton->configscene = this;
    disconnect(selbutton, &QPushButton::customContextMenuRequested, 0, 0);
//    AddContextMenuToBtn(selbutton);
    delbutton = swapScene->delbutton;
    delbutton->configscene = this;

    swapScene->selbutton = iselbutton;
    swapScene->selbutton->setText(swaptext);
    disconnect(swapScene->selbutton, &QPushButton::customContextMenuRequested, 0, 0);
    swapScene->delbutton = idelbutton;
    swapScene->selbutton->configscene = swapScene;
    swapScene->delbutton->configscene = swapScene;    
    swapScene->selbutton->click();
}
void ConfigScene::AddContextMenuToBtn(QPushButton* selbtn)
{    
    selbutton->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(selbutton, &QPushButton::customContextMenuRequested, [=](const QPoint& pos)
        {
            QMenu  menu;
            QAction* action = menu.addAction("修改名称");
            connect(action, &QAction::triggered, [=]() {
                bool  ok;
                QString text = QInputDialog::getText(nullptr, "请输入软件名称", "软件名称", QLineEdit::Normal, QDir::home().dirName(), &ok);
                if (ok && !text.isEmpty())
                {
                    SetName(text);
                    selbutton->setText(text);
                    XmlStore::UpdateSceneNameToFile(this);
                }
                });
            action = menu.addAction("监测并使用控制流程");
            action->setCheckable(true);
            action->setChecked(userSchedule);
            connect(action, &QAction::triggered, [=]() {
                BoolDialog  dlg;
                dlg.setWindowTitle("是否使用控制流程");
                int result = dlg.exec();
                if (result == QDialog::Accepted)
                {
                    userSchedule = true;
                }
                else
                {
                    userSchedule = false;
                }
                XmlStore::UpdateSceneSchedule(this);
                SetScheduleEabled(userSchedule);
                });
            menu.exec(QCursor::pos());
        });
}
void ConfigScene::SetScheduleEabled(bool eabled)
{
    if (!eabled)
    {
        for (ConfigButton* btn : sceneButtonList)
        {
            btn->setDisabled(false);
        }
    }
    else
    {   
        for (ConfigButton* btn : sceneButtonList)
        {
            int btncmd = atoi(btn->m_CmdSourceID);
            if(ConfigGlobal::mainSchedule.contains(btncmd))
                btn->setDisabled(true);
        }
        for (GroupElement* group : sceneGroupList)
        {
            for (ConfigButton* btn : group->buttonlist)
            {
                int btncmd = atoi(btn->m_CmdSourceID);
                if (ConfigGlobal::mainSchedule.contains(btncmd))
                    btn->setDisabled(true);
            }
        }
        int  firstcmd = -1;
        for (int cmdid : ConfigGlobal::mainSchedule)
        {
            for (ConfigButton* btn : sceneButtonList)
            {
                int btncmd = atoi(btn->m_CmdSourceID);
                if ((firstcmd != -1) && (btncmd == firstcmd))
                {
                    btn->setDisabled(false);
                }
                if((firstcmd == -1)&&(btncmd == cmdid))
                {
                    btn->setDisabled(false);
                   // return;
                    firstcmd = cmdid;
                }
            }
            for (GroupElement* group : sceneGroupList)
            {
                for (ConfigButton* btn : group->buttonlist)
                {
                    int btncmd = atoi(btn->m_CmdSourceID);
                    if ((firstcmd != -1) && (btncmd == firstcmd))
                    {
                        btn->setDisabled(false);
                    }
                    if ((firstcmd == -1) && (btncmd == cmdid))
                    {
                        btn->setDisabled(false);
                        // return;
                        firstcmd = cmdid;
                    }
                   
                }
            }
            if (firstcmd != -1)
            {
                break;
            }

        }
    }
}
void ConfigScene::SetBtnScheduled(ConfigButton* curbtn)
{
    int curcmd = atoi(curbtn->m_CmdSourceID);
    bool  findcurrent = false;
    int findcmdid = -1;
    for (int cmdid : ConfigGlobal::mainSchedule)
    {
        for (ConfigButton* btn : sceneButtonList)
        {
            int btncmd = atoi(btn->m_CmdSourceID);
            if (btncmd == curcmd)
            {
                findcurrent = true;
                continue;
            }
            else if ((findcmdid != -1) && (findcmdid == cmdid))
            {
                btn->setDisabled(false);
            }
            else
            {
                if(findcurrent)
                { 
                    btn->setDisabled(false);
                    findcmdid = cmdid;
                   // return;
                }
            }
        }
        for (GroupElement* group : sceneGroupList)
        {
            for (ConfigButton* btn : group->buttonlist)
            {
                int btncmd = atoi(btn->m_CmdSourceID);
                if (btncmd == curcmd)
                {
                    findcurrent = true;
                    continue;
                }
                else if ((findcmdid != -1) && (findcmdid == cmdid))
                {
                    btn->setDisabled(false);
                }
                else
                {
                    if (findcurrent)
                    {
                        btn->setDisabled(false);
                        findcmdid = cmdid;
                       // return;
                    }
                }
            }
        }
        if (findcmdid != -1)
        {
            break;
        }
    }
}
void ConfigScene::RestoreSceneInfo(SceneInfo &sceneinfo)
{
    m_path = sceneinfo.scenepath;
    m_sceneName = sceneinfo.scenename;
    m_sceneid = sceneinfo.sceneid;
    userSchedule = sceneinfo.useschedule;
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
    for (auto& alarminfo : sceneinfo.alarmInfolist)
    {
        AddAlarmElement(alarminfo);
    }
    AutoCreateOpBtn(m_sceneName);
    SetScheduleEabled(userSchedule);
}

//int ConfigScene::RemoveGroupElement( QString elementid)
//{
//    for (auto element : sceneGroupList)
//    {
//        if (element->GetID() == elementid)
//        {
//
//        }
//    }
//    QList<GroupElement*>  sceneGroupList;
//    QList<ConfigButton*>  sceneButtonList;
//    QList<ConfigPairLabel*>  scenePairLabelList;
//    QList<ConfigCurve*>  sceneCurveList;
//}
//int ConfigScene::RemoveButtonElement(QString elementid)
//{
//    for (int i = 0; i < sceneButtonList.size() ;i++)
//    {
//        if (sceneButtonList[i]->GetID() == elementid)
//        {
//            sceneButtonList.removeAt(i);
//        }
//    }
//}
//int ConfigScene::RemovePairLabelElement(QString elementid)
//{
//    for (int i = 0; i < sceneButtonList.size(); i++)
//    {
//        if (sceneButtonList[i]->GetID() == elementid)
//        {
//            sceneButtonList.removeAt(i);
//        }
//    }
//}
//int ConfigScene::RemoveCurveElement(QString elementid)
//{
//    for (int i = 0; i < sceneButtonList.size(); i++)
//    {
//        if (sceneButtonList[i]->GetID() == elementid)
//        {
//            sceneButtonList.removeAt(i);
//        }
//    }
//}

int ConfigScene::DeleteElementByIdFromOut(ControlType ctrtype, QString elementid)
{
    if (ctrtype == cConfigGroup)
        return  RemoveElement<GroupElement>(elementid, sceneGroupList, ctrtype);
    else if (ctrtype == cConfigPairLabel)
        return  RemoveElement<ConfigPairLabel>(elementid, scenePairLabelList, ctrtype);
    else if (ctrtype == cConfigButton)
        return  RemoveElement<ConfigButton>(elementid, sceneButtonList, ctrtype);
    else if (ctrtype == cConfigCurve)
        return   RemoveElement<ConfigCurve>(elementid, sceneCurveList, ctrtype);
    else if (ctrtype == cConfigAlarm)
        return   RemoveElement<ConfigAlarm>(elementid, sceneAlarmList, ctrtype);
    else
        return -3;
}
void  ConfigScene::AddButtonElement(QMap<QString,QString>& buttoninfo)
{
    ConfigButton* btn = new ConfigButton("按钮",this);
    btn->m_scene = this;
    btn->InitFromXmlInfo(buttoninfo);
    btn->show();
    sceneButtonList.push_back(btn);
}
void ConfigScene::AddAlarmElement(QMap<QString, QString>& Alarminfo)
{
    ConfigAlarm* alarm = new ConfigAlarm(this);
    alarm->InitFromXmlInfo(Alarminfo);
    alarm->show();
    alarm->updateGeometryData();
    sceneAlarmList.push_back(alarm);
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
    groupElement->m_scene = this;
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
    ControlRole  ctrlrole = (ControlRole)event->mimeData()->data("operation").toInt();
    if (ctrlrole == cTabMove)
    {
        event->ignore();
        return;
    }
    QString groupid =  event->mimeData()->data("groupid");
  
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
void ConfigScene::CreateNewElement(ControlType  ctrltype,QPoint pos)
{
    if (ctrltype == cConfigGroup)
    {
        GroupElement* groupElement = new GroupElement(this);
        groupElement->m_scene = this;

        groupElement->InitFromDefaultStyle();
        groupElement->resize(500, 200);
        groupElement->move(pos);
        groupElement->show();
        groupElement->updateGeometryData();
        sceneGroupList.push_back(groupElement);

    }
    else if (ctrltype == cConfigButton)
    {
        ConfigButton* btn = new ConfigButton("按钮", this);
        btn->m_scene = this;
        btn->InitFromDefaultStyle();
        btn->resize(200, 50);
        btn->move(pos);
        btn->show();
        sceneButtonList.push_back(btn);


    }
    else if (ctrltype == cConfigLabel)
    {
        ConfigLabel* label = new ConfigLabel(this);
        label->resize(300, 20);
        label->setText("单独标签");
        label->move(pos);
        label->show();
    }
    else if (ctrltype == cConfigPairLabel)
    {
        ConfigPairLabel* label = new ConfigPairLabel(this);
        label->InitFromDefaultStyle();

        label->resize(300, 50);
        label->move(pos);
        label->show();
        label->updateGeometryData();
        scenePairLabelList.push_back(label);

    }
    else if (ctrltype == cConfigCurve)
    {
        ConfigCurve* curve = new ConfigCurve(this);
        curve->move(pos);
        curve->resize(500, 200);
        curve->show();
        curve->updateGeometryData();
        sceneCurveList.push_back(curve);

    }
    else if (ctrltype == cConfigAlarm)
    {
        ConfigAlarm* alarm = new ConfigAlarm(this);
        alarm->InitFromDefaultStyle();
        alarm->move(pos);
        alarm->resize(300, 100);
        alarm->show();
        alarm->updateGeometryData();
        sceneAlarmList.push_back(alarm);
    }
    update();

}
bool ConfigScene::GetSrcElementProperty(QString& outinput, ControlType itype, QMap<int, int>&  propertylist)
{
    QString srcid = outinput;
    for (auto group : sceneGroupList)
    {
        if (srcid == group->GetID())
        {
            propertylist = group->getPropertyList();
            outinput = group->getStyleId();
            return true;
        }
    }
    return false;
}
void ConfigScene::CopyElement(ControlType  ctrltype, QPoint pos,QString copyedid)
{
    if (ctrltype == cConfigGroup)
    {
        GroupElement* groupElement = new GroupElement(this);
        groupElement->m_scene = this;

        groupElement->InitFromDefaultStyle();
       
        groupElement->updateGeometryData();
        QMap<int, int> propertylist;
        QString outinput = copyedid;
        GetSrcElementProperty(outinput, ctrltype, propertylist);
        groupElement->copyProperty(outinput,propertylist);
        groupElement->moveToCenterPos(pos);
        groupElement->show();
        sceneGroupList.push_back(groupElement);
    }
  /*  else if (ctrltype == cConfigButton)
    {
        ConfigButton* btn = new ConfigButton("按钮", this);
        btn->InitFromDefaultStyle();
        btn->resize(200, 50);
        btn->move(pos);
        btn->show();
        sceneButtonList.push_back(btn);
    }
    else if (ctrltype == cConfigLabel)
    {
        ConfigLabel* label = new ConfigLabel(this);
        label->resize(300, 20);
        label->setText("单独标签");
        label->move(pos);
        label->show();
    }
    else if (ctrltype == cConfigPairLabel)
    {
        ConfigPairLabel* label = new ConfigPairLabel(this);
        label->InitFromDefaultStyle();

        label->resize(300, 50);
        label->move(pos);
        label->show();
        label->updateGeometryData();
        scenePairLabelList.push_back(label);

    }
    else if (ctrltype == cConfigCurve)
    {
        ConfigCurve* curve = new ConfigCurve(this);
        curve->move(pos);
        curve->resize(500, 200);
        curve->show();
        curve->updateGeometryData();
        sceneCurveList.push_back(curve);

    }*/
    //update();

}

void ConfigScene::dropEvent(QDropEvent *event)
{
    int index =    event->mimeData()->data("index").toInt();
    QByteArray name =    event->mimeData()->data("name");
    ControlRole  ctrlrole = (ControlRole)event->mimeData()->data("operation").toInt();
    ControlType  ctrltype = (ControlType)event->mimeData()->data("controltype").toInt();
    if (ctrlrole == cCreateEntry)
    {
        CreateNewElement(ctrltype, event->pos());
    }
    if (ctrlrole == cCopycontrol)
    {
        QString  srcgroupid =event->mimeData()->data("groupid");

        CopyElement(ctrltype, event->pos(), srcgroupid);
    }
 

}
ConfigNameSpaceEnd
