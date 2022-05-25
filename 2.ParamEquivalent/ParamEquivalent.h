#ifndef PARAM_EQUIVALMENT_H
#define PARAM_EQUIVALMENT_H

#endif // !PARAM_EQUIVALMENT_H


#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ParamEquivalent.h"
#include <QFileDialog>
#include <QStringList>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QMimeData>
#include <QPainter>
#include <QLabel>

class ParamEquivalent : public QMainWindow
{
    Q_OBJECT

public:
    ParamEquivalent(QWidget *parent = Q_NULLPTR);

private:
    Ui::ParamEquivalentClass ui;


private:
    bool initSignalSlot_fun();

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);
    void mousePressEvent(QMouseEvent* event);


private slots:
    void pluginFile_btn();
    void addNewPage_btn();
    void deletePage_btn();
    void savePage_btn();



};
