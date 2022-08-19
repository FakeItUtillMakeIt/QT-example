#include "GenerateFlowCmdWidget.h"

GenerateFlowCmdWidget::GenerateFlowCmdWidget(QWidget* parent)
	: QWidget(parent)
{
	imageWhiteBg = QString::fromLocal8Bit(":/flowload/images/Flow/�׵�20%͸��.png");
	imageWhiteBg2x = QString::fromLocal8Bit(":/flowload/images/Flow/�׵�20%͸��@2x.png");
	imageCmdTitle = QString::fromLocal8Bit(":/flowload/images/Flow/ָ��_��������.png");
	imageCmdTitle2x = QString::fromLocal8Bit(":/flowload/images/Flow/ָ��_��������@2x.png");
	imageCmdClick = QString::fromLocal8Bit(":/flowload/images/Flow/ָ��_���.png");
	imageCmdClick2x = QString::fromLocal8Bit(":/flowload/images/Flow/ָ��_���@2x.png");
	imageCmdDisable = QString::fromLocal8Bit(":/flowload/images/Flow/ָ��_����.png");
	imageCmdDisable2x = QString::fromLocal8Bit(":/flowload/images/Flow/ָ��_����@2x.png");
	imageCmdFloat = QString::fromLocal8Bit(":/flowload/images/Flow/ָ��_����.png");
	imageCmdFloat2x = QString::fromLocal8Bit(":/flowload/images/Flow/ָ��_����@2x.png");
	imageCmdException = QString::fromLocal8Bit(":/flowload/images/Flow/ָ��_�쳣.png");
	imageCmdException2x = QString::fromLocal8Bit(":/flowload/images/Flow/ָ��_�쳣@2x.png");
	imageCmdUsualNoClk = QString::fromLocal8Bit(":/flowload/images/Flow/ָ��_����δ���.png");
	imageCmdUsualNoClk2x = QString::fromLocal8Bit(":/flowload/images/Flow/ָ��_����δ���@2x.png");
	imageCmdExeSuccess = QString::fromLocal8Bit(":/flowload/images/Flow/ָ��_ִ�гɹ�.png");
	imageCmdExeSuccess2x = QString::fromLocal8Bit(":/flowload/images/Flow/ָ��_ִ�гɹ�@2x.png");

	imageIcon = QString::fromLocal8Bit(":/flowload/images/Flow/icon.png");
	imageIcon2x = QString::fromLocal8Bit(":/flowload/images/Flow/icon@2x.png");

	InitLayout();


	execProgramListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
	commandListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
	backCmdListWidget->setSelectionMode(QAbstractItemView::NoSelection);

	//�����û������
	execProgramListWidget->setAttribute(Qt::WA_Disabled, QIODevice::ReadOnly);
	commandListWidget->setAttribute(Qt::WA_Disabled, QIODevice::ReadOnly);
	backCmdListWidget->setAttribute(Qt::WA_Disabled, QIODevice::ReadOnly);


	execProgramListWidget->installEventFilter(this);
	commandListWidget->installEventFilter(this);
	backCmdListWidget->installEventFilter(this);
}

GenerateFlowCmdWidget::~GenerateFlowCmdWidget()
{}

/**
	@brief  ����������¼�
	@param  watched -
	@param  event   -
	@retval         -
**/
bool GenerateFlowCmdWidget::eventFilter(QObject* watched, QEvent* event) {
	if (watched == execProgramListWidget || watched == commandListWidget || watched == backCmdListWidget)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			return true;
		}
	}
	return false;

}

