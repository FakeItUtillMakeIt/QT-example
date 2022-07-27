#include "FlowEditWidget.h"

FlowEditWidget* FlowEditWidget::instance = nullptr;

FlowEditWidget::FlowEditWidget(QWidget* parent)
	: QWidget(parent)
{
	tableColumnName << QString("���") << QString("����")
		<< QString("����") << QString("����")
		<< QString("��ע");

	this->setBaseSize(640, 480);
	InitLayout();
	initConnection();
}

FlowEditWidget::~FlowEditWidget()
{}


/**
	@brief ��ʼ�����沼��
**/
void FlowEditWidget::InitLayout() {

	this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);

	leftFlowItemList = new QListWidget;

	leftTopInput = new QLineEdit;
	leftTopAdd = new QPushButton;
	leftFlowItemList->setMaximumWidth(140);
	leftTopInput->setMaximumWidth(110);
	leftTopInput->setMaximumHeight(25);
	leftTopAdd->setMaximumWidth(25);

	leftTopInput->setStyleSheet("QLineEdit{background-color:rgb(255,255,255);border-radius:2px;}");
	leftTopAdd->setIcon(QIcon(":/ControlMonitor/images/Flow/add.png"));
	leftFlowItemList->horizontalScrollBar()->hide();
	leftFlowItemList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	leftFlowItemList->setStyleSheet("\
				QListWidget::item{ background-color:rgb(232,232,232);}\
				QListWidget::item:hover{ background-color:rgb(80,170,255);padding-top:-2px;padding-bottom:-1px;} \
				QListWidget::item:selected{ background:rgb(80,170,255);color:black;padding-top:-2px;padding-bottom:-1px;padding-right:20px;} \
				QListWidget{background:rgb(232,232,232);border:0};\
		");
	leftFlowItemList->setContentsMargins(0, 0, 0, 0);
	leftFlowItemList->setFocusPolicy(Qt::NoFocus);

	rightTopFlowName = new QLabel(QString("xxx"));
	rightTable = new CustomTableWidget;
	rightTopD1 = new QLabel();
	rightTopD2 = new QLabel;
	rightTopD3 = new QLabel;
	rightTopD4 = new QLabel;

	rightTopD1->setFixedHeight(24);

	rightTopD3->setMaximumWidth(20);
	rightTopD4->setMaximumWidth(20);
	rightTopD3->setPixmap(QPixmap(":/ControlMonitor/images/Flow/update.png"));
	rightTopD4->setPixmap(QPixmap(":/ControlMonitor/images/Flow/sort.png"));

	rightTable->verticalHeader()->hide();
	rightTable->horizontalHeader()->show();


	rightBottomBtnCancel = new QPushButton(QString("ȡ��"));
	rightBottomBtnOK = new QPushButton(QString("ȷ��"));

	rightTable->setMinimumWidth(600);
	rightTable->setMinimumHeight(400);

	rightBottomBtnCancel->setMaximumHeight(20);
	rightBottomBtnOK->setMaximumHeight(20);

	QGridLayout* leftLayout = new QGridLayout;
	QHBoxLayout* leftTopLayout = new QHBoxLayout;
	QGridLayout* rightLayout = new QGridLayout;
	QHBoxLayout* boxLayout = new QHBoxLayout;
	QHBoxLayout* bottomLayout = new QHBoxLayout;

	leftTopLayout->addWidget(leftTopInput);
	leftTopLayout->addWidget(leftTopAdd);

	leftLayout->addLayout(leftTopLayout, 0, 0);
	leftLayout->addWidget(leftFlowItemList, 1, 0);

	rightLayout->addWidget(rightTopFlowName, 0, 0, 1, 4);
	rightLayout->addWidget(rightTopD1, 0, 4, 1, 1);
	rightLayout->addWidget(rightTopD2, 0, 5, 1, 1);
	rightLayout->addWidget(rightTopD3, 0, 6, 1, 1);
	rightLayout->addWidget(rightTopD4, 0, 7, 1, 1);

	rightLayout->addWidget(rightTable, 1, 0, 1, 8);

	bottomLayout->addStretch();
	bottomLayout->addWidget(rightBottomBtnCancel);
	bottomLayout->addWidget(rightBottomBtnOK);

	boxLayout->addLayout(leftLayout);
	boxLayout->addLayout(rightLayout);
	QVBoxLayout* totalLayout = new QVBoxLayout;
	totalLayout->addLayout(boxLayout);
	totalLayout->addLayout(bottomLayout);

	this->setLayout(totalLayout);




}

