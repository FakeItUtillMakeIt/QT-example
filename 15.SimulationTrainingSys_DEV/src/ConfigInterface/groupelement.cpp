
#include "configtypeselector.h"
#include "groupelement.h"
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QMimeData>
#include <QPainter>
#include <QUuid>
#include <QImage>
#include "stylemanager.h"
#include "propertyset.h"
#include <qdrag.h>
#include <qmimedata.h>
ConfigNameSpaceStart
GroupElement::GroupElement(QWidget *parent) : QWidget(parent)
{
    memset(titletext, 0, MaximumPathLength);
    memset(m_styleID, 0, MaximumPathLength);

   this->setAttribute(Qt::WA_StyledBackground, true);
 //   DefaultUiInit();
   setAcceptDrops(true);
  // setStyleSheet("background-color:rgb(0,0,250,50)");
   m_uuid = get_uuid();
   deleteicon = QImage("://rc/delete_normal.png");
   deleteiconpressed = QImage("://rc/delete_pressed.png");
   setMouseTracking(true);
   resize(500,200);
   init_value_set();
  // updateFromValueSet();
}

void GroupElement::DefaultUiInit()
{
    m_backgroundpicture = QPixmap(":/rc/background.png");
    m_titlepicture =  QPixmap(":/rc/grouptitle.png");
    QFont font  = this->font();
    font.setPointSizeF(15);
    
    m_titlefont = font;
    m_titlecolor = QColor(0,0,0);
    m_iconpicture = QPixmap(":/rc/icon.png");
}

void GroupElement::InitFromDefaultStyle()
{
    bool bret = UpdateStyleByStyleId("default");
    if (bret == false)
    {
        DefaultUiInit();
    }
}
void GroupElement::InitFromDefineStyle(QString istyleid)
{
    if (UpdateStyleByStyleId(istyleid) == false)
    {
        InitFromDefaultStyle();
    }
}

void GroupElement::drawIconEntry(QPainter &painter)
{
    QRect iconrect = QRect(m_titleheight/4,m_titleheight/4,m_titleheight ,m_titleheight/2);
    painter.drawPixmap(iconrect,m_iconpicture);
}
void GroupElement::InitFromXmlInfo(GroupElementInfo& groupElementInfo)
{
    m_uuid = groupElementInfo.groupinfo["uuid"];
    for(auto  groupkey:m_valueSetMap.keys())
    {
        for(auto elementkey: m_valueSetMap[groupkey].valuelist.keys())
        {
            QString infokey = groupkey + ":" + QString::number(elementkey);
            QString strvalue  = groupElementInfo.groupinfo[infokey];
            m_valueSetMap[groupkey].valuelist[elementkey]->SetWidthStrValue(strvalue);
            qDebug() <<"groupkey:" << groupkey << " :elementkey:" << elementkey << " :value:" << m_valueSetMap[groupkey].valuelist[elementkey]->getStrValue() << ":strvalue" <<strvalue;
        }
    }
    updataDataFromTool();
    QString  istyleid =  m_valueSetMap["样式配置"].valuelist[GroupElement::eStyleInfo]->getStrValue();
    InitFromDefineStyle(istyleid);

    RestoreButtonFromXml(groupElementInfo);
    RestorePairLabelFromXml(groupElementInfo);
}
void GroupElement::RestoreButtonFromXml(GroupElementInfo& groupElementInfo)
{
    QList<QMap<QString,QString>>&  buttonInfoList = groupElementInfo.buttonInfolist;
    for(auto& buttoninfo: buttonInfoList)
    {
        AddButtonElement(buttoninfo);
    }

}
void GroupElement::RestorePairLabelFromXml(GroupElementInfo& groupElementInfo)
{

    QList<QMap<QString,QString>>&  pairLabelInfoList = groupElementInfo.PairLabelInfolist;
    for(auto& pairlabelInfo: pairLabelInfoList)
    {
        AddPairLabelElement(pairlabelInfo);
    }
}

