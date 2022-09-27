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
#include"twoDdisplay.h"

#include "../Database/FaultDAO.h"
#include "../Database/DeviceDAOF.h"
#include "../Database/FaultParamDAO.h"
#include "CenterOperateF.h" 
#include "TaskConfiguration.h"

#include <thread>

//�Ż����UI
#define NEW_UI
#ifndef NEW_UI
#define OLD_UI
#endif // NEW_UI

//ɾ������
//#define __DELETE_ONLY__
#ifndef __DELETE_ONLY__
#define __DELETE_RELE_TABLE__
#endif

class DeviceManager : public QMainWindow
{
	Q_OBJECT

public:
	DeviceManager(QWidget* parent = Q_NULLPTR);
	void displayStatuInfo(QString info, bool is_alarm = false);
	void Init();
	void DataFaultLoad();
	void TaskManagement();
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

	//����ע�����
	DataBaseF::FaultDAO* m_pFaultDAOF;
	DataBaseF::FaultParamDAO* m_pFaultParamDAOF;
	DataBaseF::DeviceDAO* m_pDeviceDAOF;
	CenterOperateF* m_CenterOperateF;
	QHBoxLayout* m_hlayoutF;

	//���������λ����
	TaskConfiguration* m_taskConfiguration;
	QHBoxLayout* m_hlayouttask;

	//��¼֮ǰ���ͺ�
	QString preRocket;
	QString curSelectedText;

	//֪ʶ������
	QHBoxLayout* m_hlayoutdisplay;
	twoDdisplay* m_twoDdisplay;


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