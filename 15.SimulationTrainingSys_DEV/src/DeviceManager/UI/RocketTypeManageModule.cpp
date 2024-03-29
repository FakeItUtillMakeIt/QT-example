#include "RocketTypeManageModule.h"
#include "DeviceManager.h"

RocketTypeManageModule::RocketTypeManageModule(QWidget* parent)
	: QWidget(parent)
	, m_importComPramData(nullptr)
{
	m_message = new QMessageBox();
	selectedRowNum = -1;
	columnNameList << QString("火箭ID") << QString("火箭型号名称") << QString("火箭型号描述") << QString("操作");

	m_importComPramData = new ImportComPramData();
	m_importThread = new QThread;
	m_importComPramData->moveToThread(m_importThread);
	connect(this, &RocketTypeManageModule::AddPramComSignal, m_importComPramData, &ImportComPramData::AddPramComDatas);
	connect(m_importComPramData, &ImportComPramData::ImportResult, this, &RocketTypeManageModule::ImportResultDo);
	m_importThread->start();

	InitUILayout();
	InitDisplayData();

	configInfoTable->setColumnHidden(0, true);
	configInfoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

	//更改ui.comboBox时更新窗口内容
	connect(static_cast<DeviceManager*>(this->parent()->parent()->parent()->parent()->parent()->parent()->parent()), &DeviceManager::rocketTypeChanged, this, [=]() {
		qDebug() << AppCache::instance()->m_CurrentRocketType->m_name.c_str();
		InitDisplayData();
		});

	connect(RocketInfoConfig::InfoConfigWidget::getInstance(), &RocketInfoConfig::InfoConfigWidget::updateRocketInfo, this, [=]() {
		//要更新数据库
		InitDisplayData();

		});

	connect(AddRocketTypeWidget::getInstance(), &AddRocketTypeWidget::updateRocketInfos, this, [=]() {
		emit rocketInfoChanged();
		InitDisplayData();

		});
}

RocketTypeManageModule::~RocketTypeManageModule()
{}

