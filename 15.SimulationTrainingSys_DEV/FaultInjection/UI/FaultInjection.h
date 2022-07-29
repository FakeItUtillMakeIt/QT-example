#pragma once

#include <QtWidgets/QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include "ui_FaultInjection.h"
#include "../AppCache.h"
#include "../Database/FaultDAO.h"
#include "../Database/DeviceDAO.h"
#include "../Database/FaultParamDAO.h"
#include "../Controls/MyInfoTip.h"
#include "../Controls/MyLogBox.h"

#include "CenterOperate.h" 
#include<QTime>
class FaultInjection : public QMainWindow
{
    Q_OBJECT

public:
    FaultInjection(QWidget *parent = Q_NULLPTR);
    void displayStatuInfo(QString info, bool is_alarm = false);
private:
    Ui::FaultInjectionClass ui;
    AppCache* m_app;
    bool m_isMax;
    void Init();
    DataBase::FaultDAO* m_pFaultDAO;
    DataBase::FaultParamDAO* m_pFaultParamDAO;
    DataBase::DeviceDAO* m_pDeviceDAO;

    MyLogBox* tb_show;
    MyInfoTip* m_myInfoTip;
    QTime baseTime;

    CenterOperate* m_centerOperate;

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
    void timeupdate();
    void timecount();
};
