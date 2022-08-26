#include "CommandManageModule.h"
#include "DeviceManager.h"

CommandManageModule::CommandManageModule(QWidget* parent)
	: QWidget(parent)
{
	selectedRowNum = -1;
	columnNameList << QString("指令ID") << QString("火箭型号") << QString("测发回令") << QString("指令名称") << QString("指令编码") << QString("指令类型") << QString("指令前缀") << QString("操作");

	InitUILayout();
	InitDisplayData();

	configInfoTable->setColumnHidden(0, true);

	connect(static_cast<DeviceManager*>(this->parent()->parent()->parent()->parent()), &DeviceManager::rocketTypeChanged, this, [=]() {
		qDebug() << AppCache::instance()->m_CurrentRocketType->m_name.c_str();
		InitDisplayData();
		});

	connect(CommandInfoConfig::InfoConfigWidget::getInstance(), &CommandInfoConfig::InfoConfigWidget::updateCommandInfos, this, [=]() {

		InitDisplayData();

		});
	connect(AddRocketTypeWidget::getInstance(), &AddRocketTypeWidget::updateCommandInfos, this, [=]() {
		InitDisplayData();

		});

}

CommandManageModule::~CommandManageModule()
{}


/**
	@brief 指令管理界面布局
**/
void CommandManageModule::InitUILayout() {
	//和底部状态栏颜色保持一致
	WidgetStyleSheet* wss = WidgetStyleSheet::getInstace();

	QGridLayout* mainUILay = new QGridLayout;

	QLabel* deviceParamLabel = new QLabel(QString("指令名称查询:"));
	QComboBox* deviceCombox = new QComboBox();
	QLabel* paramNameLabel = new QLabel(QString("指令名称:"));
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


	connect(insertButton, &QPushButton::clicked, this, &CommandManageModule::insertOneRowData);
	connect(configInfoTable, &QTableWidget::itemClicked, this, [=]() {
		selectedRowNum = configInfoTable->currentRow();
		});
	connect(deleteButton, &QPushButton::clicked, this, &CommandManageModule::deleteOneRowData);

	connect(paramInputName, &QLineEdit::textEdited, this, [=](QString inputText) {
		if (inputText.isEmpty())
		{
			configInfoTable->clearContents();
			InitDisplayData();
			return;
		}

		configInfoTable->clearContents();
		configInfoTable->setRowCount(0);
		DeviceDBConfigInfo::getInstance()->readCommandDB2UI();
		DeviceDBConfigInfo::getInstance()->readRocketDB2UI();
		int searchRow = 0;
		for (auto ele = DeviceDBConfigInfo::getInstance()->commandInfo.begin(); ele != DeviceDBConfigInfo::getInstance()->commandInfo.end(); ele++)
		{
			if (QString::fromStdString(ele->second[3]).contains(paramInputName->text()))
			{
				QVector<QString> rowData;
				rowData.push_back(QString::fromStdString(ele->second[0]));
				//rowData.push_back(QString::fromStdString(ele->second[1]));
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
				//rowData.push_back(QString::fromStdString(ele->second[1]));
				//rowData.push_back(QString::fromStdString(ele->second[2]));
				if (atoi(ele->second[2].c_str()) == 0)
				{
					tmpp2 = QString("无回令");
				}
				else
				{
					tmpp2 = QString::fromStdString(DeviceDBConfigInfo::getInstance()->commandInfo[atoi(ele->second[2].c_str())][3]);
				}
				//rowData.push_back(QString::fromStdString(DeviceDBConfigInfo::getInstance()->commandInfo[atoi(ele->second[2].c_str())][3]));
				rowData.push_back(tmpp2);
				rowData.push_back(QString::fromStdString(ele->second[3]));
				rowData.push_back(QString::fromStdString(ele->second[4]));
				//rowData.push_back(QString::fromStdString(ele->second[5]));
				rowData.push_back(QString::fromLocal8Bit(DeviceCommonVaries::getInstance()->commandIndex2Type[atoi(ele->second[5].c_str())].c_str()));
				rowData.push_back(QString::fromStdString(ele->second[6]));
				insertOneRow(searchRow++, rowData);
			}
		}
		});
}



