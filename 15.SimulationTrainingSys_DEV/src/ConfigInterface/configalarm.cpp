#include "configalarm.h"
#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <QUuid>
#include "configglobal.h"
#include "configtypeselector.h"
#include "moveableframe.h"
#include "propertyset.h"
#include "stylemanager.h"
#include <qaction.h>
#include <qmenu.h>
#include "paramselect.h"
ConfigNameSpaceStart
ConfigAlarm::ConfigAlarm(QWidget *parent):
    QWidget(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showListMenu(const QPoint&)));
    m_uuid = get_uuid();
    init_value_set();
}


void ConfigAlarm::setState(RunState state)
{
    m_RunState = state;
    update();
}

void ConfigAlarm::showListMenu(const QPoint& point) {

    if (!ConfigGlobal::isEditing)
    {
        return;
    }
    QMenu* cmenu = new QMenu();
    QAction* actSetCmd = cmenu->addAction("设置参数");
    QString str = "为当前标签选择参数";
    connect(actSetCmd, &QAction::triggered, [=]() {
        {
            ParamSelect  paramselect;
            paramselect.update_data(*ConfigGlobal::m_allDeviceParamPtr);
            if (paramselect.exec() == QDialog::Rejected)
                return;
            int paramid = paramselect.get_select_key();
            m_valueSetMap["数据源配置"].valuelist[ConfigAlarm::eDataSource]->SetWidthStrValue(QString::number(paramid));
            QString paramname = QString::fromLocal8Bit((*ConfigGlobal::m_allDeviceParamPtr)[paramid]->m_subParameterName.c_str());            
            m_text = paramname;
            ConfigGlobal::gpropeetyset->UpdateDataFromObject(this);
            ConfigGlobal::updateAlarmMap(paramid, this);
        }

        });
    cmenu->exec(QCursor::pos());  // 当前鼠标位置
}
void ConfigAlarm::DefaultUiInit()
{
    m_img_error = ":/rc/alarmError.png";
    m_img_init = ":/rc/alarmInit.png";
    m_img_normal = ":/rc/alarmNormal.png";
    m_display_img = true; 
    m_display_vertically = false; 
    m_color_error = QColor(250, 0, 0);
    m_color_init = QColor(220, 220, 220);
    m_color_normal = QColor(0, 250, 0);
    m_text = "未设置";    
    m_text_font = this->font();
    m_text_font.setPointSizeF(20);
    m_text_color = QColor(0, 0, 0);
    m_RunState = StateNormal;
}

QMap<QString, ConfigValueSet>& ConfigAlarm::getPropertyMap()
{
    return m_valueSetMap;
}

void ConfigAlarm::InitFromDefaultStyle()
{
   bool bret =   UpdateStyleByStyleId("default");
   if (bret == false)
   {
       DefaultUiInit();
   }
}

void ConfigAlarm::InitFromDefineStyle(QString istyleid)
{
    if (UpdateStyleByStyleId(istyleid) == false)
    {
        InitFromDefaultStyle();
    }
}


void ConfigAlarm::updateDataSource(QString datasourceid, QString datasourcename, int  addordelete)
{
    QString paramid =  m_valueSetMap["数据源配置"].valuelist[ConfigAlarm::eDataSource]->getStrValue();
    if (ConfigGlobal::VerifyNumber(paramid))
        ConfigGlobal::updateAlarmMap(paramid.toInt(), this);
    m_text = datasourcename;
    update();
}