/**
	@brief 设备管理界面布局
**/
void RocketTypeManageModule::InitUILayout() {
	//和底部状态栏颜色保持一致


	WidgetStyleSheet* wss = WidgetStyleSheet::getInstace();

	QGridLayout* mainUILay = new QGridLayout;

	QLabel* deviceParamLabel = new QLabel(QString("火箭型号查询:"));
	QComboBox* deviceCombox = new QComboBox();
	QLabel* paramNameLabel = new QLabel(QString("火箭型号:"));
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
	@brief 新增一行
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
	QPushButton* opEditBtn = new QPushButton(QString("编辑"));
	opEditBtn->setProperty("row", insertRow);
	QPushButton* opDeleteBtn = new QPushButton(QString("删除"));
	opDeleteBtn->setProperty("row", insertRow);
	QPushButton* opCfgDataBtn = new QPushButton(QString("配置"));
	opCfgDataBtn->setProperty("row", insertRow);
	QPushButton* opImpotDataBtn = new QPushButton(QString("导入"));
	opImpotDataBtn->setProperty("row", insertRow);

	//#ifdef NEW_UI
	//	opCfgDataBtn->show();
	//#endif // NEW_UI


#ifdef OLD_UI
	opCfgDataBtn->hide();
	//编辑时呼出窗口  infoConfigWidget窗口，将当前行信息传至窗口，编辑
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
	hbox->addWidget(opImpotDataBtn);
	hbox->addWidget(opDeleteBtn);


	w1->setLayout(hbox);
	w1->setStyleSheet("*{border:none;color:blue;min-height:20px;}");


	configInfoTable->setCellWidget(insertRow, columnNameList.size() - 1, w1);

	connect(opEditBtn, &QPushButton::clicked, this, [=]() {
		auto editW = AddRocketTypeWidget::getInstance();
		editW->setInfoWidget(DeviceCommonVaries::ROCKET_WIDGET);
		editW->setWindowName(QString("编辑火箭型号"));
		int curRow = opEditBtn->property("row").toInt();
		editW->setRocketInfo(configInfoTable->item(curRow, 0)->text().toInt(), configInfoTable->item(curRow, 1)->text(),
			configInfoTable->item(curRow, 2)->text());

		editW->setParent(this);
		editW->setWindowModality(Qt::ApplicationModal);
		editW->setWindowFlags(this->windowFlags() | Qt::Dialog | Qt::FramelessWindowHint);
		QRect rtWorkArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rtWorkArea, 0);
		editW->move((rtWorkArea.width() - editW->frameGeometry().width()) / 2, (rtWorkArea.height() - editW->frameGeometry().height()) / 2);
		editW->show();

		emit rocketInfoChanged();
		});

	connect(opImpotDataBtn, &QPushButton::clicked, this, [=]() {
		ImportData(opImpotDataBtn->property("row").toInt());
		});

	connect(opDeleteBtn, &QPushButton::clicked, this, [=]() {

		removeOneRow(opDeleteBtn->property("row").toInt());
		emit rocketInfoChanged();
		});

	connect(opCfgDataBtn, &QPushButton::clicked, this, [=]() {
		//获取当前火箭型号
		AllInfoConfigWidget* w = AllInfoConfigWidget::getInstance();
		w->setInfoWidgetCfg(configInfoTable->item(opCfgDataBtn->property("row").toInt(), 0)->text().toInt(), -1, -1);
		w->setCurrentUI(DeviceCommonVaries::InfoWidgetFlag::ROCKET_WIDGET);
		w->setWindowTitle(configInfoTable->item(opCfgDataBtn->property("row").toInt(), 1)->text() + QString("-箭上数据通信协议配置"));


		w->setParent(this);
		w->setWindowModality(Qt::ApplicationModal);
		w->setWindowFlags(this->windowFlags() | Qt::Dialog | Qt::FramelessWindowHint);

		QRect rtWorkArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rtWorkArea, 0);
		w->move((rtWorkArea.width() - w->frameGeometry().width()) / 2, (rtWorkArea.height() - w->frameGeometry().height()) / 2);
		w->show();
		});

}


/// <summary>
/// 导入参数和指令
/// </summary>
/// <param name="rowNumber"></param>
void RocketTypeManageModule::ImportData(int rowNumber)
{
	qDebug() << "RocketTypeManageModule：" << QThread::currentThreadId() << QThread::currentThread();

	int curRocketID;
	QString readFile;
	//读取excel文件路径
	readFile = QFileDialog::getOpenFileName(nullptr, "选择Excel文件", "", tr("Exel file(*.xls *.xlsx)"));
	if (readFile == "")
	{
		return;
	}
	curRocketID = configInfoTable->item(rowNumber, 0)->text().toInt();

	emit AddPramComSignal(curRocketID, readFile);
	m_message->setParent(nullptr);
	m_message->setText("数据导入中请稍等...");
	m_message->setWindowTitle("提示");
	m_message->setStandardButtons(QMessageBox::No);
	m_message->button(QMessageBox::No)->setHidden(true);
	m_message->show();

}

/// <summary>
/// 导入结果展示
/// </summary>
/// <param name="index"></param>
void RocketTypeManageModule::ImportResultDo(QString Qstr)
{
	if (m_message != nullptr)
	{
		m_message->close();
	}
	QMessageBox::information(nullptr, "提示", Qstr, "确定");
}

