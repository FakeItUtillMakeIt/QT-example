#include "CenterOperate.h"

#define FRAMELENGTH 10
#define FAULT_NAME "故障"
#define FAULT_ID 3

#define POWER_NAME "配电"
#define POWER_ID 1


CenterOperate::CenterOperate(QWidget* parent)
	: QWidget(parent)
	, m_pYcTimer(nullptr)
{

	ui.setupUi(parent);
	m_app = AppCache::instance();
	m_pResponseSenderSocket = new QUdpSocket(this);
	m_pYaoCeSenderSocket = new QUdpSocket(this);


	Init();
}
CenterOperate::~CenterOperate()
{
	if (m_pResponseSenderSocket != nullptr)
	{
		m_pResponseSenderSocket->close();
		delete m_pResponseSenderSocket;
		m_pResponseSenderSocket = nullptr;
	}
	if (m_pYaoCeSenderSocket != nullptr)
	{
		m_pYaoCeSenderSocket->close();
		delete m_pYaoCeSenderSocket;
		m_pYaoCeSenderSocket = nullptr;
	}
	if (m_pYcTimer != nullptr) m_pYcTimer->stop();



	AllInfoConfigWidget::closeInstance();

	AddRocketTypeWidget::closeInstance();

}

void CenterOperate::Init()
{
	m_pReceiveCMDData = new ReceiveCMDData();
	connect(m_pReceiveCMDData, &ReceiveCMDData::receiverCMD, this, &CenterOperate::receiverCMD);

	configOperateName << QString("火箭型号管理") << QString("设备管理") << QString("指令管理") << QString("参数管理");
	//添加设备管理器界面布局
	InitUILayout();
	InitFrame();
}

void CenterOperate::receiverCMD(QVariant oneCommand)
{
	Command* command = oneCommand.value<Command*>();


	//根据接收指令的类型  测发指令还是故障指令进行不同操作 
	// 故障指令直接影响设备参数，不影响设备状态，需要从fault_param_info表
	// 测发指令影响设备状态和设备参数
	if (command->m_iType == 3)
	{
		//故障指令
		dealFaultCmd(command);
	}
	else {
		//测发指令
		//设备状态切换
		switchDeviceStatus(command);
		//处理设备参数
		dealDeviceParams(command);
	}

	//发送回令
	sendCMDResponse(command->m_iBackId, command->m_iCode);
	//QMessageBox::warning(qApp->activeWindow(), QObject::tr("警告"), QString("设备管理软件收到测发指令,code等于%1！").arg(command->m_iCode));
}

/// <summary>
/// 切换设备状态
/// </summary>
void CenterOperate::switchDeviceStatus(Command* command)
{
	//1.根据获取的指令寻找对应设备
	//2.根据指令编码进行设备状态切换
	DeviceDBConfigInfo::getInstance()->readCMDDeviceStatDB2UI();
	auto a = DeviceDBConfigInfo::getInstance()->commandDeviceStatInfo;
	vector<int> deviceIdV;
	vector<int> statusIdV;

	try
	{
		if (a[command->m_id].size() == 0)
		{
			throw "联合查询没有查询到改指令对应的设备状态";
		}
		else
		{
			for (int i = 0; i < a[command->m_id].size() / 4; i++)
			{
				deviceIdV.push_back(atoi(a[command->m_id][1 + (4 * i)].c_str()));
				statusIdV.push_back(atoi(a[command->m_id][3 + (4 * i)].c_str()));
			}
		}

	}
	catch (const char* msg)
	{
		qDebug() << msg;
		return;
	}

	//从设备管理器中得到指令影响的设备
	for (auto i = 0; i < deviceIdV.size(); i++)
	{
		Device* dev = m_app->m_allDeviceCopy[deviceIdV[i]];
		//根据指令编码进行设备状态切换
		dev->m_sCurStatus = m_app->m_allDeviceStatus[statusIdV[i]]->m_statusName;
	}
}