GroupElement::RectInfo&  GroupElement::GetRectInfoByGeometry(QRect rect)
{
    GroupElement::RectInfo  rectinfo;
    if(rectmap.size() == 0)  return rectinfo;
    for(int key: rectmap.keys())
    {
        if(rectmap[key].rect.contains( rect.center()))
        {
            return rectmap[key];
        }
    }
    return  rectinfo;
}
void  GroupElement::AddButtonElement(QMap<QString,QString>& buttoninfo)
{
    ConfigButton* btn = new ConfigButton("按钮",this);
    buttonlist.push_back(btn);
    btn->m_ingroup = true;
    btn->setRole(cRealControl);
    btn->setGroupId(m_uuid);
    btn->InitFromXmlInfo(buttoninfo);
    RectInfo&  rectinfo = GetRectInfoByGeometry(btn->geometry());
    btn->setProperty("index",rectinfo.index);
    btn->show();
    widgetmap[rectinfo.index]= btn;
    widgetmap[rectinfo.index]->setProperty("type", cConfigButton);
}

void  GroupElement::AddPairLabelElement(QMap<QString,QString>& pairLabelInfo)
{
    ConfigPairLabel*  pairlabel  = new ConfigPairLabel(this);
    pairlabellist.push_back(pairlabel);
    pairlabel->m_ingroup = true;
    pairlabel->setRole(cRealControl);
    pairlabel->setGroupId(m_uuid);
    pairlabel->InitFromXmlInfo(pairLabelInfo);
    RectInfo&  rectinfo = GetRectInfoByGeometry(pairlabel->geometry());
    pairlabel->setProperty("index",rectinfo.index);
    pairlabel->show();
    widgetmap[rectinfo.index]= pairlabel;
    widgetmap[rectinfo.index]->setProperty("type", cConfigPairLabel);

}



bool GroupElement::UpdateStyleByStyleId(QString styleid)
{
    bool  bfind = false;
    m_styleinfo = nullptr;
    QList<StyleGroup *>& stylegroups  =   ConfigGlobal::gstylemanager->getGroupStyle();
    for(StyleGroup * stylegroup: stylegroups)
    {
        if(stylegroup->m_infomap[StyleGroup::Group_Style_ID].second == styleid)
            m_styleinfo = stylegroup;
    }
     if(m_styleinfo)
     {
        m_styleinfo->record_styleusers(this);
        updateFromValueSet();

        updataDataFromTool();
      //  UpdatePropertyByStyle();
        bfind = true;
     }
    return bfind;
}

QString GroupElement::GetID()
{
    return m_uuid;
}

QList<ConfigPairLabel *> &GroupElement::getPairLabelList()
{
   return pairlabellist;
}

QList<ConfigButton *> &GroupElement::getButtonList()
{
    return buttonlist;
}
QMap<QString, ConfigValueSet> &GroupElement::getPropertyMap()
{
    return m_valueSetMap;
}
void GroupElement::dragEnterEvent(QDragEnterEvent *event)
{
    QString groupid =  event->mimeData()->data("groupid");
    ControlRole  ctrlrole = (ControlRole)event->mimeData()->data("operation").toInt();
    ControlType  ctrltype = (ControlType)event->mimeData()->data("controltype").toInt();

    if((groupid == m_uuid || ctrlrole == cCreateEntry)&&(ctrltype != cConfigGroup) && (ctrltype != cConfigCurve))
        event->accept();
}

void GroupElement::dragMoveEvent(QDragMoveEvent *event)
{
    for(int key: rectmap.keys())
    {
        if(rectmap[key].rect.contains( event->pos()))
        {
            rectmap[key].show = true;
        }else
        {
            rectmap[key].show = false;
        }
    }
    update();
    event->accept();

}

void GroupElement::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
    for(int key: rectmap.keys())
    {
        rectmap[key].show = false;
    }
    update();

}

