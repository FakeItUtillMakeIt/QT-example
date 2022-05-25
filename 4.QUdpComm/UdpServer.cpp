#include "UdpServer.h"

UdpServer* UdpServer::instance = nullptr;


UdpServer::UdpServer() {
	this->setAttribute(Qt::WA_DeleteOnClose);

	window_layout = new QGridLayout(this);
	address_label = new QLabel(QString::fromLocal8Bit("服务器地址设置:"));
	port_label = new QLabel(QString::fromLocal8Bit("甲方端口设置:"));
	address_input = new QLineEdit(QString::fromLocal8Bit("127.0.0.1"));
	address_input->setEnabled(false);
	port_input = new QLineEdit(QString::fromLocal8Bit("45454"));
	connect_btn = new QPushButton(QString::fromLocal8Bit("设置"));
	send_btn = new QPushButton(QString::fromLocal8Bit("发送"));
	recv_label = new QLabel(QString::fromLocal8Bit("接收窗口"));
	send_label = new QLabel(QString::fromLocal8Bit("发送窗口"));
	recv_text = new QPlainTextEdit();
	recv_text->setReadOnly(true);
	send_text = new QPlainTextEdit();
	window_layout->addWidget(address_label, 0, 0);
	window_layout->addWidget(address_input, 0, 1);
	window_layout->addWidget(port_label, 1, 0);
	window_layout->addWidget(port_input, 1, 1);
	window_layout->addWidget(connect_btn, 2, 1);
	window_layout->addWidget(recv_label, 3, 0);
	window_layout->addWidget(recv_text, 4, 0, 2, 2);
	window_layout->addWidget(send_label, 6, 0);
	window_layout->addWidget(send_text, 7, 0, 2, 2);
	window_layout->addWidget(send_btn, 9, 1);

	connect(connect_btn, &QPushButton::clicked, this, &UdpServer::connect_slot);
	connect(send_btn, &QPushButton::clicked, this, &UdpServer::send_slot);

	udpsocket = new QUdpSocket(this);
	timer = new QTimer(this);

	connect(timer, &QTimer::timeout, this, &UdpServer::connect_slot);
	
}

UdpServer* UdpServer::get_instance() {
	if (instance==nullptr)
	{
		instance= new UdpServer();
	}
	return instance;
}

void UdpServer::recv_slot() {

	while (udpsocket->hasPendingDatagrams()) {
		QByteArray datagram;
		char data[1024];
		datagram.resize(udpsocket->pendingDatagramSize());
		
		udpsocket->readDatagram(datagram.data(), datagram.size());

		recv_text->appendPlainText(QString::fromLocal8Bit(datagram));
	}
	
}

void UdpServer::connect_slot() {

	
	udpsocket->bind(port_input->text().toUInt(),QUdpSocket::ShareAddress);
	//udpsocket->open(QIODevice::ReadWrite);
	//udpsocket->waitForConnected();
	//udpsocket->connectToHost(address, port_input->text().toUInt());
	connect(udpsocket, &QUdpSocket::readyRead, this, &UdpServer::recv_slot);
}

void UdpServer::send_slot() {
	QByteArray data;
	//data.setRawData(send_text->toPlainText().toStdString().c_str(), strlen(send_text->toPlainText().toStdString().c_str()));
	data.append(send_text->toPlainText());
	udpsocket->writeDatagram(data,data.size(), QHostAddress::Broadcast,port_input->text().toUInt()+1);

}