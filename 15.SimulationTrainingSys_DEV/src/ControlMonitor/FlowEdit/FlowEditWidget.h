#pragma once

#ifndef FLOW_EDIT_WIDGET_H
#define FLOW_EDIT_WIDGET_H


#include <QWidget>
#include <QtWidgets>


#include "../FlowEdit/FlowInfoConfig2DB.h"
#include "../FlowEdit/CustomTableWidget.h"
#include "../FlowEdit/ItemDelegate.h"


using namespace CUSTOM_TABLE_WIDGET;


class MainFlowInfoSt;
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
	void setSubFlowInfo1(QMap<int, QVector<QString>> subFlowInfo1);
	void setSubFlowInfo2(QMap<int, QVector<QString>> subFlowInfo2);

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
	QMap<int, QVector<QString>> subFlowInfo_;//子流程口令
	QMap<int, QVector<QString>> subFlowInfo1_;//子流程回令
	QMap<int, QVector<QString>> subFlowInfo2_;//子流程备注

	QMap<int, QVector<int>> subFlowCmdID_;

	QVector<QString> hadUsedCmdList;
	QVector<QString> unUsedCmdList;
	QVector<QString> hadSaveStage;

	QVector<QComboBox*> cmdComboBoxList;//存储所有流程编辑界面的combox

	QMap<QString, int> cmdName2Id;

	MainFlowInfoSt* tableFlowInfoHeader;


private:
	void InitLayout();
	void initConnection();

	void loadDBSavedFlow();
	void loadFlowDisplayFlow();

	bool eventFilter(QObject* obj, QEvent* event);

	//
	void recurseUpdate(MainFlowInfoSt* node, int type);
	void removeRowFrom2Index(int startRow, int rowSpan);
	void updateSortIndex(MainFlowInfoSt* header);
	void addRow(int rowIndex);

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

/**

	@class   MainFlowInfoSt
	@brief   主流程信息结构
	@details ~

**/
class MainFlowInfoSt {
public:
	MainFlowInfoSt* next = nullptr;
	int sortNum;
	int startRowNum;
	int rowSpanCount;
};

#endif