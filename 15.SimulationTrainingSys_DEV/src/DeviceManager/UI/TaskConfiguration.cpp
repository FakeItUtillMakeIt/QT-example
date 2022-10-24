#pragma execution_character_set("utf-8")
#include "TaskConfiguration.h"

TaskConfiguration::TaskConfiguration(QWidget *parent)
	: QWidget(parent)
    , addTask(nullptr)
    , taskDataDAO(nullptr)
    , m_tableModel(nullptr)
    , m_isAdd(false)
{
	ui.setupUi(this);
    m_app = AppCache::instance();

    ui.widget_2->hide();
	Init();
}

TaskConfiguration::~TaskConfiguration()
{


}

void TaskConfiguration::Init()
{
    taskDataDAO = new RocketDataDAO(m_app->m_outputPath);

    //设置搜索框
    ui.pb_searchButton_2->setCursor(Qt::PointingHandCursor);
    ui.pb_searchButton_2->setFixedSize(30, 30);
    
    QMargins margins = ui.le_searchedit_2->textMargins();//防止文本框输入内容位于按钮之下
    ui.le_searchedit_2->setPlaceholderText("搜索");
    
    QHBoxLayout* pSearchLayout = new QHBoxLayout();
    pSearchLayout->addStretch();
    pSearchLayout->addWidget(ui.pb_searchButton_2);
    pSearchLayout->setSpacing(0);
    pSearchLayout->setContentsMargins(8, 0, 10, 0);
    ui.le_searchedit_2->setTextMargins(13 + 8 + 2, 0, 0, 0);
    ui.le_searchedit_2->setContentsMargins(0, 0, 0, 0);
    ui.le_searchedit_2->setLayout(pSearchLayout);
    ui.le_searchedit_2->setStyleSheet("height:29px;border:1px solid #eaeaea;background-color: rgb(255, 255, 255);border-radius:14px;");
    //connect(ui.le_searchedit_2, &QPushButton::clicked, this, &TaskConfiguration::SearchSoft);
    connect(ui.le_searchedit_2, &QLineEdit::textEdited, this, &TaskConfiguration::SearchSoft);

    //表格初始化
    TaskInit(false);

    
    connect(ui.pb_addTask,&QPushButton::clicked,this,&TaskConfiguration::AddTaskShow);

}


/// <summary>
/// 表格初始化
/// </summary>
void TaskConfiguration::TaskInit(bool isSeach)
{
    ////刷新表格内容 
    int tableitem = 5;
    
    vector<int> dParams;

    ui.tv_taskView->setMinimumSize(700, 500);
    ui.tv_taskView->verticalHeader()->hide(); //隐藏头

    if (m_tableModel != nullptr)
    {
        delete m_tableModel;
        m_tableModel = nullptr;

    }
    m_tableModel = new QStandardItemModel(this);
    QStringList columnTitles;
    columnTitles << "序号" << "所属型号" << "任务岗位" << "任务软件" << "创建时间" << "操作";
    m_tableModel->setHorizontalHeaderLabels(columnTitles);
    ui.tv_taskView->setModel(m_tableModel);

    for (int i = 0; i <= tableitem; i++)
    {
        if (i == 0)
            ui.tv_taskView->setColumnWidth(i, 100);
        else if (i == tableitem || i == 1)
            ui.tv_taskView->setColumnWidth(i, 150);
        else
            ui.tv_taskView->setColumnWidth(i, 350);//设定表格第i列宽度setAlternatingRowColors(true);
    }

    //设置表格属性
    ui.tv_taskView->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置表格属性只读，不能编辑
    ui.tv_taskView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);        //表头信息显示居中
    ui.tv_taskView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);  //设定表头列宽不可变
    ui.tv_taskView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);  //设定列宽不可变
    ui.tv_taskView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);//设定第3列表头弹性拉伸
    ui.tv_taskView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);//设定第2列表头弹性拉伸
    ui.tv_taskView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);//设定第3列表头弹性拉伸
    ui.tv_taskView->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选中时整行选中
    ui.tv_taskView->setStyleSheet("selection-background-color:rgb(85, 170, 255)");//选中某行的颜色	
    ui.tv_taskView->setAlternatingRowColors(true);//设置奇偶行交替颜色
    QHeaderView* verticalHeader = ui.tv_taskView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(30);//设置高度(2)
    ui.tv_taskView->horizontalHeader()->setMinimumHeight(38);//设置表头高度
    ui.tv_taskView->horizontalHeader()->setStyleSheet("QHeaderView::section {color: black;padding-left: 4px;border: 1px solid #6c6c6c;font: 12pt 微软雅黑;}");
    //ui.tv_taskView->update();
    //ui.tv_taskView->setColumnHidden(1, true);//隐藏某行

    if (isSeach)
    {
        FalshSeachTableView();
    }
    else
    {
        FalshTableView();
    }
    
}


