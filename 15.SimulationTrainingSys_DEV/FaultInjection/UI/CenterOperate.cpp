#include "CenterOperate.h" 
#pragma execution_character_set("utf-8")//解决 qt中如何设置显示中文乱码问题

CenterOperate::CenterOperate(QWidget *parent)
    : QWidget(parent)
    , m_pFaultParamDAO2(nullptr)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);

    m_app = AppCache::instance();

    Init();
} 

void CenterOperate::Init()
{ 
    connect(ui.pb_addControlFault, &QPushButton::clicked, this, &CenterOperate::AddControlFaultClicked);
    connect(ui.pb_addMeasurementFault, &QPushButton::clicked, this, &CenterOperate::AddMeasurementFaultClicked);
    connect(ui.pb_addPowerFault, &QPushButton::clicked, this, &CenterOperate::AddPowerFaultClicked);

    m_controlError = new AddErrorCommand(); 
    connect(m_controlError, &AddErrorCommand::AddFaultItem, this, &CenterOperate::AddFaultItem);
    connect(m_controlError, &AddErrorCommand::DelFaultItem, this, &CenterOperate::DelFaultItem);
    connect(m_controlError, &AddErrorCommand::EditFaultItem, this, &CenterOperate::EditFaultItem);

    m_pFaultParamDAO2 = new DataBase::FaultParamDAO(m_app->m_outputPath);//数据库控制初初始化


    FlashFualtInfo();
}

