#include "CenterOperateF.h" 
#pragma execution_character_set("utf-8")//解决 qt中如何设置显示中文乱码问题

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
/// 定时器响应函数
/// </summary>
/// <param name="event"></param>
void CenterOperateF::timerEvent(QTimerEvent* event)
{
    bool isOK = true;
    if (event->timerId() == m_TimerID)
    {
        QMessageBox::warning(nullptr, QString("提示"), "接收故障回令超时！","确定");
        killTimer(m_TimerID);//关闭定时器
    }
}


void CenterOperateF::Init()
{ 
    //指令故障回令
    m_pReceiveCMDDataF = new ReceiveCMDDataF(m_app->m_cmdResponseReceiver->m_strIP, m_app->m_cmdResponseReceiver->m_iPort);
    connect(m_pReceiveCMDDataF, &ReceiveCMDDataF::receiverCMD, this, &CenterOperateF::receiverCMD);

    //参数故障回令
    m_pReceiveParamData = new ReceiveCMDDataF(m_app->m_paramResponseReceiver->m_strIP, m_app->m_paramResponseReceiver->m_iPort);
    connect(m_pReceiveParamData, &ReceiveCMDDataF::receiverCMD, this, &CenterOperateF::receiverCMD);


    connect(ui.pb_addControlFault, &QPushButton::clicked, this, &CenterOperateF::AddControlFaultClicked);
    connect(ui.pb_addMeasurementFault, &QPushButton::clicked, this, &CenterOperateF::AddMeasurementFaultClicked);
    connect(ui.pb_addPowerFault, &QPushButton::clicked, this, &CenterOperateF::AddPowerFaultClicked);

    m_controlError = new AddErrorCommand(); 
    connect(m_controlError, &AddErrorCommand::AddFaultItem, this, &CenterOperateF::AddFaultItem);
    connect(m_controlError, &AddErrorCommand::DelFaultItem, this, &CenterOperateF::DelFaultItem);
    connect(m_controlError, &AddErrorCommand::EditFaultItem, this, &CenterOperateF::EditFaultItem);

    m_pFaultParamDAO2 = new DataBaseF::FaultParamDAO(m_app->m_outputPath);//数据库控制初初始化
    m_pFaultDAO2 = new DataBaseF::FaultDAO(m_app->m_outputPath);

    FlashFualtInfo();
}

