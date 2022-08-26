#include "AllInfoConfigWidget.h"

AllInfoConfigWidget* AllInfoConfigWidget::instance = nullptr;

AllInfoConfigWidget::AllInfoConfigWidget(QWidget* parent)
	: QWidget(parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);

	InitUILayout();

	initConnect();

}

AllInfoConfigWidget::~AllInfoConfigWidget()
{}

/**
	@brief
	@retval  -
**/
QLayout* AllInfoConfigWidget::publicTopLayout() {
	QHBoxLayout* topLayout = new QHBoxLayout;
	QVBoxLayout* topLineVlay = new QVBoxLayout;

	topLayout->addWidget(windowIcon);
	topLayout->addWidget(windowTitle);
	topLayout->addSpacing(this->width() - 350);
	topLayout->addWidget(windowCloseBtn, 0, Qt::AlignRight);

	hframe1 = new QFrame;
	hframe1->setFrameShape(QFrame::HLine);
	//	hframe1->setFixedWidth(640);
	hframe1->setBackgroundRole(QPalette::Background);

	topLineVlay->addLayout(topLayout);
	topLineVlay->addWidget(hframe1);


	windowCloseBtn->setStyleSheet("QPushButton{height:25px;image:url(:/DeviceManager/images/close_normal.png);image-position:right;background-color:rgba(255,255,255,1);border:0px;}\
			QPushButton:hover{image:url(:/DeviceManager/images/关闭-悬浮.png);}\
			QPushButton:pressed{image:url(:/DeviceManager/images/关闭-点击.png);}");
	windowIcon->setStyleSheet("height:25px;width:25px;image:url(:/icon/icon/bb.png);left:12px;");

	topLayout->setContentsMargins(0, 0, 12, 0);
	topLineVlay->setContentsMargins(0, 0, 0, 0);

	return topLineVlay;

}
/**
	@brief
	@retval  -
**/
QLayout* AllInfoConfigWidget::publicBottomLayout() {
	QHBoxLayout* bottomLayout = new QHBoxLayout;
	QVBoxLayout* bottomLineVlay = new QVBoxLayout;

	hframe2 = new QFrame;
	hframe2->setFrameShape(QFrame::HLine);
	//	hframe1->setFixedWidth(640);
	hframe2->setBackgroundRole(QPalette::Background);
	bottomLineVlay->addWidget(hframe2);
	bottomLineVlay->addSpacing(10);

	bottomLayout->addSpacing(this->width() - 200);
	bottomLayout->addWidget(cancelBtn, 1, Qt::AlignRight);
	bottomLayout->addWidget(okBtn, 1, Qt::AlignRight);

	bottomLineVlay->addLayout(bottomLayout);

	cancelBtn->setStyleSheet("\
				\
				QPushButton:hover{background-color:transparent;\
					border-image:url(:/DeviceManager/bt_suspension.png);\
					color:rgb(255,255,255);\
					font:12pt 微软雅黑;\
					border:0px;\
					}\
				QPushButton:pressed{\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_disable.png);\
					color:rgb(255,255,255);\
					font:12pt 微软雅黑;\
					border:0px;}\
				QPushButton{\
					height:30px;width:60px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt 微软雅黑;\
					border:0px;\
					}\
		");
	okBtn->setStyleSheet("\
				\
				QPushButton:hover{background-color:transparent;\
					border-image:url(:/DeviceManager/bt_suspension.png);\
					color:rgb(255,255,255);\
					font:12pt 微软雅黑;\
					border:0px;\
					}\
				QPushButton:pressed{\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_disable.png);\
					color:rgb(255,255,255);\
					font:12pt 微软雅黑;\
					border:0px;}\
				QPushButton{\
					height:30px;width:60px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt 微软雅黑;\
					border:0px;\
					}\
		");


	bottomLineVlay->setContentsMargins(0, 0, 0, 0);
	bottomLayout->setContentsMargins(12, 0, 12, 12);

	return bottomLineVlay;

}


