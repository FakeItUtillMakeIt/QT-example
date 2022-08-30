#include "DeviceManageModule.h"
#include "DeviceManager.h"

DeviceManageModule::DeviceManageModule(QWidget* parent)
	: QWidget(parent)
{

	selectedRowNum = -1;
	columnNameList << QString("设备ID") << QString("火箭型号") << QString("名称") << QString("类型") << QString("操作");

	InitUILayout();

	InitDisplayData();

	//隐藏ID列
	configInfoTable->setColumnHidden(0, true);

	connect(static_cast<DeviceManager*>(this->parent()->parent()->parent()->parent()->parent()->parent()->parent()), &DeviceManager::rocketTypeChanged, this, [=]() {
		qDebug() << AppCache::instance()->m_CurrentRocketType->m_name.c_str();
		InitDisplayData();
		});

	//更新
	connect(DeviceInfoConfig::InfoConfigWidget::getInstance(), &DeviceInfoConfig::InfoConfigWidget::updateDeviceInfo, this, [=]() {
		InitDisplayData();
		});

	connect(AddRocketTypeWidget::getInstance(), &AddRocketTypeWidget::updateDeviceInfos, this, [=]() {
		InitDisplayData();

		});
}

DeviceManageModule::~DeviceManageModule()
{}

/**
	@brief 设备管理界面布局
**/
void DeviceManageModule::InitUILayout() {
	//和底部状态栏颜色保持一致
	WidgetStyleSheet* wss = WidgetStyleSheet::getInstace();

	QGridLayout* mainUILay = new QGridLayout;

	QLabel* deviceParamLabel = new QLabel(QString("设备名称查询:"));
	QComboBox* deviceCombox = new QComboBox();
	QLabel* paramNameLabel = new QLabel(QString("设备名称:"));
	QLineEdit* paramInputName = new QLineEdit;

	QPushButton* insertButton = new QPushButton;
	QPushButton* deleteButton = new QPushButton;
	QPushButton* updateButton = new QPushButton;
	QPushButton* settingButton = new QPushButton;
	QPushButton* collapseButton = new QPushButton;
	QPushButton* expandButton = new QPushButton;
	/*insertButton->setIcon(QIcon(":/icon/icon/+hao.png"));
	deleteButton->setIcon(QIcon(":/icon/icon/-hao.png"));*/
	insertButton->setText(QString("新增"));
	deleteButton->setText(QString("删除"));
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

	//configInfoTable->setVerticalHeaderItem(0, new QTableWidgetItem("序号"));
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

	connect(insertButton, &QPushButton::clicked, this, &DeviceManageModule::insertOneRowData);
	connect(configInfoTable, &QTableWidget::itemClicked, this, [=]() {
		selectedRowNum = configInfoTable->currentRow();
		});
	connect(deleteButton, &QPushButton::clicked, this, &DeviceManageModule::deleteOneRowData);

	connect(paramInputName, &QLineEdit::textEdited, this, [=](QString inputText) {
		if (inputText.isEmpty())
		{
			configInfoTable->clearContents();
			InitDisplayData();
			return;
		}

		configInfoTable->clearContents();
		configInfoTable->setRowCount(0);
		DeviceDBConfigInfo::getInstance()->readDeviceDB2UI();
		DeviceDBConfigInfo::getInstance()->readRocketDB2UI();
		int searchRow = 0;
		for (auto ele = DeviceDBConfigInfo::getInstance()->deviceInfo.begin(); ele != DeviceDBConfigInfo::getInstance()->deviceInfo.end(); ele++)
		{
			if (QString::fromStdString(ele->second[2]).contains(paramInputName->text()))
			{
				QVector<QString> rowData;
				rowData.push_back(QString::fromStdString(ele->second[0]));

				QString tmpp1, tmpp2;

				if (DeviceDBConfigInfo::getInstance()->rocketInfo[atoi(ele->second[1].c_str())].size() < 3)
				{
					tmpp1 = QString::fromStdString(ele->second[1]);

				}
				else
				{
					tmpp1 = QString::fromStdString(DeviceDBConfigInfo::getInstance()->rocketInfo[atoi(ele->second[1].c_str())][1]);

				}
				rowData.push_back(tmpp1);
				tmpp2 = QString::fromLocal8Bit(DeviceCommonVaries::getInstance()->deviceIndex2Type[atoi(ele->second[3].c_str())].c_str());
				//rowData.push_back(QString::fromStdString(ele->second[1]));
				rowData.push_back(QString::fromStdString(ele->second[2]));
				rowData.push_back(tmpp2);
				insertOneRow(searchRow++, rowData);
			}
		}
		});

}