void GroupElement::removeWidgetFromList(QWidget* widget)
{
    int ctrltype = widget->property("type").toInt();
    switch (ctrltype) {
    case cConfigButton:
        for (int i = 0;i < buttonlist.size(); i++)
        {
            if (buttonlist[i] == widget)
            {
                buttonlist.removeAt(i);
                break;
            }
        }
        break;
   
    case cConfigPairLabel:
        for (int i = 0; i < pairlabellist.size(); i++)
        {
            if (pairlabellist[i] == widget)
            {
                pairlabellist.removeAt(i);
                break;
            }
        }
        break;
    default:
        break;
    }

}
void GroupElement::delSelectedElements()
{
    for(int index: rectmap.keys())
    {
        if(widgetmap.contains(index)&& rectmap[index].selected)
        {        
            removeWidgetFromList(widgetmap[index]);
            rectmap[index].selected = false;
            QWidget* delwid = widgetmap.take(index);
            delete delwid;
            delwid = nullptr;
        }
    }

}

bool GroupElement::SelectStyle(StyleGroup *& istylegroup)
{
    QList<StyleGroup *>& stylegroup  =   ConfigGlobal::gstylemanager->getGroupStyle();
    ConfigTypeSelector  selector;
    selector.UpdateTyleInfo(stylegroup);
    if(selector.exec() == QDialog::Rejected)
        return false;
    int styleindex =  selector.getResult();
    m_styleinfo = stylegroup.at(styleindex);
    m_styleinfo->record_styleusers(this);
    UpdatePropertyByStyle();
    istylegroup = m_styleinfo;
    return true;
}

void GroupElement::UpdatePropertyByStyle()
{
    if(m_styleinfo == nullptr) return;
    m_backgroundpicture = QPixmap(m_styleinfo->m_infomap[StyleGroup::Group_Common_Style].second);
    m_titlepicture =  QPixmap(m_styleinfo->m_infomap[StyleGroup::Group_Title_Style].second);
    m_titlefont.fromString(m_styleinfo->m_infomap[StyleGroup::Group_Font_Style].second);
    m_titlecolor.setNamedColor(m_styleinfo->m_infomap[StyleGroup::Group_Text_Color].second);
    m_iconpicture = QPixmap(m_styleinfo->m_infomap[StyleGroup::Group_Icon_Style].second);
    update();
}

