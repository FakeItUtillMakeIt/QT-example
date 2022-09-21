#include "CenterOperateF.h" 
#pragma execution_character_set("utf-8")//��� qt�����������ʾ������������

#define FRAMELENGTH 10
CenterOperateF::CenterOperateF(QWidget* parent)
    : QWidget(parent)
    , m_pFaultParamDAO2(nullptr)
    , m_pFaultDAO2(nullptr)
    , m_isSendOne(false)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);

    m_pSocket = new QUdpSocket(this);
    m_app = AppCache::instance();

    Init();
}


/// <summary>
/// ��ʱ����Ӧ����
/// </summary>
/// <param name="event"></param>
void CenterOperateF::timerEvent(QTimerEvent* event)
{
    bool isOK = true;
    if (event->timerId() == m_TimerID)
    {
        QMessageBox::warning(nullptr, QString("��ʾ"), "���չ��ϻ��ʱ��", "ȷ��");
        killTimer(m_TimerID);//�رն�ʱ��
    }
}


void CenterOperateF::Init()
{
    //ָ����ϻ���
    m_pReceiveCMDDataF = new ReceiveCMDDataF(m_app->m_cmdResponseReceiver->m_strIP, m_app->m_cmdResponseReceiver->m_iPort);
    connect(m_pReceiveCMDDataF, &ReceiveCMDDataF::receiverCMD, this, &CenterOperateF::receiverCMD);

    //�������ϻ���
    m_pReceiveParamData = new ReceiveCMDDataF(m_app->m_paramResponseReceiver->m_strIP, m_app->m_paramResponseReceiver->m_iPort);
    connect(m_pReceiveParamData, &ReceiveCMDDataF::receiverCMD, this, &CenterOperateF::receiverCMD);


    connect(ui.pb_addControlFault, &QPushButton::clicked, this, &CenterOperateF::AddControlFaultClicked);
    connect(ui.pb_addMeasurementFault, &QPushButton::clicked, this, &CenterOperateF::AddMeasurementFaultClicked);
    connect(ui.pb_addPowerFault, &QPushButton::clicked, this, &CenterOperateF::AddPowerFaultClicked);

    m_controlError = new AddErrorCommand();
    connect(m_controlError, &AddErrorCommand::AddFaultItem, this, &CenterOperateF::AddFaultItem);
    connect(m_controlError, &AddErrorCommand::DelFaultItem, this, &CenterOperateF::DelFaultItem);
    connect(m_controlError, &AddErrorCommand::EditFaultItem, this, &CenterOperateF::EditFaultItem);

    m_pFaultParamDAO2 = new DataBaseF::FaultParamDAO(m_app->m_outputPath);//���ݿ���Ƴ���ʼ��
    m_pFaultDAO2 = new DataBaseF::FaultDAO(m_app->m_outputPath);

    FlashFualtInfo();
}