/**
	@brief 新增一行
	@param  -
**/
void DeviceManageModule::insertOneRow(int insertRow, QVector<QString> rowData) {
	configInfoTable->insertRow(insertRow);
	for (int i = 0; i < columnNameList.size() - 1; i++)
	{
		configInfoTable->setItem(insertRow, i, new  QTableWidgetItem(rowData[i]));
	}

	QWidget* w1 = new QWidget;
	QHBoxLayout* hbox = new QHBoxLayout;
	QPushButton* opEditBtn = new QPushButton(QString("编辑"));
	opEditBtn->setProperty("row", insertRow);
	QPushButton* opDeleteBtn = new QPushButton(QString("删除"));
	opDeleteBtn->setProperty("row", insertRow);
	QPushButton* opCfgDevStatBtn = new QPushButton(QString("配置"));
	opCfgDevStatBtn->setProperty("row", insertRow);

	//#ifdef NEW_UI
	//	opCfgDevStatBtn->show();
	//#endif // NEW_UI
	//

#ifdef OLD_UI
	opCfgDevStatBtn->hide();
	connect(opEditBtn, &QPushButton::clicked, this, [=]() {


		int curRow = opEditBtn->property("row").toInt();
		qDebug() << curRow;
		DeviceInfoConfig::InfoConfigWidget::getInstance()->currentDeviceFlag = DeviceCommonVaries::getInstance()->DeviceModule::UPDATE_MODULE;
		DeviceInfoConfig::InfoConfigWidget::getInstance()->editId = configInfoTable->item(curRow, 0)->text().toInt();
		DeviceInfoConfig::InfoConfigWidget::getInstance()->userSelectRocketType->setCurrentText(configInfoTable->item(curRow, 1)->text());
		DeviceInfoConfig::InfoConfigWidget::getInstance()->userInputDeviceName->setText(configInfoTable->item(curRow, 2)->text());
		DeviceInfoConfig::InfoConfigWidget::getInstance()->userSelectDeviceType->setCurrentText(configInfoTable->item(curRow, 3)->text());
		DeviceInfoConfig::InfoConfigWidget::getInstance()->show();
		});
#endif // OLD_UI

	hbox->addWidget(opEditBtn);
	hbox->addWidget(opCfgDevStatBtn);
	hbox->addWidget(opDeleteBtn);

	w1->setLayout(hbox);
	w1->setStyleSheet("*{border:none;color:blue;}");

	configInfoTable->setCellWidget(insertRow, columnNameList.size() - 1, w1);

	connect(opEditBtn, &QPushButton::clicked, this, [=]() {
		auto editW = AddRocketTypeWidget::getInstance();
		editW->setInfoWidget(DeviceCommonVaries::DEVICE_WIDGET);
		editW->setWindowName(QString("编辑设备"));
		int curRow = opEditBtn->property("row").toInt();
		editW->setDevInfo(configInfoTable->item(curRow, 0)->text().toInt(), configInfoTable->item(curRow, 1)->text(),
			configInfoTable->item(curRow, 2)->text(), configInfoTable->item(curRow, 3)->text());
		editW->show();
		});

	connect(opDeleteBtn, &QPushButton::clicked, this, [=]() {
		removeOneRow(opDeleteBtn->property("row").toInt());
		});

	connect(opCfgDevStatBtn, &QPushButton::clicked, this, [=]() {
		//

		//获取当前火箭型号
		AllInfoConfigWidget* w = AllInfoConfigWidget::getInstance();
		w->setInfoWidgetCfg(AppCache::instance()->m_CurrentRocketType->m_id, configInfoTable->item(opDeleteBtn->property("row").toInt(), 0)->text().toInt(), -1);
		w->setCurrentUI(DeviceCommonVaries::InfoWidgetFlag::DEVICE_WIDGET);
		w->setWindowTitle(configInfoTable->item(opDeleteBtn->property("row").toInt(), 2)->text() + QString("-设备配置"));

		w->show();

		});


}

/**
	@brief 删除一行
**/
void DeviceManageModule::removeOneRow(int removeRow) {
	int ret = QMessageBox::warning(DeviceInfoConfig::InfoConfigWidget::getInstance(), QString("警告"), QString("确认删除当前数据吗？"), "取消", "确定");
	if (ret == 0)
	{
		return;
	}
	int curDeviceID = configInfoTable->item(removeRow, 0)->text().toInt();
	//删除数据

	QString qSqlString = QString("DELETE FROM `simulatedtraining`.`device_info` WHERE `id` = %1").arg(curDeviceID);
	DeviceDBConfigInfo::getInstance()->customRunSql(qSqlString);
	configInfoTable->removeRow(removeRow);
}