/**
	@brief 新增一行
	@param  -
**/
void CommandManageModule::insertOneRow(int insertRow, QVector<QString> rowData) {
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
	QPushButton* opCfgCmdBtn = new QPushButton(QString("配置"));
	opCfgCmdBtn->setProperty("row", insertRow);

	//#ifdef NEW_UI
	//	opCfgCmdBtn->show();
	//#endif // NEW_UI


#ifdef OLD_UI
	opCfgCmdBtn->hide();
	connect(opEditBtn, &QPushButton::clicked, this, [=]() {

		int curRow = opEditBtn->property("row").toInt();
		qDebug() << curRow;
		CommandInfoConfig::InfoConfigWidget::getInstance()->currentDeviceFlag = DeviceCommonVaries::getInstance()->DeviceModule::UPDATE_MODULE;
		CommandInfoConfig::InfoConfigWidget::getInstance()->editId = configInfoTable->item(curRow, 0)->text().toInt();
		CommandInfoConfig::InfoConfigWidget::getInstance()->userSelectRocketType->setCurrentText(configInfoTable->item(curRow, 1)->text());
		CommandInfoConfig::InfoConfigWidget::getInstance()->userSelectBackCMD->setCurrentText(configInfoTable->item(curRow, 2)->text());
		CommandInfoConfig::InfoConfigWidget::getInstance()->userInputCMDName->setText(configInfoTable->item(curRow, 3)->text());
		CommandInfoConfig::InfoConfigWidget::getInstance()->userSelectCMDCode->setCurrentText(configInfoTable->item(curRow, 4)->text());
		CommandInfoConfig::InfoConfigWidget::getInstance()->userSelectCMDType->setCurrentText(configInfoTable->item(curRow, 5)->text());
		CommandInfoConfig::InfoConfigWidget::getInstance()->userSelectCMDPrefix->setCurrentText(configInfoTable->item(curRow, 6)->text());
		CommandInfoConfig::InfoConfigWidget::getInstance()->show();
		});
#endif // OLD_UI

	hbox->addWidget(opEditBtn);
	hbox->addWidget(opCfgCmdBtn);
	hbox->addWidget(opDeleteBtn);

	w1->setLayout(hbox);
	w1->setStyleSheet("*{border:none;color:blue;}");


	configInfoTable->setCellWidget(insertRow, columnNameList.size() - 1, w1);

	connect(opEditBtn, &QPushButton::clicked, this, [=]() {
		auto editW = AddRocketTypeWidget::getInstance();
		editW->setInfoWidget(DeviceCommonVaries::COMMAND_WIDGET);
		editW->setWindowName(QString("编辑指令"));
		int curRow = opEditBtn->property("row").toInt();
		editW->setCommandInfo(configInfoTable->item(curRow, 0)->text().toInt(), configInfoTable->item(curRow, 1)->text(),
			configInfoTable->item(curRow, 5)->text(), configInfoTable->item(curRow, 3)->text(),
			configInfoTable->item(curRow, 2)->text());
		editW->show();
		});

	connect(opDeleteBtn, &QPushButton::clicked, this, [=]() {
		removeOneRow(opDeleteBtn->property("row").toInt());
		});

	connect(opCfgCmdBtn, &QPushButton::clicked, this, [=]() {
		//获取当前火箭型号
		AllInfoConfigWidget* w = AllInfoConfigWidget::getInstance();
		w->setInfoWidgetCfg(AppCache::instance()->m_CurrentRocketType->m_id, -1, configInfoTable->item(opCfgCmdBtn->property("row").toInt(), 0)->text().toInt());
		w->setCurrentUI(DeviceCommonVaries::InfoWidgetFlag::COMMAND_WIDGET);
		w->setWindowTitle(configInfoTable->item(opCfgCmdBtn->property("row").toInt(), 3)->text() + QString("-指令配置"));

		w->show();
		});

}


