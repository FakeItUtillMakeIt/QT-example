#include "configpairlabel.h"
#include "configglobal.h"
#include "configtypeselector.h"
#include <QDrag>
#include <QHBoxLayout>
#include <QLabel>
#include <QMimeData>
#include "moveableframe.h"
#include "stylemanager.h"
#include "propertyset.h"
#include "paramselect.h"
ConfigNameSpaceStart
ConfigPairLabel::ConfigPairLabel(QWidget* parent):QWidget(parent)
{
    m_uuid = get_uuid();

    paramlayout = new QHBoxLayout;
    paramnamelabel = new QLabel("参数名称");
    paramnamelabel->setAlignment(Qt::AlignCenter);
    paramnamelabel->setStyleSheet("background-color:rgb(150,150,150)");
    paramvaluelabel = new QLabel("参数值");
    paramvaluelabel->setStyleSheet("background-color:rgb(200,200,200)");
    paramvaluelabel->setAlignment(Qt::AlignCenter);
    paramlayout->addWidget(paramnamelabel);
    paramlayout->addWidget(paramvaluelabel);
    paramlayout->setMargin(0);

    paramlayout->setSpacing(m_space);
    paramlayout->setStretch(0,m_nameLabelStrech);
    paramlayout->setStretch(1,m_valueLabelStrech);
    setLayout(paramlayout);

    //DefaultUIInit();

    init_value_set();

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showListMenu(const QPoint&)));

}

void ConfigPairLabel::showListMenu(const QPoint& point) {

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
            m_valueSetMap["数据源配置"].valuelist[ConfigPairLabel::eDataSource]->SetWidthStrValue(QString::number(paramid));
            QString paramname = QString::fromLocal8Bit((*ConfigGlobal::m_allDeviceParamPtr)[paramid]->m_subParameterName.c_str());
            paramnamelabel->setText(paramname);
            paramvaluelabel->setText("");    
            ConfigGlobal::gpropeetyset->UpdateDataFromObject(this);
            ConfigGlobal::updateLabelMap(paramid,this);
        }
       
    });
    cmenu->exec(QCursor::pos());  // 当前鼠标位置
}

void ConfigPairLabel::DefaultUIInit()
{
    m_namelabelstyle = ":/rc/nValueNormal.png";
    m_valuelabelstyle = ":/rc/vValuenormal.png";
    m_error_namelabelstyle = ":/rc/nValueError.png";
    m_error_valuelabelstyle = ":/rc/vValueError.png";
    m_textfont = this->font().toString();
    m_textcolor = QColor(0,0,0).name(QColor::QColor::HexArgb);

    QString stylecolor =  QString("color:rgb(%1,%2,%3);").arg(m_textcolor.red()).arg(m_textcolor.green()).arg(m_textcolor.blue());
    paramnamelabel->setStyleSheet(QString("border-image: url(%1);%2").arg(m_namelabelstyle).arg(stylecolor));
    paramvaluelabel->setStyleSheet(QString("border-image: url(%1);%2").arg(m_valuelabelstyle).arg(stylecolor));
    paramnamelabel->setFont(m_textfont);
    paramvaluelabel->setFont(m_textfont);
}
void ConfigPairLabel::setRole(ControlRole role)
{
    m_ctrlrole = role;
}

void ConfigPairLabel::updateDataSource(QString datasourceid, QString datasourcename, int  addordelete)
{
    paramnamelabel->setText(datasourcename);
    paramvaluelabel->setText("");
}


void ConfigPairLabel::InitFromXmlInfo(QMap<QString, QString> &pairlabelinfo)
{
    m_uuid = pairlabelinfo["uuid"];
    for(auto  groupkey:m_valueSetMap.keys())
    {
        for(auto elementkey: m_valueSetMap[groupkey].valuelist.keys())
        {
            QString infokey = groupkey + ":" + QString::number(elementkey);
            QString strvalue  = pairlabelinfo[infokey];
            m_valueSetMap[groupkey].valuelist[elementkey]->SetWidthStrValue(strvalue);
            qDebug() <<"ConfigPairLabel infokey:" << infokey << " value:" <<m_valueSetMap[groupkey].valuelist[elementkey]->getStrValue() << " strvalue:" <<strvalue;
        }
    }
    updataDataFromTool();
   QString  istyleid =  m_valueSetMap["样式配置"].valuelist[ConfigPairLabel::eStyleInfo]->getStrValue();
   InitFromDefineStyle(istyleid);

   QString  datasourceid = m_valueSetMap["数据源配置"].valuelist[ConfigPairLabel::eDataSource]->getStrValue();
   if (datasourceid == "未设置")
   {
       return;
   }
   ConfigGlobal::updateLabelMap(datasourceid.toInt(),this);
}