/**
	@brief 初始化火箭配置界面
**/
void AllInfoConfigWidget::initRocketConfigLayout() {
	windowTitle->setText("箭上通信协议配置");

	//UIGrid = new QGridLayout;

	QGridLayout* leftGrid = new QGridLayout;
	QGridLayout* rightGrid = new QGridLayout;

	QHBoxLayout* midUILayout = new QHBoxLayout;

	int rowC = 8;
	int columnC = 4;
	int row = 0;
	int column = 1;


	//左侧
	QHBoxLayout* leftTopHbx = new QHBoxLayout;
	leftTopHbx->addWidget(rocketSearch);
	leftTopHbx->addWidget(addCommuProto);
	rocketSearch->setFixedWidth(180);
	addCommuProto->setFixedWidth(30);
	leftTopHbx->setContentsMargins(0, 0, 0, 0);
	QVBoxLayout* leftVbx = new QVBoxLayout;
	leftVbx->addLayout(leftTopHbx);
	leftVbx->addWidget(rocketComProtoList);
	rocketComProtoList->setFixedWidth(220);
	leftVbx->setContentsMargins(0, 0, 0, 0);

	leftGrid->addLayout(leftVbx, 0, 0, rowC, 1);

	leftGrid->addWidget(rocketTypeParamTitle, row, column++, Qt::AlignLeft);
	leftGrid->addWidget(searchLineEdit, row++, columnC - 1, Qt::AlignRight);
	column = 1;
	leftGrid->addWidget(deviceParamTree, row, column, rowC - 2, columnC);
	leftGrid->addWidget(selectAllBox, rowC - 1, column++);
	leftGrid->addWidget(cancelAllBox, rowC - 1, column);

	//右侧
	row = 0; column = 1;
	rightGrid->addWidget(selectParamTitle, row, column, Qt::AlignLeft);
	rightGrid->addWidget(searchSelect, row, columnC - 1, Qt::AlignRight);
	row++;
	rightGrid->addWidget(hadSelectedParamsL, row, column, rowC - 1, columnC);


	midUILayout->addLayout(leftGrid);
	midUILayout->addLayout(rightGrid);
	midUILayout->setContentsMargins(12, 12, 12, 0);

	searchLineEdit->setPlaceholderText(QString("搜索"));
	searchSelect->setPlaceholderText(QString("搜索"));

	rocketWidget->setLayout(midUILayout);

}

/**
	@brief 初始化设备配置界面
**/
void AllInfoConfigWidget::initDeviceConfigLayout() {


	QWidget* scrollAreaStatContent = new QWidget;
	QWidget* scrollAreaParamContent = new QWidget;

	QGridLayout* midUILayout = new QGridLayout;
	//左侧
	QVBoxLayout* leftVlayout = new QVBoxLayout;
	leftVlayout->addWidget(searchDeviceCfg);
	leftVlayout->addWidget(deviceCfgList);
	deviceCfgList->setFixedWidth(220);
	searchDeviceCfg->setFixedWidth(220);

	//右侧1  状态
	scrollAreaStat->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollAreaStat->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QGridLayout* rightGird = new QGridLayout;
	rightGird->setContentsMargins(120, 40, 20, 60);
	int rightGridColumnC = 2;
	int curInd = 0;
	deviceStatList.clear();
	DeviceDBConfigInfo::getInstance()->readStatusInfoDB2UI();

	for (auto ele : DeviceDBConfigInfo::getInstance()->statusInfo)
	{
		QWidget* widget = new QWidget;
		widget->setFixedWidth(330);
		widget->setObjectName("widget");
		QHBoxLayout* statusHbox = new QHBoxLayout;
		statusHbox->addWidget(new QLabel(QString::fromStdString(ele.second[1])));
		QCheckBox* statusCheck = new QCheckBox;
		statusHbox->addStretch(1);
		statusHbox->addWidget(statusCheck);

		widget->setLayout(statusHbox);
		rightGird->addWidget(widget, curInd / rightGridColumnC, curInd % rightGridColumnC);
		rightGird->setRowStretch(curInd / rightGridColumnC, 1);
		widget->setStyleSheet(QString("QWidget#widget{width:330px;height:58px;border:1px solid black;border-radius:5px;background-color:rgba(255,255,255,1);}"));
		deviceStatList.push_back(widget);
		widget->setFixedHeight(58);
		curInd++;
	}

	QPushButton* addNewStat = new QPushButton;
	addNewStat->setStyleSheet(QString("QPushButton{width:330px;height:58px;border:1px solid gray;border-radius:5px;background-color:rgba(255,255,255,1);image:url(:/icon/icon/+hao.png);}"));
	addNewStat->setFixedHeight(58);
	rightGird->addWidget(addNewStat, curInd / rightGridColumnC, curInd % rightGridColumnC);
	rightGird->setVerticalSpacing(50);
	rightGird->setRowStretch(++curInd / rightGridColumnC, 1);
	//
	scrollAreaStatContent->setLayout(rightGird);
	scrollAreaStat->setWidget(scrollAreaStatContent);
	//右侧2  参数

	scrollAreaParam->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollAreaParam->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QGridLayout* rightGird2 = new QGridLayout;
	rightGird2->setContentsMargins(120, 40, 20, 60);
	int rightGridColumnC2 = 1;
	int curInd2 = 0;
	deviceParamList.clear();
	DeviceDBConfigInfo::getInstance()->readParamDB2UI();

	for (auto ele : DeviceDBConfigInfo::getInstance()->paramInfo)
	{
		QWidget* widget = new QWidget;
		widget->setObjectName("widget");
		widget->setFixedSize(600, 40);
		QHBoxLayout* statusHbox = new QHBoxLayout;
		statusHbox->addWidget(new QLabel(QString::fromStdString(ele.second[1])));
		QCheckBox* statusCheck = new QCheckBox;
		//statusHbox->setSpacing(widget->width() - 200);
		statusHbox->addStretch(1);
		statusHbox->addWidget(statusCheck);

		widget->setLayout(statusHbox);
		rightGird2->addWidget(widget, curInd2 / rightGridColumnC2, curInd2 % rightGridColumnC2);
		rightGird2->setRowStretch(curInd2 / rightGridColumnC2, 1);
		widget->setStyleSheet(QString("QWidget#widget{width:600px;height:40px;border:1px solid black;border-radius:5px;background-color:rgba(255,255,255,1);}"));
		deviceParamList.push_back(widget);

		curInd2++;
	}

	//
	scrollAreaParamContent->setLayout(rightGird2);
	scrollAreaParam->setWidget(scrollAreaParamContent);
	//右侧2上
	QHBoxLayout* rightTopLayout = new QHBoxLayout;

	rightTopLayout->addWidget(searchDevParam);

	rightTopLayout->setContentsMargins(200, 0, 0, 0);
	searchDevParam->setFixedWidth(300);

	//布局
	midUILayout->addLayout(leftVlayout, 0, 0, 2, 1);
	midUILayout->addLayout(rightTopLayout, 0, 1, 1, 2);

	midUILayout->addWidget(scrollAreaStat, 1, 1, 1, 1);
	midUILayout->addWidget(scrollAreaParam, 1, 2, 1, 1);
	midUILayout->setContentsMargins(12, 12, 12, 0);

	scrollAreaStat->setStyleSheet("*{border:none;}");
	scrollAreaParam->setStyleSheet("*{border:none;}");

	deviceWidget->setLayout(midUILayout);

}