/// <summary>
/// 处理设备参数
/// </summary>
/// <param name="command"></param>
void CenterOperate::dealDeviceParams(Command* command) {
	//获取设备到参数的对应关系
	DeviceDBConfigInfo::getInstance()->readCMDDeviceStatDB2UI();

	auto a = DeviceDBConfigInfo::getInstance()->commandDeviceStatInfo;
	/*int deviceId = 0, dStatusId = 0;*/
	vector<int> deviceIdV;
	vector<int> statusIdV;

	try
	{
		if (a[command->m_id].size() == 0)
		{
			throw "联合查询没有查询到改指令对应的设备状态";
		}
		else
		{
			for (int i = 0; i < a[command->m_id].size() / 4; i++)
			{
				deviceIdV.push_back(atoi(a[command->m_id][1 + (4 * i)].c_str()));
				statusIdV.push_back(atoi(a[command->m_id][3 + (4 * i)].c_str()));
			}

		}

	}
	catch (const char* msg)
	{
		qDebug() << msg;
		return;
	}

	vector<int> paramV;
	for (int i = 0; i < deviceIdV.size(); i++)
	{
		for (int j : m_app->m_dev2DeviceParamID[deviceIdV[i]])
		{

			DeviceParam* deviceParam = m_app->m_allDeviceParam[j];
			//参数状态和设备状态保持一致m_id

			deviceParam->m_Validity = 1;//参数数据有效
			if (deviceParam->m_curStatus.m_id == 3)
			{
				deviceParam->m_status = m_app->m_allDeviceStatus[statusIdV[i]]->m_statusName;
				deviceParam->m_preStatus.m_name = deviceParam->m_status;
				deviceParam->m_preStatus.m_id = atoi(DeviceDBConfigInfo::getInstance()->commandDeviceStatInfo[command->m_id][2].c_str());
				continue;
			}

			deviceParam->m_status = m_app->m_allDeviceStatus[statusIdV[i]]->m_statusName;
			deviceParam->m_curStatus.m_name = deviceParam->m_status;
			deviceParam->m_preStatus.m_name = deviceParam->m_curStatus.m_name;

			deviceParam->updateParamRealVal();
			deviceParam->m_curStatus.m_id = atoi(DeviceDBConfigInfo::getInstance()->commandDeviceStatInfo[command->m_id][2].c_str());
			deviceParam->m_preStatus.m_id = deviceParam->m_curStatus.m_id;
		}
	}

}

/// <summary>
/// 处理故障指令影响的设备参数 根据fault_param_info得到
/// </summary>
/// <param name="command"></param>
void CenterOperate::dealFaultCmd(Command* command) {
	//获取指令ID到故障参数表的数据
	QString qSqlString = "SELECT\
		device_param_info.device_id,\
		device_param_info.parameter_id,\
		device_param_info.createTime,\
		device_param_info.lastUpdateTime\
		FROM\
		fault_param_info\
		INNER JOIN device_param_info ON fault_param_info.device_param_info_id = device_param_info.id\
		WHERE\
		fault_param_info.command_id = %1;";
	qSqlString = qSqlString.arg(command->m_id);


	DeviceDBConfigInfo::getInstance()->customReadTableInfo(qSqlString);
	//auto b = DeviceDBConfigInfo::getInstance()->customReadInfoMap;


	//得到故障指令影响的所有设备和对应参数
	map<int, vector<int>> fault2DeviceParam;

	for (auto ele = DeviceDBConfigInfo::getInstance()->customReadInfoMap.begin(); ele != DeviceDBConfigInfo::getInstance()->customReadInfoMap.end(); ele++)
	{
		for (auto i = 1; i < ele->second.size(); i += 2)
		{
			fault2DeviceParam[ele->first].push_back(atoi(ele->second[i].c_str()));
		}

	}

	//受影响的设备
	for (auto eleDev = fault2DeviceParam.begin(); eleDev != fault2DeviceParam.end(); eleDev++)
	{
		//得到当前设备拥有的所有  设备参数  的id
		auto curDevParamV = m_app->m_dev2DeviceParamID[eleDev->first];
		//在所有设备参数的参数id中假如有故障影响的设备参数，则修改状态
		for (auto eleDevParam : curDevParamV)
		{
			for (auto eleParamID : eleDev->second)
			{
				//设备参数中参数ID等于故障指令中影响的设备参数ID
				if (m_app->m_allDeviceParam[eleDevParam]->m_subParameterId == eleParamID) {
					DeviceParam* deviceParam = m_app->m_allDeviceParam[eleDevParam];
					//参数状态和设备状态保持一致
					//第二次点击故障
					if (deviceParam->m_curStatus.m_id == FAULT_ID)
					{
						//恢复为前置状态
						deviceParam->m_status = deviceParam->m_preStatus.m_name;
						deviceParam->updateParamRealVal();
						auto curID = deviceParam->m_curStatus.m_id;
						auto curName = deviceParam->m_curStatus.m_name;
						deviceParam->m_curStatus.m_id = deviceParam->m_preStatus.m_id;
						deviceParam->m_curStatus.m_name = deviceParam->m_preStatus.m_name;
						//更换前置状态
						deviceParam->m_preStatus.m_id = curID;
						deviceParam->m_preStatus.m_name = curName;
					}
					else
					{
						if (deviceParam->m_Validity == 0)
						{
							//记录前置状态
							deviceParam->m_preStatus.m_id = POWER_ID;//POWER_ID;//配电索引
							deviceParam->m_preStatus.m_name = Utils::UTF8ToGBK(POWER_NAME);//Utils::UTF8ToGBK(POWER_NAME);//配电
						}
						else
						{
							//记录前置状态
							deviceParam->m_preStatus.m_id = deviceParam->m_curStatus.m_id;
							deviceParam->m_preStatus.m_name = deviceParam->m_curStatus.m_name;
						}

						//第一次点击故障
						deviceParam->m_status = Utils::UTF8ToGBK(FAULT_NAME);
						deviceParam->updateParamRealVal();
						deviceParam->m_curStatus.m_id = FAULT_ID;
						deviceParam->m_curStatus.m_name = deviceParam->m_status;
					}
				}
			}
		}
	}

}