void GroupElement::init_value_set()
{
    {
          ConfigValueSet  m_valueSet;
          m_valueSet.valueSetname = "样式配置";
          m_valueSet.valuelist[GroupElement::eStyleInfo] = new ConfigValue(m_styleID,eConfigStyleInfo,"样式选择");
          m_valueSet.valuelist[GroupElement::eTitleInfo] = new ConfigValue(titletext,eConfigString,"标题设置");
          m_valueSetMap["样式配置"] = m_valueSet;
          m_valueSetMap["样式配置"].valuelist[GroupElement::eStyleInfo]->SetWidthStrValue("default");
          m_valueSetMap["样式配置"].valuelist[GroupElement::eTitleInfo]->SetWidthStrValue("未设置");

     /*     char* styleinfo =  (char*)m_valueSetMap["样式配置"].valuelist[GroupElement::eStyleInfo]->value;
          memset(styleinfo,0,MaximumPathLength);
          memcpy(styleinfo,"",1);*/
    //      char* titleinfo =  (char*)m_valueSetMap["样式配置"].valuelist[GroupElement::eTitleInfo]->value;
    //      memset(titleinfo,0,MaximumPathLength);
    //      memcpy(titleinfo,titletext.toLocal8Bit().data(),strlen(titletext.toLocal8Bit().data()));
    }
    {
          ConfigValueSet  m_valueSet;
          m_valueSet.valueSetname = "位置属性";
          m_valueSet.valuelist[GroupElement::eXPos] = new ConfigValue(&m_xPos,eConfigInt,"X坐标");
          m_valueSet.valuelist[GroupElement::eYPos] = new ConfigValue(&m_yPos,eConfigInt,"Y坐标");
          m_valueSet.valuelist[GroupElement::eWidth] = new ConfigValue(&m_width,eConfigInt,"宽度");
          m_valueSet.valuelist[GroupElement::eHeight] = new ConfigValue(&m_height,eConfigInt,"高度");
          m_valueSetMap["位置属性"] = m_valueSet;

    }

    {
          ConfigValueSet  m_valueSet;
          m_valueSet.valueSetname = "布局配置";          
          m_valueSet.valuelist[GroupElement::eRowValue] = new ConfigValue(&m_row,eConfigInt,"行数");
          m_valueSet.valuelist[GroupElement::eColValue] = new ConfigValue(&m_col ,eConfigInt,"列数");
          m_valueSet.valuelist[GroupElement::eRowSpace] = new ConfigValue(&m_rowspace,eConfigInt,"行间隙");
          m_valueSet.valuelist[GroupElement::eColSpace] = new ConfigValue(&m_colspace,eConfigInt,"列间隙");
          m_valueSet.valuelist[GroupElement::eRowSpan] = new ConfigValue(&m_rowspan,eConfigInt,"行高");

          //m_valueSet.valuelist[GroupElement::eColSpan] = new ConfigValue(new int(100),eConfigInt,"列宽");
          m_valueSet.valuelist[GroupElement::eLeftMargin] = new ConfigValue(&m_leftmagin,eConfigInt,"左间距");
          m_valueSet.valuelist[GroupElement::eRightMargin] = new ConfigValue(&m_rightmargin,eConfigInt,"右间距");
          m_valueSet.valuelist[GroupElement::eTopMargin] = new ConfigValue(&m_topmagin,eConfigInt,"标题栏与内容间距");
          m_valueSet.valuelist[GroupElement::eTitleHeight] = new ConfigValue(&m_titleheight,eConfigInt,"标题栏高度");
          m_valueSetMap["布局配置"] = m_valueSet;

    }
}
QMap<int, int>& GroupElement::getPropertyList()
{
    m_propertylist[eWidth] = m_width;
    m_propertylist[eHeight] = m_height;
    m_propertylist[eRowSpan] = m_rowspan;
    m_propertylist[eColSpace] = m_colspace;
    m_propertylist[eRowSpace] = m_rowspace;
    m_propertylist[eLeftMargin] = m_leftmagin;
    m_propertylist[eRightMargin] = m_rightmargin;
    m_propertylist[eTopMargin] = m_topmagin;
    m_propertylist[eTitleHeight] = m_titleheight;
    m_propertylist[eRowValue] = m_row  ;
    m_propertylist[eColValue] = m_col;
    return  m_propertylist;
}
QString  GroupElement::getStyleId()
{
    return  m_valueSetMap["样式配置"].valuelist[GroupElement::eStyleInfo]->getStrValue();
}

void GroupElement::copyProperty(QString styleid,QMap<int,int>&  propertylist)
{
    m_valueSetMap["样式配置"].valuelist[GroupElement::eStyleInfo]->SetWidthStrValue(styleid);

    m_width = propertylist[eWidth];
    m_height = propertylist[eHeight];
    m_row = propertylist[eRowValue];
    m_col = propertylist[eColValue];
    m_rowspan = propertylist[eRowSpan];
    m_colspace = propertylist[eColSpace];
    m_rowspace = propertylist[eRowSpace];
    m_leftmagin = propertylist[eLeftMargin];
    m_rightmargin = propertylist[eRightMargin];
    m_topmagin = propertylist[eTopMargin];
    m_titleheight = propertylist[eTitleHeight];
    resetLaoutFromData();
    UpdateStyleByStyleId(m_styleID);
    update();
}

