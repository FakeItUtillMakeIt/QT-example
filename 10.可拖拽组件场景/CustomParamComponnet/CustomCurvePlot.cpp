#include "CustomCurvePlot.h"
#include "CustomParamComponent.h"

class CUSTOM_PARAM_COMPONENT::CustomParamComponent;

using namespace CUSTOM_CURVE_PLOT;

/**
    @brief  构造函数
    @param  parent - 
    @retval        - 
**/
CustomCurvePlot::CustomCurvePlot(QWidget* parent) :QCustomPlot(parent) {

	//attributeWidget = nullptr;
	selectSelf = false;
	mousePressed = false;
	this->setObjectName("plot");
	if (parent->objectName() == "customGroupBox")
	{
		myParentType = MY_PARENT_TYPE::CUSTOM_GROUPBOX;
	}
	else
	{
		myParentType = MY_PARENT_TYPE::CUSTOM_LISTWIDGET;
	}

	xdata = 0;
}



CustomCurvePlot::~CustomCurvePlot() {
	
}


/**
	@brief 生成随机数据，供plot使用测试
	单独一个参数的话，不需要线程,可以直接从数据库或者传感器获取
**/
void CustomCurvePlot::generateShuffleData() {

	if (xdata > 10000)
		xdata = 1;

	xset.resize(10);
	xset = { 0,1,2,3,4,5,6,7,8,9 };
	yset.resize(10);
	std::default_random_engine rdGen;
	rdGen.seed(xdata++);
	std::uniform_int_distribution<int> dis(1, 10);
	testPlotData = dis(rdGen);

	if (yset.size() == 10)
	{
		yset.pop_front();
	}

	yset << testPlotData;
	qDebug() << xset << yset;
	paramCurves[currentSelectParam]->setData(xset, yset);
	replot();
}

QVector<int> CustomCurvePlot::getBindParamList() {
	return mBindingParamIndexList;
}

/**
    @brief  加载属性窗口
    @retval  - 
**/
QWidget* CustomCurvePlot::loadAttributeWidget() {
	{
		attributeWidget.setMaximumWidth(200);
		this->xAxis->setRange(0, 10);
		this->yAxis->setRange(0, 10);

		QGridLayout* attributeLayout = new QGridLayout();
		QLabel* label1 = new QLabel(QString::fromLocal8Bit("添加或删除绑定:"));
		
		QPushButton* buttonAdd = new QPushButton();
		buttonAdd->setIcon(QIcon(QPixmap("img/+hao.png")));
		QPushButton* buttonSub = new QPushButton();
		buttonSub->setIcon(QIcon(QPixmap("img/-hao.png")));

		QListWidget* paramListWidget = new QListWidget;

		//添加按钮对应操作
		connect(buttonAdd, &QPushButton::clicked, this, [=]() {
			hasAddButNotNameParamNum++;
			paramListWidget->addItem(QString::fromLocal8Bit("第%1个参数").arg(hasAddButNotNameParamNum));
			paramCurves.push_back(this->addGraph());
			QTimer* timer = new QTimer(this);
			paramTimers.push_back(timer);

			connect(timer, &QTimer::timeout, this, &CUSTOM_CURVE_PLOT::CustomCurvePlot::generateShuffleData);
			});

		//删除按钮对应操作
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
				
				paramCurves[currentSelectParam]->setVisible(false);
				paramCurves[currentSelectParam]->removeFromLegend();
				
				paramCurves.remove(currentSelectParam);
				paramTimers[currentSelectParam]->stop();
				paramTimers.remove(currentSelectParam);

				qDebug() << "paramCurveSize:" << paramCurves.size();
			}
		});

		QGroupBox* groupBox = new QGroupBox();
		groupBox->setTitle(QString::fromLocal8Bit("项参数绑定"));
		QPushButton* buttonOK = new QPushButton();
		buttonOK->setText(QString::fromLocal8Bit("应用控件配置"));

		//点击LIstWidget中的item项对应操作
		connect(paramListWidget, &QListWidget::itemClicked, this, [=]() {
			currentSelectParam = paramListWidget->currentRow();
			qDebug() << "current selected:" << currentSelectParam;
			groupBox->show();
			buttonOK->show();
			});

		QLabel* labelParam = new QLabel(QString::fromLocal8Bit("绑定参数:"));
		QComboBox* comboBoxParam = new QComboBox();

		/*这里打开数据库对应表获取参数表*/
		SingleDataController* controller = SingleDataController::getInstance();
		map<int, vector<string>> paramInfos = controller->getDataBaseParamInfo();

		QVector<QString> paramList1 = { "None bind param" };
		for (auto it = paramInfos.begin(); it != paramInfos.end(); it++)
		{
			paramList1.push_back(QString::fromStdString(it->second[0]));
		}

		for each (QString param in paramList1)
		{
			comboBoxParam->addItem(param);
			comboBoxParam->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		}

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
		/*点击参数设置按钮之后设置对应参数绑定，引入mBindingParam*/
		connect(buttonParamOK, &QPushButton::clicked, this, [=]() {
			mBindingParam[currentSelectParam] = "";
			paramCurves[currentSelectParam]->setPen(QPen(comboBox1->itemData(comboBox1->currentIndex()).value<QColor>()));

			});
		/************************************************************************/

		QGridLayout* boxLayout = new QGridLayout();
		boxLayout->addWidget(label2,1,0);
		boxLayout->addWidget(comboBox1,1,1);
		boxLayout->addWidget(label3,2,0);
		boxLayout->addWidget(comboBox2,2,1);
		boxLayout->addWidget(labelParam, 0, 0);
		boxLayout->addWidget(comboBoxParam, 0, 1);
		boxLayout->addWidget(buttonParamOK, 3, 0);

		groupBox->setLayout(boxLayout);

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
			paramTimers[currentSelectParam]->start(100);
			mBindingParamIndexList=mBindingParam.keys().toVector();
			//this->update();
			});

		if (!attributeWidget.layout())
		{
			attributeWidget.setLayout(attributeLayout);
		}
		groupBox->hide();
		buttonOK->hide();

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

/**
    @brief 鼠标离开时隐藏界面操作
    @param event - 
**/
void CustomCurvePlot::leaveEvent(QEvent* event) {
	this->releaseKeyboard();

	selection->hide(this);
	selection->removeWidget(this);
}

/**
    @brief 重写鼠标点击移动事件
    @param event - 
**/
void CustomCurvePlot::mouseMoveEvent(QMouseEvent* event) {

	//selection->clear();

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