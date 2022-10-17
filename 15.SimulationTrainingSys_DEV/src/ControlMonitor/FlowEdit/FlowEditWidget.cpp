#include "FlowEditWidget.h"
#include "QLineEdit"


FlowEditWidget* FlowEditWidget::instance = nullptr;

FlowEditWidget::FlowEditWidget(QWidget* parent)
	: QWidget(parent)
{
	tableColumnName << QString("序号") << QString("阶段")
		<< QString("口令") << QString("回令")
		<< QString("备注");

	this->setBaseSize(640, 480);
	InitLayout();
	initConnection();

}

FlowEditWidget::~FlowEditWidget()
{}


/**
	@brief 初始化界面布局
**/
void FlowEditWidget::InitLayout() {
	this->setWindowTitle(QString("流程编辑"));
	this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);

	leftFlowItemList = new QListWidget;

	leftTopInput = new QLineEdit;
	leftTopAdd = new QPushButton;
	leftFlowItemList->setMaximumWidth(140);
	leftTopInput->setMaximumWidth(110);
	leftTopInput->setMaximumHeight(25);
	leftTopAdd->setMaximumWidth(25);

	leftTopInput->setStyleSheet("QLineEdit{background-color:rgb(255,255,255);border-radius:2px;font: 12px 微软雅黑;}");
	leftTopAdd->setIcon(QIcon(":/flowload/images/Flow/Edit_circleplus.png"));
	leftFlowItemList->horizontalScrollBar()->hide();
	leftFlowItemList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	leftFlowItemList->setStyleSheet("\
				QListWidget::item{ background-color:rgb(232,232,232);font: 12px 微软雅黑;}\
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

	rightBottomBtnCancel = new QPushButton(QString("取消"));
	rightBottomBtnOK = new QPushButton(QString("确定"));

	rightBottomBtnOK->setStyleSheet("*{border-radius:5px;width:66px;height:34px;border-image:url(:/flowload/images/Flow/bt_normal.png);font: 12px 微软雅黑;color:white;}");
	rightBottomBtnCancel->setStyleSheet("*{border-radius:5px;width:66px;height:34px;border-image:url(:/flowload/images/Flow/bt_normal.png);font: 12px 微软雅黑;color:white;}");
	/*rightBottomBtnOK->setStyleSheet("*{border:none;width:66px;height:34px;background-image:url(:/flowload/images/Flow/button_OK.png);font: 12px 微软雅黑;}");
	rightBottomBtnCancel->setStyleSheet("*{border:none;width:66px;height:34px;background-image:url(:/flowload/images/Flow/button_cancel.png);font: 12px 微软雅黑;}");*/

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


	//初始化链表
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
	@brief 加载FlowDisplayWidget显示流程以编辑
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
	//为空流程时 
	if (mainFlowInfo_.size() == 0)
	{
		rightTable->setRowCount(1);
		mainFlowInfo_[1].push_back("");
		mainFlowInfo_[1].push_back("");
		mainFlowInfo_[1].push_back("");
		mainFlowInfo_[1].push_back("");
	}


	//获取数据表指令信息
	auto flowInfoOp = FlowInfoConfig2DB::getInstance();
	flowInfoOp->commandInfo.clear();
	flowInfoOp->readCommandDB2FlowEdit();
	//维护当前流程中已使用的指令和未使用的指令列表
	for (auto ele = flowInfoOp->commandInfo.begin(); ele != flowInfoOp->commandInfo.end(); ele++)
	{
		unUsedCmdList.push_back(QString::fromStdString(ele->second[3]));
	}

	MainFlowInfoSt* tableFlowInfoHeader1 = tableFlowInfoHeader;
	int infoRow = 0;
	for (int i = 1; i <= mainFlowInfo_.size(); i++)
	{
		//主流程ID
		int mainID = mainFlowInfo_[i][3].toInt();
		int startSpanRow = infoRow;
		//子流程
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
		//索引
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
	@brief 默认加载数据库中已有的流程 显示至左部list中
**/
void FlowEditWidget::loadDBSavedFlow() {
	//加载主页显示流程  以编辑
	auto rocketFlowInfo = FlowInfoConfig2DB::getInstance();

}

/**
	@brief 添加新流程  暂时屏蔽添加流程
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
	@brief 加载流程
**/
void FlowEditWidget::loadFlowTable() {
	rightTopFlowName->setText(rocketType + QString("-发射流程"));

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
	@brief 右键点击事件
	@param row    -
	@param column -
**/
void FlowEditWidget::tableCellClick(int row, int column) {

	if (rightTable->horizontalHeaderItem(column)->text() == "序号"
		|| rightTable->horizontalHeaderItem(column)->text() == "回令"
		|| rightTable->horizontalHeaderItem(column)->text() == "备注")
	{
		return;
	}

	QWidget* widget = new QWidget;
	widget->setObjectName("widget");
	QVBoxLayout* boxLayout = new QVBoxLayout;
	QPushButton* frontInsertCell = new QPushButton(QString("向前插入") + tableColumnName[column]);
	QPushButton* backInsertCell = new QPushButton(QString("向后插入") + tableColumnName[column]);
	QPushButton* removeCell = new QPushButton(QString("删除") + tableColumnName[column]);

	frontInsertCell->setStyleSheet("* {color:black;border:none;font: 12px 微软雅黑;}");
	backInsertCell->setStyleSheet("* {color:black;border:none;font: 12px 微软雅黑;}");
	removeCell->setStyleSheet("* {color:red;border:none;font: 12px 微软雅黑;}");
	if (tableColumnName[column] == "阶段") boxLayout->addWidget(frontInsertCell);
	boxLayout->addWidget(backInsertCell);

	boxLayout->addWidget(removeCell);
	widget->setStyleSheet(QString("*{background-color:white;border: 1px solid gray;}"));
	widget->setLayout(boxLayout);
	widget->setWindowFlags(Qt::Popup);
	widget->raise();
	widget->setFixedWidth(rightTable->columnWidth(column));

	widget->move(cursorPos.x(), cursorPos.y());
	widget->show();


	//删除  判断为第几列
	connect(removeCell, &QPushButton::clicked, this, [=]()
		{
			auto retMsg = QMessageBox::question(nullptr, QString("警告"), QString("是否确定删除该项?"), QString("取消"), QString("确定"));
			if (retMsg == 0)
			{
				return;
			}
			auto tmpHeader = tableFlowInfoHeader;

			switch (column)
			{
			case 1:
			{//删除阶段 
				//依次更新后续项，删除链表头后相应项，
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
			{//删除口令 
				//仅更新链表相应项的rowspan
				while (tmpHeader->next)
				{
					if (row >= tmpHeader->next->startRowNum && row < (tmpHeader->next->startRowNum + tmpHeader->next->rowSpanCount))
					{
						QString stageInfo = rightTable->item(tmpHeader->next->startRowNum, 1)->text();

						rightTable->removeRow(row);
						//如果删除的为阶段的第一个元素
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

	//向前插入
	connect(frontInsertCell, &QPushButton::clicked, this, [=]()
		{
			if (column == 1)
			{//插入阶段 

			}
			//修改每行的序号

		});

	//向后插入
	connect(backInsertCell, &QPushButton::clicked, this, [=]() {

		auto tmpHeader = tableFlowInfoHeader;
		if (column == 1)
		{//插入阶段
			while (tmpHeader->next)
			{
				if (tmpHeader->next->startRowNum == row)
				{
					addRow(row + tmpHeader->next->rowSpanCount);
					//rightTable->item(row + tmpHeader->next->rowSpanCount, 0)->setText(QString::number(tmpHeader->next->sortNum + 1));
					//修改后续索引
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
		{//插入口令 
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
	@brief 编辑事件
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
	@brief 右键获取位置
	@param pos -
**/
void FlowEditWidget::tableRightMouseClick(QPoint pos) {
	cursorPos = pos;
}

/**
	@brief 双击事件
	@param row    -
	@param column -
**/
void FlowEditWidget::doubleClickCell(int row, int column) {
	rightTable->editItem(rightTable->item(row, column));
}

/**
	@brief 存储信息入数据库
			这里需要根据是现有流程还是新建流程进行区分，现有流程则修改数据库，新建流程则加入数据库
**/
void FlowEditWidget::clickOKButton() {

	if (rightTable->rowCount() == 0)
	{
		//qDebug() << QString("流程为空信息");
		this->deleteLater();
		return;
	}
	if (rocketID == -1)
	{
		return;
	}
	auto flowInfoOP = FlowInfoConfig2DB::getInstance();
	//先获取相应型号的主流程和子流程
	//将流程数据库清除
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

	//查重判断
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
				QMessageBox::warning(nullptr, QString("警告"), QString("阶段名重复(行:%1),阶段:%2！").arg(r).arg(rightTable->item(stageRow, 1)->text()), QString("确定"), QString("取消"));
				return;
			}
			hadSaveStage.push_back(rightTable->item(r, 1)->text());
		}
		if (rightTable->item(r, 2)->text().isEmpty() || rightTable->item(r, 2)->text().isNull())
		{
			QMessageBox::warning(nullptr, QString("警告"), QString("指令项为空(行:%1),阶段:%2！").arg(r).arg(rightTable->item(stageRow, 1)->text()), QString("确定"), QString("取消"));
			return;
		}
		if (hadUsedCmdList.contains(rightTable->item(r, 2)->text()))
		{
			QMessageBox::warning(nullptr, QString("警告"), QString("已存在指令%1(行:%2),阶段:%3！").arg(rightTable->item(r, 2)->text()).arg(r).arg(rightTable->item(stageRow, 1)->text()), QString("确定"), QString("取消"));
			return;
		}
		hadUsedCmdList.push_back(rightTable->item(r, 2)->text());
		if (rightTable->item(r, 3)->text().isEmpty() || rightTable->item(r, 3)->text().isNull())
		{
			QMessageBox::warning(nullptr, QString("警告"), QString("指令%1中缺少回令信息(行:%2),阶段:%3！").arg(rightTable->item(r, 2)->text()).arg(r).arg(rightTable->item(stageRow, 1)->text()), QString("确定"), QString("取消"));
			return;
		}
	}


	//先删子流程再删主流程
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
		//将主流程信息写入数据库
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
				//将子流程信息写入
				flowInfoOP->subFlowConfigOp2DB(ele.first, cmdID, cmdName, backInfo, remark);
			}
		}
		tmpHeader = tmpHeader->next;
	}


	emit updateDisPlayFlow();

	this->close();
}


/**
	@brief 取消按钮事件
**/
void FlowEditWidget::clickCancelButton() {

	this->close();
}
/**
	@brief 屏蔽滚轮事件
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
	@brief 删除阶段时递归更新链表数据
	@param node -待删除的项
**/
void FlowEditWidget::recurseUpdate(MainFlowInfoSt* node, int type) {
	if (node->next)
	{
		//更新阶段
		if (type == 1)
		{
			node->next->startRowNum = node->startRowNum + node->rowSpanCount;
			recurseUpdate(node->next, 1);
			node->next->sortNum = node->sortNum;
		}
		//更新口令
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
	@brief 删除行
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
	@brief 更新序号
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
	@brief 添加一行数据
	@param rowIndex -
**/
void FlowEditWidget::addRow(int rowIndex) {
	rightTable->insertRow(rowIndex);
	for (int i = 0; i < tableColumnName.size(); i++)
	{
		rightTable->setItem(rowIndex, i, new QTableWidgetItem);
	}

}