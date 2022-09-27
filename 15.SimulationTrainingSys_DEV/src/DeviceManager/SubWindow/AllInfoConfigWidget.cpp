#include "AllInfoConfigWidget.h"

AllInfoConfigWidget* AllInfoConfigWidget::instance = nullptr;

AllInfoConfigWidget::AllInfoConfigWidget(QWidget* parent)
	: QWidget(parent) {
	this->setWindowFlags(Qt::FramelessWindowHint);

	InitUILayout();

	initConnect();
}

AllInfoConfigWidget::~AllInfoConfigWidget() {

}

/**
	@brief �������ý���Ĺ����ϲ�
	@retval  -���ع����ϲ��Ĳ���
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
			QPushButton:hover{image:url(:/DeviceManager/images/�ر�-����.png);}\
			QPushButton:pressed{image:url(:/DeviceManager/images/�ر�-���.png);}");
	windowIcon->setStyleSheet("height:25px;width:25px;image:url(:/icon/icon/bb.png);left:12px;");

	topLayout->setContentsMargins(0, 0, 12, 0);
	topLineVlay->setContentsMargins(0, 0, 0, 0);

	return topLineVlay;

}
/**
	@brief �������ý���Ĺ����²�
	@retval  -���ع����²��Ĳ���
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
					font:12pt ΢���ź�;\
					border:0px;\
					}\
				QPushButton:pressed{\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_disable.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;}\
				QPushButton{\
					height:30px;width:60px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;\
					}\
		");
	okBtn->setStyleSheet("\
				\
				QPushButton:hover{background-color:transparent;\
					border-image:url(:/DeviceManager/bt_suspension.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;\
					}\
				QPushButton:pressed{\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_disable.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;}\
				QPushButton{\
					height:30px;width:60px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;\
					}\
		");


	bottomLineVlay->setContentsMargins(0, 0, 0, 0);
	bottomLayout->setContentsMargins(12, 0, 12, 12);

	return bottomLineVlay;

}


/**
	@brief ��ʼ��������ý���
**/
void AllInfoConfigWidget::initRocketConfigLayout() {
	windowTitle->setText("����ͨ��Э������");

	//UIGrid = new QGridLayout;

	QGridLayout* leftGrid = new QGridLayout;
	QGridLayout* rightGrid = new QGridLayout;

	QHBoxLayout* midUILayout = new QHBoxLayout;

	int rowC = 8;
	int columnC = 4;
	int row = 0;
	int column = 1;


	//���
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

	//�Ҳ�
	row = 0; column = 1;
	rightGrid->addWidget(selectParamTitle, row, column, Qt::AlignLeft);
	rightGrid->addWidget(searchSelect, row, columnC - 1, Qt::AlignRight);
	row++;
	rightGrid->addWidget(hadSelectedParamsL, row, column, rowC - 1, columnC);


	midUILayout->addLayout(leftGrid);
	midUILayout->addLayout(rightGrid);
	midUILayout->setContentsMargins(12, 12, 12, 0);

	searchLineEdit->setPlaceholderText(QString("����"));
	searchSelect->setPlaceholderText(QString("����"));

	rocketWidget->setLayout(midUILayout);


}

