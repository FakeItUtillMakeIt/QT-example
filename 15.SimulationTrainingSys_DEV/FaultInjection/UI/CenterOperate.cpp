#include "CenterOperate.h" 
#pragma execution_character_set("utf-8")//��� qt�����������ʾ������������

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

    m_pFaultParamDAO2 = new DataBase::FaultParamDAO(m_app->m_outputPath);//���ݿ���Ƴ���ʼ��


    FlashFualtInfo();
}

/// <summary>
/// ˢ�½��������Ϣ
/// </summary>
void CenterOperate::FlashFualtInfo()
{
    //���չʾ�������ϣ�fault_param_info��
    m_AllFaultItems.clear();
    for (auto it : m_app->m_FaultParamInfoFrames)
    {
        //ÿ����Ҫ�жϣ���ǰ���������Ƿ�Ӱ�쵽�������,���н��д������vector��
        bool isexist = false;
        for (int k = 0; k < m_AllFaultItems.size(); k++)
        {
            for (int j = 0; j < m_AllFaultItems[k].size(); j++)
            {
                for (int i = 0; i < m_AllFaultItems[k][j].deviceParamInfoID.size(); i++)
                {
                    if (m_AllFaultItems[k][j].FaultCommandID == it.second->m_FaultCommandID && m_AllFaultItems[k][j].Name == QString::fromStdString(it.second->m_name))
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
        oneItem.type = ParamFault;//��������
        oneItem.Name = QString::fromStdString(it.second->m_name);

        oneItem.FaultCommandID = it.second->m_FaultCommandID;//����ָ��id

        //oneItem.responseCommandID = nullptr;//��Ӱ��ָ��id (��ָ����ϵĲ���)
        oneItem.deviceParamInfoID.push_back(it.second->m_deviceParamInfoID);//����id  device_param_info_id
        oneItem.deviceID = 0;//TODO�豸id
        oneItem.createTime = QString::fromStdString(it.second->m_createTime);
        oneItem.lastUpdateTime = QString::fromStdString(it.second->m_lastUpdateTime);

        //���ơ�����������������ʾ
        switch (it.second->m_faultType)
        {
        case ControlFault:
            m_AllFaultItems[ControlFault].push_back(oneItem);//TODO��ÿ����ӻ�ɾ�����ϲ�����Ҳ��Ҫ��������޸�
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

    //���չʾָ����ϣ�fault_command_info��
    for (auto it : m_app->m_FaultCommandInfoFrames)
    {
        //ÿ����Ҫ�жϣ���ǰָ������Ƿ�Ӱ�쵽�������,���н��д������vector��
        bool isexist = false;
        for (int k = 0; k < m_AllFaultItems.size(); k++)
        {
            for (int j = 0; j < m_AllFaultItems[k].size(); j++)
            {
                for (int i = 0; i < m_AllFaultItems[k][j].responseCommandID.size(); i++)
                {
                    if (m_AllFaultItems[k][j].FaultCommandID == it.second->m_FaultCommandID && m_AllFaultItems[k][j].Name == QString::fromStdString(it.second->m_name))
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
        oneItem.type = CommandFault;//ָ�����
        oneItem.Name = QString::fromStdString(it.second->m_name);

        oneItem.FaultCommandID = it.second->m_FaultCommandID;//����ָ��id
        oneItem.responseCommandID.push_back(it.second->m_responseCommandID);//��Ӱ��ָ��id
        //oneItem.deviceParamInfoID = "";//����id (�ǲ������ϵĲ���)
        oneItem.deviceID = 0;//TODO�豸id
        oneItem.createTime = QString::fromStdString(it.second->m_createTime);
        oneItem.lastUpdateTime = QString::fromStdString(it.second->m_createTime);

        //���ơ�����������������ʾ
        switch (it.second->m_faultType)
        {
        case ControlFault:
            m_AllFaultItems[ControlFault].push_back(oneItem);//TODO��ÿ����ӻ�ɾ�����ϲ�����Ҳ��Ҫ��������޸�
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

    m_systemItems[ControlFault] = new MyFaultParameters(ui.wgt_Control);//Ϊÿ���鴴����(�Ӳ����б�)
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
/// ��ӿ���ϵͳ����
/// </summary>
void CenterOperate::AddControlFaultClicked()
{
    m_systemIndex = ControlFault;
    m_controlError->SetAddIndex(m_systemIndex, m_AllFaultItems[m_systemIndex]);

    //���뵱ǰ�еĿ���ϵͳ����



    m_controlError->showNormal();
}

/// <summary>
/// ��Ӳ���ϵͳ����
/// </summary>
void CenterOperate::AddMeasurementFaultClicked()
{
    m_systemIndex = MeasurementFault;
    m_controlError->SetAddIndex(m_systemIndex, m_AllFaultItems[m_systemIndex]);
    m_controlError->show();
}

/// <summary>
/// ��Ӷ���ϵͳ����
/// </summary>
void CenterOperate::AddPowerFaultClicked()
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
void CenterOperate::isEnabledFault(QString name, int code, bool isSave)
{
    //TODO ����UDP�鲥���͹���ָ��
    name = "1";




}

/// <summary>
/// ��ӹ�����
/// </summary>
/// <param name="name"></param>

void CenterOperate::AddFaultItem(vector<AddOneFaultInfo> addFaults)
{
    //���
    for (int i = 0; i < addFaults.size(); i++)
    {
        //д�����ݿ� 1���������ϣ�2��ָ�����
        if (addFaults[i].m_Type == 1)
        {
            //һ�����ϰ󶨶���ָ����ݿ�洢��һ����Ӧһ��
            for (auto item : addFaults[i].m_deviceParamInfoID)
            {
                FaultFaultParamInfo* oneControlFault = new FaultFaultParamInfo();
                oneControlFault->m_name = addFaults[i].m_name;
                oneControlFault->m_faultType = m_systemIndex;//���ơ���������������
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
                oneControlFault->m_faultType = m_systemIndex;//���ơ���������������
                oneControlFault->m_FaultCommandID = addFaults[i].m_FaultCommandID;
                oneControlFault->m_responseCommandID = item;
                m_pFaultParamDAO2->InsertFaultCommandInfoFrame(oneControlFault);
            }
        }
    }
    
    //ˢ�½���
    FlashFualtInfo();
}

/// <summary>
/// �༭����
/// </summary>
/// <param name="editFaults"></param>
void CenterOperate::EditFaultItem(vector<AddOneFaultInfo> editFaults)
{
    int isDel = 0;
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
                oneControlFault->m_faultType = m_systemIndex;//���ơ���������������
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
                //ɾ��
                if (isDel == 0)
                {
                    m_pFaultParamDAO2->DelFaultCommandInfoFrame(editFaults[i].m_name);
                    isDel = 1;
                }             

                //д��
                FaultFaultCommandInfo* oneControlFault = new FaultFaultCommandInfo();
                oneControlFault->m_name = editFaults[i].m_name;
                oneControlFault->m_faultType = m_systemIndex;//���ơ���������������
                oneControlFault->m_FaultCommandID = editFaults[i].m_FaultCommandID;
                oneControlFault->m_responseCommandID = item;
                m_pFaultParamDAO2->InsertFaultCommandInfoFrame(oneControlFault);
            }
        }
    }

    //ˢ�½���
    FlashFualtInfo();
}


void CenterOperate::DelFaultItem(vector<AddOneFaultInfo> delFaults)
{
    //�������� ֱ��ɾ�� 
    for (int i = 0; i < delFaults.size(); i++)
    {
        //д�����ݿ� 1���������ϣ�2��ָ�����
        if (delFaults[i].m_Type == 1)
        {
            //һ�����ϰ󶨶���ָ����ݿ�洢��һ����Ӧһ��
            for (auto item : delFaults[i].m_deviceParamInfoID)
            {
                //ɾ��
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

    //ˢ�½���
    FlashFualtInfo();
}