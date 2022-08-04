#include "CenterOperate.h" 
CenterOperate::CenterOperate(QWidget* parent)
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
	/*Command* command = oneCommand.value<Command*>();
	QMessageBox::warning(qApp->activeWindow(), QObject::tr("����"),
		QString("C3I�յ��ⷢ����ָ��,�ⷢ����code:%1�����Ͳָⷢ��code:%2").arg(command->m_iCode));*/

	QByteArray frameData = oneCommand.value<QByteArray>();
	QByteArray m_pBuff(10, Qt::Uninitialized);
	for (int i = 0; i < 10; i++)
	{
		m_pBuff[i] = frameData[i];
	}

	int m_iCode = m_pBuff[2];
	int exeStat = m_pBuff[3];
	int sendICode = m_pBuff[4];

	QMessageBox::information(qApp->activeWindow(), "", QString("C3I�յ��ⷢ����ⷢ����ID:%1��ָ��ͷ�code:%2,ִ�����:%3;").arg(m_iCode).arg(sendICode).arg(exeStat == 1 ? "�ɹ�" : "ʧ��"));
	emit curRunCmd(m_iCode, sendICode);
}
