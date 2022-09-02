﻿#ifndef STYLECURVE_H
#define STYLECURVE_H
#include "configdefine.h"
#include "qcustomplot.h"
#include "stylecurve.h"

ConfigNameSpaceStart
class ConfigCurve:public QCustomPlot
{
    Q_OBJECT
public:

    enum  ValueIndexEnum
    {
        eXPos = 0,
        eYPos,
        eWidth,
        eHeight,
        eStyleInfo,
        eMultiDataSource,
        eTitleInfo,
    };
    enum  CurveInfo
    {
       Curve_Style_ID = 0,
       Curve_Style_Name,
       Curve_Common_Color, //背景颜色
       Curve_Title_Show,//是否显示标题
       Curve_Title_Font, //标题文字字体
       Curve_Title_Color//标题文字颜色
    };
    ConfigCurve(QWidget *parent = nullptr);
    void updateGeometryData();
    void initDataSourcesFromXmlData();
    void updateCurveToGlobalInterface();
    void updataDataFromTool();
    void UpdatePropertyByStyle();
    void update_customplot_range();

    QMap<QString,ConfigValueSet>  m_valueSetMap;
    QString GetID();
    void InitFromXmlInfo(QMap<QString, QString> &curveinfo);
    void updateCurve(QString datasourceid, QString datasourcename, int addordelete);
    void updateValue(int ikey, double itime, double ivalue);
    void updateGraph0Value(int ikey, double itime, double ivalue);

    void setBaseTime(double basetime);

protected:
    void mousePressEvent(QMouseEvent *ev);
    void moveEvent(QMoveEvent *event);
    void resizeEvent(QResizeEvent *event);
    void contextMenuEvent(QContextMenuEvent* event);
private:
    QString m_uuid;
    void init_value_set();
    QList<QString>  dataSourceList;

    int m_width;
    int m_height;
    int m_xPos;
    int m_yPos;
    char m_styleID[MaximumPathLength];
    char m_title[MaximumPathLength];
    QCPTextElement* m_pCurvTitle;
    void AddTitle(QString title);
    QList<QColor>  colorlist;
    QMap<QString,QCPGraph*>  curvelist; //key 是  参数索引
  //  void timerEvent(QTimerEvent* event);

    //记录范围
    double xvalueMax = 0;
    double xvalueMin =0;
    double yvalueMax = 0;
    double yvalueMin = 0;
    bool firstinit = false;
    double m_base_time;

    bool xrool = true;
    double xrange = 20;

};
ConfigNameSpaceEnd
#endif // STYLECURVE_H
