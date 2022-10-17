#include "FlowEditWidget.h"
#include "QLineEdit"


FlowEditWidget* FlowEditWidget::instance = nullptr;

FlowEditWidget::FlowEditWidget(QWidget* parent)
	: QWidget(parent)
{
	tableColumnName << QString("���") << QString("�׶�")
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
	this->setWindowTitle(QString("���̱༭"));
	this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);

	leftFlowItemList = new QListWidget;

	leftTopInput = new QLineEdit;
	leftTopAdd = new QPushButton;
	leftFlowItemList->setMaximumWidth(140);
	leftTopInput->setMaximumWidth(110);
	leftTopInput->setMaximumHeight(25);
	leftTopAdd->setMaximumWidth(25);

	leftTopInput->setStyleSheet("QLineEdit{background-color:rgb(255,255,255);border-radius:2px;font: 12px ΢���ź�;}");
	leftTopAdd->setIcon(QIcon(":/flowload/images/Flow/Edit_circleplus.png"));
	leftFlowItemList->horizontalScrollBar()->hide();
	leftFlowItemList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	leftFlowItemList->setStyleSheet("\
				QListWidget::item{ background-color:rgb(232,232,232);font: 12px ΢���ź�;}\
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
	//rightTopD3->setPixmap(QPixmap(":/ControlMonitor/images/Flow/update.png"));
	//rightTopD4->setPixmap(QPixmap(":/ControlMonitor/images/Flow/sort.png"));

	rightTable->verticalHeader()->hide();
	rightTable->horizontalHeader()->show();
	rightTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

	rightBottomBtnCancel = new QPushButton(QString("ȡ��"));
	rightBottomBtnOK = new QPushButton(QString("ȷ��"));

	rightBottomBtnOK->setStyleSheet("*{border-radius:5px;width:66px;height:34px;border-image:url(:/flowload/images/Flow/bt_normal.png);font: 12px ΢���ź�;color:white;}");
	rightBottomBtnCancel->setStyleSheet("*{border-radius:5px;width:66px;height:34px;border-image:url(:/flowload/images/Flow/bt_normal.png);font: 12px ΢���ź�;color:white;}");
	/*rightBottomBtnOK->setStyleSheet("*{border:none;width:66px;height:34px;background-image:url(:/flowload/images/Flow/button_OK.png);font: 12px ΢���ź�;}");
	rightBottomBtnCancel->setStyleSheet("*{border:none;width:66px;height:34px;background-image:url(:/flowload/images/Flow/button_cancel.png);font: 12px ΢���ź�;}");*/

	rightTable->setMinimumWidth(600);
	rightTable->setMinimumHeight(400);

	//rightBottomBtnCancel->setMaximumHeight(20);
	//rightBottomBtnOK->setMaximumHeight(20);

	QGridLayout* leftLayout = new QGridLayout;
	QHBoxLayout* leftTopLayout = new QHBoxLayout;
	QGridLayout* rightLayout = new QGridLayout;
	QHBoxLayout* boxLayout = new QHBoxLayout;
	QHBoxLayout* bottomLayout = new QHBoxLayout;

	leftTopLayout->addWidget(leftTopInput);
	leftTopLayout->addWidget(leftTopAdd);

	leftLayout->addLayout(leftTopLayout, 0, 0);
	leftLayout->addWidget(leftFlowItemList, 1, 0);

	leftTopInput->hide();
	leftTopAdd->hide();
	leftFlowItemList->hide();

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


	//��ʼ������
	tableFlowInfoHeader = new MainFlowInfoSt;
	tableFlowInfoHeader->startRowNum = 0;
	tableFlowInfoHeader->rowSpanCount = 0;
	tableFlowInfoHeader->sortNum = 1;
	//
	FlowInfoConfig2DB::getInstance()->customReadTableInfo(QString("SELECT\
				command_info.id,\
				command_info.`name`,\
				command_info.createTime,\
				command_info.lastUpdateTime\
				FROM\
				command_info\
				WHERE\
				command_info.type = 1 AND\
				command_info.rocket_id = %1").arg(AppCache::instance()->m_CurrentRocketType->m_id));

	for (auto ele : FlowInfoConfig2DB::getInstance()->customReadInfoMap)
	{
		cmdName2Id[QString::fromStdString(ele.second[1])] = ele.first;
	}

}

