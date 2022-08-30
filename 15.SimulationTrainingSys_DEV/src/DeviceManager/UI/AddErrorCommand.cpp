#include "AddErrorCommand.h"
#include <QStandardItemModel>

#pragma execution_character_set("utf-8")//解决 qt中如何设置显示中文乱码问题
//QStandardItemModel* m_tableModel = nullptr;//TODO为什么放在头文件或者类中出错！！！

AddErrorCommand::AddErrorCommand(QWidget* parent)
    : QWidget(parent)
    , m_scrollAreaFault(nullptr)
    , m_isAddFault(false)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_ShowModal, true);

    m_app = AppCache::instance();

    setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    setAttribute(Qt::WA_TranslucentBackground, false);
    this->move(420, 108);    //设置位置

    Init();
}

AddErrorCommand::~AddErrorCommand()
{

}

void AddErrorCommand::Init()
{
    IsEnable(false);

    //添加设备类型
    int indexDevice = 0;
    for (auto item : m_app->m_FaultDeviceInfoFrames)
    {
        if (item.second->m_rocketID == m_app->m_rockedType)//根据火箭类型筛选
        {
            ui.cb_deciveType->addItem(QString::fromStdString(item.second->m_name), item.first);

            m_indexDevice.insert(pair<int, int>(indexDevice, item.first));
            indexDevice++;
        }
    }

    //表格初始化
    //InitTable();

    //设置搜索框
    ui.pb_searchButton->setCursor(Qt::PointingHandCursor);
    ui.pb_searchButton->setFixedSize(30, 30);

    QMargins margins = ui.le_searchedit->textMargins();//防止文本框输入内容位于按钮之下
    ui.le_searchedit->setPlaceholderText("搜索");

    QHBoxLayout* pSearchLayout = new QHBoxLayout();
    pSearchLayout->addStretch();
    pSearchLayout->addWidget(ui.pb_searchButton);
    pSearchLayout->setSpacing(0);
    pSearchLayout->setContentsMargins(8, 0, 10, 0);
    ui.le_searchedit->setTextMargins(13 + 8 + 2, 0, 0, 0);
    ui.le_searchedit->setContentsMargins(0, 0, 0, 0);
    ui.le_searchedit->setLayout(pSearchLayout);
    ui.le_searchedit->setStyleSheet("height:29px;border:1px solid #eaeaea;background-color: rgb(255, 255, 255);border-radius:14px;");
    connect(ui.pb_searchButton, &QPushButton::clicked, this, &AddErrorCommand::SearchFaults);
    
    //设置树状显示列表
    m_hlayout = new QHBoxLayout;
    m_myFaultTree = new MyFaultTree();
    connect(m_myFaultTree, &MyFaultTree::onFaultNodeClick, this, &AddErrorCommand::OnFaultNode);
    m_hlayout->addWidget(m_myFaultTree);
    ui.wgt_fualtList->setLayout(m_hlayout);

    //根据数据初始化窗体-父参数列表
    m_scrollAreaFault = new QScrollArea(ui.wgt_ffTree);
    m_scrollAreaFault->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollAreaFault->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollAreaFault->setWidgetResizable(true);
    m_scrollAreaFault->setStyleSheet("background-color:transparent;border:0px;");
    m_scrollAreaFault->setFrameShape(QFrame::NoFrame);
    m_scrollAreaFault->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏横向滚动条
    //设置滚动区域的窗体-父参数列表
    m_scrollAreaFault->setWidget(ui.wgt_fualtList);

    connect(ui.pb_ok, &QPushButton::clicked, this, &AddErrorCommand::AddErrorOK);
    connect(ui.pb_cancel, &QPushButton::clicked, this, &AddErrorCommand::AddErrorCanCel);
    connect(ui.pb_close, &QPushButton::clicked, this, &AddErrorCommand::AddErrorCanCel);

    connect(ui.pb_addFault, &QPushButton::clicked, this, &AddErrorCommand::AddFault);
    connect(ui.pb_edit, &QPushButton::clicked, this, &AddErrorCommand::EditFault);
    connect(ui.pb_del, &QPushButton::clicked, this, &AddErrorCommand::DelFault);

    connect(ui.cb_faultType, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &AddErrorCommand::onFaultTypeChanged);
    connect(ui.cb_deciveType, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &AddErrorCommand::onDeciveTypeChanged);

}


