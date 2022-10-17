#ifndef ConfigAlarm_H
#define ConfigAlarm_H
#include <QMouseEvent>
#include <QWidget>
#include "configdefine.h"
#include "stylealarm.h"
#include <QPoint>
ConfigNameSpaceStart

class ConfigAlarm:public QWidget
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
        eDataSource
    };
    ConfigAlarm (QWidget *parent = Q_NULLPTR);
    ~ConfigAlarm();
    void InitFromXmlInfo(QMap<QString,QString>& buttoninfo);
    void setState(ControlOperateState state);
    void setRole(ControlRole role);
    void setGroupId(QString groupid);
    QString GetID();
    bool  m_ingroup = false;
    ConfigNameSpace::StyleAlarm * m_styleinfo = nullptr;

    bool SelectStyle(StyleAlarm *&istyleinfo);
    void UpdatePropertyByStyle();
    void setState(RunState state);
    RunState m_RunState = StateNormal;
    int m_width = 0;
    int m_height = 0;
    int m_xPos = 0;
    int m_yPos = 0;
    char m_StyleID[MaximumPathLength];
    char m_ParamSourceID[MaximumPathLength];
    void DefaultUiInit();


    QString m_styleinfoId = "";
    QString m_styleinfoname = "";

    void updateGeometryData();
    void updataDataFromTool();

    QMap<QString,ConfigValueSet>  m_valueSetMap;
    void updateDataSource(QString datasourceid, QString datasourcename, int  addordelete);

    bool UpdateStyleByStyleId(QString styleid);
    void InitFromDefaultStyle();
    void InitFromDefineStyle(QString istyleid);
    QMap<QString, ConfigValueSet>& getPropertyMap();

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent* e);

#pragma region  移动
    void ipressHandler(QMouseEvent* ev);
    void imoveHandler(QMouseEvent* ev);
    void ireleaseHandler(QMouseEvent* ev);
    bool ColliderDetect(QRect  newrect);
    QPoint mLastMousePosition;
    bool mMoving;
#pragma endregion
    void UpdateObjectGeometryLimit();
    void moveEvent(QMoveEvent *event);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent* event);
protected:
    void drawLight(QPainter* painter, QRectF rect);
    void drawEcllipse(QPainter* painter, QRectF ellipseRect);
    void drawImage(QPainter* painter, QRectF rect);
private:
    ControlOperateState m_state;
    void handleDragEvent();
    ControlRole  m_ctrlrole = cRealControl;
    ControlType  m_ctrltype = cConfigAlarm;
    QString m_uuid;
    QString m_groupid  = WidgetFree;
    QString m_styleid = "";
    QPoint startPos;
    bool  dragenabled = false;
    void init_value_set();

    QString m_img_error="";
    QString m_img_init="";
    QString m_img_normal;
    bool m_display_img = false; // 0 颜色  1 图片
    bool m_display_vertically = false; // 0 左  1 下

    QColor m_color_error;
    QColor m_color_init;
    QColor m_color_normal;
    QString m_text = "1234";
    QFont m_text_font;
    QColor m_text_color;
private slots:
    void showListMenu(const QPoint& point);
};
ConfigNameSpaceEnd
#endif // ConfigAlarm_H
