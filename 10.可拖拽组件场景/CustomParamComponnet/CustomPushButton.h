#pragma once

#ifndef CUSTOM_PUSHBUTTON_H
#define CUSTOM_PUSHBUTTON_H


#include <QtWidgets>
#include <QString>
#include <qpushbutton.h>
#include "WidgetHandle.h"


/**
    @namespace CUSTOM_PUSHBUTTON
    @brief     自定义按钮类
**/
namespace CUSTOM_PUSHBUTTON {

	enum BUTTON_STATE
	{
		normal=0,
		hover=1,
		pressed=2
	};

	enum MY_PARENT_TYPE
	{
		CUSTOM_LISTWIDGET,
		CUSTOM_GROUPBOX
	};

	class CustomPushButton :
		public QPushButton
	{

		Q_OBJECT

	public:
		CustomPushButton(QString displayText="", QWidget* parent = nullptr);
		~CustomPushButton();

	protected:
		void paintEvent(QPaintEvent*);

		
	private:
		MY_PARENT_TYPE myParentType;
		QString mBindingParam;

		int buttonState;

		qreal mBorderWidth;
		QColor mBackGroundColor;
		QColor mBorderColor;
		QColor mDisableBackGroundColor;
		QColor mDisableBoderColor;
		QColor mNormalBackGroundColor;
		QColor mNormalBorderColor;

		qreal mBorderRidius;

		QPixmap mIconPixmap;
		qreal mIconMargin;
		QSize mIconSize;
		qreal mTextPixmapMargin;

		QRectF mTextPixmapRect;

		QColor mTextColor;
		QString mTextFont;
		qreal mTextSize;
		qreal mTextWidth;

		QColor fontColor;

		QString mButtonText;

		

		bool selectSelf;
		QPoint m_pos;
		QPoint m_point;

		bool mousePressed;
		WIDGET_HANDLE::Selection* selection = new WIDGET_HANDLE::Selection(this);


	public:
		void setFontColor(QColor fontC);
		void setBackGroundColor(QColor backGroundC);
		void setLabelText(QString buttonT);

		QWidget attributeWidget;
		QWidget* loadAttributeWidget();

	private:
		void paintBackGround(QPainter* painter);
		void paintIcon(QPainter* painter);
		void paintTextBackGround(QPainter* painter);
		void paintText(QPainter* painter);

	protected:
		void leaveEvent(QEvent*) Q_DECL_OVERRIDE;
		void mousePressEvent(QMouseEvent*) Q_DECL_OVERRIDE;
		void mouseMoveEvent(QMouseEvent* ev) Q_DECL_OVERRIDE;


		void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

	signals:
		void displayAttribute(QWidget&);
	};
}




#endif