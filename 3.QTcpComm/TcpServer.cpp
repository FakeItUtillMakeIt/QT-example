#include "TcpServer.h"

TcpServer* TcpServer::instance = nullptr;

TcpServer::TcpServer() {
	this->setAttribute(Qt::WA_DeleteOnClose);

	server_ = new QTcpServer(this);

	server_ip_l=new QLabel(QString::fromLocal8Bit("服务器地址设置:"));
	server_port_l=new QLabel(QString::fromLocal8Bit("服务器端口设置:"));

	server_ip_set = new QLineEdit(QString("127.0.0.1"));
	server_port_set = new QLineEdit(QString("12345"));

	server_listen_btn = new QPushButton(QString::fromLocal8Bit("开始监听"));

	server_recv_l = new QLabel(QString::fromLocal8Bit("接收消息"));
	server_recv_text = new QPlainTextEdit();
	server_recv_text->setReadOnly(true);

	server_send_l = new QLabel(QString::fromLocal8Bit("发送消息"));
	server_send_text = new QPlainTextEdit();
	
	server_send_btn = new QPushButton(QString::fromLocal8Bit("发送"));
	server_send_btn->setEnabled(false);

	QGridLayout* window_layout = new QGridLayout(this);
	window_layout->addWidget(server_ip_l, 0, 0);
	window_layout->addWidget(server_ip_set, 0, 1);
	window_layout->addWidget(server_port_l, 1, 0);
	window_layout->addWidget(server_port_set, 1, 1);
	window_layout->addWidget(server_listen_btn, 2, 1);

	window_layout->addWidget(server_recv_l, 3, 0);
	window_layout->addWidget(server_recv_text, 4, 0, 2, 2);
	window_layout->addWidget(server_send_l, 6, 0);
	window_layout->addWidget(server_send_text, 7, 0, 2, 2);
	window_layout->addWidget(server_send_btn, 9, 1);


	connect(server_listen_btn, &QPushButton::clicked, this, &TcpServer::server_listen_slot);
	connect(server_, &QTcpServer::newConnection, this, &TcpServer::server_connect_success_slot);
	connect(server_send_btn, &QPushButton::clicked, this, &TcpServer::server_send_slot);
	
	timer = new QTimer(this);


}


TcpServer::~TcpServer() {
	server_socket->write(QByteArray("close connect"));
	/*server_socket->disconnectFromHost();
	server_socket->close();*/
	server_socket->deleteLater();
	destroy(true);

}

void TcpServer::server_listen_slot() {
	
	QHostAddress host_address(server_ip_set->text());
	quint16 port = server_port_set->text().toInt();
	//bool server_ret= server_socket->bind(host_address, port);
	//server_socket->connectToHost(host_address,port);
	server_->listen(host_address, port);
	

}

void TcpServer::server_send_slot() {

	QString send_text = server_send_text->toPlainText();
	QByteArray senddata;
	senddata.append(send_text);
	server_socket->write(senddata);

}

void TcpServer::server_connect_success_slot() {
	server_socket = server_->nextPendingConnection();//获取连接
	if (server_socket)
	{
		server_ip_set->setEnabled(false);
		server_port_set->setEnabled(false);
		server_listen_btn->setEnabled(false);
		server_send_btn->setEnabled(true);
	}
	server_socket->write(QByteArray("connect success"));
	connect(server_socket, &QTcpSocket::readyRead, this, &TcpServer::server_readdata_slot);
	connect(server_socket, &QTcpSocket::disconnected, this, [this]() {
		
			server_ip_set->setEnabled(true);
			server_port_set->setEnabled(true);
			server_listen_btn->setEnabled(true);
			server_send_btn->setEnabled(false);
		
		});


}


void TcpServer::server_readdata_slot() {

	QByteArray alldata = server_socket->readAll();
	QString recvdata=static_cast<QString>(alldata);
	server_recv_text->appendPlainText(recvdata);

}