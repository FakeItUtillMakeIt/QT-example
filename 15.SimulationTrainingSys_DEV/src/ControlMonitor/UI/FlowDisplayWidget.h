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

	//������ID��ָ����Ϣ�����ƣ�
	QMap<int, QVector<QString>> subFlowInfo;
	//������ID��ָ����Ϣ�����
	QMap<int, QVector<QString>> subFlowInfo1;
	//������ID��ָ����Ϣ����ע��
	QMap<int, QVector<QString>> subFlowInfo2;

	//������ID��ָ��
	QMap<int, QVector<int>> subFlowCmdID;
	//������ID��������ID��Ӧ��ϵ
	QMap<int, QVector<int>> mainID2SubID;

	//�������ɽ���
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