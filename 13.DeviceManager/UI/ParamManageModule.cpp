#include "ParamManageModule.h"

ParamManageModule::ParamManageModule(QWidget* parent)
	: QWidget(parent)
{

	selectedRowNum = 0;
	columnNameList << QString("参数ID") << QString("参数名称") << QString("参数类型") << QString("参数单位");

	InitUILayout();
	InitDisplayData();
}

ParamManageModule::~ParamManageModule()
{

}

/**
	@brief 初始化数据库连接
**/
void ParamManageModule::InitDisplayData() {


	DeviceDBConfigInfo* paramInfoDB = DeviceDBConfigInfo::getInstance();
	paramInfoDB->readParamDB2UI();
	configInfoTable->setRowCount(paramInfoDB->paramInfo.size());
	int row = 0;
	for (auto ele = paramInfoDB->paramInfo.begin(); ele != paramInfoDB->paramInfo.end(); ele++)
	{
		//名称类型单位
		for (int col = 0; col < ele->second.size(); col++)
		{
			QString itemtext;
			//类型
			if (col == 2)
			{
				auto index = QString::fromLocal8Bit(ele->second[col].c_str()).toInt();
				itemtext = QString::fromLocal8Bit(DeviceCommonVaries::getInstance()->paramIndex2Type[index].c_str());
			}
			else {
				itemtext = ele->second[col].c_str();
			}
			configInfoTable->setItem(row, col, new QTableWidgetItem(itemtext));
		}
		row++;
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


	configInfoTable->setVerticalHeaderItem(0, new QTableWidgetItem("序号"));
	for (int col = 0; col < configInfoTable->columnCount(); col++)
	{

		QTableWidgetItem* item = new QTableWidgetItem();
		item->setText(columnNameList[col]);
		configInfoTable->setItem(0, col, new QTableWidgetItem);
		configInfoTable->setHorizontalHeaderItem(col, item);
	}

	this->setLayout(mainUILay);

	connect(insertButton, &QPushButton::clicked, this, &ParamManageModule::insertOneRowData);
	connect(configInfoTable, &QTableWidget::itemClicked, this, [=]() {
		selectedRowNum = configInfoTable->currentRow();
		});
	connect(deleteButton, &QPushButton::clicked, this, &ParamManageModule::deleteOneRowData);

}


void ParamManageModule::paintEvent(QPaintEvent* event) {

	for (int columnIndex = 0; columnIndex < configInfoTable->columnCount(); columnIndex++)
	{
		configInfoTable->setColumnWidth(columnIndex, (configInfoTable->width()) / (configInfoTable->columnCount()));
	}
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