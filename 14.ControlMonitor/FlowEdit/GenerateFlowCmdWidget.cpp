#include "GenerateFlowCmdWidget.h"

GenerateFlowCmdWidget::GenerateFlowCmdWidget(QWidget* parent)
	: QWidget(parent)
{
	imageWhiteBg = QString::fromLocal8Bit(":/flowload/images/Flow/白底20%透明.png");
	imageWhiteBg2x = QString::fromLocal8Bit(":/flowload/images/Flow/白底20%透明@2x.png");
	imageCmdTitle = QString::fromLocal8Bit(":/flowload/images/Flow/指令_参数标题.png");
	imageCmdTitle2x = QString::fromLocal8Bit(":/flowload/images/Flow/指令_参数标题@2x.png");
	imageCmdClick = QString::fromLocal8Bit(":/flowload/images/Flow/指令_点击.png");
	imageCmdClick2x = QString::fromLocal8Bit(":/flowload/images/Flow/指令_点击@2x.png");
	imageCmdDisable = QString::fromLocal8Bit(":/flowload/images/Flow/指令_禁用.png");
	imageCmdDisable2x = QString::fromLocal8Bit(":/flowload/images/Flow/指令_禁用@2x.png");
	imageCmdFloat = QString::fromLocal8Bit(":/flowload/images/Flow/指令_悬浮.png");
	imageCmdFloat2x = QString::fromLocal8Bit(":/flowload/images/Flow/指令_悬浮@2x.png");
	imageCmdException = QString::fromLocal8Bit(":/flowload/images/Flow/指令_异常.png");
	imageCmdException2x = QString::fromLocal8Bit(":/flowload/images/Flow/指令_异常@2x.png");
	imageCmdUsualNoClk = QString::fromLocal8Bit(":/flowload/images/Flow/指令_正常未点击.png");
	imageCmdUsualNoClk2x = QString::fromLocal8Bit(":/flowload/images/Flow/指令_正常未点击@2x.png");
	imageCmdExeSuccess = QString::fromLocal8Bit(":/flowload/images/Flow/指令_执行成功.png");
	imageCmdExeSuccess2x = QString::fromLocal8Bit(":/flowload/images/Flow/指令_执行成功@2x.png");

	imageIcon = QString::fromLocal8Bit(":/flowload/images/Flow/icon.png");
	imageIcon2x = QString::fromLocal8Bit(":/flowload/images/Flow/icon@2x.png");

	InitLayout();
}

GenerateFlowCmdWidget::~GenerateFlowCmdWidget()
{}

/**
	@brief 初始化布局
**/
void GenerateFlowCmdWidget::InitLayout() {

	QHBoxLayout* hboxLayout = new QHBoxLayout;

	execProgramListWidget = new QListWidget;
	commandListWidget = new QListWidget;
	backCmdListWidget = new QListWidget;

	execProgramTitle = new QLabel(QString::fromLocal8Bit("       执行程序列表"));
	commandTitle = new QLabel(QString::fromLocal8Bit("       指令列表"));
	backCmdTitle = new QLabel(QString::fromLocal8Bit("       回令列表"));

	auto aa = QString::fromLocal8Bit("QLabel{border-image:url(%1);font:bold;}").arg(imageCmdException);

	execProgramTitle->setStyleSheet(QString::fromLocal8Bit("*{border-image:url(%1);image:url(%2);font:bold 12px 微软雅黑;min-height:30px;margin-top:0px;image-position: left;}").arg(imageCmdTitle).arg(imageIcon));
	commandTitle->setStyleSheet(QString::fromLocal8Bit("*{border-image:url(%1);image:url(%2);font:bold 12px 微软雅黑;min-height:30px;image-position: left;}").arg(imageCmdTitle).arg(imageIcon));
	backCmdTitle->setStyleSheet(QString::fromLocal8Bit("*{border-image:url(%1);image:url(%2);font:bold 12px 微软雅黑;min-height:30px;image-position: left;}").arg(imageCmdTitle).arg(imageIcon));

	execProgramListWidget->setFocusPolicy(Qt::NoFocus);
	commandListWidget->setFocusPolicy(Qt::NoFocus);
	backCmdListWidget->setFocusPolicy(Qt::NoFocus);

	QString listStyle = QString("\
				QListWidget{background-color:rgba(255,255,255,0.2);border:0;}\
				QListWidget::item{border-image:url(%1);min-height:40px;font: 12px 微软雅黑;margin:0px 20px 20px 0px;}\
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

	hboxLayout->setContentsMargins(0, 0, 0, 0);

	this->setLayout(hboxLayout);

	connect(execProgramListWidget, &QListWidget::itemClicked, this, &GenerateFlowCmdWidget::clickExeProgramItem);
}

/**
	@brief 主流程信息
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
		qDebug() << mainFlowInfo[i][1];
		QListWidgetItem* newBackCmdItem = new QListWidgetItem(mainFlowInfo[i][1]);
		newBackCmdItem->setTextAlignment(Qt::AlignCenter);
		backCmdListWidget->addItem(newBackCmdItem);
		//流程名
		auto newItem = new QListWidgetItem(mainFlowInfo[i][0]);
		newItem->setTextAlignment(Qt::AlignCenter);
		execProgramListWidget->addItem(newItem);

	}

}

/**
	@brief 子流程信息
	@param subFlowInfo -
**/
void GenerateFlowCmdWidget::setSubFlowInfo(QMap<int, QVector<QString>> subFlowInfo) {
	subFlowInfo_.clear();
	//commandListWidget->clear();

	subFlowInfo_ = subFlowInfo;

}

/**
	@brief 流程关联指令信息
	@param subFlowCmdID -
**/
void GenerateFlowCmdWidget::setFlowCmdID(QMap<int, QVector<int>> subFlowCmdID) {

	subFlowCmdID_.clear();
	//backCmdListWidget->clear();

	subFlowCmdID_ = subFlowCmdID;
}

/**
	@brief 点击主流程相关信息，加载子流程指令
**/
void GenerateFlowCmdWidget::clickExeProgramItem(QListWidgetItem* clickedItem) {
	commandListWidget->clearFocus();
	commandListWidget->clearSelection();
	backCmdListWidget->clearFocus();
	backCmdListWidget->clearSelection();

	commandListWidget->clear();

	//检索主流程信息
	int currentSelectMainID = -1;
	for (int i = 1; i <= mainFlowInfo_.size(); i++)
	{
		//流程名
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