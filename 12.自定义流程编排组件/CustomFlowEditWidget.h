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

#define _USE_SPAN_
#define _USE_TABLE_ENBLED_

namespace CUSTOM_FLOW_EDIT_WIDGET {


	class CustomFlowEditWidget : public QWidget
	{
		Q_OBJECT

	public:
		CustomFlowEditWidget(QWidget *parent=nullptr);
		~CustomFlowEditWidget();

	private:
		//◊Û≤‡¿∏
		QLineEdit* leftSearchInput;
		QPushButton* leftSearchButton;
		QPushButton* leftAddFlowButton;
		QListWidget* leftProjectFlowList;

		//”“≤‡¿∏ …œ≤‡
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
		int rowHeight;
		int columnWidth;

		QMap<int, QVector<int>> mergeCell;
		QMap<int, QVector<int>> rowInclude;

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
	};


}

#endif