/**
	@brief 初始化指令配置界面
**/
void AllInfoConfigWidget::initCommandConfigLayout() {

	QGridLayout* midUILayout = new QGridLayout;

	//左侧
	QVBoxLayout* leftVlayout = new QVBoxLayout;
	leftVlayout->addWidget(searchCmdCfg);
	leftVlayout->addWidget(cmdCfgList);
	cmdCfgList->setFixedWidth(220);
	searchCmdCfg->setFixedWidth(220);
	//右侧1
	QVBoxLayout* rightTop1Layout = new QVBoxLayout;
	rightTop1Layout->addWidget(deviceLabel);
	rightTop1Layout->addWidget(deviceCombox);
	rightTop1Layout->setContentsMargins(40, 0, 0, 0);

	scrollAreaDevStat = new QScrollArea;
	scrollAreaDevStat->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollAreaDevStat->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


	connect(deviceCombox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
		QWidget* devStatComtents = new QWidget;

		int deviceId = deviceCombox->itemData(index).toInt();

		//在设备状态表中查询
		QString qSqlString = QString("SELECT\
			device_status_info.status_id,\
			status_info.`name`,\
			device_status_info.createTime,\
			device_status_info.lastUpdateTime\
			FROM\
			device_status_info\
			INNER JOIN status_info ON device_status_info.status_id = status_info.id\
			WHERE\
			device_status_info.device_id = %1").arg(deviceId);

		DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
		int rightGridColumnC = 2;
		int curInd = 0;
		QGridLayout* contentsLayout = new QGridLayout;
		contentsLayout->setContentsMargins(40, 0, 0, 0);
		for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
		{

			QWidget* widget = new QWidget;
			widget->setObjectName("widget");
			QHBoxLayout* statusHbox = new QHBoxLayout;
			statusHbox->addWidget(new QLabel(QString::fromStdString(ele.second[1])));
			QCheckBox* statusCheck = new QCheckBox;
			statusHbox->addSpacing(160);
			statusHbox->addStretch(1);
			statusHbox->addWidget(statusCheck);

			widget->setLayout(statusHbox);
			contentsLayout->addWidget(widget, curInd / rightGridColumnC, curInd % rightGridColumnC);
			contentsLayout->setRowStretch(curInd / rightGridColumnC, 1);
			widget->setStyleSheet(QString("QWidget#widget{width:330px;height:58px;border:1px solid black;border-radius:5px;background-color:rgba(255,255,255,1);}"));

			widget->setFixedHeight(58);
			curInd++;
		}

		devStatComtents->setLayout(contentsLayout);
		scrollAreaDevStat->setWidget(devStatComtents);

		});


	deviceStatLayout->addLayout(rightTop1Layout, 0, 0, 1, 1);
	deviceStatLayout->addWidget(scrollAreaDevStat, 1, 0, 1, 2);

	//右侧2
	QGridLayout* cmdFrameLayout = new QGridLayout;
	searchCmdFrame->setFixedWidth(200);
	cmdFrameLayout->addWidget(searchCmdFrame, 0, 0, 1, 1);
	cmdFrameLayout->addWidget(cmdFrameLabel, 1, 0, 1, 1);
	cmdFrameLayout->addWidget(addCmdFrame, 1, 4, 1, 1);
	cmdFrameLayout->addWidget(cmdFrameTable, 2, 0, 8, 5);

	midUILayout->addLayout(leftVlayout, 0, 0, 1, 1);
	midUILayout->addLayout(deviceStatLayout, 0, 1, 1, 1);
	midUILayout->addLayout(cmdFrameLayout, 0, 2, 1, 1);
	midUILayout->setContentsMargins(12, 12, 12, 0);

	searchCmdCfg->setPlaceholderText(QString("搜索"));

	scrollAreaDevStat->setStyleSheet("*{border:none;}");
	commandWidget->setLayout(midUILayout);

	searchCmdFrame->hide();
	cmdFrameLabel->hide();
	addCmdFrame->hide();
	cmdFrameTable->hide();
}