void AddErrorCommand::InitTable()
{
    
}

/// <summary>
/// 搜索故障参数
/// </summary>
void AddErrorCommand::SearchFaults()
{
    searchItems.clear();
    QString seachname = ui.le_searchedit->text();
    if (seachname == "")
    {
        m_myFaultTree->InitFaultTree(m_faultItems);
        return;
    }
        
    //存放收索到的数据
    for (int i = 0; i < m_faultItems.size(); i++)
    {
        if (m_faultItems[i].Name.contains(seachname))
        {
            searchItems.push_back(m_faultItems[i]);
        }
    }

    if (searchItems.size() <= 0)
    {
        QMessageBox::warning(this, tr("提示"), tr("没有搜到相关参数！"));
        return;
    }

    m_myFaultTree->InitFaultTree(searchItems);
}

void AddErrorCommand::SetAddIndex(int index, vector<ItemStruct> faultItems)
{
    QString str = "";
    QString indexStr = "";

    m_faultType = index;
    m_isAddFault = false;
    ui.cb_faultType->setEnabled(false);
    ui.cb_deciveType->setEnabled(false);
    m_faultItems = faultItems;
    m_delFaults.clear();
    m_addFaults.clear();
    m_editFaults.clear();
    m_tempParamId.clear();
    m_tempCommandId.clear();
    
    this->move(420, 100);    //设置位置


    switch (m_faultType)
    {
    case 1:
        indexStr = "控制系统故障注入";
        break;
    case 2:
        indexStr = "测量系统故障注入";
        break;
    case 3:
        indexStr = "动力系统故障注入";
        break;
    default:
        break;
    }
    
    ui.le_faultgroup->setText(indexStr);
    str = indexStr.append("编辑");
    ui.lb_title->setText(str);
    

    m_myFaultTree->InitFaultTree(m_faultItems);

    //选择第一项
    ui.pb_edit->setText("编辑");
    if (m_faultItems.size() > 0)
    {
        m_myFaultTree->selectItem(m_faultItems[0].Name);
    }
    else
    {
        ui.le_errorName->setText("");
        ui.cb_deciveType->setCurrentIndex(0);
        ui.cb_faultType->setCurrentIndex(0);
        ui.le_errorName->setReadOnly(true);

        IsEnable(false);
        ShowParamsTable(-1);//空表格
    }

    //滚动条设置
    int width = 0;
    int height = 0;
    width = 261;
    height = faultItems.size() * 40 + 80;
    m_scrollAreaFault->setGeometry(0, 0, 261, 786);
    ui.wgt_fualtList->setGeometry(QRect(0, 0, width, height));
    ui.wgt_fualtList->setMinimumSize(QSize(width, height));
}


/// <summary>
/// 添加故障
/// </summary>
void AddErrorCommand::AddFault()
{
    m_isAddFault = true;
    IsEnable(true);

    ui.pb_edit->setText("完成");
    ui.le_errorName->setText("");
    ui.cb_deciveType->setCurrentIndex(0);
    ui.cb_faultType->setCurrentIndex(0);
    ui.le_errorName->setReadOnly(false);

    m_tempParamId.clear();
    m_tempCommandId.clear();
    ShowParamsTable(0,0);//参数故障，在进行设备筛选
}

