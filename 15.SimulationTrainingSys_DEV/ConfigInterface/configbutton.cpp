﻿#include "configbutton.h"
#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <QUuid>
#include "configglobal.h"
#include "configtypeselector.h"
#include "moveableframe.h"
#include "propertyset.h"
#include "stylemanager.h"
ConfigNameSpaceStart
ConfigButton::ConfigButton(const QString &text, QWidget *parent):
    QPushButton(text,parent)
{
  //  DefaultUiInit();
    m_uuid = get_uuid();
    init_value_set();
}
void ConfigButton::DefaultUiInit()
{
    QString commstylesheet =  QString("QPushButton{ border-image: url(:/rc/normal.png);}");
    QString hoverstylesheet =  QString("QPushButton:hover{ border-image: url(:/rc/hover.png);}");
    QString pressedstylesheet =  QString("QPushButton:pressed{ border-image: url(:/rc/pressed.png);}");
    QString disabledstylesheet =  QString("QPushButton:disabled{ border-image: url(:/rc/forbit.png);}");

    normalstyle = commstylesheet + hoverstylesheet+ pressedstylesheet + disabledstylesheet;
    setStyleSheet(normalstyle);

    successstyle  = QString("QPushButton:pressed{ border-image: url(:/rc/forbit.png);}");
    failedstyle = QString("QPushButton:pressed{ border-image: url(:/rc/error.png);}");
}
void ConfigButton::InitFromDefaultStyle()
{
   bool bret =   UpdateStyleByStyleId("default");
   if (bret == false)
   {
       DefaultUiInit();
   }
}
void ConfigButton::InitFromDefineStyle(QString istyleid)
{
    if (UpdateStyleByStyleId(istyleid) == false)
    {
        InitFromDefaultStyle();
    }
}


void ConfigButton::updateDataSource(QString datasourceid, QString datasourcename, int  addordelete)
{
    setText(datasourcename);
}

void ConfigButton::init_value_set()
{

    {
          ConfigValueSet  m_valueSet;
          m_valueSet.valueSetname = "样式配置";
          m_valueSet.valuelist[ConfigButton::eStyleInfo] = new ConfigValue(new char[MaximumPathLength],eConfigStyleInfo,"样式选择");
          m_valueSetMap["样式配置"] = m_valueSet;
          m_valueSetMap["样式配置"].valuelist[ConfigButton::eStyleInfo]->SetWidthStrValue("未设置");
       /*   char* styleinfo =  (char*)m_valueSetMap["样式配置"].valuelist[ConfigButton::eStyleInfo]->value;
          memset(styleinfo,0,MaximumPathLength);
          memcpy(styleinfo,"",1);*/
    }
    {
          ConfigValueSet  m_valueSet;
          m_valueSet.valueSetname = "位置属性";
          m_valueSet.valuelist[ConfigButton::eXPos] = new ConfigValue(new int(5),eConfigInt,"X坐标");
          m_valueSet.valuelist[ConfigButton::eYPos] = new ConfigValue(new int(2),eConfigInt,"Y坐标");
          m_valueSet.valuelist[ConfigButton::eWidth] = new ConfigValue(new int(5),eConfigInt,"宽度");
          m_valueSet.valuelist[ConfigButton::eHeight] = new ConfigValue(new int(5),eConfigInt,"高度");
          m_valueSetMap["位置属性"] = m_valueSet;

    }
    {
          ConfigValueSet  m_valueSet;
          m_valueSet.valueSetname = "命令配置";
          m_valueSet.valuelist[ConfigButton::eDataSource] = new ConfigValue(new char[MaximumPathLength], eConfigCommand,"选择命令");
          m_valueSetMap["命令配置"] = m_valueSet;
          m_valueSetMap["命令配置"].valuelist[ConfigButton::eDataSource]->SetWidthStrValue("未设置");

         /* char* styleinfo =  (char*)m_valueSetMap["命令配置"].valuelist[ConfigButton::eConfigCommand]->value;
          memset(styleinfo,0,MaximumPathLength);
          memcpy(styleinfo,"",1);*/
    }
}
void ConfigButton::updateGeometryData()
{
    QRect irect = geometry();
    int m_width = irect.width();
    int m_height = irect.height();
    int m_xPos  = irect.x();
    int m_yPos = irect.y();

    *(int*)m_valueSetMap["位置属性"].valuelist[ConfigButton::eXPos]->value  = m_xPos;
    *(int*)m_valueSetMap["位置属性"].valuelist[ConfigButton::eYPos]->value  = m_yPos;
    *(int*)m_valueSetMap["位置属性"].valuelist[ConfigButton::eWidth]->value  = m_width;
    *(int*)m_valueSetMap["位置属性"].valuelist[ConfigButton::eHeight]->value  = m_height;
}
ConfigButton::~ConfigButton()
{
    qDebug() <<  "~ConfigButton";
}