void GroupElement::updateFromValueSet()
{

   // m_row  =   *(int *)m_valueSetMap["布局配置"].valuelist[eRowValue]->value;
   // m_col  =   *(int *)m_valueSetMap["布局配置"].valuelist[eColValue]->value;
   // m_rowspace  =   *(int *)m_valueSetMap["布局配置"].valuelist[eRowSpace]->value;
   // m_colspace  =   *(int *)m_valueSetMap["布局配置"].valuelist[eColSpace]->value;
   // m_rowspan  =   *(int *)m_valueSetMap["布局配置"].valuelist[eRowSpan]->value;
   //// m_colspan  =   *(int *)m_valueSetMap["布局配置"].valuelist[eColSpan]->value;
   // m_leftmagin  =   *(int *)m_valueSetMap["布局配置"].valuelist[eLeftMargin]->value;
   // m_rightmargin  =   *(int *)m_valueSetMap["布局配置"].valuelist[eRightMargin]->value;
   // m_topmagin  =   *(int *)m_valueSetMap["布局配置"].valuelist[eTopMargin]->value;
   // m_titleheight    = *(int *)m_valueSetMap["布局配置"].valuelist[eTitleHeight]->value;
   // titletext.clear();
   // titletext =  m_valueSetMap["样式配置"].valuelist[eTitleInfo]->getStrValue();
   // qDebug() << "updateFromValueSet:" << titletext;
}

void GroupElement::addElementBack(RectInfo&  rectinfo,QString text,ControlType ctrltype)
{
    QWidget*  newctrl = nullptr;
    newctrl = ctrlfactory->CreateControl(ctrltype,cRealControl,text,this,m_uuid);
    newctrl->setProperty("index",rectinfo.index);
    newctrl->setGeometry(rectinfo.rect);
    newctrl->show();
    rectinfo.show = false;
    widgetmap[rectinfo.index]= newctrl;
    update();
}

void GroupElement::addElement(RectInfo&  rectinfo,QString text,ControlType ctrltype)
{
    if (widgetmap.contains(rectinfo.index))
    {
        return;
    }

    QWidget* newwidget = nullptr;
    ConfigButton* btn = nullptr;
    ConfigLabel* label = nullptr;
    ControlRole ctrlrole = cRealControl;
    ConfigPairLabel* pairlabel = nullptr;
    QString groupuuid = m_uuid;
    QWidget* parent = this;
    switch (ctrltype) {
    case cConfigButton:
        btn  = new ConfigButton("按钮",parent);
        btn->InitFromDefaultStyle();
        buttonlist.push_back(btn);
        btn->m_ingroup = true;
        btn->setRole(ctrlrole);
        btn->setGroupId(groupuuid);
        newwidget = btn;
        break;
    case cConfigLabel:
        label = new ConfigLabel(parent);
        label->m_ingroup = true;

        label->setGroupId(groupuuid);
        label->setText("单独标签");
        label->setRole(ctrlrole);
        newwidget = label;
        break;
    case cConfigPairLabel:
        pairlabel  = new ConfigPairLabel(parent);
        pairlabel->InitFromDefaultStyle();
        pairlabellist.push_back(pairlabel);
        pairlabel->m_ingroup = true;
        pairlabel->setRole(ctrlrole);
        pairlabel->setGroupId(groupuuid);
        newwidget = pairlabel;
        break;
    default:
        newwidget = new QWidget(parent);
        break;
    }
    newwidget->setProperty("type", ctrltype);
    newwidget->setProperty("index",rectinfo.index);
    newwidget->setGeometry(rectinfo.rect);
    newwidget->show();
    rectinfo.show = false;
    widgetmap[rectinfo.index]= newwidget;
    update();

}

void GroupElement::moveElement(RectInfo&  rectinfo,int curindex)
{
    //判断当前位置有没有 控件，有则互换位置
    if(widgetmap.contains(rectinfo.index))  //替换
    {
        QWidget* widget = widgetmap[curindex]; //获取被移动widget
        QWidget* placedwidget = widgetmap[rectinfo.index];//获取鼠标悬停位置的 widget

        widget->setGeometry(rectinfo.rect);//将被移动widget 放到鼠标悬停的位置
        widgetmap[rectinfo.index] = widget;//将鼠标悬停位置的 rect 对应widget设置为被移动widget
        widget->setProperty("index",rectinfo.index);//将被移动widget的  index 属性 设置为鼠标悬停位置 rect 的  index

        QRect placerect = rectmap[curindex].rect;//获取被移动widget的  原位置
        placedwidget->setGeometry(placerect);//将鼠标悬停处的原 widget 移动到 被移动widget的原位置
        widgetmap[curindex] = placedwidget;//将被移动widget rect 对应widget设置为被移动widget
        placedwidget->setProperty("index",curindex);

        rectinfo.show = false;
        update();
    }else  //移动
    {
        QWidget* widget = widgetmap.take(curindex);
        widget->setGeometry(rectinfo.rect);
        widgetmap[rectinfo.index] = widget;
        widget->setProperty("index",rectinfo.index);
        rectinfo.show = false;
        update();
    }
}


