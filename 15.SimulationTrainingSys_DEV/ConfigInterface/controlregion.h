#ifndef CONTROLREGION_H
#define CONTROLREGION_H

#include "controlpoint.h"
#include "configdefine.h"
#include <QMouseEvent>
#include <QWidget>


ConfigNameSpaceStart
class ControlRegion:public QWidget
{
    Q_OBJECT
public:
    ControlRegion(QWidget* parent = nullptr);
    QWidget * containwid = nullptr;

private:
    void load_ctrl_point();
    bool  inited = false;
    void update_ctrl_point_pos();
protected:
    void showEvent(QShowEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void resizeEvent(QResizeEvent *event);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *e);
private:
    QPoint dragPosition;
    bool dragenabled = false;
private:
    ControlPoint*  tlPoint = nullptr;
    ControlPoint*  blPoint = nullptr;
    ControlPoint*  trPoint = nullptr;
    ControlPoint*  brPoint = nullptr;

    ControlPoint*  tmPoint = nullptr;
    ControlPoint*  bmPoint = nullptr;
    ControlPoint*  lmPoint = nullptr;
    ControlPoint*  rmPoint = nullptr;
    void show_ctrl_point(bool enabled);
signals:
    void control_changed();
    void sig_selected(QWidget* widget);
signals:
    void delete_me();
    void replace_element();
};
ConfigNameSpaceEnd
#endif // CONTROLREGION_H
