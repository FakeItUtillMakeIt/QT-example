#include "ReceiveFaultData.h"
#include <QDateTime>
#include "../../Public/CRC.h"
#define FRAMELENGTH 10
ReceiveFaultData::ReceiveFaultData(QObject* parent)
	: QObject(parent),
	m_pCMDSocket(nullptr),
	m_isRun(true)
{
    m_app = AppCache::instance();
    init();
}

ReceiveFaultData::~ReceiveFaultData()
{ 
}

void ReceiveFaultData::init()
{
    m_pCMDSocket = new QUdpSocket(this);
    m_pCMDSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
    //本地监听端口号为随意指定
    if (!m_pCMDSocket->bind(QHostAddress::AnyIPv4, m_app->m_faultReceiver->m_iPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        return;
    }
    m_pCMDSocket->joinMulticastGroup(QHostAddress(m_app->m_faultReceiver->m_strIP.c_str())); //加入组播
    connect(m_pCMDSocket, &QUdpSocket::readyRead, this, &ReceiveFaultData::receiveData); 
}


void ReceiveFaultData::receiveData()
{
    unsigned char data_buff[30];
    int lastSize = 0;
    int iLength;
    unsigned char vdata1[FRAMELENGTH];
    unsigned short crc_val;
    while (m_pCMDSocket->hasPendingDatagrams())
    {
        QHostAddress peer_addr;
        quint16 peer_port;
        QByteArray datagram;
        datagram.resize(m_pCMDSocket->pendingDatagramSize());
        m_pCMDSocket->readDatagram(datagram.data(), datagram.size(), &peer_addr, &peer_port);
        if (0 == datagram.size()) continue;
        iLength = datagram.size(); 
        if ((unsigned char)datagram.data()[0] != 0x66 || (unsigned char)datagram.data()[1] != 0xAA)
        {
            continue;
        }

        for (int i = 0; i < FRAMELENGTH; i++)
        {
            vdata1[i] = (unsigned char)datagram[i];
        }
        unsigned short checksum = CRC::CalCRC16(vdata1 + 2, FRAMELENGTH - 4);
        memcpy(&crc_val, datagram.data() + FRAMELENGTH - 2, 2);
        if (crc_val != checksum)
        {
            continue;
        } 
        Command* command = new Command();
        command->m_iType = 3;
        command->m_iCode = 0;
        command->m_iBackId = 0;
        memcpy(&command->m_iCode, datagram.data() + 2 , 2); //测发指令code  
        for (auto it = m_app->m_allCommad.begin(); it != m_app->m_allCommad.end(); it++)
        {
            if (it->second->m_iCode == command->m_iCode)
            {
                command->m_iBackId = it->second->m_iBackId;
                command->m_id = it->second->m_id;
                break;
            }
        } 
        QVariant temp;
        temp.setValue(command);
        emit receiverFault(temp);
    }
} 