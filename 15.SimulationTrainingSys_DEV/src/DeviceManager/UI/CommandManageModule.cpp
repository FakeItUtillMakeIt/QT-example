#include "CommandManageModule.h"

CommandManageModule::CommandManageModule(QWidget* parent)
	: QWidget(parent)
{
	selectedRowNum = -1;
	columnNameList << QString("ָ��ID") << QString("����ͺ�ID") << QString("�ⷢ����ID") << QString("ָ������") << QString("ָ�����") << QString("ָ������") << QString("ָ��ǰ׺") << QString("����");

	InitUILayout();

	InitDisplayData();
}

CommandManageModule::~CommandManageModule()
{}


/**
	@brief ָ��������沼��
**/
void CommandManageModule::InitUILayout() {
	//�͵ײ�״̬����ɫ����һ��
	WidgetStyleSheet* wss = WidgetStyleSheet::getInstace();

	QGridLayout* mainUILay = new QGridLayout;

	QLabel* deviceParamLabel = new QLabel(QString("ָ�����Ʋ�ѯ:"));
	QComboBox* deviceCombox = new QComboBox();
	QLabel* paramNameLabel = new QLabel(QString("ָ������:"));
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
		int searchRow = 0;
		for (auto ele = DeviceDBConfigInfo::getInstance()->commandInfo.begin(); ele != DeviceDBConfigInfo::getInstance()->commandInfo.end(); ele++)
		{
			if (QString::fromStdString(ele->second[3]).contains(paramInputName->text()))
			{
				QVector<QString> rowData;
				rowData.push_back(QString::fromStdString(ele->second[0]));
				rowData.push_back(QString::fromStdString(ele->second[1]));
				rowData.push_back(QString::fromStdString(ele->second[2]));
				rowData.push_back(QString::fromStdString(ele->second[3]));
				rowData.push_back(QString::fromStdString(ele->second[4]));
				rowData.push_back(QString::fromStdString(ele->second[5]));
				rowData.push_back(QString::fromStdString(ele->second[6]));
				insertOneRow(searchRow++, rowData);
			}
		}
		});
}



/**
	@brief ����һ��
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
	hbox->addWidget(new QPushButton(QString("�༭")));
	hbox->addWidget(new QPushButton(QString("ɾ��")));
	w1->setLayout(hbox);
	w1->setStyleSheet("*{border:none;color:blue;}");

	configInfoTable->setCellWidget(insertRow, columnNameList.size() - 1, w1);
}

/**
	@brief ��ʼ����ʾ����
**/
void CommandManageModule::InitDisplayData() {


	DeviceDBConfigInfo* commandInfoDB = DeviceDBConfigInfo::getInstance();
	commandInfoDB->readCommandDB2UI();
	//configInfoTable->setRowCount(commandInfoDB->commandInfo.size());
	int row = 0;
	for (auto ele = commandInfoDB->commandInfo.begin(); ele != commandInfoDB->commandInfo.end(); ele++)
	{

		QVector<QString> rowData;
		rowData.push_back(QString::fromStdString(ele->second[0]));
		rowData.push_back(QString::fromStdString(ele->second[1]));
		rowData.push_back(QString::fromStdString(ele->second[2]));
		rowData.push_back(QString::fromStdString(ele->second[3]));
		rowData.push_back(QString::fromStdString(ele->second[4]));
		rowData.push_back(QString::fromStdString(ele->second[5]));
		rowData.push_back(QString::fromStdString(ele->second[6]));
		insertOneRow(row++, rowData);

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
	@brief ����һ������
**/
void CommandManageModule::insertOneRowData() {

	CommandInfoConfig::InfoConfigWidget* infoConfigWidget = CommandInfoConfig::InfoConfigWidget::getInstance();
	infoConfigWidget->show();
	connect(infoConfigWidget, &CommandInfoConfig::InfoConfigWidget::updateCommandInfos, this, [=]() {
		//Ҫ�������ݿ�
		InitDisplayData();

		});
}

/**
	@brief ɾ��һ������  ��ͬ�����ݿ�  �˴��漰�Ķ��ϴ���ʱ����
**/
void CommandManageModule::deleteOneRowData() {
	if (selectedRowNum != -1)
	{
		configInfoTable->removeRow(selectedRowNum);

	}
}