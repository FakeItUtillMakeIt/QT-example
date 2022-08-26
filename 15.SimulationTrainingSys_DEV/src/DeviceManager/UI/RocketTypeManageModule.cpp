#include "RocketTypeManageModule.h"
#include "DeviceManager.h"



RocketTypeManageModule::RocketTypeManageModule(QWidget* parent)
	: QWidget(parent)
{
	selectedRowNum = -1;
	columnNameList << QString("���ID") << QString("����ͺ�����") << QString("����ͺ�����") << QString("����");


	InitUILayout();

	InitDisplayData();

	configInfoTable->setColumnHidden(0, true);

	//����ui.comboBoxʱ���´�������
	connect(static_cast<DeviceManager*>(this->parent()->parent()->parent()->parent()), &DeviceManager::rocketTypeChanged, this, [=]() {
		qDebug() << AppCache::instance()->m_CurrentRocketType->m_name.c_str();
		InitDisplayData();
		});

	connect(RocketInfoConfig::InfoConfigWidget::getInstance(), &RocketInfoConfig::InfoConfigWidget::updateRocketInfo, this, [=]() {
		//Ҫ�������ݿ�
		InitDisplayData();

		});

	connect(AddRocketTypeWidget::getInstance(), &AddRocketTypeWidget::updateRocketInfos, this, [=]() {
		InitDisplayData();

		});
}

RocketTypeManageModule::~RocketTypeManageModule()
{}

