#include "DeviceParam.h"
#include "../DeviceManager/Database/DeviceDBConfigInfo.h"

DeviceParam::DeviceParam() :
	m_status(string("��ʼ��")), m_iCurValue(0), m_iCurStr("")
{
	curIndex = 0;
	paramData = { 0 };

	timer = new QTimer();
	connect(timer, &QTimer::timeout, this, &DeviceParam::timerUpdateParam);
}

DeviceParam::~DeviceParam()
{
}

/**
	@brief ���²���ʵʱֵ
**/
void DeviceParam::updateParamRealVal() {
	//���ݲ���ID��ȡ��������
	//��ʱ��������ɢ��
	DeviceDBConfigInfo::getInstance()->readParamDB2UI();
	if (atoi(DeviceDBConfigInfo::getInstance()->paramInfo[m_subParameterId][2].c_str()) == 2)
	{
		return;
	}
	//������
	//���ݵ�ǰ״̬����ʵʱֵ��Ϣ

	paramData = m_statusRealData[m_status];
	curIndex = 0;

}

/**
	@brief ��ʱ��ˢ��ʵʱֵ
**/
void DeviceParam::timerUpdateParam() {

	if (QString::fromStdString(m_status) == QString("��ʼ��"))
	{
		return;
	}
	m_iCurValue = paramData[curIndex++];
	if (curIndex >= paramData.size())
	{
		curIndex = 0;

	}
	qDebug() << curIndex << m_iCurValue << timer->timerId() << m_subParameterId;
}