/**
	@brief ��ʼ���豸���ý���
**/
void AllInfoConfigWidget::initDeviceConfigLayout() {


	QGridLayout* midUILayout = new QGridLayout;
	//���
	QVBoxLayout* leftVlayout = new QVBoxLayout;
	leftVlayout->addWidget(searchDeviceCfg);
	leftVlayout->addWidget(deviceCfgList);
	deviceCfgList->setFixedWidth(220);
	searchDeviceCfg->setFixedWidth(220);

	//�Ҳ�1  ״̬
	scrollAreaStat->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollAreaStat->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


	//�Ҳ�2  ����
	scrollAreaParam->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	QGridLayout* rightGird2 = new QGridLayout;
	rightGird2->setContentsMargins(60, 40, 20, 60);
	int rightGridColumnC2 = 1;
	int curInd2 = 0;
	deviceParamList.clear();
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
		parameter_info.id,\
		parameter_info.`name`,\
		parameter_info.createTime,\
		parameter_info.lastUpdateTime\
		FROM\
		parameter_info\
		INNER JOIN parameter_rocket_info ON parameter_info.id = parameter_rocket_info.parameter_id\
		WHERE\
		parameter_rocket_info.rocket_id = %1\
		").arg(AppCache::instance()->m_CurrentRocketType->m_id));

	for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
	{
		QWidget* widget = new QWidget;
		widget->setObjectName("widget");
		//widget->setFixedSize(600, 40);
		widget->setFixedHeight(40);
		QHBoxLayout* statusHbox = new QHBoxLayout;
		QLabel* lab = new QLabel(QString::fromStdString(ele.second[1]));
		lab->setObjectName("paramlab");
		lab->setProperty("id", ele.first);
		statusHbox->addWidget(lab);

		//QPushButton* selectFileBtn = new QPushButton(QString("���ļ�"));

		QCheckBox* statusCheck = new QCheckBox;
		statusCheck->setObjectName("checkbox");

		statusHbox->addStretch(1);
		statusHbox->addWidget(statusCheck);
		//statusHbox->setAlignment(Qt::Alignm);

		widget->setLayout(statusHbox);
		rightGird2->addWidget(widget, curInd2 / rightGridColumnC2, curInd2 % rightGridColumnC2);
		rightGird2->setRowStretch(curInd2 / rightGridColumnC2, 1);
		widget->setStyleSheet(QString("QWidget#widget{width:600px;height:40px;border:1px solid black;border-radius:5px;background-color:rgba(255,255,255,1);}"));
		deviceParamList.push_back(widget);

		curInd2++;
	}

	scrollAreaParam->setWidgetResizable(true);
	//�Ҳ�2��
	QHBoxLayout* rightTopLayout = new QHBoxLayout;

	rightTopLayout->addWidget(searchDevParam);

	rightTopLayout->setContentsMargins(0, 0, 0, 0);
	searchDevParam->setFixedWidth(300);

	//����
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
	@brief ��ʼ��ָ�����ý���
**/
void AllInfoConfigWidget::initCommandConfigLayout() {

	QGridLayout* midUILayout = new QGridLayout;

	//���
	QVBoxLayout* leftVlayout = new QVBoxLayout;
	leftVlayout->addWidget(searchCmdCfg);
	leftVlayout->addWidget(cmdCfgList);
	cmdCfgList->setFixedWidth(220);
	searchCmdCfg->setFixedWidth(220);
	hadSelectDevStat->setFixedWidth(260);
	//�Ҳ�1
	QVBoxLayout* rightTop1Layout = new QVBoxLayout;
	rightTop1Layout->addWidget(deviceLabel);
	rightTop1Layout->addWidget(deviceCombox);


	scrollAreaDevStat = new QScrollArea;
	scrollAreaDevStat->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollAreaDevStat->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	rightTop1Layout->setContentsMargins(60, 0, 20, 60);

	deviceStatLayout->addLayout(rightTop1Layout, 0, 0, 1, 1);
	deviceStatLayout->addWidget(scrollAreaDevStat, 1, 0, 1, 2);
	deviceStatLayout->addWidget(hadSelectDevStat, 0, 3, 2, 1);

	//�Ҳ�2
	QGridLayout* cmdFrameLayout = new QGridLayout;
	searchCmdFrame->setFixedWidth(200);
	cmdFrameLayout->addWidget(searchCmdFrame, 0, 0, 1, 1);
	cmdFrameLayout->addWidget(cmdFrameLabel, 1, 0, 1, 1);
	cmdFrameLayout->addWidget(addCmdFrame, 1, 4, 1, 1);
	cmdFrameLayout->addWidget(cmdFrameTable, 2, 0, 8, 5);

	midUILayout->addLayout(leftVlayout, 0, 0, 1, 1);
	midUILayout->addLayout(deviceStatLayout, 0, 1, 1, 1);
	midUILayout->addLayout(cmdFrameLayout, 0, 1, 1, 1);
	midUILayout->setContentsMargins(12, 12, 12, 0);

	searchCmdCfg->setPlaceholderText(QString("����"));

	scrollAreaDevStat->setStyleSheet("*{border:none;}");
	commandWidget->setLayout(midUILayout);

	searchCmdFrame->hide();
	cmdFrameLabel->hide();
	addCmdFrame->hide();
	cmdFrameTable->hide();
}

/**
	@brief ���ػ���ͺ��������
	//��������Э��ʱ������Э���������Ϣ��ʾ
**/
void AllInfoConfigWidget::loadRocketInfoData() {

	deviceParamTree->setStyleSheet("QTreeWidget::item:selected{border-image:url(:/DeviceManager/bt_normal.png);}");
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
		tmpItem->setData(Qt::UserRole, ele.first);//rocket_data��ID
		rocketComProtoList->addItem(tmpItem);
	}

	//if (rocketComProtoList->count() == 0)
	{
		deviceParamTree->clear();
		//���ݻ���ͺŽ����豸�Ͳ�������
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


		deviceParamTree->clear();
		deviceParamTree->setColumnCount(2);
		deviceParamTree->headerItem()->setHidden(true);

		hadSelectedParamsL->clear();
		for (pair<int, vector< string>> ele1 : allDev2Param)
		{
			QTreeWidgetItem* eachItem = new QTreeWidgetItem;

			eachItem->setData(0, Qt::UserRole, ele1.first);
			eachItem->setText(0, QString::fromStdString(ele1.second[1]));
			eachItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
			QList<QTreeWidgetItem*> childList;
			for (int i = 0; i < ele1.second.size() / 4; i++)
			{
				QTreeWidgetItem* subItem = new QTreeWidgetItem;
				int tmpDevParamId = atoi(ele1.second[2 + 4 * i].c_str());
				subItem->setData(1, Qt::UserRole, tmpDevParamId);

				subItem->setCheckState(1, Qt::Unchecked);
				subItem->setSelected(false);

				subItem->setText(1, QString::fromStdString(ele1.second[3 + 4 * i]));

				childList.append(subItem);
			}
			eachItem->addChildren(childList);
			deviceParamTree->addTopLevelItem(eachItem);
		}
		deviceParamTree->expandAll();
	}
	if (rocketComProtoList->count() != 0)
	{
		rocketComProtoList->setCurrentRow(0);
		deviceParamTree->clearSelection();
		deviceParamTree->expandAll();
		hadSelectedParamsL->clear();

		QString qSqlString22 = QString("SELECT\
			rockect_param_info.device_parameter_id,\
			rockect_param_info.createTime,\
			rockect_param_info.lastUpdateTime\
			FROM\
			rockect_param_info\
			INNER JOIN rocket_data_info ON rocket_data_info.id = rockect_param_info.rocket_data_id\
			WHERE\
			rockect_param_info.rocket_data_id = %1 AND\
		rocket_data_info.rocket_id = %2").arg(rocketComProtoList->item(0)->data(Qt::UserRole).toInt()).arg(rocketID);
		DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString22);
		auto curSlcProtoInfo = DeviceDBConfigInfo::getInstance()->customReadInfoMap;

		for (int i = 0; i < deviceParamTree->topLevelItemCount(); i++)
		{
			//deviceParamTree->topLevelItem(i)->setCheckState(0, Qt::Checked);
			for (int j = 0; j < deviceParamTree->topLevelItem(i)->childCount(); j++)
			{
				if (curSlcProtoInfo[deviceParamTree->topLevelItem(i)->child(j)->data(1, Qt::UserRole).toInt()].empty())
				{
					deviceParamTree->topLevelItem(i)->child(j)->setCheckState(1, Qt::Unchecked);
					deviceParamTree->topLevelItem(i)->child(j)->setSelected(false);
				}
				else
				{
					deviceParamTree->topLevelItem(i)->child(j)->setCheckState(1, Qt::Checked);
					deviceParamTree->topLevelItem(i)->child(j)->setSelected(true);
					QListWidgetItem* selctDevParam = new QListWidgetItem(deviceParamTree->topLevelItem(i)->child(j)->text(1));
					selctDevParam->setData(Qt::UserRole, deviceParamTree->topLevelItem(i)->child(j)->data(1, Qt::UserRole).toInt());
					hadSelectedParamsL->addItem(selctDevParam);
				}

			}
		}
	}
#ifdef __0__
	//else
	if (0)
	{
		//����б�Ĭ��ѡ�е�һ��
		rocketComProtoList->setCurrentRow(0);
		//���ݻ���ͺŽ����豸�Ͳ�������
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

		//��Э��ʱ
		qSqlString = QString("SELECT\
			rockect_param_info.device_parameter_id,\
			rockect_param_info.createTime,\
			rockect_param_info.lastUpdateTime\
			FROM\
			rockect_param_info\
			INNER JOIN rocket_data_info ON rocket_data_info.id = rockect_param_info.rocket_data_id\
			WHERE\
			rockect_param_info.rocket_data_id = %1 AND\
		rocket_data_info.rocket_id = %2").arg(rocketComProtoList->item(0)->data(Qt::UserRole).toInt()).arg(rocketID);
		DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
		auto protoOwnDevParam = DeviceDBConfigInfo::getInstance()->customReadInfoMap;

		deviceParamTree->clear();
		deviceParamTree->setColumnCount(2);
		deviceParamTree->headerItem()->setHidden(true);

		hadSelectedParamsL->clear();
		for (pair<int, vector< string>> ele1 : allDev2Param)
		{
			QTreeWidgetItem* eachItem = new QTreeWidgetItem;

			eachItem->setData(0, Qt::UserRole, ele1.first);
			eachItem->setText(0, QString::fromStdString(ele1.second[1]));
			eachItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
			QList<QTreeWidgetItem*> childList;
			for (int i = 0; i < ele1.second.size() / 4; i++)
			{
				QTreeWidgetItem* subItem = new QTreeWidgetItem;
				int tmpDevParamId = atoi(ele1.second[2 + 4 * i].c_str());
				subItem->setData(1, Qt::UserRole, tmpDevParamId);
				if (!protoOwnDevParam[tmpDevParamId].empty())
				{
					subItem->setCheckState(1, Qt::Checked);
					subItem->setSelected(true);
					QListWidgetItem* selctDevParamItem = new QListWidgetItem(QString::fromStdString(ele1.second[3 + 4 * i]));
					selctDevParamItem->setData(Qt::UserRole, tmpDevParamId);
					hadSelectedParamsL->addItem(selctDevParamItem);
				}
				else
				{
					subItem->setCheckState(1, Qt::Unchecked);
					subItem->setSelected(false);
				}
				subItem->setText(1, QString::fromStdString(ele1.second[3 + 4 * i]));

				childList.append(subItem);
			}
			eachItem->addChildren(childList);
			deviceParamTree->addTopLevelItem(eachItem);
		}
		deviceParamTree->expandAll();
	}