/**
	@brief �豸������沼��
**/
void RocketTypeManageModule::InitUILayout() {
	//�͵ײ�״̬����ɫ����һ��


	WidgetStyleSheet* wss = WidgetStyleSheet::getInstace();

	QGridLayout* mainUILay = new QGridLayout;

	QLabel* deviceParamLabel = new QLabel(QString("����ͺŲ�ѯ:"));
	QComboBox* deviceCombox = new QComboBox();
	QLabel* paramNameLabel = new QLabel(QString("����ͺ�:"));
	QLineEdit* paramInputName = new QLineEdit;

	QPushButton* insertButton = new QPushButton;
	QPushButton* deleteButton = new QPushButton;
	QPushButton* updateButton = new QPushButton;
	QPushButton* settingButton = new QPushButton;
	QPushButton* collapseButton = new QPushButton;
	QPushButton* expandButton = new QPushButton;
	/*insertButton->setIcon(QIcon(":/icon/icon/+hao.png"));
	deleteButton->setIcon(QIcon(":/icon/icon/-hao.png"));*/
	insertButton->setText(QString("����"));
	deleteButton->setText(QString("ɾ��"));
	updateButton->setIcon(QIcon(":/icon/icon/update.png"));
	settingButton->setIcon(QIcon(":/icon/icon/setting.png"));
	collapseButton->setIcon(QIcon(":/icon/icon/collapse.png"));
	expandButton->setIcon(QIcon(":/icon/icon/expand.png"));

	configInfoTable = new QTableWidget;
	configInfoTable->setRowCount(1);
	configInfoTable->setColumnCount(columnNameList.size());
	//configInfoTable->horizontalHeader()->hide();
	//configInfoTable->verticalHeader()->hide();
	deleteButton->hide();
	updateButton->hide();
	settingButton->hide();
	collapseButton->hide();
	expandButton->hide();


	//mainUILay->addWidget(deviceParamLabel, 0, 0, 1, 1);
	//mainUILay->addWidget(deviceCombox, 0, 1, 1, 1);
	mainUILay->setColumnStretch(0, 2);
	mainUILay->addWidget(paramNameLabel, 0, 1, 1, 1);
	mainUILay->addWidget(paramInputName, 0, 2, 1, 1);
	mainUILay->addWidget(new QLabel("   "), 0, 3, 1, 1);
	mainUILay->addWidget(insertButton, 0, 4, 1, 1);
	mainUILay->addWidget(deleteButton, 0, 5, 1, 1);
	mainUILay->addWidget(updateButton, 0, 6, 1, 1);
	mainUILay->addWidget(settingButton, 0, 7, 1, 1);
	mainUILay->addWidget(collapseButton, 0, 8, 1, 1);
	mainUILay->addWidget(expandButton, 0, 9, 1, 1);
	mainUILay->addWidget(configInfoTable, 1, 0, 4, 10);

	paramNameLabel->setStyleSheet(wss->labelStyleSheet.arg("QLabel"));
	paramInputName->setStyleSheet(wss->lineEditStyleSheet.arg("QLineEdit"));
	insertButton->setStyleSheet(QString("QPushButton{color:white;}") + wss->insertButtonStyleSheet.arg("QPushButton"));
	deleteButton->setStyleSheet(QString("QPushButton{color:red;}") + wss->deleteButtonStyleSheet.arg("QPushButton"));
	updateButton->setStyleSheet(wss->iconButtonStyleSheet.arg("QPushButton"));
	settingButton->setStyleSheet(wss->iconButtonStyleSheet.arg("QPushButton"));
	collapseButton->setStyleSheet(wss->iconButtonStyleSheet.arg("QPushButton"));
	expandButton->setStyleSheet(wss->iconButtonStyleSheet.arg("QPushButton"));

	//configInfoTable->setVerticalHeaderItem(0, new QTableWidgetItem("���"));
	for (int col = 0; col < configInfoTable->columnCount(); col++)
	{

		QTableWidgetItem* item = new QTableWidgetItem();
		item->setText(columnNameList[col]);
		configInfoTable->setItem(0, col, new QTableWidgetItem);
		configInfoTable->setHorizontalHeaderItem(col, item);
	}

	configInfoTable->horizontalHeader()->setStretchLastSection(true);

	mainUILay->setContentsMargins(0, 0, 0, 0);

	this->setLayout(mainUILay);

	connect(insertButton, &QPushButton::clicked, this, &RocketTypeManageModule::insertOneRowData);
	connect(configInfoTable, &QTableWidget::itemClicked, this, [=]() {
		selectedRowNum = configInfoTable->currentRow();
		});
	connect(deleteButton, &QPushButton::clicked, this, &RocketTypeManageModule::deleteOneRowData);

	connect(paramInputName, &QLineEdit::textEdited, this, [=](QString inputText) {
		if (inputText.isEmpty())
		{
			configInfoTable->clearContents();

			InitDisplayData();
			return;
		}

		configInfoTable->clearContents();
		configInfoTable->setRowCount(0);
		DeviceDBConfigInfo::getInstance()->readRocketDB2UI();
		int searchRow = 0;
		for (auto ele = DeviceDBConfigInfo::getInstance()->rocketInfo.begin(); ele != DeviceDBConfigInfo::getInstance()->rocketInfo.end(); ele++)
		{
			if (QString::fromStdString(ele->second[1]).contains(paramInputName->text()))
			{
				QVector<QString> rowData;
				rowData.push_back(QString::fromStdString(ele->second[0]));
				rowData.push_back(QString::fromStdString(ele->second[1]));
				rowData.push_back(QString::fromStdString(ele->second[2]));

				insertOneRow(searchRow++, rowData);
			}
		}
		});
}


