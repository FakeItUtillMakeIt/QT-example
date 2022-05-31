#pragma once

#ifndef CUSTOM_GROUP_BOX_H
#define CUSTOM_GROUP_BOX_H

#include <QWidget>
#include <QtGui>
#include <qgroupbox.h>
#include "WidgetHandle.h"
#include "CustomLabel.h"
#include "CustomPushButton.h"
#include "CustomCurvePlot.h"

class CUSTOM_LABEL::CustomLabel;

class CUSTOM_PUSHBUTTON::CustomPushButton;
class CUSTOM_CURVE_PLOT::CustomCurvePlot;


namespace CUSTOM_GROUP_BOX{

	enum MY_PARENT_TYPE
	{
		CUSTOM_LISTWIDGET,
		CUSTOM_GROUPBOX
	};

	class CustomGroupBox :
		public QGroupBox
	{
		Q_OBJECT

	public:
		CustomGroupBox(QWidget* parent = nullptr);
		~CustomGroupBox();
		QWidget attributeWidget;
		QWidget* loadAttributeWidget();

	private:

		MY_PARENT_TYPE myParentType;
		QGridLayout* boxLayout;
		bool selectSelf;
		bool mousePressed;

		int hasWidgetNum = 0;

		QString myTitle;

		QPoint m_point;
		QPoint m_pos;
		WIDGET_HANDLE::Selection* selection = new WIDGET_HANDLE::Selection(this);

	protected:
		//void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

		void dragEnterEvent(QDragEnterEvent* event) Q_DECL_OVERRIDE;
		void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;
		void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;

		void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
		void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

		void leaveEvent(QEvent* event) Q_DECL_OVERRIDE;

		void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

	signals:
		void displayAttribute(QWidget&);
	};

}



#endif