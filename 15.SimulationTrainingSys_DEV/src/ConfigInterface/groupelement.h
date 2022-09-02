#ifndef GROUPELEMENT_H
#define GROUPELEMENT_H

#include "moveableframe.h"
#include "configdefine.h"
#include "ctrolfactory.h"
#include "stylegroup.h"
#include <QGridLayout>
#include <QPushButton>
#include <QWidget>
#include "configbutton.h"
#include "configglobal.h"
#include "configpairlabel.h"
#include "configalarm.h"

ConfigNameSpaceStart
class ConfigScene;
class GroupElement : public QWidget
{
    Q_OBJECT
public:
    enum  ValueIndexEnum
    {
        eRowValue,
        eColValue,
        eColSpace,
        eRowSpace,
        eRowSpan,
        eColSpan,
        eLeftMargin,
        eRightMargin,
        eTopMargin,
        eTitleHeight,
        eXPos,
        eYPos,
        eWidth,
        eHeight,
        eStyleInfo,
        eTitleInfo
    };

    struct RectInfo
    {
        QRect rect;
        bool show = false;
        int index = 0;
        bool selected = false;
    };

    explicit GroupElement(QWidget *parent = 0);
    CtrolFactory* ctrlfactory;
    QString GetID();
    ConfigScene* m_scene = nullptr;

    QList<ConfigPairLabel*> pairlabellist;
    QList<ConfigAlarm*>     alarmlist;

    QList<ConfigButton*> buttonlist;
    QList<ConfigPairLabel*>& getPairLabelList();
    QList<ConfigAlarm*>& getAlarmList();
    QList<ConfigButton*>& getButtonList();
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
public:
    int m_width;
    int m_height;
    int m_xPos;
    int m_yPos;
    void updateGeometryData();
    void updataDataFromTool();//从配置中获取最新数据
    void updataGeometryFromData();//位置数据更新，根据更新数据设置位置信息
    void updataDataSouce();//从配置中获取最新数据
    void updateFromValueSet();
    void InitFromDefaultStyle();
    void InitFromDefineStyle(QString istyleid);
    void copyProperty(QString styleid, QMap<int, int>& propertylist);
    void moveToCenterPos(QPoint centerpos);

    QMap<int, int> & getPropertyList();
    QString getStyleId();
private:
    int m_row = 5;
    int m_col = 2;
    int m_colspace = 20;
    int m_rowspace = 20;

    int m_rowspan = 30;
    int m_colspan = 100;
    int m_leftmagin = 20;
    int m_rightmargin = 20;
    int m_topmagin = 60;
    int m_titleheight = 45;

    char titletext[MaximumPathLength];
    char m_styleID[MaximumPathLength];


    RectInfo get_rect(int irow, int icol, int rowspan, int colspan);
  //  QList<RectInfo>  rectlist;
    QMap<int,QWidget*>  widgetmap;
    QMap<int,RectInfo>  rectmap;
    void addElementBack(RectInfo &rectinfo, QString text, ControlType ctrltype);
    void addElement(RectInfo &rectinfo, QString text, ControlType ctrltype);
    bool resizing = false;
    void moveElement(RectInfo &rectinfo, int index);
    QString  m_uuid;
    void drawTitle(QPainter &painter);
    void drawDeleteEntry(QPainter &painter);
    void drawTitleText(QPainter &painter);
    void drawBackGround(QPainter &painter);

    bool isdeletetriggerd(QPoint curpos);
    void handleDragEvent();
    bool  handlertriggerd = false;
    QPoint  handlertriggerdPos;
    void stateInit();
    void delSelectedElements();
    void removeWidgetFromList(QWidget* widget);

    QImage  deleteicon;
    QImage  deleteiconpressed;
    bool deletehoverd = false;


    QRect deleteentry;
    bool  deletehitted = false;

    QRect titletextentry;
    bool  titletexthitted = false;
   // QString titletext ="未设置";

    QString  titlepicture = "";


private: // 配置信息
    QMap<QString,ConfigValueSet>  m_valueSetMap;

    void init_value_set();

    void updateGeometryToTool();
    void resetLaoutFromData();

public:
    bool SelectStyle(StyleGroup *&stylegroup);
    void UpdatePropertyByStyle();

    QMap<QString, ConfigValueSet> &getPropertyMap();
    void InitFromXmlInfo(GroupElementInfo &groupElementInfo);
private:
    ConfigNameSpace::StyleGroup * m_styleinfo = nullptr;
    QPixmap  m_backgroundpicture;
    QPixmap  m_titlepicture;
    QFont    m_titlefont;
    QColor   m_titlecolor;
    QPixmap  m_iconpicture;
    QMap<int, int> m_propertylist;
    bool UpdateStyleByStyleId(QString styleid);
    void RestoreButtonFromXml(GroupElementInfo &groupElementInfo);
    void RestorePairLabelFromXml(GroupElementInfo &groupElementInfo);
    void RestoreAlarmFromXml(GroupElementInfo& groupElementInfo);
    void AddButtonElement(QMap<QString, QString> &buttoninfo);
    RectInfo &GetRectInfoByGeometry(QRect rect);
    void AddPairLabelElement(QMap<QString, QString> &buttoninfo);
    void AddAlarmElement(QMap<QString, QString>& alarminfo);

    void DefaultUiInit();
    void drawIconEntry(QPainter &painter);
};
ConfigNameSpaceEnd
#endif // GROUPELEMENT_H
