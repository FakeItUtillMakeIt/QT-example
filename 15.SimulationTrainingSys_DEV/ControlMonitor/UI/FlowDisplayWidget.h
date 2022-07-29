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
	//������ID��ָ����Ϣ
	QMap<int, QVector<QString>> subFlowInfo;
	//������ID��ָ��
	QMap<int, QVector<int>> subFlowCmdID;

	//�������ɽ���
	GenerateFlowCmdWidget* generateFlowWidget;

private:
	void InitLayout();

private slots:
	void addNewFlow();
	void loadSavedFlow();

};


#endif