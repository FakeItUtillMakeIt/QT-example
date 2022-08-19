#pragma once

#ifndef FLOW_DISPLAY_WIDGET_H
#define FLOW_DISPLAY_WIDGET_H


#include <QWidget>
#include <QtWidgets>
#include "../AppCache.h"
#include "../FlowEdit/FlowInfoConfig2DB.h"
#include "../FlowEdit/FlowEditWidget.h"
#include "../FlowEdit/GenerateFlowCmdWidget.h"

class GenerateFlowCmdWidget;

class FlowDisplayWidget : public QWidget
{
	Q_OBJECT

public:
	FlowDisplayWidget(QWidget* parent = nullptr, QString rocketCode = "", int rocketID = -1);
	~FlowDisplayWidget();

	void setRocketType(QString, int);
	AppCache* m_app;
private:
	QLabel* flowIcon;
	QLabel* flowLabel;
	QPushButton* addFlow;
	QPushButton* loadFlow;
	QTableWidget* flowTable;

	QString flowName;
	QStringList rowHeader;

	FlowEditWidget* flowEditWidget;

	int rocketID = -1;

	QMap<int, QVector<QString>> mainFlowInfo;

	//主流程ID到指令信息（名称）
	QMap<int, QVector<QString>> subFlowInfo;
	//主流程ID到指令信息（回令）
	QMap<int, QVector<QString>> subFlowInfo1;
	//主流程ID到指令信息（备注）
	QMap<int, QVector<QString>> subFlowInfo2;

	//主流程ID到指令
	QMap<int, QVector<int>> subFlowCmdID;
	//主流程ID到子流程ID对应关系
	QMap<int, QVector<int>> mainID2SubID;

	//流程生成界面
	GenerateFlowCmdWidget* generateFlowWidget;

public slots:
	void updateFlowStat(int m_iCode, int sendICode);

private:
	void InitLayout();
	
private slots:
	void addNewFlow();
	void loadSavedFlow();

signals:
	void updateMainFlowAndSubFlow(int mainIndex, QString curRunCmdName, QString backCmdInfo);
	void sendMainFlowInfo(int mainFlowIndex, bool curFlowFlag);

	void updateFlowOver();
	void sendMainflowchange();

};


#endif