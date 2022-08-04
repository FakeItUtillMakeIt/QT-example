


#include "FlowDisplayWidget.h"


/**
	@brief  ������ʾ����
	@param  parent -
	@retval        -
**/
FlowDisplayWidget::FlowDisplayWidget(QWidget* parent, QString rocketCode, int rocketID)
	: QWidget(parent), flowName(rocketCode), rocketID(rocketID)
{

	this->setAutoFillBackground(true);

	flowEditWidget = nullptr;
	rowHeader << QString("���") << QString("����") << QString("����") << QString("����") << QString("��ע");

	InitLayout();
	loadSavedFlow();
}

FlowDisplayWidget::~FlowDisplayWidget()
{
	if (FlowEditWidget::instance != nullptr)
	{
		FlowEditWidget::getInstance()->close();
	}

}


void FlowDisplayWidget::setRocketType(QString title, int id) {
	flowName = title;
	rocketID = id;
}

/**
	@brief ��ʼ�����沼��
**/
void FlowDisplayWidget::InitLayout() {
	this->setAutoFillBackground(true);
	this->setStyleSheet("background-color:rgb(245,245,245);font: 14px ΢���ź�;");

	QWidget* flowLeftTopWidget = new QWidget;
	QHBoxLayout* flowLeftTopWLayout = new QHBoxLayout;
	flowIcon = new QLabel();
	flowLabel = new QLabel(QString("xxx-��������"));
	addFlow = new QPushButton(QString("�༭"));
	loadFlow = new QPushButton(QString("��������"));

	flowLeftTopWidget->setStyleSheet("*{background-color:white;max-height:30px;margin:0px 0px 0px 0px;}");
	flowIcon->setPixmap(QPixmap(":/flowload/images/Flow/icon.png"));
	//flowIcon->setFixedHeight(20);
	flowIcon->setContentsMargins(0, 0, 0, 0);
	flowLabel->setStyleSheet("font:bold 12px ΢���ź�;");
	addFlow->setStyleSheet("height:30px;background-color:white;;color:black;border-radius:4px;font:bold 12px ΢���ź�;");
	loadFlow->setStyleSheet("height:30px;background-color:rgb(30,144,255);color:white;border:1px solid rgb(130,144,255);border-radius:4px;");
	loadFlow->hide();
	/*QString labelStyle = QString("QPushButton {font:bold;border-image:url(%1);background-image:url(%1);}").arg(QString::fromLocal8Bit(":/flowload/images/Flow/ָ��_��������.png"));
	addFlow->setStyleSheet(labelStyle);
	loadFlow->setStyleSheet(labelStyle);*/

	flowTable = new QTableWidget(this);
	flowTable->setColumnCount(rowHeader.size());
	flowTable->setHorizontalHeaderLabels(rowHeader);
	flowTable->verticalHeader()->hide();

	int columnWidth = (this->parentWidget()->width()) / rowHeader.size();

	for (int i = 0; i < rowHeader.size(); i++)
	{
		flowTable->setColumnWidth(i, columnWidth);
	}

	flowTable->horizontalHeader()->setMinimumHeight(40);
	flowTable->horizontalHeader()->setStyleSheet("font: 14px ΢���ź� bold;");
	flowTable->horizontalHeader()->setStretchLastSection(true);
	flowTable->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);

	QGridLayout* layout = new QGridLayout;

	int columnCount = 8;
	int rowCount = 10;

	/*layout->addWidget(flowIcon, 0, 0, 1, 1);
	layout->addWidget(flowLabel, 0, 1, 1, 1);

	layout->setColumnStretch(1, 7);
	layout->addWidget(addFlow, 0, columnCount - 2, 1, 1);
	layout->addWidget(loadFlow, 0, columnCount - 1, 1, 1);*/

	flowLeftTopWLayout->addWidget(flowIcon, Qt::LeftToRight);
	flowLeftTopWLayout->addWidget(flowLabel);
	flowLeftTopWLayout->addStretch(columnCount);
	flowLeftTopWLayout->addWidget(addFlow, Qt::RightToLeft);

	flowLeftTopWidget->setLayout(flowLeftTopWLayout);

	layout->addWidget(flowLeftTopWidget, 0, 0, 1, columnCount);
	layout->addWidget(flowTable, 1, 0, rowCount - 1, columnCount);

	//�������ɽ���
	generateFlowWidget = new GenerateFlowCmdWidget;

	layout->addWidget(generateFlowWidget, 0, 8, 10, 8);;

	layout->setContentsMargins(20, 10, 20, 15);

	this->setLayout(layout);

	connect(addFlow, &QPushButton::clicked, this, &FlowDisplayWidget::addNewFlow);
	connect(loadFlow, &QPushButton::clicked, this, &FlowDisplayWidget::loadSavedFlow);

	connect(this, &FlowDisplayWidget::updateMainFlowAndSubFlow, generateFlowWidget, &GenerateFlowCmdWidget::responseRecieveCmd);

}

