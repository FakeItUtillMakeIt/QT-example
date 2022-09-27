
#include "configcurve.h"
#include <QDebug>
#include "configglobal.h"
#include "propertyset.h"
#include "stylemanager.h"
#include "moveableframe.h"
#include "configtypeselector.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include "multiinputget.h"
#include "qaction.h"
#include "paramselect.h"
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
 //  addGraph();

    //曲线测试
 //startTimer(1000);
}

//void ConfigCurve::timerEvent(QTimerEvent* event)
//{
//    unsigned short m_iYear;
//    ///月
//    unsigned char m_iMonth;
//    ///日
//    unsigned char m_iDay;
//    ///时间，相对于当日零时的纳秒数
//    unsigned long long m_ullTime;
//    ///帧计数
//    using namespace boost::posix_time;
//    ptime now = microsec_clock::local_time();
//    tm t = to_tm(now);
//    m_iYear = t.tm_year + 1900;
//    m_iMonth = t.tm_mon + 1;
//    m_iDay = t.tm_mday;
//    //当天的纳秒数
//    time_duration td = now.time_of_day();
//    double itime = (unsigned long long)td.total_milliseconds() * 1000;
//    QTime  st;
//    st = QTime::currentTime();
//    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
//    double ivalue = qrand()%50;
//    updateGraph0Value(1, itime, ivalue);
//}



void ConfigCurve::contextMenuEvent(QContextMenuEvent* event)
{
    
    QAction  act_is_roll(this);
    act_is_roll.setText("曲线滚动");
    act_is_roll.setCheckable(true);
    act_is_roll.setChecked(xrool);
    connect(&act_is_roll, &QAction::triggered, [=](bool checked)
    {
            if (xrange == -1)
            {
                bool ok;
                double a = QInputDialog::getDouble(this, "输入", "请输入X轴滚动范围", 10, 0.01, 100000, 2, &ok);
                qDebug() << "value:" << a << " bool:" << ok;
                if (!ok)
                    return;
                xrool = checked;
                xrange = a;
            }
            else
            {
                xrool = checked;
            }
            update_customplot_range();
            replot(QCustomPlot::rpQueuedReplot);
        
    });

    QAction act_set_roll_range(this);
    act_set_roll_range.setText("设置滚动范围");
   // connect(&act_set_roll_range, SIGNAL(triggered()), this, SLOT(setrollchange()));
    connect(&act_set_roll_range, &QAction::triggered, [=](bool checked)
        {
            bool ok;
            double currentvalue = 10;
            if (xrange != -1)
                currentvalue = xrange;
            double a = QInputDialog::getDouble(nullptr, "输入", "请输入X轴滚动范围", currentvalue, 0.01, 100000, 2, &ok);
            if (!ok)
                return;
            qDebug() << "value:" << a << " bool:" << ok;
            xrool = true;
            xrange = a;
            update_customplot_range();
            replot(QCustomPlot::rpQueuedReplot);

        });
  //  QAction act_set_tmp_range(this);
  //  act_set_tmp_range.setText("设置临时显示范围");
  ////  connect(&act_set_tmp_range, SIGNAL(triggered()), this, SLOT(settmprange()));
  //  connect(&act_set_tmp_range, &QAction::triggered, [=](bool checked)
  //      {
  //          MultiInputGet  multiinputget;
  //          multiinputget.setWindowTitle("请输入范围");
  //          multiinputget.update_range(xvalueMin, xvalueMax, yvalueMin, yvalueMax);
  //          if (multiinputget.exec() == QDialog::Rejected)
  //              return;

  //          double xmin, xmax, ymin, ymax;
  //          if (multiinputget.get_x_range(xmin, xmax))
  //          {
  //              xAxis->setRange(xmin, xmax);
  //          }
  //          if (multiinputget.get_y_range(ymin, ymax))
  //          {
  //              yAxis->setRange(ymin, ymax);
  //          }
  //          replot();
  //      });

    QMenu menu;
    act_is_roll.setChecked(xrool);
    menu.addAction(&act_is_roll);
   // menu.addSeparator();
    menu.addAction(&act_set_roll_range);
    menu.addAction("选择数据源", [=]()
        {
            ParamSelect  paramselect;
            paramselect.enableMultiSelect(true);
            QVector<int>  idlist;
            for (auto ds : dataSourceList)
            {
                idlist.push_back(ds.toInt());
            }           
            paramselect.update_data(*ConfigGlobal::m_allDeviceParamPtr, idlist);
            if (paramselect.exec() == QDialog::Rejected)
                return;
            QVector<int> paramids = paramselect.get_multiselect_key();
            dataSourceList.clear();
            for (auto paramid : paramids)
            {               
                dataSourceList.push_back(QString::number(paramid));
            }
            updateCurve("1", "", 1);
            ConfigGlobal::gpropeetyset->UpdateDataFromObject(this);
        });
  //  menu.addSeparator();
  //  menu.addAction(&act_set_tmp_range);
    //menu.addSeparator();
    //act_stop_refresh->setChecked(curcustomplot->is_replot_suspend);
   // menu.addAction(act_stop_refresh);

  //  menu.addSeparator();
    menu.exec(event->globalPos());

}

