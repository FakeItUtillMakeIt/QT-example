#pragma once

#ifndef SUB_TABLE_WIDGET_H
#define SUB_TABLE_WIDGET_H


#include <QtWidgets>
#include <qtablewidget.h>

namespace SUB_TABLE_WIDGET {

	class SubTableWidget :
		public QTableWidget
	{

		Q_OBJECT
	public:
		SubTableWidget();

	private:
		QPoint cursorPos;
		QWidget* popWidget;

		QPushButton* frontInsertCell;
		QPushButton* backInsertCell;
		QPushButton* removeCell;

		int row, column;

	private:
		void mousePressEvent(QMouseEvent* event);
		void mouseDoubleClickEvent(QMouseEvent* event);

	private slots:
		void rightButtonClicked(int row, int column);
		void doubleClickTable(int row, int column);

	signals:
		void mouseRightClicked(QPoint);
		void opOver(QPoint);

	};

}


#endif