/// <summary>
/// 点击故障树响应函数
/// </summary>
/// <param name="name"></param>
void AddErrorCommand::OnFaultNode(QString name, int deviceid)
{
    //根据传入的faultItems和name比较，显示出当前故障的信息数据

    m_isAddFault = false;
    if (ui.pb_edit->text() == "完成")
    {
        ui.pb_edit->setText("编辑");
        IsEnable(false);
    }

    ui.le_errorName->setReadOnly(true);
    for (auto item : m_faultItems)
    {
        if (item.Name == name)
        {
            //故障名称
            ui.le_errorName->setText(name);
            m_currentComandId = item.FaultCommandID;//记录当前故障指令的id

            //故障类型
            if (item.type == 1)
            {
                m_tempParamId = item.deviceParamInfoID;//参数故障
                
                for (auto item2 : m_indexDevice)
                {
                    if (item2.second == item.deviceID)
                    {
                        ui.cb_deciveType->setCurrentIndex(item2.first);//设备类型
                        break;
                    }
                }
            }
            else
            {
                m_tempCommandId = item.responseCommandID;//指令故障

            }
            ui.cb_faultType->setCurrentIndex(item.type - 1);

            //表格显示
            FlashParamTable(name,deviceid);
        }
    }
}


/// <summary>
/// 删除故障
/// </summary>
void AddErrorCommand::DelFault()
{
    if (ui.le_errorName->text() == "")
    {
        QMessageBox::information(this, tr("提示"), "请选择需要删除的故障！");
        return;
    }

    QString qstr = "确认删除：" + ui.le_errorName->text() + "吗？";
    int ret = QMessageBox::warning(this, QString("警告"), qstr, "取消", "确定");
    if (ret == 0)
    {
        return;
    }
    //if (QMessageBox::Cancel == QMessageBox::warning(this, tr("警告"), qstr, QMessageBox::Cancel | QMessageBox::Ok)) return;

    //删除界面对应项
    ItemStruct delFault;
    delFault.Name = ui.le_errorName->text();
    delFault.deviceID = m_indexDevice[ui.cb_deciveType->currentIndex()];//TODO设备id，获取设备类型
    delFault.deviceParamInfoID = m_tempParamId;
    delFault.responseCommandID = m_tempCommandId;
    delFault.type = ui.cb_faultType->currentIndex() + 1;

    for (vector<ItemStruct>::iterator iter = m_faultItems.begin(); iter != m_faultItems.end(); iter++)
    {
        if (iter->Name == delFault.Name) {
            m_faultItems.erase(iter);
            break;
        }
    }
    //m_myFaultTree->DelToolItem(delFault);//最后一个删除存在问题
    m_myFaultTree->InitFaultTree(m_faultItems);

    //暂存下来，后续确定后删除数据库
    AddOneFaultInfo oneFault;
    oneFault.m_name = delFault.Name.toStdString();
    oneFault.m_Type = delFault.type;
    oneFault.m_FaultCommandCode = delFault.FaultCommandID;
    oneFault.m_deviceParamInfoID = delFault.deviceParamInfoID;
    oneFault.m_responseCommandID = delFault.responseCommandID;

    m_delFaults.push_back(oneFault);


    if (m_faultItems.size() <= 0)
    {
        ui.le_errorName->setText("");
        ui.cb_deciveType->setCurrentIndex(0);
        ui.cb_faultType->setCurrentIndex(0);
        ui.le_errorName->setReadOnly(true);

        IsEnable(false);
        ShowParamsTable(-1);//空表格
    }
    else
    {
        m_myFaultTree->selectItem(m_faultItems[0].Name);
    }
}


