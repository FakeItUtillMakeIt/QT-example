#include "AddErrorCommand.h"
#include <QStandardItemModel>

#pragma execution_character_set("utf-8")//��� qt�����������ʾ������������


AddErrorCommand::AddErrorCommand(QWidget *parent)
	: QWidget(parent)
    , m_scrollAreaFault(nullptr)
    , m_isAddFault(false)
{
	ui.setupUi(this); 
	setAttribute(Qt::WA_ShowModal, true);

    m_app = AppCache::instance();

    setWindowFlags(Qt::FramelessWindowHint);//ȥ��������
    setAttribute(Qt::WA_TranslucentBackground, false);
    this->move(420, 108);    //����λ��

	Init();
}

AddErrorCommand::~AddErrorCommand()
{

}

void AddErrorCommand::Init()
{
    IsEnable(false);

    //��ӹ���ָ�� 
    int index = 0;
    for (auto item : m_app->m_CommandInfoframes)
    {
        if (item.second->m_type == 3)//����ָ��
        {
            ui.cb_commandName->addItem(QString::fromStdString(item.second->m_name), item.first);
           
            m_indexCommand.insert(pair<int, int>(index, item.first));
            index++;
        }  
    }

    //����豸����
    int indexDevice = 0;
    for (auto item : m_app->m_FaultDeviceInfoFrames)
    {
        if (item.second->m_rocketID == m_app->m_rockedType)//���ݻ������ɸѡ
        {
            ui.cb_deciveType->addItem(QString::fromStdString(item.second->m_name), item.first);

            m_indexDevice.insert(pair<int, int>(indexDevice, item.first));
            indexDevice++;
        }
    }
    
    //����������
    ui.pb_searchButton->setCursor(Qt::PointingHandCursor);
    ui.pb_searchButton->setFixedSize(30, 30);

    QMargins margins = ui.le_searchedit->textMargins();//��ֹ�ı�����������λ�ڰ�ť֮��
    ui.le_searchedit->setPlaceholderText("����");

    QHBoxLayout* pSearchLayout = new QHBoxLayout();
    pSearchLayout->addStretch();
    pSearchLayout->addWidget(ui.pb_searchButton);
    pSearchLayout->setSpacing(0);
    pSearchLayout->setContentsMargins(8, 0, 10, 0);
    ui.le_searchedit->setTextMargins(13 + 8 + 2, 0, 0, 0);
    ui.le_searchedit->setContentsMargins(0, 0, 0, 0);
    ui.le_searchedit->setLayout(pSearchLayout);
    ui.le_searchedit->setStyleSheet("height:29px;border:1px solid #eaeaea;background-color: rgb(255, 255, 255);border-radius:14px;");
    connect(ui.pb_searchButton, &QPushButton::clicked, this, &AddErrorCommand::SearchFault);

    m_hlayout = new QHBoxLayout; 
    m_myFaultTree = new MyFaultTree();
    connect(m_myFaultTree, &MyFaultTree::onFaultNodeClick, this, &AddErrorCommand::OnFaultNode);
    m_hlayout->addWidget(m_myFaultTree);
    ui.wgt_fualtList->setLayout(m_hlayout);


    //�������ݳ�ʼ������-�������б�
    m_scrollAreaFault = new QScrollArea(ui.wgt_ffTree);
    m_scrollAreaFault->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollAreaFault->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollAreaFault->setWidgetResizable(true);
    m_scrollAreaFault->setStyleSheet("background-color:transparent;border:0px;");
    m_scrollAreaFault->setFrameShape(QFrame::NoFrame);
    m_scrollAreaFault->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//���غ��������
    //���ù�������Ĵ���-�������б�
    m_scrollAreaFault->setWidget(ui.wgt_fualtList);


	connect(ui.pb_ok, &QPushButton::clicked, this, &AddErrorCommand::AddErrorOK);
	connect(ui.pb_cancel, &QPushButton::clicked, this, &AddErrorCommand::AddErrorCanCel);
    connect(ui.pb_close, &QPushButton::clicked, this, &AddErrorCommand::AddErrorCanCel);

    connect(ui.pb_addFault, &QPushButton::clicked, this, &AddErrorCommand::AddFault); 
    connect(ui.pb_edit, &QPushButton::clicked, this, &AddErrorCommand::EditFault);
    connect(ui.pb_del, &QPushButton::clicked, this, &AddErrorCommand::DelFault);

    connect(ui.cb_commandName, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &AddErrorCommand::onCommandNameChanged);
    connect(ui.cb_faultType, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &AddErrorCommand::onFaultTypeChanged);
    connect(ui.cb_deciveType, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &AddErrorCommand::onDeciveTypeChanged);

}