/// <summary>
/// ˢ�½��������Ϣ
/// </summary>
void CenterOperateF::FlashFualtInfo()
{
    //����չʾ�������ϣ�fault_param_info����
    m_AllFaultItems.clear();
    for (auto it : m_app->m_FaultParamInfoFrames)
    {
        //ÿ����Ҫ�жϣ���ǰ���������Ƿ�Ӱ�쵽�������,���н��д�������vector��
        bool isexist = false;
        for (int k = 0; k < m_AllFaultItems.size(); k++)
        {
            for (int j = 0; j < m_AllFaultItems[k].size(); j++)
            {
                for (int i = 0; i < m_AllFaultItems[k][j].deviceParamInfoID.size(); i++)
                {
                    if (m_AllFaultItems[k][j].FaultCommandID == it.second->m_FaultCommandCode && m_AllFaultItems[k][j].Name == QString::fromStdString(it.second->m_name))
                    {
                        m_AllFaultItems[k][j].deviceParamInfoID.push_back(it.second->m_deviceParamInfoID);//ͬһ������Ӱ��Ĳ�ͬ�������кϲ�
                        isexist = true;
                        break;
                    }
                }
                if (isexist) break;
            }
            if (isexist) break;
        }
        if (isexist) continue;

        ItemStruct oneItem;
        oneItem.ID = it.first;

        //�󶨹���ָ��code
        for (auto itemf : m_app->m_CommandInfoframes)
        {
            if (itemf.first == it.second->m_FaultCommandCode)
            {
                oneItem.code = itemf.second->m_code;
                break;
            }
        }

        oneItem.type = ParamFault;//��������
        oneItem.Name = QString::fromStdString(it.second->m_name);
        oneItem.FaultCommandID = it.second->m_FaultCommandCode;//����ָ��id
        oneItem.deviceParamInfoID.push_back(it.second->m_deviceParamInfoID);//����id  device_param_info_id

        for (auto itemd : m_app->m_FaultDeviceParamInfoFrames)
        {
            if ((itemd.second->m_id == it.second->m_deviceParamInfoID))
            {
                oneItem.deviceID = itemd.second->m_deviceID;//�豸id
                break;
            }
        }

        oneItem.createTime = QString::fromStdString(it.second->m_createTime);
        oneItem.lastUpdateTime = QString::fromStdString(it.second->m_lastUpdateTime);

        //���ơ�����������������ʾ
        switch (it.second->m_faultType)
        {
        case ControlFault:
            oneItem.faultType = ControlFault;
            m_AllFaultItems[ControlFault].push_back(oneItem);//TODO��ÿ�����ӻ�ɾ�����ϲ�����Ҳ��Ҫ��������޸�
            break;
        case MeasurementFault:
            oneItem.faultType = MeasurementFault;
            m_AllFaultItems[2].push_back(oneItem);
            break;
        case PowerFault:
            oneItem.faultType = PowerFault;
            m_AllFaultItems[3].push_back(oneItem);
            break;
        default:
            break;
        }
    }

    //����չʾָ����ϣ�fault_command_info����
    for (auto it : m_app->m_FaultCommandInfoFrames)
    {
        //ÿ����Ҫ�жϣ���ǰָ������Ƿ�Ӱ�쵽�������,���н��д�������vector��
        bool isexist = false;
        for (int k = 0; k < m_AllFaultItems.size(); k++)
        {
            for (int j = 0; j < m_AllFaultItems[k].size(); j++)
            {
                for (int i = 0; i < m_AllFaultItems[k][j].responseCommandID.size(); i++)
                {
                    if (m_AllFaultItems[k][j].FaultCommandID == it.second->m_FaultCommandCode && m_AllFaultItems[k][j].Name == QString::fromStdString(it.second->m_name))
                    {
                        m_AllFaultItems[k][j].responseCommandID.push_back(it.second->m_responseCommandID);//ͬһ������Ӱ��Ĳ�ͬ�������кϲ�
                        isexist = true;
                        break;
                    }
                }
                if (isexist) break;
            }
            if (isexist) break;
        }
        if (isexist) continue;

        ItemStruct oneItem;
        oneItem.ID = it.first;

        //�󶨹���ָ��code
        for (auto itemf : m_app->m_CommandInfoframes)
        {
            if (itemf.first == it.second->m_FaultCommandCode)
            {
                oneItem.code = itemf.second->m_code;
                break;
            }
        }

        oneItem.type = CommandFault;//ָ�����
        oneItem.Name = QString::fromStdString(it.second->m_name);

        oneItem.FaultCommandID = it.second->m_FaultCommandCode;//����ָ��id
        oneItem.responseCommandID.push_back(it.second->m_responseCommandID);//��Ӱ��ָ��id
        //oneItem.deviceParamInfoID = "";//����id (�ǲ������ϵĲ���)
        oneItem.deviceID = 0;//TODO�豸id

        oneItem.createTime = QString::fromStdString(it.second->m_createTime);
        oneItem.lastUpdateTime = QString::fromStdString(it.second->m_createTime);

        //���ơ�����������������ʾ
        switch (it.second->m_faultType)
        {
        case ControlFault:
            oneItem.faultType = ControlFault;
            m_AllFaultItems[ControlFault].push_back(oneItem);//TODO��ÿ�����ӻ�ɾ�����ϲ�����Ҳ��Ҫ��������޸�
            break;
        case MeasurementFault:
            oneItem.faultType = MeasurementFault;
            m_AllFaultItems[2].push_back(oneItem);
            break;
        case PowerFault:
            oneItem.faultType = PowerFault;
            m_AllFaultItems[3].push_back(oneItem);
            break;
        default:
            break;
        }
    }

    for (size_t i = 0; i < m_systemItems.size(); i++)
    {
        if (m_systemItems[i] != nullptr)
        {
            delete m_systemItems[i];
            m_systemItems[i] = nullptr;
        }
    }

    m_systemItems[ControlFault] = new MyFaultParameters(ui.wgt_Control);//Ϊÿ���鴴����(�Ӳ����б�)
    connect(m_systemItems[ControlFault], &MyFaultParameters::onFaultClick, this, &CenterOperateF::isEnabledFault);
    connect(this, &CenterOperateF::backOnFaultClick, m_systemItems[ControlFault], &MyFaultParameters::backonFaultClick);

    m_systemItems[MeasurementFault] = new MyFaultParameters(ui.wgt_Measurement);
    connect(m_systemItems[MeasurementFault], &MyFaultParameters::onFaultClick, this, &CenterOperateF::isEnabledFault);
    connect(this, &CenterOperateF::backOnFaultClick, m_systemItems[MeasurementFault], &MyFaultParameters::backonFaultClick);

    m_systemItems[PowerFault] = new MyFaultParameters(ui.wgt_Power);
    connect(m_systemItems[PowerFault], &MyFaultParameters::onFaultClick, this, &CenterOperateF::isEnabledFault);
    connect(this, &CenterOperateF::backOnFaultClick, m_systemItems[PowerFault], &MyFaultParameters::backonFaultClick);

    m_systemItems[ControlFault]->Init(m_AllFaultItems[ControlFault]);
    m_systemItems[ControlFault]->show();

    m_systemItems[MeasurementFault]->Init(m_AllFaultItems[MeasurementFault]);
    m_systemItems[MeasurementFault]->show();

    m_systemItems[PowerFault]->Init(m_AllFaultItems[PowerFault]);
    m_systemItems[PowerFault]->show();
}



