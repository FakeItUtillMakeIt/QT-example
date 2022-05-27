#pragma once

#ifndef DROP_WIDGET_H
#define DROP_WIDGET_H


#include <qframe.h>
#include <QEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QPainter>
#include <QBoxLayout>

#include <QListWidgetItem>
#include "CustomLabel.h"
#include "CustomPlot.h"
#include "CustomCurvePlot.h"
#include "CustomPushButton.h"

#include "WidgetHandle.h"

class CUSTOM_LABEL::CustomLabel;
class CUSTOM_PLOT::CustomPlot;
class CUSTOM_PUSHBUTTON::CustomPushButton;
class CUSTOM_CURVE_PLOT::CustomCurvePlot;


/**
    @namespace DROP_WIDGET
    @brief     自定义可拖放区域类
**/
namespace DROP_WIDGET {

	enum DROP_TYPE
	{
		TYPE_LABEL = 0,
		TYPE_BUTTON = 1,
		TYPE_PLOT = 2
	};

	class DropWidget :
		public QFrame
	{
	public:
		DropWidget(QWidget* parent = nullptr);
		~DropWidget();

	private:
		QBoxLayout* boxLayout=new QBoxLayout(QBoxLayout::Direction::TopToBottom,this);

	protected:
		void dragEnterEvent(QDragEnterEvent* event) Q_DECL_OVERRIDE;
		void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;
		void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;
		void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

	};
}




#endif