void FlowEditWidget::initConnection() {


	connect(leftTopAdd, &QPushButton::clicked, this, &FlowEditWidget::addNewFlow);

	connect(leftFlowItemList, &QListWidget::itemPressed, this, &FlowEditWidget::loadFlowTable);

	connect(rightTable, &CustomTableWidget::mouseRightClicked, this, &FlowEditWidget::tableRightMouseClick);

	connect(rightTable, &CustomTableWidget::cellClicked, this, &FlowEditWidget::tableCellClick);

	connect(rightTable, &CustomTableWidget::cellDoubleClicked, this, &FlowEditWidget::doubleClickCell);

	connect(rightBottomBtnOK, &QPushButton::clicked, this, &FlowEditWidget::clickOKButton);

	connect(rightBottomBtnCancel, &QPushButton::clicked, this, &FlowEditWidget::clickCancelButton);
}


/**
	@brief ���������
**/
void FlowEditWidget::addNewFlow() {


	if (leftTopInput->text().isEmpty())
	{

		return;
	}
	QListWidgetItem* item1 = new QListWidgetItem(leftTopInput->text());
	item1->setTextAlignment(Qt::AlignCenter);
	item1->setIcon(QIcon(QString(":/ControlMonitor/images/Flow/squareBl.png")));
	item1->setSizeHint(QSize(leftFlowItemList->width(), 40));
	leftFlowItemList->addItem(item1);

	rocketType += rocketCode + leftTopInput->text();
}

void FlowEditWidget::setRocketType(QString rocketTypeCode) {
	auto rocketFlowInfo = FlowInfoConfig2DB::getInstance();
	rocketFlowInfo->rocketInfo.clear();
	rocketFlowInfo->readRocketDB2FlowEdit();
	for (auto ele = rocketFlowInfo->rocketInfo.begin(); ele != rocketFlowInfo->rocketInfo.end(); ele++)
	{
		auto rocketName1 = QString::fromStdString(ele->second[2]);
		if (rocketName1 == rocketTypeCode)
		{
			rocketID = QString::fromStdString(ele->second[0]).toInt();
		}

	}

	rocketCode = rocketTypeCode;
}

/**
	@brief ��������
**/
void FlowEditWidget::loadFlowTable() {
	rightTopFlowName->setText(rocketType + QString("-��������"));

	for (int i = 0; i < leftFlowItemList->count(); i++)
	{
		leftFlowItemList->item(i)->setIcon(QIcon(":/ControlMonitor/images/Flow/squareBl.png"));
	}
	leftFlowItemList->currentItem()->setIcon(QIcon(":/ControlMonitor/images/Flow/squareWl.png"));


	int i = 0;

	rightTable->setColumnCount(tableColumnName.size());

	rightTable->setRowCount(1);
	for (QString key : tableColumnName)
	{
		QTableWidgetItem* item = new QTableWidgetItem(key);
		QFont font;
		font.setBold(true);
		item->setFont(font);

		rightTable->setColumnWidth(i, rightTable->width() / 5);
		rightTable->setItem(0, i, new QTableWidgetItem);
		rightTable->setHorizontalHeaderItem(i++, item);

	}

	rightTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}