/// <summary>
/// ���ӿ���ϵͳ����
/// </summary>
void CenterOperateF::AddControlFaultClicked()
{
    m_systemIndex = ControlFault;
    m_controlError->SetAddIndex(m_systemIndex, m_AllFaultItems[m_systemIndex]);

    //���뵱ǰ�еĿ���ϵͳ����



    m_controlError->showNormal();
}

/// <summary>
/// ���Ӳ���ϵͳ����
/// </summary>
void CenterOperateF::AddMeasurementFaultClicked()
{
    m_systemIndex = MeasurementFault;
    m_controlError->SetAddIndex(m_systemIndex, m_AllFaultItems[m_systemIndex]);
    m_controlError->show();
}

/// <summary>
/// ���Ӷ���ϵͳ����
/// </summary>
void CenterOperateF::AddPowerFaultClicked()
{
    m_systemIndex = PowerFault;
    m_controlError->SetAddIndex(m_systemIndex, m_AllFaultItems[m_systemIndex]);
    m_controlError->show();
}


/// <summary>
/// ����ʧ�ܺ�ʹ��
/// UDP�鲥����ָ��
/// </summary>
/// <param name="title"></param>
/// <param name="code"></param>
/// <param name="isSave"></param>
void CenterOperateF::isEnabledFault(QString name, int code, int type, int faultType)
{
    //TODO ����UDP�鲥���͹���ָ��
    m_faultType = faultType;
    sendCMD(code, type);
}

/// <summary>
/// ���ӹ�����
/// </summary>
/// <param name="name"></param>

