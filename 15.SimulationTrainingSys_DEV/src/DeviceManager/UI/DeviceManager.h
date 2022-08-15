#pragma once
#pragma execution_character_set("utf-8")

#ifndef DEVICE_MANEGER_H
#define DEVICE_MANEGER_H


#include <QtWidgets/QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include "ui_DeviceManager.h"
#include "CenterOperate.h"
#include "../AppCache.h"
#include "../Database/DeviceDAO.h"
#include "../Database/CommandDAO.h"
#include "../Database/RocketDataDAO.h"
#include "../Controls/MyInfoTip.h"
#include "../Controls/MyLogBox.h"
#include "../Helper/DeviceHelper.h"
#include "CenterOperate.h"
#include "../MutliThread/MutliThreadOp.h"

class DeviceManager : public QMainWindow
{
	Q_OBJECT

public:
	DeviceManager(QWidget* parent = Q_NULLPTR);
	void displayStatuInfo(QString info, bool is_alarm = false);
	void Init();
	void ShowMinimized();
private:
	Ui::DeviceManagerClass ui;
	AppCache* m_app;
	bool m_isMax;
	CenterOperate* m_pCenterOperate;
	DataBase::DeviceDAO* m_pDeviceDAO;
	DataBase::CommandDAO* m_pCommandDAO;
	DataBase::RocketDataDAO* m_pRocketDataDAO;
	MyLogBox* tb_show;
	MyInfoTip* m_myInfoTip;

	CenterOperate* m_centeralWidget;
	void InitDevice();
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


	void timeUpdate();

signals:
	void rocketTypeChanged();

	void deviceLoadOver();

};


#endif