#pragma once

#ifndef CUSTOM_TABLE_WIDGET_H
#define CUSTOM_TABLE_WIDGET_H

#include <QtWidgets>
#include <qtablewidget.h>

namespace CUSTOM_TABLE_WIDGET {
	class CustomTableWidget :
		public QTableWidget
	{
		Q_OBJECT

	public:
		CustomTableWidget();
		~CustomTableWidget();

	protected:
		void mousePressEvent(QMouseEvent* event);

	signals:
		void mouseRightClicked(QPoint);
	

	};
}


#endif