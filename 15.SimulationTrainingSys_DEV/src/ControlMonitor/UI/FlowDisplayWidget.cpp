


#include "FlowDisplayWidget.h"


/**
	@brief  流程显示窗口
	@param  parent -
	@retval        -
**/
FlowDisplayWidget::FlowDisplayWidget(QWidget* parent, QString rocketCode, int rocketID)
	: QWidget(parent), flowName(rocketCode), rocketID(rocketID)
{

	this->setAutoFillBackground(true);

	flowEditWidget = nullptr;
	rowHeader << QString("序号") << QString("口令") << QString("操作") << QString("回令") << QString("备注");

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
	@brief 初始化界面布局
**/
void FlowDisplayWidget::InitLayout() {
	this->setAutoFillBackground(true);
	this->setStyleSheet("background-color:rgb(245,245,245);font: 14px 微软雅黑;");

	QWidget* flowLeftTopWidget = new QWidget;
	QHBoxLayout* flowLeftTopWLayout = new QHBoxLayout;
	flowIcon = new QLabel();
	flowLabel = new QLabel(QString("xxx-发射流程"));
	addFlow = new QPushButton(QString("编辑"));
	loadFlow = new QPushButton(QString("加载流程"));

	flowLeftTopWidget->setStyleSheet("*{background-color:white;max-height:30px;margin:0px 0px 0px 0px;}");
	flowIcon->setPixmap(QPixmap(":/flowload/images/Flow/icon.png"));
	//flowIcon->setFixedHeight(20);
	flowIcon->setContentsMargins(0, 0, 0, 0);
	flowLabel->setStyleSheet("font:bold 12px 微软雅黑;");
	addFlow->setStyleSheet("height:30px;background-color:white;;color:black;border-radius:4px;font:bold 12px 微软雅黑;");
	loadFlow->setStyleSheet("height:30px;background-color:rgb(30,144,255);color:white;border:1px solid rgb(130,144,255);border-radius:4px;");
	loadFlow->hide();
	/*QString labelStyle = QString("QPushButton {font:bold;border-image:url(%1);background-image:url(%1);}").arg(QString::fromLocal8Bit(":/flowload/images/Flow/指令_参数标题.png"));
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
	flowTable->horizontalHeader()->setStyleSheet("font: 14px 微软雅黑 bold;");
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

	//流程生成界面
	generateFlowWidget = new GenerateFlowCmdWidget;

	layout->addWidget(generateFlowWidget, 0, 8, 10, 8);;

	layout->setContentsMargins(20, 10, 20, 15);

	this->setLayout(layout);

	connect(addFlow, &QPushButton::clicked, this, &FlowDisplayWidget::addNewFlow);
	connect(loadFlow, &QPushButton::clicked, this, &FlowDisplayWidget::loadSavedFlow);

	connect(this, &FlowDisplayWidget::updateMainFlowAndSubFlow, generateFlowWidget, &GenerateFlowCmdWidget::responseRecieveCmd);

}

/**
	@brief 新建流程
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
	@brief 加载已有流程  显示到flowDisplay->flowTable上
**/
void FlowDisplayWidget::loadSavedFlow() {

	//FlowInfoConfig2DB::getInstance()->readUnionSearchDB2FlowEdit(QString::number(rocketID));

	flowTable->clearContents();
	mainFlowInfo.clear();
	subFlowInfo.clear();
	subFlowCmdID.clear();

	flowLabel->setText(QString("%1-发射流程").arg(flowName));

	flowTable->setHorizontalHeaderLabels(rowHeader);
	//从数据库中加载流程
	auto flowInfoOp = FlowInfoConfig2DB::getInstance();

	//flowInfoOp->readMainFlowDB2FlowEdit();
	//flowInfoOp->readSubFlowDB2FlowEdit();


	//// 属于同一火箭ID  索引先后  mainFlowInfo:键：索引  值：流程名  返令信息  备注  main_ID
	//for (auto ele = flowInfoOp->mainFlowInfo.begin(); ele != flowInfoOp->mainFlowInfo.end(); ele++)
	//{
	//	if (QString::fromStdString(ele->second[1]).toInt() != rocketID)
	//		continue;
	//	mainFlowInfo[QString::fromStdString(ele->second[3]).toInt()].push_back(QString::fromStdString(ele->second[2]));
	//	mainFlowInfo[QString::fromStdString(ele->second[3]).toInt()].push_back(QString::fromStdString(ele->second[4]));
	//	mainFlowInfo[QString::fromStdString(ele->second[3]).toInt()].push_back(QString::fromStdString(ele->second[5]));
	//	mainFlowInfo[QString::fromStdString(ele->second[3]).toInt()].push_back(QString::fromStdString(ele->second[0]));
	//}

	////根据main_ID  subFlowInfo键：main_ID  值:指令名称 
	////subFlowCmdID  键:main_ID  值:指令ID
	//for (auto ele = flowInfoOp->subFlowInfo.begin(); ele != flowInfoOp->subFlowInfo.end(); ele++)
	//{

	//	subFlowInfo[QString::fromStdString(ele->second[1]).toInt()].push_back(QString::fromStdString(ele->second[3]));
	//	subFlowCmdID[QString::fromStdString(ele->second[1]).toInt()].push_back(QString::fromStdString(ele->second[2]).toInt());
	//}


	//查询主流程对应子流程信息
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

	//子流程ID 主流程name 主流程index  主流程返令信息 主流程备注 主流程ID 子流程name 子流程指令ID
	for (auto ele = flowDBOp->customSearchInfo.begin(); ele != flowDBOp->customSearchInfo.end(); ele++)
	{
		mainFlowInfo[atoi(ele->second[2].c_str())].push_back(QString::fromStdString(ele->second[1]));
		mainFlowInfo[atoi(ele->second[2].c_str())].push_back(QString::fromStdString(ele->second[3]));
		mainFlowInfo[atoi(ele->second[2].c_str())].push_back(QString::fromStdString(ele->second[4]));
		mainFlowInfo[atoi(ele->second[2].c_str())].push_back(QString::fromStdString(ele->second[5]));
		//主流程索引到子流程ID
		mainID2SubID[atoi(ele->second[2].c_str())].push_back(atoi(ele->second[0].c_str()));
		//主流程ID到子流程名称
		subFlowInfo[atoi(ele->second[5].c_str())].push_back(ele->second[6].c_str());
		subFlowCmdID[atoi(ele->second[5].c_str())].push_back(atoi(ele->second[7].c_str()));
	}

	/*!
	 *  Loads the saved flow.加载至流程显示
	 */
	flowTable->setRowCount(mainFlowInfo.size());
	for (int i = 1; i <= mainFlowInfo.size(); i++)
	{
		//flowTable->insertRow(i - 1);
		int column1 = 0;
		//索引
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
	@brief 根据接收得到的icode和sendcode更新流程信息
	@param m_iCode   -当前执行指令的回令
	@param sendICode -发送指令方，根据它执行当前执行的指令
**/
void FlowDisplayWidget::updateFlowStat(int m_iCode, int sendICode) {
	//1.获得主流程和子流程
	//2.根据主流程和子流程关系（子流程执行反向得到主流程，假如子流程到当前主流程的最后一步时，切换至下一个主流程，然后依次递归）

		//查询主流程对应子流程信息
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

	//子流程ID 主流程name 主流程index  主流程返令信息 主流程备注 主流程ID 子流程name 子流程指令ID
	for (auto ele = flowDBOp->customSearchInfo.begin(); ele != flowDBOp->customSearchInfo.end(); ele++)
	{
		mainFlowInfo[atoi(ele->second[2].c_str())].push_back(QString::fromStdString(ele->second[1]));
		mainFlowInfo[atoi(ele->second[2].c_str())].push_back(QString::fromStdString(ele->second[3]));
		mainFlowInfo[atoi(ele->second[2].c_str())].push_back(QString::fromStdString(ele->second[4]));
		mainFlowInfo[atoi(ele->second[2].c_str())].push_back(QString::fromStdString(ele->second[5]));
		//主流程索引到子流程ID
		mainID2SubID[atoi(ele->second[2].c_str())].push_back(atoi(ele->second[0].c_str()));
		//主流程ID到子流程名称
		subFlowInfo[atoi(ele->second[5].c_str())].push_back(ele->second[6].c_str());
		subFlowCmdID[atoi(ele->second[5].c_str())].push_back(atoi(ele->second[7].c_str()));
	}


	//查询指令对应表
	flowDBOp->readCommandDB2FlowEdit();
	int backCmdID, sendCmdID = -1;
	//找到接收回令ID和发送指令ID
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
	//根据发送指令ID得到子流程当前执行指令name 主流程的索引
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