/**
	@brief 删除一行
**/
void RocketTypeManageModule::removeOneRow(int removeRow) {
	/*QMessageBox msgBoxWarning;
	msgBoxWarning.setText(QString("确认删除当前数据吗?"));
	msgBoxWarning.setWindowTitle(QString("警告"));
	msgBoxWarning.setWindowIcon(QIcon(":/DeviceManager/images/TeachingManagement.ico"));
	msgBoxWarning.setIcon(QMessageBox::Warning);
	msgBoxWarning.addButton(QString("取消"), QMessageBox::RejectRole);
	msgBoxWarning.addButton(QString("确定"), QMessageBox::AcceptRole);
	int ret = msgBoxWarning.exec();*/
	int ret = AllInfoConfigWidget::getInstance()->delExecResult();
	if (ret == 0)
	{
		return;
	}
	int curRocketID = configInfoTable->item(removeRow, 0)->text().toInt();

	configInfoTable->removeRow(removeRow);

#ifdef __DELETE_RELE_TABLE__
	//型号信息
	DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.rocket_info WHERE id=%1").arg(curRocketID));
	QThread* deleteThread = new QThread;
	DeleteDBDataThread* thread11 = new DeleteDBDataThread();
	thread11->setRocketID(curRocketID);
	thread11->moveToThread(deleteThread);
	connect(deleteThread, &QThread::started, thread11, &DeleteDBDataThread::deleteRocketInfo);
	connect(thread11, &DeleteDBDataThread::workFinished, this, [=]() {
		//QMessageBox::information(this, "info", "success");
		QMessageBox msg;
		msg.setText("null");
		msg.show();
		msg.close();

		});
	deleteThread->start();

#endif __DELETE_RELE_TABLE__
#ifdef __DELETE_ONLY__
	QString qSqlString = QString("DELETE FROM `simulatedtraining`.`rocket_info` WHERE `id` = %1").arg(curRocketID);
	DeviceDBConfigInfo::getInstance()->customRunSql(qSqlString);
#endif __DELETE_ONLY__

}

/**
	@brief 编辑数据
	@param  -编辑后的数据
**/
void RocketTypeManageModule::editOneRow(int rocketID, QString rocketName, QString  rocketCode) {
	//这里需要更新数据库
	DeviceDBConfigInfo::getInstance()->updateRocketInfo2DB(rocketID, rocketName, rocketCode);
}

/**
	@brief 初始化显示数据  初始化显示时  仅显示和当前选择火箭相关的条目
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
	@brief 增加一行数据
**/
void RocketTypeManageModule::insertOneRowData() {

#ifdef NEW_UI

	AddRocketTypeWidget* addRocketTypeW = AddRocketTypeWidget::getInstance();
	addRocketTypeW->setInfoWidget(DeviceCommonVaries::ROCKET_WIDGET);
	addRocketTypeW->setWindowName(QString("新增火箭型号"));
	addRocketTypeW->setRocketInfo(0, "", "");
	addRocketTypeW->show();
#endif // !1

#ifdef OLD_UI

	RocketInfoConfig::InfoConfigWidget* infoConfigWidget = RocketInfoConfig::InfoConfigWidget::getInstance();
	infoConfigWidget->show();
#endif // DEBUG
	//connect(infoConfigWidget, &RocketInfoConfig::InfoConfigWidget::updateRocketInfo, this, [=]() {
	//	//要更新数据库
	//	InitDisplayData();

	//	});
}

/**
	@brief 删除一行数据  需同步数据库  此处涉及改动较大，暂时搁置
**/
void RocketTypeManageModule::deleteOneRowData() {
	if (selectedRowNum != -1)
	{
		configInfoTable->removeRow(selectedRowNum);

	}
}



