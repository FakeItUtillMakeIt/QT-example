#pragma once
#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include "ui_DeviceManager.h"
#include "CenterOperate.h"
#include "../AppCache.h"
#include "../Database/DeviceDAO.h"
#include "../Database/CommandDAO.h"
#include "../Controls/MyInfoTip.h"
#include "../Controls/MyLogBox.h"

#include "CenterOperate.h"

class DeviceManager : public QMainWindow
{
    Q_OBJECT

public:
    DeviceManager(QWidget *parent = Q_NULLPTR);
    void displayStatuInfo(QString info, bool is_alarm = false);
private:
    Ui::DeviceManagerClass ui;
    AppCache* m_app;
    bool m_isMax;
    CenterOperate* m_pCenterOperate;
    void Init();
    DataBase::DeviceDAO* m_pDeviceDAO;
    DataBase::CommandDAO* m_pCommandDAO; 
    MyLogBox* tb_show;
    MyInfoTip* m_myInfoTip;

    CenterOperate* m_centeralWidget;

    void changeResize();
protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    QPoint mLastMousePosition;
    bool mMoving;

    void paintEvent(QPaintEvent* event);
private slots:
    void CloseWindow();
    void ShowMinimized();
};