void AddErrorCommand::InitTable()
{
}

/// <summary>
/// �������ϲ���
/// </summary>
void AddErrorCommand::SearchFault()
{
    //TODO ��������ɸѡ���������ݲ�ѡ���һ��

}

/// <summary>
/// չʾ����
/// </summary>
/// <param name="index"></param>
/// <param name="faultItems"></param>
void AddErrorCommand::SetAddIndex(int index, vector<ItemStruct> faultItems)
{
    QString str = "";
    QString indexStr = "";
    this->move(420, 100);    //����λ��

    m_faultItems = faultItems;
    m_delFaults.clear();
    m_addFaults.clear();
    m_editFaults.clear();
    m_tempParamId.clear();
    m_tempCommandId.clear();

    switch (index)
    {
    case 1:
        indexStr = "����ϵͳ����ע��";
        break;
    case 2:
        indexStr = "����ϵͳ����ע��";
        break;
    case 3:
        indexStr = "����ϵͳ����ע��";
        break;
    default:
        break;
    }

    str = indexStr.append("�༭");
    ui.lb_title->setText(str);
    ui.le_faultgroup->setText(indexStr);

    m_myFaultTree->InitFaultTree(m_faultItems);

    //ѡ���һ��
    ui.pb_edit->setText("�༭");
    if (m_faultItems.size() > 0)
    {
        m_myFaultTree->selectItem(m_faultItems[0].Name);
    }
    else
    {
        ui.le_errorName->setText("");
        ui.cb_commandName->setCurrentIndex(0);
        ui.cb_deciveType->setCurrentIndex(0);
        ui.cb_faultType->setCurrentIndex(0);
        ui.le_errorName->setReadOnly(true);

        IsEnable(false);
        ShowAllParamTable(-1);//�ձ��
    }
    
    //����������
    int width = 0;
    int height = 0;
    width = 261;
    height = faultItems.size() * 40 + 80;
    m_scrollAreaFault->setGeometry(0, 0, 261, 680);
    ui.wgt_fualtList->setGeometry(QRect(0, 0, width, height));
    ui.wgt_fualtList->setMinimumSize(QSize(width, height));
}


/// <summary>
/// ��ӹ���
/// </summary>
void AddErrorCommand::AddFault()
{
    m_isAddFault = true;
    IsEnable(true);

    ui.pb_edit->setText("���");
    ui.le_errorName->setText("");
    ui.cb_commandName->setCurrentIndex(0);
    ui.cb_deciveType->setCurrentIndex(0);
    ui.cb_faultType->setCurrentIndex(0);
    ui.le_errorName->setReadOnly(false);

    m_tempParamId.clear();
    m_tempCommandId.clear();
    ShowAllParamTable(0);
}