void RocketTypeManageModule::deleteReleDataFromDB(int curRocketID) {
	//删除和型号相关数据
	QString qSqlString;

	//参数信息
	qSqlString = QString("SELECT\
		parameter_rocket_info.parameter_id,\
		parameter_rocket_info.createTime,\
		parameter_rocket_info.lastUpdateTime\
		FROM\
		parameter_rocket_info\
		WHERE\
		parameter_rocket_info.rocket_id = %1").arg(curRocketID);
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
	auto releParamInfo = DeviceDBConfigInfo::getInstance()->customReadInfoMap;
	for (auto ele : releParamInfo)
	{
		int curParamID = ele.first;
		QString qSqlString = QString("SELECT\
		device_param_info.id,\
		device_param_info.createTime,\
		device_param_info.lastUpdateTime\
		FROM\
		device_param_info\
		WHERE\
		device_param_info.parameter_id = %1").arg(curParamID);
		DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
		auto releDataInfo = DeviceDBConfigInfo::getInstance()->customReadInfoMap;
		for (auto ele1 : releDataInfo)
		{
			DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.rockect_param_info WHERE device_parameter_id=%1").arg(ele1.first));
		}
		//设备参数
		DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.device_param_info WHERE parameter_id=%1").arg(curParamID));
		//参数型号
		DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.parameter_rocket_info WHERE parameter_id=%1").arg(curParamID));
		//参数信息
		DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.parameter_info WHERE id=%1").arg(curParamID));
	}
	//参数型号信息
	DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.param_table_info WHERE rocket_id=%1").arg(curRocketID));

	//参数型号信息
	//递归删除和设备相关的所有信息
	qSqlString = QString("SELECT\
		device_info.id,\
		device_info.createTime,\
		device_info.lastUpdateTime\
		FROM\
		device_info\
		WHERE\
		device_info.rocket_id = %1").arg(curRocketID);
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
	auto releDeviceInfo = DeviceDBConfigInfo::getInstance()->customReadInfoMap;
	for (auto ele : releDeviceInfo)
	{
		int curDeviceID = ele.first;
		//设备关联的设备状态信息
		qSqlString = QString("SELECT\
		device_status_info.id,\
		device_status_info.createTime,\
		device_status_info.lastUpdateTime\
		FROM\
		device_status_info\
		WHERE\
		device_status_info.device_id = %1").arg(curDeviceID);
		DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
		auto releDataInfo = DeviceDBConfigInfo::getInstance()->customReadInfoMap;
		for (auto ele1 : releDataInfo)
		{
			DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.command_devicestatus_info WHERE deviceStatus_id=%1").arg(ele1.first));
		}
		//箭上参数
		qSqlString = QString("SELECT\
		device_param_info.id,\
		device_param_info.createTime,\
		device_param_info.lastUpdateTime\
		FROM\
		device_param_info\
		WHERE\
		device_param_info.device_id = %1").arg(curDeviceID);
		DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
		releDataInfo = DeviceDBConfigInfo::getInstance()->customReadInfoMap;
		for (auto ele2 : releDataInfo)
		{
			DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.rockect_param_info WHERE device_parameter_id=%1").arg(ele2.first));
		}
		//设备参数
		DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.device_param_info WHERE device_id=%1").arg(curDeviceID));
		//设备状态
		DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.device_status_info WHERE device_id=%1").arg(curDeviceID));
	}
	//设备信息
	DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.device_info WHERE rocket_id=%1").arg(curRocketID));

	//递归删除和指令相关的所有信息
	qSqlString = QString("SELECT\
		command_info.id,\
		command_info.createTime,\
		command_info.lastUpdateTime\
		FROM\
		command_info\
		WHERE\
		command_info.rocket_id = %1").arg(curRocketID);
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
	auto releCmdInfo = DeviceDBConfigInfo::getInstance()->customReadInfoMap;
	for (auto ele : releCmdInfo)
	{
		int curCmdID = ele.first;
		//子流程信息
		DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.sub_flow_info WHERE command_id=%1").arg(curCmdID));
		//指令参数
		DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.command_param_info WHERE command_id=%1").arg(curCmdID));
		//指令设备状态
		DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.command_devicestatus_info WHERE command_id=%1").arg(curCmdID));
		//指令表信息
		DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.command_commandtable_info WHERE command_id=%1").arg(curCmdID));
		//指令信息
		DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.command_info WHERE id=%1").arg(curCmdID));
	}
	//指令信息
	DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.command_table_info WHERE rocket_id=%1").arg(curRocketID));
	//箭上参数
	qSqlString = QString("SELECT\
		rocket_data_info.id,\
		rocket_data_info.createTime,\
		rocket_data_info.lastUpdateTime\
		FROM\
		rocket_data_info\
		WHERE\
		rocket_data_info.rocket_id = %1").arg(curRocketID);
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
	auto releRocketDataInfo = DeviceDBConfigInfo::getInstance()->customReadInfoMap;
	for (auto ele : releRocketDataInfo)
	{
		DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.rockect_param_info WHERE rocket_data_id=%1").arg(ele.first));
	}
	//箭上数据
	DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.rocket_data_info WHERE rocket_id=%1").arg(curRocketID));
	//子流程信息
	qSqlString = QString("SELECT\
		main_flow_info.id,\
		main_flow_info.createTime,\
		main_flow_info.lastUpdateTime\
		FROM\
		main_flow_info\
		WHERE\
		main_flow_info.rocket_id = %1").arg(curRocketID);
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
	auto releMainFlowInfo = DeviceDBConfigInfo::getInstance()->customReadInfoMap;
	for (auto ele : releMainFlowInfo)
	{
		DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.sub_flow_info WHERE main_id=%1").arg(ele.first));
	}
	//主流程信息
	DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.main_flow_info WHERE rocket_id=%1").arg(curRocketID));
	//故障指令信息
	DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.fault_command_info WHERE rocket_id=%1").arg(curRocketID));
	// 故障参数信息
	DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.fault_param_info WHERE rocket_id=%1").arg(curRocketID));
	//型号岗位信息
	DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.rocket_task_info WHERE rocket_id=%1").arg(curRocketID));
	//型号信息
	DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.rocket_info WHERE id=%1").arg(curRocketID));

	emit rocketInfoChanged();
}


