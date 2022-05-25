#include "TcpCommTest.h"


TcpCommTest::TcpCommTest(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_thread = new QThread(this);

    connect(ui.commandLinkButton, SIGNAL(clicked()), this, SLOT(btnOk_slot()));

}


void TcpCommTest::btnOk_slot() {
    auto current_index = ui.comboBox->currentIndex();
    auto current_text = ui.comboBox->currentText();

    if (current_index==0)
    {
		server_window = new TcpServer();
        //TcpServer* server_window= TcpServer::getInstance();
		server_window->resize(640, 480);
		QPixmap pixmap(":/resource/ws2.jpg");
		server_window->setMask(pixmap.mask());
		server_window->show();
        //server_window->server_socket->moveToThread(m_thread);
    }
    else
    {
        client_window = new TcpClient();
        client_window->resize(640, 480);
        client_window->show();
        //client_window->moveToThread(m_thread);
    }

}