#endif  __0__


	//������ʱ����µ�Э����rocket_data��
	connect(addCommuProto, &QPushButton::pressed, this, [=]() {

		//���
		if (rocketSearch->text().isEmpty())
		{
			QMessageBox::warning(this, QString("����"), QString("Э��������Ϊ��"));

			return;
		}

		//��������ͬЭ����
		auto ccc = rocketComProtoList->count();
		for (int protoIndex = 0; protoIndex < rocketComProtoList->count(); protoIndex++)
		{
			if (rocketComProtoList->item(protoIndex)->text() == rocketSearch->text())
			{
				rocketSearch->clear();
				QMessageBox::warning(this, QString("����"), QString("Э�����ظ�"));
				return;
			}
		}
		//�ȼ�����ͬһ����ͺ��Ƿ������ͬcode
		DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
			rocket_data_info.id,\
			rocket_data_info.`name`,\
			rocket_data_info.`code`,\
			rocket_data_info.createTime,\
			rocket_data_info.lastUpdateTime\
			FROM\
			rocket_data_info\
			WHERE\
			rocket_data_info.rocket_id = %1").arg(rocketID));
		QVector<int> thisRocketCodes;
		for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
		{
			thisRocketCodes.push_back(atoi(ele.second[2].c_str()));
		}
		int addCode = 1;
		for (int i = 1; i < 255; i++)
		{
			if (thisRocketCodes.count(i) == 0)
			{
				addCode = i;
				break;
			}
		}
		DeviceDBConfigInfo::getInstance()->rocketDataInfo2DB(rocketID, rocketSearch->text(), addCode, 0x55aa);
		DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
			rocket_data_info.id,\
			rocket_data_info.`name`,\
			rocket_data_info.`code`,\
			rocket_data_info.createTime,\
			rocket_data_info.lastUpdateTime\
			FROM\
			rocket_data_info\
			WHERE\
			rocket_data_info.rocket_id = %1").arg(rocketID));
		int rocketDataId = 0;
		for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
		{
			if (addCode == atoi(ele.second[2].c_str()))
			{
				rocketDataId = ele.first;
				break;
			}
		}
		QListWidgetItem* tmpItem = new QListWidgetItem(rocketSearch->text());
		tmpItem->setData(Qt::UserRole, rocketDataId);//rocket_data��ID
		rocketComProtoList->addItem(tmpItem);

		rocketSearch->clear();
		});


	//�����״�ṹ
	connect(deviceParamTree, &QTreeWidget::itemClicked, this, [=](QTreeWidgetItem* item1, int column) {
		//����������
		auto sss = item1->text(column);
		int c = item1->childCount();
		if (column == 0)
		{
			return;
		}
		//Ϊ��СҶ��ʱ
		if (item1->childCount() == 0)
		{
			auto itemText1 = item1->text(column);
			auto itemstate1 = item1->checkState(column);
			if (item1->checkState(column) == Qt::Checked)
			{

				item1->setCheckState(column, Qt::Unchecked);
				item1->setSelected(false);
				auto findItems = hadSelectedParamsL->findItems(item1->text(column), Qt::MatchExactly);
				for (auto itemL : findItems)
				{
					hadSelectedParamsL->takeItem(hadSelectedParamsL->row(itemL));
				}
				return;
			}
			else
			{
				auto findItems = hadSelectedParamsL->findItems(item1->text(column), Qt::MatchExactly);
				if (!findItems.isEmpty())
				{
					item1->setCheckState(column, Qt::Unchecked);
					for (auto itemL : findItems)
					{
						hadSelectedParamsL->takeItem(hadSelectedParamsL->row(itemL));
					}
					return;
				}
				item1->setCheckState(column, Qt::Checked);
				item1->setSelected(true);
				QListWidgetItem* tmpitem = new QListWidgetItem(item1->text(column));
				tmpitem->setData(Qt::UserRole, item1->data(column, Qt::UserRole).toInt());

				hadSelectedParamsL->insertItem(0, tmpitem);
			}
		}
		//��Ҷ��
		for (int childIndex = 0; childIndex < item1->childCount(); childIndex++)
		{
			if (item1->child(childIndex)->checkState(column) == Qt::Checked)
			{
				item1->child(childIndex)->setCheckState(column, Qt::Unchecked);
				item1->child(childIndex)->setSelected(false);
				auto findItems = hadSelectedParamsL->findItems(item1->child(childIndex)->text(column), Qt::MatchExactly);
				for (auto itemL : findItems)
				{
					hadSelectedParamsL->takeItem(hadSelectedParamsL->row(itemL));
				}
			}
			else
			{
				item1->child(childIndex)->setCheckState(column, Qt::Checked);
				item1->child(childIndex)->setSelected(true);
				QListWidgetItem* tmpitem = new QListWidgetItem(item1->child(childIndex)->text(column));
				tmpitem->setData(Qt::UserRole, item1->child(childIndex)->data(column, Qt::UserRole).toInt());
				auto findItems = hadSelectedParamsL->findItems(item1->child(childIndex)->text(column), Qt::MatchExactly);
				if (findItems.isEmpty())
				{
					hadSelectedParamsL->insertItem(0, tmpitem);
				}
			}
		}
		//hadSelectedParamsL->update();
		});



	//�л����Э����
	connect(rocketComProtoList, &QListWidget::itemSelectionChanged, this, [=]() {
		deviceParamTree->clearSelection();
		deviceParamTree->expandAll();
		hadSelectedParamsL->clear();

		int curSlcRow = rocketComProtoList->currentRow();
		QString qSqlString22 = QString("SELECT\
			rockect_param_info.device_parameter_id,\
			rockect_param_info.createTime,\
			rockect_param_info.lastUpdateTime\
			FROM\
			rockect_param_info\
			INNER JOIN rocket_data_info ON rocket_data_info.id = rockect_param_info.rocket_data_id\
			WHERE\
			rockect_param_info.rocket_data_id = %1 AND\
		rocket_data_info.rocket_id = %2").arg(rocketComProtoList->item(curSlcRow)->data(Qt::UserRole).toInt()).arg(rocketID);
		DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString22);
		auto curSlcProtoInfo = DeviceDBConfigInfo::getInstance()->customReadInfoMap;

		for (int i = 0; i < deviceParamTree->topLevelItemCount(); i++)
		{
			//deviceParamTree->topLevelItem(i)->setCheckState(0, Qt::Checked);
			for (int j = 0; j < deviceParamTree->topLevelItem(i)->childCount(); j++)
			{
				if (curSlcProtoInfo[deviceParamTree->topLevelItem(i)->child(j)->data(1, Qt::UserRole).toInt()].empty())
				{
					deviceParamTree->topLevelItem(i)->child(j)->setCheckState(1, Qt::Unchecked);
					deviceParamTree->topLevelItem(i)->child(j)->setSelected(false);
				}
				else
				{
					deviceParamTree->topLevelItem(i)->child(j)->setCheckState(1, Qt::Checked);
					deviceParamTree->topLevelItem(i)->child(j)->setSelected(true);
					QListWidgetItem* selctDevParam = new QListWidgetItem(deviceParamTree->topLevelItem(i)->child(j)->text(1));
					selctDevParam->setData(Qt::UserRole, deviceParamTree->topLevelItem(i)->child(j)->data(1, Qt::UserRole).toInt());
					hadSelectedParamsL->addItem(selctDevParam);
				}

			}
		}

		});
	//ȫѡ��ѡ��
	connect(selectAllBox, &QCheckBox::stateChanged, this, [=](int stat) {
		hadSelectedParamsL->clear();
		if (stat == Qt::Checked)
		{

			deviceParamTree->expandAll();
			qDebug() << "ȫѡ:" << deviceParamTree->topLevelItemCount();
			for (int i = 0; i < deviceParamTree->topLevelItemCount(); i++)
			{
				//deviceParamTree->topLevelItem(i)->setCheckState(0, Qt::Checked);
				for (int j = 0; j < deviceParamTree->topLevelItem(i)->childCount(); j++)
				{
					deviceParamTree->topLevelItem(i)->child(j)->setSelected(true);
					deviceParamTree->topLevelItem(i)->child(j)->setCheckState(1, Qt::Checked);
					QListWidgetItem* tmpitem = new QListWidgetItem(deviceParamTree->topLevelItem(i)->child(j)->text(1));
					tmpitem->setData(Qt::UserRole, deviceParamTree->topLevelItem(i)->child(j)->data(1, Qt::UserRole).toInt());
					hadSelectedParamsL->addItem(tmpitem);
				}
			}
			selectAllBox->setText("ȡ��ȫѡ");
		}
		else
		{
			for (int i = 0; i < deviceParamTree->topLevelItemCount(); i++)
			{
				//deviceParamTree->topLevelItem(i)->setCheckState(0, Qt::Unchecked);
				for (int j = 0; j < deviceParamTree->topLevelItem(i)->childCount(); j++)
				{
					deviceParamTree->topLevelItem(i)->child(j)->setSelected(false);
					deviceParamTree->topLevelItem(i)->child(j)->setCheckState(1, Qt::Unchecked);
				}
			}

			selectAllBox->setText("ȫѡ");
		}
		});

	//�Ҳ��б� �����϶��ı�˳��
	hadSelectedParamsL->setDragDropMode(QAbstractItemView::InternalMove);
	hadSelectedParamsL->setAcceptDrops(true);


	windowTitle->setText("�������ͨ��Э������");

	rocketWidget->show();

	deviceWidget->hide();
	commandWidget->hide();
}

