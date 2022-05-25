#include "TcpClient.h"

TcpClient::TcpClient() {
	this->setAttribute(Qt::WA_DeleteOnClose);

	client_socket = new QTcpSocket(this);

	client_address_l = new QLabel(QString::fromLocal8Bit("服务器地址:"));
	client_port_l = new QLabel(QString::fromLocal8Bit("服务器端口:"));

	ip_address = new QLineEdit(QString("127.0.0.1"));
	ip_port = new QLineEdit(QString("12345"));
	in_label = new QLabel(QString::fromLocal8Bit("接收消息"));
	out_label = new QLabel(QString::fromLocal8Bit("发送消息"));

	in_text = new QPlainTextEdit();
	out_text = new QPlainTextEdit();

	connect_btn = new QPushButton(QString::fromLocal8Bit("连接服务器"));
	send_btn = new QPushButton(QString::fromLocal8Bit("发送"));
	send_btn->setEnabled(false);

	QGridLayout* client_layout = new QGridLayout(this);
	client_layout->addWidget(client_address_l, 0, 0);
	client_layout->addWidget(ip_address, 0, 1);
	client_layout->addWidget(client_port_l, 1, 0);
	client_layout->addWidget(ip_port, 1, 1);
	client_layout->addWidget(connect_btn, 2, 1);
	client_layout->addWidget(in_label, 3, 0);
	client_layout->addWidget(in_text, 4, 0, 2, 2);
	client_layout->addWidget(out_label, 6, 0);
	client_layout->addWidget(out_text, 7, 0, 2, 2);
	client_layout->addWidget(send_btn, 9, 1);

	connect(connect_btn, &QPushButton::clicked, this, &TcpClient::connect_slot);
	connect(send_btn, &QPushButton::clicked, this, &TcpClient::send_slot);
	connect(client_socket, &QTcpSocket::readyRead, this, &TcpClient::client_readdata_slot);
	connect(client_socket, &QTcpSocket::connected, this, [this]() {
		send_btn->setEnabled(true);
		connect_btn->setEnabled(false);
		ip_address->setEnabled(false);
		ip_port->setEnabled(false);
		});

	connect(client_socket, &QTcpSocket::disconnected, this, [this]() {
		
			ip_address->setEnabled(true);
			ip_port->setEnabled(true);
			connect_btn->setEnabled(true);
			send_btn->setEnabled(false);
		
		});

	connect(this, &TcpClient::destroyed, this, [this]() {
		client_socket->disconnectFromHost();
		});
	timer = new QTimer(this);


}

TcpClient::~TcpClient() {
	client_socket->write(QByteArray("close connect"));
	/*disconnect(client_socket, &QTcpSocket::disconnected, 0, 0);
	client_socket->disconnectFromHost();
	client_socket->close();*/
	client_socket->deleteLater();
	destroy(true);

}



void TcpClient::connect_slot() {
	QHostAddress hostaddress(ip_address->text());
	quint16 port = ip_port->text().toInt();
	
	client_socket->connectToHost(hostaddress, port);
	timer->start(10000);
}

void TcpClient::send_slot() {
	QByteArray senddata; 
	senddata.append(out_text->toPlainText());
	client_socket->write(senddata);

}

void TcpClient::client_readdata_slot() {

	QByteArray alldata = client_socket->readAll();
	QString recvdata = static_cast<QString>(alldata);
	in_text->appendPlainText(recvdata);
	
}