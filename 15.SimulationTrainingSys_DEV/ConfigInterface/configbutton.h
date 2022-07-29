#ifndef CONFIGBUTTON_H
#define CONFIGBUTTON_H
#include <QMouseEvent>
#include <QPushButton>
#include "configdefine.h"
#include "stylebutton.h"

ConfigNameSpaceStart

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

    bool SelectStyle(StyleButton *&istyleinfo);
    void UpdatePropertyByStyle();

    int m_width = 0;
    int m_height = 0;
    int m_xPos = 0;
    int m_yPos = 0;

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
    void moveEvent(QMoveEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    ControlOperateState m_state;
    void handleDragEvent();
    ControlRole  m_ctrlrole = cRealControl;
    ControlType  m_ctrltype = cConfigButton;
    QString m_uuid;
    QString m_groupid;
    QString m_styleid = "";
    QPoint startPos;
    void init_value_set();

    QString normalstyle;
    QString successstyle;
    QString failedstyle;

    void DefaultUiInit();
};
ConfigNameSpaceEnd
#endif // CONFIGBUTTON_H