/**
	@brief 加载火箭型号相关数据
**/
void AllInfoConfigWidget::loadRocketInfoData() {
	//加载原有协议
	rocketComProtoList->clear();
	QString qSqlString = QString("SELECT\
		rocket_data_info.id,\
		rocket_data_info.`name`,\
		rocket_data_info.`code`,\
		rocket_data_info.createTime,\
		rocket_data_info.lastUpdateTime\
		FROM\
		rocket_data_info\
		WHERE\
		rocket_data_info.rocket_id = %1").arg(rocketID);

	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);

	for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
	{
		QListWidgetItem* tmpItem = new QListWidgetItem(QString::fromStdString(ele.second[1]));
		tmpItem->setData(Qt::UserRole, ele.first);//rocket_data表ID
		rocketComProtoList->addItem(tmpItem);
	}

	//点击添加时添加新的协议至rocket_data表
	connect(addCommuProto, &QPushButton::clicked, this, [=]() {
		//添加
		//先检查表中同一火箭型号是否存在相同code

		DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
		QVector<int> thisRocketCodes;
		for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
		{
			thisRocketCodes.push_back(atoi(ele.second[2].c_str()));
		}
		int addCode = 1;
		for (int i = 1; i < 255; i++)
		{
			if (!thisRocketCodes.contains(i))
			{
				addCode = i;
				break;
			}
		}
		DeviceDBConfigInfo::getInstance()->rocketDataInfo2DB(rocketID, rocketSearch->text(), addCode, 0x55aa);
		DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
		int rocketDataId = 0;
		for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
		{
			if (addCode == atoi(ele.second[2].c_str()))
			{
				rocketDataId = ele.first;
			}
		}
		QListWidgetItem* tmpItem = new QListWidgetItem(rocketSearch->text());
		tmpItem->setData(Qt::UserRole, rocketDataId);//rocket_data表ID
		rocketComProtoList->addItem(tmpItem);
		});

	//根据火箭型号进行设备和参数过滤

	qSqlString = QString("SELECT\
		device_param_info.device_id,\
		device_info.`name`,\
		device_param_info.id,\
		parameter_info.`name`,\
		parameter_info.createTime,\
		parameter_info.lastUpdateTime\
		FROM\
		device_param_info\
		INNER JOIN parameter_info ON device_param_info.parameter_id = parameter_info.id\
		INNER JOIN device_info ON device_param_info.device_id = device_info.id\
		WHERE\
		device_info.rocket_id = %1").arg(rocketID);

	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
	auto allDev2Param = DeviceDBConfigInfo::getInstance()->customReadInfoMap;

	DeviceDBConfigInfo::getInstance()->readParamDB2UI();
	deviceParamTree->clear();
	deviceParamTree->setColumnCount(2);
	deviceParamTree->headerItem()->setHidden(true);

	for (pair<int, vector< string>> ele1 : allDev2Param)
	{
		QTreeWidgetItem* eachItem = new QTreeWidgetItem;
		//eachItem->setCheckState(0, Qt::Unchecked);
		eachItem->setData(0, Qt::UserRole, ele1.first);
		eachItem->setText(0, QString::fromStdString(ele1.second[1]));
		//eachItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
		eachItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
		QList<QTreeWidgetItem*> childList;
		for (int i = 0; i < ele1.second.size() / 4; i++)
		{
			QTreeWidgetItem* subItem = new QTreeWidgetItem;
			subItem->setCheckState(1, Qt::Unchecked);
			subItem->setData(1, Qt::UserRole, atoi(ele1.second[2 + 4 * i].c_str()));
			subItem->setText(1, QString::fromStdString(ele1.second[3 + 4 * i]));
			//subItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
			childList.append(subItem);
		}
		eachItem->addChildren(childList);

		deviceParamTree->addTopLevelItem(eachItem);

	}

	connect(deviceParamTree, &QTreeWidget::itemClicked, this, [=](QTreeWidgetItem* item1, int column) {
		//必须是子项
		auto sss = item1->text(column);
		int c = item1->childCount();
		if (column == 0)
		{
			return;
		}
		//为最小叶子时
		if (item1->childCount() == 0)
		{
			if (item1->checkState(column) == Qt::Checked)
			{
				item1->setCheckState(column, Qt::Unchecked);
				auto findItems = hadSelectedParamsL->findItems(item1->text(column), Qt::MatchExactly);
				for (auto itemL : findItems)
				{
					hadSelectedParamsL->takeItem(hadSelectedParamsL->row(itemL));
				}
			}
			else
			{
				item1->setCheckState(column, Qt::Checked);
				QListWidgetItem* tmpitem = new QListWidgetItem(item1->text(column));
				tmpitem->setData(Qt::UserRole, item1->data(column, Qt::UserRole));
				hadSelectedParamsL->insertItem(0, tmpitem);
			}
		}
		//非叶子
		for (int childIndex = 0; childIndex < item1->childCount(); childIndex++)
		{
			if (item1->child(childIndex)->checkState(column) == Qt::Checked)
			{
				item1->child(childIndex)->setCheckState(column, Qt::Unchecked);
				auto findItems = hadSelectedParamsL->findItems(item1->child(childIndex)->text(column), Qt::MatchExactly);
				for (auto itemL : findItems)
				{
					hadSelectedParamsL->takeItem(hadSelectedParamsL->row(itemL));
				}
			}
			else
			{
				item1->child(childIndex)->setCheckState(column, Qt::Checked);
				QListWidgetItem* tmpitem = new QListWidgetItem(item1->child(childIndex)->text(column));
				tmpitem->setData(Qt::UserRole, item1->child(childIndex)->data(column, Qt::UserRole));
				hadSelectedParamsL->insertItem(0, tmpitem);
			}
		}
		hadSelectedParamsL->update();
		});

	//右侧列表 可以拖动改变顺序
	hadSelectedParamsL->setDragDropMode(QAbstractItemView::InternalMove);
	hadSelectedParamsL->setAcceptDrops(true);


	windowTitle->setText("火箭数据通信协议配置");

	rocketWidget->show();

	deviceWidget->hide();
	commandWidget->hide();
}