/**
	@brief �½�����
**/
void FlowDisplayWidget::addNewFlow() {

	if (flowEditWidget == nullptr)
	{
		flowEditWidget = FlowEditWidget::getInstance();

		flowEditWidget->setRocketType(flowName, rocketID);
	}

	flowEditWidget->setMainFlowInfo(mainFlowInfo);
	flowEditWidget->setSubFlowInfo(subFlowInfo);
	flowEditWidget->setFlowCmdID(subFlowCmdID);

	flowEditWidget->setFixedSize(QSize(this->width() / 2, this->width() * 0.3));
	flowEditWidget->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
	flowEditWidget->show();

	connect(flowEditWidget, &FlowEditWidget::updateDisPlayFlow, this, &FlowDisplayWidget::loadSavedFlow);

}

/**
	@brief ������������  ��ʾ��flowDisplay->flowTable��
**/
void FlowDisplayWidget::loadSavedFlow() {

	//FlowInfoConfig2DB::getInstance()->readUnionSearchDB2FlowEdit(QString::number(rocketID));

	flowTable->clearContents();
	mainFlowInfo.clear();
	subFlowInfo.clear();
	subFlowCmdID.clear();

	flowLabel->setText(QString("%1-��������").arg(flowName));

	flowTable->setHorizontalHeaderLabels(rowHeader);
	//�����ݿ��м�������
	auto flowInfoOp = FlowInfoConfig2DB::getInstance();

	//flowInfoOp->readMainFlowDB2FlowEdit();
	//flowInfoOp->readSubFlowDB2FlowEdit();


	//// ����ͬһ���ID  �����Ⱥ�  mainFlowInfo:��������  ֵ��������  ������Ϣ  ��ע  main_ID
	//for (auto ele = flowInfoOp->mainFlowInfo.begin(); ele != flowInfoOp->mainFlowInfo.end(); ele++)
	//{
	//	if (QString::fromStdString(ele->second[1]).toInt() != rocketID)
	//		continue;
	//	mainFlowInfo[QString::fromStdString(ele->second[3]).toInt()].push_back(QString::fromStdString(ele->second[2]));
	//	mainFlowInfo[QString::fromStdString(ele->second[3]).toInt()].push_back(QString::fromStdString(ele->second[4]));
	//	mainFlowInfo[QString::fromStdString(ele->second[3]).toInt()].push_back(QString::fromStdString(ele->second[5]));
	//	mainFlowInfo[QString::fromStdString(ele->second[3]).toInt()].push_back(QString::fromStdString(ele->second[0]));
	//}

	////����main_ID  subFlowInfo����main_ID  ֵ:ָ������ 
	////subFlowCmdID  ��:main_ID  ֵ:ָ��ID
	//for (auto ele = flowInfoOp->subFlowInfo.begin(); ele != flowInfoOp->subFlowInfo.end(); ele++)
	//{

	//	subFlowInfo[QString::fromStdString(ele->second[1]).toInt()].push_back(QString::fromStdString(ele->second[3]));
	//	subFlowCmdID[QString::fromStdString(ele->second[1]).toInt()].push_back(QString::fromStdString(ele->second[2]).toInt());
	//}


	//��ѯ�����̶�Ӧ��������Ϣ
	QString qSqlString = "SELECT\
		sub_flow_info.id,\
		main_flow_info.`name`,\
		main_flow_info.`index`,\
		main_flow_info.backInfo,\
		main_flow_info.remark,\
		sub_flow_info.main_id,\
		sub_flow_info.`name`,\
		sub_flow_info.command_id\
		FROM\
		main_flow_info\
		INNER JOIN sub_flow_info ON sub_flow_info.main_id = main_flow_info.id\
		WHERE\
		main_flow_info.rocket_id = %1; ";

	qSqlString = qSqlString.arg(rocketID);
	auto flowDBOp = FlowInfoConfig2DB::getInstance();
	flowDBOp->customDBQuery(qSqlString);
	mainFlowInfo.clear();
	subFlowInfo.clear();
	subFlowCmdID.clear();

	//������ID ������name ������index  �����̷�����Ϣ �����̱�ע ������ID ������name ������ָ��ID
	for (auto ele = flowDBOp->customSearchInfo.begin(); ele != flowDBOp->customSearchInfo.end(); ele++)
	{
		mainFlowInfo[atoi(ele->second[2].c_str())].push_back(QString::fromStdString(ele->second[1]));
		mainFlowInfo[atoi(ele->second[2].c_str())].push_back(QString::fromStdString(ele->second[3]));
		mainFlowInfo[atoi(ele->second[2].c_str())].push_back(QString::fromStdString(ele->second[4]));
		mainFlowInfo[atoi(ele->second[2].c_str())].push_back(QString::fromStdString(ele->second[5]));
		//������������������ID
		mainID2SubID[atoi(ele->second[2].c_str())].push_back(atoi(ele->second[0].c_str()));
		//������ID������������
		subFlowInfo[atoi(ele->second[5].c_str())].push_back(ele->second[6].c_str());
		subFlowCmdID[atoi(ele->second[5].c_str())].push_back(atoi(ele->second[7].c_str()));
	}

	/*!
	 *  Loads the saved flow.������������ʾ
	 */
	flowTable->setRowCount(mainFlowInfo.size());
	for (int i = 1; i <= mainFlowInfo.size(); i++)
	{
		//flowTable->insertRow(i - 1);
		int column1 = 0;
		//����
		flowTable->setItem(i - 1, column1++, new QTableWidgetItem(QString::number(i)));
		flowTable->setItem(i - 1, column1++, new QTableWidgetItem(mainFlowInfo[i][0]));
		int mainID = mainFlowInfo[i][3].toInt();

		auto valueV = subFlowInfo.value(mainID);
		QWidget* cellWidg = new QWidget;
		QVBoxLayout* boxLayout = new QVBoxLayout;
		int cmdCount = 1;
		for (auto val : valueV)
		{
			cmdCount++;
			boxLayout->addWidget(new QLabel(val));
		}

		cellWidg->setLayout(boxLayout);
		flowTable->setRowHeight(i - 1, cmdCount * 40);
		flowTable->setCellWidget(i - 1, column1++, cellWidg);
		flowTable->setItem(i - 1, column1++, new QTableWidgetItem(mainFlowInfo[i][1]));
		flowTable->setItem(i - 1, column1++, new QTableWidgetItem(mainFlowInfo[i][2]));
	}

	if (generateFlowWidget == nullptr)
	{
		return;

	}
	generateFlowWidget->setMainFlowInfo(mainFlowInfo);
	generateFlowWidget->setSubFlowInfo(subFlowInfo);
	generateFlowWidget->setFlowCmdID(subFlowCmdID);

}

