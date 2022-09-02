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

	Init();
}

TaskConfiguration::~TaskConfiguration()
{


}

void TaskConfiguration::Init()
{
    taskDataDAO = new RocketDataDAO(m_app->m_outputPath);

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
    //connect(ui.pb_searchButton, &QPushButton::clicked, this, &AddErrorCommand::SearchFaults);

    //����ʼ��
    TaskInit();


    connect(ui.pb_addTask,&QPushButton::clicked,this,&TaskConfiguration::AddTaskShow);

}


/// <summary>
/// ����ʼ��
/// </summary>
void TaskConfiguration::TaskInit()
{
    ////ˢ�±������ 
    int tableitem = 5;
    
    vector<int> dParams;

    ui.tv_taskView->setMinimumSize(700, 500);
    ui.tv_taskView->verticalHeader()->hide(); //����ͷ

    if (m_tableModel != nullptr)
    {
        delete m_tableModel;
        m_tableModel = nullptr;

    }
    m_tableModel = new QStandardItemModel(this);
    QStringList columnTitles;
    columnTitles << "���" << "�����ͺ�" << "�����λ" << "�������" << "����ʱ��" << "����";
    m_tableModel->setHorizontalHeaderLabels(columnTitles);
    ui.tv_taskView->setModel(m_tableModel);


    for (int i = 0; i <= tableitem; i++)
    {
        if (i == 0)
            ui.tv_taskView->setColumnWidth(i, 100);
        else if (i == tableitem || i == 1)
            ui.tv_taskView->setColumnWidth(i, 150);
        else
            ui.tv_taskView->setColumnWidth(i, 350);//�趨����i�п��setAlternatingRowColors(true);
    }

    //���ñ������
    ui.tv_taskView->setEditTriggers(QAbstractItemView::NoEditTriggers);  //���ñ������ֻ�������ܱ༭
    ui.tv_taskView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);        //��ͷ��Ϣ��ʾ����
    ui.tv_taskView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);  //�趨��ͷ�п��ɱ�
    ui.tv_taskView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);  //�趨�п��ɱ�
    ui.tv_taskView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);//�趨��3�б�ͷ��������
    ui.tv_taskView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);//�趨��2�б�ͷ��������
    ui.tv_taskView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);//�趨��3�б�ͷ��������
    ui.tv_taskView->setSelectionBehavior(QAbstractItemView::SelectRows); //����ѡ��ʱ����ѡ��
    ui.tv_taskView->setStyleSheet("selection-background-color:rgb(85, 170, 255)");//ѡ��ĳ�е���ɫ	
    ui.tv_taskView->setAlternatingRowColors(true);//������ż�н�����ɫ
    QHeaderView* verticalHeader = ui.tv_taskView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(30);//���ø߶�(2)
    ui.tv_taskView->horizontalHeader()->setMinimumHeight(38);//���ñ�ͷ�߶�
    ui.tv_taskView->horizontalHeader()->setStyleSheet("QHeaderView::section {color: black;padding-left: 4px;border: 1px solid #6c6c6c;}");

    //ui.tv_taskView->setColumnHidden(1, true);//����ĳ��

    FalshTableView();
}


/// <summary>
/// ˢ�±��
/// </summary>
void TaskConfiguration::FalshTableView()
{
    int temp = 0;
    int tableitem = 0;
    //ˢ�±��
    for (auto item : m_app->m_TaskManageInfoFrames)
    {
        tableitem = 0;
        m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString("%1").arg(temp+1)));
        m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromLocal8Bit(m_app->m_CurrentRocketType->m_name.c_str())));//����i�У���0�����һ������
        m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_taskName)));
        m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_roketSoftName)));
        m_tableModel->setItem(temp, tableitem++, new QStandardItem(QString::fromStdString(item.second->m_createTime)));

        m_tableModel->setItem(temp, tableitem++, new QStandardItem("ɾ��"));
        ui.tv_taskView->setModel(m_tableModel);

        //��Ӵ���ؼ�
        m_deleteBtn = new QToolButton();
        m_deleteBtn->setText("ɾ��");
        m_deleteBtn->setStyleSheet("border-image: url(:/FaultInjection/images/bt_normal.png);");
        connect(m_deleteBtn, &QToolButton::clicked,this,&TaskConfiguration::DeleteOneTask);
        ui.tv_taskView->setIndexWidget(m_tableModel->index(temp, 5), m_deleteBtn);

        temp++;
    }

    if (m_isAdd)
    {
        ui.tv_taskView->selectRow(temp - 1);//ѡ����û��Ч��������
        m_isAdd = false;
    }
    

}

/// <summary>
/// չʾ��ӽ���
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

    //addTask->setWindowFlags(addTask->windowFlags() | Qt::WindowStaysOnTopHint);//�������
}

/// <summary>
/// ��������λ
/// </summary>
/// <param name="oneTask"></param>
void TaskConfiguration::AddTaskFun(TaskInfo oneTask)
{
    //д�����ݿⲢˢ�±��
    TaskManageInfo* taskInfo = new TaskManageInfo();

    taskInfo->m_rocketId = m_app->m_CurrentRocketType->m_id;
    taskInfo->m_taskName = oneTask.jobName.toStdString();
    taskInfo->m_roketSoftName = oneTask.softName.toStdString();

    if (taskDataDAO->InsertTaskInfo(taskInfo))
    {
        m_isAdd = true;
        TaskInit();
        //FalshTableView();
        QMessageBox::information(nullptr,"��Ϣ","��λ��ӳɹ���");
    }
    else
    {
        QMessageBox::information(nullptr,"����","д�����ݿ�ʧ�ܣ�","ȷ��");
        return;
    }
}

/// <summary>
/// ɾ����λ
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

    //��ȡ��ǰ������id
    QAbstractItemModel* model = ui.tv_taskView->model();
    index = model->index(row, 3);
    string taskSoftName = model->data(index).toString().toStdString();//ע��������Ϻ�ָ����ϰ󶨵�Ӱ��id��һ��

    QString qstr = QString("ȷ��ɾ����%1 ��").arg(QString::fromStdString(taskSoftName));// "ȷ��ɾ����" + ui.le_errorName->text() + "��";
    int ret = QMessageBox::warning(this, QString("����"), qstr, "ȡ��", "ȷ��");
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
        QMessageBox::information(nullptr, "����", "ɾ�����ݿ�ʧ�ܣ�", "ȷ��");
        return;
    }
    
    TaskInit();
}