/**
	@brief 编辑数据
	@param  -编辑后的数据
**/
void DeviceManageModule::editOneRow(int paramID, int rocketType, QString deviceName, int virtualDev) {
	//这里需要更新数据库
	DeviceDBConfigInfo::getInstance()->updateDeviceInfo2DB(paramID, rocketType, deviceName, virtualDev);
}

/**
	@brief 初始化显示数据
**/
void DeviceManageModule::InitDisplayData() {

	configInfoTable->clearContents();
	configInfoTable->setRowCount(0);
	DeviceDBConfigInfo* deviceInfoDB = DeviceDBConfigInfo::getInstance();
	deviceInfoDB->readDeviceDB2UI();
	deviceInfoDB->readRocketDB2UI();
	deviceInfoDB->readStatusInfoDB2UI();
	//configInfoTable->setRowCount(deviceInfoDB->deviceInfo.size());
	int row = 0;
	for (auto ele = deviceInfoDB->deviceInfo.begin(); ele != deviceInfoDB->deviceInfo.end(); ele++)
	{

		QVector<QString> rowData;
		rowData.push_back(QString::fromStdString(ele->second[0]));
		QString tmpp1, tmpp2;

		if (DeviceDBConfigInfo::getInstance()->rocketInfo[atoi(ele->second[1].c_str())].size() < 3)
		{
			tmpp1 = QString::fromStdString(ele->second[1]);

		}
		else
		{
			tmpp1 = QString::fromStdString(deviceInfoDB->rocketInfo[atoi(ele->second[1].c_str())][1]);

		}
		rowData.push_back(tmpp1);
		tmpp2 = QString::fromLocal8Bit(DeviceCommonVaries::getInstance()->deviceIndex2Type[atoi(ele->second[3].c_str())].c_str());
		//rowData.push_back(QString::fromStdString(ele->second[1]));
		rowData.push_back(QString::fromStdString(ele->second[2]));
		rowData.push_back(tmpp2);

		if (tmpp1 == QString::fromLocal8Bit(AppCache::instance()->m_CurrentRocketType->m_name.c_str()))
		{
			insertOneRow(row++, rowData);

		}
	}
}

/**
	@brief
	@param event -
**/
void DeviceManageModule::paintEvent(QPaintEvent* event) {

	for (int columnIndex = 0; columnIndex < configInfoTable->columnCount(); columnIndex++)
	{
		configInfoTable->setColumnWidth(columnIndex, (configInfoTable->width() - 100) / (configInfoTable->columnCount()));
	}

	configInfoTable->horizontalHeader()->setStretchLastSection(true);
	configInfoTable->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
}

/**
	@brief 增加一行数据
**/
void DeviceManageModule::insertOneRowData() {

#ifdef NEW_UI
	AddRocketTypeWidget* addRocketTypeW = AddRocketTypeWidget::getInstance();
	addRocketTypeW->setInfoWidget(DeviceCommonVaries::DEVICE_WIDGET);
	addRocketTypeW->setWindowName(QString("新增设备"));
	addRocketTypeW->setDevInfo(0, "", "", "");
	addRocketTypeW->show();
#endif // NEW_UI

#ifdef OLD_UI
	DeviceInfoConfig::InfoConfigWidget* infoConfigWidget = DeviceInfoConfig::InfoConfigWidget::getInstance();
	infoConfigWidget->show();
#endif // OLD_UI

	////更新
	//connect(infoConfigWidget, &DeviceInfoConfig::InfoConfigWidget::updateDeviceInfo, this, [=]() {
	//	InitDisplayData();
	//	});
}

/**
	@brief 删除一行数据 需同步数据库 此处涉及改动较大，暂时搁置
**/
void DeviceManageModule::deleteOneRowData() {

	int curDeviceID = configInfoTable->item(selectedRowNum, 0)->text().toInt();
	//删除数据

	QString qSqlString = QString("DELETE FROM `simulatedtraining`.`device_info` WHERE `id` = %1").arg(curDeviceID);
	DeviceDBConfigInfo::getInstance()->customRunSql(qSqlString);
	if (selectedRowNum != -1)
	{
		configInfoTable->removeRow(selectedRowNum);

	}
}