/**
	@brief 加载设备相关数据

**/
void AllInfoConfigWidget::loadDeviceInfoData() {
	deviceCfgList->clear();
	//list
	deviceCfgList->addItem(QString("    设备状态"));
	deviceCfgList->addItem(QString("    设备参数绑定"));
	deviceCfgList->item(0)->setSelected(true);

	deviceCfgList->setFocusPolicy(Qt::NoFocus);

	QString ss = QString("QListWidget{border:none;background-color:rgba(249,249,249,1);font:微软雅黑 14px bold;text-align:center;}\
				QListWidget::item{border-radius:5px;padding:10px;image:url(:/icon/icon/bb.png);image-position:left;margin:0 0 10 0;min-height:30px;}\
				QListWidget::item:hover{image:url(:/icon/icon/ww.png);background-color:rgba(63,144,255,1);}\
				QListWidget::item:selected{image:url(:/icon/icon/ww.png);background-color:rgba(63,144,255,1);}");

	deviceCfgList->setStyleSheet(ss);

	connect(deviceCfgList, &QListWidget::currentTextChanged,
		[=](const QString& text) {
			if (text == QString("    设备状态"))
			{
				scrollAreaStat->show();//进入时默认不显示
				searchDevParam->hide();
				scrollAreaParam->hide();
			}
			else
			{
				scrollAreaStat->hide();//进入时默认不显示
				searchDevParam->show();
				scrollAreaParam->show();
			}
		});

	//设备显示
	rocketWidget->hide();
	commandWidget->hide();
	deviceWidget->show();
	scrollAreaParam->hide();
	searchDevParam->hide();

	windowTitle->setText(QString("-设备配置"));
}

