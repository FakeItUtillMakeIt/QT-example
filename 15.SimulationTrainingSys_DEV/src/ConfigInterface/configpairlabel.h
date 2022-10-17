#ifndef CONFIGPAIRLABEL_H
#define CONFIGPAIRLABEL_H
#include "configdefine.h"
#include <QLabel>
#include <QWidget>
#include "stylepairlabel.h"
#include <qaction.h>
#include <qmenu.h>
#include "../Model/DeviceParam.h"

ConfigNameSpaceStart
class ConfigPairLabel :public QWidget
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

    ConfigPairLabel(QWidget* parent = nullptr);
    void setName(QString name);
    void setValue(QString value);
    void updateGeometryData();
    void updataDataFromTool();
    void UpdateObjectGeometryLimit();
    bool SelectStyle(StylePairLabel *&istyleinfo);
    void UpdatePropertyByStyle();
    bool  m_ingroup = false;
    QString GetID();
    QMap<QString,ConfigValueSet> & getPropertyMap();

    void setRole(ControlRole role);
    void setGroupId(QString groupid);
    void InitFromXmlInfo(QMap<QString, QString> &buttoninfo);
    void updateDataSource(QString datasourceid, QString datasourcename, int  addordelete);
    void InitFromDefaultStyle();
    void InitFromDefineStyle(QString istyleid);
    void updateValue(double value, int error = 0);
    void updateText(QString text, int error = 0);
    void DefaultUIInit();

protected:
    void mousePressEvent(QMouseEvent *event);
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
    QLabel* namelabel;
    QLabel* valuelabel;
    int m_nameLabelStrech =5;
    int m_valueLabelStrech = 3;
    int m_width;
    int m_height;
    int m_xPos;
    int m_yPos;
    char m_StyleID[MaximumPathLength];
    char m_ParamSourceID[MaximumPathLength];
    int errorstate = 0;
    QString m_styleinfoId = "";
    QString m_styleinfoname = "";

    int m_space = 5;

    QString m_namelabelstyle;
    QString m_valuelabelstyle;
    QString m_error_namelabelstyle;
    QString m_error_valuelabelstyle;
    QFont  m_textfont;
    QColor  m_textcolor;

    QLabel* paramnamelabel;
    QLabel* paramvaluelabel;
    QHBoxLayout* paramlayout;
    QMap<QString,ConfigValueSet>  m_valueSetMap;    

    ConfigNameSpace::StylePairLabel * m_styleinfo = nullptr;


    void init_value_set();
    void updataGeometryFromData();

    QPoint startPos;
    QString m_groupid = WidgetFree;
   ControlRole  m_ctrlrole = cRealControl;
    ControlType  m_ctrltype = cConfigPairLabel;
    QString m_uuid;
    void handleDragEvent();
    bool UpdateStyleByStyleId(QString styleid);
    bool  dragenabled = false;

private slots:
    void showListMenu(const QPoint& point);
};
ConfigNameSpaceEnd
#endif // CONFIGPAIRLABEL_H