void ConfigCurve::AddTitle(QString title)
{
    plotLayout()->insertRow(0);     // pCurveWidget、curveWidget为在ui中定义的QCustomPlot对象指针
    memcpy(m_title,title.toLocal8Bit().data(),strlen(title.toLocal8Bit().data()));
    m_pCurvTitle = new QCPTextElement(this, QString::fromLocal8Bit( m_title));
    QFont  font = this->font();
    font.setFamily("微软雅黑");
    font.setItalic(false);
    font.setBold(false);
    font.setPointSize(13);
    m_pCurvTitle->setFont(font);
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
    ConfigGlobal::updateCurveMap(dataSourceList, this);

}

void ConfigCurve::updateCurveToGlobalInterface()
{
   ConfigGlobal::updateCurveMap(dataSourceList,this);
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
 /*   if(addordelete == 1)
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
    }*/
    clearGraphs();
    curvelist.clear();
    for (auto datasource : dataSourceList)
    {
        QCPGraph* graph = addGraph();
        QString datasourcename = QString::fromLocal8Bit((*ConfigGlobal::m_allDeviceParamPtr)[datasource.toInt()]->m_subParameterName.c_str());
        int datasourceid = datasource.toInt();
        graph->setName(datasourcename);
        curvelist[datasource] = graph;
        QColor  color = colorlist.back();
        colorlist.pop_back();
        graph->setPen(QPen(color));
        if (colorlist.size() == 0)
        {
            colorlist << QColor(250, 0, 0) << QColor(0, 250, 0) << QColor(0, 250, 0);
        }
    }
   ConfigGlobal::updateCurveMap(dataSourceList,this);
   replot();
}
void ConfigCurve::setBaseTime(double basetime)
{
    m_base_time = basetime;
}
void ConfigCurve::updateGraph0Value(int ikey, double itime, double ivalue)
{  
    if (!firstinit)
    {
        m_base_time = itime;
        firstinit = true;
        xvalueMax = xvalueMin = 0;
        yvalueMax = yvalueMin = ivalue;
        graph(0)->addData(0, ivalue);
        return;
    }
    double  newXValue = (itime - m_base_time) / 1000000;
    double  newYValue = ivalue;
   graph(0)->addData(newXValue, newYValue);

    if (newXValue < xvalueMin)
    {
        xvalueMin = newXValue;
    }
    else if (newXValue > xvalueMax)
    {
        xvalueMax = newXValue;
    }
    if (newYValue < yvalueMin)
    {
        yvalueMin = newYValue;
    }
    else if (newYValue > yvalueMax)
    {
        yvalueMax = newYValue;
    }
    update_customplot_range();
    replot(QCustomPlot::rpQueuedReplot);

}
void ConfigCurve::updateValue(int ikey,double itime,double ivalue)
{   
    QString strkey = QString::number(ikey);
    if (!curvelist.contains(strkey))
        return;

     if (!firstinit)
     {
         m_base_time = itime;
         firstinit = true;
         xvalueMax = xvalueMin = 0;
         yvalueMax = yvalueMin = ivalue;
         curvelist[strkey]->addData(0, ivalue);
         return;
     }
     double  newXValue = (itime - m_base_time)/1000000;
     double  newYValue = ivalue;
     curvelist[strkey]->addData(newXValue, newYValue);

     if (newXValue < xvalueMin)
     {
         xvalueMin = newXValue;
     }
     else if (newXValue > xvalueMax)
     {
         xvalueMax = newXValue;
     }
     if (newYValue < yvalueMin)
     {
         yvalueMin = newYValue;
     }
     else if (newYValue > yvalueMax)
     {
         yvalueMax = newYValue;
     }
     update_customplot_range();
     replot(QCustomPlot::rpQueuedReplot);

}
void ConfigCurve::update_customplot_range()
{
    if (!firstinit)
        return;   
    if (xvalueMin == xvalueMax)
    {
        xAxis->setRange(xvalueMin, xvalueMax + 0.1);
    }
    else
    {
        double offset = xvalueMax - xvalueMin;
        if (xrool)
            xAxis->setRange(xvalueMax - xrange, xvalueMax + 1);
        else
            xAxis->setRange(xvalueMin, xvalueMax + 1);

    }
    if (yvalueMin == yvalueMax)
    {
        yAxis->setRange(yvalueMin - 0.1, yvalueMax + 0.1);
    }
    else
    {
        double offset = yvalueMax - yvalueMin;
        yAxis->setRange(yvalueMin - offset * 0.1, yvalueMax + offset * 0.1);
    }
}
void ConfigCurve::UpdatePropertyByStyle()
{
    QMap<int,QPair<bool,QString>>  m_infomap;
    ConfigGlobal::gstylemanager->GetCurveStyle(m_styleID,m_infomap);
    QString fontstr = m_infomap[ConfigCurve::Curve_Title_Font].second;
    if(!fontstr.isNull())
    {
        qDebug() << "curve set font";
        QFont m_textfont;
        m_textfont.fromString(fontstr);        
        m_pCurvTitle->setFont(m_textfont);
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
