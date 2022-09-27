//#pragma execution_character_set("utf-8")
#include "FlowDisplayWidget.h"
#include "..\..\Public\Utils.h"
#define ONEROWHEIGHT 35

/**
	@brief  流程显示窗口
	@param  parent -
	@retval        -
**/
FlowDisplayWidget::FlowDisplayWidget(QWidget* parent, QString rocketCode, int rocketID)
	: QWidget(parent), flowName(rocketCode), rocketID(rocketID)
{

	this->setAutoFillBackground(true);
	m_app = AppCache::instance();
	flowEditWidget = nullptr;
	rowHeader << QString("序号") << QString("阶段") << QString("口令") << QString("回令") << QString("备注");

	InitLayout();
	//loadSavedFlow();
}

FlowDisplayWidget::~FlowDisplayWidget()
{
	if (FlowEditWidget::instance != nullptr)
	{
		FlowEditWidget::getInstance()->close();
	}

}

/*!
 *  设置火箭型号.
 *
 *      @param [in] title
 *      @param [in] id
 */
void FlowDisplayWidget::setRocketType(QString title, int id) {
	flowName = title;
	rocketID = id;
	//InitLayout();
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
	//QString info = QString::fromStdString(Utils::GBKToUTF8(m_app->m_CurrentRocketType->m_name.c_str()));
	flowLabel = new QLabel(QString("%1-发射流程").arg(flowName));
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

	//int columnWidth = (this->parentWidget()->width()) / rowHeader.size();

	//for (int i = 0; i < rowHeader.size(); i++)
	//{
	//	flowTable->setColumnWidth(i, columnWidth);
	//}
	flowTable->setColumnWidth(0, 50);
	flowTable->setColumnWidth(1, 200);
	flowTable->setColumnWidth(2, 200);
	flowTable->setColumnWidth(3, 200);
	flowTable->setColumnWidth(4, 200);

	flowTable->horizontalHeader()->setMinimumHeight(40);
	flowTable->horizontalHeader()->setStyleSheet("font: 14px 微软雅黑 bold;");
	flowTable->horizontalHeader()->setStretchLastSection(true);
	flowTable->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);

	QGridLayout* layout = new QGridLayout;

	int columnCount = 8;
	int rowCount = 10;
	flowLeftTopWLayout->addWidget(flowIcon, Qt::LeftToRight);
	flowLeftTopWLayout->addWidget(flowLabel);
	flowLeftTopWLayout->addStretch(columnCount);
	flowLeftTopWLayout->addWidget(addFlow, Qt::RightToLeft);

	flowLeftTopWidget->setLayout(flowLeftTopWLayout);

	layout->addWidget(flowLeftTopWidget, 0, 0, 1, columnCount);
	layout->addWidget(flowTable, 1, 0, rowCount - 1, columnCount);


	//流程生成界面
	generateFlowWidget = new GenerateFlowCmdWidget;

	loadSavedFlow();

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
void FlowDisplayWidget::addNewFlow()
{
	if (flowEditWidget == nullptr)
	{
		flowEditWidget = FlowEditWidget::getInstance();

		flowEditWidget->setRocketType(flowName, AppCache::instance()->m_CurrentRocketType->m_id);
	}

	flowEditWidget->setMainFlowInfo(mainFlowInfo);
	flowEditWidget->setSubFlowInfo(subFlowInfo);
	flowEditWidget->setSubFlowInfo1(subFlowInfo1);
	flowEditWidget->setSubFlowInfo2(subFlowInfo2);
	flowEditWidget->setFlowCmdID(subFlowCmdID);

	flowEditWidget->setFixedSize(QSize(this->width() / 2, this->width() * 0.3));
	flowEditWidget->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
	flowEditWidget->show();

	connect(flowEditWidget, &FlowEditWidget::updateDisPlayFlow, this, &FlowDisplayWidget::loadSavedFlow);
}

