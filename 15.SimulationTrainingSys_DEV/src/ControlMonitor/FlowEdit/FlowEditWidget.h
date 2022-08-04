#pragma once

#ifndef FLOW_EDIT_WIDGET_H
#define FLOW_EDIT_WIDGET_H


#include <QWidget>
#include <QtWidgets>

#include "../FlowEdit/FlowInfoConfig2DB.h"
#include "../FlowEdit/CustomTableWidget.h"

using namespace CUSTOM_TABLE_WIDGET;

/**

	@class   FlowEditWidget
	@brief   流程编排界面
	@details ~

**/
class FlowEditWidget : public QWidget
{
	Q_OBJECT

private:
	FlowEditWidget(QWidget* parent = nullptr);
	~FlowEditWidget();

public:
	static FlowEditWidget* instance;
	static FlowEditWidget* getInstance() {
		if (instance == nullptr)
		{
			instance = new FlowEditWidget;
		}
		return instance;
	}

	static void closeInstance() {
		if (instance)
		{
			instance->deleteLater();
		}
	}

	void setRocketType(QString rocketTypeCode, int rocketId);
	void setMainFlowInfo(QMap<int, QVector<QString>> mainFlowInfo);
	void setSubFlowInfo(QMap<int, QVector<QString>> subFlowInfo);
	void setFlowCmdID(QMap<int, QVector<int>> subFlowCmdID);

private:
	QListWidget* leftFlowItemList;
	QLineEdit* leftTopInput;
	QPushButton* leftTopAdd;

	QLabel* rightTopFlowName;
	CustomTableWidget* rightTable;
	QLabel* rightTopD1;
	QLabel* rightTopD2;
	QLabel* rightTopD3;
	QLabel* rightTopD4;
	QPushButton* rightBottomBtnCancel;
	QPushButton* rightBottomBtnOK;

	QStringList tableColumnName;

	QPoint cursorPos;

	QString rocketType;
	QString rocketCode;
	int rocketID = -1;

	QMap<int, QVector<QString>> mainFlowInfo_;
	QMap<int, QVector<QString>> subFlowInfo_;
	QMap<int, QVector<int>> subFlowCmdID_;


private:
	void InitLayout();
	void initConnection();

	void loadDBSavedFlow();
	void loadFlowDisplayFlow();

private slots:
	void addNewFlow();
	void loadFlowTable();
	void tableCellClick(int row, int column);
	void tableRightMouseClick(QPoint pos);
	void doubleClickCell(int row, int column);
	void tableEditClick();

	void clickOKButton();
	void clickCancelButton();

signals:
	void updateDisPlayFlow();

};


#endif