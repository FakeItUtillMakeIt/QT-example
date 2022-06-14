#pragma once
#ifndef CUSTOM_LABEL_H
#define CUSTOM_LABEL_H

#include <qlabel.h>
#include <QObject>
#include <QtWidgets>
#include <QKeyEvent>
#include "WidgetHandle.h"


#include "SingleDataController.h"

using namespace std;
/**
    @namespace CUSTOM_LABEL
    @brief     自定义标签类
**/
namespace CUSTOM_LABEL {

	enum MY_PARENT_TYPE
	{
		CUSTOM_LISTWIDGET,
		CUSTOM_GROUPBOX
	};

	class CustomLabel :
		public QLabel
	{
		Q_OBJECT

	public:
		CustomLabel(QString displayText="",QWidget* parent=nullptr);
		~CustomLabel();

	private:
		int mBindingParamIndex;

		MY_PARENT_TYPE myParentType;

		int myType = 0;
		QPoint dragStartPoint;

		QPixmap pixmap;
		QColor mBackGroundColor;
		QColor mBorderColor;

		qreal mBorderWidth;
		qreal mRoundRadius;

		QColor fontColor;

		QString mLabelText;

		QWidget* attributeWidget;

		bool selectSelf;
		QPoint m_pos;
		QPoint m_point;
		
		bool mousePressed;
		WIDGET_HANDLE::Selection* selection = new WIDGET_HANDLE::Selection(this);

	public:
		QWidget attributeWidget1;

	public:
		void setFontColor(QColor fontC);
		void setBackGroundColor(QColor backGroundC);
		void setLabelText(QString labelT);

		QWidget* loadAttributeWidget();

	private:
		void getDataBaseParamList(QString tableName, QVector<QString>& paramList);


	protected:
		void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
		void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
		void mouseMoveEvent(QMouseEvent* ev) Q_DECL_OVERRIDE;
		void leaveEvent(QEvent* event) Q_DECL_OVERRIDE;

		void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

	

	signals:
		void displayAttribute(QWidget&);

	public slots:
		void updateAttributeDisplay();

	};
}



#endif