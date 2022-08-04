#pragma once

#include <QtWidgets/QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include "ui_MyFramework.h"
#include "CenterOperate.h"
#include "../AppCache.h"
#include "../Database/UserDAO.h"
#include "../Controls/MyInfoTip.h"
#include "../Controls/MyLogBox.h"
class MyFramework : public QMainWindow
{
    Q_OBJECT

public:
    MyFramework(QWidget *parent = Q_NULLPTR);
    void displayStatuInfo(QString info, bool is_alarm = false);
private:
    Ui::MyFrameworkClass ui;
    AppCache* m_app;
    bool m_isMax;
    CenterOperate* m_pCenterOperate;
    void Init();
    DataBase::UserDAO* m_pUserDAO;
    MyLogBox* tb_show;
    MyInfoTip* m_myInfoTip;

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
