#pragma once
#include <QtWidgets>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QGridLayout>
#include <QTimer>

class TcpServer :
    public QWidget
{

private:
    
    static TcpServer* instance;
    QTimer* timer;

public:
	TcpServer();
    ~TcpServer();

	static TcpServer* getInstance() {
		if (instance == nullptr)
		{
			instance = new TcpServer();
		}

		return instance;
	}
		
	
private:
    QTcpServer* server_;
    QTcpSocket* server_socket;

    QLabel* server_ip_l;
    QLabel* server_port_l;

    QLineEdit* server_ip_set;
    QLineEdit* server_port_set;

    QPushButton* server_listen_btn;

    QLabel* server_recv_l;
    QLabel* server_send_l;
    QPlainTextEdit* server_recv_text;
    QPlainTextEdit* server_send_text;

    QPushButton* server_send_btn;

private slots:
    void server_listen_slot();
    void server_send_slot();
    void server_connect_success_slot();
    void server_readdata_slot();

};

