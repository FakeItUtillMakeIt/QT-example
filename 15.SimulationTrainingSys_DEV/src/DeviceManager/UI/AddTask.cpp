#pragma execution_character_set("utf-8")
#include "AddTask.h"
#include <QtWidgets/qmessagebox.h>



AddTask::AddTask(string rocktName,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    m_app = AppCache::instance();

    setAttribute(Qt::WA_ShowModal, true);
    setWindowFlags(Qt::FramelessWindowHint);//ȥ��������
    setAttribute(Qt::WA_TranslucentBackground, false);

    m_rocktName = QString::fromLocal8Bit(rocktName.data());
    Init();
}

/// <summary>
/// ��ʼ��
/// </summary>
void AddTask::Init()
{
    ui.le_roketName->setText(m_rocktName);

    connect(ui.pb_close_2, &QPushButton::clicked, this, &AddTask::IsCancel);
    connect(ui.pb_cancel_2, &QPushButton::clicked, this, &AddTask::IsCancel); 
    connect(ui.pb_ok_2, &QPushButton::clicked, this, &AddTask::IsOK);
}

AddTask::~AddTask()
{

}

void AddTask::IsOK()
{
    TaskInfo oneTask;
    oneTask.jobName = ui.le_taskName->text();
    oneTask.softName = ui.le_roketSoftName->text();

    if (!ValidityCheck(oneTask)) return;

    emit addTask(oneTask);
    //this->close();
}

/// <summary>
/// ��Ч�Լ��
/// </summary>
/// <param name="softName"></param>
/// <returns></returns>
bool AddTask::ValidityCheck(TaskInfo oneTask)
{
    if (oneTask.jobName == "" || oneTask.softName == "")
    {
        QMessageBox::warning(this, QString("��ʾ"), "���Ʋ���Ϊ��", "ȷ��");
        return false;
    }

    for (auto item : m_app->m_TaskManageInfoFrames)
    {
        if (oneTask.softName.toStdString() == item.second->m_roketSoftName)
        {
            QMessageBox::information(nullptr, "��ʾ", "�ø�λ����Ѵ���", "ȷ��");
            return false;
        }
    }

    return true;
}


void AddTask::IsCancel()
{
    this->close();
}


/// <summary>
/// �߿�����
/// </summary>
/// <param name="event"></param>
void AddTask::paintEvent(QPaintEvent* event)
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
void AddTask::mousePressEvent(QMouseEvent* event)
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
void AddTask::mouseMoveEvent(QMouseEvent* event)
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
void AddTask::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        mMoving = false;
        setCursor(Qt::ArrowCursor);
    }
}