void AllInfoConfigWidget::loadCmdInfoData() {
	cmdCfgList->clear();
	//list
	cmdCfgList->addItem(QString("    指令设备状态绑定"));
	cmdCfgList->addItem(QString("    帧内容配置"));

	cmdCfgList->item(0)->setSelected(true);

	cmdCfgList->setFocusPolicy(Qt::NoFocus);

	QString ss = QString("QListWidget{border:none;background-color:rgba(249,249,249,1);font:微软雅黑 14px bold;text-align:center;}\
				QListWidget::item{border-radius:5px;padding:10px;image:url(:/icon/icon/bb.png);image-position:left;margin:0 0 10 0;min-height:30px;}\
				QListWidget::item:hover{image:url(:/icon/icon/ww.png);background-color:rgba(63,144,255,1);}\
				QListWidget::item:selected{image:url(:/icon/icon/ww.png);background-color:rgba(63,144,255,1);}");

	cmdCfgList->setStyleSheet(ss);

	connect(cmdCfgList, &QListWidget::currentTextChanged,
		[=](const QString& text) {
			if (text == QString("    指令设备状态绑定"))
			{
				scrollAreaDevStat->show();//进入时默认不显示
				deviceCombox->show();
				deviceLabel->show();

				searchCmdFrame->hide();
				cmdFrameLabel->hide();
				addCmdFrame->hide();
				cmdFrameTable->hide();

			}
			else
			{
				searchCmdFrame->show();
				cmdFrameLabel->show();
				addCmdFrame->show();
				cmdFrameTable->show();


				scrollAreaDevStat->hide();//进入时默认不显示
				deviceCombox->hide();
				deviceLabel->hide();
			}
		});

	QString qSqlString = QString("SELECT\
		device_info.id,\
		device_info.`name`,\
		device_info.createTime,\
		device_info.lastUpdateTime\
		FROM\
		device_info\
		WHERE\
		device_info.rocket_id = %1").arg(AppCache::instance()->m_CurrentRocketType->m_id);

	deviceCombox->clear();
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
	auto infodata = DeviceDBConfigInfo::getInstance()->customReadInfoMap;
	for (auto ele : infodata)
	{
		QString tmpName = ele.second[1].c_str();
		deviceCombox->addItem(tmpName, ele.first);
	}


	windowTitle->setText(QString("-指令配置"));

	rocketWidget->hide();
	deviceWidget->hide();
	commandWidget->show();
}

