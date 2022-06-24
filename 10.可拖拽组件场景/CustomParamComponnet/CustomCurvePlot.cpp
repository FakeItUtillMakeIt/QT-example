#include "CustomCurvePlot.h"
#include "CustomParamComponent.h"

class CUSTOM_PARAM_COMPONENT::CustomParamComponent;

using namespace CUSTOM_CURVE_PLOT;

/**
    @brief  ���캯��
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
	@brief ����������ݣ���plotʹ�ò���
	����һ�������Ļ�������Ҫ�߳�,����ֱ�Ӵ����ݿ���ߴ�������ȡ
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
    @brief  �������Դ���
    @retval  - 
**/
QWidget* CustomCurvePlot::loadAttributeWidget() {
	{
		attributeWidget.setMaximumWidth(200);
		this->xAxis->setRange(0, 10);
		this->yAxis->setRange(0, 10);

		QGridLayout* attributeLayout = new QGridLayout();
		QLabel* label1 = new QLabel(QString::fromLocal8Bit("��ӻ�ɾ����:"));
		
		QPushButton* buttonAdd = new QPushButton();
		buttonAdd->setIcon(QIcon(QPixmap("img/+hao.png")));
		QPushButton* buttonSub = new QPushButton();
		buttonSub->setIcon(QIcon(QPixmap("img/-hao.png")));

		QListWidget* paramListWidget = new QListWidget;

		//��Ӱ�ť��Ӧ����
		connect(buttonAdd, &QPushButton::clicked, this, [=]() {
			hasAddButNotNameParamNum++;
			paramListWidget->addItem(QString::fromLocal8Bit("��%1������").arg(hasAddButNotNameParamNum));
			paramCurves.push_back(this->addGraph());
			QTimer* timer = new QTimer(this);
			paramTimers.push_back(timer);

			connect(timer, &QTimer::timeout, this, &CUSTOM_CURVE_PLOT::CustomCurvePlot::generateShuffleData);
			});

		//ɾ����ť��Ӧ����
		connect(buttonSub, &QPushButton::clicked, this, [=]() {
			if (hasAddButNotNameParamNum == 0)
		{
				QMessageBox::warning(this, "Warning!!!", QString::fromLocal8Bit("��������Ϊ0!!!"));
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
		groupBox->setTitle(QString::fromLocal8Bit("�������"));
		QPushButton* buttonOK = new QPushButton();
		buttonOK->setText(QString::fromLocal8Bit("Ӧ�ÿؼ�����"));

		//���LIstWidget�е�item���Ӧ����
		connect(paramListWidget, &QListWidget::itemClicked, this, [=]() {
			currentSelectParam = paramListWidget->currentRow();
			qDebug() << "current selected:" << currentSelectParam;
			groupBox->show();
			buttonOK->show();
			});

		QLabel* labelParam = new QLabel(QString::fromLocal8Bit("�󶨲���:"));
		QComboBox* comboBoxParam = new QComboBox();

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
			comboBoxParam->addItem(param);
			comboBoxParam->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		}

		QLabel* label2 = new QLabel(QString::fromLocal8Bit("���ε�ɫ:"));
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

		QLabel* label3 = new QLabel(QString::fromLocal8Bit("���ο��:"));
		QComboBox* comboBox2 = new QComboBox();
		QVector<qreal> widthList = { 1.0,2.0,3.0 };
		for each (qreal width in widthList)
		{
			
			comboBox2->addItem(QString::number(width));
			comboBox2->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		}

		QPushButton* buttonParamOK = new QPushButton(QString::fromLocal8Bit("ȷ������"));
		/*����������ð�ť֮�����ö�Ӧ�����󶨣�����mBindingParam*/
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

		//�������Ա༭��������
		//ȫ���������֮�󱣴���sql�������½���
		connect(buttonOK, &QPushButton::clicked, this, [=]() {
			/*����Curve*/
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
	//�źŲ�
#ifdef ONLY_CONTROL_COMPARAM
	connect(this, &CustomCurvePlot::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);

#endif
	if (myParentType == MY_PARENT_TYPE::CUSTOM_LISTWIDGET)
	{
		//�źŲ�
		connect(this, &CustomCurvePlot::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);

	}
	else
	{
		//�źŲ�
		connect(this, &CustomCurvePlot::displayAttribute, static_cast<CUSTOM_PARAM_COMPONENT::CustomParamComponent*>(this->parent()->parent()->parent()), &CUSTOM_PARAM_COMPONENT::CustomParamComponent::displayAttributeWindow);
	}

	/*����GroupBox*/
	if (myParentType == MY_PARENT_TYPE::CUSTOM_LISTWIDGET)
	{
		selection->addWidget(this);
		selection->show(this);
	}

	/*�ڴ��Ż�*/
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
    @brief ����뿪ʱ���ؽ������
    @param event - 
**/
void CustomCurvePlot::leaveEvent(QEvent* event) {
	this->releaseKeyboard();

	selection->hide(this);
	selection->removeWidget(this);
}

/**
    @brief ��д������ƶ��¼�
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
	@brief ��Ӧ�����¼�
	@param event -
**/
void CustomCurvePlot::keyPressEvent(QKeyEvent* event) {

	//�Ȼ�ȡ�������
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