/**
	@brief ���ݽ��յõ���icode��sendcode����������Ϣ
	@param m_iCode   -��ǰִ��ָ��Ļ���
	@param sendICode -����ָ���������ִ�е�ǰִ�е�ָ��
**/
void FlowDisplayWidget::updateFlowStat(int m_iCode, int sendICode) {
	//1.��������̺�������
	//2.���������̺������̹�ϵ��������ִ�з���õ������̣����������̵���ǰ�����̵����һ��ʱ���л�����һ�������̣�Ȼ�����εݹ飩

		//��ѯ�����̶�Ӧ��������Ϣ
	QString qSqlString = "SELECT\
		sub_flow_info.id,\
		main_flow_info.`name`,\
		main_flow_info.`index`,\
		main_flow_info.backInfo,\
		main_flow_info.remark,\
		sub_flow_info.main_id,\
		sub_flow_info.`name`,\
		sub_flow_info.command_id\
		FROM\
		main_flow_info\
		INNER JOIN sub_flow_info ON sub_flow_info.main_id = main_flow_info.id\
		WHERE\
		main_flow_info.rocket_id = %1; ";

	qSqlString = qSqlString.arg(rocketID);
	auto flowDBOp = FlowInfoConfig2DB::getInstance();
	flowDBOp->customDBQuery(qSqlString);
	mainFlowInfo.clear();
	subFlowInfo.clear();
	subFlowCmdID.clear();

	//������ID ������name ������index  �����̷�����Ϣ �����̱�ע ������ID ������name ������ָ��ID
	for (auto ele = flowDBOp->customSearchInfo.begin(); ele != flowDBOp->customSearchInfo.end(); ele++)
	{
		mainFlowInfo[atoi(ele->second[2].c_str())].push_back(QString::fromStdString(ele->second[1]));
		mainFlowInfo[atoi(ele->second[2].c_str())].push_back(QString::fromStdString(ele->second[3]));
		mainFlowInfo[atoi(ele->second[2].c_str())].push_back(QString::fromStdString(ele->second[4]));
		mainFlowInfo[atoi(ele->second[2].c_str())].push_back(QString::fromStdString(ele->second[5]));
		//������������������ID
		mainID2SubID[atoi(ele->second[2].c_str())].push_back(atoi(ele->second[0].c_str()));
		//������ID������������
		subFlowInfo[atoi(ele->second[5].c_str())].push_back(ele->second[6].c_str());
		subFlowCmdID[atoi(ele->second[5].c_str())].push_back(atoi(ele->second[7].c_str()));
	}


	//��ѯָ���Ӧ��
	flowDBOp->readCommandDB2FlowEdit();
	int backCmdID, sendCmdID = -1;
	//�ҵ����ջ���ID�ͷ���ָ��ID
	for (auto ele = flowDBOp->commandInfo.begin(); ele != flowDBOp->commandInfo.end(); ele++)
	{
		if (m_iCode == atoi(ele->second[4].c_str()))
		{
			backCmdID = ele->first;
		}
		if (sendICode == atoi(ele->second[4].c_str()))
		{
			sendCmdID = ele->first;
		}
	}
	if (sendCmdID == -1)
	{
		return;
	}
	//���ݷ���ָ��ID�õ������̵�ǰִ��ָ��name �����̵�����
	QString curRunCmdName;
	int mainFlowIndex;
	int subFlowID;
	QString backCmdInfo;
	for (auto ele = flowDBOp->customSearchInfo.begin(); ele != flowDBOp->customSearchInfo.end(); ele++)
	{
		if (atoi(ele->second[7].c_str()) == sendCmdID)
		{
			curRunCmdName = QString::fromStdString(ele->second[6]);
			mainFlowIndex = atoi(ele->second[2].c_str());
			subFlowID = atoi(ele->second[0].c_str());
			backCmdInfo = QString::fromStdString(ele->second[3].c_str());
		}
	}

	emit updateMainFlowAndSubFlow(mainFlowIndex, curRunCmdName, backCmdInfo);

}