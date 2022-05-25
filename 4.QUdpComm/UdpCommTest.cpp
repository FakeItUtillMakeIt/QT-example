#include "UdpCommTest.h"


UdpCommTest::UdpCommTest(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.commandLinkButton, &QCommandLinkButton::clicked, this, &UdpCommTest::btn_OK_slot);
}


void UdpCommTest::btn_OK_slot() {
    if (ui.comboBox->currentIndex()==0)
    {
        UdpServer* udpserver =  UdpServer::get_instance();
        udpserver->show();
    }
    else if (ui.comboBox->currentIndex()==1)
    {
        UdpClient* udpclient = new UdpClient;
        udpclient->show();
    }
}