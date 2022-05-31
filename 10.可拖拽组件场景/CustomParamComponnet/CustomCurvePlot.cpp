#include "CustomCurvePlot.h"
#include "CustomParamComponent.h"

class CUSTOM_PARAM_COMPONENT::CustomParamComponent;

using namespace CUSTOM_CURVE_PLOT;

CustomCurvePlot::CustomCurvePlot(QWidget* parent) :QCustomPlot(parent) {

	//attributeWidget = nullptr;
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

CustomCurvePlot::~CustomCurvePlot() {
	
}

QWidget* CustomCurvePlot::loadAttributeWidget() {
	{
		
		attributeWidget.setMaximumWidth(200);

		QGridLayout* attributeLayout = new QGridLayout();
		QLabel* label1 = new QLabel(QString::fromLocal8Bit("添加或删除绑定:"));
		
		QPushButton* buttonAdd = new QPushButton();
		buttonAdd->setIcon(QIcon(QPixmap("img/+hao.png")));
		QPushButton* buttonSub = new QPushButton();
		buttonSub->setIcon(QIcon(QPixmap("img/-hao.png")));

		QListWidget* paramListWidget = new QListWidget;
		connect(buttonAdd, &QPushButton::clicked, this, [=]() {
			hasAddButNotNameParamNum++;
			paramListWidget->addItem(QString::fromLocal8Bit("第%1个参数").arg(hasAddButNotNameParamNum));
			});

		connect(buttonSub, &QPushButton::clicked, this, [=]() {
			if (hasAddButNotNameParamNum == 0)
		{
				QMessageBox::warning(this, "Warning!!!", QString::fromLocal8Bit("参数个数为0!!!"));
				return;
		}
			qDebug() << paramListWidget->currentIndex().row();
			if (paramListWidget->currentIndex().row()!=-1)
			{
				auto item1=paramListWidget->takeItem(paramListWidget->currentRow());
				item1->~QListWidgetItem();
			}
			});

		connect(paramListWidget, &QListWidget::itemClicked, this, [=]() {
			currentSelectParam = paramListWidget->currentRow();
			});

		QGroupBox* groupBox = new QGroupBox();
		groupBox->setTitle(QString::fromLocal8Bit("项参数绑定"));

		QLabel* label2 = new QLabel(QString::fromLocal8Bit("线形底色:"));
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

		QLabel* label3 = new QLabel(QString::fromLocal8Bit("线形宽度:"));
		QComboBox* comboBox2 = new QComboBox();
		QVector<qreal> widthList = { 1.0,2.0,3.0 };
		for each (qreal width in widthList)
		{
			
			comboBox2->addItem(QString::number(width));
	
			comboBox2->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		}

		QPushButton* buttonParamOK = new QPushButton(QString::fromLocal8Bit("确认设置"));
		/*点击参数设置按钮之后设置对应参数绑定*/
		connect(buttonParamOK, &QPushButton::clicked, this, [=]() {

			});

		QGridLayout* boxLayout = new QGridLayout();
		boxLayout->addWidget(label2,0,0);
		boxLayout->addWidget(comboBox1,0,1);
		boxLayout->addWidget(label3,1,0);
		boxLayout->addWidget(comboBox2,1,1);
		boxLayout->addWidget(buttonParamOK, 3, 0);

		groupBox->setLayout(boxLayout);


		QPushButton* buttonOK = new QPushButton();
		buttonOK->setText(QString::fromLocal8Bit("应用控件配置"));
		
		
		attributeLayout->addWidget(label1, 0, 0,1,2);
		attributeLayout->addWidget(buttonAdd, 1, 0);
		attributeLayout->addWidget(buttonSub, 1, 1);
		attributeLayout->addWidget(paramListWidget, 2, 0, 1, 2);
		attributeLayout->addWidget(groupBox, 3, 0,1,2);
		attributeLayout->addWidget(buttonOK, 4, 1,1,1);

		

		//处理属性编辑窗口配置
		//全部配置完成之后保存至sql，并更新界面
		connect(buttonOK, &QPushButton::clicked, this, [=]() {
			/*操作Curve*/
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
#ifdef ONLY_CONTROL_COMPARAM
	connect(this, &CustomCurvePlot::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);

#endif
	if (myParentType == MY_PARENT_TYPE::CUSTOM_LISTWIDGET)
	{
		//信号槽
		connect(this, &CustomCurvePlot::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);

	}
	else
	{
		//信号槽
		connect(this, &CustomCurvePlot::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);
	}

	/*隔离GroupBox*/
	if (myParentType == MY_PARENT_TYPE::CUSTOM_LISTWIDGET)
	{
		selection->addWidget(this);
		selection->show(this);
	}

	/*内存优化*/
	if (attributeWidget.isActiveWindow() == false)
	{
		loadAttributeWidget();
	}

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