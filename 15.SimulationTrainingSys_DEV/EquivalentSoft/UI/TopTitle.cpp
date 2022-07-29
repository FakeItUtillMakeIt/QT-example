#include "TopTitle.h" 
TopTitle::TopTitle(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    m_app = AppCache::instance();
    ui.lb_title->setText(m_app->m_soft->GetName());
    Init();
} 

void TopTitle::Init()
{
    connect(ui.pb_close, &QPushButton::clicked, this, [this]() {
        emit CloseWindow();
    });
    connect(ui.pb_min, &QPushButton::clicked, this, [this]() {
        emit ShowMinimized();
    });
}