void FlowEditWidget::initConnection()
{
	connect(leftTopAdd, &QPushButton::clicked, this, &FlowEditWidget::addNewFlow);
	connect(leftFlowItemList, &QListWidget::itemPressed, this, &FlowEditWidget::loadFlowTable);
	connect(rightTable, &CustomTableWidget::mouseRightClicked, this, &FlowEditWidget::tableRightMouseClick);
	connect(rightTable, &CustomTableWidget::cellClicked, this, &FlowEditWidget::tableCellClick);
	connect(rightTable, &CustomTableWidget::cellDoubleClicked, this, &FlowEditWidget::doubleClickCell);
	connect(rightBottomBtnOK, &QPushButton::clicked, this, &FlowEditWidget::clickOKButton);
	connect(rightBottomBtnCancel, &QPushButton::clicked, this, &FlowEditWidget::clickCancelButton);
}

void FlowEditWidget::setMainFlowInfo(QMap<int, QVector<QString>> mainFlowInfo) {
	mainFlowInfo_.clear();
	mainFlowInfo_ = mainFlowInfo;
}
void FlowEditWidget::setSubFlowInfo(QMap<int, QVector<QString>> subFlowInfo) {
	subFlowInfo_.clear();
	subFlowInfo_ = subFlowInfo;
}

void FlowEditWidget::setSubFlowInfo1(QMap<int, QVector<QString>> subFlowInfo1) {
	subFlowInfo1_.clear();
	subFlowInfo1_ = subFlowInfo1;
}

void FlowEditWidget::setSubFlowInfo2(QMap<int, QVector<QString>> subFlowInfo2) {
	subFlowInfo2_.clear();
	subFlowInfo2_ = subFlowInfo2;
}

void FlowEditWidget::setFlowCmdID(QMap<int, QVector<int>> subFlowCmdID) {
	subFlowCmdID_.clear();
	subFlowCmdID_ = subFlowCmdID;
	loadFlowDisplayFlow();
}

/**
	@brief ����FlowDisplayWidget��ʾ�����Ա༭
**/
void FlowEditWidget::loadFlowDisplayFlow() {


	rightTable->clear();

	rightTable->setItemDelegate(new Delegate);

	rightTable->setRowCount(0);
	rightTopFlowName->setText(rocketType);
	rightTable->setColumnCount(tableColumnName.size());
	for (int i = 0; i < tableColumnName.size(); i++)
	{
		QTableWidgetItem* item1 = new QTableWidgetItem(tableColumnName[i]);
		item1->setTextAlignment(Qt::AlignCenter);
		rightTable->setHorizontalHeaderItem(i, item1);
		rightTable->setColumnWidth(i, rightTable->width() / 5);

	}
	rightTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//Ϊ������ʱ 
	if (mainFlowInfo_.size() == 0)
	{
		rightTable->setRowCount(1);
		mainFlowInfo_[1].push_back("");
		mainFlowInfo_[1].push_back("");
		mainFlowInfo_[1].push_back("");
		mainFlowInfo_[1].push_back("");
	}


	//��ȡ���ݱ�ָ����Ϣ
	auto flowInfoOp = FlowInfoConfig2DB::getInstance();
	flowInfoOp->commandInfo.clear();
	flowInfoOp->readCommandDB2FlowEdit();
	//ά����ǰ��������ʹ�õ�ָ���δʹ�õ�ָ���б�
	for (auto ele = flowInfoOp->commandInfo.begin(); ele != flowInfoOp->commandInfo.end(); ele++)
	{
		unUsedCmdList.push_back(QString::fromStdString(ele->second[3]));
	}

	MainFlowInfoSt* tableFlowInfoHeader1 = tableFlowInfoHeader;
	int infoRow = 0;
	for (int i = 1; i <= mainFlowInfo_.size(); i++)
	{
		//������ID
		int mainID = mainFlowInfo_[i][3].toInt();
		int startSpanRow = infoRow;
		//������
		QString subCmd, subBackCmd, subRemark;
		for (int subIndex = 0; subIndex < subFlowInfo_.value(mainID).size(); subIndex++)
		{
			rightTable->insertRow(infoRow);
			subCmd = subFlowInfo_.value(mainID)[subIndex];
			subCmd = subCmd.replace(QRegExp("[\\s+\n\t]"), "");
			subBackCmd = subFlowInfo1_.value(mainID)[subIndex];
			subBackCmd = subBackCmd.replace(QRegExp("[\\s+\n\t]"), "");
			subRemark = subFlowInfo2_.value(mainID)[subIndex];
			subRemark = subRemark.replace(QRegExp("[\\s+\n\t]"), "");

			rightTable->setItem(infoRow, 2, new QTableWidgetItem(subCmd));
			rightTable->setItem(infoRow, 3, new QTableWidgetItem(subBackCmd));
			rightTable->setItem(infoRow, 4, new QTableWidgetItem(subRemark));

			infoRow++;
		}
		//����
		rightTable->setItem(startSpanRow, 0, new QTableWidgetItem(QString::number(i)));
		rightTable->setItem(startSpanRow, 1, new QTableWidgetItem(mainFlowInfo_[i][0]));
		rightTable->setSpan(startSpanRow, 0, subFlowInfo_.value(mainID).size(), 1);
		rightTable->setSpan(startSpanRow, 1, subFlowInfo_.value(mainID).size(), 1);

		MainFlowInfoSt* tableFlowInfo = new MainFlowInfoSt;
		tableFlowInfo->sortNum = i;
		tableFlowInfo->startRowNum = startSpanRow;
		tableFlowInfo->rowSpanCount = subFlowInfo_.value(mainID).size();
		tableFlowInfoHeader1->next = tableFlowInfo;
		tableFlowInfoHeader1 = tableFlowInfoHeader1->next;

	}


}

