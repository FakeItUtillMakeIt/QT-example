#ifndef CONFIGTABBUTTON_H
#define CONFIGTABBUTTON_H

#include <QMouseEvent>
#include <QPushButton>
#include "configdefine.h"
ConfigNameSpaceStart
class ConfigScene;
class ConfigTabButton:public QPushButton
{
public:
    ConfigTabButton(const QString& text,QWidget* parent = Q_NULLPTR);
    ConfigTabButton(QWidget* parent = Q_NULLPTR);
    ConfigScene* configscene = nullptr;
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void dropEvent(QDropEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
private:
    void handleDragEvent();
    QPoint startPos;
};
ConfigNameSpaceEnd
#endif // CONFIGTABBUTTON_H
