#include "CustomCurvePlot.h"

using namespace CUSTOM_CURVE_PLOT;

CustomCurvePlot::CustomCurvePlot(QWidget* parent) :QCustomPlot(parent) {

	attributeWidget = nullptr;
	selectSelf = false;
}

CustomCurvePlot::~CustomCurvePlot() {
	
}

void CustomCurvePlot::loadAttributeWidget(QWidget* widget) {
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

void CustomCurvePlot::mousePressEvent(QMouseEvent* event) {
	//呼出属性栏
	selection->addWidget(this);

	if (!attributeWidget)
	{
		//loadAttributeWidget(attributeWidget);
	}
	if (event->button() == Qt::LeftButton)
	{
		selectSelf = true;

		m_point = event->globalPos();

		m_pos = this->frameGeometry().topLeft();

	}
	mousePressed = true;
	QWidget::mousePressEvent(event);

}


void CustomCurvePlot::mouseMoveEvent(QMouseEvent* event) {
	if ((event->buttons() == Qt::LeftButton) && mousePressed)
	{
		QPoint relativePos = event->globalPos() - m_point;

		this->move(m_pos + relativePos);
		selection->clear();
	}

	
	QWidget::mouseMoveEvent(event);
}

/**
	@brief 响应键盘事件
	@param event -
**/
void CustomCurvePlot::keyPressEvent(QKeyEvent* event) {

	//先获取本身对象
	if (!selectSelf)
	{
		return;
	}
	if (event->key() == Qt::Key_Delete)
	{
		qDebug() << "key event";
		selection->~Selection();
		this->~CustomCurvePlot();
	}

}