/**
	@brief ����һ��
	@param  -
**/
void RocketTypeManageModule::insertOneRow(int insertRow, QVector<QString> rowData) {
	configInfoTable->insertRow(insertRow);
	for (int i = 0; i < columnNameList.size() - 1; i++)
	{
		configInfoTable->setItem(insertRow, i, new  QTableWidgetItem(rowData[i]));
	}

	QWidget* w1 = new QWidget;
	QHBoxLayout* hbox = new QHBoxLayout;
	QPushButton* opEditBtn = new QPushButton(QString("�༭"));
	opEditBtn->setProperty("row", insertRow);
	QPushButton* opDeleteBtn = new QPushButton(QString("ɾ��"));
	opDeleteBtn->setProperty("row", insertRow);
	QPushButton* opCfgDataBtn = new QPushButton(QString("����"));
	opCfgDataBtn->setProperty("row", insertRow);

	//#ifdef NEW_UI
	//	opCfgDataBtn->show();
	//#endif // NEW_UI


#ifdef OLD_UI
	opCfgDataBtn->hide();
	//�༭ʱ��������  infoConfigWidget���ڣ�����ǰ����Ϣ�������ڣ��༭
	connect(opEditBtn, &QPushButton::clicked, this, [=]() {

		int curRow = opEditBtn->property("row").toInt();
		qDebug() << curRow;

		RocketInfoConfig::InfoConfigWidget::getInstance()->currentDeviceFlag = DeviceCommonVaries::getInstance()->DeviceModule::UPDATE_MODULE;
		RocketInfoConfig::InfoConfigWidget::getInstance()->editId = configInfoTable->item(curRow, 0)->text().toInt();
		RocketInfoConfig::InfoConfigWidget::getInstance()->userInputRocketName->setText(configInfoTable->item(curRow, 1)->text());
		RocketInfoConfig::InfoConfigWidget::getInstance()->userInputRocketDescript->setText(configInfoTable->item(curRow, 2)->text());

		RocketInfoConfig::InfoConfigWidget::getInstance()->show();
		});
#endif // OLD_UI

	hbox->addWidget(opEditBtn);
	hbox->addWidget(opCfgDataBtn);
	hbox->addWidget(opDeleteBtn);

	w1->setLayout(hbox);
	w1->setStyleSheet("*{border:none;color:blue;}");


	configInfoTable->setCellWidget(insertRow, columnNameList.size() - 1, w1);

	connect(opEditBtn, &QPushButton::clicked, this, [=]() {
		auto editW = AddRocketTypeWidget::getInstance();
		editW->setInfoWidget(DeviceCommonVaries::ROCKET_WIDGET);
		editW->setWindowName(QString("�༭����ͺ�"));
		int curRow = opEditBtn->property("row").toInt();
		editW->setRocketInfo(configInfoTable->item(curRow, 0)->text().toInt(), configInfoTable->item(curRow, 1)->text(),
			configInfoTable->item(curRow, 2)->text());
		editW->show();
		});

	connect(opDeleteBtn, &QPushButton::clicked, this, [=]() {
		removeOneRow(opDeleteBtn->property("row").toInt());
		});

	connect(opCfgDataBtn, &QPushButton::clicked, this, [=]() {
		//��ȡ��ǰ����ͺ�
		AllInfoConfigWidget* w = AllInfoConfigWidget::getInstance();
		w->setInfoWidgetCfg(configInfoTable->item(opCfgDataBtn->property("row").toInt(), 0)->text().toInt(), -1, -1);
		w->setCurrentUI(DeviceCommonVaries::InfoWidgetFlag::ROCKET_WIDGET);
		w->setWindowTitle(configInfoTable->item(opCfgDataBtn->property("row").toInt(), 1)->text() + QString("-��������ͨ��Э������"));

		w->show();
		});

}


/**
	@brief ɾ��һ��
**/
void RocketTypeManageModule::removeOneRow(int removeRow) {
	int ret = QMessageBox::warning(RocketInfoConfig::InfoConfigWidget::getInstance(), QString("����"), QString("ȷ��ɾ����ǰ������"), "ȡ��", "ȷ��");
	if (ret == 0)
	{
		return;
	}
	int curRocketID = configInfoTable->item(removeRow, 0)->text().toInt();
	//ɾ������

	QString qSqlString = QString("DELETE FROM `simulatedtraining`.`rocket_info` WHERE `id` = %1").arg(curRocketID);
	DeviceDBConfigInfo::getInstance()->customRunSql(qSqlString);
	configInfoTable->removeRow(removeRow);
}

/**
	@brief �༭����
	@param  -�༭�������
**/
void RocketTypeManageModule::editOneRow(int rocketID, QString rocketName, QString  rocketCode) {
	//������Ҫ�������ݿ�
	DeviceDBConfigInfo::getInstance()->updateRocketInfo2DB(rocketID, rocketName, rocketCode);
}

/**
	@brief ��ʼ����ʾ����  ��ʼ����ʾʱ  ����ʾ�͵�ǰѡ������ص���Ŀ
**/
void RocketTypeManageModule::InitDisplayData() {

	configInfoTable->clearContents();
	configInfoTable->setRowCount(0);
	DeviceDBConfigInfo* rocketInfoDB = DeviceDBConfigInfo::getInstance();
	rocketInfoDB->readRocketDB2UI();
	//configInfoTable->setRowCount(rocketInfoDB->rocketInfo.size());
	int row = 0;
	for (auto ele = rocketInfoDB->rocketInfo.begin(); ele != rocketInfoDB->rocketInfo.end(); ele++)
	{

		QVector<QString> rowData;
		rowData.push_back(QString::fromStdString(ele->second[0]));
		rowData.push_back(QString::fromStdString(ele->second[1]));
		rowData.push_back(QString::fromStdString(ele->second[2]));

		insertOneRow(row++, rowData);

	}
}