void ConfigAlarm::init_value_set()
{
    {
          ConfigValueSet  m_valueSet;
          m_valueSet.valueSetname = "样式配置";
          m_valueSet.valuelist[ConfigAlarm::eStyleInfo] = new ConfigValue(m_StyleID,eConfigStyleInfo,"样式选择");
          m_valueSetMap["样式配置"] = m_valueSet;
          m_valueSetMap["样式配置"].valuelist[ConfigAlarm::eStyleInfo]->SetWidthStrValue("未设置");
     }
    {
          ConfigValueSet  m_valueSet;
          m_valueSet.valueSetname = "位置属性";
          m_valueSet.valuelist[ConfigAlarm::eXPos] = new ConfigValue(&m_xPos,eConfigInt,"X坐标");
          m_valueSet.valuelist[ConfigAlarm::eYPos] = new ConfigValue(&m_yPos,eConfigInt,"Y坐标");
          m_valueSet.valuelist[ConfigAlarm::eWidth] = new ConfigValue(&m_width,eConfigInt,"宽度");
          m_valueSet.valuelist[ConfigAlarm::eHeight] = new ConfigValue(&m_height,eConfigInt,"高度");
          m_valueSetMap["位置属性"] = m_valueSet;

    }
    {
        ConfigValueSet  m_valueSet;
        m_valueSet.valueSetname = "数据源配置";
        m_valueSet.valuelist[ConfigAlarm::eDataSource] = new ConfigValue(m_ParamSourceID, eConfigDataSource, "选择数据源");
        m_valueSetMap["数据源配置"] = m_valueSet;
        m_valueSetMap["数据源配置"].valuelist[ConfigAlarm::eDataSource]->SetWidthStrValue("未设置");
    }
}
void ConfigAlarm::updateGeometryData()
{
    QRect irect = geometry();
    m_width = irect.width();
    m_height = irect.height();
    m_xPos  = irect.x();
    m_yPos = irect.y();
}
ConfigAlarm::~ConfigAlarm()
{
    qDebug() <<  "~ConfigAlarm";
}

void ConfigAlarm::InitFromXmlInfo(QMap<QString, QString> &buttoninfo)
{
    m_uuid = buttoninfo["uuid"];
    for(auto  groupkey:m_valueSetMap.keys())
    {
        for(auto elementkey: m_valueSetMap[groupkey].valuelist.keys())
        {
            QString infokey = groupkey + ":" + QString::number(elementkey);
            QString strvalue  = buttoninfo[infokey];
            m_valueSetMap[groupkey].valuelist[elementkey]->SetWidthStrValue(strvalue);
        }
    }
    QString datasourceid = m_valueSetMap["数据源配置"].valuelist[ConfigAlarm::eDataSource]->getStrValue();
    if (ConfigGlobal::VerifyNumber(datasourceid))
    {
        if ((*ConfigGlobal::m_allCommadPrt).find(datasourceid.toInt()) != (*ConfigGlobal::m_allCommadPrt).end())
        {
            ConfigGlobal::updateAlarmMap(datasourceid.toInt(),this);
            string showtext = (*ConfigGlobal::m_allCommadPrt)[datasourceid.toInt()]->m_sName;
            datasourceid = QString::fromLocal8Bit(showtext.c_str());
            m_text = datasourceid;
        }
    }

    updataDataFromTool();
   QString  istyleid =  m_valueSetMap["样式配置"].valuelist[ConfigAlarm::eStyleInfo]->getStrValue();
   InitFromDefineStyle(istyleid);
}

void ConfigAlarm::setState(ControlOperateState state)
{
    m_state = state;
}
void ConfigAlarm::setRole(ControlRole role)
{
    m_ctrlrole = role;
}

void ConfigAlarm::setGroupId(QString groupid)
{
    m_groupid = groupid;
}

QString ConfigAlarm::GetID()
{
    return m_uuid;
}

void ConfigAlarm::mousePressEvent(QMouseEvent *ev)
{
    if(!ConfigGlobal::isEditing)
    {
       QWidget::mousePressEvent(ev);
       return;
    }
    qDebug() << "ConfigAlarm:"  << m_valueSetMap["数据源配置"].valuelist[ConfigAlarm::eDataSource]->getStrValue();
    if (ev->button() == Qt::LeftButton)
            startPos = ev->pos();

    updateGeometryData();
    ConfigGlobal::gpropeetyset->setObject(cConfigAlarm,this, m_valueSetMap);
    if (!m_ingroup)
    {
        MoveAbleFrame::update_ctrl_point_pos(this, (QWidget*)parent());
        MoveAbleFrame::setControlStyle(cConfigAlarm, false,m_uuid);
    }
   if(m_ingroup)
     ev->ignore();

}