/**
	@brief 删除一行
**/
void CommandManageModule::removeOneRow(int removeRow) {


	int ret = QMessageBox::warning(CommandInfoConfig::InfoConfigWidget::getInstance(), QString("警告"), QString("确认删除当前数据吗？"), "取消", "确定");
	if (ret == 0)
	{
		return;
	}
	int curDeviceID = configInfoTable->item(removeRow, 0)->text().toInt();
	//删除数据

	QString qSqlString = QString("DELETE FROM `simulatedtraining`.`command_info` WHERE `id` = %1").arg(curDeviceID);
	DeviceDBConfigInfo::getInstance()->customRunSql(qSqlString);
	configInfoTable->removeRow(removeRow);
}

/**
	@brief 编辑数据
	@param  -编辑后的数据
**/
void CommandManageModule::editOneRow(int commandID, int rocketID, int backID, QString cmdName, int cmdCode, int cmdType, int cmdPrefix) {
	//这里需要更新数据库
	DeviceDBConfigInfo::getInstance()->updateCommandInfo2DB(commandID, cmdName, rocketID, backID, cmdCode, cmdType, cmdPrefix);
}


/**
	@brief 初始化显示数据
**/
void CommandManageModule::InitDisplayData() {

	configInfoTable->clearContents();
	configInfoTable->setRowCount(0);
	DeviceDBConfigInfo* commandInfoDB = DeviceDBConfigInfo::getInstance();
	commandInfoDB->readCommandDB2UI();
	//configInfoTable->setRowCount(commandInfoDB->commandInfo.size());
	int row = 0;
	for (auto ele = commandInfoDB->commandInfo.begin(); ele != commandInfoDB->commandInfo.end(); ele++)
	{

		QVector<QString> rowData;
		rowData.push_back(QString::fromStdString(ele->second[0]));
		//rowData.push_back(QString::fromStdString(ele->second[1]));
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
		//rowData.push_back(QString::fromStdString(ele->second[1]));
		//rowData.push_back(QString::fromStdString(ele->second[2]));
		if (atoi(ele->second[2].c_str()) == 0)
		{
			tmpp2 = QString("无回令");
		}
		else
		{
			tmpp2 = QString::fromStdString(DeviceDBConfigInfo::getInstance()->commandInfo[atoi(ele->second[2].c_str())][3]);
		}
		//rowData.push_back(QString::fromStdString(DeviceDBConfigInfo::getInstance()->commandInfo[atoi(ele->second[2].c_str())][3]));
		rowData.push_back(tmpp2);
		rowData.push_back(QString::fromStdString(ele->second[3]));
		rowData.push_back(QString::fromStdString(ele->second[4]));
		//rowData.push_back(QString::fromStdString(ele->second[5]));
		rowData.push_back(QString::fromLocal8Bit(DeviceCommonVaries::getInstance()->commandIndex2Type[atoi(ele->second[5].c_str())].c_str()));
		rowData.push_back(QString::fromStdString(ele->second[6]));

		if (tmpp1 == QString::fromLocal8Bit(AppCache::instance()->m_CurrentRocketType->m_name.c_str()))
		{
			insertOneRow(row++, rowData);
		}
	}
}



void CommandManageModule::paintEvent(QPaintEvent* event) {

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
void CommandManageModule::insertOneRowData() {


#ifdef NEW_UI
	AddRocketTypeWidget* addRocketTypeW = AddRocketTypeWidget::getInstance();
	addRocketTypeW->setInfoWidget(DeviceCommonVaries::COMMAND_WIDGET);
	addRocketTypeW->setWindowName(QString("新增指令"));
	addRocketTypeW->setCommandInfo(0, "", "", "", "");
	addRocketTypeW->show();
#endif // NEW_UI

#ifdef OLD_UI
	CommandInfoConfig::InfoConfigWidget* infoConfigWidget = CommandInfoConfig::InfoConfigWidget::getInstance();
	infoConfigWidget->show();

#endif // OLD_UI


	//connect(infoConfigWidget, &CommandInfoConfig::InfoConfigWidget::updateCommandInfos, this, [=]() {
	//	//要更新数据库
	//	InitDisplayData();

	//	});
}

/**
	@brief 删除一行数据  需同步数据库  此处涉及改动较大，暂时搁置
**/
void CommandManageModule::deleteOneRowData() {
	if (selectedRowNum != -1)
	{
		configInfoTable->removeRow(selectedRowNum);

	}
}