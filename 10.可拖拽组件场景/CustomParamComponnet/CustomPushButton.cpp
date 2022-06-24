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
	 this->setObjectName("button");
	 this->grabKeyboard();
	 selectSelf = false;

	 if (parent->objectName() == "customGroupBox")
	 {
		 myParentType = MY_PARENT_TYPE::CUSTOM_GROUPBOX;

}
	 else
	 {
		 myParentType = MY_PARENT_TYPE::CUSTOM_LISTWIDGET;
	 }

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

int CustomPushButton::getBindParamIndex() {
	return mBindingParamIndex;
}

QWidget* CustomPushButton::loadAttributeWidget() {
	{
		attributeWidget.setMaximumWidth(200);
		

		QGridLayout* attributeLayout = new QGridLayout();
		QLabel* label1 = new QLabel(QString::fromLocal8Bit("�ؼ�����:"));
		QLineEdit* lineEdit1 = new QLineEdit();
		lineEdit1->setReadOnly(false);
		lineEdit1->setEnabled(true);
		QLabel* label2 = new QLabel(QString::fromLocal8Bit("�ؼ���ɫ:"));
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

		QLabel* label3 = new QLabel(QString::fromLocal8Bit("������:"));
		QComboBox* comboBox2 = new QComboBox();
		
		
		/*��������ݿ��Ӧ���ȡ������*/
		SingleDataController* controller = SingleDataController::getInstance();
		map<int, vector<string>> paramInfos = controller->getDataBaseParamInfo();

		QVector<QString> paramList1 = { "None bind param" };
		for (auto it = paramInfos.begin(); it != paramInfos.end(); it++)
		{
			paramList1.push_back(QString::fromStdString(it->second[0]));
		}
		
		for each (QString param in paramList1)
		{
			
			comboBox2->addItem(param);
			comboBox2->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		}

		QPushButton* button1 = new QPushButton();

		button1->setText(QString::fromLocal8Bit("Ӧ�ÿؼ�����"));

		attributeLayout->addWidget(label1, 0, 0);
		attributeLayout->addWidget(lineEdit1, 0, 1);
		attributeLayout->addWidget(label2, 1, 0);
		attributeLayout->addWidget(comboBox1, 1, 1);
		attributeLayout->addWidget(label3, 2, 0);
		attributeLayout->addWidget(comboBox2, 2, 1);
		attributeLayout->addWidget(button1, 3, 1);


		connect(button1, &QPushButton::clicked, this, [=]() {
			
			mDisableBackGroundColor = comboBox1->itemData(comboBox1->currentIndex()).value<QColor>();
			mBindingParam = comboBox2->currentText();
			mBindingParamIndex = comboBox2->currentIndex();
			mButtonText = lineEdit1->text();
			qDebug() << comboBox1->itemData(comboBox1->currentIndex())<<mBindingParam;
			qDebug() << mDisableBackGroundColor;
			this->update();
			});

		if (!attributeWidget.layout())
		{
			attributeWidget.setLayout(attributeLayout);
		}


		//return &attributeWidget;
		return nullptr;
	}
}


/**
    @brief ��дpaintEvent�ı����
    @param  - 
**/
void CustomPushButton::paintEvent(QPaintEvent*) {


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
	

	//���ģʽ
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
    @brief ��д�뿪�¼�
    @param  - 
**/
void CustomPushButton::leaveEvent(QEvent*) {
	buttonState = BUTTON_STATE::normal;
	this->releaseKeyboard();
	selection->hide(this);
	selection->removeWidget(this);
	update();
}

/**
    @brief ��д�����¼�
    @param e - 
**/
void CustomPushButton::mousePressEvent(QMouseEvent* event) {
	this->raise();

	//�źŲ�
#ifdef ONLY_CONTROL_COMPARAM
	connect(this, &CustomPushButton::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);
#endif //ONLY_CONTROL_COMPARAM
	if (myParentType == MY_PARENT_TYPE::CUSTOM_LISTWIDGET)
	{
		//�źŲ�
		if (!this->parent()->parent())
		{
			return;
		}
		connect(this, &CustomPushButton::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);
	}
	else
	{
		//�źŲ�
		if (!this->parent()->parent()->parent())
		{
			return;
		}
		connect(this, &CustomPushButton::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);
	}

	/*����GroupBox*/
	if (myParentType == MY_PARENT_TYPE::CUSTOM_LISTWIDGET)
	{
		selection->addWidget(this);
		selection->show(this);
	}
	

	/*if (!attributeWidget)
	{
		attributeWidget=loadAttributeWidget();
	}*/

	if (attributeWidget.isActiveWindow()==false)
	{
		loadAttributeWidget();
	}
	

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
	selection->clear();

	

	if (this->parent()->objectName() == "customGroupBox")
	{
		return;
	}
	if ((event->buttons() == Qt::LeftButton) && mousePressed)
	{
		QPoint relativePos = event->globalPos() - m_point;

		this->move(m_pos + relativePos);
	}
	QWidget::mouseMoveEvent(event);
	
}

/**
	@brief ��Ӧ�����¼�
	@param event -
**/
void CustomPushButton::keyPressEvent(QKeyEvent* event) {

	//�Ȼ�ȡ�������
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