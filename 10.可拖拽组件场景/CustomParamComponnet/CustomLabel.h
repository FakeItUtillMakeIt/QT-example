#pragma once
#ifndef CUSTOM_LABEL_H
#define CUSTOM_LABEL_H

#include <qlabel.h>
#include <QObject>
#include <QtWidgets>
#include <QKeyEvent>
#include "WidgetHandle.h"


/**
    @namespace CUSTOM_LABEL
    @brief     自定义标签类
**/
namespace CUSTOM_LABEL {

	class CustomLabel :
		public QLabel
	{
		Q_OBJECT

	public:
		CustomLabel(QString displayText="",QWidget* parent=nullptr);
		~CustomLabel();

	private:
		QPixmap pixmap;
		QColor mBackGroundColor;
		QColor mBorderColor;

		qreal mBorderWidth;
		qreal mRoundRadius;

		QColor fontColor;

		QString mLabelText;

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