void ConfigButton::InitFromXmlInfo(QMap<QString, QString> &buttoninfo)
{
    m_uuid = buttoninfo["uuid"];
    for(auto  groupkey:m_valueSetMap.keys())
    {
        for(auto elementkey: m_valueSetMap[groupkey].valuelist.keys())
        {
            QString infokey = groupkey + ":" + QString::number(elementkey);
            QString strvalue  = buttoninfo[infokey];
            m_valueSetMap[groupkey].valuelist[elementkey]->SetWidthStrValue(strvalue);
            qDebug() <<"infokey:" << infokey << " value:" <<m_valueSetMap[groupkey].valuelist[elementkey]->getStrValue() << " strvalue:" <<strvalue;
        }
    }
    QString datasourceid = m_valueSetMap["命令配置"].valuelist[ConfigButton::eDataSource]->getStrValue();
    if (ConfigGlobal::VerifyNumber(datasourceid))
    {
        if ((*ConfigGlobal::m_allCommadPrt).find(datasourceid.toInt()) != (*ConfigGlobal::m_allCommadPrt).end())
        {
            string showtext = (*ConfigGlobal::m_allCommadPrt)[datasourceid.toInt()]->m_sName;
            datasourceid = QString::fromLocal8Bit(showtext.c_str());
            setText(datasourceid);
        }
    }

    updataDataFromTool();
   QString  istyleid =  m_valueSetMap["样式配置"].valuelist[ConfigButton::eStyleInfo]->getStrValue();
   InitFromDefineStyle(istyleid);
}

void ConfigButton::setState(ControlOperateState state)
{
    m_state = state;
}
void ConfigButton::setRole(ControlRole role)
{
    m_ctrlrole = role;
}

void ConfigButton::setGroupId(QString groupid)
{
    m_groupid = groupid;
}

QString ConfigButton::GetID()
{
    return m_uuid;
}

void ConfigButton::mousePressEvent(QMouseEvent *ev)
{
    if(!ConfigGlobal::isEditing)
    {
       QPushButton::mousePressEvent(ev);
       return;
    }
    qDebug() << "configbutton press:"  << m_valueSetMap["命令配置"].valuelist[ConfigButton::eDataSource]->getStrValue();
    if (ev->button() == Qt::LeftButton)
            startPos = ev->pos();

    updateGeometryData();
    ConfigGlobal::gpropeetyset->setObject(cConfigButton,this, m_valueSetMap);
    if(!m_ingroup)
      MoveAbleFrame::update_ctrl_point_pos(this,(QWidget*)parent());
   if(m_ingroup)
     ev->ignore();

}

void ConfigButton::mouseMoveEvent(QMouseEvent *e)
{
    if(m_ingroup)
    {
        if (e->buttons() & Qt::LeftButton) {
           int distance = (e->pos() - startPos).manhattanLength();
           if (distance >= QApplication::startDragDistance())
               handleDragEvent();
        }
    }

    QPushButton::mouseMoveEvent(e);
}
void ConfigButton::updataDataFromTool()
{
    m_xPos   = *(int*)m_valueSetMap["位置属性"].valuelist[ConfigButton::eXPos]->value;
    m_yPos   = *(int*)m_valueSetMap["位置属性"].valuelist[ConfigButton::eYPos]->value;
    m_width  = *(int*)m_valueSetMap["位置属性"].valuelist[ConfigButton::eWidth]->value;
    m_height = *(int*)m_valueSetMap["位置属性"].valuelist[ConfigButton::eHeight]->value;
    setGeometry(m_xPos,m_yPos,m_width,m_height);
}
void ConfigButton::moveEvent(QMoveEvent *event)
{
    updateGeometryData();
    ConfigGlobal::gpropeetyset->UpdateDataFromObject(this);

    if(!m_ingroup)
        MoveAbleFrame::update_ctrl_point_pos_2();
    QPushButton::moveEvent(event);
}