/**
	@brief �����豸�������

**/
void AllInfoConfigWidget::loadDeviceInfoData() {
	deviceCfgList->clear();

	//list
	deviceCfgList->addItem(QString("    �豸״̬"));
	deviceCfgList->addItem(QString("    �豸������"));
	deviceCfgList->item(0)->setSelected(true);
	deviceCfgList->setCurrentRow(0);
	deviceCfgList->setFocusPolicy(Qt::NoFocus);
	//����ʱĬ����ʾ״̬����
	deviceStatContentUpdate();
	QString ss = QString("QListWidget{border:none;background-color:rgba(249,249,249,1);font:΢���ź� 14px bold;text-align:center;}\
				QListWidget::item{border-radius:5px;padding:10px;image:url(:/icon/icon/bb.png);image-position:left;margin:0 0 10 0;min-height:30px;}\
				QListWidget::item:hover{image:url(:/icon/icon/ww.png);background-color:rgba(63,144,255,1);}\
				QListWidget::item:selected{image:url(:/icon/icon/ww.png);background-color:rgba(63,144,255,1);}");

	deviceCfgList->setStyleSheet(ss);
	deviceStatContentUpdate();
	deviceParamContentUpdate();

	connect(deviceCfgList, &QListWidget::currentTextChanged,
		[=](const QString& text) {
			if (text == QString("    �豸״̬"))
			{
				//deviceStatContentUpdate();

				scrollAreaStat->show();//����ʱĬ����ʾ
				searchDevParam->hide();
				scrollAreaParam->hide();
			}
			else
			{
				//deviceParamContentUpdate();

				scrollAreaStat->hide();//����ʱĬ�ϲ���ʾ
				searchDevParam->show();
				scrollAreaParam->show();

			}
		});


	//�豸��ʾ
	rocketWidget->hide();
	commandWidget->hide();
	deviceWidget->show();
	scrollAreaParam->hide();
	searchDevParam->hide();

	windowTitle->setText(QString("-�豸����"));
}

