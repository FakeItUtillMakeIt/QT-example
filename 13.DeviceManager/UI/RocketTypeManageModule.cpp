#include "RocketTypeManageModule.h"


RocketTypeManageModule::RocketTypeManageModule(QWidget* parent)
	: QWidget(parent)
{
	selectedRowNum = -1;
	columnNameList << QString("火箭ID") << QString("火箭型号名称") << QString("火箭型号类型");


	InitUILayout();

	InitDisplayData();
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

	//configInfoTable->setVerticalHeaderLabels(QStringList("111"));
	configInfoTable->setVerticalHeaderItem(0, new QTableWidgetItem("序号"));
	for (int col = 0; col < configInfoTable->columnCount(); col++)
	{

		QTableWidgetItem* item = new QTableWidgetItem();
		item->setText(columnNameList[col]);
		configInfoTable->setItem(0, col, new QTableWidgetItem);
		configInfoTable->setHorizontalHeaderItem(col, item);
	}

	configInfoTable->horizontalHeader()->setStretchLastSection(true);

	this->setLayout(mainUILay);

	connect(insertButton, &QPushButton::clicked, this, &RocketTypeManageModule::insertOneRowData);
	connect(configInfoTable, &QTableWidget::itemClicked, this, [=]() {
		selectedRowNum = configInfoTable->currentRow();
		});
	connect(deleteButton, &QPushButton::clicked, this, &RocketTypeManageModule::deleteOneRowData);

}


/**
	@brief 初始化显示数据
**/
void RocketTypeManageModule::InitDisplayData() {


	DeviceDBConfigInfo* rocketInfoDB = DeviceDBConfigInfo::getInstance();
	rocketInfoDB->readRocketDB2UI();
	configInfoTable->setRowCount(rocketInfoDB->rocketInfo.size());
	int row = 0;
	for (auto ele = rocketInfoDB->rocketInfo.begin(); ele != rocketInfoDB->rocketInfo.end(); ele++)
	{
		//名称类型单位
		for (int col = 0; col < ele->second.size(); col++)
		{
			QString itemtext;
			//类型
			if (col == 5)
			{
				auto index = QString::fromLocal8Bit(ele->second[col].c_str()).toInt();
				itemtext = QString::fromLocal8Bit(DeviceCommonVaries::getInstance()->deviceIndex2Type[index].c_str());
			}
			else {
				itemtext = ele->second[col].c_str();
			}
			configInfoTable->setItem(row, col, new QTableWidgetItem(itemtext));
		}
		row++;
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

	RocketInfoConfig::InfoConfigWidget* infoConfigWidget = RocketInfoConfig::InfoConfigWidget::getInstance();
	infoConfigWidget->show();
	connect(infoConfigWidget, &RocketInfoConfig::InfoConfigWidget::updateRocketInfo, this, [=]() {
		//要更新数据库
		InitDisplayData();

		});
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