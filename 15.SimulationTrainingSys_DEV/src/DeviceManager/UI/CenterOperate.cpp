#include "CenterOperate.h"

#define FRAMELENGTH 10
#define FAULT_NAME "����"
#define FAULT_ID 3

#define POWER_NAME "���"
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

	configOperateName << QString("����ͺŹ���") << QString("�豸����") << QString("ָ�����") << QString("��������");
	//����豸���������沼��
	InitUILayout();
	InitFrame();
}

void CenterOperate::receiverCMD(QVariant oneCommand)
{
	Command* command = oneCommand.value<Command*>();


	//���ݽ���ָ�������  �ָⷢ��ǹ���ָ����в�ͬ���� 
	// ����ָ��ֱ��Ӱ���豸��������Ӱ���豸״̬����Ҫ��fault_param_info��
	// �ָⷢ��Ӱ���豸״̬���豸����
	if (command->m_iType == 3)
	{
		//����ָ��
		dealFaultCmd(command);
	}
	else {
		//�ָⷢ��
		//�豸״̬�л�
		switchDeviceStatus(command);
		//�����豸����
		dealDeviceParams(command);
	}

	//���ͻ���
	sendCMDResponse(command->m_iBackId, command->m_iCode);
	//QMessageBox::warning(qApp->activeWindow(), QObject::tr("����"), QString("�豸��������յ��ָⷢ��,code����%1��").arg(command->m_iCode));
}

