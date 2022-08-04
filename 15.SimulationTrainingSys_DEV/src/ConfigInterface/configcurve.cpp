
#include "configcurve.h"
#include <QDebug>
#include "configglobal.h"
#include "propertyset.h"
#include "stylemanager.h"
#include "moveableframe.h"
#include "configtypeselector.h"

ConfigNameSpaceStart

ConfigCurve::ConfigCurve(QWidget *parent):QCustomPlot(parent)
{

    legend->setVisible(true);
    legend->setBrush(QBrush(Qt::transparent));     //设置图例透明无边框
    legend->setBorderPen(Qt::NoPen);
    legend->setTextColor(QColor(50,50,50,250));
    setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    memset(m_styleID,0,MaximumPathLength);
    memset(m_title,0,MaximumPathLength);
    m_uuid = get_uuid();
    AddTitle("未命名");
    init_value_set();

    colorlist << QColor(250,0,0) <<QColor(0,250,0) <<QColor(0,0,250);

}
void ConfigCurve::AddTitle(QString title)
{
    plotLayout()->insertRow(0);     // pCurveWidget、curveWidget为在ui中定义的QCustomPlot对象指针
    memcpy(m_title,title.toLocal8Bit().data(),strlen(title.toLocal8Bit().data()));
    m_pCurvTitle = new QCPTextElement(this, QString::fromLocal8Bit( m_title));
    plotLayout()->addElement(0, 0, m_pCurvTitle);    //给坐标设置标题
}
void ConfigCurve::InitFromXmlInfo(QMap<QString, QString> & curveinfo)
{
    m_uuid = curveinfo["uuid"];
    for(auto  groupkey:m_valueSetMap.keys())
    {
        for(auto elementkey: m_valueSetMap[groupkey].valuelist.keys())
        {
            QString infokey = groupkey + ":" + QString::number(elementkey);
            QString strvalue  = curveinfo[infokey];
            m_valueSetMap[groupkey].valuelist[elementkey]->SetWidthStrValue(strvalue);
        }
    }
    updataDataFromTool();
    initDataSourcesFromXmlData();
    QString  istyleid =  m_valueSetMap["样式配置"].valuelist[ConfigCurve::eStyleInfo]->getStrValue();
    UpdatePropertyByStyle();
    ConfigGlobal::gstylemanager->RegisterCurveByStyleId(istyleid,this);
}
void ConfigCurve::initDataSourcesFromXmlData()
{
    for (auto paramid : dataSourceList)
    {
        QString paramname = QString::fromLocal8Bit((*ConfigGlobal::m_allDeviceParamPtr)[paramid.toInt()]->m_subParameterName.c_str());
        updateCurve(paramid, paramname,1);
    }    
}
void ConfigCurve::updateGeometryData()
{
    QRect irect = geometry();
    m_width = irect.width();
    m_height = irect.height();
    m_xPos  = irect.x();
    m_yPos = irect.y();
}
void ConfigCurve::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << "dataSourceList:" <<dataSourceList;
    if(!ConfigGlobal::isEditing)
    {
       QCustomPlot::mouseMoveEvent(ev);
       return;
    }
    updateGeometryData();
    ConfigGlobal::gpropeetyset->setObject(cConfigCurve,this, m_valueSetMap);
    MoveAbleFrame::update_ctrl_point_pos(this,(QWidget*)parent());
    MoveAbleFrame::setControlStyle(cConfigCurve, false,m_uuid);

    QCustomPlot::mousePressEvent(ev);
}

void ConfigCurve::moveEvent(QMoveEvent *event)
{
    updateGeometryData();
    ConfigGlobal::gpropeetyset->UpdateDataFromObject(this);

    MoveAbleFrame::update_ctrl_point_pos_2();
    QCustomPlot::moveEvent(event);

}

