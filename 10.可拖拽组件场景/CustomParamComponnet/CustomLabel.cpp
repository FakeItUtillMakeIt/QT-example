#include "CustomLabel.h"
#include "CustomParamComponent.h"
#include "DropWidget.h"

class DROP_WIDGET::DropWidget;

class CUSTOM_PARAM_COMPONENT::CustomParamComponent;

using namespace CUSTOM_LABEL;


/**
    @brief  ���캯��
    @param  displayText - 
    @param  parent      - 
    @retval             - 
**/
CustomLabel::CustomLabel(QString displayText,QWidget* parent):QLabel(parent) {

	mBackGroundColor = Qt::darkGray;
	mBorderColor = Qt::darkCyan;

	mBorderWidth = 4;
	mRoundRadius = 4;

	fontColor = Qt::black;

	mLabelText = displayText;
	this->setMaximumSize(QSize(100, 20));
	attributeWidget = nullptr;

	this->setFocusPolicy(Qt::ClickFocus);
	this->grabKeyboard();
	selectSelf = false;
	//Ĭ�ϲ��󶨲���
	mBindingParamIndex = 0;

	this->setObjectName("label");
	if (parent->objectName()=="customGroupBox")
	{
		myParentType = MY_PARENT_TYPE::CUSTOM_GROUPBOX;
		
}
	else
	{
		myParentType = MY_PARENT_TYPE::CUSTOM_LISTWIDGET;
	}

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

int CustomLabel::getBindParamIndex() {
	return mBindingParamIndex;
}

/**
    @brief  �������Դ���
    @retval  - 
**/
QWidget* CustomLabel::loadAttributeWidget() {
	{

		attributeWidget1.setMaximumWidth(200);

		QGridLayout* attributeLayout = new QGridLayout();
		QLabel* label1 = new QLabel(QString::fromLocal8Bit("�ؼ�����:"));
		QLineEdit* lineEdit1 = new QLineEdit();
		lineEdit1->setReadOnly(false);
		lineEdit1->setEnabled(true);
		QLabel* label2 = new QLabel(QString::fromLocal8Bit("�ؼ���ɫ"));
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

		QLabel* label3 = new QLabel(QString::fromLocal8Bit("������:"));
		QComboBox* comboBox2 = new QComboBox();
		/*��������ݿ��Ӧ���ȡ������*/
		SingleDataController* controller = SingleDataController::getInstance();
		map<int, vector<string>> paramInfos=controller->getDataBaseParamInfo();
		
		QVector<QString> paramList = { "None bind param" };
		for (auto it=paramInfos.begin();it!=paramInfos.end();it++)
		{
			paramList.push_back(QString::fromStdString(it->second[0]));
		}
		

		for each(QString paramName in paramList)
		{
			comboBox2->addItem(paramName);
		}

		QPushButton* button1 = new QPushButton();
		button1->setText(QString::fromLocal8Bit("Ӧ�ÿؼ�����"));

		attributeLayout->addWidget(label1, 0, 0);
		attributeLayout->addWidget(lineEdit1, 0, 1);
		attributeLayout->addWidget(label2, 1, 0);
		attributeLayout->addWidget(comboBox1, 1, 1);
		//�����Ƿ����ð󶨲���ȷ�����label�Ǵ�label������Ҫ��ˢ�²�����label
		attributeLayout->addWidget(label3, 2, 0);
		attributeLayout->addWidget(comboBox2, 2, 1);

		attributeLayout->addWidget(button1, 3, 1);

		/**/
		connect(button1, &QPushButton::clicked, this, [=]() {
			mBindingParamIndex = comboBox2->currentIndex();
			mLabelText = lineEdit1->text();
			mBackGroundColor = comboBox1->itemData(comboBox1->currentIndex()).value<QColor> ();

			qDebug() << comboBox1->itemData(comboBox1->currentIndex());
			qDebug() << mBackGroundColor<<mBindingParamIndex;
			this->update();
			});
		/**/

		if (!attributeWidget1.layout())
		{
			attributeWidget1.setLayout(attributeLayout);
		}

		return &attributeWidget1;

	}
}


/**
    @brief �������ݿ������ȡ���ݿ������
    @param sqlString - 
    @param paramList - 
**/
void CustomLabel::getDataBaseParamList(QString tableName, QVector<QString>& paramList) {

	//mysql_query(&my_connection, "INSERT INTO `simulatedtraining`.`user_info`(`id`, `userName`, `password`) VALUES (4, 'lijin', '111')");

}

/**
    @brief ��дpaintEvent�ı����
    @param  - 
**/
void CustomLabel::paintEvent(QPaintEvent*) {



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


	//�����󶨱༭
	//��Ϊ0����˲���
	if (mBindingParamIndex!=0)
	{
		/*�������󶨵�����¼�,����ò�����Ҫ���£�����Ҫ�½�һ���̻߳�ȡʵʱ����*/
		qDebug() << "bindingPram";
	}

}


/**
    @brief 
    @param event - 
**/
void CustomLabel::mousePressEvent(QMouseEvent* event) {
	

	this->raise();

	//����������
#ifdef ONLY_CONTROL_COMPARAM
	//�źŲ�
	connect(this, &CUSTOM_LABEL::CustomLabel::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);
#endif // ONLY_CONTROL_COMPARAM
	//������ݸ���������Ӧ�۽��ն���ĸ���
	if (myParentType == MY_PARENT_TYPE::CUSTOM_LISTWIDGET)
	{
		//�źŲ�
		connect(this, &CUSTOM_LABEL::CustomLabel::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);

	}
	else
	{
		//�źŲ�
		connect(this, &CUSTOM_LABEL::CustomLabel::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);
	}

	/*����GroupBox*/
	if (myParentType==MY_PARENT_TYPE::CUSTOM_LISTWIDGET)
	{
		selection->addWidget(this);
		selection->show(this);
	}


	if (attributeWidget1.isActiveWindow() == false)
	{
		loadAttributeWidget();
	}
	

	if (event->button() == Qt::LeftButton)
	{
		selectSelf = true;

		m_point = event->globalPos();

		m_pos = this->frameGeometry().topLeft();

		dragStartPoint = event->pos();
	}
	mousePressed = true;
	QWidget::mousePressEvent(event);

	//�����������ź�
	emit displayAttribute(attributeWidget1);

	this->grabKeyboard();

}

void CustomLabel::mouseMoveEvent(QMouseEvent* event) {
	selection->clear();
	//mousePressed = false;
	this->releaseKeyboard();
	
	if (this->parent()->objectName()=="customGroupBox")
	{
		return;
	}
	
	if ((event->buttons() == Qt::LeftButton) && mousePressed)
	{

		QPoint relativePos = event->globalPos() - m_point;
		this->move(m_pos + relativePos);

		this->move(m_pos + relativePos);
	}

	
	QWidget::mouseMoveEvent(event);


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
    @brief ��Ӧ�����¼�
    @param event - 
**/
void CustomLabel::keyPressEvent(QKeyEvent* event) {

	//�Ȼ�ȡ�������
	if (!selectSelf)
	{
		return;
	}

	//��ӦDelete
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