void ConfigButton::resizeEvent(QResizeEvent *event)
{
    updateGeometryData();
    ConfigGlobal::gpropeetyset->UpdateDataFromObject(this);

    if(!m_ingroup)
        MoveAbleFrame::update_ctrl_point_pos_2();
    QPushButton::resizeEvent(event);
}
bool ConfigButton::SelectStyle(StyleButton *& istyleinfo)
{
    QList<StyleButton *>& stylebuttons  =   ConfigGlobal::gstylemanager->getButtonStyle();
    ConfigTypeSelector  selector;
    selector.UpdateTyleInfo(stylebuttons);
    if(selector.exec() == QDialog::Rejected)
        return false;
    int styleindex =  selector.getResult();
    qDebug() << "styleindex" <<styleindex;
    m_styleinfo = stylebuttons.at(styleindex);
    m_styleinfo->record_styleusers(this);
    istyleinfo = m_styleinfo;
    UpdatePropertyByStyle();
    return true;
}
bool ConfigButton::UpdateStyleByStyleId(QString styleid)
{
    bool befind = false;
    m_styleinfo = nullptr;
    QList<StyleButton *>& stylebuttons  =   ConfigGlobal::gstylemanager->getButtonStyle();
    for(StyleButton * stylebtn: stylebuttons)
    {
        if(stylebtn->m_infomap[StyleButton::Button_Style_ID].second == styleid)
            m_styleinfo = stylebtn;
    }
     if(m_styleinfo)
     {
         befind = true;
        m_styleinfo->record_styleusers(this);
        UpdatePropertyByStyle();
     }
    return befind;
}

void ConfigButton::UpdatePropertyByStyle()
{
    QMap<int,QPair<bool,QString>> & m_infomap = m_styleinfo->m_infomap;
    QString icommonstyle,ipressedstyle,ihoverdstyle,disabledstyle;
    QFont m_textfont;
    QColor m_textcolor;

    if(m_infomap.contains(StyleButton::Button_Style_ID))
        m_styleinfoId = m_infomap[StyleButton::Button_Style_ID].second;
    if(m_infomap.contains(StyleButton::Button_Style_Name))
        m_styleinfoname = m_infomap[StyleButton::Button_Style_Name].second;
    if(m_infomap.contains(StyleButton::Button_Common_Style))
        icommonstyle = m_infomap[StyleButton::Button_Common_Style].second;
    if(m_infomap.contains(StyleButton::Button_Hoverd_Style))
        ihoverdstyle = m_infomap[StyleButton::Button_Hoverd_Style].second;
    if(m_infomap.contains(StyleButton::Button_Pressed_Style))
        ipressedstyle = m_infomap[StyleButton::Button_Pressed_Style].second;
    if(m_infomap.contains(StyleButton::Button_Diable_Style))
        disabledstyle = m_infomap[StyleButton::Button_Diable_Style].second;
    if(m_infomap.contains(StyleButton::Button_Success_Style))
        successstyle = m_infomap[StyleButton::Button_Success_Style].second;
    if(m_infomap.contains(StyleButton::Button_Failed_Style))
        failedstyle = m_infomap[StyleButton::Button_Failed_Style].second;


    if(m_infomap.contains(StyleButton::Button_Font_Style))
    {
        m_textfont.fromString(m_infomap[StyleButton::Button_Font_Style].second);
    }
    if(m_infomap.contains(StyleButton::Button_Text_Color))
    {
        m_textcolor.setNamedColor(m_infomap[StyleButton::Button_Text_Color].second);
    }

    QString stylecolor =  QString("color:rgb(%1,%2,%3);").arg(m_textcolor.red()).arg(m_textcolor.green()).arg(m_textcolor.blue());

    QString commstylesheet =  QString( "QPushButton{ border-image: url(%1);%2}").arg(icommonstyle).arg(stylecolor);
    QString hoverstylesheet =  QString( "QPushButton:hover{ border-image: url(%1);}").arg(ihoverdstyle);
    QString pressedstylesheet =  QString( "QPushButton:pressed{ border-image: url(%1);}").arg(ipressedstyle);
    QString disabledstylesheet =  QString( "QPushButton:disabled{ border-image: url(%1);}").arg(disabledstyle);

    normalstyle = commstylesheet + hoverstylesheet+ pressedstylesheet+ disabledstylesheet;
    setStyleSheet(normalstyle);
    setFont(m_textfont);

//    char* styleidinfo =  (char*)m_valueSetMap["样式配置"].valuelist[ConfigButton::eStyleInfo]->value;
//    memset(styleidinfo,0,MaximumPathLength);
//    memcpy(styleidinfo,m_styleinfoId.toStdString().c_str(),strlen(m_styleinfoId.toStdString().c_str()));

}
void ConfigButton::handleDragEvent()
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