/**
	@brief Ĭ�ϼ������ݿ������е����� ��ʾ����list��
**/
void FlowEditWidget::loadDBSavedFlow() {
	//������ҳ��ʾ����  �Ա༭
	auto rocketFlowInfo = FlowInfoConfig2DB::getInstance();

}

/**
	@brief ���������  ��ʱ�����������
**/
void FlowEditWidget::addNewFlow() {
	return;

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

void FlowEditWidget::setRocketType(QString rocketTypeCode, int rocketId) {

	rocketCode = rocketTypeCode;
	rocketID = rocketId;

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

/**
	@brief �Ҽ�����¼�
	@param row    -
	@param column -
**/
void FlowEditWidget::tableCellClick(int row, int column) {

	if (rightTable->horizontalHeaderItem(column)->text() == "���"
		|| rightTable->horizontalHeaderItem(column)->text() == "����"
		|| rightTable->horizontalHeaderItem(column)->text() == "��ע")
	{
		return;
	}

	QWidget* widget = new QWidget;
	widget->setObjectName("widget");
	QVBoxLayout* boxLayout = new QVBoxLayout;
	QPushButton* frontInsertCell = new QPushButton(QString("��ǰ����") + tableColumnName[column]);
	QPushButton* backInsertCell = new QPushButton(QString("������") + tableColumnName[column]);
	QPushButton* removeCell = new QPushButton(QString("ɾ��") + tableColumnName[column]);

	frontInsertCell->setStyleSheet("* {color:black;border:none;font: 12px ΢���ź�;}");
	backInsertCell->setStyleSheet("* {color:black;border:none;font: 12px ΢���ź�;}");
	removeCell->setStyleSheet("* {color:red;border:none;font: 12px ΢���ź�;}");
	if (tableColumnName[column] == "�׶�") boxLayout->addWidget(frontInsertCell);
	boxLayout->addWidget(backInsertCell);

	boxLayout->addWidget(removeCell);
	widget->setStyleSheet(QString("*{background-color:white;border: 1px solid gray;}"));
	widget->setLayout(boxLayout);
	widget->setWindowFlags(Qt::Popup);
	widget->raise();
	widget->setFixedWidth(rightTable->columnWidth(column));

	widget->move(cursorPos.x(), cursorPos.y());
	widget->show();


	//ɾ��  �ж�Ϊ�ڼ���
	connect(removeCell, &QPushButton::clicked, this, [=]()
		{
			auto retMsg = QMessageBox::question(nullptr, QString("����"), QString("�Ƿ�ȷ��ɾ������?"), QString("ȡ��"), QString("ȷ��"));
			if (retMsg == 0)
			{
				return;
			}
			auto tmpHeader = tableFlowInfoHeader;

			switch (column)
			{
			case 1:
			{//ɾ���׶� 
				//���θ��º����ɾ������ͷ����Ӧ�
				int startRow1 = 0;
				int rowSpan1 = 0;
				while (tmpHeader->next)
				{
					if (tmpHeader->next->startRowNum == row)
					{
						startRow1 = row;
						rowSpan1 = tmpHeader->next->rowSpanCount;
						removeRowFrom2Index(startRow1, rowSpan1);
						tmpHeader->next->rowSpanCount = 0;
						recurseUpdate(tmpHeader->next, 1);
						tmpHeader->next = tmpHeader->next->next;
						updateSortIndex(tableFlowInfoHeader->next);
						break;
					}
					tmpHeader = tmpHeader->next;
				}

				break;
			}
			case 2:
			{//ɾ������ 
				//������������Ӧ���rowspan
				while (tmpHeader->next)
				{
					if (row >= tmpHeader->next->startRowNum && row < (tmpHeader->next->startRowNum + tmpHeader->next->rowSpanCount))
					{
						QString stageInfo = rightTable->item(tmpHeader->next->startRowNum, 1)->text();

						rightTable->removeRow(row);
						//���ɾ����Ϊ�׶εĵ�һ��Ԫ��
						if (row == tmpHeader->next->startRowNum && tmpHeader->next->rowSpanCount != 1)
						{
							rightTable->setItem(row, 1, new QTableWidgetItem(stageInfo));
						}
						tmpHeader->next->rowSpanCount -= 1;

						recurseUpdate(tmpHeader->next, 2);
						if (tmpHeader->next->rowSpanCount == 0)
						{
							tmpHeader->next = tmpHeader->next->next;
						}
						updateSortIndex(tableFlowInfoHeader->next);
						break;
					}
					tmpHeader = tmpHeader->next;
				}
				break;
			}
			default:
				break;
			}
		});

	//��ǰ����
	connect(frontInsertCell, &QPushButton::clicked, this, [=]()
		{
			if (column == 1)
			{//����׶� 

			}
			//�޸�ÿ�е����

		});

	//������
	connect(backInsertCell, &QPushButton::clicked, this, [=]() {

		auto tmpHeader = tableFlowInfoHeader;
		if (column == 1)
		{//����׶�
			while (tmpHeader->next)
			{
				if (tmpHeader->next->startRowNum == row)
				{
					addRow(row + tmpHeader->next->rowSpanCount);
					//rightTable->item(row + tmpHeader->next->rowSpanCount, 0)->setText(QString::number(tmpHeader->next->sortNum + 1));
					//�޸ĺ�������
					recurseUpdate(tmpHeader->next, 3);
					MainFlowInfoSt* newStageNode = new MainFlowInfoSt;
					newStageNode->sortNum = tmpHeader->next->sortNum + 1;
					newStageNode->rowSpanCount = 1;
					newStageNode->startRowNum = tmpHeader->next->startRowNum + tmpHeader->next->rowSpanCount;
					newStageNode->next = tmpHeader->next->next;
					tmpHeader->next->next = newStageNode;
					updateSortIndex(tableFlowInfoHeader->next);
					break;
				}
				tmpHeader = tmpHeader->next;
			}

		}
		if (column == 2)
		{//������� 
			while (tmpHeader->next)
			{
				if (row >= tmpHeader->next->startRowNum && row < (tmpHeader->next->startRowNum + tmpHeader->next->rowSpanCount))
				{
					addRow(row + 1);
					recurseUpdate(tmpHeader->next, 4);
					tmpHeader->next->rowSpanCount += 1;
					rightTable->setSpan(tmpHeader->next->startRowNum, 0, tmpHeader->next->rowSpanCount, 1);
					rightTable->setSpan(tmpHeader->next->startRowNum, 1, tmpHeader->next->rowSpanCount, 1);
					break;
				}
				tmpHeader = tmpHeader->next;
			}
		}
		}
	);

}

/**
	@brief �༭�¼�
**/
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

/**
	@brief �Ҽ���ȡλ��
	@param pos -
**/
void FlowEditWidget::tableRightMouseClick(QPoint pos) {
	cursorPos = pos;
}

/**
	@brief ˫���¼�
	@param row    -
	@param column -
**/
void FlowEditWidget::doubleClickCell(int row, int column) {
	rightTable->editItem(rightTable->item(row, column));
}

/**
	@brief �洢��Ϣ�����ݿ�
			������Ҫ�������������̻����½����̽������֣������������޸����ݿ⣬�½�������������ݿ�
**/
void FlowEditWidget::clickOKButton() {

	if (rightTable->rowCount() == 0)
	{
		//qDebug() << QString("����Ϊ����Ϣ");
		this->deleteLater();
		return;
	}
	if (rocketID == -1)
	{
		return;
	}
	auto flowInfoOP = FlowInfoConfig2DB::getInstance();
	//�Ȼ�ȡ��Ӧ�ͺŵ������̺�������
	//���������ݿ����
	QString qSqlString = "SELECT\
		sub_flow_info.id\
		FROM\
		main_flow_info\
		INNER JOIN sub_flow_info ON sub_flow_info.main_id = main_flow_info.id\
		WHERE\
		main_flow_info.rocket_id = %1";
	qSqlString = qSqlString.arg(rocketID);
	flowInfoOP->customDBQuery(qSqlString);
	vector<int> v;
	v.reserve(flowInfoOP->customSearchInfo.size());

	for_each(flowInfoOP->customSearchInfo.begin(), flowInfoOP->customSearchInfo.end(),
		[&v](const unordered_map<int, vector<string>>::value_type& p) {v.push_back(p.first); });

	//�����ж�
	hadUsedCmdList.clear();
	hadSaveStage.clear();
	int stageRow = -1;
	for (int r = 0; r < rightTable->rowCount(); r++)
	{
		if (rightTable->item(r, 1) && !rightTable->item(r, 1)->text().isEmpty())
		{
			stageRow = r;
			if (hadSaveStage.contains(rightTable->item(r, 1)->text()))
			{
				QMessageBox::warning(nullptr, QString("����"), QString("�׶����ظ�(��:%1),�׶�:%2��").arg(r).arg(rightTable->item(stageRow, 1)->text()), QString("ȷ��"), QString("ȡ��"));
				return;
			}
			hadSaveStage.push_back(rightTable->item(r, 1)->text());
		}
		if (rightTable->item(r, 2)->text().isEmpty() || rightTable->item(r, 2)->text().isNull())
		{
			QMessageBox::warning(nullptr, QString("����"), QString("ָ����Ϊ��(��:%1),�׶�:%2��").arg(r).arg(rightTable->item(stageRow, 1)->text()), QString("ȷ��"), QString("ȡ��"));
			return;
		}
		if (hadUsedCmdList.contains(rightTable->item(r, 2)->text()))
		{
			QMessageBox::warning(nullptr, QString("����"), QString("�Ѵ���ָ��%1(��:%2),�׶�:%3��").arg(rightTable->item(r, 2)->text()).arg(r).arg(rightTable->item(stageRow, 1)->text()), QString("ȷ��"), QString("ȡ��"));
			return;
		}
		hadUsedCmdList.push_back(rightTable->item(r, 2)->text());
		if (rightTable->item(r, 3)->text().isEmpty() || rightTable->item(r, 3)->text().isNull())
		{
			QMessageBox::warning(nullptr, QString("����"), QString("ָ��%1��ȱ�ٻ�����Ϣ(��:%2),�׶�:%3��").arg(rightTable->item(r, 2)->text()).arg(r).arg(rightTable->item(stageRow, 1)->text()), QString("ȷ��"), QString("ȡ��"));
			return;
		}
	}


	//��ɾ��������ɾ������
	flowInfoOP->clearSubFlowDB(v);
	flowInfoOP->clearMainFlowDB(rocketID);

	auto tmpHeader = tableFlowInfoHeader;
	while (tmpHeader->next)
	{
		QString mainFlowName;
		QString mainFlowIndex;
		QString backInfo;
		QString remark;
		QString cmdName;
		int cmdID;
		mainFlowName = rightTable->item(tmpHeader->next->startRowNum, 1)->text();
		mainFlowIndex = QString::number(tmpHeader->next->sortNum);
		backInfo = "";
		remark = "";
		//����������Ϣд�����ݿ�
		flowInfoOP->mainFlowConfigOp2DB(rocketID, mainFlowName, mainFlowIndex, backInfo, remark);
		flowInfoOP->readMainFlowDB2FlowEdit();
		FlowInfoConfig2DB::getInstance()->customReadTableInfo(QString("SELECT\
			main_flow_info.id,\
			main_flow_info.createTime,\
			main_flow_info.lastUpdateTime\
			FROM\
			main_flow_info\
			WHERE\
			main_flow_info.rocket_id = %1 AND\
			main_flow_info.`name`='%2' AND\
			main_flow_info.`index`='%3'").arg(rocketID).arg(mainFlowName).arg(mainFlowIndex));
		for (auto ele : FlowInfoConfig2DB::getInstance()->customReadInfoMap)
		{
			for (int indexStart = 0; indexStart < tmpHeader->next->rowSpanCount; indexStart++)
			{
				cmdName = rightTable->item(tmpHeader->next->startRowNum + indexStart, 2)->text();
				cmdID = cmdName2Id[cmdName];
				auto qeq = rightTable->item(tmpHeader->next->startRowNum + indexStart, 2)->data(Qt::DisplayRole);

				backInfo = rightTable->item(tmpHeader->next->startRowNum + indexStart, 3)->text();
				remark = rightTable->item(tmpHeader->next->startRowNum + indexStart, 4)->text();
				//����������Ϣд��
				flowInfoOP->subFlowConfigOp2DB(ele.first, cmdID, cmdName, backInfo, remark);
			}
		}
		tmpHeader = tmpHeader->next;
	}


	emit updateDisPlayFlow();

	this->close();
}


/**
	@brief ȡ����ť�¼�
**/
void FlowEditWidget::clickCancelButton() {

	this->close();
}
/**
	@brief ���ι����¼�
**/
bool FlowEditWidget::eventFilter(QObject* obj, QEvent* event)
{
	if (obj->inherits("QComboBox")) {

		if (event->type() == QEvent::Wheel)
		{
			return true;
		}
	}
	return false;
}

/**
	@brief ɾ���׶�ʱ�ݹ������������
	@param node -��ɾ������
**/
void FlowEditWidget::recurseUpdate(MainFlowInfoSt* node, int type) {
	if (node->next)
	{
		//���½׶�
		if (type == 1)
		{
			node->next->startRowNum = node->startRowNum + node->rowSpanCount;
			recurseUpdate(node->next, 1);
			node->next->sortNum = node->sortNum;
		}
		//���¿���
		if (type == 2)
		{
			node->next->startRowNum -= 1;
			recurseUpdate(node->next, 2);
		}
		if (type == 3)
		{
			node->next->sortNum += 1;
			node->next->startRowNum += 1;
			//rightTable->item(node->next->startRowNum, 0)->setText(QString::number(node->next->sortNum));
			recurseUpdate(node->next, 3);
		}
		if (type == 4)
		{
			node->next->startRowNum += 1;
			recurseUpdate(node->next, 4);
		}
	}

}

/**
	@brief ɾ����
	@param startRow -
	@param rowSpan  -
**/
void FlowEditWidget::removeRowFrom2Index(int startRow, int rowSpan) {

	for (int i = rowSpan; i > 0; i--)
	{
		rightTable->removeRow(startRow + i - 1);
	}

}
/**
	@brief �������
	@param header -
**/
void FlowEditWidget::updateSortIndex(MainFlowInfoSt* header) {
	int i = 1;
	while (header)
	{
		header->sortNum = i;
		if (rightTable->item(header->startRowNum, 0))
		{
			rightTable->item(header->startRowNum, 0)->setText(QString::number(i++));
		}
		else
		{
			rightTable->setItem(header->startRowNum, 0, new QTableWidgetItem(QString::number(i++)));
		}
		header = header->next;
	}
}

/**
	@brief ���һ������
	@param rowIndex -
**/
void FlowEditWidget::addRow(int rowIndex) {
	rightTable->insertRow(rowIndex);
	for (int i = 0; i < tableColumnName.size(); i++)
	{
		rightTable->setItem(rowIndex, i, new QTableWidgetItem);
	}

}