/**
	@brief 加载已有流程  显示到flowDisplay->flowTable上
**/
void FlowDisplayWidget::loadSavedFlow()
{
	flowTable->clearContents();
	mainFlowInfo.clear();
	subFlowInfo.clear();
	subFlowCmdID.clear();
	//flowLabel->setText(QString("%1-发射流程").arg(AppCache::instance()->m_CurrentRocketType->m_name.c_str()));
	flowTable->setHorizontalHeaderLabels(rowHeader);
	//从数据库中加载流程  
	//查询主流程对应子流程信息
	QString qSqlString = "SELECT * FROM main_flow_info WHERE rocket_id = %1; ";
	qSqlString = qSqlString.arg(AppCache::instance()->m_CurrentRocketType->m_id);
	auto flowDBOp = FlowInfoConfig2DB::getInstance();
	flowDBOp->customDBQuery1(qSqlString);
	mainFlowInfo.clear();
	for (auto ele = flowDBOp->customSearchInfo1.begin(); ele != flowDBOp->customSearchInfo1.end(); ele++)
	{
		mainFlowInfo[atoi(ele->second[3].c_str())].push_back(QString::fromStdString(ele->second[2]));
		mainFlowInfo[atoi(ele->second[3].c_str())].push_back(QString::fromStdString(ele->second[4]));
		mainFlowInfo[atoi(ele->second[3].c_str())].push_back(QString::fromStdString(ele->second[5]));
		mainFlowInfo[atoi(ele->second[3].c_str())].push_back(QString::fromStdString(ele->second[0]));
	}

	//查询主流程对应子流程信息
	qSqlString = "SELECT\
		sub_flow_info.id,\
		main_flow_info.`name`,\
		main_flow_info.`index`,\
		main_flow_info.backInfo,\
		main_flow_info.remark,\
		sub_flow_info.main_id,\
		sub_flow_info.`name` as sub_name,\
		sub_flow_info.command_id,\
        sub_flow_info.backInfo as sub_backInfo,\
		sub_flow_info.remark as sub_remark\
		FROM\
		main_flow_info\
		INNER JOIN sub_flow_info ON sub_flow_info.main_id = main_flow_info.id\
		WHERE\
		main_flow_info.rocket_id = %1; ";

	qSqlString = qSqlString.arg(AppCache::instance()->m_CurrentRocketType->m_id);
	flowDBOp->customDBQuery(qSqlString);
	subFlowInfo.clear();
	subFlowInfo1.clear();
	subFlowInfo2.clear();
	subFlowCmdID.clear();

	//子流程ID 主流程name 主流程index  主流程返令信息 主流程备注 主流程ID 子流程name 子流程指令ID
	for (auto ele = flowDBOp->customSearchInfo.begin(); ele != flowDBOp->customSearchInfo.end(); ele++)
	{
		//主流程索引到子流程ID
		mainID2SubID[atoi(ele->second[2].c_str())].push_back(atoi(ele->second[0].c_str()));
		//主流程ID到子流程名称
		subFlowInfo[atoi(ele->second[5].c_str())].push_back(ele->second[6].c_str());
		subFlowInfo1[atoi(ele->second[5].c_str())].push_back(ele->second[8].c_str());
		subFlowInfo2[atoi(ele->second[5].c_str())].push_back(ele->second[9].c_str());
		subFlowCmdID[atoi(ele->second[5].c_str())].push_back(atoi(ele->second[7].c_str()));
	}

	/*!
	 *  Loads the saved flow.加载至流程显示
	 */
	flowTable->setRowCount(mainFlowInfo.size());
	QLabel* label;
	int width, height;
	for (int i = 1; i <= mainFlowInfo.size(); i++)
	{
		int column1 = 0;
		//索引
		flowTable->setItem(i - 1, column1++, new QTableWidgetItem(QString::number(i)));
		flowTable->setItem(i - 1, column1++, new QTableWidgetItem(mainFlowInfo[i][0]));
		int mainID = mainFlowInfo[i][3].toInt();

		//子流程口令 
		QWidget* cellWidg = new QWidget;
		QVBoxLayout* boxLayout = new QVBoxLayout;
		int cmdCount = 1, maxSize = 12, rows1 = 0, rows2 = 0, rows3 = 0, oneRow = 0;

		for (auto val : subFlowInfo.value(mainID))
		{
			oneRow = val.size() % maxSize == 0 ? val.size() / maxSize : val.size() / maxSize + 1; //12表示最大字数
			rows1 += oneRow;
			cmdCount++;
			label = new QLabel(val.replace(QRegExp("[\s+\n\t]"), ""));
			//label->setStyleSheet("QLabel{text-align:center;}");
			//label->adjustSize();
			//label->setGeometry(0, 0, 200, ONEROWHEIGHT * 4);
			//label->setWordWrap(true);
			label->setFixedHeight(ONEROWHEIGHT);
			boxLayout->addWidget(label);

		}

		cellWidg->setLayout(boxLayout);
		flowTable->setCellWidget(i - 1, column1++, cellWidg);

		//子流程回令 
		cellWidg = new QWidget;
		boxLayout = new QVBoxLayout;
		for (auto val : subFlowInfo1.value(mainID))
		{
			oneRow = val.size() % maxSize == 0 ? val.size() / maxSize : val.size() / maxSize + 1; //12表示最大字数
			rows2 += oneRow;

			label = new QLabel(val.replace(QRegExp("[\s\n\t]"), ""));
			//label->setStyleSheet("QLabel{border:1px solid red;text-align:center;}");
			//label->adjustSize();
			//label->setGeometry(0, 0, 200, ONEROWHEIGHT * 4);
			//label->setWordWrap(true);
			label->setFixedHeight(ONEROWHEIGHT);
			boxLayout->addWidget(label);
		}

		cellWidg->setLayout(boxLayout);
		flowTable->setCellWidget(i - 1, column1++, cellWidg);

		//子流程备注 
		maxSize = 17;
		cellWidg = new QWidget;
		boxLayout = new QVBoxLayout;
		QLineEdit* lineEidt;
		for (auto val : subFlowInfo2.value(mainID))
		{
			oneRow = val.size() % maxSize == 0 ? val.size() / maxSize : val.size() / maxSize + 1; //12表示最大字数
			rows3 += oneRow;
			/*label = new QLabel(val);
			label->adjustSize();
			label->setGeometry(0, 0, 200, 27 * 4);
			label->setWordWrap(true);*/
			lineEidt = new QLineEdit;
			lineEidt->setText(val);
			lineEidt->setCursorPosition(0);
			lineEidt->setStyleSheet("QLineEdit{border:none; text-align: left;}");
			//lineEidt->setGeometry(0, 0, 200, ONEROWHEIGHT * 4);
			boxLayout->addWidget(lineEidt);
		}
		if (rows1 < rows2) rows1 = rows2;
		if (rows1 < rows3) rows1 = rows3;

		cellWidg->setLayout(boxLayout);
		flowTable->setRowHeight(i - 1, (rows1 == 1 || rows1 == 0) ? 35 : rows1 * ONEROWHEIGHT);
		flowTable->setCellWidget(i - 1, column1++, cellWidg);
	}

	if (generateFlowWidget == nullptr)
	{
		return;
	}

	generateFlowWidget->setMainFlowInfo(mainFlowInfo);
	generateFlowWidget->setSubFlowInfo(subFlowInfo);
	generateFlowWidget->setFlowCmdID(subFlowCmdID);

	emit updateFlowOver();
	emit sendMainflowchange();
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
	QString qSqlString = "SELECT * FROM main_flow_info WHERE rocket_id = %1; ";
	qSqlString = qSqlString.arg(AppCache::instance()->m_CurrentRocketType->m_id);
	auto flowDBOp = FlowInfoConfig2DB::getInstance();
	flowDBOp->customDBQuery1(qSqlString);
	mainFlowInfo.clear();
	for (auto ele = flowDBOp->customSearchInfo1.begin(); ele != flowDBOp->customSearchInfo1.end(); ele++)
	{
		mainFlowInfo[atoi(ele->second[3].c_str())].push_back(QString::fromStdString(ele->second[2]));
		mainFlowInfo[atoi(ele->second[3].c_str())].push_back(QString::fromStdString(ele->second[4]));
		mainFlowInfo[atoi(ele->second[3].c_str())].push_back(QString::fromStdString(ele->second[5]));
		mainFlowInfo[atoi(ele->second[3].c_str())].push_back(QString::fromStdString(ele->second[0]));
	}

	//查询主流程对应子流程信息
	qSqlString = "SELECT\
		sub_flow_info.id,\
		main_flow_info.`name`,\
		main_flow_info.`index`,\
		main_flow_info.backInfo,\
		main_flow_info.remark,\
		sub_flow_info.main_id,\
		sub_flow_info.`name`,\
		sub_flow_info.command_id,\
        sub_flow_info.backInfo as sub_backInfo,\
		sub_flow_info.remark as sub_remark\
		FROM\
		main_flow_info\
		INNER JOIN sub_flow_info ON sub_flow_info.main_id = main_flow_info.id\
		WHERE\
		main_flow_info.rocket_id = %1; ";

	qSqlString = qSqlString.arg(AppCache::instance()->m_CurrentRocketType->m_id);
	//auto flowDBOp = FlowInfoConfig2DB::getInstance();
	flowDBOp->customDBQuery(qSqlString);
	subFlowInfo.clear();
	subFlowInfo1.clear();
	subFlowInfo2.clear();
	subFlowCmdID.clear();

	//子流程ID 主流程name 主流程index  主流程返令信息 主流程备注 主流程ID 子流程name 子流程指令ID
	for (auto ele = flowDBOp->customSearchInfo.begin(); ele != flowDBOp->customSearchInfo.end(); ele++)
	{
		//主流程索引到子流程ID
		mainID2SubID[atoi(ele->second[2].c_str())].push_back(atoi(ele->second[0].c_str()));
		//主流程ID到子流程名称
		subFlowInfo[atoi(ele->second[5].c_str())].push_back(ele->second[6].c_str());
		subFlowInfo1[atoi(ele->second[5].c_str())].push_back(ele->second[8].c_str());
		subFlowInfo2[atoi(ele->second[5].c_str())].push_back(ele->second[9].c_str());
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
	QString curRunCmdName = "";
	int mainFlowIndex = -1;
	int subFlowID = -1;
	QString backCmdInfo;
	for (auto ele = flowDBOp->customSearchInfo.begin(); ele != flowDBOp->customSearchInfo.end(); ele++)
	{
		if (atoi(ele->second[7].c_str()) == sendCmdID)
		{
			curRunCmdName = QString::fromStdString(ele->second[6]);
			mainFlowIndex = atoi(ele->second[2].c_str());
			subFlowID = atoi(ele->second[0].c_str());
			backCmdInfo = QString::fromStdString(ele->second[8].c_str());
		}
	}
	if (mainFlowIndex == -1 && curRunCmdName == "")
	{
		//QMessageBox::warning(this, "warning", "流程中无此指令");
		return;
	}

	//判断当前执行指令是否是主流程中的最后一个指令
	bool lastCmdFlag = false;
	//检索主流程信息

	auto cmdInfoList = subFlowInfo[mainFlowInfo[mainFlowIndex][3].toInt()];
	if (cmdInfoList.size() == 0)
	{
		return;
	}
	//直到收到 当前子流程中的最后一个指令时才将主流程回令信息显示
	if (cmdInfoList[cmdInfoList.size() - 1] == curRunCmdName)
	{
		lastCmdFlag = true;
	}

	emit sendMainFlowInfo(mainFlowIndex, lastCmdFlag);
	emit updateMainFlowAndSubFlow(mainFlowIndex, curRunCmdName, backCmdInfo);

}