﻿#ifndef CONFIGBUTTON_H
#define CONFIGBUTTON_H
#include <QMouseEvent>
#include <QPushButton>
#include "configdefine.h"
#include "stylebutton.h"

ConfigNameSpaceStart
class ConfigScene;
class ConfigButton:public QPushButton
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
    ConfigButton(const QString &text, QWidget *parent = Q_NULLPTR);
    ~ConfigButton();
    void InitFromXmlInfo(QMap<QString,QString>& buttoninfo);
    void setState(ControlOperateState state);
    void setRole(ControlRole role);
    void setGroupId(QString groupid);
    QString GetID();
    bool  m_ingroup = false;
    ConfigNameSpace::StyleButton * m_styleinfo = nullptr;
    ConfigScene* m_scene = nullptr;
    bool SelectStyle(StyleButton *&istyleinfo);
    void UpdatePropertyByStyle();
    void setState(RunState state);
    void Schedule();
    int m_width = 0;
    int m_height = 0;
    int m_xPos = 0;
    int m_yPos = 0;
    char m_StyleID[MaximumPathLength];
    char m_CmdSourceID[MaximumPathLength];

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
protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent* e);

    void moveEvent(QMoveEvent *event);
    void resizeEvent(QResizeEvent *event);

#pragma region  移动
    void ipressHandler(QMouseEvent* ev);
    void imoveHandler(QMouseEvent* ev);
    void ireleaseHandler(QMouseEvent* ev);
    QPoint mLastMousePosition;
    bool mMoving;
#pragma endregion
private:
    ControlOperateState m_state;
    void handleDragEvent();
    ControlRole  m_ctrlrole = cRealControl;
    ControlType  m_ctrltype = cConfigButton;
    QString m_uuid;
    QString m_groupid = WidgetFree;
    QString m_styleid = "";
    QPoint startPos;
    bool  dragenabled = false;
    void init_value_set();

    QString normalstyle;
    QString successstyle;
    QString failedstyle;

    void UpdateObjectGeometryLimit();

private slots:
    void showListMenu(const QPoint& point);
    void handleEvent();
};
ConfigNameSpaceEnd
#endif // CONFIGBUTTON_H