void ConfigPairLabel::InitFromDefaultStyle()
{
    bool bret = UpdateStyleByStyleId("default");
    if (bret == false)
    {
        DefaultUIInit();
    }
}
void ConfigPairLabel::InitFromDefineStyle(QString istyleid)
{
    if (UpdateStyleByStyleId(istyleid) == false)
    {
        InitFromDefaultStyle();
    }
}

bool ConfigPairLabel::UpdateStyleByStyleId(QString styleid)
{
    bool  bfind = false;
    m_styleinfo = nullptr;
    QList<StylePairLabel *>& stylePairLabels  =   ConfigGlobal::gstylemanager->getPairLabelStyle();
    for(StylePairLabel * stylelabel: stylePairLabels)
    {
        if(stylelabel->m_infomap[StylePairLabel::PairLabel_Style_ID].second == styleid)
            m_styleinfo = stylelabel;
    }
     if(m_styleinfo)
     {
        m_styleinfo->record_styleusers(this);
        UpdatePropertyByStyle();
        bfind = true;
     }
    return bfind;
}
void ConfigPairLabel::setGroupId(QString groupid)
{
    m_groupid = groupid;
}
void ConfigPairLabel::init_value_set()
{

    {
          ConfigValueSet  m_valueSet;
          m_valueSet.valueSetname = "样式配置";
          m_valueSet.valuelist[ConfigPairLabel::eStyleInfo] = new ConfigValue(m_StyleID,eConfigStyleInfo,"样式选择");
          m_valueSetMap["样式配置"] = m_valueSet;
          m_valueSetMap["样式配置"].valuelist[ConfigPairLabel::eStyleInfo]->SetWidthStrValue("未设置");

          //char* styleinfo =  (char*)m_valueSetMap["样式配置"].valuelist[ConfigPairLabel::eStyleInfo]->value;
          //memset(styleinfo,0,MaximumPathLength);
          //memcpy(styleinfo,"",1);
    }
    {
          ConfigValueSet  m_valueSet;
          m_valueSet.valueSetname = "位置属性";
          m_valueSet.valuelist[ConfigPairLabel::eXPos] = new ConfigValue(&m_xPos,eConfigInt,"X坐标");
          m_valueSet.valuelist[ConfigPairLabel::eYPos] = new ConfigValue(&m_yPos,eConfigInt,"Y坐标");
          m_valueSet.valuelist[ConfigPairLabel::eWidth] = new ConfigValue(&m_width,eConfigInt,"宽度");
          m_valueSet.valuelist[ConfigPairLabel::eHeight] = new ConfigValue(&m_height,eConfigInt,"高度");
          m_valueSetMap["位置属性"] = m_valueSet;

    }
    {
          ConfigValueSet  m_valueSet;
          m_valueSet.valueSetname = "数据源配置";
          m_valueSet.valuelist[ConfigPairLabel::eDataSource] = new ConfigValue(m_ParamSourceID,eConfigDataSource,"选择数据源");
          m_valueSetMap["数据源配置"] = m_valueSet;
          m_valueSetMap["数据源配置"].valuelist[ConfigPairLabel::eDataSource]->SetWidthStrValue("未设置");
          //char* styleinfo =  (char*)m_valueSetMap["数据源配置"].valuelist[ConfigPairLabel::eDataSource]->value;
          //memset(styleinfo,0,MaximumPathLength);
          //memcpy(styleinfo,"",1);
    }
}

void ConfigPairLabel::updateGeometryData()
{
    QRect irect = geometry();
    m_width = irect.width();
    m_height = irect.height();
    m_xPos  = irect.x();
    m_yPos = irect.y();

}
void ConfigPairLabel::updataDataFromTool()
{
     QString  datasourceid = m_valueSetMap["数据源配置"].valuelist[ConfigPairLabel::eDataSource]->getStrValue();
    if (ConfigGlobal::VerifyNumber(datasourceid))
    {
        if ((*ConfigGlobal::m_allDeviceParamPtr).find(datasourceid.toInt()) != (*ConfigGlobal::m_allDeviceParamPtr).end())
        {
            string showtext = (*ConfigGlobal::m_allDeviceParamPtr)[datasourceid.toInt()]->m_subParameterName;
            paramnamelabel->setText(QString::fromLocal8Bit(showtext.c_str()));
            paramvaluelabel->setText("");
            ConfigGlobal::updateLabelMap(datasourceid.toInt(),this);
        }
    }
   updataGeometryFromData();
}
void ConfigPairLabel::updateText(QString text, int error)
{
    if (ConfigGlobal::isEditing) return;
    if ((error == 3) && (errorstate != error))
    {
        QString stylecolor = QString("color:rgb(%1,%2,%3);").arg(m_textcolor.red()).arg(m_textcolor.green()).arg(m_textcolor.blue());
        paramnamelabel->setStyleSheet(QString("border-image: url(%1);%2").arg(m_error_namelabelstyle).arg(stylecolor));
        paramvaluelabel->setStyleSheet(QString("border-image: url(%1);%2").arg(m_error_valuelabelstyle).arg(stylecolor));
        errorstate = error;
    }
    else if ((error != 3) && (errorstate != error))
    {
        QString stylecolor = QString("color:rgb(%1,%2,%3);").arg(m_textcolor.red()).arg(m_textcolor.green()).arg(m_textcolor.blue());
        paramnamelabel->setStyleSheet(QString("border-image: url(%1);%2").arg(m_namelabelstyle).arg(stylecolor));
        paramvaluelabel->setStyleSheet(QString("border-image: url(%1);%2").arg(m_valuelabelstyle).arg(stylecolor));
        errorstate = error;
    }
    paramvaluelabel->setText(text);
}