/// <summary>
/// �����������Ӧ����
/// </summary>
/// <param name="name"></param>
void AddErrorCommand::OnFaultNode(QString name)
{
    //���ݴ����faultItems��name�Ƚϣ���ʾ����ǰ���ϵ���Ϣ����

    if (ui.pb_edit->text() == "���")
    {
        IsEnable(false);
        ui.le_errorName->setReadOnly(true);
        ui.pb_edit->setText("�༭");
    }
    
    ui.le_errorName->setReadOnly(true);
    for (auto item : m_faultItems)
    {
        if (item.Name == name)
        {
            //��������
            ui.le_errorName->setText(name);

            //����ָ��
            for (auto item2: m_indexCommand)
            {
                if (item2.second == item.FaultCommandID)
                {
                    ui.cb_commandName->setCurrentIndex(item2.first);
                }
            }

            if (item.type - 1 == 0)
            {
                m_tempParamId = item.deviceParamInfoID;//��������
            }
            else
            {
                m_tempCommandId = item.responseCommandID;//ָ�����
            }

            //��������
            ui.cb_faultType->setCurrentIndex(item.type - 1);

            //�豸���� TODO���費��Ҫ�ڲ������ϱ�������豸����
            for (auto item2 : m_indexDevice)
            {
                //if (item2.second == item.FaultDeviceType)
                //{
                //    ui.cb_deciveType->setCurrentIndex(item2.first);
                //}
            }

            //�����ʾ
            FlashParamTable(name);
        }
    }
}


/// <summary>
/// ɾ������
/// </summary>
void AddErrorCommand::DelFault()
{
    if (ui.le_errorName->text() == "")
    {
        QMessageBox::information(this, tr("��ʾ"), "��ѡ����Ҫɾ���Ĺ��ϣ�");
        return;
    }

    QString qstr = "�Ƿ�ɾ����" + ui.le_errorName->text() + "��";
    if (QMessageBox::Cancel == QMessageBox::warning(this, tr("����"), qstr,QMessageBox::Cancel | QMessageBox::Ok))
    {
        return;
    }

    //ɾ�������Ӧ��
    ItemStruct delFault;
    delFault.Name = ui.le_errorName->text();
    delFault.deviceID = m_indexDevice[ui.cb_deciveType->currentIndex()];//TODO�豸id����ȡ�豸����
    delFault.deviceParamInfoID = m_tempParamId;
    delFault.responseCommandID = m_tempCommandId;
    delFault.FaultCommandID = m_indexCommand[ui.cb_commandName->currentIndex()];//��ȡ����ָ������id
    delFault.type = ui.cb_faultType->currentIndex() + 1;
    
    for (vector<ItemStruct>::iterator iter = m_faultItems.begin(); iter != m_faultItems.end(); iter++)
    {
        if (iter->Name == delFault.Name) {
            m_faultItems.erase(iter);
            break;
        }
    }
    //m_myFaultTree->DelToolItem(delFault);//���һ��ɾ����������
    m_myFaultTree->InitFaultTree(m_faultItems);

    //�ݴ�����������ȷ����ɾ�����ݿ�
    AddOneFaultInfo oneFault;
    oneFault.m_name = delFault.Name.toStdString();
    oneFault.m_Type = delFault.type;
    oneFault.m_FaultCommandID = delFault.FaultCommandID;
    oneFault.m_deviceParamInfoID = delFault.deviceParamInfoID;
    oneFault.m_responseCommandID = delFault.responseCommandID;

    m_delFaults.push_back(oneFault);
}