/// <summary>
/// 刷新表格
/// </summary>
void TaskConfiguration::FalshTableView()
{
    int temp = 0;
    int tableitem = 0;
    //刷新表格
    for (auto item : m_app->m_TaskManageInfoFrames)
    {
        tableitem = 0;
        m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString("%1").arg(temp+1)));
        m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromLocal8Bit(m_app->m_CurrentRocketType->m_name.c_str())));//表格第i行，第0列添加一项内容
        m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_taskName)));
        m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_roketSoftName)));
        m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_createTime)));

        m_tableModel->setItem(temp, tableitem++, new QStandardItem("删除"));
        
        for (size_t i = 0; i < tableitem; i++)
        {
            m_tableModel->item(temp, i)->setTextAlignment(Qt::AlignCenter);//居中
        }
        ui.tv_taskView->setModel(m_tableModel);

        //添加处理控件
        m_deleteBtn = new QToolButton();
        m_deleteBtn->setText("删除");
        m_deleteBtn->setStyleSheet("border-image: url(:/FaultInjection/images/bt_normal.png);");
        connect(m_deleteBtn, &QToolButton::clicked,this,&TaskConfiguration::DeleteOneTask);
        ui.tv_taskView->setIndexWidget(m_tableModel->index(temp, 5), m_deleteBtn);
       
        temp++;
    }

    if (m_isAdd)
    {
        ui.tv_taskView->selectRow(temp - 1);//选择！！！
        m_isAdd = false;
    }
}

/// <summary>
/// 展示添加界面
/// </summary>
void TaskConfiguration::AddTaskShow()
{
    if (addTask != nullptr)
    {
        delete addTask;
        addTask = nullptr;
    }

    addTask = new AddTask(m_app->m_CurrentRocketType->m_name);
    connect(addTask, &AddTask::addTask, this, &TaskConfiguration::AddTaskFun);
    addTask->show();

    //addTask->setWindowFlags(addTask->windowFlags() | Qt::WindowStaysOnTopHint);//至于最顶部
}

/// <summary>
/// 添加任务岗位
/// </summary>
/// <param name="oneTask"></param>
void TaskConfiguration::AddTaskFun(TaskInfo oneTask)
{
    //写入数据库并刷新表格
    TaskManageInfo* taskInfo = new TaskManageInfo();

    taskInfo->m_rocketId = m_app->m_CurrentRocketType->m_id;
    taskInfo->m_taskName = oneTask.jobName.toStdString();
    taskInfo->m_roketSoftName = oneTask.softName.toStdString();

    if (taskDataDAO->InsertTaskInfo(taskInfo))
    {
        m_isAdd = true;
        TaskInit(false);
        //FalshTableView();
        QMessageBox::information(nullptr,"信息","岗位添加成功！");
    }
    else
    {
        QMessageBox::information(nullptr,"警告","写入数据库失败！","确定");
        return;
    }
}

