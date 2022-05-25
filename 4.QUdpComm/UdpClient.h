#pragma once
#include <QtWidgets>
#include <QtNetwork>
#include <QTimer>

class UdpClient
	:public QWidget
{
	Q_OBJECT

public:
	UdpClient();
	~UdpClient() {
		close();
	}

private:
	QUdpSocket* udpsocket;

	QLabel* address_label;
	QLabel* port_label;
	QLineEdit* address_input;
	QLineEdit* port_input;
	QPushButton* connect_btn;
	QPushButton* send_btn;
	QGridLayout* window_layout;
	QLabel* recv_label;
	QLabel* send_label;
	QPlainTextEdit* recv_text;
	QPlainTextEdit* send_text;

	QTimer* timer;
private:
	void connect_slot();
	void send_slot();
	void recv_slot();
};

