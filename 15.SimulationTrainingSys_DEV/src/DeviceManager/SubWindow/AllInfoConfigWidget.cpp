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
	topLayout->addSpacing(this->width() - 250);
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
	windowTitle->setText("火箭型号配置");

	//UIGrid = new QGridLayout;

	QGridLayout* leftGrid = new QGridLayout;
	QGridLayout* rightGrid = new QGridLayout;

	QHBoxLayout* midUILayout = new QHBoxLayout;

	int rowC = 8;
	int columnC = 4;
	int row = 0;
	int column = 0;


	//左侧
	leftGrid->addWidget(rocketTypeParamTitle, row, column++, Qt::AlignLeft);
	leftGrid->addWidget(searchLineEdit, row++, columnC - 1, Qt::AlignRight);
	column = 0;
	leftGrid->addWidget(deviceParamTree, row, column, rowC - 2, columnC);
	leftGrid->addWidget(selectAllBox, rowC - 1, column++);
	leftGrid->addWidget(cancelAllBox, rowC - 1, column);

	//右侧
	row = 0; column = 0;
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
		widget->setObjectName("widget");
		QHBoxLayout* statusHbox = new QHBoxLayout;
		statusHbox->addWidget(new QLabel(QString::fromStdString(ele.second[1])));
		QCheckBox* statusCheck = new QCheckBox;
		statusHbox->setSpacing(160);
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

	midUILayout->addLayout(leftVlayout, 0, 0, 1, 1);
	midUILayout->addLayout(deviceStatLayout, 0, 1, 1, 1);
	midUILayout->setContentsMargins(12, 12, 12, 0);

	searchCmdCfg->setPlaceholderText(QString("搜索"));

	scrollAreaDevStat->setStyleSheet("*{border:none;}");
	commandWidget->setLayout(midUILayout);



}

/**
	@brief 加载火箭型号相关数据
**/
void AllInfoConfigWidget::loadRocketInfoData() {


	//根据火箭型号进行设备和参数过滤
	QString qSqlString = QString("SELECT\
		device_info.id,\
		device_param_info.parameter_id,\
		parameter_info.createTime,\
		parameter_info.lastUpdateTime\
		FROM\
		rocket_info\
		INNER JOIN device_info ON rocket_info.id = device_info.rocket_id\
		INNER JOIN device_param_info ON device_info.id = device_param_info.device_id\
		INNER JOIN parameter_info ON device_param_info.parameter_id = parameter_info.id\
		WHERE\
		rocket_info.id = %1").arg(AppCache::instance()->m_CurrentRocketType->m_id);


	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
	auto rocketHadConfigData = DeviceDBConfigInfo::getInstance()->customReadInfoMap;
	qSqlString = QString("SELECT\
		device_info.id,\
		device_info.`name`,\
		device_info.createTime,\
		device_info.lastUpdateTime\
		FROM\
		device_info\
		WHERE\
		device_info.rocket_id = %1;").arg(AppCache::instance()->m_CurrentRocketType->m_id);
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
	auto allDevCurRocket = DeviceDBConfigInfo::getInstance()->customReadInfoMap;

	DeviceDBConfigInfo::getInstance()->readParamDB2UI();

	for (pair<int, vector< string>> ele1 : allDevCurRocket)
	{
		QTreeWidgetItem* eachItem = new QTreeWidgetItem;
		eachItem->setCheckState(0, Qt::Unchecked);
		eachItem->setText(0, QString::fromStdString(ele1.second[1]));
		for (pair<int, vector<string>> ele2 : DeviceDBConfigInfo::getInstance()->paramInfo)
		{
			QTreeWidgetItem* subItem = new QTreeWidgetItem;
			subItem->setCheckState(1, Qt::Unchecked);
			subItem->setText(1, QString::fromStdString(ele2.second[1]));
			eachItem->addChild(subItem);
		}
		deviceParamTree->addTopLevelItem(eachItem);
	}

	windowTitle->setText("火箭型号配置");

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

	windowTitle->setText("设备配置");
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

			}
			else
			{
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
	cmdFrameTable = new QTableWidget;

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
	@brief
**/
void AllInfoConfigWidget::clickedOkBtn() {

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