/// <summary>
/// 发送测发回令
/// </summary>
/// <param name="cmd_id"></param>
void CenterOperate::sendCMDResponse(int cmd_id, int sendCmd_code)
{
	if (m_app->m_allCommad.find(cmd_id) == m_app->m_allCommad.end())
	{
		return;
	}
	Command* command = m_app->m_allCommad[cmd_id];
	QByteArray m_pBuff(FRAMELENGTH, Qt::Uninitialized);
	m_pBuff[0] = 0x55;
	m_pBuff[1] = 0xBB;
	memcpy(m_pBuff.data() + 2, &command->m_iCode, 2);//测发指令code 
	m_pBuff[4] = 0x01; //参数，执行成功
	memcpy(m_pBuff.data() + 5, &sendCmd_code, 2);
	m_pBuff[7] = 0x00;
	m_pBuff[8] = 0x00;
	m_pBuff[9] = 0x00;
	unsigned char vdata1[FRAMELENGTH];
	for (int i = 0; i < FRAMELENGTH; i++)
	{
		vdata1[i] = (unsigned char)m_pBuff[i];
	}
	unsigned short checksum = CRC::CalCRC16(vdata1 + 2, FRAMELENGTH - 4);
	memcpy(m_pBuff.data() + FRAMELENGTH - 2, &checksum, 2);
	m_pResponseSenderSocket->writeDatagram(m_pBuff, QHostAddress(m_app->m_responseSender->m_strIP.c_str()), m_app->m_responseSender->m_iPort);
}

/// <summary>
/// 发送箭上数据
/// </summary>
void CenterOperate::sendRocketData()
{
	if (m_app->m_CurrentRocketDataFrame == nullptr) return;
	RocketDataFrame* pFrame = m_app->m_CurrentRocketDataFrame;
	pFrame->head().UpdateTime();
	if (!pFrame->Serialize((unsigned char*)m_pBuff.constData(), FRAMEMAXLENGTH))
	{
		return;
	}
	m_pYaoCeSenderSocket->writeDatagram(m_pBuff, QHostAddress(m_app->m_yaoCeSender->m_strIP.c_str()), m_app->m_yaoCeSender->m_iPort);
}

