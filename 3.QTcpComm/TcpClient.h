#pragma once
#include <QtWidgets>
#include <QTcpSocket>
#include <QGridLayout>
#include <QHostAddress>
#include <QTimer>

class TcpClient :
    public QWidget
{
public:
    TcpClient();
	~TcpClient();


private:
	QTcpSocket* client_socket;
	
	QLabel* client_address_l;
	QLabel* client_port_l;

	QLineEdit* ip_address;
	QLineEdit* ip_port;

	QLabel* in_label;
	QLabel* out_label;
	QPlainTextEdit* in_text;
	QPlainTextEdit* out_text;
	QPushButton* connect_btn;
	QPushButton* send_btn;

	QTimer* timer;

private slots:
	void connect_slot();
	void send_slot();
	void client_readdata_slot();
};