/// <summary>
/// �༭����
/// </summary>
void AddErrorCommand::EditFault()
{
    if (ui.le_errorName->text() == "")
    {
        QMessageBox::information(this, tr("��ʾ"), "������������ƣ�");
        return;
    }

    if (m_faultItems.size() < 0)
    {
        QMessageBox::information(this, tr("��ʾ"), "����ӹ��ϣ�");
        return;
    }

    if (ui.pb_edit->text() == "�༭")
    {
        ui.pb_edit->setText("���");

        IsEnable(true);

        int index = ui.cb_faultType->currentIndex();
        ShowAllParamTable(index);//�����ȫ��ʾ

    }
    else
    {
        ui.pb_edit->setText("�༭");

        //���ܱ༭��ʧ�ܣ�ֻ��ʾ�����Ĳ���
        IsEnable(false);

        //��ӹ���  TODOע��ָ����ϺͲ�������ͬʱ���ڵ������Ҫ����������Ĺ��������Ƿ�������һ��������Ϣ�����������ͬʱ������
        if (m_isAddFault == true)
        {
            //�������
            ItemStruct addFault;
            addFault.Name = ui.le_errorName->text();
            addFault.deviceID = m_indexDevice[ui.cb_deciveType->currentIndex()];//TODO�豸id����ȡ�豸����
            addFault.deviceParamInfoID = m_tempParamId;
            addFault.responseCommandID = m_tempCommandId;
            addFault.FaultCommandID = m_indexCommand[ui.cb_commandName->currentIndex()];//��ȡ����ָ������id
            addFault.type = ui.cb_faultType->currentIndex() + 1;
            m_myFaultTree->addToolItem(addFault);

            m_faultItems.push_back(addFault);
            

            //�ݴ�����������ȷ����д�����ݿ�
            AddOneFaultInfo oneFault;
            oneFault.m_name = addFault.Name.toStdString();
            oneFault.m_Type = addFault.type;
            oneFault.m_FaultCommandID = addFault.FaultCommandID;
            oneFault.m_deviceParamInfoID = addFault.deviceParamInfoID;
            oneFault.m_responseCommandID = addFault.responseCommandID;
            m_addFaults.push_back(oneFault);
        }
        else//�޸�
        {
            //�޸Ľ���  ��name�ǲ������޸ĵģ�������
            ItemStruct editFault;
            editFault.Name = ui.le_errorName->text();
            editFault.deviceID = m_indexDevice[ui.cb_deciveType->currentIndex()];//TODO�豸id����ȡ�豸����
            editFault.deviceParamInfoID = m_tempParamId;
            editFault.responseCommandID = m_tempCommandId;
            editFault.FaultCommandID = m_indexCommand[ui.cb_commandName->currentIndex()];//��ȡ����ָ������id
            editFault.type = ui.cb_faultType->currentIndex() + 1;

            m_myFaultTree->EditToolItem(editFault);

            //�޸�
            for (int i = 0; i < m_faultItems.size(); i++)
            {
                if (m_faultItems[i].Name == editFault.Name)
                {
                    m_faultItems[i] = editFault;
                    break;
                }
            }

           //�ݴ�����������ȷ����д�޸����ݿ�
           AddOneFaultInfo oneFault;
           oneFault.m_name = editFault.Name.toStdString();
           oneFault.m_Type = editFault.type;
           oneFault.m_FaultCommandID = editFault.FaultCommandID;
           oneFault.m_deviceParamInfoID = editFault.deviceParamInfoID;
           oneFault.m_responseCommandID = editFault.responseCommandID;

           m_editFaults.push_back(oneFault);
        }

        m_isAddFault = false;

    }

}

/// <summary>
/// ��������ѡ����Ӧ
/// </summary>
/// <param name="tempName"></param>
void AddErrorCommand::onCommandNameChanged(QString tempName)
{
    //ˢ�²������
    
}

/// <summary>
/// ��������ѡ����Ӧ
/// </summary>
/// <param name="tempName"></param>
void AddErrorCommand::onFaultTypeChanged(QString tempName)
{
    if (tempName == "��������")
    {
        ui.cb_deciveType->setVisible(true);
        ui.lb_device->setVisible(true);

        ShowAllParamTable(0);
    }
    else if(tempName == "ָ�����")
    {
        ui.cb_deciveType->setVisible(false);
        ui.lb_device->setVisible(false);

        ShowAllParamTable(1);
    }
    else
    {
        ui.cb_deciveType->setVisible(false);
        ui.lb_device->setVisible(false);
        ShowAllParamTable(2);
    }
}

/// <summary>
/// �豸����ѡ����Ӧ
/// </summary>
/// <param name="tempName"></param>
void AddErrorCommand::onDeciveTypeChanged(QString tempName)
{

    //ˢ�²������

    
}