/// <summary>
/// 删除岗位
/// </summary>
void TaskConfiguration::DeleteOneTask()
{

    QModelIndex index;
    QToolButton* senderObj = qobject_cast<QToolButton*>(sender());
    if (senderObj == nullptr)
    {
        return;
    }

    QModelIndex idx = ui.tv_taskView->indexAt(QPoint(senderObj->frameGeometry().x(), senderObj->frameGeometry().y()));
    int row = idx.row();

    //获取当前参数的id
    QAbstractItemModel* model = ui.tv_taskView->model();
    index = model->index(row, 3);
    string taskSoftName = model->data(index).toString().toStdString();//注意参数故障和指令故障绑定的影响id表不一样

    QString qstr = QString("确认删除：%1 吗？").arg(QString::fromStdString(taskSoftName));// "确认删除：" + ui.le_errorName->text() + "吗？";
    QMessageBox msgBoxWarning;
    msgBoxWarning.setText(qstr);
    msgBoxWarning.setWindowTitle(QString("警告"));
    msgBoxWarning.setWindowIcon(this->windowIcon());
    msgBoxWarning.setIcon(QMessageBox::Warning);
    msgBoxWarning.addButton(QString("取消"), QMessageBox::RejectRole);
    msgBoxWarning.addButton(QString("确定"), QMessageBox::AcceptRole);
    int ret = msgBoxWarning.exec();
    if (ret == 0)
    {
        return;
    }

    TaskManageInfo* deleteInfo = new TaskManageInfo();
    for (auto item : m_app->m_TaskManageInfoFrames)
    {
        if (taskSoftName == item.second->m_roketSoftName)
        {
            deleteInfo = item.second;
            break;
        }
    }
    
    if (false == taskDataDAO->DeleteTaskInfo(deleteInfo))
    {
        QMessageBox::information(nullptr, "警告", "删除数据库失败！", "确定");
        return;
    }
    
    TaskInit(false);
}

/// <summary>
/// 根据软件名称查找
/// </summary>
/// <param name="softName"></param>
void TaskConfiguration::SearchSoft(QString softName)
{
    if (softName.isEmpty())
    {
        TaskInit(false);
        return;
    }

   
    m_TaskManageSearchFrames.clear();
    for (auto item : m_app->m_TaskManageInfoFrames)
    {
        if (QString::fromStdString(item.second->m_roketSoftName).contains(softName))
        {
            //m_app->m_TaskManageInfoFrames.insert(pair<int, TaskManageInfo*>(item.first, item.second));
            m_TaskManageSearchFrames.insert(item);
        }
    }

    //展示筛选后的数据
    TaskInit(true);
   

   
    //int a = m_tableModel->rowCount();
    //for (size_t i = 0; i <= m_tableModel->rowCount(); i++)
    //{
    //    m_tableModel->removeRow(i);
    //}
    //ui.tv_taskView->setModel(m_tableModel);
}

/// <summary>
/// 查找刷新表格
/// </summary>
void TaskConfiguration::FalshSeachTableView()
{
    int temp = 0;
    int tableitem = 0;
    //刷新表格
    for (auto item : m_TaskManageSearchFrames)
    {
        tableitem = 0;
        m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString("%1").arg(temp + 1)));
        m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromLocal8Bit(m_app->m_CurrentRocketType->m_name.c_str())));//表格第i行，第0列添加一项内容
        m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_taskName)));
        m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_roketSoftName)));
        m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_createTime)));

        m_tableModel->setItem(temp, tableitem++, new QStandardItem("删除"));

        for (size_t i = 0; i < tableitem; i++)
        {
            m_tableModel->item(temp, i)->setTextAlignment(Qt::AlignCenter);//居中
        }
        ui.tv_taskView->setModel(m_tableModel);

        //添加处理控件
        m_deleteBtn = new QToolButton();
        m_deleteBtn->setText("删除");
        m_deleteBtn->setStyleSheet("border-image: url(:/FaultInjection/images/bt_normal.png);");
        connect(m_deleteBtn, &QToolButton::clicked, this, &TaskConfiguration::DeleteOneTask);
        ui.tv_taskView->setIndexWidget(m_tableModel->index(temp, 5), m_deleteBtn);

        temp++;
    }

}