void RocketTypeManageModule::paintEvent(QPaintEvent* event) {

	for (int columnIndex = 0; columnIndex < configInfoTable->columnCount(); columnIndex++)
	{
		//displayWidget->setColumnWidth(columnIndex, (this->parentWidget()->width()) / (displayWidget->columnCount() + 1));
		configInfoTable->setColumnWidth(columnIndex, (configInfoTable->width() - 100) / (configInfoTable->columnCount()));
	}

	configInfoTable->horizontalHeader()->setStretchLastSection(true);
	configInfoTable->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
}

/**
	@brief ����һ������
**/
void RocketTypeManageModule::insertOneRowData() {

#ifdef NEW_UI

	AddRocketTypeWidget* addRocketTypeW = AddRocketTypeWidget::getInstance();
	addRocketTypeW->setInfoWidget(DeviceCommonVaries::ROCKET_WIDGET);
	addRocketTypeW->setWindowName(QString("��������ͺ�"));
	addRocketTypeW->setRocketInfo(0, "", "");
	addRocketTypeW->show();
#endif // !1

#ifdef OLD_UI

	RocketInfoConfig::InfoConfigWidget* infoConfigWidget = RocketInfoConfig::InfoConfigWidget::getInstance();
	infoConfigWidget->show();
#endif // DEBUG
	//connect(infoConfigWidget, &RocketInfoConfig::InfoConfigWidget::updateRocketInfo, this, [=]() {
	//	//Ҫ�������ݿ�
	//	InitDisplayData();

	//	});
}

/**
	@brief ɾ��һ������  ��ͬ�����ݿ�  �˴��漰�Ķ��ϴ���ʱ����
**/
void RocketTypeManageModule::deleteOneRowData() {
	if (selectedRowNum != -1)
	{
		configInfoTable->removeRow(selectedRowNum);

	}
}


/**
	@brief	   ����������ô���
	@retval  -
**/
RocketDataCfgW::RocketDataCfgW() {



	scrollArea = new QTableWidget;

	QVBoxLayout* vlayout = new QVBoxLayout;
	QHBoxLayout* hlayout = new QHBoxLayout;
	QHBoxLayout* bottomHlayout = new QHBoxLayout;

	scrollArea->setColumnCount(3);

	scrollArea->setHorizontalHeaderItem(0, new QTableWidgetItem(QString("ָ������")));
	scrollArea->setHorizontalHeaderItem(1, new QTableWidgetItem(QString("ָ�����")));
	scrollArea->setHorizontalHeaderItem(2, new QTableWidgetItem(QString("ָ��ǰ׺")));

	scrollArea->horizontalHeader()->setStretchLastSection(true);
	scrollArea->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);


	int cmdIndex = 0;
	for each (auto var in AppCache::instance()->m_allCommad)
	{

		scrollArea->insertRow(cmdIndex);

		QWidget* firstColumnW = new QWidget;
		QHBoxLayout* firstLayout = new QHBoxLayout;
		firstLayout->addWidget(new QCheckBox);
		firstLayout->addWidget(new QLabel(QString::fromLocal8Bit(var.second->m_sName.c_str())));
		firstColumnW->setLayout(firstLayout);

		scrollArea->setCellWidget(cmdIndex, 0, firstColumnW);

		scrollArea->setCellWidget(cmdIndex, 1, new QLineEdit);
		scrollArea->setCellWidget(cmdIndex, 2, new QLineEdit);
		scrollArea->setRowHeight(cmdIndex, 40);
		cmdIndex++;

	}

	QPushButton* rOKBtn = new QPushButton(QString("���"));
	QPushButton* rCancelBtn = new QPushButton(QString("ȡ��"));

	scrollArea->setFixedSize(480, 500);
	vlayout->addWidget(scrollArea);

	bottomHlayout->addStretch(4);
	bottomHlayout->addWidget(rCancelBtn);
	bottomHlayout->addWidget(rOKBtn);
	vlayout->addLayout(bottomHlayout);

	this->setLayout(vlayout);
	this->setFixedSize(500, 580);
	this->setWindowTitle(QString("���������Ϣ����"));
	this->setWindowFlags(Qt::WindowStaysOnTopHint);

}