/// <summary>
/// ��ʾ��ѡ����
/// </summary>
/// index 0����ʾ��ʾ�������Ͽ�ѡ�Ĳ�����1����ʾָ����Ͽ�ѡ�Ĳ���;2��ȫ��ʾ.
/// <param name="index"></param>
void AddErrorCommand::ShowAllParamTable(int index)
{
    ui.tv_paramInfo->setMinimumSize(700, 500);
    ui.tv_paramInfo->verticalHeader()->hide(); //����ͷ

    QStandardItemModel* m_tableModel = new QStandardItemModel(this);
    ui.tv_paramInfo->setModel(m_tableModel); // recommend to set model before detail settings

    QStringList columnTitles;
    columnTitles << "���" << "ID" << "����" << "����ʱ��" << "������ʱ��" << "ѡ��";
    m_tableModel->setHorizontalHeaderLabels(columnTitles);


    ////ˢ�±������ 
    int tableitem = 0;
    int temp = 0;
    bool isexit = false;
    if (index == 0 || index == 2)
    {
        //��������豸��������
        for (int i = 0; i < m_app->m_FaultParameterInfoFrames.size(); i++)
        {
            isexit = false;
            tableitem = 0;
            m_tableModel->setItem(i, tableitem++, new QStandardItem(QString("%1").arg(i)));
            m_tableModel->setItem(i, tableitem++, new QStandardItem(QString("%1").arg(m_app->m_FaultParameterInfoFrames[i + 1]->m_id)));//����i�У���0�����һ������
            m_tableModel->setItem(i, tableitem++, new QStandardItem(QString::fromStdString(m_app->m_FaultParameterInfoFrames[i + 1]->m_name)));
            m_tableModel->setItem(i, tableitem++, new QStandardItem(QString::fromStdString(m_app->m_FaultParameterInfoFrames[i + 1]->m_createTime)));
            m_tableModel->setItem(i, tableitem++, new QStandardItem(QString::fromStdString(m_app->m_FaultParameterInfoFrames[i + 1]->m_lastUpdateTime)));

            m_tableModel->setItem(i, tableitem++, new QStandardItem("ѡ��"));
            ui.tv_paramInfo->setModel(m_tableModel);

            //��Ӵ���ؼ�
            m_chooseBtn = new QToolButton();
            ui.tv_paramInfo->setIndexWidget(m_tableModel->index(i, 5), m_chooseBtn);

            //��֮ǰ�Ѿ��󶨵Ĳ������б�ʶ
            for (int k = 0; k < m_tempParamId.size(); k++)
            {
                if (m_tempParamId[k] == m_app->m_FaultParameterInfoFrames[i + 1]->m_id)
                {
                    isexit = true;
                    break;
                }
            }

            if (isexit)
            {
                m_chooseBtn->setStyleSheet("image:url(:/FaultInjection/images/CheckboxOn.png);border:0px;");
            }
            else
            {
                m_chooseBtn->setStyleSheet("image:url(:/FaultInjection/images/CheckboxOff.png);border:0px;");
            }
           
            connect(m_chooseBtn, &QPushButton::clicked, this, &AddErrorCommand::IsChooseParam);

            temp++;
        }
    }
    
    if (index == 1 || index == 2)
    {
        //�������ָ���������
        //TODO ������Ҫʹ������m_app->m_CommandInfoframes[j + 1]�����ʣ�����ǰ�ж��Ƿ���ڣ����Ҫ�ж��Ƿ�����ꡣ
        for (auto item : m_app->m_CommandInfoframes)
        {
            if (item.second->m_type != 1)
            {
                continue;
            }

            isexit = false;
            tableitem = 0;
            m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString("%1").arg(temp)));
            m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString("%1").arg(item.second->m_id)));//����i�У���0�����һ������
            m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_name)));
            m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_createTime)));
            m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_lastUpdateTime)));

            m_tableModel->setItem(temp, tableitem++, new QStandardItem("ѡ��"));
            ui.tv_paramInfo->setModel(m_tableModel);

            //��Ӵ���ؼ�
            m_chooseBtn = new QToolButton();
            ui.tv_paramInfo->setIndexWidget(m_tableModel->index(temp, 5), m_chooseBtn);

            //��֮ǰ�Ѿ��󶨵Ĳ������б�ʶ
            for (int i = 0; i < m_tempCommandId.size(); i++)
            {
                if (m_tempCommandId[i] == item.second->m_id)
                {
                    isexit = true;
                    break;
                }
            }

            if (isexit)
            {
                m_chooseBtn->setStyleSheet("image:url(:/FaultInjection/images/CheckboxOn.png);border:0px;");
            }
            else
            {
                m_chooseBtn->setStyleSheet("image:url(:/FaultInjection/images/CheckboxOff.png);border:0px;");
            }
            connect(m_chooseBtn, &QPushButton::clicked, this, &AddErrorCommand::IsChooseParam);

            temp++;
        }
    }

    ////���ñ������
    for (int i = 0; i <= tableitem; i++)
    {
        if (i == 0 || i == 1)
            ui.tv_paramInfo->setColumnWidth(i, 50);
        else if (i == 6)
            ui.tv_paramInfo->setColumnWidth(i, 100);
        else
            ui.tv_paramInfo->setColumnWidth(i, 200);//�趨����i�п��setAlternatingRowColors(true);
    }
    ui.tv_paramInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);  //���ñ������ֻ�������ܱ༭
    ui.tv_paramInfo->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);        //��ͷ��Ϣ��ʾ����
    ui.tv_paramInfo->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);  //�趨��ͷ�п��ɱ�
    ui.tv_paramInfo->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);//�趨��3�б�ͷ��������
    ui.tv_paramInfo->setSelectionBehavior(QAbstractItemView::SelectRows); //����ѡ��ʱ����ѡ��
    //ui.tv_paramInfo->setStyleSheet("QTableView::item{selection-background-color:rgb(224, 239, 255)}");//�ı�ѡ������ɫ
    //ui.tv_paramInfo->setContextMenuPolicy(Qt::CustomContextMenu);//��Ҫ�ڱ��ʹ���Ҽ��˵�����Ҫ����������

    ui.tv_paramInfo->setAlternatingRowColors(true);//������ż�н�����ɫ
    QHeaderView* verticalHeader = ui.tv_paramInfo->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(30);//���ø߶�(2)
    ui.tv_paramInfo->horizontalHeader()->setMinimumHeight(38);//���ñ�ͷ�߶�
    ui.tv_paramInfo->horizontalHeader()->setStyleSheet("QHeaderView::section {"
        "color: black;padding-left: 4px;border: 1px solid #6c6c6c;}");

    //ui.tv_paramInfo->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);//������ѡ
}