/// <summary>
/// 刷新界面故障信息
/// </summary>
void CenterOperateF::FlashFualtInfo()
{
    //添加展示参数故障（fault_param_info表）
    m_AllFaultItems.clear();
    for (auto it : m_app->m_FaultParamInfoFrames)
    {
        //每次需要判断，当前参数故障是否影响到多个参数,若有进行处理放在vector中
        bool isexist = false;
        for (int k = 0; k < m_AllFaultItems.size(); k++)
        {
            for (int j = 0; j < m_AllFaultItems[k].size(); j++)
            {
                for (int i = 0; i < m_AllFaultItems[k][j].deviceParamInfoID.size(); i++)
                {
                    if (m_AllFaultItems[k][j].FaultCommandID == it.second->m_FaultCommandCode && m_AllFaultItems[k][j].Name == QString::fromStdString(it.second->m_name))
                    {
                        m_AllFaultItems[k][j].deviceParamInfoID.push_back(it.second->m_deviceParamInfoID);//同一个故障影响的不同参数进行合并
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

        //绑定故障指令code
        for (auto itemf : m_app->m_CommandInfoframes)
        {
            if (itemf.first == it.second->m_FaultCommandCode)
            {
                oneItem.code = itemf.second->m_code;
                break;
            }
        }

        oneItem.type = ParamFault;//参数故障
        oneItem.Name = QString::fromStdString(it.second->m_name);
        oneItem.FaultCommandID = it.second->m_FaultCommandCode;//故障指令id
        oneItem.deviceParamInfoID.push_back(it.second->m_deviceParamInfoID);//参数id  device_param_info_id

        for (auto itemd : m_app->m_FaultDeviceParamInfoFrames)
        {
            if ((itemd.second->m_id == it.second->m_deviceParamInfoID))
            {
                oneItem.deviceID = itemd.second->m_deviceID;//设备id
                break;
            }
        }

        oneItem.createTime = QString::fromStdString(it.second->m_createTime);
        oneItem.lastUpdateTime = QString::fromStdString(it.second->m_lastUpdateTime);

        //控制、测量、动力分类显示
        switch (it.second->m_faultType)
        {
        case ControlFault:
            oneItem.faultType = ControlFault;
            m_AllFaultItems[ControlFault].push_back(oneItem);//TODO在每次添加或删除故障参数够也需要对其进行修改
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

    //添加展示指令故障（fault_command_info表）
    for (auto it : m_app->m_FaultCommandInfoFrames)
    {
        //每次需要判断，当前指令故障是否影响到多个参数,若有进行处理放在vector中
        bool isexist = false;
        for (int k = 0; k < m_AllFaultItems.size(); k++)
        {
            for (int j = 0; j < m_AllFaultItems[k].size(); j++)
            {
                for (int i = 0; i < m_AllFaultItems[k][j].responseCommandID.size(); i++)
                {
                    if (m_AllFaultItems[k][j].FaultCommandID == it.second->m_FaultCommandCode && m_AllFaultItems[k][j].Name == QString::fromStdString(it.second->m_name))
                    {
                        m_AllFaultItems[k][j].responseCommandID.push_back(it.second->m_responseCommandID);//同一个故障影响的不同参数进行合并
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

        //绑定故障指令code
        for (auto itemf: m_app->m_CommandInfoframes)
        {
            if (itemf.first == it.second->m_FaultCommandCode)
            {
                oneItem.code = itemf.second->m_code;
                break;
            }
        }

        oneItem.type = CommandFault;//指令故障
        oneItem.Name = QString::fromStdString(it.second->m_name);

        oneItem.FaultCommandID = it.second->m_FaultCommandCode;//故障指令id
        oneItem.responseCommandID.push_back(it.second->m_responseCommandID);//受影响指令id
        //oneItem.deviceParamInfoID = "";//参数id (是参数故障的参数)
        oneItem.deviceID = 0;//TODO设备id

        oneItem.createTime = QString::fromStdString(it.second->m_createTime);
        oneItem.lastUpdateTime = QString::fromStdString(it.second->m_createTime);

        //控制、测量、动力分类显示
        switch (it.second->m_faultType)
        {
        case ControlFault:
            oneItem.faultType = ControlFault;
            m_AllFaultItems[ControlFault].push_back(oneItem);//TODO在每次添加或删除故障参数够也需要对其进行修改
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

    m_systemItems[ControlFault] = new MyFaultParameters(ui.wgt_Control);//为每个组创建树(子参数列表)
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
/// 添加控制系统故障
/// </summary>
void CenterOperateF::AddControlFaultClicked()
{
    m_systemIndex = ControlFault;
    m_controlError->SetAddIndex(m_systemIndex, m_AllFaultItems[m_systemIndex]);

    //传入当前有的控制系统故障



    m_controlError->showNormal();
}

/// <summary>
/// 添加测量系统故障
/// </summary>
void CenterOperateF::AddMeasurementFaultClicked()
{
    m_systemIndex = MeasurementFault;
    m_controlError->SetAddIndex(m_systemIndex, m_AllFaultItems[m_systemIndex]);
    m_controlError->show();
}

/// <summary>
/// 添加动力系统故障
/// </summary>
void CenterOperateF::AddPowerFaultClicked()
{
    m_systemIndex = PowerFault;
    m_controlError->SetAddIndex(m_systemIndex, m_AllFaultItems[m_systemIndex]);
    m_controlError->show(); 
}


/// <summary>
/// 故障失能和使能
/// UDP组播发送指令
/// </summary>
/// <param name="title"></param>
/// <param name="code"></param>
/// <param name="isSave"></param>
void CenterOperateF::isEnabledFault(QString name, int code, int type, int faultType, bool isAddFault)
{
    //TODO 这里UDP组播发送故障指令
    m_faultType = faultType;
    sendCMD(code,type, isAddFault);
}

/// <summary>
/// 添加故障项
/// </summary>
/// <param name="name"></param>

void CenterOperateF::AddFaultItem(vector<AddOneFaultInfo> addFaults)
{
    if (addFaults.size() <= 0) return;

    int commandcode = 0; 
    for (int i = 0; i < addFaults.size(); i++)
    {
        //先添加故障回令和故障指令
        FaultCommandInfo* oneCommandInfo = new FaultCommandInfo();
        oneCommandInfo->m_name = addFaults[i].m_name + "回令";
        oneCommandInfo->m_backID = 0;
        oneCommandInfo->m_prefix = 0X55AA;
        oneCommandInfo->m_code = m_pFaultDAO2->GetNewCode();
        oneCommandInfo->m_type = 3;
        oneCommandInfo->m_rocketID = m_app->m_rockedType;
        m_pFaultDAO2->InsertCommandInfo(oneCommandInfo);

        oneCommandInfo->m_name = addFaults[i].m_name + "指令";
        oneCommandInfo->m_backID = oneCommandInfo->m_id;//绑定回令id
        oneCommandInfo->m_code = m_pFaultDAO2->GetNewCode();;
        m_pFaultDAO2->InsertCommandInfo(oneCommandInfo);

        commandcode = oneCommandInfo->m_code;//得到指令code

        //再添加 1：参数故障；2：指令故障
        if (addFaults[i].m_Type == 1)
        {
            //一个故障绑定多条指令，数据库存储是一条对应一条
            for (auto item : addFaults[i].m_deviceParamInfoID)
            {
                FaultFaultParamInfo* oneControlFault = new FaultFaultParamInfo();
                oneControlFault->m_name = addFaults[i].m_name;
                oneControlFault ->m_rocketID = m_app->m_rockedType;
                oneControlFault->m_faultType = m_systemIndex;//控制、测量、动力分类
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
                oneCommanFault->m_faultType = m_systemIndex;//控制、测量、动力分类
                oneCommanFault->m_FaultCommandCode = commandcode;//addFaults[i].m_FaultCommandCode;
                oneCommanFault->m_responseCommandID = item;
                m_pFaultParamDAO2->InsertFaultCommandInfoFrame(oneCommanFault);
            }
        }
    }
    
    //刷新界面
    FlashFualtInfo();

}

/// <summary>
/// 编辑故障  不需要对故障指令进行编辑，因为不能改变名称
/// </summary>
/// <param name="editFaults"></param>
void CenterOperateF::EditFaultItem(vector<AddOneFaultInfo> editFaults)
{
    if (editFaults.size() <= 0) return;

#if 1 //能修改名称

    //删除
    string delName;
    //根据名称 直接删除 
    for (int i = 0; i < editFaults.size(); i++)
    {
        //对故障指令和故障回令进行删除
        delName = editFaults[i].m_oldName;
        m_pFaultDAO2->DelCommandInfoParam(delName + "指令");
        m_pFaultDAO2->DelCommandInfoParam(delName + "回令");

        if (editFaults[i].m_Type == 1)
        {
            //删除故障(该故障名称的所有绑定参数)
            m_pFaultParamDAO2->DelFaultParamInfoFrame(delName);
        }
        else
        {
            //删除故障(该故障名称的所有绑定指令)
            m_pFaultParamDAO2->DelFaultCommandInfoFrame(delName);
        }
    }

    //添加
    AddFaultItem(editFaults);
    //刷新界面
    FlashFualtInfo();

#else //原本的不能修改名称

    int isDel = 0;
    string editName;
    //直接删除，然后写入
    for (int i = 0; i < editFaults.size(); i++)
    {
        //写入数据库 1：参数故障；2：指令故障
        if (editFaults[i].m_Type == 1)
        {
            //删除
            m_pFaultParamDAO2->DelFaultParamInfoFrame(editFaults[i].m_oldName);

            //写入
            for (auto item : editFaults[i].m_deviceParamInfoID)
            {
                FaultFaultParamInfo* oneControlFault = new FaultFaultParamInfo();
                oneControlFault->m_name = editFaults[i].m_name;
                oneControlFault->m_rocketID = m_app->m_rockedType;
                oneControlFault->m_faultType = m_systemIndex;//控制、测量、动力分类
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
                //删除
                if (isDel == 0)
                {
                    m_pFaultParamDAO2->DelFaultCommandInfoFrame(editFaults[i].m_name);
                    isDel = 1;
                }             

                //写入
                FaultFaultCommandInfo* oneControlFault = new FaultFaultCommandInfo();
                oneControlFault->m_name = editFaults[i].m_name;
                oneControlFault->m_rocketID = m_app->m_rockedType;
                oneControlFault->m_faultType = m_systemIndex;//控制、测量、动力分类
                oneControlFault->m_FaultCommandCode = editFaults[i].m_FaultCommandCode;
                oneControlFault->m_responseCommandID = item;
                m_pFaultParamDAO2->InsertFaultCommandInfoFrame(oneControlFault);
            }
        }
    }
    //刷新界面
    FlashFualtInfo();

#endif // 0
}


/// <summary>
/// 删除故障
/// </summary>
/// <param name="delFaults"></param>
void CenterOperateF::DelFaultItem(vector<AddOneFaultInfo> delFaults)
{
    if (delFaults.size() <= 0) return;

    string delName;
    //根据名称 直接删除 
    for (int i = 0; i < delFaults.size(); i++)
    {
        //对故障指令和故障回令进行删除
        delName = delFaults[i].m_name;
        m_pFaultDAO2->DelCommandInfoParam(delName + "指令");
        m_pFaultDAO2->DelCommandInfoParam(delName + "回令");

        if (delFaults[i].m_Type == 1)
        {
            //删除故障(该故障名称的所有绑定参数)
            m_pFaultParamDAO2->DelFaultParamInfoFrame(delName);
        }
        else
        {
            //删除故障(该故障名称的所有绑定指令)
            m_pFaultParamDAO2->DelFaultCommandInfoFrame(delName);
        }
    }

    //刷新界面
    FlashFualtInfo();
}

/// <summary>
/// 收到故障注入回令解析
/// </summary>
/// <param name="oneCommand"></param>
void CenterOperateF::receiverCMD(QByteArray oneCommand)
{
    //防止一发多收，似的注入状态未变的情况
    if (m_isSendOne == false)
        return;
    int backCode = 0, sendCode = 0; 
    memcpy(&backCode, oneCommand.data() + 2, 2);//测发指令code 
    memcpy(&sendCode, oneCommand.data() + 5, 2);//测发指令code  
    if (backCode != m_BackCode)
        return;
    if (sendCode != m_sendCode)
        return; 
    if (oneCommand.at(4) == 0x01)
        emit backOnFaultClick(m_faultType,true);//故障设定成功
    else
        emit backOnFaultClick(m_faultType,false);//故障设定失败

    m_isSendOne = false;
    killTimer(m_TimerID);//关闭定时器
}

/// <summary>
/// 测控指令发送
/// </summary>
/// <param name="cmd_id"></param>
void CenterOperateF::sendCMD(int code, int type, bool isAddFault)
{
    if (m_app->m_CommandInfoframes.find(code) == m_app->m_CommandInfoframes.end())
    {
        return;
    }

    FaultCommandInfo* command = nullptr;
    PeerInfo* sendPeerInfo = nullptr;
    char headType = 0x55;//参数故障
    if (type == 1)
    {
        sendPeerInfo = m_app->m_paramSender;
        command = m_app->m_CommandInfoframes[code];
    }
    else
    {
        headType = 0x66;//指令故障
        sendPeerInfo = m_app->m_cmdSender;
        command = m_app->m_CommandInfoframes[code];
    }

    //FaultCommandInfo* command = m_app->m_CommandInfoframes[cmd_id];
    QByteArray m_pBuff(FRAMELENGTH, Qt::Uninitialized);
    m_pBuff[0] = headType;//帧头2字节
    m_pBuff[1] = 0xAA;
    //m_pBuff[2] = command->m_code; //测发指令code 1字节
    memcpy(m_pBuff.data() + 2, &command->m_code, 2);//测发指令code 
    m_pBuff[4] = isAddFault ? 0x01:0x00;//1:故障+1；0：故障-1
    m_pBuff[5] = 0x00;//预留
    m_pBuff[6] = 0x00;//预留
    m_pBuff[7] = 0x00;//预留
    m_pBuff[8] = 0x00;//校验和2字节
    m_pBuff[9] = 0x00;

    m_sendCode = command->m_code;
    for (auto item: m_app->m_CommandInfoframes)
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
    memcpy(m_pBuff.data() + FRAMELENGTH - 2, &checksum, 2);//填充校验位

    int SendLen = m_pSocket->writeDatagram(m_pBuff, QHostAddress(sendPeerInfo->m_strIP.c_str()), sendPeerInfo->m_iPort); 
    if (SendLen != m_pBuff.size())
    {
        QMessageBox::warning(this, tr("提示"), tr("故障指令发送失败！"));
        return;
    }

    m_isSendOne = true;
    m_TimerID = this->startTimer(5000);//3秒没有收到，就是设置失败
    
    //测试
    //receiverCMD("");
}