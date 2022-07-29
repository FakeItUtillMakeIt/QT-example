#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H
#include "configdefine.h"
#include <QPaintEvent>
#include <QWidget>


ConfigNameSpaceStart
class ControlPoint:public QWidget
{
    Q_OBJECT
public:
    enum  Direct
    {
        dTopLeft,
        dBottomLeft,
        dTopRight,
        dBottomRight,
        dTopMiddle,
        dBottomMiddle,
        dLeftMiddle,
        dRightMiddle,
    };
    enum  CtrolType
    {
        cMoveCtrol,
        cResizeControl
    };
    ControlPoint(int size = 10,Direct direct = dTopLeft,QWidget* parent = nullptr);
    CtrolType m_controltype  = cResizeControl;

    void update_contrlwidget(QWidget* controlwidget);
    void enable_move(bool bval);
    void setControlTyle(CtrolType ctrltype);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *e);
private:
    QPoint dragPosition;
    QPoint lastPosition;
    int m_size = 10;
    QWidget* m_ctrlwidget;
    QRect m_avaiblegeometry;
    bool m_moveenabled = false;
    Direct m_direct;
    void eventHandler(QPoint& interval);
    QRect& ajust_distance(QRect &rect, QPoint &interval);
signals:
    void control_moved(QPoint interval);
};
ConfigNameSpaceEnd
#endif // CONTROLPOINT_H
