#include "ParamManageModule.h"
#include "DeviceManager.h"

ParamManageModule::ParamManageModule(QWidget* parent)
	: QWidget(parent)
{

	selectedRowNum = 0;
	columnNameList << QString("参数ID") << QString("参数名称") << QString("参数类型") << QString("参数单位") << QString("操作");

	InitUILayout();
	InitDisplayData();

	configInfoTable->setColumnHidden(0, true);

	connect(static_cast<DeviceManager*>(this->parent()->parent()->parent()->parent()), &DeviceManager::rocketTypeChanged, this, [=]() {
		qDebug() << AppCache::instance()->m_CurrentRocketType->m_name.c_str();
		InitDisplayData();
		});
}

ParamManageModule::~ParamManageModule()
{

}

/**
	@brief 初始化数据库连接
**/
void ParamManageModule::InitDisplayData() {
	configInfoTable->clearContents();
	configInfoTable->setRowCount(0);

	DeviceDBConfigInfo* paramInfoDB = DeviceDBConfigInfo::getInstance();
	paramInfoDB->readParamDB2UI();

	//configInfoTable->setRowCount(paramInfoDB->paramInfo.size());
	int row = 0;
	for (auto ele = paramInfoDB->paramInfo.begin(); ele != paramInfoDB->paramInfo.end(); ele++)
	{


		QVector<QString> rowData;
		rowData.push_back(QString::fromStdString(ele->second[0]));
		rowData.push_back(QString::fromStdString(ele->second[1]));
		rowData.push_back(QString::fromLocal8Bit(DeviceCommonVaries::getInstance()->paramIndex2Type[atoi(ele->second[2].c_str())].c_str()));
		rowData.push_back(QString::fromStdString(ele->second[3]));
		insertOneRow(row++, rowData);

	}

}

/**
	@brief 参数管理界面布局
	UI可以显示数据库中已有的参数
**/
void ParamManageModule::InitUILayout() {
	//和底部状态栏颜色保持一致
	WidgetStyleSheet* wss = WidgetStyleSheet::getInstace();

	QGridLayout* mainUILay = new QGridLayout;

	QLabel* deviceParamLabel = new QLabel(QString("设备参数查询:"));
	QComboBox* deviceCombox = new QComboBox();
	QLabel* paramNameLabel = new QLabel(QString("参数名称:"));
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
	mainUILay->setColumnStretch(0, 1);
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

	connect(insertButton, &QPushButton::clicked, this, &ParamManageModule::insertOneRowData);
	connect(configInfoTable, &QTableWidget::itemClicked, this, [=]() {
		selectedRowNum = configInfoTable->currentRow();
		});
	connect(deleteButton, &QPushButton::clicked, this, &ParamManageModule::deleteOneRowData);

	connect(paramInputName, &QLineEdit::textEdited, this, [=](QString inputText) {
		if (inputText.isEmpty())
		{
			configInfoTable->clearContents();
			InitDisplayData();
			return;
		}

		configInfoTable->clearContents();
		configInfoTable->setRowCount(0);
		DeviceDBConfigInfo::getInstance()->readParamDB2UI();
		int searchRow = 0;
		for (auto ele = DeviceDBConfigInfo::getInstance()->paramInfo.begin(); ele != DeviceDBConfigInfo::getInstance()->paramInfo.end(); ele++)
		{
			if (QString::fromStdString(ele->second[1]).contains(paramInputName->text()))
			{
				QVector<QString> rowData;
				rowData.push_back(QString::fromStdString(ele->second[0]));
				rowData.push_back(QString::fromStdString(ele->second[1]));
				rowData.push_back(QString::fromLocal8Bit(DeviceCommonVaries::getInstance()->paramIndex2Type[atoi(ele->second[2].c_str())].c_str()));
				rowData.push_back(QString::fromStdString(ele->second[3]));
				insertOneRow(searchRow++, rowData);
			}
		}
		});
}


