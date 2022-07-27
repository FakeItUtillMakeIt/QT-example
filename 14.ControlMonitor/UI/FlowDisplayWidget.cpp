#ifndef FLOW_DISPLAY_WIDGET_H
#define FLOW_DISPLAY_WIDGET_H


#include "FlowDisplayWidget.h"


/**
	@brief  流程显示窗口
	@param  parent -
	@retval        -
**/
FlowDisplayWidget::FlowDisplayWidget(QWidget* parent)
	: QWidget(parent)
{

	flowEditWidget = nullptr;
	rowHeader << QString("序号") << QString("口令") << QString("操作") << QString("回令") << QString("备注");

	InitLayout();

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
	this->setStyleSheet("background-color:rgb(245,245,245);");

	flowIcon = new QLabel();
	flowLabel = new QLabel(QString("xxx-发射流程"));
	addFlow = new QPushButton(QString("新建流程"));
	loadFlow = new QPushButton(QString("加载流程"));

	flowIcon->setPixmap(QPixmap(":/ControlMonitor/images/Flow/flow.png"));
	flowIcon->setFixedHeight(20);
	addFlow->setStyleSheet("height:30px;background-color:rgb(30,144,255);color:white;border:1px solid rgb(130,144,255);border-radius:4px;");
	loadFlow->setStyleSheet("height:30px;background-color:rgb(30,144,255);color:white;border:1px solid rgb(130,144,255);border-radius:4px;");
	/*QString labelStyle = QString("QPushButton {font:bold;border-image:url(%1);background-image:url(%1);}").arg(QString::fromLocal8Bit(":/flowload/images/Flow/指令_参数标题.png"));
	addFlow->setStyleSheet(labelStyle);
	loadFlow->setStyleSheet(labelStyle);*/

	flowTable = new QTableWidget(this);
	flowTable->setColumnCount(rowHeader.size());
	flowTable->setHorizontalHeaderLabels(rowHeader);
	flowTable->verticalHeader()->hide();

	int columnWidth = (this->parentWidget()->width() / 2 - 100) / rowHeader.size();

	for (int i = 0; i < rowHeader.size(); i++)
	{
		flowTable->setColumnWidth(i, columnWidth);
	}

	flowTable->horizontalHeader()->setMinimumHeight(40);
	flowTable->horizontalHeader()->setStyleSheet("font:bold;");
	flowTable->horizontalHeader()->setStretchLastSection(true);
	flowTable->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);

	QGridLayout* layout = new QGridLayout;

	int columnCount = 8;
	int rowCount = 10;

	layout->addWidget(flowIcon, 0, 0, 1, 1);
	layout->addWidget(flowLabel, 0, 1, 1, 1);
	layout->setColumnStretch(1, 7);
	layout->addWidget(addFlow, 0, columnCount - 2, 1, 1);
	layout->addWidget(loadFlow, 0, columnCount - 1, 1, 1);
	layout->addWidget(flowTable, 1, 0, rowCount - 1, columnCount);

	//流程生成界面
	generateFlowWidget = new GenerateFlowCmdWidget;

	layout->addWidget(generateFlowWidget, 0, 8, 10, 8);;

	this->setLayout(layout);

	connect(addFlow, &QPushButton::clicked, this, &FlowDisplayWidget::addNewFlow);
	connect(loadFlow, &QPushButton::clicked, this, &FlowDisplayWidget::loadSavedFlow);

}

/**
	@brief 新建流程
**/
void FlowDisplayWidget::addNewFlow() {

	if (flowEditWidget == nullptr)
	{
		flowEditWidget = FlowEditWidget::getInstance();
		flowEditWidget->setRocketType(QString("Ling"));
	}

	flowEditWidget->setFixedSize(QSize(this->width() / 2, this->width() * 0.3));
	flowEditWidget->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
	flowEditWidget->show();

}

/**
	@brief 加载已有流程  显示到flowDisplay->flowTable上
**/
void FlowDisplayWidget::loadSavedFlow() {

	flowTable->clearContents();
	mainFlowInfo.clear();
	subFlowInfo.clear();
	subFlowCmdID.clear();

	flowLabel->setText(QString("%1-发射流程").arg(flowName));

	flowTable->setHorizontalHeaderLabels(rowHeader);
	//从数据库中加载流程
	auto flowInfoOp = FlowInfoConfig2DB::getInstance();

	flowInfoOp->readMainFlowDB2FlowEdit();
	flowInfoOp->readSubFlowDB2FlowEdit();



	// 属于同一火箭ID  索引先后  mainFlowInfo:键：索引  值：流程名  返令信息  备注  main_ID
	for (auto ele = flowInfoOp->mainFlowInfo.begin(); ele != flowInfoOp->mainFlowInfo.end(); ele++)
	{
		if (QString::fromStdString(ele->second[1]).toInt() != rocketID)
			continue;
		mainFlowInfo[QString::fromStdString(ele->second[3]).toInt()].push_back(QString::fromStdString(ele->second[2]));
		mainFlowInfo[QString::fromStdString(ele->second[3]).toInt()].push_back(QString::fromStdString(ele->second[4]));
		mainFlowInfo[QString::fromStdString(ele->second[3]).toInt()].push_back(QString::fromStdString(ele->second[5]));
		mainFlowInfo[QString::fromStdString(ele->second[3]).toInt()].push_back(QString::fromStdString(ele->second[0]));
	}

	//根据main_ID  subFlowInfo键：main_ID  值:指令名称 
	//subFlowCmdID  键:main_ID  值:指令ID
	for (auto ele = flowInfoOp->subFlowInfo.begin(); ele != flowInfoOp->subFlowInfo.end(); ele++)
	{

		subFlowInfo[QString::fromStdString(ele->second[1]).toInt()].push_back(QString::fromStdString(ele->second[3]));
		subFlowCmdID[QString::fromStdString(ele->second[1]).toInt()].push_back(QString::fromStdString(ele->second[2]).toInt());
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

#endif