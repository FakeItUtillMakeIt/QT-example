#pragma once

#include <QtWidgets/QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include "ui_controlmonitor.h"
#include "CenterOperate.h"
#include"twoDdisplay.h"
#include "../AppCache.h"
#include "../Database/UserDAO.h"
#include "../Database/DeviceDAO.h"
#include "../Database/CommandDAO.h"
#include "../Database/mainFlowDao.h"
#include "../Controls/MyInfoTip.h"
#include "../Controls/MyLogBox.h"

#include<QTime>
#include<qjsonarray.h>
#include "Light_info.h"
#include<Qfont>

#include "FlowDisplayWidget.h"
class ControlMonitor : public QMainWindow
{
	Q_OBJECT

public:
	ControlMonitor(QWidget* parent = Q_NULLPTR);
	void displayStatuInfo(QString info, bool is_alarm = false);
private:
	Ui::ControlMonitorClass ui;
	AppCache* m_app;
	bool m_isMax;
	CenterOperate* m_pCenterOperate;
	void Init();
	DataBase::UserDAO* m_pUserDAO;
	DataBase::DeviceDAO* m_pDeviceDAO;
	DataBase::CommandDAO* m_pCommandDAO;
	DataBase::mainFlowDao* m_pmainFlowDao;
	MyLogBox* tb_show;
	MyInfoTip* m_myInfoTip;
	QList<Light_info*> light_info;
	int lightnumber;
	bool lightflag;
	QTimer* flush;
	QTime baseTime;
	QFont font;
	void changeResize();
	bool mainflow_finish;
	QLabel* curlabel;

	FlowDisplayWidget* flowDisplayWidget = nullptr;

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
	void zhukongclick();
	void erwei_displayclick();
	void light_load();
	void flash_load();
	void light_flash();
	void lighttest();

	void recvMainFlow(int mainFlowIndex, bool curFlowFlag);
};