void GroupElement::dropEvent(QDropEvent *event)
{
    int index =    event->mimeData()->data("index").toInt();
    QByteArray name =    event->mimeData()->data("name");
    ControlRole  ctrlrole = (ControlRole)event->mimeData()->data("operation").toInt();
    ControlType  ctrltype = (ControlType)event->mimeData()->data("controltype").toInt();

    for(int key: rectmap.keys())
    {
        if(rectmap[key].rect.contains( event->pos()))
        {
            rectmap[key].show = true;
            if(ctrlrole == cCreateEntry)
              addElement(rectmap[key],QString::number(key),ctrltype);
            else if(ctrlrole == cRealControl)
              moveElement(rectmap[key],index);
        }
        else
        {
            rectmap[key].show = false;
        }
    }
    update();

}
GroupElement::RectInfo  GroupElement::get_rect(int irow, int icol, int rowspan,int colspan)
{
     int left = m_leftmagin  + icol*(colspan + m_colspace);
     int top  = m_topmagin + irow*(rowspan+ m_rowspace);
     RectInfo  rectinfo;
     rectinfo.rect = QRect(left,top,colspan,rowspan);
     rectinfo.show   = false;
     return rectinfo;

}
void GroupElement::paintEvent(QPaintEvent *event)
{
    QPainter  painter(this);
    drawBackGround(painter);
    if(resizing)
    {
        QWidget::paintEvent(event);
        return;
    }
    for(int key:rectmap.keys())
    {
      if(ConfigGlobal::isEditing)
       {
          painter.save();
          if (rectmap[key].show || rectmap[key].selected)
          {              
              QPen pen = painter.pen();
              pen.setStyle(Qt::DashLine);
              pen.setWidth(5);
              pen.setColor(Qt::green);
              painter.setPen(pen);
              painter.drawRect(rectmap[key].rect);
          }
          else
          {
              QPen pen = painter.pen();
              pen.setStyle(Qt::DashLine);
              pen.setWidth(1);
              pen.setColor(Qt::black);
              painter.setPen(pen);
              painter.drawRect(rectmap[key].rect);
          }         
          painter.restore();
      }
    }
    drawTitle(painter);
    if(ConfigGlobal::isEditing)
      drawDeleteEntry(painter);

    drawTitleText(painter);
    drawIconEntry(painter);
    QWidget::paintEvent(event);
}
void GroupElement::drawTitleText(QPainter& painter)
{
    QColor brushColor;
    int penwidth = 0;
    if(titletexthitted)
    {
        brushColor = QColor(0,250,0);
        penwidth = 1;
    }
    else
    {
        brushColor = QColor(250,0,0);
        penwidth = 0;
    }

//    painter.save();
//    QPen pen;
//    pen.setColor(QColor(0,0,0));
//    pen.setStyle(Qt::SolidLine);
//    pen.setWidth(penwidth);
//    painter.setBrush(brushColor);
//    painter.setPen(pen);

////    titletextentry = QRect(width()/5,2,width()*4/5 - width()/10 ,m_titleheight -4);
        titletextentry = QRect(m_titleheight+10,0,width() - 2* m_titleheight,m_titleheight );
////    if(deletehoverd)
////    {
////      painter.drawRect(titletextentry);
////    }
////    else
////    {
////      painter.drawRect(titletextentry);
////    }
//    painter.restore();
    {
    painter.save();

    QPen pen;
   // pen.setColor(QColor(0,0,0));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    painter.setBrush(brushColor);
    pen.setColor(m_titlecolor);

    painter.setPen(pen);
    painter.setFont(m_titlefont);
    painter.drawText(titletextentry,Qt::AlignVCenter,QString::fromLocal8Bit(titletext) );
    painter.restore();
    }


}
void GroupElement::drawBackGround(QPainter& painter)
{

    painter.drawPixmap(rect(),m_backgroundpicture);


}
void GroupElement::drawTitle(QPainter& painter)
{
    painter.save();
    QPen pen;
    pen.setColor(QColor(250,0,0));
    pen.setStyle(Qt::DotLine);
    pen.setWidth(2);
    painter.setBrush(QColor(0,250,0));
    painter.setPen(pen);
    QRect  titlerect = QRect(0,0,width(),m_titleheight);
  //  titlerect.adjust(0,0,0,-10);
    painter.drawPixmap(titlerect,m_titlepicture);
    painter.restore();

}
void GroupElement::drawDeleteEntry(QPainter& painter)
{
    QColor brushColor;
    if(deletehitted)
        brushColor = QColor(0,250,0);
    else
        brushColor = QColor(250,0,0);

  //  qDebug() << "delete brushColor:" << brushColor << " deletehitted:" << deletehitted;
    painter.save();
    QPen pen;
    pen.setColor(QColor(250,0,0));
    pen.setStyle(Qt::DotLine);
    pen.setWidth(0);
    painter.setBrush(brushColor);
    painter.setPen(pen);
    deleteentry = QRect(width()-m_titleheight, m_titleheight/4 ,m_titleheight/2,m_titleheight/2);
  //  painter.drawRect(deleteentry);
    if(deletehoverd)
    {
      painter.drawImage(deleteentry,deleteicon);
    }
    else
    {
      painter.drawImage(deleteentry,deleteiconpressed);
    }
    painter.restore();

}
void GroupElement::resetLaoutFromData()
{
    resizing = true;
    int m_colspan = (width() - m_leftmagin - m_rightmargin + m_colspace)/m_col - m_colspace;
    rectmap.clear();
    int index = 0;
    for(int irow = 0; irow < m_row; irow++)
    {
        for(int icol =0; icol < m_col; icol++)
        {
            rectmap[index] = get_rect(irow,icol,m_rowspan,m_colspan);
            rectmap[index].index = index++;
        }
    }

    for(int index: rectmap.keys())
    {
        if(widgetmap.contains(index))
        {
            widgetmap[index]->setGeometry( rectmap[index].rect);
        }
    }
    resizing = false;


}
void GroupElement::resizeEvent(QResizeEvent *event)
{
    MoveAbleFrame::update_ctrl_point_pos_2();
    resetLaoutFromData();
    updateGeometryToTool();
    QWidget::resizeEvent(event);
}

