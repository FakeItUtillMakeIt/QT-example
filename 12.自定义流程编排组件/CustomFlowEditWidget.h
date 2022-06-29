#pragma once

#ifndef CUSTOM_FLOW_EDIT_WIDGET_H
#define CUSTOM_FLOW_EDIT_WIDGET_H


#include <QtWidgets>
#include <QWidget>
#include <QtXlsx/QtXlsx>
#include <QtXlsx>

#include "CustomTableWidget.h"
#include "SubTableWidget.h"
using namespace CUSTOM_TABLE_WIDGET;
using namespace SUB_TABLE_WIDGET;

#define _USE_SPAN_
#define _USE_TABLE_ENBLED_


/**
    @namespace CUSTOM_FLOW_EDIT_WIDGET
    @brief     自定义流程编辑窗口
**/
namespace CUSTOM_FLOW_EDIT_WIDGET {


	class CustomFlowEditWidget : public QWidget
	{
		Q_OBJECT

	public:
		CustomFlowEditWidget(QWidget *parent=nullptr);
		~CustomFlowEditWidget();

	private:
		//左侧栏
		QLineEdit* leftSearchInput;
		QPushButton* leftSearchButton;
		QPushButton* leftAddFlowButton;
		QListWidget* leftProjectFlowList;

		//右侧栏 上侧
		QLabel* rightFlowNameLabel;
		QPushButton* rightFlowEditButton;
		QPushButton* rightFlowDeleteButton;
		QPushButton* rightFlowUpdateButton;

		CustomTableWidget* rightFlowTable;

		QPushButton* bottomCancleButton;
		QPushButton* bottomOKButton;

		QStringList tableColumnKey;

	private:
		int currentFlowIndex;
		QPoint cursorPos;
		int rowHeight=60;
		int columnWidth;

		QMap<int, QVector<int>> mergeCell;
		QMap<int, QVector<int>> rowInclude;
		QMap<int, int> rowMaxRowCount;
	private:
		void initConnection();
		void setStylize();

	

	private slots:
		void addNewFlow();
		void loadFlowTable();
		void tableCellClick(int row, int column);
		void tableRightMouseClick(QPoint pos);
		void tableEditClick();
		void doubleClickTable(int row, int column);
		void adjustCellHeight(QPoint);

		void okButtonClick();
		void cancelButtonClick();
	};


}

#endif