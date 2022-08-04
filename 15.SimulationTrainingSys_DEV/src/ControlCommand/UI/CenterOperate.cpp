#include "CenterOperate.h" 
#define FRAMELENGTH 10
CenterOperate::CenterOperate(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(parent);   
    m_app = AppCache::instance();
    m_pSocket = new QUdpSocket(this);
    Init();
} 
CenterOperate::~CenterOperate()
{

}
void CenterOperate::Init()
{ 
    m_pReceiveCMDData = new ReceiveCMDData();
    connect(m_pReceiveCMDData, &ReceiveCMDData::receiverCMD, this, &CenterOperate::receiverCMD);

    connect(ui.pushButton, &QPushButton::clicked, this, &CenterOperate::sendCMD);
}

void CenterOperate::receiverCMD(QVariant oneCommand)
{
    Command* command = oneCommand.value<Command*>(); 
    QMessageBox::warning(qApp->activeWindow(), QObject::tr("警告"),
        QString("测发控软件收到测发回令指令,code等于%1，执行%2！").arg(command->m_iCode).arg(command->m_iBackId == 1 ? "成功" : "失败"));
}

/// <summary>
/// 测控指令发送
/// </summary>
/// <param name="cmd_id"></param>
void CenterOperate::sendCMD(int cmd_id)
{  
    cmd_id = 1;
    if (m_app->m_allCommad.find(cmd_id) == m_app->m_allCommad.end())
    {
        return;
    }
    Command* command = m_app->m_allCommad[cmd_id]; 
    QByteArray m_pBuff(FRAMELENGTH, Qt::Uninitialized);
    m_pBuff[0] = 0x55;
    m_pBuff[1] = 0xAA; 
    m_pBuff[2] = command->m_iCode; //测发指令code
    m_pBuff[3] = 0x00;
    m_pBuff[4] = 0x00;
    m_pBuff[5] = 0x00;
    m_pBuff[6] = 0x00;
    m_pBuff[7] = 0x00;
    m_pBuff[8] = 0x00;
    m_pBuff[9] = 0x00;
    unsigned char vdata1[FRAMELENGTH];
    for (int i = 0; i < FRAMELENGTH; i++)
    {
        vdata1[i] = (unsigned char)m_pBuff[i];
    }
    unsigned short checksum = CRC::CalCRC16(vdata1 + 2, FRAMELENGTH - 4);
    memcpy(m_pBuff.data() + FRAMELENGTH - 2, &checksum, 2);
    m_pSocket->writeDatagram(m_pBuff, QHostAddress(m_app->m_cmdSender->m_strIP.c_str()), m_app->m_cmdSender->m_iPort);
}