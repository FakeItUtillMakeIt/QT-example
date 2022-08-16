#pragma once

#include <QtWidgets/QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include "ui_controlcommand.h"
#include "CenterOperate.h"
#include "../AppCache.h"
#include "../Database/DeviceDAO.h"
#include "../Database/UserDAO.h"
#include "../Database/CommandDAO.h"
#include "../Database/RocketDataDAO.h"
#include "../Controls/MyInfoTip.h"
#include "../Controls/MyLogBox.h" 
#include "../Receiver/ReceiveRocketData.h" 

#include<QTime>
class ControlCommand : public QMainWindow
{
    Q_OBJECT

public:
    ControlCommand(QWidget *parent = Q_NULLPTR);
    void displayStatuInfo(QString info, bool is_alarm = false);
    bool InitFrame(); 
private:
    Ui::ControlCommandClass ui;
    AppCache* m_app;
    bool m_isMax;
    CenterOperate* m_pCenterOperate;
    void Init();
    void CreatConfigInterface();
    DataBase::UserDAO* m_pUserDAO;
    DataBase::DeviceDAO* m_pDeviceDAO;
    DataBase::CommandDAO* m_pCommandDAO;
    DataBase::RocketDataDAO* m_pRocketDataDAO;
    MyLogBox* tb_show;
    MyInfoTip* m_myInfoTip;
    QTime baseTime;
    ReceiveRocketData* m_pReceiveRocketData; //接收测发指令 

    void changeResize();
    map<int, DeviceParam*> m_allDeviceParam;//设备参数

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