/**
	@brief	   火箭数据配置窗口
	@retval  -
**/
RocketDataCfgW::RocketDataCfgW() {



	scrollArea = new QTableWidget;

	QVBoxLayout* vlayout = new QVBoxLayout;
	QHBoxLayout* hlayout = new QHBoxLayout;
	QHBoxLayout* bottomHlayout = new QHBoxLayout;

	scrollArea->setColumnCount(3);

	scrollArea->setHorizontalHeaderItem(0, new QTableWidgetItem(QString("指令名称")));
	scrollArea->setHorizontalHeaderItem(1, new QTableWidgetItem(QString("指令编码")));
	scrollArea->setHorizontalHeaderItem(2, new QTableWidgetItem(QString("指令前缀")));

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

	QPushButton* rOKBtn = new QPushButton(QString("添加"));
	QPushButton* rCancelBtn = new QPushButton(QString("取消"));

	scrollArea->setFixedSize(480, 500);
	vlayout->addWidget(scrollArea);

	bottomHlayout->addStretch(4);
	bottomHlayout->addWidget(rCancelBtn);
	bottomHlayout->addWidget(rOKBtn);
	vlayout->addLayout(bottomHlayout);

	this->setLayout(vlayout);
	this->setFixedSize(500, 580);
	this->setWindowTitle(QString("火箭数据信息配置"));
	this->setWindowFlags(Qt::WindowStaysOnTopHint);


}

/**
	@brief 获取table中选中的所有指令信息
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
	@brief  火箭参数配置
	@retval  -
**/
RocketParamCfgW1::RocketParamCfgW1() {

	paramIndex = new QLabel(QString("参数索引:"));
	paramLength = new QLabel(QString("参数字节长度:"));
	paramType = new QLabel(QString("参数类型:"));
	userInputParamLength = new QLineEdit;
	userInputParamIndex = new QLineEdit;
	userSlectParamType = new QComboBox;

	cancelCfgBtn = new QPushButton(QString("取消"));
	okCfgBtn = new QPushButton(QString("确定"));

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
	headerLabels << QString("箭上返回指令") << QString("参数名称");
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
	@brief 响应参数点击信号
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