void CenterOperateF::AddFaultItem(vector<AddOneFaultInfo> addFaults)
{
    if (addFaults.size() <= 0) return;

    int commandcode = 0;
    for (int i = 0; i < addFaults.size(); i++)
    {
        //�����ӹ��ϻ���͹���ָ��
        FaultCommandInfo* oneCommandInfo = new FaultCommandInfo();
        oneCommandInfo->m_name = addFaults[i].m_name + "����";
        oneCommandInfo->m_backID = 0;
        oneCommandInfo->m_prefix = 0X55AA;
        oneCommandInfo->m_code = m_pFaultDAO2->GetNewCode();
        oneCommandInfo->m_type = 3;
        oneCommandInfo->m_rocketID = m_app->m_rockedType;
        m_pFaultDAO2->InsertCommandInfo(oneCommandInfo);

        oneCommandInfo->m_name = addFaults[i].m_name + "ָ��";
        oneCommandInfo->m_backID = oneCommandInfo->m_id;//�󶨻���id
        oneCommandInfo->m_code = m_pFaultDAO2->GetNewCode();;
        m_pFaultDAO2->InsertCommandInfo(oneCommandInfo);

        commandcode = oneCommandInfo->m_code;//�õ�ָ��code

        //������ 1���������ϣ�2��ָ�����
        if (addFaults[i].m_Type == 1)
        {
            //һ�����ϰ󶨶���ָ����ݿ�洢��һ����Ӧһ��
            for (auto item : addFaults[i].m_deviceParamInfoID)
            {
                FaultFaultParamInfo* oneControlFault = new FaultFaultParamInfo();
                oneControlFault->m_name = addFaults[i].m_name;
                oneControlFault->m_rocketID = m_app->m_rockedType;
                oneControlFault->m_faultType = m_systemIndex;//���ơ���������������
                oneControlFault->m_FaultCommandCode = commandcode;//addFaults[i].m_FaultCommandCode;
                oneControlFault->m_deviceParamInfoID = item;
                m_pFaultParamDAO2->InsertFaultParamInfo(oneControlFault);
            }
        }
        else
        {

            for (auto item : addFaults[i].m_responseCommandID)
            {
                FaultFaultCommandInfo* oneCommanFault = new FaultFaultCommandInfo();
                oneCommanFault->m_name = addFaults[i].m_name;
                oneCommanFault->m_rocketID = m_app->m_rockedType;
                oneCommanFault->m_faultType = m_systemIndex;//���ơ���������������
                oneCommanFault->m_FaultCommandCode = commandcode;//addFaults[i].m_FaultCommandCode;
                oneCommanFault->m_responseCommandID = item;
                m_pFaultParamDAO2->InsertFaultCommandInfoFrame(oneCommanFault);
            }
        }
    }

    //ˢ�½���
    FlashFualtInfo();

}

/// <summary>
/// �༭����  ����Ҫ�Թ���ָ����б༭����Ϊ���ܸı�����
/// </summary>
/// <param name="editFaults"></param>
void CenterOperateF::EditFaultItem(vector<AddOneFaultInfo> editFaults)
{
    if (editFaults.size() <= 0) return;

    int isDel = 0;
    string editName;
    //ֱ��ɾ����Ȼ��д��
    for (int i = 0; i < editFaults.size(); i++)
    {
        //д�����ݿ� 1���������ϣ�2��ָ�����
        if (editFaults[i].m_Type == 1)
        {
            isDel = 0;
            for (auto item : editFaults[i].m_deviceParamInfoID)
            {
                //ɾ��
                if (isDel == 0)
                {
                    m_pFaultParamDAO2->DelFaultParamInfoFrame(editFaults[i].m_name);
                    isDel = 1;
                }

                //д��
                FaultFaultParamInfo* oneControlFault = new FaultFaultParamInfo();
                oneControlFault->m_name = editFaults[i].m_name;
                oneControlFault->m_rocketID = m_app->m_rockedType;
                oneControlFault->m_faultType = m_systemIndex;//���ơ���������������
                oneControlFault->m_FaultCommandCode = editFaults[i].m_FaultCommandCode;
                oneControlFault->m_deviceParamInfoID = item;
                m_pFaultParamDAO2->InsertFaultParamInfo(oneControlFault);
            }
        }
        else
        {
            isDel = 0;
            for (auto item : editFaults[i].m_responseCommandID)
            {
                //ɾ��
                if (isDel == 0)
                {
                    m_pFaultParamDAO2->DelFaultCommandInfoFrame(editFaults[i].m_name);
                    isDel = 1;
                }

                //д��
                FaultFaultCommandInfo* oneControlFault = new FaultFaultCommandInfo();
                oneControlFault->m_name = editFaults[i].m_name;
                oneControlFault->m_rocketID = m_app->m_rockedType;
                oneControlFault->m_faultType = m_systemIndex;//���ơ���������������
                oneControlFault->m_FaultCommandCode = editFaults[i].m_FaultCommandCode;
                oneControlFault->m_responseCommandID = item;
                m_pFaultParamDAO2->InsertFaultCommandInfoFrame(oneControlFault);
            }
        }
    }

    //ˢ�½���
    FlashFualtInfo();
}