/// <summary>
/// 编辑故障
/// </summary>
void AddErrorCommand::EditFault()
{
    if (ui.le_errorName->text() == "")
    {
        QMessageBox::information(this, tr("提示"), "请输入故障名称！");
        return;
    }

    if (m_faultItems.size() < 0)
    {
        QMessageBox::information(this, tr("提示"), "请添加故障！");
        return;
    }

    if (ui.pb_edit->text() == "编辑")
    {
        ui.pb_edit->setText("完成");

        IsEnable(true);

        ShowParamsTable(ui.cb_faultType->currentIndex(),ui.cb_deciveType->currentIndex());//表格中全显示
    }
    else
    {
        //TODO  目前不支持即影响参数故障又影响指令故障。如果需要则需要判断是参数故障还是指令故障，如果是修改且需要判断之前是什么故障，更要注意数据库的增删该查
        if (ui.cb_faultType->currentIndex() == 0)
        {
            if (m_tempParamId.size() <= 0)
            {
                QMessageBox::information(this, tr("提示"), "故障至少影响一个参数！");
                return;
            }
        }
        else
        {
            if (m_tempCommandId.size() <= 0)
            {
                QMessageBox::information(this, tr("提示"), "故障至少影响一个指令！");
                return;
            }
        }
       

        //添加故障  TODO注意指令故障和参数故障同时存在的情况（要考虑两个表的故障名称是否重名，一个故障信息对两个表进行同时操作）
        if (m_isAddFault == true)
        {
            //添加故障有效性判断
            if (AddFaultValidity() == false) return;

            //加入界面m_faultType
            ItemStruct addFault;
            addFault.faultType = m_faultType;
            addFault.Name = ui.le_errorName->text();
            addFault.deviceID = m_indexDevice[ui.cb_deciveType->currentIndex()];//TODO设备id，获取设备类型
            addFault.deviceParamInfoID = m_tempParamId;
            addFault.responseCommandID = m_tempCommandId;
            addFault.type = ui.cb_faultType->currentIndex() + 1;
            m_myFaultTree->addToolItem(addFault);

            m_faultItems.push_back(addFault);


            //暂存下来，后续确定后写入数据库
            AddOneFaultInfo oneFault;
            oneFault.faultType = m_faultType;
            oneFault.m_name = addFault.Name.toStdString();
            oneFault.m_Type = addFault.type;
            oneFault.m_FaultCommandCode = addFault.FaultCommandID;
            oneFault.m_deviceParamInfoID = addFault.deviceParamInfoID;
            oneFault.m_responseCommandID = addFault.responseCommandID;
            m_addFaults.push_back(oneFault);

            m_isAddFault = false;
        }
        else//修改
        {
            //修改界面  （name是不可以修改的！！！）
            ItemStruct editFault;
            editFault.faultType = m_faultType;
            editFault.Name = ui.le_errorName->text();
            editFault.deviceID = m_indexDevice[ui.cb_deciveType->currentIndex()];//TODO设备id，获取设备类型
            editFault.FaultCommandID = m_currentComandId;
            editFault.deviceParamInfoID = m_tempParamId;
            editFault.responseCommandID = m_tempCommandId;
            editFault.type = ui.cb_faultType->currentIndex() + 1;

            m_myFaultTree->EditToolItem(editFault);

            //修改
            for (int i = 0; i < m_faultItems.size(); i++)
            {
                if (m_faultItems[i].Name == editFault.Name)
                {
                    m_faultItems[i] = editFault;
                    break;
                }
            }

            //暂存下来，后续确定后写修改数据库
            AddOneFaultInfo oneFault;
            oneFault.faultType = m_faultType;
            oneFault.m_name = editFault.Name.toStdString();
            oneFault.m_Type = editFault.type;
            oneFault.m_FaultCommandCode = editFault.FaultCommandID;
            oneFault.m_deviceParamInfoID = editFault.deviceParamInfoID;
            oneFault.m_responseCommandID = editFault.responseCommandID;

            m_editFaults.push_back(oneFault);
        }
     
        IsEnable(false);
        m_isAddFault = false;
        ui.pb_edit->setText("编辑");
    }


    //滚动条设置
    int width = 0;
    int height = 0;
    width = 261;
    height = m_faultItems.size() * 40 + 80;
    m_scrollAreaFault->setGeometry(0, 0, 261, 786);
    ui.wgt_fualtList->setGeometry(QRect(0, 0, width, height));
    ui.wgt_fualtList->setMinimumSize(QSize(width, height));
}