/// <summary>
/// ˢ�²������
/// </summary>
/// <param name="tempName"></param>
void AddErrorCommand::FlashParamTable(QString tempName)
{
    ui.tv_paramInfo->setMinimumSize(700, 500);
    ui.tv_paramInfo->verticalHeader()->hide(); //����ͷ

    QStandardItemModel* m_tableModel = new QStandardItemModel(this);
    ui.tv_paramInfo->setModel(m_tableModel); // recommend to set model before detail settings

    QStringList columnTitles;
    columnTitles << "���" << "ID" << "����" << "����ʱ��" << "������ʱ��" << "ѡ��";
    m_tableModel->setHorizontalHeaderLabels(columnTitles);

    m_tempCommandId.clear();
    m_tempParamId.clear();

    ////ˢ�±������ 
    int tableitem = 0;
    for (int j = 0; j < m_faultItems.size(); j++)
    {
        if (tempName != m_faultItems[j].Name)
        {
            continue;
        }

        if (m_faultItems[j].type == 1)
        {
            //��������
            for (int i = 0; i < m_faultItems[j].deviceParamInfoID.size(); i++)
            {
                tableitem = 0;
                m_tableModel->setItem(i, tableitem++, new QStandardItem(QString("%1").arg(i)));
                m_tableModel->setItem(i, tableitem++, new QStandardItem(QString("%1").arg(m_faultItems[j].deviceParamInfoID[i])));//����i�У���0�����һ������

                for (auto item: m_app->m_FaultParameterInfoFrames)
                {
                    if (item.first == m_faultItems[j].deviceParamInfoID[i])
                    {
                        m_tableModel->setItem(i, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_name)));
                        m_tableModel->setItem(i, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_createTime)));
                        m_tableModel->setItem(i, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_lastUpdateTime)));

                        m_tempParamId.push_back(item.first);
                        break;
                    }
                }
                
                m_tableModel->setItem(i, tableitem++, new QStandardItem("ѡ��"));
                ui.tv_paramInfo->setModel(m_tableModel);

                //��Ӵ���ؼ�
                m_chooseBtn = new QToolButton();
                ui.tv_paramInfo->setIndexWidget(m_tableModel->index(i, 5), m_chooseBtn);
                m_chooseBtn->setStyleSheet("image:url(:/FaultInjection/images/CheckboxOn.png);border:0px;");
                connect(m_chooseBtn, &QPushButton::clicked, this, &AddErrorCommand::IsChooseParam);
            }
        }
        else
        {
            //ָ�����
            for (int i = 0; i < m_faultItems[j].responseCommandID.size(); i++)
            {
                tableitem = 0;
                m_tableModel->setItem(i, tableitem++, new QStandardItem(QString("%1").arg(i)));
                m_tableModel->setItem(i, tableitem++, new QStandardItem(QString("%1").arg(m_faultItems[j].responseCommandID[i])));//����i�У���0�����һ������

                for (auto item : m_app->m_CommandInfoframes)
                {
                    if ((item.first == m_faultItems[j].responseCommandID[i]) && (item.second->m_type == 1))//ָ�����ͣ�1��ʾ�ָⷢ�2��ʾ�ⷢ���3��ʾ����ָ��
                    {
                        m_tableModel->setItem(i, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_name)));
                        m_tableModel->setItem(i, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_createTime)));
                        m_tableModel->setItem(i, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_lastUpdateTime)));
                        
                        m_tempCommandId.push_back(item.first);
                        break;
                    }
                }

                m_tableModel->setItem(i, tableitem++, new QStandardItem("ѡ��"));
                ui.tv_paramInfo->setModel(m_tableModel);

                //��Ӵ���ؼ�
                m_chooseBtn = new QToolButton();
                ui.tv_paramInfo->setIndexWidget(m_tableModel->index(i, 5), m_chooseBtn);
                m_chooseBtn->setStyleSheet("image:url(:/FaultInjection/images/CheckboxOn.png);border:0px;");
                connect(m_chooseBtn, &QPushButton::clicked, this, &AddErrorCommand::IsChooseParam);
            }
        }     
    }

    ////���ñ������
    for (int i = 0; i <= tableitem; i++)
    {
        if (i == 0 || i == 1)
            ui.tv_paramInfo->setColumnWidth(i, 50);
        else if (i == 6)
            ui.tv_paramInfo->setColumnWidth(i, 100);
        else
            ui.tv_paramInfo->setColumnWidth(i, 200);//�趨����i�п��setAlternatingRowColors(true);
    }
    ui.tv_paramInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);  //���ñ������ֻ�������ܱ༭
    ui.tv_paramInfo->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);        //��ͷ��Ϣ��ʾ����
    ui.tv_paramInfo->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);  //�趨��ͷ�п��ɱ�
    ui.tv_paramInfo->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);//�趨��3�б�ͷ��������
    ui.tv_paramInfo->setSelectionBehavior(QAbstractItemView::SelectRows); //����ѡ��ʱ����ѡ��
    //ui.tv_paramInfo->setStyleSheet("QTableView::item{selection-background-color:rgb(224, 239, 255)}");//�ı�ѡ������ɫ
    //ui.tv_paramInfo->setContextMenuPolicy(Qt::CustomContextMenu);//��Ҫ�ڱ��ʹ���Ҽ��˵�����Ҫ����������

    ui.tv_paramInfo->setAlternatingRowColors(true);//������ż�н�����ɫ
    QHeaderView* verticalHeader = ui.tv_paramInfo->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(30);//���ø߶�(2)
    ui.tv_paramInfo->horizontalHeader()->setMinimumHeight(38);//���ñ�ͷ�߶�
    ui.tv_paramInfo->horizontalHeader()->setStyleSheet("QHeaderView::section {"
        "color: black;padding-left: 4px;border: 1px solid #6c6c6c;}");

    //ui.tv_paramInfo->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);//������ѡ

}


