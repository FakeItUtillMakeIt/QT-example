#pragma once

#ifndef CUSTOM_PLOT_H
#define CUSTOM_PLOT_H

#include <QtWidgets>
#include <QVector>
#include <QString>
#include <qcustomplot.h>

namespace CUSTOM_PLOT {

	class CustomPlot :
		public QWidget
	{
	public:

		CustomPlot( QWidget* parent = nullptr);
		~CustomPlot();

	public:
		//
		void addOneCurve(QString curveName, Qt::GlobalColor curveColor, Qt::PenStyle curveStyle=Qt::PenStyle::SolidLine, double curveLineWidth=1);
		void setCurveXYAxis(double xMin, double xMax, double yMin, double yMax, QString xAxisLabel="", QString yAxisLabel="");
		void setDataSource(int numCurve, QVector<double> x=xDefault, QVector<double> y=yDefault);

		void loadAttributeWidget(QWidget* widget);

	private:
		int curveNum = 0;
		static QVector<qreal> xDefault;
		static QVector<qreal> yDefault;

		QCustomPlot* curvePlot;
		QVector<QString> curveNames;
		QVector<QVector<double>> xSets;
		QVector<QVector<double>> ySets;

		QWidget* attributeWidget;

	protected:
		//void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

	};

}



#endif