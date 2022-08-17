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
	rightTopD3->setPixmap(QPixmap(":/ControlMonitor/images/Flow/update.png"));
	rightTopD4->setPixmap(QPixmap(":/ControlMonitor/images/Flow/sort.png"));

	rightTable->verticalHeader()->hide();
	rightTable->horizontalHeader()->show();


	rightBottomBtnCancel = new QPushButton(QString(""));
	rightBottomBtnOK = new QPushButton(QString(""));

	rightBottomBtnOK->setStyleSheet("*{border:none;width:66px;height:34px;background-image:url(:/flowload/images/Flow/button_OK.png);font: 12px ΢���ź�;}");
	rightBottomBtnCancel->setStyleSheet("*{border:none;width:66px;height:34px;background-image:url(:/flowload/images/Flow/button_cancel.png);font: 12px ΢���ź�;}");

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


	//��ʼ��ʱ�����ݿ������������

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
	else
	{
		rightTable->setRowCount(mainFlowInfo_.size());
	}

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	//��ȡ���ݱ�ָ����Ϣ
	auto flowInfoOp = FlowInfoConfig2DB::getInstance();
	flowInfoOp->commandInfo.clear();
	flowInfoOp->readCommandDB2FlowEdit();
	//ά����ǰ��������ʹ�õ�ָ���δʹ�õ�ָ���б�
	for (auto ele = flowInfoOp->commandInfo.begin(); ele != flowInfoOp->commandInfo.end(); ele++)
	{
		unUsedCmdList.push_back(QString::fromStdString(ele->second[3]));
	}

	for (int i = 1; i <= mainFlowInfo_.size(); i++)
	{
		int c = 0;
		rightTable->setItem(i - 1, c++, new QTableWidgetItem(QString::number(i)));//���
		rightTable->setItem(i - 1, c++, new QTableWidgetItem(mainFlowInfo_[i][0]));//�׶�
		//�����̿���
		QWidget* newWidget = new QWidget;
		newWidget->setMouseTracking(false);
		QVBoxLayout* newVbox = new QVBoxLayout;
		auto cmdInfoList = subFlowInfo_[mainFlowInfo_[i][3].toInt()];
		for (auto cmdInfo : cmdInfoList)
		{
			QComboBox* combo1 = new QComboBox;
			cmdComboBoxList.push_back(combo1);
			for (auto ele = flowInfoOp->commandInfo.begin(); ele != flowInfoOp->commandInfo.end(); ele++)
			{
				combo1->addItem(QString::fromStdString(ele->second[3]), QString::fromStdString(ele->second[0]).toInt());
			}
			combo1->setCurrentText(cmdInfo);
			newVbox->addWidget(combo1);
		}
		newWidget->setLayout(newVbox);
		rightTable->setCellWidget(i - 1, c++, newWidget);


		//�����̻���
		newWidget = new QWidget;
		newWidget->setMouseTracking(false);
		newVbox = new QVBoxLayout;
		for (auto cmdInfo : subFlowInfo1_[mainFlowInfo_[i][3].toInt()])
		{
			newVbox->addWidget(new QLineEdit(cmdInfo));
		}
		newWidget->setLayout(newVbox);
		rightTable->setCellWidget(i - 1, c++, newWidget);


		//�����̱�ע
		newWidget = new QWidget;
		newWidget->setMouseTracking(false);
		newVbox = new QVBoxLayout;
		for (auto cmdInfo : subFlowInfo2_[mainFlowInfo_[i][3].toInt()])
		{
			newVbox->addWidget(new QLineEdit(cmdInfo));
		}
		newWidget->setLayout(newVbox);
		rightTable->setCellWidget(i - 1, c++, newWidget);
		int size = cmdInfoList.size() == 0 ? 1 : cmdInfoList.size();
		rightTable->setRowHeight(i - 1, 40 * size);
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
	widget->setStyleSheet(QString("*{background-color:white;border: 1px  gray;}"));
	widget->setLayout(boxLayout);
	widget->setWindowFlags(Qt::Popup);
	widget->raise();
	widget->setFixedWidth(rightTable->columnWidth(column));

	widget->move(cursorPos.x(), cursorPos.y());
	widget->show();

	//ɾ��  �ж�Ϊ�ڼ���
	connect(removeCell, &QPushButton::clicked, this, [=]()
		{
			switch (column)
			{
			case 1:
			{//ɾ���׶� 
				rightTable->removeRow(row);
				//�޸�ÿ�е����
				for (int r = 0; r < rightTable->rowCount(); r++)
				{
					rightTable->item(r, 0)->setText(QString::number(r + 1));

				}
				widget->deleteLater();
				break;
			}
			case 2:
			{//ɾ������ 
				QList<QComboBox*> allCombo = rightTable->cellWidget(row, column)->findChildren<QComboBox*>();
				QList<QLineEdit*> allBackInfo = rightTable->cellWidget(row, column + 1)->findChildren<QLineEdit*>();
				QList<QLineEdit*> allRemark = rightTable->cellWidget(row, column + 2)->findChildren<QLineEdit*>();
				if (allCombo.size() <= 0)
				{
					QMessageBox::warning(this, QString("����"), QString("�ý׶�û�п��"));
					return;
				}
				QComboBox* combox = allCombo.last();
				rightTable->cellWidget(row, column)->layout()->removeWidget(combox);
				delete combox;

				rightTable->cellWidget(row, column)->layout();

				QLineEdit* backInfo = allBackInfo.last();
				rightTable->cellWidget(row, column + 1)->layout()->removeWidget(backInfo);
				delete backInfo;

				QLineEdit* ramark = allRemark.last();
				rightTable->cellWidget(row, column + 2)->layout()->removeWidget(ramark);
				delete ramark;

				int size = allCombo.size() - 1 < 2 ? 1 : allCombo.size() - 1;
				rightTable->setRowHeight(row, 40 * size);
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
				rightTable->insertRow(row);
				for (int i = 0; i < tableColumnName.size(); i++)
				{
					if (i == 2 || i == 3 || i == 4)
					{
						QWidget* newWidget = new QWidget;
						QVBoxLayout* newVbox = new QVBoxLayout;
						newWidget->setLayout(newVbox);
						rightTable->setCellWidget(row, i, newWidget);
						continue;
					}
					rightTable->setItem(row, i, new QTableWidgetItem);
				}
			}
			//�޸�ÿ�е����
			for (int r = 0; r < rightTable->rowCount(); r++)
			{
				rightTable->item(r, 0)->setText(QString::number(r + 1));
			}
			widget->deleteLater();
		});

	//������
	connect(backInsertCell, &QPushButton::clicked, this, [=]() {
		if (column == 1)
		{//����׶�
			rightTable->insertRow(row + 1);
			for (int i = 0; i < tableColumnName.size(); i++)
			{
				if (i == 2 || i == 3 || i == 4)
				{
					QWidget* newWidget = new QWidget;
					QVBoxLayout* newVbox = new QVBoxLayout;
					newWidget->setLayout(newVbox);
					rightTable->setCellWidget(row + 1, i, newWidget);
					continue;
				}
				rightTable->setItem(row + 1, i, new QTableWidgetItem);

			}
		}
		if (column == 2)
		{//������� 
			//�������
			QComboBox* newcombox = new QComboBox;
			cmdComboBoxList.push_back(newcombox);
			//��ȡ���ݱ�ָ����Ϣ
			auto flowInfoOp = FlowInfoConfig2DB::getInstance();
			flowInfoOp->commandInfo.clear();
			flowInfoOp->readCommandDB2FlowEdit();
			for (auto ele = flowInfoOp->commandInfo.begin(); ele != flowInfoOp->commandInfo.end(); ele++)
			{
				newcombox->addItem(QString::fromStdString(ele->second[3]), QString::fromStdString(ele->second[0]).toInt());
			}
			auto cellW = rightTable->cellWidget(row, 2);
			cellW->layout()->addWidget(newcombox);
			int wCount = cellW->children().count();

			//������� 
			cellW = rightTable->cellWidget(row, 3);
			cellW->layout()->addWidget(new QLineEdit());

			//���뱸ע 
			cellW = rightTable->cellWidget(row, 4);
			cellW->layout()->addWidget(new QLineEdit());

			rightTable->setRowHeight(row, 30 * wCount);
		}

		//�޸�ÿ�����
		for (int r = 0; r < rightTable->rowCount(); r++)
		{
			rightTable->item(r, 0)->setText(QString::number(r + 1));
		}

		widget->deleteLater();
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
	for (int r = 0; r < rightTable->rowCount(); r++)
	{
		QList<QComboBox*> allCombo = rightTable->cellWidget(r, 2)->findChildren<QComboBox*>();
		for (auto obj : allCombo)
		{
			QString cmdName = obj->currentText();
			if (hadUsedCmdList.contains(cmdName))
			{
				QMessageBox::warning(this, QString("����"), QString("�����̴��ڷ�Ψһ���������ģ�"));
				return;
			}
			hadUsedCmdList.push_back(cmdName);
		}
	}

	//��ɾ��������ɾ������
	flowInfoOP->clearSubFlowDB(v);
	flowInfoOP->clearMainFlowDB(rocketID);

	for (int r = 0; r < rightTable->rowCount(); r++)
	{
		QString mainFlowName;
		QString mainFlowIndex;
		QString backInfo;
		QString remark;

		int mainFlowID = 0;
		QString subFlowName;
		int emitCmdId;

		mainFlowName = rightTable->item(r, 1)->text();
		mainFlowIndex = rightTable->item(r, 0)->text();
		backInfo = ""; //rightTable->item(r, 3)->text();
		remark = ""; //rightTable->item(r, 4)->text();

		//����������Ϣд�����ݿ�
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
		QList<QLineEdit*> allSubBackInfo = rightTable->cellWidget(r, 3)->findChildren<QLineEdit*>();
		QList<QLineEdit*> allSubRemark = rightTable->cellWidget(r, 4)->findChildren<QLineEdit*>();
		int count = 0;
		for (auto obj : allCombo)
		{
			QString cmdName;
			int cmdID;
			QString backInfo;
			QString remark;

			cmdName = obj->currentText();
			cmdID = obj->currentData().toInt();

			backInfo = allSubBackInfo[count]->text();
			remark = allSubRemark[count]->text();
			//����������Ϣд��
			flowInfoOP->subFlowConfigOp2DB(mainFlowID, cmdID, cmdName, backInfo, remark);
			count++;
		}
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