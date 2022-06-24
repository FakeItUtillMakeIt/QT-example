#include "CustomLabel.h"
#include "CustomParamComponent.h"
#include "DropWidget.h"

class DROP_WIDGET::DropWidget;

class CUSTOM_PARAM_COMPONENT::CustomParamComponent;

using namespace CUSTOM_LABEL;


/**
    @brief  构造函数
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
	//默认不绑定参数
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
    @brief  加载属性窗口
    @retval  - 
**/
QWidget* CustomLabel::loadAttributeWidget() {
	{

		attributeWidget1.setMaximumWidth(200);

		QGridLayout* attributeLayout = new QGridLayout();
		QLabel* label1 = new QLabel(QString::fromLocal8Bit("控件名称:"));
		QLineEdit* lineEdit1 = new QLineEdit();
		lineEdit1->setReadOnly(false);
		lineEdit1->setEnabled(true);
		QLabel* label2 = new QLabel(QString::fromLocal8Bit("控件底色"));
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

		QLabel* label3 = new QLabel(QString::fromLocal8Bit("参数绑定:"));
		QComboBox* comboBox2 = new QComboBox();
		/*这里打开数据库对应表获取参数表*/
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
		button1->setText(QString::fromLocal8Bit("应用控件配置"));

		attributeLayout->addWidget(label1, 0, 0);
		attributeLayout->addWidget(lineEdit1, 0, 1);
		attributeLayout->addWidget(label2, 1, 0);
		attributeLayout->addWidget(comboBox1, 1, 1);
		//根据是否配置绑定参数确定这个label是纯label还是需要绑定刷新参数的label
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
    @brief 根据数据库表名获取数据库参数表
    @param sqlString - 
    @param paramList - 
**/
void CustomLabel::getDataBaseParamList(QString tableName, QVector<QString>& paramList) {

	//mysql_query(&my_connection, "INSERT INTO `simulatedtraining`.`user_info`(`id`, `userName`, `password`) VALUES (4, 'lijin', '111')");

}

/**
    @brief 重写paintEvent改变外观
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


	//参数绑定编辑
	//不为0则绑定了参数
	if (mBindingParamIndex!=0)
	{
		/*做参数绑定的相关事件,假如该参数需要更新，则需要新建一个线程获取实时数据*/
		qDebug() << "bindingPram";
	}

}


/**
    @brief 
    @param event - 
**/
void CustomLabel::mousePressEvent(QMouseEvent* event) {
	

	this->raise();

	//呼出属性栏
#ifdef ONLY_CONTROL_COMPARAM
	//信号槽
	connect(this, &CUSTOM_LABEL::CustomLabel::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);
#endif // ONLY_CONTROL_COMPARAM
	//这里根据父类型做相应槽接收对象的更改
	if (myParentType == MY_PARENT_TYPE::CUSTOM_LISTWIDGET)
	{
		//信号槽
		connect(this, &CUSTOM_LABEL::CustomLabel::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);

	}
	else
	{
		//信号槽
		connect(this, &CUSTOM_LABEL::CustomLabel::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);
	}

	/*隔离GroupBox*/
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

	//呼出属性栏信号
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