/// <summary>
/// 添加故障有效性判断
/// </summary>
bool AddErrorCommand::AddFaultValidity()
{
    string faultName = ui.le_errorName->text().toStdString() + "指令";

    for (auto item: m_app->m_CommandInfoframes)
    {
        if (item.second->m_name == faultName)
        {
            QMessageBox::information(this, tr("提示"), "该故障名称已存在！");
            return false;
        }
    }
    return true;
}


/// <summary>
/// 参数类型选择响应
/// </summary>
/// <param name="tempName"></param>
void AddErrorCommand::onFaultTypeChanged(QString tempName)
{
    if (tempName == "参数故障")
    {
        ui.cb_deciveType->setVisible(true);
        ui.lb_device->setVisible(true);

        ShowParamsTable(0,ui.cb_deciveType->currentIndex());
    }
    else if (tempName == "指令故障")
    {
        ui.cb_deciveType->setVisible(false);
        ui.lb_device->setVisible(false);

        ShowParamsTable(1);
    }
    else
    {
        ui.cb_deciveType->setVisible(false);
        ui.lb_device->setVisible(false);
        ShowParamsTable(2);
    }
}

/// <summary>
/// 设备类型选择响应 刷新参数表格
/// </summary>
/// <param name="tempName"></param>
void AddErrorCommand::onDeciveTypeChanged(QString tempName)
{
    int deviceIdex = ui.cb_deciveType->currentIndex();

    ShowParamsTable(0, deviceIdex);
}


/// <summary>
/// 显示可选参数
/// </summary>
/// index 0：显示显示参数故障可选的参数；1：显示指令故障可选的参数;2：全显示.
/// <param name="index"></param>
/// 
void AddErrorCommand::ShowParamsTable(int index, int deviceIndex)
{
    ////刷新表格内容 
    int tableitem = 0;
    int temp = 0;
    bool isexit = false;
    bool isshow = false;
    vector<int> dParams;

    ui.tv_paramInfo->setMinimumSize(700, 500);
    ui.tv_paramInfo->verticalHeader()->hide(); //隐藏头

    QStandardItemModel*  m_tableModel = new QStandardItemModel(this);
    QStringList columnTitles;
    columnTitles << "序号" << "ID" << "名称" << "创建时间" << "最后更新时间" << "选择";
    m_tableModel->setHorizontalHeaderLabels(columnTitles);
    ui.tv_paramInfo->setModel(m_tableModel);

    if (index == 0 || index == 2)
    {
        //添加所有设备参数类型
        for (auto item : m_app->m_FaultParameterInfoFrames)
        {
            //判断是否进行设备筛选
            dParams = m_app->m_DeviceIDParamID[m_indexDevice[deviceIndex]];
            for (auto item2 : m_app->m_DeviceIDParamID)
            {
                if (std::find(dParams.begin(), dParams.end(), item.second->m_id) != dParams.end())
                {
                    isshow = true;
                    break;
                }
            }
            if (isshow == false) continue;

            isshow = false;
            isexit = false;
            tableitem = 0;
            m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString("%1").arg(temp)));
            m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString("%1").arg(item.second->m_id)));//表格第i行，第0列添加一项内容
            m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_name)));
            m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_createTime)));
            m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_lastUpdateTime)));

            m_tableModel->setItem(temp, tableitem++, new QStandardItem("选择"));
            ui.tv_paramInfo->setModel(m_tableModel);

            //添加处理控件
            m_chooseBtn = new QToolButton();
            ui.tv_paramInfo->setIndexWidget(m_tableModel->index(temp, 5), m_chooseBtn);

            //对之前已经绑定的参数进行标识
            for (auto itemd : m_app->m_FaultDeviceParamInfoFrames)
            {
                if ((itemd.second->m_deviceID == m_indexDevice[deviceIndex]) && (item.first == itemd.second->m_parameterID))
                {
                    for (int k = 0; k < m_tempParamId.size(); k++)
                    {
                        if (m_tempParamId[k] == itemd.second->m_id)
                        {
                            isexit = true;
                            break;
                        }
                    }
                    break;
                }
                if (isexit == true) break;
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
        //添加所有指令参数类型
        //TODO 尽量不要使用这种m_app->m_CommandInfoframes[j + 1]来访问，访问前判断是否存在，最后要判断是否遍历完。
        for (auto item : m_app->m_CommandInfoframes)
        {
            if (item.second->m_type != 1)
            {
                continue;
            }

            isexit = false;
            tableitem = 0;
            m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString("%1").arg(temp)));
            m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString("%1").arg(item.second->m_id)));//表格第i行，第0列添加一项内容
            m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_name)));
            m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_createTime)));
            m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_lastUpdateTime)));

            m_tableModel->setItem(temp, tableitem++, new QStandardItem("选择"));
            ui.tv_paramInfo->setModel(m_tableModel);

            //添加处理控件
            m_chooseBtn = new QToolButton();
            ui.tv_paramInfo->setIndexWidget(m_tableModel->index(temp, 5), m_chooseBtn);

            //对之前已经绑定的参数进行标识
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


    for (int i = 0; i <= tableitem; i++)
    {
        if (i == 0 || i == 1)
            ui.tv_paramInfo->setColumnWidth(i, 50);
        else if (i == tableitem - 1)
            ui.tv_paramInfo->setColumnWidth(i, 100);
        else
            ui.tv_paramInfo->setColumnWidth(i, 200);//设定表格第i列宽度setAlternatingRowColors(true);
    }
    //设置表格属性
    ui.tv_paramInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置表格属性只读，不能编辑
    ui.tv_paramInfo->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);        //表头信息显示居中
    ui.tv_paramInfo->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);  //设定表头列宽不可变
    ui.tv_paramInfo->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);  //设定列宽不可变
    ui.tv_paramInfo->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);//设定第3列表头弹性拉伸
    ui.tv_paramInfo->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选中时整行选中

    ui.tv_paramInfo->setAlternatingRowColors(true);//设置奇偶行交替颜色
    QHeaderView* verticalHeader = ui.tv_paramInfo->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(30);//设置高度(2)
    ui.tv_paramInfo->horizontalHeader()->setMinimumHeight(38);//设置表头高度
    ui.tv_paramInfo->horizontalHeader()->setStyleSheet("QHeaderView::section {color: black;padding-left: 4px;border: 1px solid #6c6c6c;}");

    ui.tv_paramInfo->setColumnHidden(1, true);
}