/// <summary>
/// 刷新界面故障信息
/// </summary>
void CenterOperate::FlashFualtInfo()
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
                    if (m_AllFaultItems[k][j].FaultCommandID == it.second->m_FaultCommandID && m_AllFaultItems[k][j].Name == QString::fromStdString(it.second->m_name))
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
        oneItem.type = ParamFault;//参数故障
        oneItem.Name = QString::fromStdString(it.second->m_name);

        oneItem.FaultCommandID = it.second->m_FaultCommandID;//故障指令id

        //oneItem.responseCommandID = nullptr;//受影响指令id (是指令故障的参数)
        oneItem.deviceParamInfoID.push_back(it.second->m_deviceParamInfoID);//参数id  device_param_info_id
        oneItem.deviceID = 0;//TODO设备id
        oneItem.createTime = QString::fromStdString(it.second->m_createTime);
        oneItem.lastUpdateTime = QString::fromStdString(it.second->m_lastUpdateTime);

        //控制、测量、动力分类显示
        switch (it.second->m_faultType)
        {
        case ControlFault:
            m_AllFaultItems[ControlFault].push_back(oneItem);//TODO在每次添加或删除故障参数够也需要对其进行修改
            break;
        case MeasurementFault:
            m_AllFaultItems[2].push_back(oneItem);
            break;
        case PowerFault:
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
                    if (m_AllFaultItems[k][j].FaultCommandID == it.second->m_FaultCommandID && m_AllFaultItems[k][j].Name == QString::fromStdString(it.second->m_name))
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
        oneItem.type = CommandFault;//指令故障
        oneItem.Name = QString::fromStdString(it.second->m_name);

        oneItem.FaultCommandID = it.second->m_FaultCommandID;//故障指令id
        oneItem.responseCommandID.push_back(it.second->m_responseCommandID);//受影响指令id
        //oneItem.deviceParamInfoID = "";//参数id (是参数故障的参数)
        oneItem.deviceID = 0;//TODO设备id
        oneItem.createTime = QString::fromStdString(it.second->m_createTime);
        oneItem.lastUpdateTime = QString::fromStdString(it.second->m_createTime);

        //控制、测量、动力分类显示
        switch (it.second->m_faultType)
        {
        case ControlFault:
            m_AllFaultItems[ControlFault].push_back(oneItem);//TODO在每次添加或删除故障参数够也需要对其进行修改
            break;
        case MeasurementFault:
            m_AllFaultItems[2].push_back(oneItem);
            break;
        case PowerFault:
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
    connect(m_systemItems[ControlFault], &MyFaultParameters::onFaultClick, this, &CenterOperate::isEnabledFault);

    m_systemItems[MeasurementFault] = new MyFaultParameters(ui.wgt_Measurement);
    connect(m_systemItems[MeasurementFault], &MyFaultParameters::onFaultClick, this, &CenterOperate::isEnabledFault);

    m_systemItems[PowerFault] = new MyFaultParameters(ui.wgt_Power);
    connect(m_systemItems[PowerFault], &MyFaultParameters::onFaultClick, this, &CenterOperate::isEnabledFault);

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
void CenterOperate::AddControlFaultClicked()
{
    m_systemIndex = ControlFault;
    m_controlError->SetAddIndex(m_systemIndex, m_AllFaultItems[m_systemIndex]);

    //传入当前有的控制系统故障



    m_controlError->showNormal();
}

/// <summary>
/// 添加测量系统故障
/// </summary>
void CenterOperate::AddMeasurementFaultClicked()
{
    m_systemIndex = MeasurementFault;
    m_controlError->SetAddIndex(m_systemIndex, m_AllFaultItems[m_systemIndex]);
    m_controlError->show();
}

/// <summary>
/// 添加动力系统故障
/// </summary>
void CenterOperate::AddPowerFaultClicked()
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
void CenterOperate::isEnabledFault(QString name, int code, bool isSave)
{
    //TODO 这里UDP组播发送故障指令
    name = "1";




}

/// <summary>
/// 添加故障项
/// </summary>
/// <param name="name"></param>

void CenterOperate::AddFaultItem(vector<AddOneFaultInfo> addFaults)
{
    //添加
    for (int i = 0; i < addFaults.size(); i++)
    {
        //写入数据库 1：参数故障；2：指令故障
        if (addFaults[i].m_Type == 1)
        {
            //一个故障绑定多条指令，数据库存储是一条对应一条
            for (auto item : addFaults[i].m_deviceParamInfoID)
            {
                FaultFaultParamInfo* oneControlFault = new FaultFaultParamInfo();
                oneControlFault->m_name = addFaults[i].m_name;
                oneControlFault->m_faultType = m_systemIndex;//控制、测量、动力分类
                oneControlFault->m_FaultCommandID = addFaults[i].m_FaultCommandID;
                oneControlFault->m_deviceParamInfoID = item;
                m_pFaultParamDAO2->InsertFaultParamInfo(oneControlFault);
            }
        }
        else
        {
            for (auto item : addFaults[i].m_responseCommandID)
            {
                FaultFaultCommandInfo* oneControlFault = new FaultFaultCommandInfo();
                oneControlFault->m_name = addFaults[i].m_name;
                oneControlFault->m_faultType = m_systemIndex;//控制、测量、动力分类
                oneControlFault->m_FaultCommandID = addFaults[i].m_FaultCommandID;
                oneControlFault->m_responseCommandID = item;
                m_pFaultParamDAO2->InsertFaultCommandInfoFrame(oneControlFault);
            }
        }
    }
    
    //刷新界面
    FlashFualtInfo();
}

/// <summary>
/// 编辑故障
/// </summary>
/// <param name="editFaults"></param>
void CenterOperate::EditFaultItem(vector<AddOneFaultInfo> editFaults)
{
    int isDel = 0;
    //直接删除，然后写入
    for (int i = 0; i < editFaults.size(); i++)
    {
        //写入数据库 1：参数故障；2：指令故障
        if (editFaults[i].m_Type == 1)
        {
            isDel = 0;
            for (auto item : editFaults[i].m_deviceParamInfoID)
            {
                //删除
                if (isDel == 0)
                {
                    m_pFaultParamDAO2->DelFaultParamInfoFrame(editFaults[i].m_name);
                    isDel = 1;
                }

                //写入
                FaultFaultParamInfo* oneControlFault = new FaultFaultParamInfo();
                oneControlFault->m_name = editFaults[i].m_name;
                oneControlFault->m_faultType = m_systemIndex;//控制、测量、动力分类
                oneControlFault->m_FaultCommandID = editFaults[i].m_FaultCommandID;
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
                oneControlFault->m_faultType = m_systemIndex;//控制、测量、动力分类
                oneControlFault->m_FaultCommandID = editFaults[i].m_FaultCommandID;
                oneControlFault->m_responseCommandID = item;
                m_pFaultParamDAO2->InsertFaultCommandInfoFrame(oneControlFault);
            }
        }
    }

    //刷新界面
    FlashFualtInfo();
}


void CenterOperate::DelFaultItem(vector<AddOneFaultInfo> delFaults)
{
    //根据名称 直接删除 
    for (int i = 0; i < delFaults.size(); i++)
    {
        //写入数据库 1：参数故障；2：指令故障
        if (delFaults[i].m_Type == 1)
        {
            //一个故障绑定多条指令，数据库存储是一条对应一条
            for (auto item : delFaults[i].m_deviceParamInfoID)
            {
                //删除
                m_pFaultParamDAO2->DelFaultParamInfoFrame(delFaults[i].m_name);
            }
        }
        else
        {
            for (auto item : delFaults[i].m_responseCommandID)
            {
                m_pFaultParamDAO2->DelFaultCommandInfoFrame(delFaults[i].m_name);
            }
        }
    }

    //刷新界面
    FlashFualtInfo();
}