void GroupElement::moveEvent(QMoveEvent *event)
{
    MoveAbleFrame::update_ctrl_point_pos_2();
    updateGeometryToTool();
   QWidget::moveEvent(event);
}
bool GroupElement::isdeletetriggerd(QPoint curpos)
{
    if(deleteentry.contains(curpos))
    {
        qDebug() << "deleteentry pressed";
        deletehitted = true;
        delSelectedElements();
        update();
        return true;
    }
    return false;
}
void GroupElement::handleDragEvent()
{
    QDrag* dg = new QDrag(this);
    //将需要拖动的数据放入QMimedata对象中，该对象用于保存需要传递的数据，数据的内
    //容完全由程序员自行设定。通常为界面上所选择内容。
    QMimeData* md = new QMimeData;

    md->setData("index", property("index").toByteArray());    			//这是md中存储的内容(即拖放时传递的数据)。
    md->setData("name", "text");    			//这是md中存储的内容(即拖放时传递的数据)。
    md->setData("operation", QByteArray::number(cCopycontrol));    			//这是md中存储的内容(即拖放时传递的数据)。
    md->setData("controltype", QByteArray::number(cConfigGroup));    			//这是md中存储的内容(即拖放时传递的数据)。
    md->setData("groupid", m_uuid.toLatin1());    			//这是md中存储的内容(即拖放时传递的数据)。
    dg->setMimeData(md);   			//步骤1：设置拖动的数据。该函数会获得md的所有权。
    dg->setPixmap(this->grab());
    dg->setHotSpot(QPoint(this->width() / 2, this->height() / 2));
    dg->exec(Qt::CopyAction | Qt::MoveAction);

}
void GroupElement::mousePressEvent(QMouseEvent *event)
{
    if(!ConfigGlobal::isEditing)
    {
        QWidget::mousePressEvent(event);
        return;
    }
    qDebug() << "GroupElement::mousePressEvent:" << event->modifiers();

    /*复制  判断如果 CTRL 被同时按下，进入赋值状态*/
    if (event->modifiers() == Qt::ShiftModifier)
    {
        qDebug() << "GroupElement::mousePressEvent ControlModifier";
        handlertriggerd = true;
        handlertriggerdPos = event->pos();
      //  handleDragEvent();
        QWidget::mousePressEvent(event);

        return;
    }       
    qDebug() << "GroupElement::mousePressEvent";
    ConfigGlobal::gpropeetyset->setObject(cConfigGroup,this, m_valueSetMap);
    MoveAbleFrame::update_ctrl_point_pos(this,(QWidget*)parent());
    MoveAbleFrame::setControlStyle(cConfigGroup,false,m_uuid);
    QPoint hitpos =  event->pos();
    if(isdeletetriggerd(hitpos))
    {
        for(int index: rectmap.keys())
        {
            rectmap[index].selected = false;
        }
        QWidget::mousePressEvent(event);
        return;
    }

    if(titletextentry.contains(hitpos))
    {

        titletexthitted = true;

    }else
    {
        titletexthitted = false;
    }


    bool hitrect = false;
    for(int index: rectmap.keys())
    {
        if(rectmap[index].rect.contains(hitpos)&& widgetmap.contains(index))
        {
            rectmap[index].selected = true;
            hitrect = true;
        }else
        {
            if(!(event->modifiers()&Qt::ControlModifier))
                rectmap[index].selected = false;

        }
    }
    if(!hitrect)
    {
        for(int index: rectmap.keys())
        {
            rectmap[index].selected = false;

        }
    }
    //MoveAbleFrame::mousePressEvent(event);
}