/// <summary>
/// 刷新参数表格
/// </summary>
/// <param name="tempName"></param>
void AddErrorCommand::FlashParamTable(QString tempName, int deviceid)
{
    m_tempCommandId.clear();
    m_tempParamId.clear();

    ui.tv_paramInfo->setMinimumSize(700, 500);
    ui.tv_paramInfo->verticalHeader()->hide(); //隐藏头

    QStandardItemModel* m_tableModel = new QStandardItemModel(this);
    QStringList columnTitles;
    columnTitles << "序号" << "ID" << "名称" << "创建时间" << "最后更新时间" << "选择";
    m_tableModel->setHorizontalHeaderLabels(columnTitles);
    ui.tv_paramInfo->setModel(m_tableModel);

    ////刷新表格内容 
    int tableitem = 0;
    for (int j = 0; j < m_faultItems.size(); j++)
    {
        if (tempName != m_faultItems[j].Name)
        {
            continue;
        }

        if (m_faultItems[j].type == 1)
        {
            //参数故障
            for (int i = 0; i < m_faultItems[j].deviceParamInfoID.size(); i++)
            {
                tableitem = 0;
                m_tableModel->setItem(i, tableitem++, new QStandardItem(QString("%1").arg(i)));
                m_tableModel->setItem(i, tableitem++, new QStandardItem(QString("%1").arg(m_faultItems[j].deviceParamInfoID[i])));//表格第i行，第0列添加一项内容

                bool isfind = false;
                for (auto item : m_app->m_FaultDeviceParamInfoFrames) ///m_app->m_FaultParameterInfoFrames
                {
                    if ((item.first == m_faultItems[j].deviceParamInfoID[i]))//TODO 20220118修改参数故障绑定的id相同  且设备相同 //&& (item.second->m_deviceID == deviceid)
                    {
                        //根据参数id显示参数信息
                        for (auto itemP : m_app->m_FaultParameterInfoFrames)
                        {
                            if (itemP.first == item.second->m_parameterID)
                            {
                                m_tableModel->setItem(i, tableitem++, new QStandardItem(QString::fromStdString(itemP.second->m_name)));
                                m_tableModel->setItem(i, tableitem++, new QStandardItem(QString::fromStdString(itemP.second->m_createTime)));
                                m_tableModel->setItem(i, tableitem++, new QStandardItem(QString::fromStdString(itemP.second->m_lastUpdateTime)));                  
                                m_tableModel->setItem(i, tableitem++, new QStandardItem("选择"));

                                m_tempParamId.push_back(item.first);//获取device_param_info id

                                isfind = true;
                                break;
                            }
                        }
                    }
                    if (isfind) break;
                }

                ui.tv_paramInfo->setModel(m_tableModel);

                //添加处理控件
                m_chooseBtn = new QToolButton();
                ui.tv_paramInfo->setIndexWidget(m_tableModel->index(i, 5), m_chooseBtn);
                m_chooseBtn->setStyleSheet("image:url(:/FaultInjection/images/CheckboxOn.png);border:0px;");
                connect(m_chooseBtn, &QPushButton::clicked, this, &AddErrorCommand::IsChooseParam);
            }
        }
        else
        {
            //指令故障
            for (int i = 0; i < m_faultItems[j].responseCommandID.size(); i++)
            {
                tableitem = 0;
                m_tableModel->setItem(i, tableitem++, new QStandardItem(QString("%1").arg(i)));
                m_tableModel->setItem(i, tableitem++, new QStandardItem(QString("%1").arg(m_faultItems[j].responseCommandID[i])));//表格第i行，第0列添加一项内容

                for (auto item : m_app->m_CommandInfoframes)
                {
                    if ((item.first == m_faultItems[j].responseCommandID[i]) && (item.second->m_type == 1))//指令类型：1表示测发指令、2表示测发回令、3表示故障指令
                    {
                        m_tableModel->setItem(i, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_name)));
                        m_tableModel->setItem(i, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_createTime)));
                        m_tableModel->setItem(i, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_lastUpdateTime)));

                        m_tempCommandId.push_back(item.first);
                        break;
                    }
                }

                m_tableModel->setItem(i, tableitem++, new QStandardItem("选择"));
                ui.tv_paramInfo->setModel(m_tableModel);

                //添加处理控件
                m_chooseBtn = new QToolButton();
                ui.tv_paramInfo->setIndexWidget(m_tableModel->index(i, 5), m_chooseBtn);
                m_chooseBtn->setStyleSheet("image:url(:/FaultInjection/images/CheckboxOn.png);border:0px;");
                connect(m_chooseBtn, &QPushButton::clicked, this, &AddErrorCommand::IsChooseParam);
            }
        }
    }

    for (int i = 0; i <= tableitem; i++)
    {
        if (i == 0 || i == 1)
            ui.tv_paramInfo->setColumnWidth(i, 50);
        else if (i == tableitem - 1)
            ui.tv_paramInfo->setColumnWidth(i, 100);
        else
            ui.tv_paramInfo->setColumnWidth(i, 200);//设定表格第i列宽度setAlternatingRowColors(true);
    }

     //设置表格属性
    ui.tv_paramInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置表格属性只读，不能编辑
    ui.tv_paramInfo->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);        //表头信息显示居中
    ui.tv_paramInfo->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);  //设定表头列宽不可变
    ui.tv_paramInfo->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);  //设定列宽不可变
    ui.tv_paramInfo->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);//设定第3列表头弹性拉伸
    ui.tv_paramInfo->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选中时整行选中

    ui.tv_paramInfo->setAlternatingRowColors(true);//设置奇偶行交替颜色
    QHeaderView* verticalHeader = ui.tv_paramInfo->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(30);//设置高度(2)
    ui.tv_paramInfo->horizontalHeader()->setMinimumHeight(38);//设置表头高度
    ui.tv_paramInfo->horizontalHeader()->setStyleSheet("QHeaderView::section {color: black;padding-left: 4px;border: 1px solid #6c6c6c;}");

    ui.tv_paramInfo->setColumnHidden(1, true);
}