/**
	@brief ��ȡtable��ѡ�е�����ָ����Ϣ
**/
void RocketDataCfgW::getDataInfoCfgData() {

}

/**
	@brief
	@retval  -
**/
RocketParamCfgW::RocketParamCfgW() {

	tableArea = new QTableWidget;


}


/**
	@brief  �����������
	@retval  -
**/
RocketParamCfgW1::RocketParamCfgW1() {

	paramIndex = new QLabel(QString("��������:"));
	paramLength = new QLabel(QString("�����ֽڳ���:"));
	paramType = new QLabel(QString("��������:"));
	userInputParamLength = new QLineEdit;
	userInputParamIndex = new QLineEdit;
	userSlectParamType = new QComboBox;

	cancelCfgBtn = new QPushButton(QString("ȡ��"));
	okCfgBtn = new QPushButton(QString("ȷ��"));

	paramInfoL = new QGridLayout;
	QVBoxLayout* vlayout = new QVBoxLayout;
	QHBoxLayout* hbox = new QHBoxLayout;
	paramInfoL->addWidget(paramIndex, 0, 0);
	paramInfoL->addWidget(paramLength, 1, 0);
	paramInfoL->addWidget(paramType, 2, 0);
	paramInfoL->addWidget(userInputParamIndex, 0, 1);
	paramInfoL->addWidget(userInputParamLength, 1, 1);
	paramInfoL->addWidget(userSlectParamType, 2, 1);

	hbox->addWidget(cancelCfgBtn);
	hbox->addWidget(okCfgBtn);

	vlayout->addLayout(paramInfoL);
	vlayout->addSpacing(440);
	vlayout->addLayout(hbox);

	treeArea = new QTreeWidget;

	QStringList headerLabels;
	headerLabels << QString("���Ϸ���ָ��") << QString("��������");
	treeArea->setColumnCount(headerLabels.count());
	treeArea->setHeaderLabels(headerLabels);

	DeviceDBConfigInfo::getInstance()->readCommandDB2UI();
	DeviceDBConfigInfo::getInstance()->readParamDB2UI();
	for each (auto var in DeviceDBConfigInfo::getInstance()->commandInfo)
	{
		QTreeWidgetItem* root = new QTreeWidgetItem;
		root->setCheckState(0, Qt::Unchecked);
		root->setText(0, QString::fromStdString(var.second[3].c_str()));
		for each (auto var1 in DeviceDBConfigInfo::getInstance()->paramInfo)
		{
			QTreeWidgetItem* subItem = new QTreeWidgetItem;
			subItem->setCheckState(1, Qt::Unchecked);
			subItem->setText(1, QString::fromStdString(var1.second[1].c_str()));

			root->addChild(subItem);

		}
		treeArea->addTopLevelItem(root);

	}


	QHBoxLayout* hlayout = new QHBoxLayout;
	hlayout->addWidget(treeArea);
	hlayout->addLayout(vlayout);

	this->setLayout(hlayout);
	this->setFixedSize(480, 600);
	this->setWindowFlags(Qt::WindowStaysOnTopHint);
	displayParamInfo(false);

	connect(treeArea, &QTreeWidget::itemClicked, this, &RocketParamCfgW1::resClickParam);

	connect(okCfgBtn, &QPushButton::clicked, this, [=]() {

		});
}

/**
	@brief
**/
void RocketParamCfgW1::displayParamInfo(bool dis) {
	if (dis == true)
	{
		paramIndex->show();
		paramLength->show();
		paramType->show();
		userInputParamIndex->show();
		userInputParamLength->show();
		userSlectParamType->show();
	}
	else
	{
		paramIndex->hide();
		paramLength->hide();
		paramType->hide();
		userInputParamIndex->hide();
		userInputParamLength->hide();
		userSlectParamType->hide();
	}
}

/**
	@brief ��Ӧ��������ź�
	@param item   -
	@param column -
**/
void RocketParamCfgW1::resClickParam(QTreeWidgetItem* item, int column) {

	if (item->isSelected() && column == 1)
	{
		displayParamInfo(true);
	}
	else
	{
		displayParamInfo(false);
	}
}