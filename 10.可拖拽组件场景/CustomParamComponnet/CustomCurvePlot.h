#pragma once
#ifndef CUSTOM_CURVE_PLOT_H
#define CUSTOM_CURVE_PLOT_H

#include <random>
#include "qcustomplot.h"
#include "WidgetHandle.h"

/**
    @namespace CUSTOM_CURVE_PLOT
    @brief     �Զ��廭ͼ�࣬�̳���qcustomplot��
**/
namespace CUSTOM_CURVE_PLOT {

	enum MY_PARENT_TYPE
	{
		CUSTOM_LISTWIDGET,
		CUSTOM_GROUPBOX
	};

	class CustomCurvePlot :
		public QCustomPlot
	{
		Q_OBJECT
	public:
		CustomCurvePlot(QWidget* parent=nullptr);
		~CustomCurvePlot();

	public:
		QWidget attributeWidget;
		QWidget* loadAttributeWidget();

		QVector<int> getBindParamList();

	private:
		QVector<int> mBindingParamIndexList;
		MY_PARENT_TYPE myParentType;
		//�洢��󶨲�����QListWidget�д��������Ĺ�ϵ��
		//��ѡ��һ������ʱ��������󶨴��ڵ�������˳�����õ�������
		QMap<int, QString> mBindingParam;

		int hasAddButNotNameParamNum = 0;
		int currentSelectParam = -1;

		WIDGET_HANDLE::Selection* selection = new WIDGET_HANDLE::Selection(this);
		bool selectSelf;

		QPoint m_pos;
		QPoint m_point;

		bool mousePressed;

		QVector< QCPGraph*> paramCurves;
		QVector<QTimer*> paramTimers;

		qreal testPlotData;
		qreal xdata;
		QCPGraph* graph1;
		QVector<qreal> xset, yset;

		void generateShuffleData();
		

	protected:
		void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
		void mouseMoveEvent(QMouseEvent* ev) Q_DECL_OVERRIDE;
		void leaveEvent(QEvent* event) Q_DECL_OVERRIDE;

		void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

	signals:
		void displayAttribute(QWidget&);
	};

}



#endif