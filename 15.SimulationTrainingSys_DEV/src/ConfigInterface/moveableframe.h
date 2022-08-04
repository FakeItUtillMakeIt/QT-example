#ifndef MOVEABLEFRAME_H
#define MOVEABLEFRAME_H

#include "controlpoint.h"
#include "configdefine.h"
#include <QFocusEvent>
#include <QWidget>

ConfigNameSpaceStart
class MoveAbleFrame:public QObject
{
public:
    MoveAbleFrame(QObject* parent = nullptr);
    void focusInEvent(QFocusEvent *event);

    static void CreateControl();
    static void hide_ctrl_point();
    static void update_ctrl_point_pos(QWidget *controledWidget, QWidget *parenWidget);
    static void update_ctrl_point_pos_2();
    static void setControlStyle(ControlType  m_ctrtype,bool ingroup,QString uuid);
    static void GetControlWidget(ControlType& ctrtype, QString& uuid, bool& ingroup);
protected:
    void focusOutEvent(QFocusEvent *event);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void moveEvent(QMoveEvent *event);

private:
    static ControlPoint*  tlPoint;
    static ControlPoint*  blPoint;
    static ControlPoint*  trPoint;
    static ControlPoint*  brPoint;

    static ControlPoint*  tmPoint;
    static ControlPoint*  bmPoint;
    static ControlPoint*  lmPoint;
    static ControlPoint*  rmPoint;
    static QWidget* g_controledWidget;
    static QWidget* g_pointParent;

    static QList<ControlPoint*> ctrllist;
   // void update_ctrl_point_pos();
    static QPoint transPoint(QPoint point,ControlPoint::Direct direct = ControlPoint::dBottomLeft);
    static QPoint transPoint(int x, int y,ControlPoint::Direct direct = ControlPoint::dBottomLeft);
    static int ctrlsize;
    QString prestyle = "";
    bool focused = false;
    static ControlType  m_ctrtype;
    static bool   controlenabled;
    static bool   beingroup;
    static QString m_controledUuid;

};
ConfigNameSpaceEnd
#endif // MOVEABLEFRAME_H
