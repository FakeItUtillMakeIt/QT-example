#include "SearchFault.h"

SearchFault::SearchFault(QWidget *parent)
	: QWidget(parent)
    , m_scrollSearchFault(nullptr)
{
	ui.setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    setAttribute(Qt::WA_TranslucentBackground, false);

    Init();
}

SearchFault::~SearchFault()
{
   
}


void SearchFault::Init()
{
    //根据数据初始化窗体-父参数列表
    m_scrollSearchFault = new QScrollArea(ui.wgt_ff);
    m_scrollSearchFault->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollSearchFault->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollSearchFault->setWidgetResizable(true);
    m_scrollSearchFault->setStyleSheet("background-color:transparent;border:0px;");
    m_scrollSearchFault->setFrameShape(QFrame::NoFrame);
    m_scrollSearchFault->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏横向滚动条
    //设置滚动区域的窗体-父参数列表
    m_scrollSearchFault->setWidget(ui.wgt_f);


    //搜索树状显示列表
    m_searchLayout = new QHBoxLayout;
    m_searchTree = new MyFaultTree();
    //connect(m_searchTree, &MyFaultTree::onFaultNodeClick, this, &SearchFault::OnSearchFaultNode);
    m_searchLayout->addWidget(m_searchTree);
    ui.wgt_f->setLayout(m_searchLayout);
}


void SearchFault::ShowSearchFaults(vector<ItemStruct> Items)
{
    QString str = "";
    QString indexStr = "";
    m_searchTree->InitFaultTree(Items);

    //滚动条设置
    int width = 0;
    int height = 0;
    width = 261;
    height = Items.size() * 40 + 80;
    m_scrollSearchFault->setGeometry(0, 0, 261, 680);
    ui.wgt_f->setGeometry(QRect(0, 0, width, height));
    ui.wgt_f->setMinimumSize(QSize(width, height));

}


/// <summary>
/// 边框设置
/// </summary>
/// <param name="event"></param>
void SearchFault::paintEvent(QPaintEvent* event)
{
    int border_width = 1;
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
void SearchFault::mousePressEvent(QMouseEvent* event)
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
void SearchFault::mouseMoveEvent(QMouseEvent* event)
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
void SearchFault::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        mMoving = false;
        setCursor(Qt::ArrowCursor);
    }
}