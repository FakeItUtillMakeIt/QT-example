#include "CustomLabel.h"
#include "CustomParamComponent.h"
#include "DropWidget.h"

class DROP_WIDGET::DropWidget;

class CUSTOM_PARAM_COMPONENT::CustomParamComponent;

using namespace CUSTOM_LABEL;

CustomLabel::CustomLabel(QString displayText,QWidget* parent):QLabel(parent) {

	mBackGroundColor = Qt::darkGray;
	mBorderColor = Qt::darkCyan;

	mBorderWidth = 4;
	mRoundRadius = 4;

	fontColor = Qt::black;

	mLabelText = displayText;
	this->setMaximumSize(QSize(100, 20));
	
	selectSelf = false;



}

CustomLabel::~CustomLabel() {

	
}

/**
    @brief 
    @param fontC - 
**/
void CustomLabel::setFontColor(QColor fontC) {
	fontColor = fontC;
	update();
}

/**
    @brief 
    @param backGroundC - 
**/
void CustomLabel::setBackGroundColor(QColor backGroundC) {
	mBackGroundColor = backGroundC;
	update();
}

/**
    @brief 
    @param labelT - 
**/
void CustomLabel::setLabelText(QString labelT) {
	mLabelText = labelT;
	update();
}

QWidget* CustomLabel::loadAttributeWidget() {
	{
		
		//QWidget* widget = new QWidget();
		//attributeWidget1 = new QWidget();

		//attributeWidget1->setMaximumWidth(200);

		//QGridLayout* attributeLayout = new QGridLayout(attributeWidget1);
		//QLabel* label1 = new QLabel(QString::fromLocal8Bit("参数名称:"));
		//QLineEdit* lineEdit1 = new QLineEdit();
		//lineEdit1->setReadOnly(false);
		//lineEdit1->setEnabled(true);
		//QLabel* label2 = new QLabel(QString::fromLocal8Bit("边框底色"));
		//QComboBox* comboBox1 = new QComboBox();
		//QStringList colorList = QColor::colorNames();
		//for each (QString color in colorList)
		//{
		//	QPixmap pix(QSize(70, 20));
		//	pix.fill(QColor(color));
		//	comboBox1->addItem(QIcon(pix), "");
		//	comboBox1->setIconSize(QSize(70, 20));
		//	comboBox1->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		//}

		//QPushButton* button1 = new QPushButton();
		//button1->setText("button1");

		//attributeLayout->addWidget(label1, 0, 0);
		//attributeLayout->addWidget(lineEdit1, 0, 1);
		//attributeLayout->addWidget(label2, 1, 0);
		//attributeLayout->addWidget(comboBox1, 1, 1);
		//attributeLayout->addWidget(button1, 2, 1);

		//connect(button1, &QPushButton::clicked, this, [&] (){
		//	mLabelText = lineEdit1->text();
		//	this->update();
		//	});

		//attributeWidget1->setLayout(attributeLayout);

		////widget->show();
		//attributeWidget1->setObjectName("attr");
		//return attributeWidget1;

		attributeWidget1.setMaximumWidth(200);

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
			comboBox1->addItem(QIcon(pix), "",QColor(color));
			comboBox1->setIconSize(QSize(70, 20));
			comboBox1->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		}

		QPushButton* button1 = new QPushButton();
		button1->setText("button1");

		attributeLayout->addWidget(label1, 0, 0);
		attributeLayout->addWidget(lineEdit1, 0, 1);
		attributeLayout->addWidget(label2, 1, 0);
		attributeLayout->addWidget(comboBox1, 1, 1);
		attributeLayout->addWidget(button1, 2, 1);

		connect(button1, &QPushButton::clicked, this, [=]() {
			mLabelText = lineEdit1->text();
			mBackGroundColor = comboBox1->itemData(comboBox1->currentIndex()).value<QColor> ();
			qDebug() << comboBox1->itemData(comboBox1->currentIndex());
			qDebug() << mBackGroundColor;
			this->update();
			});

		if (!attributeWidget1.layout())
		{
			attributeWidget1.setLayout(attributeLayout);
		}

		return &attributeWidget1;

	}
}


/**
    @brief 重写paintEvent改变外观
    @param  - 
**/
void CustomLabel::paintEvent(QPaintEvent*) {

	//信号槽
	connect(this, &CUSTOM_LABEL::CustomLabel::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);

	QPainter painter(this);
	QPen pen;
	QBrush brush;

	painter.setRenderHint(QPainter::Antialiasing);

	pen.setColor(mBorderColor);
	pen.setStyle(Qt::SolidLine);

	painter.setPen(pen);
	QRect borderRect = QRect(this->rect().x(), this->rect().y(), this->rect().width() + mBorderWidth, this->rect().height() + mBorderWidth);
	painter.drawRoundedRect(borderRect, mRoundRadius, mRoundRadius);

	brush.setColor(mBackGroundColor);
	brush.setStyle(Qt::SolidPattern);
	painter.setBrush(brush);

	painter.drawRoundedRect(this->rect(), mRoundRadius, mRoundRadius);

	QPen newPen = QPen(fontColor);
	newPen.setStyle(Qt::SolidLine);
	painter.setPen(newPen);
	QFont font = painter.font();
	font.setPixelSize(20);
	font.setBold(true);
	painter.setFont(font);

	painter.drawText(this->rect(), mLabelText, Qt::AlignHCenter | Qt::AlignVCenter);
}


/**
    @brief 
    @param event - 
**/
void CustomLabel::mousePressEvent(QMouseEvent* event) {
	
	this->raise();

	selection->addWidget(this);
	selection->show(this);
	

	if (!&attributeWidget1)
	{
		//attributeWidget1=loadAttributeWidget();
		loadAttributeWidget();
	}
	loadAttributeWidget();
	if (event->button() == Qt::LeftButton)
	{
		selectSelf = true;

		m_point = event->globalPos();

		m_pos = this->frameGeometry().topLeft();

	}
	mousePressed = true;
	QWidget::mousePressEvent(event);

	//呼出属性栏信号
	emit displayAttribute(attributeWidget1);

	this->grabKeyboard();
}

void CustomLabel::mouseMoveEvent(QMouseEvent* event) {
	if ((event->buttons() == Qt::LeftButton) && mousePressed)
	{
		QPoint relativePos = event->globalPos() - m_point;

		this->move(m_pos + relativePos);
	}

	selection->clear();
	QWidget::mouseMoveEvent(event);

	//mousePressed = false;
	this->releaseKeyboard();

}

/**
    @brief 
    @param event - 
**/
void CustomLabel::leaveEvent(QEvent* event) {
	this->releaseKeyboard();
	selection->hide(this);
	selection->removeWidget(this);
}

/**
    @brief 响应键盘事件
    @param event - 
**/
void CustomLabel::keyPressEvent(QKeyEvent* event) {

	//先获取本身对象
	if (!selectSelf)
	{
		return;
	}

	//响应Delete
	if (event->key()==Qt::Key_Delete)
	{
		qDebug() << "key event";

		selection->~Selection();
		this->~CustomLabel();
		
	}
	
	//QWidget::keyPressEvent(event);
	this->releaseKeyboard();
	selectSelf = false;
}

void CustomLabel::updateAttributeDisplay() {
	qDebug() << "--------";



}