/**
	@brief �豸״̬����
**/
void AllInfoConfigWidget::deviceStatContentUpdate() {
	QGridLayout* rightVBox = new QGridLayout;
	QGridLayout* rightGird = new QGridLayout;
	//rightGird->setContentsMargins(120, 40, 20, 60);
	rightGird->setContentsMargins(0, 0, 0, 0);
	int rightGridColumnC = 1;
	int curInd = 0;
	deviceStatList.clear();
	DeviceDBConfigInfo::getInstance()->readStatusInfoDB2UI();

	for (auto ele : DeviceDBConfigInfo::getInstance()->statusInfo)
	{
		QWidget* widget = new QWidget;
		//widget->setFixedWidth(330);
		widget->setObjectName("widget");
		QHBoxLayout* statusHbox = new QHBoxLayout;
		QLabel* lab = new QLabel(QString::fromStdString(ele.second[1]));
		lab->setFixedWidth(200);
		lab->setProperty("id", ele.first);
		lab->setObjectName("status");
		statusHbox->addWidget(lab);

		QLabel* fileLab = new QLabel;
		fileLab->setFixedWidth(500);
		fileLab->setFixedHeight(30);
		fileLab->setObjectName("fileLab");
		fileLab->setAlignment(Qt::AlignVCenter);
		statusHbox->addWidget(fileLab);
		statusHbox->addStretch(1);
		QPushButton* fileSlctBtn = new QPushButton(QString("ѡ���ļ�"));
		statusHbox->addWidget(fileSlctBtn);

		statusHbox->addStretch(1);

		QCheckBox* statusCheck = new QCheckBox;
		statusHbox->addStretch(1);
		statusHbox->addWidget(statusCheck);
		statusCheck->setObjectName("checkbox");

		widget->setLayout(statusHbox);
		rightGird->addWidget(widget, curInd / rightGridColumnC, curInd % rightGridColumnC);
		rightGird->setRowStretch(curInd / rightGridColumnC, 1);
		widget->setStyleSheet(QString("QWidget#widget{width:330px;height:58px;border:1px solid black;border-radius:5px;background-color:rgba(255,255,255,1);}"));
		deviceStatList.push_back(widget);
		widget->setFixedHeight(40);
		curInd++;

		connect(fileSlctBtn, &QPushButton::clicked, this, [=]() {
			//���˸�ʽtxt
			QString filepath1 = QFileDialog::getOpenFileName(nullptr, QString(), QString(), QString(tr("*.txt")));
			if (filepath1.isEmpty())
			{
				return;
			}
			filepath1 = filepath1.split("/device/")[1];
			fileLab->setWordWrap(true);
			fileLab->setText(filepath1);
			});

	}

	rightGird->setVerticalSpacing(20);

	addNewStat = new QPushButton;
	addNewStat->setStyleSheet(QString("QPushButton{width:330px;height:58px;border:1px solid gray;border-radius:5px;background-color:rgba(255,255,255,1);image:url(:/icon/icon/+hao.png);}"));
	addNewStat->setFixedHeight(40);
	addNewStat->setFixedWidth(330);
	QHBoxLayout* hwbox = new QHBoxLayout;
	hwbox->addWidget(addNewStat);
	hwbox->setContentsMargins(0, 40, 20, 0);
	rightVBox->setContentsMargins(60, 40, 20, 60);
	rightVBox->addLayout(rightGird, 0, 0, 3, 2);
	rightVBox->addLayout(hwbox, 4, 0, 1, 1);
	QWidget* scrollAreaStatContent = new QWidget;
	scrollAreaStatContent->setLayout(rightVBox);
	scrollAreaStat->setWidget(scrollAreaStatContent);
	scrollAreaStatContent->setObjectName("content");

	scrollAreaStat->setWidgetResizable(true);

	//�豸����״̬��
	//���ݵ�ǰ�豸����ѡ����
	QString qSqlString = QString("SELECT\
		device_status_info.status_id,\
		device_status_info.dataPath,\
		device_status_info.createTime,\
		device_status_info.lastUpdateTime\
		FROM\
		device_status_info\
		INNER JOIN device_info ON device_status_info.device_id = device_info.id\
		WHERE\
		device_status_info.device_id = %1 AND\
		device_info.rocket_id = %2").arg(deviceID).arg(AppCache::instance()->m_CurrentRocketType->m_id);

	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
	auto deviceHadStatInfo = DeviceDBConfigInfo::getInstance()->customReadInfoMap;

	for (auto firW : scrollAreaStat->widget()->children())
	{
		QCheckBox* checkBox = firW->findChild<QCheckBox*>("checkbox");
		if (checkBox == nullptr)
		{
			continue;
		}
		int statId = firW->findChild<QLabel*>("status")->property("id").toInt();
		QLabel* fileLab1 = firW->findChild<QLabel*>("fileLab");
		if (!deviceHadStatInfo[statId].empty())
		{
			checkBox->setCheckState(Qt::Checked);
			fileLab1->setText(QString::fromLocal8Bit(deviceHadStatInfo[statId][1].c_str()));
		}
		else
		{
			checkBox->setCheckState(Qt::Unchecked);
		}
	}

	connect(addNewStat, &QPushButton::clicked, this, [=]() {
		//��Ҫ����һ��״̬��
		QWidget* newStatW = new QWidget;
		QVBoxLayout* newStatWLayout = new QVBoxLayout;
		QLabel* statName = new QLabel(QString("״̬��:"));
		QLineEdit* lineStat = new QLineEdit;
		QPushButton* okStat = new QPushButton(QString("ȷ��"));
		newStatWLayout->addWidget(statName);
		newStatWLayout->addWidget(lineStat);
		newStatWLayout->addWidget(okStat);
		newStatW->setLayout(newStatWLayout);
		newStatW->show();

		connect(okStat, &QPushButton::clicked, this, [=]() {
			newStatW->deleteLater();

			QWidget* widget = new QWidget;
			//widget->setFixedWidth(330);
			widget->setObjectName("widget");
			QHBoxLayout* statusHbox = new QHBoxLayout;
			QLabel* statLabb = new QLabel(lineStat->text());
			statLabb->setObjectName("status");
			statLabb->setFixedWidth(200);
			statusHbox->addWidget(statLabb);

			QLabel* fileLab = new QLabel;
			fileLab->setFixedWidth(500);
			fileLab->setObjectName("fileLab");
			statusHbox->addWidget(fileLab);
			statusHbox->addStretch(1);
			QPushButton* fileSlctBtn = new QPushButton(QString("ѡ���ļ�"));
			statusHbox->addWidget(fileSlctBtn);

			statusHbox->addStretch(1);

			QCheckBox* statusCheck = new QCheckBox;

			statusHbox->addWidget(statusCheck);
			statusCheck->setObjectName("checkbox");
			widget->setFixedHeight(40);
			widget->setLayout(statusHbox);
			widget->setStyleSheet(QString("QWidget#widget{width:330px;height:58px;border:1px solid black;border-radius:5px;background-color:rgba(255,255,255,1);}"));
			rightGird->addWidget(widget);
			//ͬ�������ݱ�
			DeviceDBConfigInfo::getInstance()->customRunSql(QString("INSERT INTO `simulatedtraining`.`status_info`(`name`) VALUES ('%1')").arg(lineStat->text()));
			//��������״̬ID����Ϊ����
			DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT status_info.id,status_info.createTime,status_info.lastUpdateTime FROM status_info WHERE status_info.`name` = '%1'").arg(lineStat->text()));
			auto onlyData = DeviceDBConfigInfo::getInstance()->customReadInfoMap.begin();
			statLabb->setProperty("id", onlyData->first);

			connect(fileSlctBtn, &QPushButton::clicked, this, [=]() {
				//���˸�ʽtxt
				QString filepath1 = QFileDialog::getOpenFileName(nullptr, QString(), QString(), QString(tr("*.txt")));
				if (filepath1.isEmpty())
				{
					return;
				}
				filepath1 = filepath1.split("/device/")[1];
				fileLab->setWordWrap(true);
				fileLab->setText(filepath1);
				});

			});


		});

}



/**
	@brief �豸�������ݸ���
**/
void AllInfoConfigWidget::deviceParamContentUpdate() {
	QGridLayout* rightGird2 = new QGridLayout;
	rightGird2->setContentsMargins(60, 40, 20, 60);
	int rightGridColumnC2 = 1;
	int curInd2 = 0;
	deviceParamList.clear();
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(QString("SELECT\
		parameter_info.id,\
		parameter_info.`name`,\
		parameter_info.createTime,\
		parameter_info.lastUpdateTime\
		FROM\
		parameter_info\
		INNER JOIN parameter_rocket_info ON parameter_info.id = parameter_rocket_info.parameter_id\
		WHERE\
		parameter_rocket_info.rocket_id = %1\
		").arg(AppCache::instance()->m_CurrentRocketType->m_id));

	for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
	{
		QWidget* widget = new QWidget;
		widget->setObjectName("widget");
		//widget->setFixedSize(600, 40);
		widget->setFixedHeight(40);
		QHBoxLayout* statusHbox = new QHBoxLayout;
		QLabel* lab = new QLabel(QString::fromStdString(ele.second[1]));
		lab->setObjectName("paramlab");
		lab->setProperty("id", ele.first);
		statusHbox->addWidget(lab);

		//QPushButton* selectFileBtn = new QPushButton(QString("���ļ�"));

		QCheckBox* statusCheck = new QCheckBox;
		statusCheck->setObjectName("checkbox");

		statusHbox->addStretch(1);
		statusHbox->addWidget(statusCheck);
		//statusHbox->setAlignment(Qt::Alignm);

		widget->setLayout(statusHbox);
		rightGird2->addWidget(widget, curInd2 / rightGridColumnC2, curInd2 % rightGridColumnC2);
		rightGird2->setRowStretch(curInd2 / rightGridColumnC2, 1);
		widget->setStyleSheet(QString("QWidget#widget{width:600px;height:40px;border:1px solid black;border-radius:5px;background-color:rgba(255,255,255,1);}"));
		deviceParamList.push_back(widget);

		curInd2++;
	}
	rightGird2->setRowStretch(curInd2, 1);
	QWidget* scrollAreaParamContent = new QWidget;
	//
	rightGird2->setVerticalSpacing(20);
	scrollAreaParamContent->setLayout(rightGird2);
	scrollAreaParam->setWidget(scrollAreaParamContent);

	//�豸���в�����
	QString qSqlString11 = QString("SELECT\
		device_param_info.parameter_id,\
		device_param_info.createTime,\
		device_param_info.lastUpdateTime\
		FROM\
		device_param_info\
		WHERE\
		device_param_info.device_id = %1").arg(deviceID);
	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString11);
	auto deviceHadParamInfo = DeviceDBConfigInfo::getInstance()->customReadInfoMap;

	for (auto firW : scrollAreaParam->widget()->children())
	{
		QCheckBox* checkBox = firW->findChild<QCheckBox*>("checkbox");
		if (checkBox == nullptr)
		{
			continue;
		}
		int paramId = firW->findChild<QLabel*>("paramlab")->property("id").toInt();
		if (!deviceHadParamInfo[paramId].empty())
		{
			checkBox->setCheckState(Qt::Checked);
		}
		else
		{
			checkBox->setCheckState(Qt::Unchecked);
		}
	}
}