void FlowEditWidget::tableCellClick(int row, int column) {
	if (rightTable->horizontalHeaderItem(column)->text().contains(tableColumnName[column]))
	{
		QWidget* widget = new QWidget;
		QVBoxLayout* boxLayout = new QVBoxLayout;
		QPushButton* frontInsertCell = new QPushButton(QString("��ǰ����") + tableColumnName[column]);
		QPushButton* backInsertCell = new QPushButton(QString("������") + tableColumnName[column]);
		QPushButton* removeCell = new QPushButton(QString("ɾ��") + tableColumnName[column]);
		frontInsertCell->setStyleSheet("* {color:black;border:none;}");
		backInsertCell->setStyleSheet("* {color:black;border:none;}");
		removeCell->setStyleSheet("* {color:red;border:none;}");
		boxLayout->addWidget(frontInsertCell);
		boxLayout->addWidget(backInsertCell);
		boxLayout->addWidget(removeCell);

		widget->setLayout(boxLayout);

		widget->setWindowFlags(Qt::Popup);
		widget->raise();
		widget->setFixedWidth(rightTable->columnWidth(column));

		widget->move(cursorPos.x(), cursorPos.y());
		widget->show();

		//ɾ��
		connect(removeCell, &QPushButton::clicked, this, [=]() {
			rightTable->removeRow(row);
			widget->deleteLater();
			});

		//��ǰ����
		connect(frontInsertCell, &QPushButton::clicked, this, [=]() {

			if (column == 0 || column == 1)
			{
				if (row == 0)
				{
					rightTable->insertRow(row);
					for (int i = 0; i < tableColumnName.size(); i++)
					{

						rightTable->setItem(row, i, new QTableWidgetItem);

					}
				}
				else
				{
					rightTable->insertRow(row - 1);
					for (int i = 0; i < tableColumnName.size(); i++)
					{
						rightTable->setItem(row - 1, i, new QTableWidgetItem);
					}
				}

			}

			for (int r = 0; r < rightTable->rowCount(); r++)
			{
				rightTable->item(r, 0)->setText(QString::number(r + 1));
			}

			widget->deleteLater();

			}
		);

		//������
		connect(backInsertCell, &QPushButton::clicked, this, [=]() {
			if (column == 0 || column == 1)
			{
				rightTable->insertRow(row + 1);
				for (int i = 0; i < tableColumnName.size(); i++)
				{

					rightTable->setItem(row + 1, i, new QTableWidgetItem);

				}
			}
			if (column == 2)
			{
				auto cellW = rightTable->cellWidget(row, column);
				QComboBox* newcombox = new QComboBox;

				//��ȡ���ݱ�ָ����Ϣ
				auto flowInfoOp = FlowInfoConfig2DB::getInstance();
				flowInfoOp->commandInfo.clear();
				flowInfoOp->readCommandDB2FlowEdit();
				for (auto ele = flowInfoOp->commandInfo.begin(); ele != flowInfoOp->commandInfo.end(); ele++)
				{
					newcombox->addItem(QString::fromStdString(ele->second[3]), QString::fromStdString(ele->second[0]).toInt());
				}

				if (cellW != nullptr)
				{

					cellW->layout()->addWidget(newcombox);
					int wCount = cellW->children().count();
					rightTable->setRowHeight(row, 30 * wCount);

				}
				else {
					QWidget* cellWidget1 = new QWidget;
					QVBoxLayout* boxL = new QVBoxLayout;

					boxL->addWidget(newcombox);
					cellWidget1->setLayout(boxL);
					rightTable->setCellWidget(row, column, cellWidget1);
					rightTable->setRowHeight(row, 30 * rightTable->cellWidget(row, column)->children().count());
				}

			}

			for (int r = 0; r < rightTable->rowCount(); r++)
			{
				rightTable->item(r, 0)->setText(QString::number(r + 1));
			}

			widget->deleteLater();
			}
		);
	}
}

void FlowEditWidget::tableEditClick() {
	rightTable->setEditTriggers(QAbstractItemView::CurrentChanged);
	for (int r = 1; r < rightTable->rowCount(); r++)
	{
		for (int c = 1; c < rightTable->columnCount(); c++)
		{
			rightTable->item(r, c)->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		}
	}
}

void FlowEditWidget::tableRightMouseClick(QPoint pos) {
	cursorPos = pos;
}


void FlowEditWidget::doubleClickCell(int row, int column) {
	rightTable->editItem(rightTable->item(row, column));
}

/**
	@brief �洢��Ϣ�����ݿ�
**/
void FlowEditWidget::clickOKButton() {

	if (rightTable->rowCount() == 0)
	{
		qDebug() << QString("����Ϊ����Ϣ");
		this->deleteLater();
		return;
	}

	if (rocketID == -1)
	{
		return;
	}

	auto flowInfoOP = FlowInfoConfig2DB::getInstance();

	for (int r = 0; r < rightTable->rowCount(); r++)
	{

		QString mainFlowName;
		QString mainFlowIndex;
		QString backInfo;
		QString remark;

		int mainFlowID;
		QString subFlowName;
		int emitCmdId;

		mainFlowName = rightTable->item(r, 1)->text();
		mainFlowIndex = rightTable->item(r, 0)->text();
		backInfo = rightTable->item(r, 3)->text();
		remark = rightTable->item(r, 4)->text();

		flowInfoOP->mainFlowConfigOp2DB(rocketID, mainFlowName, mainFlowIndex, backInfo, remark);

		flowInfoOP->readMainFlowDB2FlowEdit();
		for (auto ele = flowInfoOP->mainFlowInfo.begin(); ele != flowInfoOP->mainFlowInfo.end(); ele++)
		{

			if (QString::fromStdString(ele->second[1]).toInt() == rocketID && QString::fromStdString(ele->second[2]) == mainFlowName)
			{
				mainFlowID = QString::fromStdString(ele->second[0]).toInt();
			}
		}

		//
		QList<QComboBox*> allCombo = rightTable->cellWidget(r, 2)->findChildren<QComboBox*>();
		for (auto obj : allCombo)
		{
			QString cmdName;
			int cmdID;

			cmdName = obj->currentText();
			cmdID = obj->currentData().toInt();

			flowInfoOP->subFlowConfigOp2DB(mainFlowID, cmdID, cmdName, remark);
		}

	}

	this->close();
}
void FlowEditWidget::clickCancelButton() {
	this->close();
}