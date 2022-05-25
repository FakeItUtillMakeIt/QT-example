#include "CustomPlot.h"

using namespace CUSTOM_PLOT;

CustomPlot::CustomPlot(QWidget* parent):QWidget(parent) {
	QGridLayout* layout1 = new QGridLayout(parent);

	curvePlot = new QCustomPlot();
	curvePlot->setMaximumSize(200, 200);
	layout1->addWidget(curvePlot);

	//curvePlot->setAutoFillBackground(true);
	//curvePlot->resize(w->size());
	this->setLayout(layout1);

	this->setFixedSize(QSize(200, 200));

	attributeWidget = nullptr;
}


CustomPlot::~CustomPlot() {

}

/**
    @brief 
    @param numCurve - 
    @param x        - 
    @param y        - 
**/
void CUSTOM_PLOT::CustomPlot::setDataSource(int numCurve, QVector<double> x, QVector<double> y) {
	curvePlot->graph(numCurve)->setData(x, y);
	curvePlot->replot();
}

/**
    @brief 设置坐标轴区间范围
    @param xMin       - 
    @param xMax       - 
    @param yMin       - 
    @param yMax       - 
    @param xAxisLabel - 
    @param yAxisLabel - 
**/
void CUSTOM_PLOT::CustomPlot::setCurveXYAxis(double xMin, double xMax, double yMin, double yMax,QString xAxisLabel,QString yAxisLabel) {

	if (!curvePlot)
	{
		return;
	}
	curvePlot->xAxis->setRange(xMin, xMax);
	curvePlot->yAxis->setRange(yMin, yMax);
	curvePlot->xAxis->setLabel(xAxisLabel);
	curvePlot->yAxis->setLabel(yAxisLabel);
	curvePlot->legend->setVisible(true);
	curvePlot->legend->setBrush(QColor(255, 255, 255, 150));
}

/**
    @brief 添加曲线
    @param curveName      - 
    @param curveColor     - 
    @param curveLineWidth - 
**/
void CUSTOM_PLOT::CustomPlot::addOneCurve( QString curveName,Qt::GlobalColor curveColor,Qt::PenStyle curveStyle,double curveLineWidth) {

	curvePlot->addGraph();
	QPen pen = QPen(curveColor);
	pen.setWidthF(curveLineWidth);
	pen.setStyle(curveStyle);
	//画笔
	curvePlot->graph(curveNum)->setPen(pen);
	//图例
	curvePlot->graph(curveNum)->setName(curveName);

	curveNum++;
	curvePlot->replot();
}

/**
    @brief  
    @param  widget - 
    @retval        - 
**/
void CustomPlot::loadAttributeWidget(QWidget* widget) {
	{
		if (!widget)
		{
			widget = new QWidget();
		}

		QGridLayout* attributeLayout = new QGridLayout(attributeWidget);
		QLabel* label1 = new QLabel(QString::fromLocal8Bit("参数名称:"));
		QLineEdit* lineEdit1 = new QLineEdit();
		QLabel* label2 = new QLabel(QString::fromLocal8Bit("边框底色"));
		QComboBox* comboBox1 = new QComboBox();
		QStringList colorList = QColor::colorNames();
		for each (QString color in colorList)
		{
			QPixmap pix(QSize(70, 20));
			pix.fill(QColor(color));
			comboBox1->addItem(QIcon(pix), "");
			comboBox1->setIconSize(QSize(70, 20));
			comboBox1->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		}
		attributeLayout->addWidget(label1, 0, 0);
		attributeLayout->addWidget(lineEdit1, 0, 1);
		attributeLayout->addWidget(label2, 1, 0);
		attributeLayout->addWidget(comboBox1, 1, 1);

		widget->setLayout(attributeLayout);

		widget->show();
	}
}


/**
	@brief
	@param event -
**/
//void CustomPlot::mousePressEvent(QMouseEvent* event) {
//	//呼出属性栏
//
//	if (!attributeWidget)
//	{
//		 loadAttributeWidget(attributeWidget);
//
//	}
//
//}