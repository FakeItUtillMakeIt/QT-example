#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_equivalentsoft.h" 
#include "CenterOperate.h"
#include <QMouseEvent>
#include "../AppCache.h"
#include "../Database/DeviceDAO.h"
#include "../Database/CommandDAO.h"
#include "../Database/RocketDataDAO.h"
#include "../Database/UserDAO.h"
#include "../Controls/MyInfoTip.h"
#include "../Controls/MyLogBox.h"
#include "../Receiver/ReceiveRocketData.h"

#include<QTime>

class EquivalentSoft : public QMainWindow
{
    Q_OBJECT

public:
    EquivalentSoft(QWidget *parent = Q_NULLPTR);
    void displayStatuInfo(QString info, bool is_alarm = false);
    bool InitFrame();
    void CreatConfigInterface();

private:
    Ui::EquivalentSoftClass ui;
    AppCache* m_app; 
    bool m_isMax;
    CenterOperate* m_pCenterOperate;
    void Init();
    DataBase::UserDAO* m_pUserDAO;
    DataBase::DeviceDAO* m_pDeviceDAO;
    DataBase::CommandDAO* m_pCommandDAO;
    DataBase::RocketDataDAO* m_pRocketDataDAO;
    MyLogBox* tb_show;
    MyInfoTip* m_myInfoTip;
    QTime baseTime;
    QWidget* hbj;
    QWidget* hbj2;
    QWidget* cur;
    QPushButton* btnToDelete = nullptr;
    int count;
    ReceiveRocketData* m_pReceiveRocketData; //接收测发指令
    void AutoAddDxq(QPushButton* selbtn, QPushButton* delbtn); 
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
    void adddxq();
    void closehbj();
    void closehbj2();
    void deletedxq();
    void adddxq_ok();
    void delete_ok();
    void selected();
    void editdxq();
   
};
