#include "CustomCurvePlot.h"
#include "CustomParamComponent.h"

class CUSTOM_PARAM_COMPONENT::CustomParamComponent;

using namespace CUSTOM_CURVE_PLOT;

CustomCurvePlot::CustomCurvePlot(QWidget* parent) :QCustomPlot(parent) {

	//attributeWidget = nullptr;
	selectSelf = false;
	
}

CustomCurvePlot::~CustomCurvePlot() {
	
}

QWidget* CustomCurvePlot::loadAttributeWidget() {
	{
		
		attributeWidget.setMaximumWidth(200);

		QGridLayout* attributeLayout = new QGridLayout();
		QLabel* label1 = new QLabel(QString::fromLocal8Bit("参数名称:"));
		QLineEdit* lineEdit1 = new QLineEdit();
		lineEdit1->setReadOnly(false);
		lineEdit1->setEnabled(true);
		QLabel* label2 = new QLabel(QString::fromLocal8Bit("边框底色"));
		QComboBox* comboBox1 = new QComboBox();
		QStringList colorList = QColor::colorNames();
		for each (QString color in colorList)
		{
			QPixmap pix(QSize(70, 20));
			pix.fill(QColor(color));
			comboBox1->addItem(QIcon(pix), "", QColor(color));
			comboBox1->setIconSize(QSize(70, 20));
			comboBox1->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		}

		QPushButton* button1 = new QPushButton();

		button1->setText("button1");
		QGroupBox* groupBox = new QGroupBox();
		groupBox->setTitle(QString::fromLocal8Bit("参数绑定"));
		QLabel* labelGroup1 = new QLabel("param1:");
		QComboBox* comboGroup1 = new QComboBox();
		QHBoxLayout* boxLayout = new QHBoxLayout();
		boxLayout->addWidget(labelGroup1);
		boxLayout->addWidget(comboGroup1);
		groupBox->setLayout(boxLayout);

		attributeLayout->addWidget(label1, 0, 0);
		attributeLayout->addWidget(lineEdit1, 0, 1);
		attributeLayout->addWidget(label2, 1, 0);
		attributeLayout->addWidget(comboBox1, 1, 1);
		attributeLayout->addWidget(groupBox,2,0,1,2);
		attributeLayout->addWidget(button1, 3, 1);

		//处理属性编辑窗口配置
		connect(button1, &QPushButton::clicked, this, [=]() {
			
			qDebug() << comboBox1->itemData(comboBox1->currentIndex());

			this->update();
			});

		if (!attributeWidget.layout())
		{
			attributeWidget.setLayout(attributeLayout);
		}

		return &attributeWidget;
	}
}

void CustomCurvePlot::mousePressEvent(QMouseEvent* event) {
	this->raise();
	//信号槽
	connect(this, &CustomCurvePlot::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);

	selection->addWidget(this);
	selection->show(this);

	/*if (!attributeWidget)
	{
		attributeWidget=loadAttributeWidget();
	}*/

	loadAttributeWidget();
	if (event->button() == Qt::LeftButton)
	{
		selectSelf = true;

		m_point = event->globalPos();

		m_pos = this->frameGeometry().topLeft();

	}
	mousePressed = true;
	QWidget::mousePressEvent(event);

	emit displayAttribute(attributeWidget);

	this->grabKeyboard();
}

void CustomCurvePlot::leaveEvent(QEvent* event) {
	this->releaseKeyboard();

	selection->hide(this);
	selection->removeWidget(this);
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