/// <summary>
/// �л��豸״̬
/// </summary>
void CenterOperate::switchDeviceStatus(Command* command)
{
	//1.���ݻ�ȡ��ָ��Ѱ�Ҷ�Ӧ�豸
	//2.����ָ���������豸״̬�л�
	DeviceDBConfigInfo::getInstance()->readCMDDeviceStatDB2UI();
	auto a = DeviceDBConfigInfo::getInstance()->commandDeviceStatInfo;
	vector<int> deviceIdV;
	vector<int> statusIdV;

	try
	{
		if (a[command->m_id].size() == 0)
		{
			throw "���ϲ�ѯû�в�ѯ����ָ���Ӧ���豸״̬";
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

	//���豸�������еõ�ָ��Ӱ����豸
	for (auto i = 0; i < deviceIdV.size(); i++)
	{
		Device* dev = m_app->m_allDeviceCopy[deviceIdV[i]];
		//����ָ���������豸״̬�л�
		dev->m_sCurStatus = m_app->m_allDeviceStatus[statusIdV[i]]->m_statusName;
	}
}

/// <summary>
/// �����豸����
/// </summary>
/// <param name="command"></param>
void CenterOperate::dealDeviceParams(Command* command) {
	//��ȡ�豸�������Ķ�Ӧ��ϵ
	DeviceDBConfigInfo::getInstance()->readCMDDeviceStatDB2UI();

	auto a = DeviceDBConfigInfo::getInstance()->commandDeviceStatInfo;
	/*int deviceId = 0, dStatusId = 0;*/
	vector<int> deviceIdV;
	vector<int> statusIdV;

	try
	{
		if (a[command->m_id].size() == 0)
		{
			throw "���ϲ�ѯû�в�ѯ����ָ���Ӧ���豸״̬";
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
			//����״̬���豸״̬����һ��m_id

			deviceParam->m_Validity = 1;//����������Ч
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
/// �������ָ��Ӱ����豸���� ����fault_param_info�õ�
/// </summary>
/// <param name="command"></param>
void CenterOperate::dealFaultCmd(Command* command) {
	//��ȡָ��ID�����ϲ����������
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


	//�õ�����ָ��Ӱ��������豸�Ͷ�Ӧ����
	map<int, vector<int>> fault2DeviceParam;

	for (auto ele = DeviceDBConfigInfo::getInstance()->customReadInfoMap.begin(); ele != DeviceDBConfigInfo::getInstance()->customReadInfoMap.end(); ele++)
	{
		for (auto i = 1; i < ele->second.size(); i += 2)
		{
			fault2DeviceParam[ele->first].push_back(atoi(ele->second[i].c_str()));
		}

	}

	//��Ӱ����豸
	for (auto eleDev = fault2DeviceParam.begin(); eleDev != fault2DeviceParam.end(); eleDev++)
	{
		//�õ���ǰ�豸ӵ�е�����  �豸����  ��id
		auto curDevParamV = m_app->m_dev2DeviceParamID[eleDev->first];
		//�������豸�����Ĳ���id�м����й���Ӱ����豸���������޸�״̬
		for (auto eleDevParam : curDevParamV)
		{
			for (auto eleParamID : eleDev->second)
			{
				//�豸�����в���ID���ڹ���ָ����Ӱ����豸����ID
				if (m_app->m_allDeviceParam[eleDevParam]->m_subParameterId == eleParamID) {
					DeviceParam* deviceParam = m_app->m_allDeviceParam[eleDevParam];
					//����״̬���豸״̬����һ��
					//�ڶ��ε������
					if (deviceParam->m_curStatus.m_id == FAULT_ID)
					{
						//�ָ�Ϊǰ��״̬
						deviceParam->m_status = deviceParam->m_preStatus.m_name;
						deviceParam->updateParamRealVal();
						auto curID = deviceParam->m_curStatus.m_id;
						auto curName = deviceParam->m_curStatus.m_name;
						deviceParam->m_curStatus.m_id = deviceParam->m_preStatus.m_id;
						deviceParam->m_curStatus.m_name = deviceParam->m_preStatus.m_name;
						//����ǰ��״̬
						deviceParam->m_preStatus.m_id = curID;
						deviceParam->m_preStatus.m_name = curName;
					}
					else
					{
						if (deviceParam->m_Validity == 0)
						{
							//��¼ǰ��״̬
							deviceParam->m_preStatus.m_id = POWER_ID;//POWER_ID;//�������
							deviceParam->m_preStatus.m_name = Utils::UTF8ToGBK(POWER_NAME);//Utils::UTF8ToGBK(POWER_NAME);//���
						}
						else
						{
							//��¼ǰ��״̬
							deviceParam->m_preStatus.m_id = deviceParam->m_curStatus.m_id;
							deviceParam->m_preStatus.m_name = deviceParam->m_curStatus.m_name;
						}

						//��һ�ε������
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
/// ���Ͳⷢ����
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
	memcpy(m_pBuff.data() + 2, &command->m_iCode, 2);//�ָⷢ��code 
	m_pBuff[4] = 0x01; //������ִ�гɹ�
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
/// ���ͼ�������
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
	@brief ��������������沼��
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
			//������Ӧ���Դ���

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

	//Ĭ����ʾ
	ui.listWidget->setCurrentItem(ui.listWidget->item(0));

	emit ui.listWidget->itemPressed(ui.listWidget->currentItem());

}

/// <summary>
/// ��ʼ��֡
/// </summary>
bool CenterOperate::InitFrame()
{
	//����Э��֡
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
		QMessageBox::warning(qApp->activeWindow(), QObject::tr("����"), "û�а󶨼�������Э�飡");
		return false;
	}
	RocketDataFrame* pFrame = m_app->m_CurrentRocketDataFrame;
	pFrame->head().setFrameCount(1);
	pFrame->head().setFrameLen(pFrame->params().size() * PARAM_LENGTH + FRAMEHEAD_LENGTH);
	pFrame->head().setFrameType(FRAME_TYPE_YC_PARAMETER);
	pFrame->head().setParamCount(pFrame->params().size());
	m_pBuff.resize(pFrame->Size());

	m_pYcTimer = new QTimer();
	connect(m_pYcTimer, &QTimer::timeout, this, &CenterOperate::sendRocketData);//���ͼ�������
	m_pYcTimer->start(50);

	return true;
}