void ConfigPairLabel::updateValue(double value,int error)
{
    if (ConfigGlobal::isEditing) return;
    if((error == 3)&&(errorstate != error))
    {
        QString stylecolor = QString("color:rgb(%1,%2,%3);").arg(m_textcolor.red()).arg(m_textcolor.green()).arg(m_textcolor.blue());
        paramnamelabel->setStyleSheet(QString("border-image: url(%1);%2").arg(m_error_namelabelstyle).arg(stylecolor));
        paramvaluelabel->setStyleSheet(QString("border-image: url(%1);%2").arg(m_error_valuelabelstyle).arg(stylecolor));
        errorstate = error;
    }
    else if ((error != 3) && (errorstate != error))
    {
        QString stylecolor = QString("color:rgb(%1,%2,%3);").arg(m_textcolor.red()).arg(m_textcolor.green()).arg(m_textcolor.blue());
        paramnamelabel->setStyleSheet(QString("border-image: url(%1);%2").arg(m_namelabelstyle).arg(stylecolor));
        paramvaluelabel->setStyleSheet(QString("border-image: url(%1);%2").arg(m_valuelabelstyle).arg(stylecolor));
        errorstate = error;
    }
    paramvaluelabel->setText(QString::number(value, 'f', 3));
}


bool ConfigPairLabel::SelectStyle(StylePairLabel *& istyleinfo)
{
    QList<StylePairLabel *>& stylepairlabel  =   ConfigGlobal::gstylemanager->getPairLabelStyle();
    ConfigTypeSelector  selector;
    selector.UpdateTyleInfo(stylepairlabel);
    if(selector.exec() == QDialog::Rejected)
        return false;
    int styleindex =  selector.getResult();
    qDebug() << "styleindex" <<styleindex;
    m_styleinfo = stylepairlabel.at(styleindex);
    m_styleinfo->record_styleusers(this);
    istyleinfo = m_styleinfo;
    UpdatePropertyByStyle();
    return true;
}

