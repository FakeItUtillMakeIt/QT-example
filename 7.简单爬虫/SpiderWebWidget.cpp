#include "SpiderWebWidget.h"

SpiderWebWidget::SpiderWebWidget(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    windowLayout = new QGridLayout(this);
    webLabel = new QLabel();
    windowLayout->addWidget(webLabel, 0, 0);
    webUrlInput = new QLineEdit();
    windowLayout->addWidget(webUrlInput, 0, 1);
    webClickBtn = new QPushButton();
    windowLayout->addWidget(webClickBtn, 1, 1);
    webRetInfo = new QPlainTextEdit();
    webRetInfo->setReadOnly(true);
    windowLayout->addWidget(webRetInfo, 2, 0, 2, 2);

    webLabel->setText(QString::fromLocal8Bit("输入爬取网站:"));
    webClickBtn->setText(QString::fromLocal8Bit("开始爬取"));

    this->centralWidget()->setLayout(windowLayout);

    connect(webClickBtn, &QPushButton::clicked, this, &SpiderWebWidget::click_slot);

}


void SpiderWebWidget::click_slot() {

    QString q_url = webUrlInput->text();
    spider = new SpiderWebScript;
    
    spider->excuteScript(q_url.toStdString());
    webRetInfo->clear();
    webRetInfo->appendHtml(QString::fromStdString(spider->_response));
}