/**
	@brief 初始化界面布局
**/
void AllInfoConfigWidget::InitUILayout() {
	this->setFixedSize(QSize(1200, 940));

	this->setStyleSheet("*{background-color:rgba(255,255,255,1);padding:2px;}\
			QLineEdit{background-color:rgba(238, 240, 243, 1);border:2px groove gray;border-radius:10px;padding:2px 4px;font: 12pt 微软雅黑;}\
			QLabel{font: 12pt 微软雅黑;}\
			QComboBox{border: 1px solid darkgray;font: 10pt Arial;selection - background - color: rgb(0, 170, 255);font: 12pt 微软雅黑; }");


	//公共上侧
	windowIcon = new QLabel;
	windowTitle = new QLabel("test");
	windowCloseBtn = new QPushButton;

	//公共下侧
	cancelBtn = new QPushButton(QString("取消"));
	okBtn = new QPushButton(QString("确定"));
	/*!
	 *  Inits the u i layout.火箭型号
	 */
	 //左侧
	rocketSearch = new QLineEdit;
	rocketSearch->setPlaceholderText(QString("搜索"));
	addCommuProto = new QPushButton;
	addCommuProto->setStyleSheet(QString("QPushButton{min-height:30px;border:none;image:url(:/icon/icon/+hao.png);}"));
	rocketComProtoList = new QListWidget;
	//rocketComProtoList->setStyleSheet(WidgetStyleSheet::getInstace()->deviceManageListSS.arg(":/icon/icon/ww.png").arg(":/icon/icon/bb.png"));
	rocketComProtoList->setStyleSheet(QString("QListWidget{border:none;background-color:rgba(249,249,249,1);font:微软雅黑 14px bold;text-align:center;border-right-color:rgb(142,145,145);}\
				QListWidget::item{border-radius:3px;padding:5px;margin:0 0 10 0;min-height:20px;focus:NoFocus;}\
				QListWidget::item:hover{background-color:rgba(63,144,255,1);}\
				QListWidget::item:selected{background-color:rgba(63,144,255,1);}"));
	rocketComProtoList->setFocusPolicy(Qt::NoFocus);
	rocketComProtoList->setContentsMargins(0, 0, 0, 0);
	rocketTypeParamTitle = new QLabel(QString("型号参数"));
	searchLineEdit = new QLineEdit;
	searchLineEdit->setStatusTip(QString("搜索"));
	deviceParamTree = new QTreeWidget;
	selectAllBox = new QCheckBox;
	selectAllBox->setText(QString("全选"));
	cancelAllBox = new QCheckBox;
	cancelAllBox->setText(QString("反选"));
	//右侧
	selectParamTitle = new QLabel(QString("已选择参数"));
	searchSelect = new QLineEdit;
	searchSelect->setStatusTip(QString("搜索"));
	hadSelectedParamsL = new QListWidget;

	/*!
	 *  Inits the u i layout.设备配置
	 */
	 //左
	searchDeviceCfg = new QLineEdit;
	searchDeviceCfg->setPlaceholderText(QString("搜索"));
	addDeviceCfg = new QPushButton;
	deviceCfgList = new QListWidget;
	//右1
	devStatusLayout = new QGridLayout;
	//右2
	searchDevParam = new QLineEdit;
	searchDevParam->setPlaceholderText(QString("搜索"));
	devParamLayout = new QGridLayout;

	/*!
	 *  Inits the u i layout.指令配置
	 */
	 //左侧
	searchCmdCfg = new QLineEdit;
	searchCmdCfg->setPlaceholderText(QString("搜索"));
	addCmdCfg = new QPushButton;
	cmdCfgList = new QListWidget;
	//右1
	deviceLabel = new QLabel(QString("设备"));
	deviceCombox = new QComboBox;
	deviceStatLayout = new QGridLayout;
	//右2
	searchCmdFrame = new QLineEdit;
	searchCmdFrame->setPlaceholderText(QString("搜索"));

	cmdFrameLabel = new QLabel(QString("指令帧内容列表"));
	addCmdFrame = new QPushButton(QString("新增"));

	addCmdFrame->setStyleSheet("\
				\
				QPushButton:hover{background-color:transparent;\
					border-image:url(:/DeviceManager/bt_suspension.png);\
					color:rgb(255,255,255);\
					font:12pt 微软雅黑;\
					border:0px;\
					}\
				QPushButton:pressed{\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_disable.png);\
					color:rgb(255,255,255);\
					font:12pt 微软雅黑;\
					border:0px;}\
				QPushButton{\
					height:30px;width:60px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt 微软雅黑;\
					border:0px;\
					}\
		");

	hadSelectedParamsL->setStyleSheet(QString("QListWidget{border:none;background-color:rgba(249,249,249,1);font:微软雅黑 14px bold;text-align:center;border-right-color:rgb(142,145,145);}\
				QListWidget::item{border-radius:3px;padding:5px;margin:0 0 10 0;min-height:20px;focus:NoFocus;}\
				QListWidget::item:hover{background-color:rgba(63,144,255,1);}\
				QListWidget::item:selected{background-color:rgba(63,144,255,1);}"));
	hadSelectedParamsL->setFocusPolicy(Qt::NoFocus);

	cmdFrameTable = new QTableWidget;
	QStringList tableHeaders;
	tableHeaders << QString("帧内容名称") << QString("帧内容字节长度") << QString("帧内容类型") << QString("帧内容默认值") << QString("操作");
	cmdFrameTable->setColumnCount(tableHeaders.size());
	cmdFrameTable->horizontalHeader()->setMinimumHeight(40);
	cmdFrameTable->horizontalHeader()->setStyleSheet("font: 14px 微软雅黑 bold;");
	cmdFrameTable->horizontalHeader()->setStretchLastSection(true);
	cmdFrameTable->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
	cmdFrameTable->setHorizontalHeaderLabels(tableHeaders);
	cmdFrameTable->horizontalHeader()->show();
	cmdFrameTable->verticalHeader()->show();

	for (int i = 0; i < cmdFrameTable->columnCount(); i++)
	{
		cmdFrameTable->setColumnWidth(i, 160);
	}

	scrollAreaStat = new QScrollArea;
	scrollAreaParam = new QScrollArea;

	rocketWidget = new QWidget;
	deviceWidget = new QWidget;
	commandWidget = new QWidget;

	initRocketConfigLayout();
	initDeviceConfigLayout();
	initCommandConfigLayout();

	UIGrid = new QGridLayout;

	UIGrid->addLayout(publicTopLayout(), 0, 0, 1, 16);
	UIGrid->addWidget(rocketWidget, 1, 0, 1, 16);
	UIGrid->addWidget(deviceWidget, 2, 0, 1, 16);
	UIGrid->addWidget(commandWidget, 2, 0, 1, 16);
	UIGrid->addLayout(publicBottomLayout(), 4, 0, 1, 16);
	UIGrid->setContentsMargins(0, 12, 0, 0);

	rocketWidget->hide();
	deviceWidget->hide();
	this->setLayout(UIGrid);

}