void ConfigPairLabel::UpdatePropertyByStyle()
{
    QMap<int,QPair<bool,QString>> & m_infomap = m_styleinfo->m_infomap;
    if(m_infomap.contains(StylePairLabel::PairLabel_Style_ID))
        m_styleinfoId = m_infomap[StylePairLabel::PairLabel_Style_ID].second;
    if(m_infomap.contains(StylePairLabel::PairLabel_Style_ID))
        m_styleinfoname = m_infomap[StylePairLabel::PairLabel_Style_Name].second;
    if(m_infomap.contains(StylePairLabel::PairLabel_Name_Strech))
        m_nameLabelStrech = m_infomap[StylePairLabel::PairLabel_Name_Strech].second.toInt();
    if(m_infomap.contains(StylePairLabel::PairLabel_Value_Strech))
        m_valueLabelStrech = m_infomap[StylePairLabel::PairLabel_Value_Strech].second.toInt();
    if(m_infomap.contains(StylePairLabel::PairLabel_Name_Style))
        m_namelabelstyle = m_infomap[StylePairLabel::PairLabel_Name_Style].second;
    if(m_infomap.contains(StylePairLabel::PairLabel_Value_Style))
        m_valuelabelstyle =m_infomap[StylePairLabel::PairLabel_Value_Style].second;

    if(m_infomap.contains(StylePairLabel::PairLabel_Error_Name_Style))
        m_error_namelabelstyle = m_infomap[StylePairLabel::PairLabel_Error_Name_Style].second;
    if(m_infomap.contains(StylePairLabel::PairLabel_Error_Value_Style))
        m_error_valuelabelstyle =m_infomap[StylePairLabel::PairLabel_Error_Value_Style].second;

    if(m_infomap.contains(StylePairLabel::PairLabel_Font_Style))
    {
        m_textfont.fromString(m_infomap[StylePairLabel::PairLabel_Font_Style].second);
    }
    if(m_infomap.contains(StylePairLabel::PairLabel_Text_Color))
    {
        m_textcolor.setNamedColor(m_infomap[StylePairLabel::PairLabel_Text_Color].second);
    }
    if(m_infomap.contains(StylePairLabel::PairLabel_Space))
    {
       m_space =  m_infomap[StylePairLabel::PairLabel_Space].second.toInt();
    }
    QString stylecolor =  QString("color:rgb(%1,%2,%3);").arg(m_textcolor.red()).arg(m_textcolor.green()).arg(m_textcolor.blue());
    paramnamelabel->setStyleSheet(QString("border-image: url(%1);%2").arg(m_namelabelstyle).arg(stylecolor));
    paramvaluelabel->setStyleSheet(QString("border-image: url(%1);%2").arg(m_valuelabelstyle).arg(stylecolor));
    qDebug() << "m_nameLabelStrech:"<<m_nameLabelStrech;
    qDebug() << "m_valueLabelStrech:"<<m_valueLabelStrech;

    paramlayout->setStretch(0,m_nameLabelStrech);
    paramlayout->setStretch(1,m_valueLabelStrech);
    paramlayout->setSpacing(m_space);
    paramnamelabel->setFont(m_textfont);
    paramvaluelabel->setFont(m_textfont);

//    char* styleidinfo =  (char*)m_valueSetMap["样式配置"].valuelist[ConfigPairLabel::eStyleInfo]->value;
//    memset(styleidinfo,0,MaximumPathLength);
    //    memcpy(styleidinfo,m_styleinfoId.toStdString().c_str(),strlen(m_styleinfoId.toStdString().c_str()));
}

QString ConfigPairLabel::GetID()
{
    return m_uuid;
}

QMap<QString, ConfigValueSet> &ConfigPairLabel::getPropertyMap()
{
    return m_valueSetMap;
}
void ConfigPairLabel::updataGeometryFromData()
{
    setGeometry(m_xPos,m_yPos,m_width,m_height);
}

void ConfigPairLabel::mousePressEvent(QMouseEvent *event)
{
    if(!ConfigGlobal::isEditing)
    {
       QWidget::mousePressEvent(event);
       return;
    }

   // qDebug() <<"ConfigPairLabel:mousePressEvent:" << m_valueSetMap["数据源配置"].valuelist[ConfigPairLabel::eDataSource]->getStrValue();
    if (event->button() == Qt::LeftButton)
            startPos = event->pos();
    if(!m_ingroup)
    {
         MoveAbleFrame::update_ctrl_point_pos(this,(QWidget*)parent());
         MoveAbleFrame::setControlStyle(cConfigPairLabel, false,m_uuid);

    }
    updateGeometryData();
    ConfigGlobal::gpropeetyset->setObject(cConfigPairLabel,this, m_valueSetMap);
    if(m_ingroup)
     QWidget::mousePressEvent(event);
}
void ConfigPairLabel::handleDragEvent()
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
void ConfigPairLabel::mouseMoveEvent(QMouseEvent *ev)
{
    if(!ConfigGlobal::isEditing)
    {
       QWidget::mouseMoveEvent(ev);
       return;
    }
    if(m_ingroup)
    {
        if (ev->buttons() & Qt::LeftButton) {
           int distance = (ev->pos() - startPos).manhattanLength();
           if (distance >= QApplication::startDragDistance())
           {
               handleDragEvent();
               return;
           }
        }
    }
    QWidget::mouseMoveEvent(ev);
}

void ConfigPairLabel::moveEvent(QMoveEvent *event)
{
    if(!m_ingroup)
         MoveAbleFrame::update_ctrl_point_pos_2();

    updateGeometryData();
    ConfigGlobal::gpropeetyset->UpdateDataFromObject(this);

    QWidget::moveEvent(event);
}

void ConfigPairLabel::resizeEvent(QResizeEvent *event)
{
    if(!m_ingroup)
        MoveAbleFrame::update_ctrl_point_pos_2();

    updateGeometryData();
    ConfigGlobal::gpropeetyset->UpdateDataFromObject(this);

    QWidget::resizeEvent(event);
}

void ConfigPairLabel::setName(QString name)
{
    namelabel->setText(name);
}

void ConfigPairLabel::setValue(QString value)
{
    valuelabel->setText(value);
}

ConfigNameSpaceEnd
