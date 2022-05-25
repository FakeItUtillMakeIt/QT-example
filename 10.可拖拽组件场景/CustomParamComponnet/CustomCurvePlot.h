#pragma once
#ifndef CUSTOM_CURVE_PLOT_H
#define CUSTOM_CURVE_PLOT_H


#include "qcustomplot.h"
#include "WidgetHandle.h"

/**
    @namespace CUSTOM_CURVE_PLOT
    @brief     自定义信号图类
**/
namespace CUSTOM_CURVE_PLOT {

	class CustomCurvePlot :
		public QCustomPlot
	{

	public:
		CustomCurvePlot(QWidget* parent=nullptr);
		~CustomCurvePlot();

		QWidget* attributeWidget;
		void loadAttributeWidget(QWidget* widget);

	private:

		WIDGET_HANDLE::Selection* selection = new WIDGET_HANDLE::Selection(this);
		bool selectSelf;

		QPoint m_pos;
		QPoint m_point;

		bool mousePressed;

	protected:
		void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
		void mouseMoveEvent(QMouseEvent* ev) Q_DECL_OVERRIDE;
		void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
	};
}



#endif