/// <summary>
/// ɾ������
/// </summary>
/// <param name="delFaults"></param>
void CenterOperateF::DelFaultItem(vector<AddOneFaultInfo> delFaults)
{
    if (delFaults.size() <= 0) return;

    string delName;
    //�������� ֱ��ɾ�� 
    for (int i = 0; i < delFaults.size(); i++)
    {
        //�Թ���ָ��͹��ϻ������ɾ��
        delName = delFaults[i].m_name;
        m_pFaultDAO2->DelCommandInfoParam(delName + "ָ��");
        m_pFaultDAO2->DelCommandInfoParam(delName + "����");

        if (delFaults[i].m_Type == 1)
        {
            //ɾ������(�ù������Ƶ����а󶨲���)
            m_pFaultParamDAO2->DelFaultParamInfoFrame(delName);
        }
        else
        {
            //ɾ������(�ù������Ƶ����а�ָ��)
            m_pFaultParamDAO2->DelFaultCommandInfoFrame(delName);
        }
    }

    //ˢ�½���
    FlashFualtInfo();
}

/// <summary>
/// �յ�����ע��������
/// </summary>
/// <param name="oneCommand"></param>
void CenterOperateF::receiverCMD(QByteArray oneCommand)
{
    //��ֹһ�����գ��Ƶ�ע��״̬δ������
    if (m_isSendOne == false)
        return;
    int backCode = 0, sendCode = 0;
    memcpy(&backCode, oneCommand.data() + 2, 2);//�ָⷢ��code 
    memcpy(&sendCode, oneCommand.data() + 5, 2);//�ָⷢ��code  
    if (backCode != m_BackCode)
        return;
    if (sendCode != m_sendCode)
        return;
    if (oneCommand.at(4) == 0x01)
        emit backOnFaultClick(m_faultType, true);//�����趨�ɹ�
    else
        emit backOnFaultClick(m_faultType, false);//�����趨ʧ��

    m_isSendOne = false;
    killTimer(m_TimerID);//�رն�ʱ��
}

/// <summary>
/// ���ָ���
/// </summary>
/// <param name="cmd_id"></param>
void CenterOperateF::sendCMD(int code, int type)
{
    if (m_app->m_CommandInfoframes.find(code) == m_app->m_CommandInfoframes.end())
    {
        return;
    }

    FaultCommandInfo* command = nullptr;
    PeerInfo* sendPeerInfo = nullptr;
    char headType = 0x55;//��������
    if (type == 1)
    {
        sendPeerInfo = m_app->m_paramSender;
        command = m_app->m_CommandInfoframes[code];
    }
    else
    {
        headType = 0x66;//ָ�����
        sendPeerInfo = m_app->m_cmdSender;
        command = m_app->m_CommandInfoframes[code];
    }

    //FaultCommandInfo* command = m_app->m_CommandInfoframes[cmd_id];
    QByteArray m_pBuff(FRAMELENGTH, Qt::Uninitialized);
    m_pBuff[0] = headType;//֡ͷ2�ֽ�
    m_pBuff[1] = 0xAA;
    //m_pBuff[2] = command->m_code; //�ָⷢ��code 1�ֽ�
    memcpy(m_pBuff.data() + 2, &command->m_code, 2);//�ָⷢ��code 
    m_pBuff[4] = 0x00;//Ԥ��
    m_pBuff[5] = 0x00;//Ԥ��
    m_pBuff[6] = 0x00;//Ԥ��
    m_pBuff[7] = 0x00;//Ԥ��
    m_pBuff[8] = 0x00;//У���2�ֽ�
    m_pBuff[9] = 0x00;

    m_sendCode = command->m_code;
    for (auto item : m_app->m_CommandInfoframes)
    {
        if (item.second->m_id == command->m_backID)
        {
            m_BackCode = item.second->m_code;
            break;
        }
    }

    unsigned char vdata1[FRAMELENGTH];
    for (int i = 0; i < FRAMELENGTH; i++)
    {
        vdata1[i] = (unsigned char)m_pBuff[i];
    }
    unsigned short checksum = CRC::CalCRC16(vdata1 + 2, FRAMELENGTH - 4);
    memcpy(m_pBuff.data() + FRAMELENGTH - 2, &checksum, 2);//���У��λ

    int SendLen = m_pSocket->writeDatagram(m_pBuff, QHostAddress(sendPeerInfo->m_strIP.c_str()), sendPeerInfo->m_iPort);
    if (SendLen != m_pBuff.size())
    {
        QMessageBox::warning(this, tr("��ʾ"), tr("����ָ���ʧ�ܣ�"));
        return;
    }

    m_isSendOne = true;
    m_TimerID = this->startTimer(5000);//3��û���յ�����������ʧ��

    //����
    //receiverCMD("");
}