/**
	@brief ��ʼ������
**/
void GenerateFlowCmdWidget::InitLayout() {

	QHBoxLayout* hboxLayout = new QHBoxLayout;

	execProgramListWidget = new QListWidget;
	commandListWidget = new QListWidget;
	backCmdListWidget = new QListWidget;

	execProgramTitle = new QLabel(QString::fromLocal8Bit("       ִ�г����б�"));
	commandTitle = new QLabel(QString::fromLocal8Bit("       ָ���б�"));
	backCmdTitle = new QLabel(QString::fromLocal8Bit("       �����б�"));

	auto aa = QString::fromLocal8Bit("QLabel{border-image:url(%1);font:bold;}").arg(imageCmdException);

	execProgramTitle->setStyleSheet(QString::fromLocal8Bit("*{border-image:url(%1);image:url(%2);font:bold 12px ΢���ź�;min-height:30px;margin-top:0px;image-position: left;}").arg(imageCmdTitle).arg(imageIcon));
	commandTitle->setStyleSheet(QString::fromLocal8Bit("*{border-image:url(%1);image:url(%2);font:bold 12px ΢���ź�;min-height:30px;image-position: left;}").arg(imageCmdTitle).arg(imageIcon));
	backCmdTitle->setStyleSheet(QString::fromLocal8Bit("*{border-image:url(%1);image:url(%2);font:bold 12px ΢���ź�;min-height:30px;image-position: left;}").arg(imageCmdTitle).arg(imageIcon));

	execProgramListWidget->setFocusPolicy(Qt::NoFocus);
	commandListWidget->setFocusPolicy(Qt::NoFocus);
	backCmdListWidget->setFocusPolicy(Qt::NoFocus);

	QString listStyle = QString("\
				QListWidget{background-color:rgba(255,255,255,0.2);border:0;margin:1px 0px 0px 0px;}\
				QListWidget::item{border-image:url(%1);min-height:40px;font: 12px ΢���ź�;margin:0px 20px 20px 0px;selection-background-color: darkblue;}\
				QListWidget::item:hover{ border-image:url(%2);padding-top:-2px;padding-bottom:-1px;min-height:40px;} \
				QListWidget::item:selected{ border-image:url(%3);color:black;padding-top:-2px;padding-bottom:-1px;padding-right:20px;min-height:40px;} \
		").arg(imageCmdUsualNoClk).arg(imageCmdFloat).arg(imageCmdExeSuccess);

	execProgramListWidget->setStyleSheet(listStyle);
	commandListWidget->setStyleSheet(listStyle);
	backCmdListWidget->setStyleSheet(listStyle);

	QVBoxLayout* execProgramLayout = new QVBoxLayout;
	QVBoxLayout* commandLayout = new QVBoxLayout;
	QVBoxLayout* backCmdLayout = new QVBoxLayout;

	execProgramLayout->addWidget(execProgramTitle);
	execProgramLayout->addWidget(execProgramListWidget);

	//execProgramLayout->setMargin(0);

	commandLayout->addWidget(commandTitle);
	commandLayout->addWidget(commandListWidget);

	backCmdLayout->addWidget(backCmdTitle);
	backCmdLayout->addWidget(backCmdListWidget);

	hboxLayout->addLayout(execProgramLayout);
	hboxLayout->addLayout(commandLayout);
	hboxLayout->addLayout(backCmdLayout);

	hboxLayout->setContentsMargins(2, 0, 0, 0);

	this->setLayout(hboxLayout);


}

/**
	@brief ��������Ϣ
	@param mainFlowInfo -
**/
void GenerateFlowCmdWidget::setMainFlowInfo(QMap<int, QVector<QString>> mainFlowInfo) {
	mainFlowInfo_.clear();
	execProgramListWidget->clear();
	commandListWidget->clear();
	backCmdListWidget->clear();

	mainFlowInfo_ = mainFlowInfo;

	for (int i = 1; i <= mainFlowInfo.size(); i++)
	{
		//������
		auto newItem = new QListWidgetItem(mainFlowInfo[i][0]);
		newItem->setTextAlignment(Qt::AlignCenter);
		execProgramListWidget->addItem(newItem);
	}

}

/**
	@brief ��������Ϣ
	@param subFlowInfo -
**/
void GenerateFlowCmdWidget::setSubFlowInfo(QMap<int, QVector<QString>> subFlowInfo) {
	subFlowInfo_.clear();
	//commandListWidget->clear();

	subFlowInfo_ = subFlowInfo;

}

/**
	@brief ���̹���ָ����Ϣ
	@param subFlowCmdID -
**/
void GenerateFlowCmdWidget::setFlowCmdID(QMap<int, QVector<int>> subFlowCmdID) {

	subFlowCmdID_.clear();
	//backCmdListWidget->clear();

	subFlowCmdID_ = subFlowCmdID;


	//responseRecieveCmd(1, QString::fromLocal8Bit("��װ�����"), QString::fromLocal8Bit("��װ�������"));
}

