//#pragma execution_character_set("utf-8")
#include "FlowDisplayWidget.h"
#include "..\..\Public\Utils.h"
#define ONEROWHEIGHT 35

/**
	@brief  ������ʾ����
	@param  parent -
	@retval        -
**/
FlowDisplayWidget::FlowDisplayWidget(QWidget* parent, QString rocketCode, int rocketID)
	: QWidget(parent), flowName(rocketCode), rocketID(rocketID)
{

	this->setAutoFillBackground(true);
	m_app = AppCache::instance();
	flowEditWidget = nullptr;
	rowHeader << QString("���") << QString("�׶�") << QString("����") << QString("����") << QString("��ע");

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
 *  ���û���ͺ�.
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
	@brief ��ʼ�����沼��
**/
void FlowDisplayWidget::InitLayout() {
	this->setAutoFillBackground(true);
	this->setStyleSheet("background-color:rgb(245,245,245);font: 14px ΢���ź�;");

	QWidget* flowLeftTopWidget = new QWidget;
	QHBoxLayout* flowLeftTopWLayout = new QHBoxLayout;
	flowIcon = new QLabel();
	//QString info = QString::fromStdString(Utils::GBKToUTF8(m_app->m_CurrentRocketType->m_name.c_str()));
	flowLabel = new QLabel(QString("%1-��������").arg(flowName));
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
	flowTable->horizontalHeader()->setStyleSheet("font: 14px ΢���ź� bold;");
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

	//�������ɽ���
	generateFlowWidget = new GenerateFlowCmdWidget;

	loadSavedFlow();

	layout->addWidget(generateFlowWidget, 0, 8, 10, 8);;

	layout->setContentsMargins(20, 10, 20, 50);

	this->setLayout(layout);

	connect(addFlow, &QPushButton::clicked, this, &FlowDisplayWidget::addNewFlow);
	connect(loadFlow, &QPushButton::clicked, this, &FlowDisplayWidget::loadSavedFlow);

	connect(this, &FlowDisplayWidget::updateMainFlowAndSubFlow, generateFlowWidget, &GenerateFlowCmdWidget::responseRecieveCmd);

}

/**
	@brief �½�����
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

	flowEditWidget->setParent(this);
	flowEditWidget->setWindowModality(Qt::ApplicationModal);
	flowEditWidget->setWindowFlags(this->windowFlags() | Qt::Dialog | Qt::WindowCloseButtonHint);
	QRect rtWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rtWorkArea, 0);
	flowEditWidget->move((rtWorkArea.width() - flowEditWidget->frameGeometry().width()) / 2, (rtWorkArea.height() - flowEditWidget->frameGeometry().height()) / 2);
	flowEditWidget->show();

	connect(flowEditWidget, &FlowEditWidget::updateDisPlayFlow, this, &FlowDisplayWidget::loadSavedFlow);
}


/**
	@brief	�޸�������ʾ����
	������������  ��ʾ��flowDisplay->flowTable��
**/
void FlowDisplayWidget::loadSavedFlow() {
	flowTable->clearContents();
	flowTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

	flowTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	mainFlowInfo.clear();
	subFlowInfo.clear();
	subFlowCmdID.clear();
	//flowLabel->setText(QString("%1-��������").arg(AppCache::instance()->m_CurrentRocketType->m_name.c_str()));
	flowTable->setHorizontalHeaderLabels(rowHeader);
	//�����ݿ��м�������  
	//��ѯ�����̶�Ӧ��������Ϣ
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

	//��ѯ�����̶�Ӧ��������Ϣ
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

	int totalRowCount = 0;
	//������ID ������name ������index  �����̷�����Ϣ �����̱�ע ������ID ������name ������ָ��ID
	for (auto ele = flowDBOp->customSearchInfo.begin(); ele != flowDBOp->customSearchInfo.end(); ele++)
	{
		//������������������ID
		mainID2SubID[atoi(ele->second[2].c_str())].push_back(atoi(ele->second[0].c_str()));
		//������ID������������
		subFlowInfo[atoi(ele->second[5].c_str())].push_back(ele->second[6].c_str());
		subFlowInfo1[atoi(ele->second[5].c_str())].push_back(ele->second[8].c_str());
		subFlowInfo2[atoi(ele->second[5].c_str())].push_back(ele->second[9].c_str());
		subFlowCmdID[atoi(ele->second[5].c_str())].push_back(atoi(ele->second[7].c_str()));
		totalRowCount++;
	}

	/*!
	 *  Loads the saved flow.������������ʾ
	 */
	 //start
	flowTable->setRowCount(totalRowCount);
	int infoRow = 0;
	for (int i = 1; i <= mainFlowInfo.size(); i++)
	{
		//������ID
		int mainID = mainFlowInfo[i][3].toInt();
		int startSpanRow = infoRow;
		//������
		QString subCmd, subBackCmd, subRemark;
		for (int subIndex = 0; subIndex < subFlowInfo.value(mainID).size(); subIndex++)
		{

			subCmd = subFlowInfo.value(mainID)[subIndex];
			subCmd = subCmd.replace(QRegExp("[\s+\n\t]"), "");
			subBackCmd = subFlowInfo1.value(mainID)[subIndex];
			subBackCmd = subBackCmd.replace(QRegExp("[\s+\n\t]"), "");
			subRemark = subFlowInfo2.value(mainID)[subIndex];
			subRemark = subRemark.replace(QRegExp("[\s+\n\t]"), "");

			flowTable->setItem(infoRow, 2, new QTableWidgetItem(subCmd));
			flowTable->setItem(infoRow, 3, new QTableWidgetItem(subBackCmd));
			QLineEdit* editRemark = new QLineEdit(subRemark);
			editRemark->setCursorPosition(0);
			editRemark->setReadOnly(true);
			flowTable->setCellWidget(infoRow, 4, editRemark);
			infoRow++;
		}
		//����
		flowTable->setItem(startSpanRow, 0, new QTableWidgetItem(QString::number(i)));
		flowTable->setItem(startSpanRow, 1, new QTableWidgetItem(mainFlowInfo[i][0]));
		flowTable->setSpan(startSpanRow, 0, subFlowInfo.value(mainID).size(), 1);
		flowTable->setSpan(startSpanRow, 1, subFlowInfo.value(mainID).size(), 1);

	}
	flowTable->setStyleSheet("QLineEdit{border:none;}");

	//end

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
	@brief ���ݽ��յõ���icode��sendcode����������Ϣ
	@param m_iCode   -��ǰִ��ָ��Ļ���
	@param sendICode -����ָ���������ִ�е�ǰִ�е�ָ��
**/
void FlowDisplayWidget::updateFlowStat(int m_iCode, int sendICode) {
	//1.��������̺�������
	//2.���������̺������̹�ϵ��������ִ�з���õ������̣����������̵���ǰ�����̵����һ��ʱ���л�����һ�������̣�Ȼ�����εݹ飩


	//��ѯ�����̶�Ӧ��������Ϣ
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

	//��ѯ�����̶�Ӧ��������Ϣ
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

	//������ID ������name ������index  �����̷�����Ϣ �����̱�ע ������ID ������name ������ָ��ID
	for (auto ele = flowDBOp->customSearchInfo.begin(); ele != flowDBOp->customSearchInfo.end(); ele++)
	{
		//������������������ID
		mainID2SubID[atoi(ele->second[2].c_str())].push_back(atoi(ele->second[0].c_str()));
		//������ID������������
		subFlowInfo[atoi(ele->second[5].c_str())].push_back(ele->second[6].c_str());
		subFlowInfo1[atoi(ele->second[5].c_str())].push_back(ele->second[8].c_str());
		subFlowInfo2[atoi(ele->second[5].c_str())].push_back(ele->second[9].c_str());
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
		//QMessageBox::warning(this, "warning", "�������޴�ָ��");
		return;
	}

	//�жϵ�ǰִ��ָ���Ƿ����������е����һ��ָ��
	bool lastCmdFlag = false;
	//������������Ϣ

	auto cmdInfoList = subFlowInfo[mainFlowInfo[mainFlowIndex][3].toInt()];
	if (cmdInfoList.size() == 0)
	{
		return;
	}
	//ֱ���յ� ��ǰ�������е����һ��ָ��ʱ�Ž������̻�����Ϣ��ʾ
	if (cmdInfoList[cmdInfoList.size() - 1] == curRunCmdName)
	{
		lastCmdFlag = true;
	}

	emit sendMainFlowInfo(mainFlowIndex, lastCmdFlag);
	emit updateMainFlowAndSubFlow(mainFlowIndex, curRunCmdName, backCmdInfo);

}