void GroupElement::mouseMoveEvent(QMouseEvent *event)
{
    if(deleteentry.contains(event->pos())&&!deletehoverd)
    {
       deletehoverd = true;
       update();
    }
    else  if(!deleteentry.contains(event->pos())&&deletehoverd)
    {
        deletehoverd = false;
        update();
    }
    qDebug() << "GroupElement::mouseMoveEvent:" << handlertriggerd;
    if ( (event->buttons()== Qt::LeftButton)&&handlertriggerd) {
        int distance = (event->pos() - handlertriggerdPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
        {
            handlertriggerd = false;
            handleDragEvent();
        }
    }

    QWidget::mouseMoveEvent(event);
}
void GroupElement::stateInit()
{
     deletehitted= false;
     update();
}
void GroupElement::mouseReleaseEvent(QMouseEvent *event)
{
    stateInit();
    QWidget::mouseReleaseEvent(event);
}

void GroupElement::updateGeometryData()
{
    QRect irect = geometry();
    m_width = irect.width();
    m_height = irect.height();
    m_xPos  = irect.x();
    m_yPos = irect.y();
}
void GroupElement::updateGeometryToTool()
{
    QRect irect = this->geometry();
    int xx = m_xPos;
    m_width = irect.width();
    m_height = irect.height();
    m_xPos  = irect.x();
    m_yPos = irect.y();

    ConfigGlobal::gpropeetyset->UpdateDataFromObject(this);
}

void GroupElement::updataDataFromTool()
{
   updataGeometryFromData();
    UpdatePropertyByStyle();
   resetLaoutFromData();
   update();
}

void GroupElement::updataGeometryFromData()
{
    setGeometry(m_xPos,m_yPos,m_width,m_height);
}


ConfigNameSpaceEnd
