#include "CommandManageModule.h"

CommandManageModule::CommandManageModule(QWidget* parent)
	: QWidget(parent)
{
	selectedRowNum = -1;
	columnNameList << QString("ָ��ID") << QString("����ͺ�ID") << QString("�ⷢ����ID") << QString("ָ������") << QString("ָ�����") << QString("ָ������") << QString("ָ��ǰ׺");

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
	insertButton->setStyleSheet(wss->insertButtonStyleSheet.arg("QPushButton"));
	deleteButton->setStyleSheet(wss->deleteButtonStyleSheet.arg("QPushButton"));
	updateButton->setStyleSheet(wss->iconButtonStyleSheet.arg("QPushButton"));
	settingButton->setStyleSheet(wss->iconButtonStyleSheet.arg("QPushButton"));
	collapseButton->setStyleSheet(wss->iconButtonStyleSheet.arg("QPushButton"));
	expandButton->setStyleSheet(wss->iconButtonStyleSheet.arg("QPushButton"));

	configInfoTable->setVerticalHeaderItem(0, new QTableWidgetItem("���"));
	for (int col = 0; col < configInfoTable->columnCount(); col++)
	{

		QTableWidgetItem* item = new QTableWidgetItem();
		item->setText(columnNameList[col]);
		configInfoTable->setItem(0, col, new QTableWidgetItem);
		configInfoTable->setHorizontalHeaderItem(col, item);
	}

	this->setLayout(mainUILay);

	connect(insertButton, &QPushButton::clicked, this, &CommandManageModule::insertOneRowData);
	connect(configInfoTable, &QTableWidget::itemClicked, this, [=]() {
		selectedRowNum = configInfoTable->currentRow();
		});
	connect(deleteButton, &QPushButton::clicked, this, &CommandManageModule::deleteOneRowData);

}

/**
	@brief ��ʼ����ʾ����
**/
void CommandManageModule::InitDisplayData() {


	DeviceDBConfigInfo* commandInfoDB = DeviceDBConfigInfo::getInstance();
	commandInfoDB->readCommandDB2UI();
	configInfoTable->setRowCount(commandInfoDB->commandInfo.size());
	int row = 0;
	for (auto ele = commandInfoDB->commandInfo.begin(); ele != commandInfoDB->commandInfo.end(); ele++)
	{
		//�������͵�λ
		for (int col = 0; col < ele->second.size(); col++)
		{
			QString itemtext;
			//����
			if (col == 4)
			{
				auto index = QString::fromLocal8Bit(ele->second[col].c_str()).toInt();
				itemtext = QString::fromLocal8Bit(DeviceCommonVaries::getInstance()->commandIndex2Code[index].c_str());
			}
			else if (col == 5)
			{
				auto index = QString::fromLocal8Bit(ele->second[col].c_str()).toInt();
				itemtext = QString::fromLocal8Bit(DeviceCommonVaries::getInstance()->commandIndex2Type[index].c_str());

			}
			else if (col == 6)
			{
				auto index = QString::fromLocal8Bit(ele->second[col].c_str()).toInt();
				itemtext = QString::fromLocal8Bit(DeviceCommonVaries::getInstance()->commandIndex2Prefix[index].c_str());

			}
			else {
				itemtext = ele->second[col].c_str();
			}
			configInfoTable->setItem(row, col, new QTableWidgetItem(itemtext));
		}
		row++;
	}
}



void CommandManageModule::paintEvent(QPaintEvent* event) {

	for (int columnIndex = 0; columnIndex < configInfoTable->columnCount(); columnIndex++)
	{
		configInfoTable->setColumnWidth(columnIndex, (configInfoTable->width()) / (configInfoTable->columnCount()));
	}
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
	@brief ɾ��һ������
**/
void CommandManageModule::deleteOneRowData() {
	if (selectedRowNum != -1)
	{
		configInfoTable->removeRow(selectedRowNum);

	}
}