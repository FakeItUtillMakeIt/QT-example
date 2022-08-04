#ifndef CONFIGLABEL_H
#define CONFIGLABEL_H
#include "configdefine.h"
#include <QLabel>
#include <QMouseEvent>

ConfigNameSpaceStart

class ConfigLabel:public QLabel
{
    Q_OBJECT
public:
    ConfigLabel(QWidget *parent=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags());
    void setState(ControlOperateState state);
    void setRole(ControlRole role);
    void setGroupId(QString groupid);
    bool  m_ingroup = false;

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void moveEvent(QMoveEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    ControlOperateState m_state;
    void handleDragEvent();
    ControlRole  m_ctrlrole = cRealControl;
    ControlType  m_ctrltype = cConfigLabel;
    QString m_uuid;
    QString m_groupid;
    QString m_styleid = "";
    QPoint startPos;

    void updateGeometryData();
};
ConfigNameSpaceEnd
#endif // CONFIGLABEL_H