/**
	@brief	  根据传入的窗口类型更换UI
	@param switch_on -
**/
void AllInfoConfigWidget::setCurrentUI(DeviceCommonVaries::InfoWidgetFlag switch_on) {
	curWidget = switch_on;

	switch (switch_on)
	{
	case DeviceCommonVaries::ROCKET_WIDGET:
		//initRocketConfigLayout();
		loadRocketInfoData();
		//this->setLayout(UIGrid);
		break;
	case DeviceCommonVaries::DEVICE_WIDGET:
		//initDeviceConfigLayout();
		loadDeviceInfoData();
		//this->setLayout(UIGrid);
		break;
	case DeviceCommonVaries::COMMAND_WIDGET:
		loadCmdInfoData();
		break;
	default:
		break;
	}

}

void AllInfoConfigWidget::setWindowTitle(QString title) {
	windowTitle->setText(title);
}

/**
	@brief	  显示页配置
	@param rocketId -火箭型号ID
	@param deviceId -设备ID
	@param cmdId    -指令ID
**/
void AllInfoConfigWidget::setInfoWidgetCfg(int rocketId, int deviceId, int cmdId) {
	rocketID = rocketId;
	deviceID = deviceId;
	cmdID = cmdId;
}

/**
	@brief
**/
void AllInfoConfigWidget::widgetConfig() {

}

/**
	@brief 初始化信号与槽
**/
void AllInfoConfigWidget::initConnect() {

	connect(windowCloseBtn, &QPushButton::clicked, this, [=]() {
		instance->close();
		});

	connect(cancelBtn, &QPushButton::clicked, this, &AllInfoConfigWidget::clickedCancelBtn);
	connect(okBtn, &QPushButton::clicked, this, &AllInfoConfigWidget::clickedOkBtn);

	connect(searchLineEdit, &QLineEdit::editingFinished, this, [=]() {

		});

	connect(searchSelect, &QLineEdit::editingFinished, this, [=]() {

		});

}

/**
	@brief
**/
void AllInfoConfigWidget::clickedCancelBtn() {

	this->close();
}

/**
	@brief 点击确定按钮
**/
void AllInfoConfigWidget::clickedOkBtn() {
	//判别当前操作配置
	if (curWidget == DeviceCommonVaries::ROCKET_WIDGET)
	{
		//读取rocketComProtoList中选中的选项作为使用的协议
		int rocketDataID = rocketComProtoList->currentItem()->data(Qt::UserRole).toInt();
		//将之前该协议的配置清除
		DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM `simulatedtraining`.`rockect_param_info` WHERE `rocket_data_id` = %1").arg(rocketDataID));
		int paramIndex = -1;
		//将这里的数据以排序顺序写入rocketParam表
		//确定时得到当前顺序
		for (int rowR = 0; rowR < hadSelectedParamsL->count(); rowR++)
		{
			DeviceDBConfigInfo::getInstance()->rocketParamInfo2DB(rocketDataID, hadSelectedParamsL->item(rowR)->data(Qt::UserRole).toInt(), rowR + 1, 1, 0);
		}
		QMessageBox::information(this, QString("信息"), QString("协议配置成功"));
	}
	else if (curWidget == DeviceCommonVaries::DEVICE_WIDGET)
	{

		QMessageBox::information(this, QString("信息"), QString("设备配置成功"));
	}
	else if (curWidget == DeviceCommonVaries::COMMAND_WIDGET)
	{

		QMessageBox::information(this, QString("信息"), QString("指令配置成功"));
	}

	this->close();
}

/**
	@brief 鼠标按下操作
	@param event -
**/
void AllInfoConfigWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = true;
		mLastMousePosition = event->globalPos();
	}
}

/**
	@brief 鼠标移动操作
	@param event -
**/
void AllInfoConfigWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons().testFlag(Qt::LeftButton) && mMoving)
	{
		setCursor(Qt::OpenHandCursor);
		this->move(this->pos() + (event->globalPos() - mLastMousePosition));
		mLastMousePosition = event->globalPos();
	}
}

/**
	@brief 鼠标释放操作
	@param event -
**/
void AllInfoConfigWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = false;
		setCursor(Qt::ArrowCursor);
	}
}