/**
	@brief 新增一行
	@param  -
**/
void ParamManageModule::insertOneRow(int insertRow, QVector<QString> rowData) {
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
	QPushButton* opCfgSwitchBtn = new QPushButton(QString("配置开关量"));
	opCfgSwitchBtn->setProperty("row", insertRow);

	opCfgSwitchBtn->hide();

	hbox->addWidget(opEditBtn);
	hbox->addWidget(opDeleteBtn);
	//hbox->addWidget(opCfgSwitchBtn);

	/*hbox->addWidget(new QPushButton(QString("编辑")));
	hbox->addWidget(new QPushButton(QString("删除")));*/
	w1->setLayout(hbox);
	w1->setStyleSheet("*{border:none;color:blue;}");


	configInfoTable->setCellWidget(insertRow, columnNameList.size() - 1, w1);

	connect(opEditBtn, &QPushButton::clicked, this, [=]() {

		int curRow = opEditBtn->property("row").toInt();
		qDebug() << curRow;

		ParamInfoConfig::InfoConfigWidget::getInstance()->currentDeviceFlag = DeviceCommonVaries::getInstance()->DeviceModule::UPDATE_MODULE;
		ParamInfoConfig::InfoConfigWidget::getInstance()->editId = configInfoTable->item(curRow, 0)->text().toInt();
		ParamInfoConfig::InfoConfigWidget::getInstance()->userInputParamName->setText(configInfoTable->item(curRow, 1)->text());
		ParamInfoConfig::InfoConfigWidget::getInstance()->userSelectType->setCurrentText(configInfoTable->item(curRow, 2)->text());
		ParamInfoConfig::InfoConfigWidget::getInstance()->userSelcetUnit->setCurrentText(configInfoTable->item(curRow, 3)->text());

		ParamInfoConfig::InfoConfigWidget::getInstance()->show();
		//editOneRow(configInfoTable->item(curRow, 0)->text().toInt(), configInfoTable->item(curRow, 1)->text(), configInfoTable->item(curRow, 2)->text().toInt(), configInfoTable->item(curRow, 3)->text());

		});
	connect(opDeleteBtn, &QPushButton::clicked, this, [=]() {
		removeOneRow(opDeleteBtn->property("row").toInt());
		});
}

/**
	@brief 删除一行
**/
void ParamManageModule::removeOneRow(int removeRow) {
	int ret = QMessageBox::warning(this, QString("警告"), QString("确认删除当前数据吗？"), "取消", "确定");
	if (ret == 0)
	{
		return;
	}
	int curDeviceID = configInfoTable->item(removeRow, 0)->text().toInt();
	//删除数据

	QString qSqlString = QString("DELETE FROM `simulatedtraining`.`parameter_info` WHERE `id` = %1").arg(curDeviceID);
	DeviceDBConfigInfo::getInstance()->customRunSql(qSqlString);
	configInfoTable->removeRow(removeRow);
}

/**
	@brief 编辑数据
	@param  -编辑后的数据
**/
void ParamManageModule::editOneRow(int paramID, QString paramName, int  paramType, QString paramUnit) {
	//这里需要更新数据库
	DeviceDBConfigInfo::getInstance()->updateParamInfo2DB(paramID, paramName, paramType, paramUnit);
}

void ParamManageModule::paintEvent(QPaintEvent* event) {

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
void ParamManageModule::insertOneRowData() {

	ParamInfoConfig::InfoConfigWidget* infoConfigWidget = ParamInfoConfig::InfoConfigWidget::getInstance();
	infoConfigWidget->show();

	connect(infoConfigWidget, &ParamInfoConfig::InfoConfigWidget::updateParams, this, [=]() {
		//要更新数据库
		InitDisplayData();

		});

	return;

}

/**
	@brief 删除一行数据  需同步数据库  此处涉及改动较大，暂时搁置
**/
void ParamManageModule::deleteOneRowData() {
	if (selectedRowNum != -1)
	{
		configInfoTable->removeRow(selectedRowNum);

	}
}