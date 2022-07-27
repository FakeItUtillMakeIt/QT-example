#include "CenterOperate.h" 
CenterOperate::CenterOperate(QWidget *parent)
    : QWidget(parent)
    , m_pRocketStructShow(nullptr)
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
    m_pRocketStructShow = new RocketStructShow(this);
    m_pRocketStructShow->setGeometry(0, 0, 1920, 795);
}
 
void CenterOperate::receiverCMD(QVariant oneCommand)
{
    Command* command = oneCommand.value<Command*>();
    QMessageBox::warning(qApp->activeWindow(), QObject::tr("����"),
        QString("C3I�յ��ⷢ����ָ��,code����%1��ִ��%2��").arg(command->m_iCode).arg(command->m_iBackId == 1 ? "�ɹ�" : "ʧ��"));
}