/**
	@brief ��������������Ϣ������������ָ��
**/
void GenerateFlowCmdWidget::clickExeProgramItem(QListWidgetItem* clickedItem) {
	commandListWidget->clearFocus();
	commandListWidget->clearSelection();
	backCmdListWidget->clearFocus();
	backCmdListWidget->clearSelection();

	commandListWidget->clear();

	clickedItem->setSelected(true);
	execProgramListWidget->scrollToItem(clickedItem);
	//������������Ϣ
	int currentSelectMainID = -1;
	for (int i = 1; i <= mainFlowInfo_.size(); i++)
	{
		//������
		if (mainFlowInfo_[i][0] == clickedItem->text()) {
			currentSelectMainID = mainFlowInfo_[i][3].toInt();
			break;
		}
	}

	auto cmdInfoList = subFlowInfo_[currentSelectMainID];
	for (auto cmdInfo : cmdInfoList)
	{
		auto newCmdItem = new QListWidgetItem(cmdInfo);
		newCmdItem->setTextAlignment(Qt::AlignCenter);
		commandListWidget->addItem(newCmdItem);
	}



}

/**
	@brief ��Ӧ����ָ��ʱ����ʵʱ�仯
	@param mainFlowIndex -
	@param curRunCmdName -
	@param backCmdInfo   -
**/
void GenerateFlowCmdWidget::responseRecieveCmd(int mainFlowIndex, QString curRunCmdName, QString backCmdInfo) {

	auto exeItem = execProgramListWidget->item(mainFlowIndex - 1);
	clickExeProgramItem(exeItem);
	execProgramListWidget->setCurrentItem(exeItem);

	QListWidgetItem* cmdItem;
	for (int i = 0; i < commandListWidget->count(); i++)
	{
		if (curRunCmdName == commandListWidget->item(i)->text())
		{
			commandListWidget->item(i)->setSelected(true);
		}
	}

	//QListWidgetItem* backInfoItem = new QListWidgetItem(curRunCmdName + QString::fromLocal8Bit("���յ�"));
	QListWidgetItem* backInfoItem = new QListWidgetItem(backCmdInfo);
	backInfoItem->setTextAlignment(Qt::AlignCenter);

	backCmdListWidget->addItem(backInfoItem);
	backCmdListWidget->setCurrentItem(backInfoItem);
	backCmdListWidget->scrollToItem(backInfoItem);

	//�����Ѿ����յ�����Ϣ
	hadRunExeProgram.push_back(mainFlowInfo_[mainFlowIndex][0]);
	hadRunCmd.push_back(curRunCmdName);
	hadRecievedBackInfo.push_back(backCmdInfo);
	//hadRecievedBackInfo.push_back(curRunCmdName + QString::fromLocal8Bit("���յ�"));
	//QListWidgetItem* backInfoItem1 = new QListWidgetItem(backCmdInfo);
	//backInfoItem1->setTextAlignment(Qt::AlignCenter);
	//backCmdListWidget->addItem(backInfoItem1);

	////������������Ϣ
	//int currentSelectMainID = -1;
	//for (int i = 1; i <= mainFlowInfo_.size(); i++)
	//{
	//	//������
	//	if (mainFlowInfo_[i][0] == exeItem->text()) {
	//		currentSelectMainID = mainFlowInfo_[i][3].toInt();
	//		break;
	//	}
	//}

	//auto cmdInfoList = subFlowInfo_[currentSelectMainID];
	//if (cmdInfoList.size() == 0)
	//{
	//	return;
	//}
	//ֱ���յ� ��ǰ�������е����һ��ָ��ʱ�Ž������̻�����Ϣ��ʾ
	//if (cmdInfoList[cmdInfoList.size() - 1] == curRunCmdName)
	//{
	//	hadRecievedBackInfo.push_back(backCmdInfo);
	//	QListWidgetItem* backInfoItem1 = new QListWidgetItem(backCmdInfo);
	//	backInfoItem1->setTextAlignment(Qt::AlignCenter);
	//	backCmdListWidget->addItem(backInfoItem1);
	//}


}