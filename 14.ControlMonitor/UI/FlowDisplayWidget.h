#pragma once

#ifndef FLOW_DISPLAY_WIDGET_H
#define FLOW_DISPLAY_WIDGET_H


#include <QWidget>
#include <QtWidgets>

#include "../FlowEdit/FlowInfoConfig2DB.h"
#include "../FlowEdit/FlowEditWidget.h"
#include "../FlowEdit/GenerateFlowCmdWidget.h"

class FlowDisplayWidget : public QWidget
{
	Q_OBJECT

public:
	FlowDisplayWidget(QWidget* parent = nullptr, QString rocketCode = "", int rocketID = -1);
	~FlowDisplayWidget();

	void setRocketType(QString, int);

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
	//主流程ID到指令信息
	QMap<int, QVector<QString>> subFlowInfo;
	//主流程ID到指令
	QMap<int, QVector<int>> subFlowCmdID;

	//流程生成界面
	GenerateFlowCmdWidget* generateFlowWidget;

private:
	void InitLayout();

private slots:
	void addNewFlow();
	void loadSavedFlow();

};


#endif