/// <summary>
/// ����ѡ�����ȡ��ѡ��
/// </summary>
void AddErrorCommand::IsChooseParam()
{
    bool isChoose = true;

    QModelIndex index;
    QToolButton* senderObj = qobject_cast<QToolButton*>(sender());
    if (senderObj == nullptr)
    {
        return;
    }

    QModelIndex idx =ui.tv_paramInfo->indexAt(QPoint(senderObj->frameGeometry().x(),senderObj->frameGeometry().y()));
    int row=idx.row();
    
    //��ȡ��ǰ������id
    QAbstractItemModel* model = ui.tv_paramInfo->model();
    index = model->index(row,1);
    int id = model->data(index).toInt();

    QString name = ui.le_errorName->text();//��ȡ��ǰ��������

    if (m_isAddFault == false)
    {

    }
    vector<int>* tempIDs = nullptr;
    if (ui.cb_faultType->currentIndex() == 0)
    {
        tempIDs = &m_tempParamId;//��������
    }
    else
    {
        tempIDs = &m_tempCommandId;//ָ�����
    }

    for (vector<int>::iterator iter = tempIDs->begin(); iter != tempIDs->end(); iter++)
    {        
        if (*iter == id) 
        {
            isChoose = false;
            break;
        }
    }

    if (isChoose)
    {
        tempIDs->push_back(id);
        senderObj->setStyleSheet("image:url(:/FaultInjection/images/CheckboxOn.png);border:0px;");
    }
    else
    {
        //��vector��ɾ��ָ��Ԫ�� 
        for (vector<int>::iterator iter = tempIDs->begin(); iter != tempIDs->end(); iter++)
        {
            if (*iter == id) {
                tempIDs->erase(iter);
                break;
            }
        }
        senderObj->setStyleSheet("image:url(:/FaultInjection/images/CheckboxOff.png);border:0px;");
    }
}