/// <summary>
/// 参数选择或者取消选择
/// </summary>
void AddErrorCommand::IsChooseParam()
{
    if (ui.pb_edit->text() == "编辑")
    {
        QMessageBox::information(this, tr("提示"), "请进入编辑模式！");
        return;
    }

    bool isChoose = true;
    QModelIndex index;
    QToolButton* senderObj = qobject_cast<QToolButton*>(sender());
    if (senderObj == nullptr)
    {
        return;
    }

    QModelIndex idx = ui.tv_paramInfo->indexAt(QPoint(senderObj->frameGeometry().x(), senderObj->frameGeometry().y()));
    int row = idx.row();

    //获取当前参数的id
    QAbstractItemModel* model = ui.tv_paramInfo->model();
    index = model->index(row, 1);
    int paramId = model->data(index).toInt();//注意参数故障和指令故障绑定的影响id表不一样

    vector<int>* tempIDs = nullptr;
    if (ui.cb_faultType->currentIndex() == 0)
    {
        tempIDs = &m_tempParamId;//参数故障

        //（当时参数指令的时候，指令和）根据选择的设备和参数来共同确定 device_param_info
        int deviceid = m_indexDevice[ui.cb_deciveType->currentIndex()];
        for (auto item: m_app->m_FaultDeviceParamInfoFrames)
        {
            if ((item.second->m_deviceID == deviceid) && (item.second->m_parameterID == paramId))
            {
                paramId = item.second->m_id;//获取device_param_info id
                break;
            }
        }
    }
    else
    {
        tempIDs = &m_tempCommandId;//指令故障
    }

    for (vector<int>::iterator iter = tempIDs->begin(); iter != tempIDs->end(); iter++)
    {
        if (*iter == paramId)
        {
            isChoose = false;
            break;
        }
    }

    if (isChoose)
    {
        tempIDs->push_back(paramId);
        senderObj->setStyleSheet("image:url(:/FaultInjection/images/CheckboxOn.png);border:0px;");
    }
    else
    {
        //从vector中删除指定元素 
        for (vector<int>::iterator iter = tempIDs->begin(); iter != tempIDs->end(); iter++)
        {
            if (*iter == paramId) {
                tempIDs->erase(iter);
                break;
            }
        }
        senderObj->setStyleSheet("image:url(:/FaultInjection/images/CheckboxOff.png);border:0px;");
    }
}


/// <summary>
/// 使能控制
/// </summary>
/// <param name="isOk"></param>
void AddErrorCommand::IsEnable(bool isOk)
{
    ui.cb_faultType->setEnabled(isOk);
    ui.cb_deciveType->setEnabled(isOk);

    //除了添加故障的时候，其他不可以修改故障类型
    if (m_isAddFault == false)
    {
        ui.cb_faultType->setEnabled(false);
    }
    else
    {
        ui.cb_faultType->setEnabled(true);
    }
    
}

/// <summary>
/// 确定编辑响应函数
/// </summary>
void AddErrorCommand::AddErrorOK()
{
    //数据入库，界面添加故障按钮
    if (ui.pb_edit->text() == "完成")
    {
        QMessageBox::information(this, tr("提示"), "完成当前故障编辑！");
        return;
    }

    emit AddFaultItem(m_addFaults);

    emit EditFaultItem(m_editFaults);

    emit DelFaultItem(m_delFaults);
    this->hide();
}

void AddErrorCommand::AddErrorCanCel()
{
    this->hide();
}



/// <summary>
/// 边框设置
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
/// 按下鼠标
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
/// 移动鼠标
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
/// 释放鼠标
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