void AllInfoConfigWidget::loadCmdInfoData() {
	cmdCfgList->clear();
	//list
	cmdCfgList->addItem(QString("    ָ���豸״̬��"));
	cmdCfgList->addItem(QString("    ֡��������"));

	cmdCfgList->item(0)->setSelected(true);
	cmdCfgList->setCurrentRow(0);

	//ָ���豸״̬
	QString qSqlStringCmd = QString("SELECT\
		command_devicestatus_info.deviceStatus_id,\
		device_info.`name`,\
		status_info.`name`,\
		command_devicestatus_info.createTime,\
		command_devicestatus_info.lastUpdateTime\
		FROM\
		command_devicestatus_info\
		INNER JOIN command_info ON command_info.id = command_devicestatus_info.command_id\
		INNER JOIN device_status_info ON command_devicestatus_info.deviceStatus_id = device_status_info.id\
		INNER JOIN device_info ON device_status_info.device_id = device_info.id\
		INNER JOIN status_info ON device_status_info.status_id = status_info.id\
		WHERE\
		command_info.rocket_id = %1 AND\
		command_devicestatus_info.command_id = %2").arg(rocketID).arg(cmdID);

	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlStringCmd);
	cmdDevStatInfo.clear();
	cmdDevStatInfo = DeviceDBConfigInfo::getInstance()->customReadInfoMap;
	for (auto ele : cmdDevStatInfo)
	{
		QListWidgetItem* listImte = new QListWidgetItem;
		listImte->setText(QString::fromStdString(ele.second[1]) + QString::fromStdString(ele.second[2]));
		listImte->setData(Qt::UserRole, ele.first);
		hadSelectDevStat->addItem(listImte);
	}

	connect(deviceCombox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
		QWidget* devStatComtents = new QWidget;

		int deviceId = deviceCombox->itemData(index).toInt();

		//���豸״̬���в�ѯ
		QString qSqlString = QString("SELECT\
			device_status_info.status_id,\
			status_info.`name`,\
			device_status_info.id,\
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

		for (auto ele : DeviceDBConfigInfo::getInstance()->customReadInfoMap)
		{

			QWidget* widget = new QWidget;
			widget->setObjectName("widget");
			QHBoxLayout* statusHbox = new QHBoxLayout;
			QLabel* devStatLab = new QLabel(QString::fromStdString(ele.second[1]));
			devStatLab->setObjectName("devStatLab");

			int devStatId1 = atoi(ele.second[2].c_str());
			devStatLab->setProperty("devStatId", devStatId1);
			statusHbox->addWidget(devStatLab);

			QCheckBox* statusCheck = new QCheckBox;

			if (cmdDevStatInfo[devStatId1].empty())
			{
				statusCheck->setCheckState(Qt::Unchecked);
			}
			else
			{
				statusCheck->setCheckState(Qt::Checked);
				QListWidgetItem* cmdDevStatitem = new QListWidgetItem;
				cmdDevStatitem->setText(deviceCombox->currentText() + QString::fromStdString(ele.second[1]));
				cmdDevStatitem->setData(Qt::UserRole, devStatId1);
				if (hadSelectDevStat->findItems(cmdDevStatitem->text(), Qt::MatchExactly).size() == 0)
				{
					hadSelectDevStat->addItem(cmdDevStatitem);
				}
			}

			for (int tmpi = 0; tmpi < hadSelectDevStat->count(); tmpi++)
			{
				if (hadSelectDevStat->item(tmpi)->data(Qt::UserRole) == devStatId1)
				{
					statusCheck->setCheckState(Qt::Checked);
				}
			}

			statusCheck->setObjectName("checkbox");
			statusHbox->addSpacing(160);
			statusHbox->addStretch(1);
			statusHbox->addWidget(statusCheck);

			widget->setLayout(statusHbox);
			contentsLayout->addWidget(widget, curInd / rightGridColumnC, curInd % rightGridColumnC);
			contentsLayout->setRowStretch(curInd / rightGridColumnC, 1);
			widget->setStyleSheet(QString("QWidget#widget{width:330px;height:58px;border:1px solid black;border-radius:5px;background-color:rgba(255,255,255,1);}"));

			widget->setFixedHeight(58);
			widget->setFixedWidth(220);
			curInd++;

			connect(statusCheck, &QCheckBox::stateChanged, this, [=](int stat1) {
				if (stat1 == Qt::Checked)
				{
					QListWidgetItem* tmpItem11 = new QListWidgetItem(deviceCombox->currentText() + QString::fromStdString(ele.second[1]));
					tmpItem11->setData(Qt::UserRole, atoi(ele.second[2].c_str()));
					if (hadSelectDevStat->findItems(tmpItem11->text(), Qt::MatchExactly).size() == 0)
					{
						hadSelectDevStat->addItem(tmpItem11);
					}

				}
				else
				{
					auto findItems = hadSelectDevStat->findItems(deviceCombox->currentText() + QString::fromStdString(ele.second[1]), Qt::MatchExactly);
					for (auto itemL : findItems)
					{
						hadSelectDevStat->takeItem(hadSelectDevStat->row(itemL));
					}
				}
				});
		}
		contentsLayout->setVerticalSpacing(40);
		contentsLayout->setHorizontalSpacing(40);
		//rightTop1Layout->setContentsMargins(60, 0, 20, 60);
		contentsLayout->setContentsMargins(60, 20, 20, 60);
		devStatComtents->setLayout(contentsLayout);
		scrollAreaDevStat->setWidget(devStatComtents);

		});

	//ָ��֡����
	qSqlStringCmd = QString("SELECT\
		command_param_info.id,\
		command_param_info.`name`,\
		command_param_info.length,\
		command_param_info.defaultValue,\
		command_param_info.resultType,\
		command_param_info.createTime,\
		command_param_info.lastUpdateTime\
		FROM\
		command_param_info\
		INNER JOIN command_info ON command_param_info.command_id = command_info.id\
		WHERE\
		command_info.rocket_id = %1 AND\
		command_param_info.command_id = %2").arg(rocketID).arg(cmdID);

	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlStringCmd);
	auto curCmdFrame = DeviceDBConfigInfo::getInstance()->customReadInfoMap;
	for (auto framContent : curCmdFrame)
	{
		int insertRoww = cmdFrameTable->rowCount();
		cmdFrameTable->insertRow(insertRoww);
		cmdFrameTable->setItem(insertRoww, 0, new QTableWidgetItem(QString::fromLocal8Bit(framContent.second[1].c_str())));
		cmdFrameTable->setItem(insertRoww, 1, new QTableWidgetItem(QString::fromLocal8Bit(framContent.second[2].c_str())));
		cmdFrameTable->setItem(insertRoww, 2, new QTableWidgetItem(QString::fromLocal8Bit(framContent.second[3].c_str())));
		cmdFrameTable->setItem(insertRoww, 3, new QTableWidgetItem(QString::fromLocal8Bit(framContent.second[4].c_str())));

	}


	cmdCfgList->setFocusPolicy(Qt::NoFocus);

	QString ss = QString("QListWidget{border:none;background-color:rgba(249,249,249,1);font:΢���ź� 14px bold;text-align:center;}\
				QListWidget::item{border-radius:5px;padding:10px;image:url(:/icon/icon/bb.png);image-position:left;margin:0 0 10 0;min-height:30px;}\
				QListWidget::item:hover{image:url(:/icon/icon/ww.png);background-color:rgba(63,144,255,1);}\
				QListWidget::item:selected{image:url(:/icon/icon/ww.png);background-color:rgba(63,144,255,1);}");

	cmdCfgList->setStyleSheet(ss);
	//�л�
	connect(cmdCfgList, &QListWidget::currentTextChanged,
		[=](const QString& text) {
			if (text == QString("    ָ���豸״̬��"))
			{
				scrollAreaDevStat->show();//����ʱĬ�ϲ���ʾ
				deviceCombox->show();
				deviceLabel->show();
				hadSelectDevStat->show();

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


				scrollAreaDevStat->hide();//����ʱĬ�ϲ���ʾ
				deviceCombox->hide();
				deviceLabel->hide();
				hadSelectDevStat->hide();
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


	windowTitle->setText(QString("-ָ������"));

	rocketWidget->hide();
	deviceWidget->hide();
	commandWidget->show();
}

/**
	@brief ��ʼ�����沼��
**/
void AllInfoConfigWidget::InitUILayout() {
	this->setFixedSize(QSize(1200, 940));

	this->setStyleSheet("*{background-color:rgba(255,255,255,1);padding:2px;}\
			QLineEdit{background-color:rgba(238, 240, 243, 1);border:2px groove gray;border-radius:10px;padding:2px 4px;font: 12pt ΢���ź�;}\
			QLabel{font: 12pt ΢���ź�;}\
			QCheckBox{}\
			QComboBox{border: 1px solid darkgray;font: 10pt Arial;selection - background - color: rgb(0, 170, 255);font: 12pt ΢���ź�; }");


	//�����ϲ�
	windowIcon = new QLabel;
	windowTitle = new QLabel("test");
	windowCloseBtn = new QPushButton;

	//�����²�
	cancelBtn = new QPushButton(QString("ȡ��"));
	okBtn = new QPushButton(QString("ȷ��"));
	/*!
	 *  Inits the u i layout.����ͺ�
	 */
	 //���
	rocketSearch = new QLineEdit;
	rocketSearch->setPlaceholderText(QString("����/����Э��"));
	addCommuProto = new QPushButton;
	addCommuProto->setStyleSheet(QString("QPushButton{min-height:30px;border:none;image:url(:/icon/icon/+hao.png);}"));
	addCommuProto->setAutoRepeat(true);
	addCommuProto->setAutoRepeatInterval(100);
	rocketComProtoList = new QListWidget;
	//rocketComProtoList->setStyleSheet(WidgetStyleSheet::getInstace()->deviceManageListSS.arg(":/icon/icon/ww.png").arg(":/icon/icon/bb.png"));
	rocketComProtoList->setStyleSheet(QString("QListWidget{border:none;background-color:rgba(249,249,249,1);font:΢���ź� 14px bold;text-align:center;border-right-color:rgb(142,145,145);}\
				QListWidget::item{border-radius:3px;padding:5px;margin:0 0 10 0;min-height:20px;focus:NoFocus;}\
				QListWidget::item:hover{background-color:rgba(63,144,255,1);}\
				QListWidget::item:selected{background-color:rgba(63,144,255,1);}"));
	rocketComProtoList->setFocusPolicy(Qt::NoFocus);
	rocketComProtoList->setContentsMargins(0, 0, 0, 0);
	rocketTypeParamTitle = new QLabel(QString("�ͺŲ���"));
	searchLineEdit = new QLineEdit;
	searchLineEdit->setStatusTip(QString("����"));

	deviceParamTree = new QTreeWidget;
	selectAllBox = new QCheckBox;
	selectAllBox->setText(QString("ȫѡ"));
	cancelAllBox = new QCheckBox;
	cancelAllBox->setText(QString("��ѡ"));
	cancelAllBox->hide();
	//�Ҳ�
	selectParamTitle = new QLabel(QString("��ѡ�����"));
	searchSelect = new QLineEdit;
	searchSelect->setStatusTip(QString("����"));

	hadSelectedParamsL = new QListWidget;

	/*!
	 *  Inits the u i layout.�豸����
	 */
	 //��
	searchDeviceCfg = new QLineEdit;
	searchDeviceCfg->setPlaceholderText(QString("����"));
	addDeviceCfg = new QPushButton;
	deviceCfgList = new QListWidget;
	//��1
	devStatusLayout = new QGridLayout;
	//��2
	searchDevParam = new QLineEdit;
	searchDevParam->setPlaceholderText(QString("����"));
	devParamLayout = new QGridLayout;

	/*!
	 *  Inits the u i layout.ָ������
	 */
	 //���
	searchCmdCfg = new QLineEdit;
	searchCmdCfg->setPlaceholderText(QString("����"));
	addCmdCfg = new QPushButton;
	cmdCfgList = new QListWidget;
	//��1
	deviceLabel = new QLabel(QString("�豸"));
	deviceCombox = new QComboBox;
	hadSelectDevStat = new QListWidget;
	deviceStatLayout = new QGridLayout;
	//��2
	searchCmdFrame = new QLineEdit;
	searchCmdFrame->setPlaceholderText(QString("����"));

	cmdFrameLabel = new QLabel(QString("ָ��֡�����б�"));
	addCmdFrame = new QPushButton(QString("����"));

	addCmdFrame->setStyleSheet("\
				\
				QPushButton:hover{background-color:transparent;\
					border-image:url(:/DeviceManager/bt_suspension.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;\
					}\
				QPushButton:pressed{\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_disable.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;}\
				QPushButton{\
					height:30px;width:60px;\
					background-color:transparent;\
					border-image:url(:/DeviceManager/bt_normal.png);\
					color:rgb(255,255,255);\
					font:12pt ΢���ź�;\
					border:0px;\
					}\
		");

	hadSelectedParamsL->setStyleSheet(QString("QListWidget{border:none;background-color:rgba(249,249,249,1);font:΢���ź� 14px bold;text-align:center;border-right-color:rgb(142,145,145);}\
				QListWidget::item{border-radius:3px;padding:5px;margin:0 0 10 0;min-height:20px;focus:NoFocus;}\
				QListWidget::item:hover{background-color:rgba(63,144,255,1);}\
				QListWidget::item:selected{background-color:rgba(63,144,255,1);}"));
	hadSelectedParamsL->setFocusPolicy(Qt::NoFocus);

	hadSelectDevStat->setStyleSheet(QString("QListWidget{border:none;background-color:rgba(249,249,249,1);font:΢���ź� 14px bold;text-align:center;border-right-color:rgb(142,145,145);}\
				QListWidget::item{border-radius:3px;padding:5px;margin:0 0 10 0;min-height:20px;focus:NoFocus;}\
				QListWidget::item:hover{background-color:rgba(63,144,255,1);}\
				QListWidget::item:selected{background-color:rgba(63,144,255,1);}"));
	hadSelectDevStat->setFocusPolicy(Qt::NoFocus);

	cmdFrameTable = new QTableWidget;
	QStringList tableHeaders;
	tableHeaders << QString("֡��������") << QString("֡�����ֽڳ���") << QString("֡��������") << QString("֡����Ĭ��ֵ") << QString("����");
	cmdFrameTable->setColumnCount(tableHeaders.size());
	cmdFrameTable->horizontalHeader()->setMinimumHeight(40);
	cmdFrameTable->horizontalHeader()->setStyleSheet("font: 14px ΢���ź� bold;");
	cmdFrameTable->horizontalHeader()->setStretchLastSection(true);
	cmdFrameTable->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);

	cmdFrameTable->verticalHeader()->sectionResizeMode(QHeaderView::Interactive);
	cmdFrameTable->setHorizontalHeaderLabels(tableHeaders);
	cmdFrameTable->horizontalHeader()->show();
	cmdFrameTable->verticalHeader()->show();
	cmdFrameTable->verticalHeader()->setMinimumWidth(40);

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
	@brief	  ���ݴ���Ĵ������͸���UI
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
	@brief	  ��ʾҳ����
	@param rocketId -����ͺ�ID
	@param deviceId -�豸ID
	@param cmdId    -ָ��ID
**/
void AllInfoConfigWidget::setInfoWidgetCfg(int rocketId, int deviceId, int cmdId) {
	rocketID = rocketId;
	deviceID = deviceId;
	cmdID = cmdId;
	hadSelectDevStat->clear();
}

/**
	@brief
**/
void AllInfoConfigWidget::widgetConfig() {

}

/**
	@brief ��ʼ���ź����
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

	//����֡����
	connect(addCmdFrame, &QPushButton::clicked, this, [=]() {
		int curRow = cmdFrameTable->rowCount();
		cmdFrameTable->insertRow(curRow);
		cmdFrameTable->setItem(curRow, 2, new QTableWidgetItem("unsigned char"));
		cmdFrameTable->setItem(curRow, 3, new QTableWidgetItem("0x55AA"));
		QTableWidgetItem* cellItem = new QTableWidgetItem;
		cellItem->setText("ɾ��");
		cellItem->setTextAlignment(Qt::AlignCenter);
		cellItem->setTextColor(QColor(24, 132, 255));
		cmdFrameTable->setItem(curRow, cmdFrameTable->columnCount() - 1, cellItem);

		});

	connect(cmdFrameTable, &QTableWidget::cellClicked, this, [=](int row, int column) {
		if (column == (cmdFrameTable->columnCount() - 1))
		{
			cmdFrameTable->removeRow(row);
		}
		});

}

/**
	@brief
**/
void AllInfoConfigWidget::clickedCancelBtn() {

	this->close();
}

/**
	@brief ���ȷ����ť
**/
void AllInfoConfigWidget::clickedOkBtn() {
	//�б�ǰ��������
	if (curWidget == DeviceCommonVaries::ROCKET_WIDGET)
	{
		if (rocketComProtoList->count() == 0)
		{
			QMessageBox::warning(this, "����", "��Э�飬������Ӽ���Э��");
			return;
		}
		//��ȡrocketComProtoList��ѡ�е�ѡ����Ϊʹ�õ�Э��
		int rocketDataID = rocketComProtoList->currentItem()->data(Qt::UserRole).toInt();
		//��֮ǰ��Э����������
		DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM `simulatedtraining`.`rockect_param_info` WHERE `rocket_data_id` = %1").arg(rocketDataID));
		int paramIndex = -1;
		//�����������������˳��д��rocketParam��
		//ȷ��ʱ�õ���ǰ˳��
		for (int rowR = 0; rowR < hadSelectedParamsL->count(); rowR++)
		{
			DeviceDBConfigInfo::getInstance()->rocketParamInfo2DB(rocketDataID, hadSelectedParamsL->item(rowR)->data(Qt::UserRole).toInt(), rowR + 1, 1, 0);
		}
		QMessageBox::information(this, QString("��Ϣ"), QString("Э�����óɹ�"));
	}
	else if (curWidget == DeviceCommonVaries::DEVICE_WIDGET)
	{
		//����Ϊ�豸״̬���滹���豸�����󶨽���
		//if (deviceCfgList->currentRow() == 0)

		{
			//��ɾ�����и��豸����Ϣ
			DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM `simulatedtraining`.`device_status_info` WHERE device_status_info.device_id = %1").arg(deviceID));
			//��ȡѡ��checkbox���� 
			for (auto firW : scrollAreaStat->widget()->children())
			{
				QCheckBox* checkBox = firW->findChild<QCheckBox*>("checkbox");
				if (checkBox == nullptr)
				{
					continue;
				}
				//������ѡ�е�״̬��
				auto pro = checkBox->checkState();
				if (pro == Qt::Checked)
				{
					//д��
					int statId = firW->findChild<QLabel*>("status")->property("id").toInt();
					QString filepath1 = firW->findChild<QLabel*>("fileLab")->text();
					if (filepath1.isEmpty())
					{
						QMessageBox::warning(nullptr, "����", "��δ���ļ���!");
						return;
					}
					DeviceDBConfigInfo::getInstance()->deviceStatusInfo2DB(deviceID, statId, filepath1);
				}

			}
		}
		//else//�豸�����󶨽���
		{
			//��ɾ�����и��豸����Ϣ
			DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM `simulatedtraining`.`device_param_info` WHERE device_param_info.device_id = %1").arg(deviceID));
			for (auto firW : scrollAreaParam->widget()->children())
			{
				QCheckBox* checkBox = firW->findChild<QCheckBox*>("checkbox");
				if (checkBox == nullptr)
				{
					continue;
				}
				//������ѡ�е�״̬��
				auto pro = checkBox->checkState();
				if (pro == Qt::Checked)
				{
					//д��
					int paramId = firW->findChild<QLabel*>("paramlab")->property("id").toInt();
					DeviceDBConfigInfo::getInstance()->deviceParamInfo2DB(deviceID, paramId);
				}

			}
		}
		QMessageBox::information(this, QString("��Ϣ"), QString("�豸���óɹ�"));
	}
	//ָ�����ý���  ��װ���Ѿ����õ���Ϣ
	else if (curWidget == DeviceCommonVaries::COMMAND_WIDGET)
	{
		if (cmdCfgList->currentRow() == 0)//ָ���豸״̬��
		{
			//�Ƚ���������ɾ��
			DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.command_devicestatus_info WHERE command_devicestatus_info.command_id=%1").arg(cmdID));
			for (int i = 0; i < hadSelectDevStat->count(); i++)
			{
				DeviceDBConfigInfo::getInstance()->commandDeviceStatInfo2DB(cmdID, hadSelectDevStat->item(i)->data(Qt::UserRole).toInt());
			}

			//for (auto firW : scrollAreaDevStat->widget()->children())
			//{
			//	QCheckBox* checkBox = firW->findChild<QCheckBox*>("checkbox");
			//	if (checkBox == nullptr)
			//	{
			//		continue;
			//	}
			//	//������ѡ�е�״̬��
			//	auto pro = checkBox->checkState();
			//	if (pro == Qt::Checked)
			//	{
			//		//�޸�
			//		int devStatId = firW->findChild<QLabel*>("devStatLab")->property("devStatId").toInt();
			//		DeviceDBConfigInfo::getInstance()->commandDeviceStatInfo2DB(cmdID, devStatId);
			//	}
			//}
		}
		else //ָ��֡����  ��װ���Ѿ����õ���Ϣ
		{
			//�Ƚ���������ɾ��
			DeviceDBConfigInfo::getInstance()->customRunSql(QString("DELETE FROM simulatedtraining.command_param_info WHERE command_param_info.command_id=%1").arg(cmdID));
			//��ȡcmdFrameTable�е�����
			for (int row = 0; row < cmdFrameTable->rowCount(); row++)
			{
				//code����row
				QString name1 = cmdFrameTable->item(row, 0)->text();
				int code1 = row + 1;
				int index1 = row + 1;
				int length1 = cmdFrameTable->item(row, 1)->text().toInt();
				QString resType = cmdFrameTable->item(row, 2)->text();
				int defaultVal = cmdFrameTable->item(row, 3)->text().toInt(nullptr, 16);

				DeviceDBConfigInfo::getInstance()->commandParamInfo2DB(cmdID, name1, code1, index1, length1, resType, defaultVal);
			}
		}
		QMessageBox::information(this, QString("��Ϣ"), QString("ָ�����óɹ�"));
	}

	this->close();
}

/**
	@brief ��갴�²���
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
	@brief ����ƶ�����
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
	@brief ����ͷŲ���
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