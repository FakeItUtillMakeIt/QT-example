#include "CustomPushButton.h"
#include "CustomParamComponent.h"

class CUSTOM_PARAM_COMPONENT::CustomParamComponent;

using namespace CUSTOM_PUSHBUTTON;

CustomPushButton::CustomPushButton(QString displayText,QWidget* parent):QPushButton(parent) {
	buttonState = CUSTOM_PUSHBUTTON::BUTTON_STATE::normal;

	 mBorderWidth=10;

	 mDisableBackGroundColor=Qt::gray;
	 mDisableBoderColor=Qt::gray;
	 mNormalBackGroundColor=Qt::darkGreen;
	 mNormalBorderColor=Qt::green;

	 mBorderRidius=4;

	 mIconPixmap=QPixmap();
	 mIconMargin=2;
	 mIconSize=QSize();
	 mTextPixmapMargin=2;

	 mTextPixmapRect=QRect();

	 mTextColor=Qt::black;
	 mTextFont= "Helvetica [Cronyx]";
	 mTextSize=10;
	 mTextWidth=1;
	 fontColor = Qt::black;
	 mButtonText=displayText;

	 this->setBaseSize(QSize(100, 20));
	 //attributeWidget = nullptr;
	 
	 this->grabKeyboard();
	 selectSelf = false;
}

CustomPushButton::~CustomPushButton() {

}


/**
    @brief 
    @param fontC - 
**/
void CustomPushButton::setFontColor(QColor fontC) {
	fontColor = fontC;
	update();
}

/**
    @brief 
    @param backGroundC - 
**/
void CustomPushButton::setBackGroundColor(QColor backGroundC) {
	mBackGroundColor = backGroundC;
	update();
}

/**
    @brief 
    @param buttonT - 
**/
void CustomPushButton::setLabelText(QString buttonT) {
	mButtonText = buttonT;
	update();
}


QWidget* CustomPushButton::loadAttributeWidget() {
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
		QPushButton* button2 = new QPushButton();
		button1->setText("button1");

		attributeLayout->addWidget(label1, 0, 0);
		attributeLayout->addWidget(lineEdit1, 0, 1);
		attributeLayout->addWidget(label2, 1, 0);
		attributeLayout->addWidget(comboBox1, 1, 1);
		attributeLayout->addWidget(button1, 2, 1);

		attributeLayout->addWidget(button2, 3, 1);

		connect(button1, &QPushButton::clicked, this, [=]() {
			
			mBackGroundColor = comboBox1->itemData(comboBox1->currentIndex()).value<QColor>();
			qDebug() << comboBox1->itemData(comboBox1->currentIndex());
			qDebug() << mBackGroundColor;
			this->update();
			});

		attributeWidget.setLayout(attributeLayout);

		//widget->show();
		//attributeWidget.setObjectName("attr");
		return &attributeWidget;
	}
}


/**
    @brief 重写paintEvent改变外观
    @param  - 
**/
void CustomPushButton::paintEvent(QPaintEvent*) {

	//信号槽
	connect(this, &CustomPushButton::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);


	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	
	QPainterPath path;
	QBrush brush;
	QPen pen;

	switch (buttonState)
	{
	case BUTTON_STATE::normal:
		brush.setColor(mDisableBackGroundColor);
		pen.setColor(mDisableBoderColor);
		break;
	case BUTTON_STATE::pressed:
		brush.setColor(mNormalBackGroundColor);
		pen.setColor(mNormalBorderColor);
		break;
	default:
		break;
	}
	

	//填充模式
	pen.setStyle(Qt::SolidLine);
	pen.setColor(Qt::black);
	painter.setPen(pen);
	
	//painter.setBrush(QBrush(Qt::black));
	QRect borderRect = QRect(this->rect().x(), this->rect().y(),this->rect().width()+mBorderWidth, this->rect().height() + mBorderWidth);
	painter.drawRoundedRect(borderRect, mBorderRidius, mBorderRidius);

	brush.setStyle(Qt::SolidPattern);
	
	painter.setBrush(brush);
	painter.drawRoundedRect(this->rect(), mBorderRidius, mBorderRidius);
	
	//painter.fillRect(,brush);
	painter.setPen(QPen(fontColor));
	QFont font = painter.font();
	font.setPixelSize(20);
	font.setBold(true);
	
	painter.setFont(font);
	painter.drawText(this->rect(), mButtonText, Qt::AlignHCenter | Qt::AlignVCenter);
	
}

/**
    @brief 重写离开事件
    @param  - 
**/
void CustomPushButton::leaveEvent(QEvent*) {
	buttonState = BUTTON_STATE::normal;
	this->releaseKeyboard();
	update();
}

/**
    @brief 重写按下事件
    @param e - 
**/
void CustomPushButton::mousePressEvent(QMouseEvent* event) {
	this->raise();
	selection->addWidget(this);
	

	/*if (!attributeWidget)
	{
		attributeWidget=loadAttributeWidget();
	}*/
	loadAttributeWidget();

	if (event->button() == Qt::LeftButton)
	{
		buttonState = BUTTON_STATE::pressed;
		selectSelf = true;
		selectSelf = true;

		m_point = event->globalPos();

		m_pos = this->frameGeometry().topLeft();

	}
	mousePressed = true;
	QWidget::mousePressEvent(event);

	emit displayAttribute(attributeWidget);

	this->grabKeyboard();
	
}


void CustomPushButton::mouseMoveEvent(QMouseEvent* event) {
	if ((event->buttons() == Qt::LeftButton) && mousePressed)
	{
		QPoint relativePos = event->globalPos() - m_point;

		this->move(m_pos + relativePos);
	}

	selection->clear();

	QWidget::mouseMoveEvent(event);
}

/**
	@brief 响应键盘事件
	@param event -
**/
void CustomPushButton::keyPressEvent(QKeyEvent* event) {

	//先获取本身对象
	if (!selectSelf)
	{
		return;
	}
	if (event->key() == Qt::Key_Delete)
	{
		qDebug() << "key event";
		selection->~Selection();
		this->~CustomPushButton();
	}

}