void ConfigCurve::resizeEvent(QResizeEvent *event)
{
    updateGeometryData();

    ConfigGlobal::gpropeetyset->UpdateDataFromObject(this);

    MoveAbleFrame::update_ctrl_point_pos_2();
    QCustomPlot::resizeEvent(event);

}
void ConfigCurve::updataDataFromTool()
{
    setGeometry(m_xPos,m_yPos,m_width,m_height);
    m_pCurvTitle->setText(QString::fromLocal8Bit( m_title));
    replot();
}
void ConfigCurve::updateCurve(QString datasourceid,QString datasourcename,int  addordelete)
{
    if(addordelete == 1)
    {
       QCPGraph* graph  =   addGraph();
       graph->setName(datasourcename);
        curvelist[datasourceid] = graph;
       QColor  color  =  colorlist.back();
       colorlist.pop_back();
       graph->setPen(QPen(color));
       if(colorlist.size()  == 0)
       {
           colorlist << QColor(250,0,0) <<QColor(0,250,0) <<QColor(0,250,0);
       }
    }
    else
    {
        QCPGraph* graph = curvelist[datasourceid];
        curvelist.remove(datasourceid);
        removeGraph(graph);
    }

   replot();
}


void ConfigCurve::UpdatePropertyByStyle()
{
    QMap<int,QPair<bool,QString>>  m_infomap;
    ConfigGlobal::gstylemanager->GetCurveStyle(m_styleID,m_infomap);
    QString fontstr = m_infomap[ConfigCurve::Curve_Title_Font].second;
    if(!fontstr.isNull())
    {
        qDebug() << "curve set font";
        QFont font;
        font.fromString(fontstr);
        m_pCurvTitle->setFont(font);
    }
    QString colorstr = m_infomap[ConfigCurve::Curve_Title_Color].second;
    if(!colorstr.isNull())
    {
        QColor color;
        color.setNamedColor(colorstr);
        m_pCurvTitle->setTextColor(color);

    }
    colorstr = m_infomap[ConfigCurve::Curve_Common_Color].second;
    if(!colorstr.isNull())
    {
        QColor color;
        color.setNamedColor(colorstr);
        setBackground(color);
    }
    QString titleshowstr = m_infomap[ConfigCurve::Curve_Title_Show].second;
    if(!titleshowstr.isNull())
    {
        if(titleshowstr == "是")
        {
            m_pCurvTitle->setVisible(true);
        }
        else if(titleshowstr == "否")
        {
            m_pCurvTitle->setVisible(false);
        }
    }

    replot();
}

QString ConfigCurve::GetID()
{
     return m_uuid;
}
void ConfigCurve::init_value_set()
{
    {
          ConfigValueSet  m_valueSet;
          m_valueSet.valueSetname = "样式配置";
          m_valueSet.valuelist[ConfigCurve::eStyleInfo] = new ConfigValue(m_styleID,eConfigStyleInfo,"样式选择");
          m_valueSet.valuelist[ConfigCurve::eTitleInfo] = new ConfigValue(m_title,eConfigString,"标题设置");
          m_valueSetMap["样式配置"] = m_valueSet;

    }
    {
          ConfigValueSet  m_valueSet;
          m_valueSet.valueSetname = "位置属性";
          m_valueSet.valuelist[ConfigCurve::eXPos] = new ConfigValue(&m_xPos,eConfigInt,"X坐标");
          m_valueSet.valuelist[ConfigCurve::eYPos] = new ConfigValue(&m_yPos,eConfigInt,"Y坐标");
          m_valueSet.valuelist[ConfigCurve::eWidth] = new ConfigValue(&m_width,eConfigInt,"宽度");
          m_valueSet.valuelist[ConfigCurve::eHeight] = new ConfigValue(&m_height,eConfigInt,"高度");
          m_valueSetMap["位置属性"] = m_valueSet;

    }
    {
          ConfigValueSet  m_valueSet;
          m_valueSet.valueSetname = "数据源配置";
          m_valueSet.valuelist[ConfigCurve::eMultiDataSource] = new ConfigValue(&dataSourceList,eConfigMultiDataSource,"选择数据源");
          m_valueSetMap["数据源配置"] = m_valueSet;
    }
}
ConfigNameSpaceEnd
