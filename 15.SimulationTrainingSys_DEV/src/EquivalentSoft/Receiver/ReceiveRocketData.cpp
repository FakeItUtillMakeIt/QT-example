#include "ReceiveRocketData.h"
#include <QDateTime>
#include "../../Public/CRC.h"
#include "../../ConfigInterface/configglobal.h"
#define FRAMELENGTH 10
ReceiveRocketData::ReceiveRocketData(QObject* parent)
	: QObject(parent),
	m_pSocket(nullptr),
	m_isRun(true)
{
    m_app = AppCache::instance();
    init();
}

ReceiveRocketData::~ReceiveRocketData()
{ 
}

void ReceiveRocketData::init()
{
    m_pSocket = new QUdpSocket(this);
    m_pSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
    //本地监听端口号为随意指定
    if (!m_pSocket->bind(QHostAddress::AnyIPv4, m_app->m_yaoCeReceiver->m_iPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        return;
    }
    m_pSocket->joinMulticastGroup(QHostAddress(m_app->m_yaoCeReceiver->m_strIP.c_str())); //加入组播
    connect(m_pSocket, &QUdpSocket::readyRead, this, &ReceiveRocketData::receiveData); 
}


void ReceiveRocketData::receiveData()
{
    unsigned char data_buff[30];
    int lastSize = 0;
    int iLength;
    unsigned char vdata1[FRAMELENGTH];
    unsigned short crc_val;
    unsigned short frameLength;
    while (m_pSocket->hasPendingDatagrams())
    {
        QHostAddress peer_addr;
        quint16 peer_port;
        QByteArray datagram;
        datagram.resize(m_pSocket->pendingDatagramSize());
        m_pSocket->readDatagram(datagram.data(), datagram.size(), &peer_addr, &peer_port);
        if (0 == datagram.size()) continue;
        iLength = datagram.size(); 
        if ((unsigned char)datagram.data()[0] != FRAMEHEAD_SYNC_1 || (unsigned char)datagram.data()[1] != FRAMEHEAD_SYNC_2)
        {
            continue;
        }
        memcpy(&frameLength, (unsigned char*)datagram.data() + FRAMEHEAD_ADDR_LENGTH, 2);
        if (frameLength > datagram.size()) break;
        if (!m_app->m_CurrentRocketDataFrame->DeSerialize((unsigned char*)datagram.data(), frameLength))
        { 
            break;
        }
        ConfigNameSpace::ConfigGlobal::dataupdated = true; 
    }
} 