/**
	@brief 管理器软件主界面布局
**/
void CenterOperate::InitUILayout() {

	//this->setMouseTracking(true);
	WidgetStyleSheet* wss = WidgetStyleSheet::getInstace();
	QFont font;
	font.setBold(true);
	font.setPixelSize(16);
	QIcon icon(":/icon/icon/circled-right.png");
	QIcon iconClick(":/icon/icon/ww.png");
	QIcon iconRelease(":/icon/icon/bb.png");
	for each (QString configName in configOperateName)
	{
		QListWidgetItem* item = new QListWidgetItem(configName);

		ui.listWidget->addItem(item);
	}
	ui.listWidget->setFocusPolicy(Qt::NoFocus);

	QString ss = wss->deviceManageListSS.arg(":/icon/icon/ww.png").arg(":/icon/icon/bb.png");

	ui.listWidget->setStyleSheet(ss);
	//ui.listWidget->setContentsMargins(10, 10, 10, 10);

	ui.listWidget->setLayoutDirection(Qt::LeftToRight);
	ui.listWidget->horizontalScrollBar()->hide();
	ui.listWidget->verticalScrollBar()->hide();

	paramManageUI = new ParamManageModule(this);
	ui.gridLayout->addWidget(paramManageUI, 0, 1);
	deviceManageUI = new DeviceManageModule(this);
	ui.gridLayout->addWidget(deviceManageUI, 0, 2);
	commandManageUI = new CommandManageModule(this);
	ui.gridLayout->addWidget(commandManageUI, 0, 3);
	rocketManageUI = new RocketTypeManageModule(this);
	ui.gridLayout->addWidget(rocketManageUI, 0, 4);
	paramManageUI->hide();
	deviceManageUI->hide();
	commandManageUI->hide();
	rocketManageUI->hide();
	//


	connect(ui.listWidget, &QListWidget::itemPressed, this, [=]() {
		ui.widget->close();

		for (int i = 0; i < ui.listWidget->count(); i++)
		{
			ui.listWidget->item(i)->setIcon(iconRelease);
		}
		ui.listWidget->currentItem()->setIcon(iconClick);

		switch (ui.listWidget->currentRow())
		{
		case 3:
			//呼出对应属性窗口

			paramManageUI->show();
			deviceManageUI->hide();
			commandManageUI->hide();
			rocketManageUI->hide();

			break;
		case 1:
			paramManageUI->hide();
			deviceManageUI->show();
			commandManageUI->hide();
			rocketManageUI->hide();

			break;
		case 2:
			paramManageUI->hide();
			deviceManageUI->hide();
			commandManageUI->show();
			rocketManageUI->hide();
			break;
		case 0:
			paramManageUI->hide();
			deviceManageUI->hide();
			commandManageUI->hide();
			rocketManageUI->show();
			break;
		default:
			break;
		}

		});

	connect(paramManageUI, &ParamManageModule::changed, this, []() {
		qDebug() << "param changed";
		});

	//默认显示
	ui.listWidget->setCurrentItem(ui.listWidget->item(0));

	emit ui.listWidget->itemPressed(ui.listWidget->currentItem());

}

/// <summary>
/// 初始化帧
/// </summary>
bool CenterOperate::InitFrame()
{
	//设置协议帧
	//m_app->m_CurrentRocketDataFrame = m_app->m_RocketDataFrame[m_app->m_CurrentRocketType->m_id];
	m_app->m_CurrentRocketDataFrame = nullptr;
	for (auto pair : m_app->m_RocketDataFrame)
	{
		if (pair.second->m_rocketId == m_app->m_CurrentRocketType->m_id)
		{
			m_app->m_CurrentRocketDataFrame = pair.second;
		}
	}
	if (m_app->m_CurrentRocketDataFrame == nullptr)
	{
		QMessageBox::warning(qApp->activeWindow(), QObject::tr("警告"), "没有绑定箭上数据协议！");
		return false;
	}
	RocketDataFrame* pFrame = m_app->m_CurrentRocketDataFrame;
	pFrame->head().setFrameCount(1);
	pFrame->head().setFrameLen(pFrame->params().size() * PARAM_LENGTH + FRAMEHEAD_LENGTH);
	pFrame->head().setFrameType(FRAME_TYPE_YC_PARAMETER);
	pFrame->head().setParamCount(pFrame->params().size());
	m_pBuff.resize(pFrame->Size());

	m_pYcTimer = new QTimer();
	connect(m_pYcTimer, &QTimer::timeout, this, &CenterOperate::sendRocketData);//发送箭上数据
	m_pYcTimer->start(50);

	return true;
}



