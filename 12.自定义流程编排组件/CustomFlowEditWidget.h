#pragma once

#ifndef CUSTOM_FLOW_EDIT_WIDGET_H
#define CUSTOM_FLOW_EDIT_WIDGET_H


#include <QtWidgets>
#include <QWidget>
#include <QtXlsx/QtXlsx>
#include <QtXlsx>

#include "CustomTableWidget.h"
using namespace CUSTOM_TABLE_WIDGET;

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

	private:
		void initConnection();
		void setStylize();



	private slots:
		void addNewFlow();
		void loadFlowTable();
		void tableCellClick(int row, int column);
		void tableRightMouseClick(QPoint pos);

	};


}

#endif