void ConfigAlarm::mouseMoveEvent(QMouseEvent *e)
{
    if (!ConfigGlobal::isEditing)
    {
        QWidget::mouseMoveEvent(e);
        return;
    }
    if(m_ingroup)
    {
        if (e->buttons() & Qt::LeftButton) {
           int distance = (e->pos() - startPos).manhattanLength();
           if (distance >= QApplication::startDragDistance())
               handleDragEvent();
        }
    }

    QWidget::mouseMoveEvent(e);
}
void ConfigAlarm::updataDataFromTool()
{
    setGeometry(m_xPos,m_yPos,m_width,m_height);
}
void ConfigAlarm::moveEvent(QMoveEvent *event)
{
    updateGeometryData();
    ConfigGlobal::gpropeetyset->UpdateDataFromObject(this);

    if(!m_ingroup)
        MoveAbleFrame::update_ctrl_point_pos_2();
    QWidget::moveEvent(event);
}

void ConfigAlarm::resizeEvent(QResizeEvent *event)
{
    updateGeometryData();
    ConfigGlobal::gpropeetyset->UpdateDataFromObject(this);

    if(!m_ingroup)
        MoveAbleFrame::update_ctrl_point_pos_2();
    QWidget::resizeEvent(event);
}
bool ConfigAlarm::SelectStyle(StyleAlarm *& istyleinfo)
{
    QList<StyleAlarm *>& styleAlarms  =   ConfigGlobal::gstylemanager->getAlarmStyle();
    ConfigTypeSelector  selector;
    selector.UpdateTyleInfo(styleAlarms);
    if(selector.exec() == QDialog::Rejected)
        return false;
    int styleindex =  selector.getResult();
    qDebug() << "styleindex" <<styleindex;
    m_styleinfo = styleAlarms.at(styleindex);
    m_styleinfo->record_styleusers(this);
    istyleinfo = m_styleinfo;
    UpdatePropertyByStyle();
    return true;
}
bool ConfigAlarm::UpdateStyleByStyleId(QString styleid)
{
    bool befind = false;
    m_styleinfo = nullptr;
    QList<StyleAlarm *>& StyleAlarms  =   ConfigGlobal::gstylemanager->getAlarmStyle();
    for(StyleAlarm * stylebtn: StyleAlarms)
    {
        if(stylebtn->m_infomap[StyleAlarm::Alarm_Style_ID].second == styleid)
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

void ConfigAlarm::UpdatePropertyByStyle()
{
    QMap<int,QPair<bool,QString>> & m_infomap = m_styleinfo->m_infomap;
    if(m_infomap.contains(StyleAlarm::Alarm_Style_ID))
        m_styleinfoId = m_infomap[StyleAlarm::Alarm_Style_ID].second;
    if(m_infomap.contains(StyleAlarm::Alarm_Style_Name))
        m_styleinfoname = m_infomap[StyleAlarm::Alarm_Style_Name].second;
    if(m_infomap.contains(StyleAlarm::Alarm_Diaplay_Type))
        m_display_img = (m_infomap[StyleAlarm::Alarm_Diaplay_Type].second == "是"?true:false);
    if(m_infomap.contains(StyleAlarm::Alarm_Text_Position))
        m_display_vertically = (m_infomap[StyleAlarm::Alarm_Text_Position].second == "是" ? true : false);
    if(m_infomap.contains(StyleAlarm::Alarm_Init_Img))
        m_img_init = m_infomap[StyleAlarm::Alarm_Init_Img].second;
    if(m_infomap.contains(StyleAlarm::Alarm_Normal_Img))
        m_img_normal = m_infomap[StyleAlarm::Alarm_Normal_Img].second;
    if(m_infomap.contains(StyleAlarm::Alarm_Err_Img))
        m_img_error = m_infomap[StyleAlarm::Alarm_Err_Img].second;
    if (m_infomap.contains(StyleAlarm::Alarm_Init_Color))
        m_color_init = m_infomap[StyleAlarm::Alarm_Init_Color].second;
    if (m_infomap.contains(StyleAlarm::Alarm_Normal_Color))
        m_color_normal = m_infomap[StyleAlarm::Alarm_Normal_Color].second;
    if (m_infomap.contains(StyleAlarm::Alarm_Err_Color))
        m_color_error = m_infomap[StyleAlarm::Alarm_Err_Color].second;

    if(m_infomap.contains(StyleAlarm::ALarm_Font_Style))
    {
        m_text_font.fromString(m_infomap[StyleAlarm::ALarm_Font_Style].second);
    }
    if(m_infomap.contains(StyleAlarm::Alarm_Text_Color))
    {
        m_text_color.setNamedColor(m_infomap[StyleAlarm::Alarm_Text_Color].second);
    }
    update();
}
void ConfigAlarm::handleDragEvent()
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
void ConfigAlarm::drawLight(QPainter* painter, QRectF rect)
{
    if (!m_display_img)
        drawEcllipse(painter, rect);
    else
        drawImage(painter, rect);
}

void ConfigAlarm::drawImage(QPainter* painter, QRectF  imgrect)
{
    //  QRectF  imgrect = QRectF(-m_width/2,-m_width/2,m_width,m_width);
    QString imgpath = m_img_init;
    if (m_RunState == StateNormal)
        imgpath = m_img_init;
    else if (m_RunState == StateSuccess)
        imgpath = m_img_normal;
    else if (m_RunState == StateFailed)
        imgpath = m_img_error;
    painter->drawImage(imgrect, QImage(imgpath).scaled(imgrect.width(),imgrect.height()));

}
void ConfigAlarm::drawEcllipse(QPainter* painter, QRectF ellipseRect)
{
    QColor eclipsecolor = m_color_init;
    if (m_RunState == StateNormal)
        eclipsecolor = m_color_init;
    else if (m_RunState == StateSuccess)
        eclipsecolor = m_color_normal;
    else if (m_RunState == StateFailed)
        eclipsecolor = m_color_error;
    painter->save();
    painter->setBrush(eclipsecolor);
    painter->setPen(QColor(250, 250, 250, 0));
    painter->drawEllipse(ellipseRect);
    painter->restore();
}
void ConfigAlarm::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    QRectF alarmrect;
    QRectF textrect;

    painter.setRenderHint(QPainter::Antialiasing);  /* 使用反锯齿(如果可用) */
    m_width = width();
    m_height = height();
    if (m_display_vertically)
    {
        painter.translate(m_width >> 1, (m_height * 3) >> 3);   /* 坐标变换为窗体中心 */
        int alarmlength = (m_height * 3) >> 2;
        int textlength = m_height >> 2;
        alarmlength = (alarmlength > m_width ? m_width : alarmlength);
        alarmrect = QRectF(-alarmlength / 2, -alarmlength / 2, alarmlength, alarmlength);
        textrect = QRectF(-m_width / 2, alarmlength / 2, m_width, textlength);
        drawLight(&painter, alarmrect);
        painter.setPen(m_text_color);
        painter.setFont(m_text_font);
        painter.drawText(textrect, Qt::AlignCenter, m_text);
    }
    else
    {
        int alarmlength = m_width >> 1;
        alarmlength = (alarmlength > m_height ? m_height : alarmlength);
        int textlength = alarmlength;
        painter.translate(m_width * 3 >> 2, m_height >> 1);   /* 坐标变换为窗体中心 */
        alarmrect = QRectF(-alarmlength / 2, -alarmlength / 2, alarmlength, alarmlength);
        drawLight(&painter, alarmrect);
        painter.setPen(m_text_color);
        painter.setFont(m_text_font);
        painter.translate(-m_width * 3 >> 2, -m_height >> 1);   /* 坐标变换为窗体中心 */
        textrect = QRectF(0, 0, m_width >> 1, m_height);
        painter.drawText(textrect, Qt::AlignHCenter | Qt::AlignVCenter, m_text);
    }

    painter.restore();
    QWidget::paintEvent(event);
}
ConfigNameSpaceEnd