/// <summary>
/// comboxʹ�ܿ���
/// </summary>
/// <param name="isOk"></param>
void AddErrorCommand::IsEnable(bool isOk)
{
    //ui.le_errorName->setReadOnly(!m_isAddFault);
    ui.cb_commandName->setEnabled(isOk);
    ui.cb_faultType->setEnabled(isOk);
    ui.cb_deciveType->setEnabled(isOk);
}


/// <summary>
/// ���ȷ��
/// </summary>
void AddErrorCommand::AddErrorOK()
{

    if (ui.pb_edit->text() == "���")
    {
        QMessageBox::information(this, tr("��ʾ"), "��ѡ����ɵ�ǰ���ϱ༭��");
        return;
    }

    //������⣬������ӹ��ϰ�ť
	emit AddFaultItem(m_addFaults);

    emit EditFaultItem(m_editFaults);

    emit DelFaultItem(m_delFaults);
	this->hide();
}

/// <summary>
/// ���ȡ��
/// </summary>
void AddErrorCommand::AddErrorCanCel()
{
	this->hide();
}



/// <summary>
/// �߿�����
/// </summary>
/// <param name="event"></param>
void AddErrorCommand::paintEvent(QPaintEvent* event)
{
    int border_width = 5;
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(border_width, border_width, this->width() - border_width * 2, this->height() - border_width * 2);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));
    QColor color(0, 0, 0, 250);
    for (int i = 0; i < border_width; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(border_width - i, border_width - i, this->width() - (border_width - i) * 2, this->height() - (border_width - i) * 2);
        color.setAlpha(125 - qSqrt(i) * 50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}

/// <summary>
/// �������
/// </summary>
/// <param name="event"></param>
void AddErrorCommand::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        mMoving = true;
        mLastMousePosition = event->globalPos();
    }
}

/// <summary>
/// �ƶ����
/// </summary>
/// <param name="event"></param>
void AddErrorCommand::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons().testFlag(Qt::LeftButton) && mMoving)
    {
        setCursor(Qt::OpenHandCursor);
        this->move(this->pos() + (event->globalPos() - mLastMousePosition));
        mLastMousePosition = event->globalPos();
    }
}

/// <summary>
/// �ͷ����
/// </summary>
/// <param name="event"></param>
void AddErrorCommand::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        mMoving = false;
        setCursor(Qt::ArrowCursor);
    }
}